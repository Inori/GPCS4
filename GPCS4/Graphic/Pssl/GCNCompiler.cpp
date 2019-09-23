#include "GCNCompiler.h"

#include <array>

namespace pssl
{;

constexpr uint32_t PerVertex_Position = 0;
constexpr uint32_t PerVertex_CullDist = 1;
constexpr uint32_t PerVertex_ClipDist = 2;

GCNCompiler::GCNCompiler(const PsslProgramInfo& progInfo):
	GCNCompiler(progInfo, {})
{

}

GCNCompiler::GCNCompiler(const PsslProgramInfo& progInfo, const std::vector<VertexInputSemantic>& inputSemantic):
	m_programInfo(progInfo),
	m_vsInputSemantic(inputSemantic)
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