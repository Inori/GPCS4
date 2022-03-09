#include "GCNCompiler.h"

#include "PsslBindingCalculator.h"
#include "PsslShaderRegField.h"
#include "UtilString.h"

#include "../Gnm/GnmSharpBuffer.h"

#include <array>

LOG_CHANNEL(Graphic.Pssl.GCNCompiler);

namespace pssl
{;

constexpr uint32_t kPerVertexPosition = 0;
constexpr uint32_t kPerVertexCullDist = 1;
constexpr uint32_t kPerVertexClipDist = 2;

GCNCompiler::GCNCompiler(
	const PsslProgramInfo& progInfo,
	const GcnAnalysisInfo& analysis,
	const GcnShaderInput&  shaderInput) :
	m_programInfo(progInfo),
	m_analysis(&analysis),
	m_shaderInput(shaderInput),
	m_branchLabels(std::move(m_analysis->branchLabels))
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
	emitBranchLabelTry();

	compileInstruction(ins);

	updateProgramCounter(ins);
}

void GCNCompiler::compileInstruction(GCNInstruction& ins)
{
	Instruction::InstructionCategory insCategory = ins.instruction->GetInstructionCategory();

	LOG_ASSERT(insCategory != Instruction::CategoryUnknown,
			   "instruction category not initialized.");

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

RcPtr<vlt::VltShader> GCNCompiler::finalize()
{
	switch (m_programInfo.shaderType())
	{
	case PsslProgramType::VertexShader:
		emitVsFinalize();
		break;
	case PsslProgramType::HullShader:
		emitHsFinalize();
		break;
	case PsslProgramType::DomainShader:
		emitDsFinalize();
		break;
	case PsslProgramType::GeometryShader:
		emitGsFinalize();
		break;
	case PsslProgramType::PixelShader:
		emitPsFinalize();
		break;
	case PsslProgramType::ComputeShader:
		emitCsFinalize();
		break;
	}

	// Declare the entry point, we now have all the
	// information we need, including the interfaces
	m_module.addEntryPoint(m_entryPointId,
						   m_programInfo.executionModel(), "main",
						   m_entryPointInterfaces.size(),
						   m_entryPointInterfaces.data());
	m_module.setDebugName(m_entryPointId, "main");

	return new vlt::VltShader(
		m_programInfo.shaderStage(),
		m_module.compile(),
		m_programInfo.key(),
		std::move(m_resourceSlots));
}

void GCNCompiler::emitInit()
{
	// Set up common capabilities for all shaders
	m_module.enableCapability(spv::CapabilityShader);
	m_module.enableCapability(spv::CapabilityImageQuery);

	emitDclStateRegisters();
	emitDclShaderResourceUD();

	// Initialize the shader module with capabilities
	// etc. Each shader type has its own peculiarities.
	switch (m_programInfo.shaderType())
	{
	case PsslProgramType::VertexShader:		emitVsInit();	break;
	case PsslProgramType::HullShader:		emitHsInit();	break;
	case PsslProgramType::DomainShader:		emitDsInit();	break;
	case PsslProgramType::GeometryShader:	emitGsInit();	break;
	case PsslProgramType::PixelShader:		emitPsInit();	break;
	case PsslProgramType::ComputeShader:	emitCsInit();	break;
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
	m_module.setDebugName(m_vs.mainFunctionId, "vsMain");

	emitFunctionBegin(
		m_vs.mainFunctionId,
		m_module.defVoidType(),
		m_module.defFunctionType(
			m_module.defVoidType(), 0, nullptr));

	emitFunctionLabel();

	if (m_shaderInput.vsInputSemantics.has_value())
	{
		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_vs.fsFunctionId, 0, nullptr);
	}

	// Some initialization steps need to place in function block.
	emitGprInitializeVS();
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
	m_module.setExecutionMode(m_entryPointId,
							  spv::ExecutionModeOriginUpperLeft);

	// Main function of the pixel shader
	m_ps.functionId = m_module.allocateId();
	m_module.setDebugName(m_ps.functionId, "psMain");

	emitDclPixelInput();
	emitDclPixelOutput();

	this->emitFunctionBegin(
		m_ps.functionId,
		m_module.defVoidType(),
		m_module.defFunctionType(
			m_module.defVoidType(), 0, nullptr));
	this->emitFunctionLabel();

	// Some initialization steps need to place in function block.
	emitGprInitializePS();
}

void GCNCompiler::emitCsInit()
{
	// Main function of the compute shader
	m_cs.functionId = m_module.allocateId();
	m_module.setDebugName(m_cs.functionId, "csMain");

	emitDclThreadGroup();
	emitDclThreadGroupSharedMemory();

	this->emitFunctionBegin(
		m_cs.functionId,
		m_module.defVoidType(),
		m_module.defFunctionType(
			m_module.defVoidType(), 0, nullptr));
	this->emitFunctionLabel();

	emitGprInitializeCS();
}

void GCNCompiler::emitVsFinalize()
{
	this->emitMainFunctionBegin();

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_vs.mainFunctionId, 0, nullptr);

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
	emitMainFunctionBegin();

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_ps.functionId, 0, nullptr);

	emitFunctionEnd();
}

void GCNCompiler::emitCsFinalize()
{
	this->emitMainFunctionBegin();

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_cs.functionId, 0, nullptr);

	this->emitFunctionEnd();
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
	if (m_insideBlock)
	{
		m_module.opReturn();
		m_insideBlock = false;
	}

	if (m_insideFunction)
	{
		m_module.functionEnd();
		m_insideFunction = false;
	}
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
	m_insideBlock = true;
}

void GCNCompiler::emitDclVertexInput()
{
	do
	{
		if (!m_shaderInput.vsInputSemantics.has_value())
		{
			break;
		}

		for (const auto& inputSemantic : m_shaderInput.vsInputSemantics.value())
		{
			SpirvRegisterInfo info(SpirvScalarType::Float32, inputSemantic.sizeInElements, 0, spv::StorageClassInput);
			uint32_t          inputId = emitNewVariable(info,
                                               str::format("inParam%d", inputSemantic.semantic));

			// Use semantic index for location, so vulkan code need to match.
			m_module.decorateLocation(inputId, inputSemantic.semantic);
			m_entryPointInterfaces.push_back(inputId);

			m_vs.vsInputs[inputSemantic.semantic] = SpirvRegisterPointer(info.atype.vtype, inputId);
		}
	} while (false);
}

void GCNCompiler::emitDclVertexOutput()
{
	// Declare the per-vertex output block. This is where
	// the vertex shader will write the vertex position.
	const uint32_t perVertexStructType  = getPerVertexBlockId();
	const uint32_t perVertexPointerType = m_module.defPointerType(
		perVertexStructType, spv::StorageClassOutput);

	m_perVertexOut = m_module.newVar(perVertexPointerType, spv::StorageClassOutput);

	m_entryPointInterfaces.push_back(m_perVertexOut);
	m_module.setDebugName(m_perVertexOut, "vsVertexOut");

	// Declare other vertex output.
	// like normal or texture coordinate
	do
	{
		for (const auto& expInfo : m_analysis->expParams)
		{
			if (expInfo.target == EXPInstruction::TGT::TGTExpPosMin)
			{
				// Already handled above
				continue;
			}

			uint32_t          outLocation = expInfo.target - (uint32_t)EXPInstruction::TGT::TGTExpParamMin;
			SpirvRegisterInfo info(SpirvScalarType::Float32, expInfo.regIndices.size(),
								   0, spv::StorageClassOutput);
			uint32_t          outputId = emitNewVariable(info,
                                                str::format("outParam%d", outLocation));

			m_module.decorateLocation(outputId, outLocation);

			m_vs.vsOutputs[expInfo.target] = SpirvRegisterPointer(info.atype.vtype, outputId);
			m_entryPointInterfaces.push_back(outputId);
			++outLocation;
		}
	} while (false);
}

void GCNCompiler::emitEmuFetchShader()
{
	do
	{
		if (!m_shaderInput.vsInputSemantics.has_value())
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
		m_module.setDebugName(m_vs.fsFunctionId, "vsFetch");

		for (const auto& inputSemantic : m_shaderInput.vsInputSemantics.value())
		{
			for (uint32_t i = 0; i != inputSemantic.sizeInElements; ++i)
			{
				uint32_t vgprIdx = inputSemantic.vgpr + i;

				// Declare a new vgpr reg
				SpirvRegisterInfo info(SpirvScalarType::Float32, 1,
									   0, spv::StorageClassPrivate);
				uint32_t          vgprId = emitNewVariable(info,
                                                  str::format("v%d", vgprIdx));

				auto& input = m_vs.vsInputs[inputSemantic.semantic];

				// Access vector member
				auto element = emitVectorLoad(
					input,
					spv::StorageClassInput,
					GcnRegMask::select(i));

				// Store input value to our new vgpr reg.
				m_module.opStore(vgprId, element.id);

				// Save to the map
				m_vgprs[vgprIdx] = SpirvRegisterPointer(info.atype.vtype, vgprId);
			}
		}

		emitFunctionEnd();
	} while (false);
}

void GCNCompiler::emitDclPixelInput()
{
	for (uint32_t i = 0; i != m_analysis->vinterpAttrCount; ++i)
	{
		// Treat all input variables as vec4
		SpirvRegisterInfo info(SpirvScalarType::Float32, 4,
							   0, spv::StorageClassInput);
		uint32_t          inputId = emitNewVariable(info,
                                           str::format("inParam%d", i));

		m_module.decorateLocation(inputId, i);

		m_ps.psInputs[i] = SpirvRegisterPointer(info.atype.vtype, inputId);
		m_entryPointInterfaces.push_back(inputId);
	}
}

void GCNCompiler::emitDclPixelOutput()
{
	size_t expCount = m_analysis->expParams.size();
	for (size_t i = 0; i != expCount; ++i)
	{
		// TODO:
		// Currently I don't detect the target's type,
		// we need to support different target like mrtz for more complex shaders
		// in the future.
		const auto& exp            = m_analysis->expParams[i];
		uint32_t    componentCount = exp.isCompressed ? exp.regIndices.size() * 2 : exp.regIndices.size();

		SpirvRegisterInfo info(SpirvScalarType::Float32, componentCount,
							   0, spv::StorageClassOutput);
		uint32_t          outputId = emitNewVariable(info,
                                            str::format("outParam%d", i));

		m_module.decorateLocation(outputId, i);

		m_ps.psOutputs.emplace(exp.target, SpirvRegisterPointer(info.atype.vtype, outputId));
		m_entryPointInterfaces.push_back(outputId);
	}
}

void GCNCompiler::emitGprInitializeVS()
{
	/// VGPR

	// Declare gl_VertexIndex
	SpirvRegisterValue vertexIndex = emitVsSystemValueLoad(SpirvSystemValue::VertexId, 1);
	// v0 is the index of current vertex within vertex buffer
	SpirvRegisterPointer v0 = emitVgprCreate(
		0,
		SpirvScalarType::Uint32,
		vertexIndex.id);

	m_vgprs[0] = v0;

	/// SGPR
	emitInitUserDataRegisters(m_shaderInput.meta.vs.userSgprCount);
}

void GCNCompiler::emitGprInitializePS()
{
	// For sgprs and vgprs, we should initialize them
	// following the ISA manual:
	// 7. Appendix: GPR Allocation and Initialization
	// e.g. We could declare another uniform buffer to hold
	// the 16 user data registers.
	//

	/// VGPR
	const SPI_PS_INPUT_ADDR* spiPsInputAddr = 
		reinterpret_cast<const SPI_PS_INPUT_ADDR*>(&m_shaderInput.meta.ps.spiPsInputAddr);

	// TODO:
	// Currently I just create some dummy registers.

	uint32_t vindex = 0;
	if (spiPsInputAddr->persp_sample_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}
	if (spiPsInputAddr->persp_center_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}
	if (spiPsInputAddr->persp_centroid_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}
	if (spiPsInputAddr->persp_pull_model_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}
	if (spiPsInputAddr->linear_sample_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}
	if (spiPsInputAddr->linear_center_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}
	if (spiPsInputAddr->linear_centroid_ena)
	{
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32);
		++vindex;
	}

	SpirvRegisterValue fragCoord;
	if (spiPsInputAddr->pos_x_float_ena || 
		spiPsInputAddr->pos_y_float_ena || 
		spiPsInputAddr->pos_z_float_ena || 
		spiPsInputAddr->pos_w_float_ena)
	{
		fragCoord = emitPsSystemValueLoad(SpirvSystemValue::Position, GcnRegMask::firstN(4));
	}
	if (spiPsInputAddr->pos_x_float_ena)
	{
		auto posX       = emitRegisterExtract(fragCoord, GcnRegMask::select(0));
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, posX.id);
		++vindex;
	}
	if (spiPsInputAddr->pos_y_float_ena)
	{
		auto posY       = emitRegisterExtract(fragCoord, GcnRegMask::select(1));
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, posY.id);
		++vindex;
	}
	if (spiPsInputAddr->pos_z_float_ena)
	{
		auto posZ       = emitRegisterExtract(fragCoord, GcnRegMask::select(2));
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, posZ.id);
		++vindex;
	}
	if (spiPsInputAddr->pos_w_float_ena)
	{
		auto posW       = emitRegisterExtract(fragCoord, GcnRegMask::select(3));
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, posW.id);
		++vindex;
	}
	if (spiPsInputAddr->front_face_ena)
	{
	}
	if (spiPsInputAddr->ancillary_ena)
	{
	}
	if (spiPsInputAddr->sample_coverage_ena)
	{
	}
	if (spiPsInputAddr->pos_fixed_pt_ena)
	{
	}

	/// SGPR
	emitInitUserDataRegisters(m_shaderInput.meta.ps.userSgprCount);

	uint32_t sindex = m_shaderInput.meta.ps.userSgprCount;
	// TODO:
	// For temp

	// This should be s_ps_state.
	// I'm not sure what it is.
	m_sgprs[sindex] = emitSgprCreate(sindex, SpirvScalarType::Float32);
	++sindex;

	m_sgprs[16] = emitSgprCreate(16, SpirvScalarType::Float32);
}

void GCNCompiler::emitGprInitializeCS()
{
	const COMPUTE_PGM_RSRC2* computePgmRsrc2 =
		reinterpret_cast<const COMPUTE_PGM_RSRC2*>(&m_shaderInput.meta.cs.computePgmRsrc2);

	/// VGPR
	uint32_t vindex = 0;

	SpirvRegisterValue threadId = emitCsSystemValueLoad(SpirvSystemValue::ThreadId, 0);

	auto tidX = emitRegisterBitcast(
		emitRegisterExtract(threadId, GcnRegMask::select(0)),
		SpirvScalarType::Float32);
	m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, tidX.id);
	++vindex;

	if (computePgmRsrc2->tidig_comp_cnt >= 1)
	{
		auto tidY = emitRegisterBitcast(
			emitRegisterExtract(threadId, GcnRegMask::select(1)),
			SpirvScalarType::Float32);
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, tidY.id);
		++vindex;
	}
	if (computePgmRsrc2->tidig_comp_cnt >= 2)
	{
		auto tidZ = emitRegisterBitcast(
			emitRegisterExtract(threadId, GcnRegMask::select(1)),
			SpirvScalarType::Float32);
		m_vgprs[vindex] = emitVgprCreate(vindex, SpirvScalarType::Float32, tidZ.id);
		++vindex;
	}

	/// SGPR
	emitInitUserDataRegisters(m_shaderInput.meta.cs.userSgprCount);

	uint32_t sindex = m_shaderInput.meta.cs.userSgprCount;

	SpirvRegisterValue groupId;
	if (computePgmRsrc2->tgid_x_en || computePgmRsrc2->tgid_x_en || computePgmRsrc2->tgid_x_en)
	{
		groupId = emitCsSystemValueLoad(SpirvSystemValue::ThreadGroupId, 0);
	}
	if (computePgmRsrc2->tgid_x_en)
	{
		auto tgidX = emitRegisterBitcast(
			emitRegisterExtract(groupId, GcnRegMask::select(0)), 
			SpirvScalarType::Float32);
		m_sgprs[sindex] = emitSgprCreate(sindex, SpirvScalarType::Float32, tgidX.id);
		++sindex;
	}
	if (computePgmRsrc2->tgid_y_en)
	{
		auto tgidY = emitRegisterBitcast(
			emitRegisterExtract(groupId, GcnRegMask::select(1)),
			SpirvScalarType::Float32);
		m_sgprs[sindex] = emitSgprCreate(sindex, SpirvScalarType::Float32, tgidY.id);
		++sindex;
	}
	if (computePgmRsrc2->tgid_z_en)
	{
		auto tgidZ = emitRegisterBitcast(
			emitRegisterExtract(groupId, GcnRegMask::select(2)),
			SpirvScalarType::Float32);
		m_sgprs[sindex] = emitSgprCreate(sindex, SpirvScalarType::Float32, tgidZ.id);
		++sindex;
	}
	if (computePgmRsrc2->tg_size_en)
	{
	}
	if (computePgmRsrc2->scratch_en)
	{
	}
}

void GCNCompiler::emitInitUserDataRegisters(uint32_t count)
{
	for (uint32_t i = 0; i != count; ++i)
	{
		uint32_t regValue = m_shaderInput.userSgpr.at(i);

		SpirvRegisterValue spvValue;
		spvValue.type.ctype  = SpirvScalarType::Uint32;
		spvValue.type.ccount = 1;
		spvValue.id          = m_module.constu32(regValue);

		// We use float type to hold all registers, so cast uint register
		// value to float
		spvValue   = emitRegisterBitcast(spvValue, SpirvScalarType::Float32);
		m_sgprs[i] = emitSgprCreate(i, SpirvScalarType::Float32, spvValue.id);
	}
}

void GCNCompiler::emitDclStateRegisters()
{
	SpirvVectorType u32Type;
	u32Type.ctype  = SpirvScalarType::Uint32;
	u32Type.ccount = 1;

	m_stateRegs.m0.type = u32Type;
	m_stateRegs.m0.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "m0");

	m_stateRegs.vcc.lo.type = u32Type;
	m_stateRegs.vcc.lo.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "vcc_lo");
	m_stateRegs.vcc.hi.type = u32Type;
	m_stateRegs.vcc.hi.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "vcc_hi");

	// we assume thread_id = 1, so exec initialized to 1
	m_stateRegs.exec.lo.type = u32Type;
	m_stateRegs.exec.lo.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "exec_lo", m_module.constu32(1));
	m_stateRegs.exec.hi.type = u32Type;
	m_stateRegs.exec.hi.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "exec_hi", m_module.constu32(0));
}

void GCNCompiler::emitDclShaderResource(const GcnShaderResourceInstance& res)
{
	switch (res.usageType)
	{
	case kShaderInputUsageImmConstBuffer:
		emitDclImmConstBuffer(res);
		break;
	case kShaderInputUsageImmResource:
	case kShaderInputUsageImmRwResource:
		emitDclImmResource(res);
		break;
	case kShaderInputUsageImmSampler:
		emitDclImmSampler(res);
		break;
	case kShaderInputUsageImmVertexBuffer:
		// just used to pass warning
		break;
	default:
		LOG_WARN("unknown shader resource type found %d", res.usageType);
		break;
	}
}

void GCNCompiler::emitDclShaderResourceUD()
{
	// Declare resources in User Data.
	for (const auto& res : m_shaderInput.shaderResources.ud)
	{
		emitDclShaderResource(res);
	}
}

void GCNCompiler::emitDclShaderResourceEUD(uint32_t dstRegIndex, uint32_t eudOffsetDw)
{
	const auto& eudResources = m_shaderInput.shaderResources.eud->resources;

	// Find the matched resource at eudOffsetDw in EUD table.
	auto iter = std::find_if(eudResources.begin(), eudResources.end(),
							 [eudOffsetDw](const auto& resPair) {
								 return resPair.first == eudOffsetDw;
							 });

	// Declare the found resource.
	auto res = iter->second;
	// Reset startRegister to S_LOAD_DWORDXN instruction's destination register.
	// Following progress will use this to set resource in proper slot.
	res.res.startRegister = dstRegIndex;
	emitDclShaderResource(res);
}

void GCNCompiler::emitDclImmConstBuffer(const GcnShaderResourceInstance& res)
{
	// For PSSL resource buffer, it's hard to detect how many variables have been declared,
	// and even if we know, it's almost useless, because the shader could access part of a variable,
	// like the upper-left mat3x3 of a mat4x4, thus can't be accessed via AccessChain.
	// So here we treat all the uniform buffer together as a dword array.
	// Then we can access any element in this array.

	// Based on the above, there're at least 2 ways to achieve this.
	// First is UBO and second is SSBO.
	//
	// 1. For UBO, the disadvantage is that we can not declare a variable-length array of UBO member,
	// thus we have to get this information from stride field of input V# buffer,
	// which make things a little more complicated.
	// But what we gain is performance, UBO access is usually faster than SSBO.
	//
	// 2. For SSBO, the advantage is that it support variable-length arrays, which will make
	// the compiler implementation a little easier.
	// Besides, SSBO support write to the buffer,
	// The disadvantage is that it will slower than UBO.
	//
	// Currently I can not determine which one is better, and how much performance we could gain from using UBO,
	// but I just choose the UBO way first due to performance reason. Maybe need to change in the future.

	const VSharpBuffer* vsharpBuffer = reinterpret_cast<const VSharpBuffer*>(res.res.resource);
	// TODO:
	// For constant buffer size, here I take the runtime value directly,
	// but in fact, the value could be changed during runtime theoretically.
	// We should use shader specialization constants instead in the future.
	uint32_t bufferSize = vsharpBuffer->stride * vsharpBuffer->num_records;
	uint32_t arraySize  = bufferSize / sizeof(uint32_t);

	uint32_t arrayId = m_module.defArrayTypeUnique(
		getScalarTypeId(SpirvScalarType::Uint32),
		m_module.constu32(arraySize));

	// It seems the official glsl compiler(glslangValidator.exe) always
	// set the ArrayStride to 16 no matter what type the element is.
	// This is std140 standard, but what we should use is std430
	// We should specify the correct stride, for a float array, it's sizeof(float) == 4 .
	// This will trigger a validation warning.
	m_module.decorateArrayStride(arrayId, 4);

	// spirv-cross doesn't support buffer block expressed as any of std430, std140 and etc.
	// to use spirv-cross to view the output spv file, enable this and disable above line.
	// m_module.decorateArrayStride(arrayId, 16);

	uint32_t uboStuctId = m_module.defStructTypeUnique(1, &arrayId);
	m_module.decorateBlock(uboStuctId);
	m_module.memberDecorateOffset(uboStuctId, 0, 0);
	m_module.setDebugName(uboStuctId, "UniformBufferObject");
	m_module.setDebugMemberName(uboStuctId, 0, "data");

	uint32_t uboPtrId = m_module.defPointerType(uboStuctId, spv::StorageClassUniform);
	uint32_t uboId    = m_module.newVar(uboPtrId, spv::StorageClassUniform);

	m_module.decorateDescriptorSet(uboId, 0);

	// Note:
	// The calculated bindingId is not "correct", it's a dummy value.
	// We'll remap binding id before compiling pipeline in VltShader class.
	uint32_t bindingId = computeConstantBufferBinding(m_programInfo.shaderType(), res.res.startRegister);
	m_module.decorateBinding(uboId, bindingId);

	m_module.setDebugName(uboId, "ubo");

	SpirvConstantBuffer constBuffer;
	constBuffer.varId                           = uboId;
	constBuffer.size                            = bufferSize;
	m_constantBuffers.at(res.res.startRegister) = constBuffer;

	m_resourceSlots.push_back({ bindingId, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER });
}

void GCNCompiler::emitDclImmSampler(const GcnShaderResourceInstance& res)
{
	// The sampler start register
	const uint32_t samplerId = res.res.startRegister;

	//const SSharpBuffer* ssharpBuffer = reinterpret_cast<SSharpBuffer*>(res.res.resource);

	const uint32_t samplerType = m_module.defSamplerType();
	// The sampler type is opaque, but we still have to
	// define a pointer and a variable in oder to use it
	const uint32_t samplerPtrType = m_module.defPointerType(
		samplerType, spv::StorageClassUniformConstant);

	// Define the sampler variable
	const uint32_t varId = m_module.newVar(samplerPtrType,
										   spv::StorageClassUniformConstant);
	m_module.setDebugName(varId,
						  str::format("sampler%d", samplerId).c_str());

	m_module.decorateDescriptorSet(varId, 0);

	uint32_t bindingId = computeSamplerBinding(m_programInfo.shaderType(), res.res.startRegister);
	m_module.decorateBinding(varId, bindingId);

	SpirvSampler sampler;
	sampler.varId            = varId;
	sampler.typeId           = samplerType;
	m_samplers.at(samplerId) = sampler;

	m_resourceSlots.push_back({ bindingId, VK_DESCRIPTOR_TYPE_SAMPLER });
}

void GCNCompiler::emitDclImmBuffer(const GcnShaderResourceInstance& res)
{
	const VSharpBuffer* vsharpBuffer = reinterpret_cast<const VSharpBuffer*>(res.res.resource);

	uint32_t elemType   = getScalarTypeId(SpirvScalarType::Uint32);
	uint32_t arrayType  = m_module.defRuntimeArrayTypeUnique(elemType);
	uint32_t structType = m_module.defStructTypeUnique(1, &arrayType);
	uint32_t ptrType    = m_module.defPointerType(structType, spv::StorageClassUniform);

	uint32_t resTypeId = m_module.defPointerType(elemType, spv::StorageClassUniform);
	uint32_t varId     = m_module.newVar(ptrType, spv::StorageClassUniform);

	m_module.decorateArrayStride(arrayType, sizeof(uint32_t));
	m_module.decorate(structType, spv::DecorationBufferBlock);
	m_module.memberDecorateOffset(structType, 0, 0);

	m_module.setDebugName(structType, "StorageBufferObject");
	m_module.setDebugMemberName(structType, 0, "data");

	// TODO:
	// Detect if the buffer is writable
	//if (!isUav)
	//{
	//	m_module.decorate(varId, spv::DecorationNonWritable);
	//}

	// Note:
	// The calculated bindingId is not "correct", it's a dummy value.
	// We'll remap binding id before compiling pipeline in VltShader class.
	uint32_t bindingId = computeResBinding(m_programInfo.shaderType(), res.res.startRegister);

	m_module.decorateDescriptorSet(varId, 0);
	m_module.decorateBinding(varId, bindingId);

	m_module.setDebugName(varId, "sbo");

	SpirvRegularBuffer regularBuffer;
	regularBuffer.varId                        = varId;
	m_regularBuffers.at(res.res.startRegister) = regularBuffer;

	m_resourceSlots.push_back({ bindingId, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER });
}

void GCNCompiler::emitDclImmTexture(const GcnShaderResourceInstance& res)
{
	const uint32_t registerId = res.res.startRegister;

	const TSharpBuffer* tsharpBuffer = reinterpret_cast<const TSharpBuffer*>(res.res.resource);

	// TODO:
	// We should define the type info according to tsharpBuffer
	SpirvImageInfo typeInfo;
	typeInfo.format  = spv::ImageFormatUnknown;
	typeInfo.dim     = spv::Dim2D;
	typeInfo.array   = 0;
	typeInfo.ms      = 0;
	typeInfo.sampled = 1;

	const uint32_t sampledTypeId = getScalarTypeId(SpirvScalarType::Float32);
	const uint32_t imageTypeId   = m_module.defImageType(sampledTypeId,
                                                       typeInfo.dim, 0, typeInfo.array, typeInfo.ms, typeInfo.sampled,
                                                       typeInfo.format);

	const uint32_t resourcePtrType = m_module.defPointerType(
		imageTypeId, spv::StorageClassUniformConstant);

	const uint32_t varId = m_module.newVar(resourcePtrType,
										   spv::StorageClassUniformConstant);

	m_module.setDebugName(varId,
						  str::format("texture%d", registerId).c_str());

	m_module.decorateDescriptorSet(varId, 0);

	uint32_t bindingId = computeResBinding(m_programInfo.shaderType(), res.res.startRegister);
	m_module.decorateBinding(varId, bindingId);

	SpirvTexture texture;
	texture.imageInfo         = typeInfo;
	texture.varId             = varId;
	texture.imageTypeId       = imageTypeId;
	m_textures.at(registerId) = texture;

	m_resourceSlots.push_back({ bindingId, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE });
}

void GCNCompiler::emitDclImmResource(const GcnShaderResourceInstance& res)
{
	if (res.sharpType == PsslSharpType::TSharp)
	{
		emitDclImmTexture(res);
	}
	else
	{
		emitDclImmBuffer(res);
	}
}

void GCNCompiler::emitDclThreadGroup()
{
	m_cs.workgroupSizeX = m_shaderInput.meta.cs.threadGroupX;
	m_cs.workgroupSizeY = m_shaderInput.meta.cs.threadGroupY;
	m_cs.workgroupSizeZ = m_shaderInput.meta.cs.threadGroupZ;

	m_module.setLocalSize(m_entryPointId,
						  m_cs.workgroupSizeX,
						  m_cs.workgroupSizeY,
						  m_cs.workgroupSizeZ);
}

void GCNCompiler::emitDclThreadGroupSharedMemory()
{
	// Declare LDS data.

	// We use uint array to represent LDS data.
	const uint32_t elementStride = sizeof(uint32_t);
	const uint32_t elementCount  = m_shaderInput.meta.cs.ldsSize / elementStride;

	SpirvRegisterInfo varInfo;
	varInfo.atype.vtype.ctype  = SpirvScalarType::Uint32;
	varInfo.atype.vtype.ccount = 1;
	varInfo.atype.alength     = elementCount;
	varInfo.sclass           = spv::StorageClassWorkgroup;

	m_cs.lds.type = varInfo.atype.vtype;
	m_cs.lds.id   = emitNewVariable(varInfo);
	
	m_module.setDebugName(m_cs.lds.id, "lds");
}

SpirvRegisterValue GCNCompiler::emitVsSystemValueLoad(
	SpirvSystemValue sv,
	GcnRegMask       mask)
{
	SpirvRegisterValue result;
	switch (sv)
	{
	case SpirvSystemValue::VertexId:
	{
		const uint32_t typeId = getScalarTypeId(SpirvScalarType::Uint32);

		if (m_vs.builtinVertexId == 0)
		{
			m_vs.builtinVertexId = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInVertexIndex,
				"gl_VertexIndex");
		}

		if (m_vs.builtinBaseVertex == 0)
		{
			m_vs.builtinBaseVertex = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInBaseVertex,
				"gl_BaseVertex");
		}

		result.type.ctype  = SpirvScalarType::Uint32;
		result.type.ccount = 1;
		result.id          = m_module.opISub(typeId,
                                    m_module.opLoad(typeId, m_vs.builtinVertexId),
                                    m_module.opLoad(typeId, m_vs.builtinBaseVertex));
	}
	break;
	case SpirvSystemValue::InstanceId:
	{
		const uint32_t typeId = getScalarTypeId(SpirvScalarType::Uint32);

		if (m_vs.builtinInstanceId == 0)
		{
			m_vs.builtinInstanceId = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInInstanceIndex,
				"gl_InstanceID");
		}

		if (m_vs.builtinBaseInstance == 0)
		{
			m_vs.builtinBaseInstance = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInBaseInstance,
				"gl_BaseInstance");
		}

		result.type.ctype  = SpirvScalarType::Uint32;
		result.type.ccount = 1;
		result.id          = m_module.opISub(typeId,
                                    m_module.opLoad(typeId, m_vs.builtinInstanceId),
                                    m_module.opLoad(typeId, m_vs.builtinBaseInstance));
	}
	break;
	default:
		LOG_ASSERT(false, "Unhandled VS SV input: ", sv);
		break;
	}
	return result;
}

SpirvRegisterValue GCNCompiler::emitPsSystemValueLoad(
	SpirvSystemValue sv,
	GcnRegMask       mask)
{
	SpirvRegisterValue result;
	switch (sv)
	{
	case SpirvSystemValue::Position:
	{
		if (m_ps.builtinFragCoord == 0)
		{
			m_ps.builtinFragCoord = emitNewBuiltinVariable(
				{ { SpirvScalarType::Float32, 4, 0 },
				  spv::StorageClassInput },
				spv::BuiltInFragCoord,
				"gl_FragCoord");
		}

		SpirvRegisterPointer ptrIn;
		ptrIn.type = { SpirvScalarType::Float32, 4 };
		ptrIn.id   = m_ps.builtinFragCoord;

		// The X, Y and Z components of the S_POSITION semantic
		// are identical to Vulkan's FragCoord builtin, but we
		// need to compute the reciprocal of the W component.
		SpirvRegisterValue fragCoord = emitValueLoad(ptrIn);

		uint32_t componentIndex = 3;
		uint32_t t_f32          = m_module.defFloatType(32);
		uint32_t v_wComp        = m_module.opCompositeExtract(t_f32, fragCoord.id, 1, &componentIndex);
		v_wComp                 = m_module.opFDiv(t_f32, m_module.constf32(1.0f), v_wComp);

		fragCoord.id = m_module.opCompositeInsert(
			getVectorTypeId(fragCoord.type),
			v_wComp, fragCoord.id,
			1, &componentIndex);

		result = emitRegisterExtract(fragCoord, mask);
	}
	break;

	case SpirvSystemValue::IsFrontFace:
	{
		if (m_ps.builtinIsFrontFace == 0)
		{
			m_ps.builtinIsFrontFace = emitNewBuiltinVariable(
				{ { SpirvScalarType::Bool, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInFrontFacing,
				"gl_FrontFacing");
		}

		result.type.ctype  = SpirvScalarType::Uint32;
		result.type.ccount = 1;
		result.id          = m_module.opSelect(
            getVectorTypeId(result.type),
            m_module.opLoad(
                m_module.defBoolType(),
                m_ps.builtinIsFrontFace),
            m_module.constu32(0xFFFFFFFF),
            m_module.constu32(0x00000000));
	}
	break;

	case SpirvSystemValue::PrimitiveId:
	{
		if (m_primitiveIdIn == 0)
		{
			m_module.enableCapability(spv::CapabilityGeometry);

			m_primitiveIdIn = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInPrimitiveId,
				"gl_PrimitiveID");
		}

		SpirvRegisterPointer ptrIn;
		ptrIn.type = { SpirvScalarType::Uint32, 1 };
		ptrIn.id   = m_primitiveIdIn;

		result = emitValueLoad(ptrIn);
	}
	break;

	case SpirvSystemValue::SampleIndex:
	{
		if (m_ps.builtinSampleId == 0)
		{
			m_module.enableCapability(spv::CapabilitySampleRateShading);

			m_ps.builtinSampleId = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInSampleId,
				"gl_SampleID");
		}

		SpirvRegisterPointer ptrIn;
		ptrIn.type.ctype  = SpirvScalarType::Uint32;
		ptrIn.type.ccount = 1;
		ptrIn.id          = m_ps.builtinSampleId;

		result = emitValueLoad(ptrIn);
	}
	break;

	case SpirvSystemValue::RenderTargetId:
	{
		if (m_ps.builtinLayer == 0)
		{
			m_module.enableCapability(spv::CapabilityGeometry);

			m_ps.builtinLayer = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInLayer,
				"gl_Layer");
		}

		SpirvRegisterPointer ptr;
		ptr.type.ctype  = SpirvScalarType::Uint32;
		ptr.type.ccount = 1;
		ptr.id          = m_ps.builtinLayer;

		result = emitValueLoad(ptr);
	}
	break;

	case SpirvSystemValue::ViewportId:
	{
		if (m_ps.builtinViewportId == 0)
		{
			m_module.enableCapability(spv::CapabilityMultiViewport);

			m_ps.builtinViewportId = emitNewBuiltinVariable(
				{ { SpirvScalarType::Uint32, 1, 0 },
				  spv::StorageClassInput },
				spv::BuiltInViewportIndex,
				"gl_ViewportIndex");
		}

		SpirvRegisterPointer ptr;
		ptr.type.ctype  = SpirvScalarType::Uint32;
		ptr.type.ccount = 1;
		ptr.id          = m_ps.builtinViewportId;

		result = emitValueLoad(ptr);
	}
	break;

	default:
		LOG_ASSERT(false, "Unhandled PS SV input: ", sv);
		break;
	}
	return result;
}

SpirvRegisterValue GCNCompiler::emitCsSystemValueLoad(
	SpirvSystemValue sv,
	GcnRegMask       mask)
{
	SpirvRegisterValue result;

	switch (sv)
	{
	case SpirvSystemValue::ThreadId:
	{
		if (m_cs.builtinGlobalInvocationId == 0)
		{
			m_cs.builtinGlobalInvocationId = 
				emitNewBuiltinVariable({ { SpirvScalarType::Uint32, 3, 0 },
										 spv::StorageClassInput },
									   spv::BuiltInGlobalInvocationId,
									   "gl_GlobalInvocationID");
		}

		SpirvRegisterPointer ptr;
		ptr.type.ctype  = SpirvScalarType::Uint32;
		ptr.type.ccount = 3;
		ptr.id          = m_cs.builtinGlobalInvocationId;
		result          = emitValueLoad(ptr);
	}
		break;
	case SpirvSystemValue::ThreadGroupId:
	{
		if (m_cs.builtinWorkgroupId == 0)
		{
			m_cs.builtinWorkgroupId =
				emitNewBuiltinVariable({ { SpirvScalarType::Uint32, 3, 0 },
										 spv::StorageClassInput },
									   spv::BuiltInWorkgroupId,
									   "gl_WorkGroupID");
		}

		SpirvRegisterPointer ptr;
		ptr.type.ctype  = SpirvScalarType::Uint32;
		ptr.type.ccount = 3;
		ptr.id          = m_cs.builtinWorkgroupId;
		result          = emitValueLoad(ptr);
	}
		break;
	case SpirvSystemValue::ThreadIdInGroup:
	{
		if (m_cs.builtinLocalInvocationId == 0)
		{
			m_cs.builtinLocalInvocationId = 
				emitNewBuiltinVariable({ { SpirvScalarType::Uint32, 3, 0 },
										 spv::StorageClassInput },
									   spv::BuiltInLocalInvocationId,
									   "gl_LocalInvocationID");
		}

		SpirvRegisterPointer ptr;
		ptr.type.ctype  = SpirvScalarType::Uint32;
		ptr.type.ccount = 3;
		ptr.id          = m_cs.builtinLocalInvocationId;
		result          = emitValueLoad(ptr);
	}
		break;
	case SpirvSystemValue::ThreadIndexInGroup:
	{
		if (m_cs.builtinLocalInvocationIndex == 0)
		{
			m_cs.builtinLocalInvocationIndex =
				emitNewBuiltinVariable({ { SpirvScalarType::Uint32, 1, 0 },
										 spv::StorageClassInput },
									   spv::BuiltInLocalInvocationIndex,
									   "gl_LocalInvocationIndex");
		}

		SpirvRegisterPointer ptr;
		ptr.type.ctype  = SpirvScalarType::Uint32;
		ptr.type.ccount = 1;
		ptr.id          = m_cs.builtinLocalInvocationIndex;
		result          = emitValueLoad(ptr);
	}
		break;
	default:
		LOG_ASSERT(false, "Unhandled CS SV input: ", sv);
		break;
	}

	return result;
}

SpirvRegisterValue GCNCompiler::emitValueLoad(const SpirvRegisterPointer& reg)
{
	uint32_t varId = m_module.opLoad(
		getVectorTypeId(reg.type),
		reg.id);
	return SpirvRegisterValue(reg.type, varId);
}

template <SpirvGprType GprType>
SpirvRegisterValue GCNCompiler::emitGprLoad(
	uint32_t        index,
	SpirvScalarType dstType)
{
	SpirvRegisterValue result;
	do
	{
		LOG_ASSERT(dstType != SpirvScalarType::Unknown, "shouldn't load unknown type.");
		LOG_ASSERT(!isDoubleWordType(dstType), "shouldn't load double word type here.");

		auto& gpr = GprType == SpirvGprType::Scalar ? m_sgprs[index] : m_vgprs[index];

		if (gpr.id == InvalidSpvId)
		{
			// Some instructions will specify 0 as one of their SRCs,
			// here 0 can be treated as both s0/v0 and an empty src.
			// An empty src means the instruction doesn't use this src, e.g. SRC2 for v3_mac_f32.
			// In such case, we shouldn't do anything.
			LOG_WARN("try to load uninitialized gpr %s%d",
					 GprType == SpirvGprType::Scalar ? "s" : "v", index);
			break;
		}

		result = emitValueLoad(gpr);

		if (dstType != SpirvScalarType::Float32 || 
			gpr.type.ctype != SpirvScalarType::Float32)
		{
			result = emitRegisterBitcast(result, dstType);
		}

	} while (false);
	return result;
}

void GCNCompiler::emitValueStore(
	const SpirvRegisterPointer& ptr,
	const SpirvRegisterValue&   src,
	const GcnRegMask&           writeMask)
{
	SpirvRegisterValue value = src;
	// If the component types are not compatible,
	// we need to bit-cast the source variable.
	if (src.type.ctype != ptr.type.ctype)
	{
		value = emitRegisterBitcast(value, ptr.type.ctype);
	}

	// If the source value consists of only one component,
	// it is stored in all components of the destination.
	if (value.type.ccount == 1)
	{
		value = emitRegisterExtend(value, writeMask.popCount());
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
		tmp                    = emitRegisterInsert(tmp, value, writeMask);

		m_module.opStore(ptr.id, tmp.id);
	}
}

template <SpirvGprType GprType>
void GCNCompiler::emitGprStore(
	uint32_t                  index,
	const SpirvRegisterValue& value)
{
	auto& gpr = GprType == SpirvGprType::Scalar ? m_sgprs[index] : m_vgprs[index];

	if (gpr.id == InvalidSpvId)  // Not initialized
	{
		// We use float variable to hold all registers.
		gpr = emitGprCreate<GprType>(index, SpirvScalarType::Float32);
	}

	SpirvRegisterValue srcVal = value;
	if (value.type.ctype != SpirvScalarType::Float32)
	{
		srcVal = emitRegisterBitcast(value, SpirvScalarType::Float32);
	}

	emitValueStore(gpr, srcVal, GcnRegMask::select(0));
}

SpirvRegisterPointer GCNCompiler::emitVgprCreate(
	uint32_t                index,
	SpirvScalarType         type,
	std::optional<uint32_t> initId)
{
	return emitGprCreate<SpirvGprType::Vector>(index, type, initId);
}

SpirvRegisterPointer GCNCompiler::emitSgprCreate(
	uint32_t                index,
	SpirvScalarType         type,
	std::optional<uint32_t> initId)
{
	return emitGprCreate<SpirvGprType::Scalar>(index, type, initId);
}

template <SpirvGprType GprType>
SpirvRegisterPointer pssl::GCNCompiler::emitGprCreate(
	uint32_t                index,
	SpirvScalarType         type,
	std::optional<uint32_t> initId)
{
	SpirvRegisterPointer result;
	result.type.ctype  = type;
	result.type.ccount = 1;

	const char* fmtString =
		GprType == SpirvGprType::Scalar ? "s%d" : "v%d";

	result.id = emitNewVariable(
		{ result.type, spv::StorageClassPrivate },
		str::format(fmtString, index));

	if (initId.has_value())
	{
		m_module.opStore(result.id, initId.value());
	}

	return result;
}

void GCNCompiler::emitSgprArrayStore(uint32_t                  startIdx,
									 const SpirvRegisterValue* values,
									 uint32_t                  count)
{
	for (uint32_t i = 0; i != count; ++i)
	{
		emitGprStore<SpirvGprType::Scalar>(startIdx + i, values[i]);
	}
}

void GCNCompiler::emitVgprArrayStore(uint32_t                  startIdx,
									 const SpirvRegisterValue* values,
									 uint32_t                  count)
{
	for (uint32_t i = 0; i != count; ++i)
	{
		emitGprStore<SpirvGprType::Vector>(startIdx + i, values[i]);
	}
}

void GCNCompiler::emitVgprVectorStore(
	uint32_t                  startIdx,
	const SpirvRegisterValue& srcVec,
	const GcnRegMask&         writeMask)
{
	uint32_t componentCount = writeMask.popCount();
	uint32_t fpTypeId       = getScalarTypeId(SpirvScalarType::Float32);
	for (uint32_t i = 0; i != componentCount; ++i)
	{
		if (!writeMask[i])
		{
			continue;
		}

		SpirvRegisterValue value;
		value.type.ctype  = SpirvScalarType::Float32;
		value.type.ccount = 1;
		value.id          = m_module.opCompositeExtract(fpTypeId, srcVec.id, 1, &i);

		emitGprStore<SpirvGprType::Vector>(startIdx + i, value);
	}
}

SpirvRegisterValue GCNCompiler::emitSgprPairLoad(uint32_t firstIndex)
{
	// The type of a SGPR pair will be only Uint64

	// Load the higher and lower sgpr
	auto high = emitGprLoad<SpirvGprType::Scalar>(firstIndex, SpirvScalarType::Uint32);
	auto low  = emitGprLoad<SpirvGprType::Scalar>(firstIndex + 1, SpirvScalarType::Uint32);

	// Convert the two Uint32 to a vec2
	auto vec2U32 = emitRegisterConcat(high, low);

	// Bitcast to Uint64
	return emitRegisterBitcast(vec2U32, SpirvScalarType::Uint64);
}

void GCNCompiler::emitSgprPairStore(uint32_t firstIndex, const SpirvRegisterValue& srcReg)
{
	LOG_ASSERT(srcReg.type.ctype == SpirvScalarType::Uint64,
			   "src register to store to a SGPR pair must be Uint64");

	// Bitcast to vec2
	auto vec2U32 = emitRegisterBitcast(srcReg, SpirvScalarType::Uint32);

	SpirvRegisterValue high;
	SpirvRegisterValue low;
	high.type.ctype  = SpirvScalarType::Uint32;
	high.type.ccount = 1;
	low.type         = high.type;

	const uint32_t u32TypeId = getVectorTypeId(high.type);

	// Access high and low part of the Uint64
	uint32_t index = 0;
	low.id         = m_module.opCompositeExtract(u32TypeId, vec2U32.id, 1, &index);

	index   = 1;
	high.id = m_module.opCompositeExtract(u32TypeId, vec2U32.id, 1, &index);

	// Store
	emitGprStore<SpirvGprType::Scalar>(firstIndex, high);
	emitGprStore<SpirvGprType::Scalar>(firstIndex + 1, low);
}

SpirvRegisterValue GCNCompiler::emitLiteralConstLoad(uint32_t value, SpirvScalarType dstType)
{
	// For mov instructions, there's no type specified (Unknown)
	// in such case we use Uint32
	if (dstType == SpirvScalarType::Unknown)
	{
		dstType = SpirvScalarType::Uint32;
	}

	SpirvRegisterValue result;
	result.type.ctype  = dstType;
	result.type.ccount = 1;
	switch (dstType)
	{
	case SpirvScalarType::Uint32:
		result.id = m_module.constu32(value);
		break;
	case SpirvScalarType::Sint32:
		result.id = m_module.consti32(*reinterpret_cast<int32_t*>(&value));
		break;
	case SpirvScalarType::Float32:
		result.id = m_module.constf32(*reinterpret_cast<float*>(&value));
		break;
	default:
		LOG_ERR("wrong literal const type %d", dstType);
		break;
	}
	return result;
}

SpirvRegisterValue GCNCompiler::emitStateRegisterLoad(
	const GcnStateRegister& reg,
	SpirvScalarType         dstType,
	const std::string&      name /*= ""*/)
{
	SpirvRegisterValue result;
	if (isDoubleWordType(dstType))
	{
		result = reg.load(m_module);
		m_module.setDebugName(result.id, name.c_str());
	}
	else
	{
		result = emitValueLoad(reg.lo);
		result = emitRegisterBitcast(result, dstType);
	}
	return result;
}

void GCNCompiler::emitStateRegisterStore(
	const GcnStateRegister&   reg,
	const SpirvRegisterValue& src)
{
	SpirvRegisterValue value = src;
	if (src.type.ctype == SpirvScalarType::Sint32 ||
		src.type.ctype == SpirvScalarType::Sint64)
	{
		value = emitRegisterBitcast(
			value, 
			src.type.ctype == SpirvScalarType::Sint32 ? 
			SpirvScalarType::Uint32 : SpirvScalarType::Uint64);
	}

	LOG_ASSERT(value.type.ccount == 1 &&
				   (value.type.ctype == SpirvScalarType::Uint32 ||
					value.type.ctype == SpirvScalarType::Uint64),
			   "value store to state regeister should be either Uint32 or Uint64.");

	if (value.type.ctype == SpirvScalarType::Uint32)
	{
		emitValueStore(reg.lo, value, 1);
	}
	else
	{
		reg.store(m_module, value);
	}
}

// Used with with 7 bits SDST, 8 bits SSRC or 9 bits SRC
// See table "SDST, SSRC and SRC Operands" in section 3.1 of GPU Shader Core ISA manual
SpirvRegisterValue GCNCompiler::emitLoadScalarOperand(
	uint32_t srcOperand, uint32_t regIndex, SpirvScalarType dstType, uint32_t literalConst /*= 0*/)
{
	Instruction::OperandSRC src = static_cast<Instruction::OperandSRC>(srcOperand);
	SpirvRegisterValue      operand;

	switch (src)
	{
	case Instruction::OperandSRC::SRCScalarGPRMin... Instruction::OperandSRC::SRCScalarGPRMax:
		operand = !isDoubleWordType(dstType) ?
			emitGprLoad<SpirvGprType::Scalar>(regIndex, dstType) : 
			emitSgprPairLoad(regIndex);
		break;
	case Instruction::OperandSRC::SRCVccLo:
		operand = emitStateRegisterLoad(m_stateRegs.vcc, dstType, "vcc");
		break;
	case Instruction::OperandSRC::SRCVccHi:
		operand = emitStateRegisterLoad(m_stateRegs.vcc, dstType);
		break;
	case Instruction::OperandSRC::SRCM0:
		operand = emitValueLoad(m_stateRegs.m0);
		break;
	case Instruction::OperandSRC::SRCExecLo:
		operand = emitStateRegisterLoad(m_stateRegs.exec, dstType, "exec");
		break;
	case Instruction::OperandSRC::SRCExecHi:
		operand = emitStateRegisterLoad(m_stateRegs.exec, dstType);
		break;
	case Instruction::OperandSRC::SRCConstZero:
	case Instruction::OperandSRC::SRCSignedConstIntPosMin... Instruction::OperandSRC::SRCSignedConstIntPosMax:
	case Instruction::OperandSRC::SRCSignedConstIntNegMin... Instruction::OperandSRC::SRCSignedConstIntNegMax:
		operand = emitInlineConstantInteger(src, regIndex, dstType);
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_0_5... Instruction::OperandSRC::SRCConstFloatNeg_4_0:
		operand = emitInlineConstantFloat(src);
		break;
	case Instruction::OperandSRC::SRCVCCZ:
		break;
	case Instruction::OperandSRC::SRCEXECZ:
		break;
	case Instruction::OperandSRC::SRCSCC:
		//operand = emitValueLoad(m_statusRegs.scc);
		LOG_ASSERT(false, "scc load not supported yet.");
		break;
	case Instruction::OperandSRC::SRCLdsDirect:
		break;
	case Instruction::OperandSRC::SRCLiteralConst:
		operand = emitLiteralConstLoad(literalConst, dstType);
		break;
	// For 9 bits SRC operand
	case Instruction::OperandSRC::SRCVectorGPRMin... Instruction::OperandSRC::SRCVectorGPRMax:
		operand = emitGprLoad<SpirvGprType::Vector>(regIndex, dstType);
		break;
	default:
		LOG_ERR("error operand range %d", (uint32_t)srcOperand);
		break;
	}

	return operand;
}

// Used with 8 bits VSRC/VDST
// for 9 bits SRC, call emitLoadScalarOperand instead
// See table "VSRC and VDST Operands" in section 3.1 of GPU Shader Core ISA manual
SpirvRegisterValue GCNCompiler::emitLoadVectorOperand(uint32_t index, SpirvScalarType dstType)
{
	LOG_ASSERT(!isDoubleWordType(dstType), "only support 32 bits VGPR load");
	return emitGprLoad<SpirvGprType::Vector>(index, dstType);
}

// Used with 7 bits SDST
void GCNCompiler::emitStoreScalarOperand(uint32_t dstOperand, uint32_t regIndex, const SpirvRegisterValue& srcReg)
{
	Instruction::OperandSDST dst = static_cast<Instruction::OperandSDST>(dstOperand);

	switch (dst)
	{
	case Instruction::OperandSDST::SDSTScalarGPRMin... Instruction::OperandSDST::SDSTScalarGPRMax:
	{
		srcReg.type.ctype != SpirvScalarType::Uint64 ?
			emitGprStore<SpirvGprType::Scalar>(regIndex, srcReg) :
			emitSgprPairStore(regIndex, srcReg);
	}
	break;
	case Instruction::OperandSDST::SDSTVccLo:
		emitStateRegisterStore(m_stateRegs.vcc, srcReg);
		break;
	case Instruction::OperandSDST::SDSTVccHi:
		emitValueStore(m_stateRegs.vcc.hi, srcReg, 1);
		break;
	case Instruction::OperandSDST::SDSTExecLo:
		emitStateRegisterStore(m_stateRegs.exec, srcReg);
		break;
	case Instruction::OperandSDST::SDSTExecHi:
		emitValueStore(m_stateRegs.exec.hi, srcReg, 1);
		break;
	case Instruction::OperandSDST::SDSTM0:
		emitValueStore(m_stateRegs.m0, srcReg, 1);
		break;
	default:
		LOG_ERR("error operand range %d", (uint32_t)dst);
		break;
	}
}

// Used with 8 bits VSRC/VDST
// for 9 bits SRC, call emitLoadScalarOperand instead
// See table "VSRC and VDST Operands" in section 3.1 of GPU Shader Core ISA manual
void GCNCompiler::emitStoreVectorOperand(uint32_t dstIndex, const SpirvRegisterValue& srcReg)
{
	emitGprStore<SpirvGprType::Vector>(dstIndex, srcReg);
}

SpirvRegisterValue GCNCompiler::emitInlineConstantFloat(Instruction::OperandSRC src)
{
	float value = 0.0;
	switch (src)
	{
	case Instruction::OperandSRC::SRCConstFloatPos_0_5:
		value = 0.5;
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_0_5:
		value = -0.5;
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_1_0:
		value = 1.0;
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_1_0:
		value = -1.0;
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_2_0:
		value = 2.0;
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_2_0:
		value = -2.0;
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_4_0:
		value = 4.0;
		break;
	case Instruction::OperandSRC::SRCConstFloatNeg_4_0:
		value = -4.0;
		break;
	}

	uint32_t valueId = m_module.constf32(value);
	return SpirvRegisterValue(SpirvScalarType::Float32, 1, valueId);
}

SpirvRegisterValue GCNCompiler::emitInlineConstantInteger(
	Instruction::OperandSRC src,
	uint32_t                regIndex,
	SpirvScalarType         dstType)
{
	int32_t value = 0;
	switch (src)
	{
	case Instruction::OperandSRC::SRCConstZero:
		// Sometimes 0 is used in float point instructions, like V_MAX_F32
		// After translate to spirv, this will raise a validation layer error.
		// This could be safely ignored.
		value = 0;
		break;
	case Instruction::OperandSRC::SRCSignedConstIntPosMin... Instruction::OperandSRC::SRCSignedConstIntPosMax:
		value = (int32_t)regIndex - 128;
		break;
	case Instruction::OperandSRC::SRCSignedConstIntNegMin... Instruction::OperandSRC::SRCSignedConstIntNegMax:
		value = 192 - (int32_t)regIndex;
		break;
	default:
		break;
	}

	LOG_ASSERT(dstType != SpirvScalarType::Unknown, "do not support unknown type.");
	//uint32_t valueId = InvalidSpvId;
	//switch (dstType)
	//{
	//case pssl::SpirvScalarType::Uint32:
	//	valueId = m_module.constu32(value);
	//	break;
	//case pssl::SpirvScalarType::Uint64:
	//	valueId = m_module.constu64(value);
	//	break;
	//case pssl::SpirvScalarType::Sint32:
	//	valueId = m_module.consti32(value);
	//	break;
	//case pssl::SpirvScalarType::Sint64:
	//	valueId = m_module.consti64(value);
	//	break;
	//case pssl::SpirvScalarType::Float32:
	//	valueId = m_module.constf32(value);
	//	break;
	//case pssl::SpirvScalarType::Float64:
	//	valueId = m_module.constf64(value);
	//	break;
	//}
	//return SpirvRegisterValue(dstType, 1, valueId);

	SpirvRegisterValue result;
	result.type.ccount = 1;
	result.type.ctype  = SpirvScalarType::Sint32;
	result.id          = m_module.consti32(value);
	return result;
}

uint32_t GCNCompiler::emitLoadSampledImage(const SpirvTexture& textureResource, const SpirvSampler& samplerResource)
{
	const uint32_t sampledImageType = m_module.defSampledImageType(textureResource.imageTypeId);

	return m_module.opSampledImage(sampledImageType,
								   m_module.opLoad(textureResource.imageTypeId, textureResource.varId),
								   m_module.opLoad(samplerResource.typeId, samplerResource.varId));
}

SpirvRegisterValue GCNCompiler::emitPackFloat16(const SpirvRegisterValue& v2floatVec)
{
	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Uint32;
	result.type.ccount = 1;

	const uint32_t u32Type = getVectorTypeId(result.type);
	result.id              = m_module.opPackHalf2x16(u32Type, v2floatVec.id);
	return result;
}

SpirvRegisterValue GCNCompiler::emitUnpackFloat16(const SpirvRegisterValue& uiVec)
{
	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Float32;
	result.type.ccount = 2;

	const uint32_t v2fpType = getVectorTypeId(result.type);
	result.id               = m_module.opUnpackHalf2x16(v2fpType, uiVec.id);
	return result;
}

uint32_t GCNCompiler::emitNewVariable(
	const SpirvRegisterInfo& info,
	const std::string&       name /* = "" */,
	std::optional<uint32_t>  initValueId /*= std::nullopt*/)
{
	const uint32_t ptrTypeId = getPointerTypeId(info);
	uint32_t       varId     = InvalidSpvId;
	if (!initValueId.has_value())
	{
		varId = m_module.newVar(ptrTypeId, info.sclass);
	}
	else
	{
		varId = m_module.newVarInit(ptrTypeId, info.sclass, initValueId.value());
	}

	if (!name.empty())
	{
		m_module.setDebugName(varId, name.c_str());
	}
	return varId;
}

uint32_t GCNCompiler::emitNewBuiltinVariable(
	const SpirvRegisterInfo& info,
	spv::BuiltIn             builtIn,
	const char*              name)
{
	const uint32_t varId = emitNewVariable(info);

	m_module.setDebugName(varId, name);
	m_module.decorateBuiltIn(varId, builtIn);

	if (m_programInfo.shaderType() == PsslProgramType::PixelShader && 
		info.atype.vtype.ctype != SpirvScalarType::Float32 && 
		info.atype.vtype.ctype != SpirvScalarType::Bool && 
		info.sclass == spv::StorageClassInput)
	{
		m_module.decorate(varId, spv::DecorationFlat);
	}

	m_entryPointInterfaces.push_back(varId);
	return varId;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVecf32(float x, float y, float z, float w, const GcnRegMask& writeMask)
{
	// TODO refactor these functions into one single template
	std::array<uint32_t, 4> ids            = { 0, 0, 0, 0 };
	uint32_t                componentIndex = 0;

	if (writeMask[0])
		ids[componentIndex++] = m_module.constf32(x);
	if (writeMask[1])
		ids[componentIndex++] = m_module.constf32(y);
	if (writeMask[2])
		ids[componentIndex++] = m_module.constf32(z);
	if (writeMask[3])
		ids[componentIndex++] = m_module.constf32(w);

	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Float32;
	result.type.ccount = componentIndex;
	result.id          = componentIndex > 1
					? m_module.constComposite(
						  getVectorTypeId(result.type),
						  componentIndex, ids.data())
					: ids[0];
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVecu32(uint32_t x, uint32_t y, uint32_t z, uint32_t w, const GcnRegMask& writeMask)
{
	std::array<uint32_t, 4> ids            = { 0, 0, 0, 0 };
	uint32_t                componentIndex = 0;

	if (writeMask[0])
		ids[componentIndex++] = m_module.constu32(x);
	if (writeMask[1])
		ids[componentIndex++] = m_module.constu32(y);
	if (writeMask[2])
		ids[componentIndex++] = m_module.constu32(z);
	if (writeMask[3])
		ids[componentIndex++] = m_module.constu32(w);

	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Uint32;
	result.type.ccount = componentIndex;
	result.id          = componentIndex > 1
					? m_module.constComposite(
						  getVectorTypeId(result.type),
						  componentIndex, ids.data())
					: ids[0];
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVeci32(int32_t x, int32_t y, int32_t z, int32_t w, const GcnRegMask& writeMask)
{
	std::array<uint32_t, 4> ids            = { 0, 0, 0, 0 };
	uint32_t                componentIndex = 0;

	if (writeMask[0])
		ids[componentIndex++] = m_module.consti32(x);
	if (writeMask[1])
		ids[componentIndex++] = m_module.consti32(y);
	if (writeMask[2])
		ids[componentIndex++] = m_module.consti32(z);
	if (writeMask[3])
		ids[componentIndex++] = m_module.consti32(w);

	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Sint32;
	result.type.ccount = componentIndex;
	result.id          = componentIndex > 1
					? m_module.constComposite(
						  getVectorTypeId(result.type),
						  componentIndex, ids.data())
					: ids[0];
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVecf64(double xy, double zw, const GcnRegMask& writeMask)
{
	std::array<uint32_t, 2> ids            = { 0, 0 };
	uint32_t                componentIndex = 0;

	if (writeMask[0] && writeMask[1])
		ids[componentIndex++] = m_module.constf64(xy);
	if (writeMask[2] && writeMask[3])
		ids[componentIndex++] = m_module.constf64(zw);

	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Float64;
	result.type.ccount = componentIndex;
	result.id          = componentIndex > 1
					? m_module.constComposite(
						  getVectorTypeId(result.type),
						  componentIndex, ids.data())
					: ids[0];
	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterBitcast(SpirvRegisterValue srcValue, SpirvScalarType dstType)
{
	SpirvScalarType srcType = srcValue.type.ctype;

	if (srcType == dstType)
		return srcValue;

	SpirvRegisterValue result;
	result.type.ctype  = dstType;
	result.type.ccount = srcValue.type.ccount;

	if (isDoubleWordType(srcType))
		result.type.ccount *= 2;
	if (isDoubleWordType(dstType))
		result.type.ccount /= 2;

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
	result.type.ctype  = value.type.ctype;
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

SpirvRegisterValue GCNCompiler::emitRegisterExtract(
	SpirvRegisterValue value,
	GcnRegMask         mask)
{
	return emitRegisterSwizzle(value,
							   GcnRegSwizzle(0, 1, 2, 3), mask);
}

SpirvRegisterValue GCNCompiler::emitRegisterInsert(
	SpirvRegisterValue dstValue,
	SpirvRegisterValue srcValue,
	GcnRegMask         srcMask)
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
		const uint32_t componentId = srcMask.firstComponent();

		result.id = m_module.opCompositeInsert(typeId,
											   srcValue.id, dstValue.id, 1, &componentId);
	}
	else
	{
		// Both arguments are vectors. We can determine which
		// components to take from which vector and use the
		// OpVectorShuffle instruction.
		std::array<uint32_t, 4> components;
		uint32_t                srcComponentId = dstValue.type.ccount;

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

SpirvRegisterValue GCNCompiler::emitRegisterConcat(SpirvRegisterValue value1, SpirvRegisterValue value2)
{
	std::array<uint32_t, 2> ids = { { value1.id, value2.id } };

	SpirvRegisterValue result;
	result.type.ctype  = value1.type.ctype;
	result.type.ccount = value1.type.ccount + value2.type.ccount;
	result.id          = m_module.opCompositeConstruct(
        getVectorTypeId(result.type),
        ids.size(), ids.data());
	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterExtend(SpirvRegisterValue value, uint32_t size)
{
	if (size == 1)
	{
		return value;
	}

	std::array<uint32_t, 4> ids = { {
		value.id,
		value.id,
		value.id,
		value.id,
	} };

	SpirvRegisterValue result;
	result.type.ctype  = value.type.ctype;
	result.type.ccount = size;
	result.id          = m_module.opCompositeConstruct(
        getVectorTypeId(result.type),
        size, ids.data());
	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterAbsolute(SpirvRegisterValue value)
{
	const uint32_t typeId = getVectorTypeId(value.type);

	switch (value.type.ctype)
	{
	case SpirvScalarType::Float32:
		value.id = m_module.opFAbs(typeId, value.id);
		break;
	case SpirvScalarType::Sint32:
		value.id = m_module.opSAbs(typeId, value.id);
		break;
	default:
		LOG_WARN("GCNCompiler: Cannot get absolute value for given type");
	}

	return value;
}

SpirvRegisterValue GCNCompiler::emitRegisterNegate(SpirvRegisterValue value)
{
	const uint32_t typeId = getVectorTypeId(value.type);

	switch (value.type.ctype)
	{
	case SpirvScalarType::Float32:
		value.id = m_module.opFNegate(typeId, value.id);
		break;
	case SpirvScalarType::Float64:
		value.id = m_module.opFNegate(typeId, value.id);
		break;
	case SpirvScalarType::Sint32:
		value.id = m_module.opSNegate(typeId, value.id);
		break;
	case SpirvScalarType::Sint64:
		value.id = m_module.opSNegate(typeId, value.id);
		break;
	default:
		LOG_WARN("GCNCompiler: Cannot negate given type");
	}

	return value;
}

SpirvRegisterValue GCNCompiler::emitRegisterZeroTest(SpirvRegisterValue value, SpirvZeroTest test)
{
	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Bool;
	result.type.ccount = 1;

	LOG_ASSERT(value.type.ctype == SpirvScalarType::Uint32 || value.type.ctype == SpirvScalarType::Uint64,
			   "value to perform zero test should only be Uint32 or Uint64.");

	const uint32_t zeroId = value.type.ctype == SpirvScalarType::Uint32 ? 
		m_module.constu32(0u) : 
		m_module.constu64(0u);
	const uint32_t typeId = getVectorTypeId(result.type);

	result.id = test == SpirvZeroTest::TestZ
					? m_module.opIEqual(typeId, value.id, zeroId)
					: m_module.opINotEqual(typeId, value.id, zeroId);
	return result;
}

SpirvRegisterValue GCNCompiler::emitRegisterMaskBits(SpirvRegisterValue value, uint32_t mask)
{
	SpirvRegisterValue maskVector = emitBuildConstVecu32(
		mask, mask, mask, mask, GcnRegMask::firstN(value.type.ccount));

	SpirvRegisterValue result;
	result.type = value.type;
	result.id   = m_module.opBitwiseAnd(
        getVectorTypeId(result.type),
        value.id, maskVector.id);
	return result;
}


SpirvRegisterPointer GCNCompiler::emitArrayAccess(
	SpirvRegisterPointer pointer,
	spv::StorageClass    sclass,
	uint32_t             index)
{
	uint32_t ptrTypeId = m_module.defPointerType(
		getVectorTypeId(pointer.type), sclass);

	SpirvRegisterPointer result;
	result.type = pointer.type;
	result.id   = m_module.opAccessChain(
        ptrTypeId, pointer.id, 1, &index);
	return result;
}


SpirvRegisterPointer GCNCompiler::emitVectorAccess(
	SpirvRegisterPointer pointer,
	spv::StorageClass    sclass,
	GcnRegMask           mask)
{
	LOG_ASSERT(mask.popCount() == 1, "only one component is supposed.");
	uint32_t composite = m_module.constu32(mask.firstComponent());
	return emitArrayAccess(pointer, sclass, composite);
}

SpirvRegisterValue GCNCompiler::emitVectorLoad(
	SpirvRegisterPointer pointer,
	spv::StorageClass    sclass,
	GcnRegMask           mask)
{
	auto ptr = emitVectorAccess(pointer, sclass, mask);

	SpirvRegisterValue result;
	result.type.ccount = 1;
	result.type.ctype  = pointer.type.ctype;
	result.id          = m_module.opLoad(getVectorTypeId(result.type), ptr.id);
	return result;
}

void GCNCompiler::emitVectorStore(
	SpirvRegisterPointer pointer,
	spv::StorageClass    sclass,
	GcnRegMask           mask,
	SpirvRegisterValue   value)
{
	auto ptr = emitVectorAccess(pointer, sclass, mask);
	m_module.opStore(ptr.id, value.id);
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

	uint32_t t_f32    = m_module.defFloatType(32);
	uint32_t t_f32_v4 = m_module.defVectorType(t_f32, 4);
	//     uint32_t t_f32_a4 = m_module.defArrayType(t_f32, m_module.constu32(4));

	std::array<uint32_t, 1> members;
	members[kPerVertexPosition] = t_f32_v4;
	//     members[PerVertex_CullDist] = t_f32_a4;
	//     members[PerVertex_ClipDist] = t_f32_a4;

	uint32_t typeId = m_module.defStructTypeUnique(
		members.size(), members.data());

	m_module.memberDecorateBuiltIn(typeId, kPerVertexPosition, spv::BuiltInPosition);
	//     m_module.memberDecorateBuiltIn(typeId, PerVertex_CullDist, spv::BuiltInCullDistance);
	//     m_module.memberDecorateBuiltIn(typeId, PerVertex_ClipDist, spv::BuiltInClipDistance);
	m_module.decorateBlock(typeId);

	m_module.setDebugName(typeId, "gl_PerVertex");
	m_module.setDebugMemberName(typeId, kPerVertexPosition, "gl_Position");
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
	case SpirvScalarType::Uint32:
		typeId = m_module.defIntType(32, 0);
		break;
	case SpirvScalarType::Uint64:
		typeId = m_module.defIntType(64, 0);
		break;
	case SpirvScalarType::Sint32:
		typeId = m_module.defIntType(32, 1);
		break;
	case SpirvScalarType::Sint64:
		typeId = m_module.defIntType(64, 1);
		break;
	case SpirvScalarType::Float32:
		typeId = m_module.defFloatType(32);
		break;
	case SpirvScalarType::Float64:
		typeId = m_module.defFloatType(64);
		break;
	case SpirvScalarType::Bool:
		typeId = m_module.defBoolType();
		break;
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

uint32_t GCNCompiler::getArrayTypeId(const SpirvArrayType& type)
{
	uint32_t typeId = getVectorTypeId(type.vtype);

	if (type.alength != 0)
	{
		typeId = m_module.defArrayType(typeId,
									   m_module.constu32(type.alength));
	}

	return typeId;
}

uint32_t GCNCompiler::getPointerTypeId(const SpirvRegisterInfo& type)
{
	return m_module.defPointerType(
		getArrayTypeId(type.atype),
		type.sclass);
}

bool GCNCompiler::isDoubleWordType(SpirvScalarType type) const
{
	return type == SpirvScalarType::Sint64 || type == SpirvScalarType::Uint64 || type == SpirvScalarType::Float64;
}

uint32_t GCNCompiler::findResourceBufferId(uint32_t regIndex)
{
	uint32_t bufferId = InvalidSpvId;

	do
	{
		SpirvConstantBuffer& constBuffer = m_constantBuffers.at(regIndex);
		bufferId                         = constBuffer.varId;
		if (bufferId != InvalidSpvId)
		{
			break;
		}

		SpirvRegularBuffer& regularBuffer = m_regularBuffers.at(regIndex);
		bufferId                          = regularBuffer.varId;
		if (bufferId != InvalidSpvId)
		{
			break;
		}

	} while (false);

	return bufferId;
}

SpirvScalarType GCNCompiler::getScalarType(Instruction::OperandType operandType)
{
	SpirvScalarType resultType = SpirvScalarType::Unknown;

	// For double type operand, we take the destination type.
	// e.g. TypeI32I24 -> Sint32
	switch (operandType)
	{
	case Instruction::TypeB32:
	case Instruction::TypeU32:
	case Instruction::TypeU32U24:
		resultType = SpirvScalarType::Uint32;
		break;
	case Instruction::TypeB64:
	case Instruction::TypeU64:
		resultType = SpirvScalarType::Uint64;
		break;
	case Instruction::TypeF32:
		resultType = SpirvScalarType::Float32;
		break;
	case Instruction::TypeF64:
		resultType = SpirvScalarType::Float64;
		break;
	case Instruction::TypeI32:
	case Instruction::TypeI32I24:
		resultType = SpirvScalarType::Sint32;
		break;
	case Instruction::TypeI64:
		resultType = SpirvScalarType::Sint64;
		break;
	default:
		LOG_ERR("unsupported operand type %d", static_cast<uint32_t>(operandType));
		break;
	}
	return resultType;
}

const char* GCNCompiler::getTypeName(SpirvScalarType type)
{
	const char* name = nullptr;
	switch (type)
	{
	case SpirvScalarType::Uint32:
		name = "uint";
		break;
	case SpirvScalarType::Uint64:
		name = "ulong";
		break;
	case SpirvScalarType::Sint32:
		name = "int";
		break;
	case SpirvScalarType::Sint64:
		name = "long";
		break;
	case SpirvScalarType::Float32:
		name = "float";
		break;
	case SpirvScalarType::Float64:
		name = "float64";
		break;
	case SpirvScalarType::Bool:
		name = "bool";
		break;
	}
	return name;
}

}  // namespace pssl