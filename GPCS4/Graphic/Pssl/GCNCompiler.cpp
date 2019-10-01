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
			auto inputReg = emitDclFloatVector(SpirvScalarType::Float32, inputSemantic.sizeInElements, spv::StorageClassInput);
			m_vs.vsInputs[inputSemantic.semantic] = inputReg;

			// Use semantic index for location, so vulkan code need to match.
			m_module.decorateLocation(inputReg.id, inputSemantic.semantic);
			m_entryPointInterfaces.push_back(inputReg.id);
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
				auto vgprReg = emitDclFloat(SpirvScalarType::Float32, 
					spv::StorageClassPrivate, UtilString::Format("v%d", vgprIdx));
				uint32_t inputVarId = m_vs.vsInputs[inputSemantic.semantic].id;

				// Access vector member
				uint32_t fpPtrTypeId = m_module.defFloatPointerType(32, spv::StorageClassPrivate);
				uint32_t accessIndexArray[] = { m_module.constu32(i) };
				uint32_t inputElementId = m_module.opAccessChain(
					fpPtrTypeId,
					inputVarId,
					1, accessIndexArray);

				// Store input value to our new vgpr reg.
				uint32_t loadId = m_module.opLoad(fpPtrTypeId, inputElementId);
				m_module.opStore(vgprReg.id, loadId);

				// Save to the map
				m_vgprs[vgprIdx] = vgprReg;
			}
		}

		emitFunctionEnd();
	} while (false);
}

SpirvRegisterPointer GCNCompiler::emitDclFloat(SpirvScalarType type,
	spv::StorageClass storageCls, const std::string& debugName /*= ""*/)
{
	uint32_t width = type == SpirvScalarType::Float32 ? 32 : 64;
	uint32_t fpPtrTypeId = m_module.defFloatPointerType(width, storageCls);
	uint32_t varId = m_module.newVar(fpPtrTypeId, storageCls);
	if (!debugName.empty())
	{
		m_module.setDebugName(varId, debugName.c_str());
	}
	
	return SpirvRegisterPointer(type, 1, varId);
}

SpirvRegisterPointer GCNCompiler::emitDclFloatVector(SpirvScalarType type, uint32_t count,
	spv::StorageClass storageCls, const std::string& debugName /*= ""*/)
{
	uint32_t width = type == SpirvScalarType::Float32 ? 32 : 64;
	uint32_t fpTypeId = m_module.defFloatType(width);
	uint32_t vfpTypeId = m_module.defVectorType(fpTypeId, count);
	uint32_t vfpPtrTypeId = m_module.defPointerType(vfpTypeId, storageCls);
	uint32_t varId = m_module.newVar(vfpPtrTypeId, storageCls);
	if (!debugName.empty())
	{
		m_module.setDebugName(varId, debugName.c_str());
	}
	return SpirvRegisterPointer(type, count, varId);
}

SpirvRegisterValue GCNCompiler::emitValueLoad(const SpirvRegisterPointer& reg)
{
	uint32_t varId = m_module.opLoad(
		getVectorTypeId(reg.type),
		reg.id);
	return SpirvRegisterValue(reg.type, varId);
}

SpirvRegisterValue GCNCompiler::emitSgprLoad(uint32_t index)
{
	return emitValueLoad(m_sgprs[index]);
}

SpirvRegisterValue GCNCompiler::emitVgprLoad(uint32_t index)
{
	return emitValueLoad(m_vgprs[index]);
}

void GCNCompiler::emitValueStore(
	const SpirvRegisterPointer &ptr,
	const SpirvRegisterValue &src, 
	const GcnRegMask &writeMask)
{
	SpirvRegisterValue value = src;
	// If the component types are not compatible,
	// we need to bit-cast the source variable.
	if (src.type.ctype != ptr.type.ctype)
	{
		value = emitRegisterBitcast(src, ptr.type.ctype);
	}
		
	// If the source value consists of only one component,
	// it is stored in all components of the destination.
	if (src.type.ccount == 1)
	{
		value = emitRegisterExtend(src, writeMask.popCount());
	}
		
	if (ptr.type.ccount == writeMask.popCount()) 
	{
		// Simple case: We write to the entire register
		m_module.opStore(ptr.id, value.id);
	}
	else 
	{
		// We only write to part of the destination
		// register, so we need to load and modify it
		SpirvRegisterValue tmp = emitValueLoad(ptr);
		tmp = emitRegisterInsert(tmp, value, writeMask);

		m_module.opStore(ptr.id, tmp.id);
	}
}

void GCNCompiler::emitSgprStore(uint32_t dstIdx, const SpirvRegisterValue& srcReg)
{
	emitValueStore(m_sgprs[dstIdx], srcReg, 1);
}

void GCNCompiler::emitVgprStore(uint32_t dstIdx, const SpirvRegisterValue& srcReg)
{
	emitValueStore(m_vgprs[dstIdx], srcReg, 1);
}

// See table "SDST, SSRC and SRC Operands" in section 3.1 of GPU Shader Core ISA manual
pssl::SpirvRegisterValue GCNCompiler::emitLoadScalarOperand(uint32_t index, uint32_t literalConst /*= 0*/)
{
	Instruction::OperandSRC src = static_cast<Instruction::OperandSRC>(index);
	SpirvRegisterValue operand;

	switch (src)
	{
	case Instruction::OperandSRC::SRCScalarGPRMin ... Instruction::OperandSRC::SRCScalarGPRMax:
	{
		operand = emitSgprLoad((uint32_t)src);
	}
		break;
	case Instruction::OperandSRC::SRCVccLo:
		break;
	case Instruction::OperandSRC::SRCVccHi:
		break;
	case Instruction::OperandSRC::SRCM0:
		break;
	case Instruction::OperandSRC::SRCExecLo:
		break;
	case Instruction::OperandSRC::SRCExecHi:
		break;
	case Instruction::OperandSRC::SRCConstZero:
		break;
	case Instruction::OperandSRC::SRCSignedConstIntPosMin ... Instruction::OperandSRC::SRCSignedConstIntPosMax:
	{

	}
		break;
	case Instruction::OperandSRC::SRCSignedConstIntNegMin ... Instruction::OperandSRC::SRCSignedConstIntNegMax:
	{

	}
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_0_5:
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_0_5:
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_1_0:
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_1_0:
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_2_0:
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_2_0:
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_4_0:
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_4_0:
		break;
	case Instruction::OperandSRC::SRCVCCZ:
		break;
	case Instruction::OperandSRC::SRCEXECZ:
		break;
	case Instruction::OperandSRC::SRCSCC:
		break;
	case Instruction::OperandSRC::SRCLdsDirect:
		break;
	case Instruction::OperandSRC::SRCLiteralConst:
	{

	}
		break;
	// For 9 bits SRC operand
	case Instruction::OperandSRC::SRCVectorGPRMin ... Instruction::OperandSRC::SRCVectorGPRMax:
	{

	}
		break;
	default:
		LOG_ERR("error operand range %d", (uint32_t)src);
		break;
	}

	return operand;
}

// See table "VSRC and VDST Operands" in section 3.1 of GPU Shader Core ISA manual
SpirvRegisterValue GCNCompiler::emitLoadVectorOperand(uint32_t index)
{

}

void GCNCompiler::emitStoreScalarOperand(uint32_t dstIndex, const SpirvRegisterValue& srcReg)
{
	Instruction::OperandSDST dst = static_cast<Instruction::OperandSDST>(dstIndex);
	
	switch (dst)
	{
	case Instruction::OperandSDST::SDSTScalarGPRMin ... Instruction::OperandSDST::SDSTScalarGPRMax:
	{
		emitSgprStore((uint32_t)dst, srcReg);
	}
		break;
	case Instruction::OperandSDST::SDSTVccLo:
		break;
	case Instruction::OperandSDST::SDSTVccHi:
		break;
	case Instruction::OperandSDST::SDSTM0:
		break;
	case Instruction::OperandSDST::SDSTExecLo:
		break;
	case Instruction::OperandSDST::SDSTExecHi:
		break;
	default:
		LOG_ERR("error operand range %d", (uint32_t)dst);
		break;
	}
}

void GCNCompiler::emitStoreVectorOperand(uint32_t dstIndex, const SpirvRegisterValue& srcReg)
{

}

SpirvRegisterValue GCNCompiler::emitRegisterBitcast(SpirvRegisterValue srcValue, SpirvScalarType dstType)
{
	SpirvScalarType srcType = srcValue.type.ctype;

	if (srcType == dstType)
		return srcValue;

	SpirvRegisterValue result;
	result.type.ctype = dstType;
	result.type.ccount = srcValue.type.ccount;

	if (isWideType(srcType)) result.type.ccount *= 2;
	if (isWideType(dstType)) result.type.ccount /= 2;

	result.id = m_module.opBitcast(
		getVectorTypeId(result.type),
		srcValue.id);
	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterSwizzle(SpirvRegisterValue value, GcnRegSwizzle swizzle, GcnRegMask writeMask)
{
	if (value.type.ccount == 1)
	{
		return emitRegisterExtend(value, writeMask.popCount());
	}
		
	std::array<uint32_t, 4> indices;

	uint32_t dstIndex = 0;

	for (uint32_t i = 0; i < 4; i++) 
	{
		if (writeMask[i])
		{
			indices[dstIndex++] = swizzle[i];
		}
	}

	// If the swizzle combined with the mask can be reduced
	// to a no-op, we don't need to insert any instructions.
	bool isIdentitySwizzle = dstIndex == value.type.ccount;

	for (uint32_t i = 0; i < dstIndex && isIdentitySwizzle; i++)
		isIdentitySwizzle &= indices[i] == i;

	if (isIdentitySwizzle)
	{
		return value;
	}

	// Use OpCompositeExtract if the resulting vector contains
	// only one component, and OpVectorShuffle if it is a vector.
	SpirvRegisterValue result;
	result.type.ctype = value.type.ctype;
	result.type.ccount = dstIndex;

	const uint32_t typeId = getVectorTypeId(result.type);

	if (dstIndex == 1) 
	{
		result.id = m_module.opCompositeExtract(
			typeId, value.id, 1, indices.data());
	}
	else 
	{
		result.id = m_module.opVectorShuffle(
			typeId, value.id, value.id,
			dstIndex, indices.data());
	}

	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterExtract(SpirvRegisterValue value, GcnRegMask mask)
{
	return emitRegisterSwizzle(value,
		GcnRegSwizzle(0, 1, 2, 3), mask);
}

SpirvRegisterValue GCNCompiler::emitRegisterInsert(SpirvRegisterValue dstValue, SpirvRegisterValue srcValue, GcnRegMask srcMask)
{
	SpirvRegisterValue result;
	result.type = dstValue.type;

	const uint32_t typeId = getVectorTypeId(result.type);

	if (srcMask.popCount() == 0) 
	{
		// Nothing to do if the insertion mask is empty
		result.id = dstValue.id;
	}
	else if (dstValue.type.ccount == 1) 
	{
		// Both values are scalar, so the first component
		// of the write mask decides which one to take.
		result.id = srcMask[0] ? srcValue.id : dstValue.id;
	}
	else if (srcValue.type.ccount == 1) 
	{
		// The source value is scalar. Since OpVectorShuffle
		// requires both arguments to be vectors, we have to
		// use OpCompositeInsert to modify the vector instead.
		const uint32_t componentId = srcMask.firstSet();

		result.id = m_module.opCompositeInsert(typeId,
			srcValue.id, dstValue.id, 1, &componentId);
	}
	else 
	{
		// Both arguments are vectors. We can determine which
		// components to take from which vector and use the
		// OpVectorShuffle instruction.
		std::array<uint32_t, 4> components;
		uint32_t srcComponentId = dstValue.type.ccount;

		for (uint32_t i = 0; i < dstValue.type.ccount; i++)
		{
			components.at(i) = srcMask[i] ? srcComponentId++ : i;
		}
			
		result.id = m_module.opVectorShuffle(
			typeId, dstValue.id, srcValue.id,
			dstValue.type.ccount, components.data());
	}

	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterExtend(SpirvRegisterValue value, uint32_t size)
{
	if (size == 1)
	{
		return value;
	}
		
	std::array<uint32_t, 4> ids = { {
	  value.id, value.id,
	  value.id, value.id,
	} };

	SpirvRegisterValue result;
	result.type.ctype = value.type.ctype;
	result.type.ccount = size;
	result.id = m_module.opCompositeConstruct(
		getVectorTypeId(result.type),
		size, ids.data());
	return result;
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

uint32_t GCNCompiler::getScalarTypeId(SpirvScalarType type)
{
	if (type == SpirvScalarType::Float64)
	{
		m_module.enableCapability(spv::CapabilityFloat64);
	}

	if (type == SpirvScalarType::Sint64 || type == SpirvScalarType::Uint64)
	{
		m_module.enableCapability(spv::CapabilityInt64);
	}

	uint32_t typeId = 0;
	switch (type) 
	{
	case SpirvScalarType::Uint32:  typeId = m_module.defIntType(32, 0); break;
	case SpirvScalarType::Uint64:  typeId = m_module.defIntType(64, 0); break;
	case SpirvScalarType::Sint32:  typeId = m_module.defIntType(32, 1); break;
	case SpirvScalarType::Sint64:  typeId = m_module.defIntType(64, 1); break;
	case SpirvScalarType::Float32: typeId = m_module.defFloatType(32); break;
	case SpirvScalarType::Float64: typeId = m_module.defFloatType(64); break;
	case SpirvScalarType::Bool:    typeId = m_module.defBoolType(); break;
	}
	return typeId;
}

uint32_t GCNCompiler::getVectorTypeId(const SpirvVectorType& type)
{
	uint32_t typeId = this->getScalarTypeId(type.ctype);

	if (type.ccount > 1)
	{
		typeId = m_module.defVectorType(typeId, type.ccount);
	}

	return typeId;
}

bool GCNCompiler::isWideType(SpirvScalarType type) const
{
	return type == SpirvScalarType::Sint64
		|| type == SpirvScalarType::Uint64
		|| type == SpirvScalarType::Float64;
}

} // namespace pssl