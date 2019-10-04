#include "PsslShaderModule.h"
#include "Platform/UtilFile.h"

#ifdef GPCS4_DEBUG
// Dump shader to file
#define PSSL_DUMP_SHADER
#endif





namespace pssl
{;

PsslShaderModule::PsslShaderModule(const uint32_t* code, const std::vector<PsslShaderResource>& shaderResTab):
	m_code(code),
	m_progInfo((const uint8_t*)code),
	m_shaderResourceTable(shaderResTab)
{
#ifdef GPCS4_DUMP_SHADER
	dumpShader(m_progInfo.shaderType(), (const uint8_t*)code, m_progInfo.codeSizeBytes());
#endif  // GPCS4_DUMP_SHADER
}

PsslShaderModule::PsslShaderModule(const uint32_t* code, const uint32_t* fsCode,
	const std::vector<PsslShaderResource>& shaderResTab):
	PsslShaderModule(code, shaderResTab)
{
	parseFetchShader(fsCode);
}

PsslShaderModule::~PsslShaderModule()
{

}


RcPtr<gve::GveShader> PsslShaderModule::compile()
{
	return m_vsInputSemantic.size() ? compileWithFS() : compileNoFS();
}

RcPtr<gve::GveShader> PsslShaderModule::compileWithFS()
{
	const uint32_t* codeEnd = m_code + m_progInfo.codeSizeDwords();
	GCNCodeSlice codeSlice(m_code, codeEnd);

	GcnAnalysisInfo analysisInfo;
	GCNAnalyzer analyzer(analysisInfo);
	this->runAnalyzer(analyzer, codeSlice);

	const auto resBuffers = findResourceBuffers();
	GCNCompiler compiler(m_progInfo, analysisInfo, m_vsInputSemantic, resBuffers);
	runCompiler(compiler, codeSlice);
	return compiler.finalize();
}

RcPtr<gve::GveShader> PsslShaderModule::compileNoFS()
{
	const uint32_t* codeEnd = m_code + m_progInfo.codeSizeDwords();
	GCNCodeSlice codeSlice(m_code, codeEnd);

	GcnAnalysisInfo analysisInfo;
	GCNAnalyzer analyzer(analysisInfo);
	this->runAnalyzer(analyzer, codeSlice);

	const auto resBuffers = findResourceBuffers();
	GCNCompiler compiler(m_progInfo, analysisInfo, resBuffers);
	runCompiler(compiler, codeSlice);
	return compiler.finalize();
}

void PsslShaderModule::parseFetchShader(const uint32_t* fsCode)
{
	PsslFetchShader fsShader(fsCode);

	const uint32_t* fsCodeEnd = fsCode + fsShader.m_codeLengthDw;
	GCNCodeSlice fsCodeSlice(fsCode, fsCodeEnd);

	decodeFetchShader(fsCodeSlice, fsShader);
	extractInputSemantic(fsShader);

#ifdef GPCS4_DUMP_SHADER
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
			semantic.semantic = semanIdx;
			semantic.vgpr = vecLoadIns->GetVDATA();
			semantic.sizeInElements = (uint32_t)vecLoadIns->GetOp() + 1;
			semantic.reserved = 0;

			m_vsInputSemantic.push_back(semantic);

			++semanIdx;
		}
	} while (false);
}

std::vector<pssl::GcnResourceBuffer> PsslShaderModule::findResourceBuffers()
{
	std::vector<pssl::GcnResourceBuffer> resultRes;
	do
	{
		uint32_t usageSlotCount = m_progInfo.inputUsageSlotCount();
		for (uint32_t i = 0; i != usageSlotCount; ++i)
		{
			const InputUsageSlot* usageSlot = m_progInfo.inputUsageSlot(i);
			ShaderInputUsageType usageType = static_cast<ShaderInputUsageType>(usageSlot->usageType);

			switch (usageType)
			{
			case kShaderInputUsageImmSampler:
				break;
			case kShaderInputUsageImmConstBuffer:
			{
				PsslShaderResource res;
				bool found = findShaderResource(usageSlot->startRegister, res);
				LOG_ASSERT(found == true, "can not find matched shader resource.");
				SpirvResourceType type = usageSlot->resourceType == 0 ? 
					SpirvResourceType::VSharp : SpirvResourceType::TSharp;
				resultRes.emplace_back(type, res);
			}
				break;
			default:
				break;
			}
		}
	} while (false);
	return resultRes;
}

bool PsslShaderModule::findShaderResource(uint32_t startSlot, PsslShaderResource& outRes)
{
	bool found = false;
	do 
	{
		for (const auto& res : m_shaderResourceTable)
		{
			if (res.startSlot == startSlot)
			{
				outRes = res;
				found = true;
				break;
			}
		}
	} while (false);
	return found;
}

void PsslShaderModule::dumpShader(PsslProgramType type, const uint8_t* code, uint32_t size)
{
	char filename[64] = { 0 };
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

pssl::PsslKey PsslShaderModule::key()
{
	return m_progInfo.key();
}

std::vector<VertexInputSemantic> PsslShaderModule::vsInputSemantic()
{
	return m_vsInputSemantic;
}

}  // namespace pssl