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
	const uint32_t perVertexStructType = this->getPerVertexBlockId();
	const uint32_t perVertexPointerType = m_module.defPointerType(
		perVertexStructType, spv::StorageClassOutput);

	m_perVertexOut = m_module.newVar(
		perVertexPointerType, spv::StorageClassOutput);
	m_entryPointInterfaces.push_back(m_perVertexOut);
	m_module.setDebugName(m_perVertexOut, "vs_vertex_out");

	// Main function of the vertex shader
	m_vs.functionId = m_module.allocateId();
	m_module.setDebugName(m_vs.functionId, "vs_main");

	this->emitFunctionBegin(
		m_vs.functionId,
		m_module.defVoidType(),
		m_module.defFunctionType(
			m_module.defVoidType(), 0, nullptr));
	this->emitFunctionLabel();
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

void GCNCompiler::emitFunctionBegin(uint32_t entryPoint, uint32_t returnType, uint32_t funcType)
{
	this->emitFunctionEnd();

	m_module.functionBegin(
		returnType, entryPoint, funcType,
		spv::FunctionControlMaskNone);

	m_insideFunction = true;
}

void GCNCompiler::emitFunctionEnd()
{
	if (m_insideFunction) 
	{
		m_module.opReturn();
		m_module.functionEnd();
	}

	m_insideFunction = false;
}

void GCNCompiler::emitFunctionLabel()
{
	m_module.opLabel(m_module.allocateId());
}

uint32_t GCNCompiler::getPerVertexBlockId()
{
	// Should be:
	// out gl_PerVertex
	// {
	//   vec4 gl_Position;
	//   float gl_PointSize;
	//   float gl_ClipDist[];
	//   float gl_CullDist[];
	// };

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

	m_module.setDebugName(typeId, "gl_PerVertex");
	m_module.setDebugMemberName(typeId, PerVertex_Position, "Position");
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



} // namespace pssl