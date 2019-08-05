#include "GCNCompiler.h"

#include <array>

namespace pssl
{;

constexpr uint32_t PerVertex_Position = 0;
constexpr uint32_t PerVertex_CullDist = 1;
constexpr uint32_t PerVertex_ClipDist = 2;

GCNCompiler::GCNCompiler(const PsslProgramInfo& progInfo,
	std::optional<PsslFetchShader> fsShader):
	m_fsShader(std::move(fsShader)),
	m_programInfo(progInfo)
{
	// Declare an entry point ID. We'll need it during the
	// initialization phase where the execution mode is set.
	m_entryPointId = m_module.allocateId();

	// Set the shader name so that we recognize it in renderdoc
	m_module.setDebugSource(
		spv::SourceLanguageUnknown, 0,
		m_module.addDebugString(progInfo.getKey().toString().c_str()),
		nullptr);

	//// Set the memory model. This is the same for all shaders.
	m_module.setMemoryModel(
		spv::AddressingModelLogical,
		spv::MemoryModelGLSL450);

	// Make sure our interface registers are clear
	//for (uint32_t i = 0; i < DxbcMaxInterfaceRegs; i++) {
	//	m_vRegs.at(i) = DxbcRegisterPointer{ };
	//	m_oRegs.at(i) = DxbcRegisterPointer{ };
	//}

	this->emitInit();
}

GCNCompiler::~GCNCompiler()
{
}

void GCNCompiler::emitInit()
{
	// Set up common capabilities for all shaders
	m_module.enableCapability(spv::CapabilityShader);
	m_module.enableCapability(spv::CapabilityImageQuery);

	// Initialize the shader module with capabilities
	// etc. Each shader type has its own peculiarities.
	switch (m_programInfo.getShaderType())
	{
	case VertexShader:   emitVsInit(); break;
	case HullShader:     emitHsInit(); break;
	case DomainShader:   emitDsInit(); break;
	case GeometryShader: emitGsInit(); break;
	case PixelShader:    emitPsInit(); break;
	case ComputeShader:  emitCsInit(); break;
	}
}

void GCNCompiler::emitVsInit()
{
	m_module.enableCapability(spv::CapabilityClipDistance);
	m_module.enableCapability(spv::CapabilityCullDistance);
	m_module.enableCapability(spv::CapabilityDrawParameters);

	m_module.enableExtension("SPV_KHR_shader_draw_parameters");

	std::vector<VertexInputSemantic> inputSemantics = parseFetchShader();

	// Declare the per-vertex output block. This is where
	// the vertex shader will write the vertex position.
	const uint32_t perVertexStruct = this->getPerVertexBlockId();
	const uint32_t perVertexPointer = m_module.defPointerType(
		perVertexStruct, spv::StorageClassOutput);

	m_perVertexOut = m_module.newVar(
		perVertexPointer, spv::StorageClassOutput);
	m_entryPointInterfaces.push_back(m_perVertexOut);
	m_module.setDebugName(m_perVertexOut, "vs_vertex_out");

}

void GCNCompiler::emitHsInit()
{

}

void GCNCompiler::emitDsInit()
{

}

void GCNCompiler::emitGsInit()
{

}

void GCNCompiler::emitPsInit()
{

}

void GCNCompiler::emitCsInit()
{

}

void GCNCompiler::emitDclInputArray(uint32_t vertexCount)
{
	//PsslArrayType info;
	//info.ctype = PsslScalarType::Float32;
	//info.ccount = 4;
	//info.alength = m_isgn != nullptr ? m_isgn->maxRegisterCount() : 0;

	//if (info.alength == 0)
	//	return;

	//// Define the array type. This will be two-dimensional
	//// in some shaders, with the outer index representing
	//// the vertex ID within an invocation.
	//uint32_t arrayTypeId = getArrayTypeId(info);

	//if (vertexCount != 0) {
	//	arrayTypeId = m_module.defArrayType(
	//		arrayTypeId, m_module.constu32(vertexCount));
	//}

	//// Define the actual variable. Note that this is private
	//// because we will copy input registers and some system
	//// variables to the array during the setup phase.
	//const uint32_t ptrTypeId = m_module.defPointerType(
	//	arrayTypeId, spv::StorageClassPrivate);

	//const uint32_t varId = m_module.newVar(
	//	ptrTypeId, spv::StorageClassPrivate);

	//m_module.setDebugName(varId, "shader_in");
	//m_vArray = varId;
}

void GCNCompiler::emitDclInputPerVertex(uint32_t vertexCount, const char* varName)
{

}

uint32_t GCNCompiler::emitDclClipCullDistanceArray(uint32_t length, spv::BuiltIn builtIn, spv::StorageClass storageClass)
{

}

uint32_t GCNCompiler::getPerVertexBlockId()
{
	uint32_t t_f32 = m_module.defFloatType(32);
	uint32_t t_f32_v4 = m_module.defVectorType(t_f32, 4);
	//     uint32_t t_f32_a4 = m_module.defArrayType(t_f32, m_module.constu32(4));

	std::array<uint32_t, 1> members;
	members[PerVertex_Position] = t_f32_v4;
	//     members[PerVertex_CullDist] = t_f32_a4;
	//     members[PerVertex_ClipDist] = t_f32_a4;

	uint32_t typeId = m_module.defStructTypeUnique(
		members.size(), members.data());

	m_module.memberDecorateBuiltIn(typeId, PerVertex_Position, spv::BuiltInPosition);
	//     m_module.memberDecorateBuiltIn(typeId, PerVertex_CullDist, spv::BuiltInCullDistance);
	//     m_module.memberDecorateBuiltIn(typeId, PerVertex_ClipDist, spv::BuiltInClipDistance);
	m_module.decorateBlock(typeId);

	m_module.setDebugName(typeId, "s_per_vertex");
	m_module.setDebugMemberName(typeId, PerVertex_Position, "position");
	//     m_module.setDebugMemberName(typeId, PerVertex_CullDist, "cull_dist");
	//     m_module.setDebugMemberName(typeId, PerVertex_ClipDist, "clip_dist");
	return typeId;
}

std::vector<VertexInputSemantic> GCNCompiler::parseFetchShader()
{
	std::vector<VertexInputSemantic> inputSemantics;

	do 
	{
		if (!m_fsShader)
		{
			break;
		}


		//s_load_dwordx4 s[8:11], s[2:3], 0x00                      // 00000000: C0840300
		//s_load_dwordx4 s[12:15], s[2:3], 0x04                     // 00000004: C0860304
		//s_load_dwordx4 s[16:19], s[2:3], 0x08                     // 00000008: C0880308
		//s_waitcnt     lgkmcnt(0)                                  // 0000000C: BF8C007F
		//buffer_load_format_xyzw v[4:7], v0, s[8:11], 0 idxen      // 00000010: E00C2000 80020400
		//buffer_load_format_xyz v[8:10], v0, s[12:15], 0 idxen     // 00000018: E0082000 80030800
		//buffer_load_format_xy v[12:13], v0, s[16:19], 0 idxen     // 00000020: E0042000 80040C00
		//s_waitcnt     0                                           // 00000028: BF8C0000
		//s_setpc_b64   s[0:1]                                      // 0000002C: BE802000

		// A common fetch shader looks like the above, the instructions are generated
		// using input semantics on cpu side.
		// We take the reverse way, extract the original input semantics from these instructions.

		uint32_t semanIdx = 0;
		for (auto& ins : m_fsShader->m_instructionList)
		{
			if (ins.instruction->GetInstructionClass() != Instruction::VectorMemBufFmt)
			{
				// We only care about the buffer_load_format_xxx instructions
				continue;
			}

			SIMUBUFInstruction* vecLoadIns = castTo<SIMUBUFInstruction>(ins);

			VertexInputSemantic semantic = {0};
			semantic.semantic = semanIdx;
			semantic.vgpr = vecLoadIns->GetVDATA();
			semantic.sizeInElements = (uint32_t)vecLoadIns->GetOp() + 1;
			semantic.reserved = 0;

			inputSemantics.push_back(semantic);

			++semanIdx;
		}
	} while (false);

	return inputSemantics;
}

void GCNCompiler::processInstruction(GCNInstruction& ins)
{
	Instruction::InstructionCategory insCategory = ins.instruction->GetInstructionCategory();
	switch (insCategory)
	{
	case Instruction::ScalarALU:
		emitScalarALU(ins);
		break;
	case Instruction::ScalarMemory:
		emitScalarMemory(ins);
		break;
	case Instruction::VectorALU:
		emitVectorALU(ins);
		break;
	case Instruction::VectorMemory:
		emitVectorMemory(ins);
		break;
	case Instruction::FlowControl:
		emitFlowControl(ins);
		break;
	case Instruction::DataShare:
		emitDataShare(ins);
		break;
	case Instruction::VectorInterpolation:
		emitVectorInterpolation(ins);
		break;
	case Instruction::Export:
		emitExport(ins);
		break;
	case Instruction::DebugProfile:
		emitDebugProfile(ins);
		break;
	case Instruction::CategoryUnknown:
	case Instruction::InstructionsCategoriesCount:
		LOG_FIXME("Instruction category not initialized. Encoding %d", ins.instruction->GetInstructionFormat());
		break;
	default:
		break;
	}
}

RcPtr<gve::GveShader> GCNCompiler::finalize()
{
	return RcPtr<gve::GveShader>(new gve::GveShader());
}






//////////////////////////////////////////////////////////////////////////





} // namespace pssl