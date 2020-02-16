#include "PsslShaderModule.h"

#include "PsslContants.h"
#include "GCNAnalyzer.h"
#include "GCNCompiler.h"

#include "Platform/UtilFile.h"
#include "../Gve/GveShader.h"

LOG_CHANNEL(Graphic.Pssl.PsslShaderModule);

namespace pssl
{;

const uint32_t kMaxUserDataCount = 16;

const uint8_t PsslShaderModule::m_shaderResourceSizeInDwords[kShaderInputUsageImmDispatchDrawInstances + 1] = 
{
	8,  // kShaderInputUsageImmResource
	4,  // kShaderInputUsageImmSampler
	4,  // kShaderInputUsageImmConstBuffer
	4,  // kShaderInputUsageImmVertexBuffer
	8,  // kShaderInputUsageImmRwResource
	1,  // kShaderInputUsageImmAluFloatConst
	1,  // kShaderInputUsageImmAluBool32Const
	1,  // kShaderInputUsageImmGdsCounterRange
	1,  // kShaderInputUsageImmGdsMemoryRange
	1,  // kShaderInputUsageImmGwsBase
	2,  // kShaderInputUsageImmShaderResourceTable
	0,  //
	0,  //
	1,  // kShaderInputUsageImmLdsEsGsSize
	0,  //
	0,  //
	0,  //
	0,  //
	2,  // kShaderInputUsageSubPtrFetchShader
	2,  // kShaderInputUsagePtrResourceTable
	2,  // kShaderInputUsagePtrInternalResourceTable
	2,  // kShaderInputUsagePtrSamplerTable
	2,  // kShaderInputUsagePtrConstBufferTable
	2,  // kShaderInputUsagePtrVertexBufferTable
	2,  // kShaderInputUsagePtrSoBufferTable
	2,  // kShaderInputUsagePtrRwResourceTable
	2,  // kShaderInputUsagePtrInternalGlobalTable
	2,  // kShaderInputUsagePtrExtendedUserData
	2,  // kShaderInputUsagePtrIndirectResourceTable
	2,  // kShaderInputUsagePtrIndirectInternalResourceTable
	2,  // kShaderInputUsagePtrIndirectRwResourceTable
	0,  //
	0,  //
	0,  //
	1,  // kShaderInputUsageImmGdsKickRingBufferOffset
	1,  // kShaderInputUsageImmVertexRingBufferOffset
	2,  // kShaderInputUsagePtrDispatchDraw
	1,  // kShaderInputUsageImmDispatchDrawInstances
};

PsslShaderModule::PsslShaderModule(const uint32_t* code) :
	m_code(code),
	m_progInfo((const uint8_t*)code)
{
#ifdef PSSL_DUMP_SHADER
	dumpShader(m_progInfo.shaderType(), (const uint8_t*)code, m_progInfo.codeSizeBytes());
#endif  // GPCS4_DUMP_SHADER
}

PsslShaderModule::~PsslShaderModule()
{
}

RcPtr<gve::GveShader> PsslShaderModule::compile()
{
	const uint32_t* codeEnd = m_code + m_progInfo.codeSizeDwords();
	GCNCodeSlice codeSlice(m_code, codeEnd);

	// Analyze shader global information
	GcnAnalysisInfo analysisInfo;
	GCNAnalyzer analyzer(analysisInfo);
	runAnalyzer(analyzer, codeSlice);

	// Generate input
	GcnShaderInput shaderInput;
	shaderInput.shaderResources = getShaderResources();
	if (!m_vsInputSemantic.empty())
	{
		shaderInput.vsInputSemantics = m_vsInputSemantic;
	}

	// Recompile
	GCNCompiler compiler(m_progInfo, analysisInfo, shaderInput);
	runCompiler(compiler, codeSlice);

	return compiler.finalize();
}

std::vector<GcnShaderResourceInstance> 
PsslShaderModule::linearlizeShaderResources(const GcnShaderResources& nestedResources)
{
	std::vector<GcnShaderResourceInstance> linearResourceTable;

	linearResourceTable.insert(linearResourceTable.begin(),
		nestedResources.ud.begin(), nestedResources.ud.end());

	if (nestedResources.eud.has_value())
	{
		for (const auto& eudRes : nestedResources.eud->resources)
		{
			linearResourceTable.emplace_back(eudRes.second);
		}
	}

	// TODO:
	// SRT support

	return linearResourceTable;
}

void PsslShaderModule::parseFetchShader(const uint32_t* fsCode)
{
	PsslFetchShader fsShader(fsCode);

	const uint32_t* fsCodeEnd = fsCode + fsShader.m_codeLengthDw;
	GCNCodeSlice fsCodeSlice(fsCode, fsCodeEnd);

	decodeFetchShader(fsCodeSlice, fsShader);
	extractInputSemantic(fsShader);

#ifdef PSSL_DUMP_SHADER
	dumpShader(FetchShader, (const uint8_t*)fsCode, fsShader.m_codeLengthDw * sizeof(uint32_t));
#endif  // GPCS4_DUMP_SHADER
}

void PsslShaderModule::decodeFetchShader(GCNCodeSlice slice, PsslFetchShader& fsShader)
{
	GCNDecodeContext decoder;

	while (!slice.atEnd())
	{
		decoder.decodeInstruction(slice);

		GCNInstruction& inst = decoder.getInstruction();
		// store the decoded instructions for use later in compile.
		fsShader.m_instructionList.push_back(std::move(inst));
	}
}

void PsslShaderModule::extractInputSemantic(PsslFetchShader& fsShader)
{
	do
	{
		//s_load_dwordx4 s[8:11], s[2:3], 0x00                      // 00000000: C0840300
		//s_load_dwordx4 s[12:15], s[2:3], 0x04                     // 00000004: C0860304
		//s_load_dwordx4 s[16:19], s[2:3], 0x08                     // 00000008: C0880308
		//s_waitcnt     lgkmcnt(0)                                  // 0000000C: BF8C007F
		//buffer_load_format_xyzw v[4:7], v0, s[8:11], 0 idxen      // 00000010: E00C2000 80020400
		//buffer_load_format_xyz v[8:10], v0, s[12:15], 0 idxen     // 00000018: E0082000 80030800
		//buffer_load_format_xy v[12:13], v0, s[16:19], 0 idxen     // 00000020: E0042000 80040C00
		//s_waitcnt     0                                           // 00000028: BF8C0000
		//s_setpc_b64   s[0:1]                                      // 0000002C: BE802000

		// A normal fetch shader looks like the above, the instructions are generated
		// using input semantics on cpu side.
		// We take the reverse way, extract the original input semantics from these instructions.

		uint32_t semanIdx = 0;
		for (auto& ins : fsShader.m_instructionList)
		{
			if (ins.instruction->GetInstructionClass() != Instruction::VectorMemBufFmt)
			{
				// We only care about the buffer_load_format_xxx instructions
				continue;
			}

			SIMUBUFInstruction* vecLoadIns = dynamic_cast<SIMUBUFInstruction*>(ins.instruction.get());

			VertexInputSemantic semantic = { 0 };
			semantic.semantic            = semanIdx;
			semantic.vgpr                = vecLoadIns->GetVDATA();
			semantic.sizeInElements      = (uint32_t)vecLoadIns->GetOp() + 1;
			semantic.reserved            = 0;

			m_vsInputSemantic.push_back(semantic);

			++semanIdx;
		}
	} while (false);
}

const void* PsslShaderModule::findShaderResourceInUserData(uint32_t startRegister)
{
	const void* resPtr = nullptr;
	do
	{
		for (const auto& res : m_shaderInputTable)
		{
			if (res.startRegister == startRegister)
			{
				resPtr = res.resource;
				break;
			}
		}
	} while (false);
	return resPtr;
}

const void* PsslShaderModule::findShaderResourceInEUD(uint32_t eudOffsetInDword)
{
	if (!m_eudTable)
	{
		m_eudTable = findShaderResourceByType(kShaderInputUsagePtrExtendedUserData);
	}

	const void* resPtr = m_eudTable + eudOffsetInDword;

	return resPtr;
}

bool PsslShaderModule::parseShaderInput()
{
	bool ret = false;
	do 
	{
		if (m_shaderInputTable.empty())
		{
			// Some shaders has no input resource.
			ret = true;
			break;
		}

		parseResImm();
		parseResEud();
		parseResPtrTable();

		if (!checkUnhandledRes())
		{
			LOG_ASSERT(false, "Unhandled resource type found");
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

void PsslShaderModule::parseResImm()
{
	const auto& inputUsageSlots = m_progInfo.inputUsageSlot();
	for (auto& slot : inputUsageSlots)
	{
		uint8_t usageType      = slot.usageType;
		uint32_t startRegister = slot.startRegister;

		// immediate resources, must be within 16 user data regs.
		switch (usageType)
		{
		case kShaderInputUsageImmGdsCounterRange:
		case kShaderInputUsageImmGdsMemoryRange:
		case kShaderInputUsageImmLdsEsGsSize:
		case kShaderInputUsagePtrInternalGlobalTable:
		case kShaderInputUsageImmGdsKickRingBufferOffset:
		case kShaderInputUsageImmVertexRingBufferOffset:
		case kShaderInputUsagePtrDispatchDraw:
		case kShaderInputUsageImmDispatchDrawInstances:
		{
			GcnShaderResourceInstance res = {};
			res.usageType          = static_cast<ShaderInputUsageType>(usageType);
			res.res.startRegister  = startRegister;
			res.res.sizeDwords     = m_shaderResourceSizeInDwords[usageType];
			res.res.resource       = findShaderResourceInUserData(slot.startRegister);
			LOG_ASSERT(res.res.resource != nullptr, "can not found imm type resource %d", usageType);

			m_shaderResources.ud.push_back(res);
		}
			break;
		case kShaderInputUsageImmShaderResourceTable:
			LOG_ASSERT(false, "SRT is not supported yet.");
			break;
		default:
			break;
		}
	}
}

void PsslShaderModule::parseResEud()
{
	const auto& inputUsageSlots = m_progInfo.inputUsageSlot();

	// Detect if there's an EUD.
	auto iter = std::find_if(inputUsageSlots.begin(), inputUsageSlots.end(), 
	[](const auto& slot)
	{
		return slot.usageType == kShaderInputUsagePtrExtendedUserData;
	});

	// If there is, initialize it.
	if (iter != inputUsageSlots.end())
	{
		m_shaderResources.eud                = std::make_optional<GcnShaderResourceEUD>();
		m_shaderResources.eud->startRegister = iter->startRegister;
	}

	for (auto& slot : inputUsageSlots)
	{
		uint8_t usageType      = slot.usageType;
		uint32_t startRegister = slot.startRegister;

		bool isInUserData         = startRegister < kMaxUserDataCount;
		uint16_t eudOffsetInDword = (startRegister >= kMaxUserDataCount) ? (startRegister - kMaxUserDataCount) : 0;

		// below resources can either be inside user data registers or the EUD
		switch (usageType)
		{
		case kShaderInputUsageImmResource:
		case kShaderInputUsageImmRwResource:
		case kShaderInputUsageImmSampler:
		case kShaderInputUsageImmConstBuffer:
		case kShaderInputUsageImmVertexBuffer:
		{
			GcnShaderResourceInstance res  = {};
			res.usageType          = static_cast<ShaderInputUsageType>(usageType);
			res.res.startRegister  = startRegister;
			res.res.sizeDwords     = m_shaderResourceSizeInDwords[usageType];
			res.res.resource       = isInUserData ? 
				findShaderResourceInUserData(startRegister) : 
				findShaderResourceInEUD(eudOffsetInDword);
			LOG_ASSERT(res.res.resource != nullptr, "can not found imm type resource %d", usageType);

			isInUserData ? 
			m_shaderResources.ud.push_back(res) : 
			m_shaderResources.eud->resources.push_back(std::make_pair(eudOffsetInDword, res));
		}
			break;
		default:
			break;
		}
	}
}

void PsslShaderModule::parseResPtrTable()
{
	const auto& inputUsageSlots = m_progInfo.inputUsageSlot();
	for (auto& slot : inputUsageSlots)
	{
		uint8_t usageType      = slot.usageType;
		uint32_t startRegister = slot.startRegister;

		// resource in tables
		switch (usageType)
		{
		case kShaderInputUsagePtrResourceTable:
		case kShaderInputUsagePtrRwResourceTable:
		case kShaderInputUsagePtrConstBufferTable:
		case kShaderInputUsagePtrSamplerTable:
			LOG_ASSERT(false, "not supported yet.");
			break;
		case kShaderInputUsagePtrVertexBufferTable:
		{
			const uint32_t* vertexBufferTable = reinterpret_cast<const uint32_t*>(findShaderResourceInUserData(startRegister));
			LOG_ASSERT(vertexBufferTable != nullptr, "can not find vertex buffer table.");

			// TODO:
			// Support Es Ls and Cs.
			for (auto& sematic : m_vsInputSemantic)
			{
				uint32_t semanticIndex = sematic.semantic;

				GcnShaderResourceInstance res = {};
				// Convert to imm type.
				res.usageType         = kShaderInputUsageImmVertexBuffer;
				// startRegister is not meaningful for vertex buffer inputs, just give it a random value.
				res.res.startRegister = semanticIndex;
				res.res.resource      = &vertexBufferTable[semanticIndex * kDwordSizeVertexBuffer];
				res.res.sizeDwords    = kDwordSizeVertexBuffer;

				m_shaderResources.ud.push_back(res);
			}
		}
			break;
		default:
			break;
		}
	}
}

bool PsslShaderModule::checkUnhandledRes()
{
	bool allHandled             = true;

	const auto& inputUsageSlots = m_progInfo.inputUsageSlot();
	for (auto& slot : inputUsageSlots)
	{
		switch (slot.usageType)
		{
		case kShaderInputUsageImmResource:
		case kShaderInputUsageImmRwResource:
		case kShaderInputUsageImmSampler:
		case kShaderInputUsageImmConstBuffer:
		case kShaderInputUsageImmVertexBuffer:
		case kShaderInputUsageImmShaderResourceTable:
		case kShaderInputUsageSubPtrFetchShader:
		case kShaderInputUsagePtrExtendedUserData:
		case kShaderInputUsagePtrResourceTable:
		case kShaderInputUsagePtrRwResourceTable:
		case kShaderInputUsagePtrConstBufferTable:
		case kShaderInputUsagePtrVertexBufferTable:
		case kShaderInputUsagePtrSamplerTable:
		case kShaderInputUsagePtrInternalGlobalTable:
		case kShaderInputUsageImmGdsCounterRange:
		case kShaderInputUsageImmGdsMemoryRange:
		case kShaderInputUsageImmLdsEsGsSize:
		case kShaderInputUsageImmGdsKickRingBufferOffset:
		case kShaderInputUsageImmVertexRingBufferOffset:
		case kShaderInputUsagePtrDispatchDraw:
		case kShaderInputUsageImmDispatchDrawInstances:
		// case Gnm::kShaderInputUsagePtrSoBufferTable:
			break;
		default:
			// Not handled
		{
			allHandled = false;
			LOG_ASSERT(allHandled, "Input Usage Slot type %d not handled.", slot.usageType);
		}
			break;
		}
	}
	
	return allHandled;
}

const uint32_t* PsslShaderModule::findShaderResourceByType(ShaderInputUsageType usageType)
{
	const uint32_t* resPtr    = nullptr;

	do 
	{
		uint32_t startRegister      = 0;
		const auto& inputUsageSlots = m_progInfo.inputUsageSlot();
		for (auto& slot : inputUsageSlots)
		{
			if (slot.usageType != usageType)
			{
				continue;
			}

			startRegister = slot.startRegister;
			break;
		}

		LOG_ASSERT(startRegister != 0, "can not find resource declaration in shader slots.");

		resPtr = reinterpret_cast<const uint32_t*>(findShaderResourceInUserData(startRegister));

		LOG_ASSERT(resPtr != nullptr, "can not find resource in input user data from game.");

	} while (false);

	return resPtr;
}

void PsslShaderModule::dumpShader(PsslProgramType type, const uint8_t* code, uint32_t size)
{
	char filename[64]  = { 0 };
	const char* format = nullptr;

	switch (type)
	{
	case pssl::PixelShader:
		format = "%016llX.ps.bin";
		break;
	case pssl::VertexShader:
		format = "%016llX.vs.bin";
		break;
	case pssl::GeometryShader:
		format = "%016llX.gs.bin";
		break;
	case pssl::HullShader:
		format = "%016llX.hs.bin";
		break;
	case pssl::DomainShader:
		format = "%016llX.ds.bin";
		break;
	case pssl::ComputeShader:
		format = "%016llX.cs.bin";
		break;
	case pssl::FetchShader:
		format = "%016llX.fs.bin";
		break;
	default:
		break;
	}

	sprintf_s(filename, 64, format, m_progInfo.key().toUint64());
	UtilFile::StoreFile(filename, code, size);
}

void PsslShaderModule::runAnalyzer(GCNAnalyzer& analyzer, GCNCodeSlice slice)
{
	GCNDecodeContext decoder;

	while (!slice.atEnd())
	{
		decoder.decodeInstruction(slice);

		analyzer.processInstruction(decoder.getInstruction());
	}
}

void PsslShaderModule::runCompiler(GCNCompiler& compiler, GCNCodeSlice slice)
{
	GCNDecodeContext decoder;

	while (!slice.atEnd())
	{
		decoder.decodeInstruction(slice);

		compiler.processInstruction(decoder.getInstruction());
	}
}

std::vector<InputUsageSlot> PsslShaderModule::inputUsageSlots()
{
	return m_progInfo.inputUsageSlot();
}

void PsslShaderModule::defineFetchShader(const uint32_t* fsCode)
{
	parseFetchShader(fsCode);
}

void PsslShaderModule::defineShaderInput(const std::vector<PsslShaderResource>& shaderInputTab)
{
	// store the table, delay parse it when really needed.
	m_shaderInputTable.assign(shaderInputTab.cbegin(), shaderInputTab.cend());
}

const GcnShaderResources& PsslShaderModule::getShaderResources()
{
	do
	{
		if (!m_shaderResources.ud.empty())
		{
			// already parsed
			break;
		}

		if (!parseShaderInput())
		{
			LOG_ASSERT(false, "parse shader input table failed.");
		}

	} while (false);
	return m_shaderResources;
}

PsslKey PsslShaderModule::key()
{
	return m_progInfo.key();
}

std::vector<VertexInputSemantic> PsslShaderModule::vsInputSemantic()
{
	return m_vsInputSemantic;
}

}  // namespace pssl