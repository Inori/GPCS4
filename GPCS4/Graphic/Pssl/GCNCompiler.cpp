#include "GCNCompiler.h"
#include "Platform/UtilString.h"
#include <array>

namespace pssl
{;

constexpr uint32_t PerVertex_Position = 0;
constexpr uint32_t PerVertex_CullDist = 1;
constexpr uint32_t PerVertex_ClipDist = 2;

GCNCompiler::GCNCompiler(const PsslProgramInfo& progInfo, const GcnAnalysisInfo& analysis):
	GCNCompiler(progInfo, analysis, {})
{

}

GCNCompiler::GCNCompiler(const PsslProgramInfo& progInfo, const GcnAnalysisInfo& analysis, 
	const std::vector<VertexInputSemantic>& inputSemantic):
	m_programInfo(progInfo),
	m_vsInputSemantics(inputSemantic),
	m_analysis(&analysis)
{
	// Declare an entry point ID. We'll need it during the
	// initialization phase where the execution mode is set.
	m_entryPointId = m_module.allocateId();

	// Set the shader name so that we recognize it in renderdoc
	m_module.setDebugSource(
		spv::SourceLanguageUnknown, 0,
		m_module.addDebugString(progInfo.key().toString().c_str()),
		nullptr);

	//// Set the memory model. This is the same for all shaders.
	m_module.setMemoryModel(
		spv::AddressingModelLogical,
		spv::MemoryModelGLSL450);


	emitInit();
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
	switch (m_programInfo.shaderType())
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
	//m_module.enableCapability(spv::CapabilityClipDistance);
	//m_module.enableCapability(spv::CapabilityCullDistance);
	m_module.enableCapability(spv::CapabilityDrawParameters);

	m_module.enableExtension("SPV_KHR_shader_draw_parameters");

	emitDclVertexInput();
	emitDclVertexOutput();
	emitEmuFetchShader();

	// Main function of the vertex shader
	m_vs.mainFunctionId = m_module.allocateId();
	m_module.setDebugName(m_vs.mainFunctionId, "vs_main");

	emitFunctionBegin(
		m_vs.mainFunctionId,
		m_module.defVoidType(),
		m_module.defFunctionType(
		m_module.defVoidType(), 0, nullptr));

	emitFunctionLabel();

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_vs.fsFunctionId, 0, nullptr);
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

void GCNCompiler::emitVsFinalize()
{
	this->emitMainFunctionBegin();

	//emitInputSetup();

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_vs.mainFunctionId, 0, nullptr);

	//emitOutputSetup();
	
	this->emitFunctionEnd();
}

void GCNCompiler::emitHsFinalize()
{

}

void GCNCompiler::emitDsFinalize()
{

}

void GCNCompiler::emitGsFinalize()
{

}

void GCNCompiler::emitPsFinalize()
{

}

void GCNCompiler::emitCsFinalize()
{

}

void GCNCompiler::emitFunctionBegin(uint32_t entryPoint, uint32_t returnType, uint32_t funcType)
{
	emitFunctionEnd();

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

void GCNCompiler::emitMainFunctionBegin()
{
	emitFunctionBegin(
		m_entryPointId,
		m_module.defVoidType(),
		m_module.defFunctionType(
		m_module.defVoidType(), 0, nullptr));

	emitFunctionLabel();
}

void GCNCompiler::emitFunctionLabel()
{
	m_module.opLabel(m_module.allocateId());
}

void GCNCompiler::emitDclVertexInput()
{
	do 
	{
		if (m_vsInputSemantics.empty())
		{
			break;
		}

		for (const auto& inputSemantic : m_vsInputSemantics)
		{
			// TODO:
			// Not sure if all vertex inputs are float type
			auto inputReg = emitDclFloatVector(32, inputSemantic.sizeInElements, spv::StorageClassInput);
			m_vs.vsInputs[inputSemantic.semantic] = inputReg;

			// Use semantic index for location, so vulkan code need to match.
			m_module.decorateLocation(inputReg.varId, inputSemantic.semantic);
			m_entryPointInterfaces.push_back(inputReg.varId);
		}
	} while (false);
}

void GCNCompiler::emitDclVertexOutput()
{
	// Declare the per-vertex output block. This is where
	// the vertex shader will write the vertex position.
	const uint32_t perVertexStructType = getPerVertexBlockId();
	const uint32_t perVertexPointerType = m_module.defPointerType(
		perVertexStructType, spv::StorageClassOutput);

	m_perVertexOut = m_module.newVar(perVertexPointerType, spv::StorageClassOutput);
	m_entryPointInterfaces.push_back(m_perVertexOut);
	m_module.setDebugName(m_perVertexOut, "vs_vertex_out");
}

void GCNCompiler::emitEmuFetchShader()
{
	do 
	{
		if (m_vsInputSemantics.empty())
		{
			break;
		}

		m_vs.fsFunctionId = m_module.allocateId();

		emitFunctionBegin(
			m_vs.fsFunctionId,
			m_module.defVoidType(),
			m_module.defFunctionType(
			m_module.defVoidType(), 0, nullptr));
		emitFunctionLabel();
		m_module.setDebugName(m_vs.fsFunctionId, "vs_fetch");

		for (const auto& inputSemantic : m_vsInputSemantics)
		{
			for (uint32_t i = 0; i != inputSemantic.sizeInElements; ++i)
			{
				uint32_t vgprIdx = inputSemantic.vgpr + i;

				// Declare a new vgpr reg
				// TODO:
				// Not sure if all vertex inputs are float type
				auto vgprReg = emitDclFloat(32, spv::StorageClassPrivate, UtilString::Format("v%d", vgprIdx));
				uint32_t inputVarId = m_vs.vsInputs[inputSemantic.semantic].varId;

				// Access vector member
				uint32_t fpPtrTypeId = m_module.defFloatPointerType(32, spv::StorageClassPrivate);
				uint32_t accessIndexArray[] = { m_module.constu32(i) };
				uint32_t inputElementId = m_module.opAccessChain(
					fpPtrTypeId,
					inputVarId,
					1, accessIndexArray);

				// Store input value to our new vgpr reg.
				uint32_t loadId = m_module.opLoad(fpPtrTypeId, inputElementId);
				m_module.opStore(vgprReg.varId, loadId);

				// Save to the map
				m_vgprs[vgprIdx] = vgprReg;
			}
		}

		emitFunctionEnd();
	} while (false);
}

pssl::SpirvRegister GCNCompiler::emitDclFloat(uint32_t width, 
	spv::StorageClass storageCls, const std::string& debugName /*= ""*/)
{
	uint32_t fpPtrTypeId = m_module.defFloatPointerType(width, storageCls);
	uint32_t varId = m_module.newVar(fpPtrTypeId, storageCls);
	if (!debugName.empty())
	{
		m_module.setDebugName(varId, debugName.c_str());
	}
	return SpirvRegister(fpPtrTypeId, varId);
}

SpirvRegister GCNCompiler::emitDclFloatVector(uint32_t width, uint32_t count, 
	spv::StorageClass storageCls, const std::string& debugName /*= ""*/)
{
	uint32_t fpTypeId = m_module.defFloatType(width);
	uint32_t vfpTypeId = m_module.defVectorType(fpTypeId, count);
	uint32_t vfpPtrTypeId = m_module.defPointerType(vfpTypeId, storageCls);
	uint32_t varId = m_module.newVar(vfpPtrTypeId, storageCls);
	if (!debugName.empty())
	{
		m_module.setDebugName(varId, debugName.c_str());
	}
	return SpirvRegister(vfpPtrTypeId, varId);
}

SpirvRegister GCNCompiler::emitValueLoad(const SpirvRegister& reg)
{

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
	switch (m_programInfo.shaderType())
	{
	case VertexShader:   this->emitVsFinalize(); break;
	case HullShader:     this->emitHsFinalize(); break;
	case DomainShader:   this->emitDsFinalize(); break;
	case GeometryShader: this->emitGsFinalize(); break;
	case PixelShader:    this->emitPsFinalize(); break;
	case ComputeShader:  this->emitCsFinalize(); break;
	}

	// Declare the entry point, we now have all the
	// information we need, including the interfaces
	m_module.addEntryPoint(m_entryPointId,
		m_programInfo.executionModel(), "main",
		m_entryPointInterfaces.size(),
		m_entryPointInterfaces.data());
	m_module.setDebugName(m_entryPointId, "main");

	return new gve::GveShader(m_programInfo.shaderStage(), 
		m_module.compile(), 
		m_programInfo.key());
}



} // namespace pssl