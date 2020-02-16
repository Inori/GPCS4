#include "GCNCompiler.h"

#include "PsslBindingCalculator.h"

#include "../Gnm/GnmSharpBuffer.h"
#include "Platform/UtilString.h"

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
	const GcnShaderInput& shaderInput) :
	m_programInfo(progInfo),
	m_analysis(&analysis),
	m_shaderInput(shaderInput),
	m_branchLabels(m_analysis->branchLabels)
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

RcPtr<gve::GveShader> GCNCompiler::finalize()
{
	switch (m_programInfo.shaderType())
	{
	case VertexShader:		emitVsFinalize(); break;
	case HullShader:		emitHsFinalize(); break;
	case DomainShader:		emitDsFinalize(); break;
	case GeometryShader:	emitGsFinalize(); break;
	case PixelShader:		emitPsFinalize(); break;
	case ComputeShader:		emitCsFinalize(); break;
	}

	// Declare the entry point, we now have all the
	// information we need, including the interfaces
	m_module.addEntryPoint(m_entryPointId,
						   m_programInfo.executionModel(), "main",
						   m_entryPointInterfaces.size(),
						   m_entryPointInterfaces.data());
	m_module.setDebugName(m_entryPointId, "main");

	return new gve::GveShader(
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

	// Initialize the shader module with capabilities
	// etc. Each shader type has its own peculiarities.
	switch (m_programInfo.shaderType())
	{
	case VertexShader:		emitVsInit(); break;
	case HullShader:		emitHsInit(); break;
	case DomainShader:		emitDsInit(); break;
	case GeometryShader:	emitGsInit(); break;
	case PixelShader:		emitPsInit(); break;
	case ComputeShader:		emitCsInit(); break;
	}
}

void GCNCompiler::emitVsInit()
{
	//m_module.enableCapability(spv::CapabilityClipDistance);
	//m_module.enableCapability(spv::CapabilityCullDistance);
	m_module.enableCapability(spv::CapabilityDrawParameters);

	m_module.enableExtension("SPV_KHR_shader_draw_parameters");
	
	emitDclStatusRegisters();
	emitDclVertexInput();
	emitDclVertexOutput();
	emitDclShaderResourceUD();
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

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_vs.fsFunctionId, 0, nullptr);

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

	emitDclStatusRegisters();
	emitDclPixelInput();
	emitDclPixelOutput();
	emitDclShaderResourceUD();

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
	emitMainFunctionBegin();

	m_module.opFunctionCall(
		m_module.defVoidType(),
		m_ps.functionId, 0, nullptr);

	emitFunctionEnd();
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
		if (!m_shaderInput.vsInputSemantics.has_value())
		{
			break;
		}

		for (const auto& inputSemantic : m_shaderInput.vsInputSemantics.value())
		{
			SpirvRegisterInfo info(SpirvScalarType::Float32, inputSemantic.sizeInElements, 0, spv::StorageClassInput);
			uint32_t inputId = emitNewVariable(info,
											   UtilString::Format("inParam%d", inputSemantic.semantic));

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

			uint32_t outLocation = expInfo.target - (uint32_t)EXPInstruction::TGT::TGTExpParamMin;
			SpirvRegisterInfo info(SpirvScalarType::Float32, expInfo.regIndices.size(),
								   0, spv::StorageClassOutput);
			uint32_t outputId = emitNewVariable(info,
												UtilString::Format("outParam%d", outLocation));

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
				uint32_t vgprId = emitNewVariable(info,
												  UtilString::Format("v%d", vgprIdx));

				auto& input = m_vs.vsInputs[inputSemantic.semantic];

				// Access vector member
				uint32_t fpPtrTypeId = getPointerTypeId(info);
				auto element         = emitRegisterComponentLoad(input, i, spv::StorageClassInput);

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
		uint32_t inputId = emitNewVariable(info,
										   UtilString::Format("inParam%d", i));

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
		const auto& exp         = m_analysis->expParams[i];
		uint32_t componentCount = exp.isCompressed ? exp.regIndices.size() * 2 : exp.regIndices.size();

		SpirvRegisterInfo info(SpirvScalarType::Float32, componentCount,
							   0, spv::StorageClassOutput);
		uint32_t outputId = emitNewVariable(info,
											UtilString::Format("outParam%d", i));

		m_module.decorateLocation(outputId, i);

		m_ps.psOutputs.emplace(exp.target, SpirvRegisterPointer(info.atype.vtype, outputId));
		m_entryPointInterfaces.push_back(outputId);
	}
}

void GCNCompiler::emitGprInitializeVS()
{
	// VGPRs

	// v0 is the index of current vertex within vertex buffer
	SpirvRegisterPointer v0;
	v0.type.ctype  = SpirvScalarType::Sint32;
	v0.type.ccount = 1;

	// Declare gl_VertexIndex
	uint32_t vtxIdxId = emitNewVariable({ v0.type, spv::StorageClassInput }, "gl_VertexIndex");
	m_module.decorateBuiltIn(vtxIdxId, spv::BuiltInVertexIndex);
	m_entryPointInterfaces.push_back(vtxIdxId);

	// The builtin gl_VertexIndex is read only, but v0 is writable.
	// Thus we need to make a copy instead of write gl_VertexIndex in 0 slot directly.
	v0.id              = emitNewVariable({ v0.type, spv::StorageClassPrivate }, "v0");
	m_module.opCopyMemory(v0.id, vtxIdxId);

	m_vgprs.emplace(0, v0);
}

void GCNCompiler::emitGprInitializePS()
{
	// TODO:
	// For sgprs and vgprs, we should initialize them
	// following the ISA manual:
	// 7. Appendix: GPR Allocation and Initialization
	// e.g. We could declare another uniform buffer to hold
	// the 16 user data registers.
	//
	// Currently I just create which I use.

	SpirvRegisterPointer s0;
	s0.type.ctype   = SpirvScalarType::Uint32;
	s0.type.ccount = 1;
	s0.id           = emitNewVariable({ s0.type, spv::StorageClassPrivate },
                             UtilString::Format("s%d", 0));
	m_sgprs.emplace(0, s0);

	SpirvRegisterPointer s12;
	s12.type.ctype  = SpirvScalarType::Float32;
	s12.type.ccount = 1;
	s12.id          = emitNewVariable({ s12.type, spv::StorageClassPrivate },
                             UtilString::Format("s%d", 12));
	m_sgprs.emplace(12, s12);

	SpirvRegisterPointer s16;
	s16.type.ctype  = SpirvScalarType::Float32;
	s16.type.ccount = 1;
	s16.id          = emitNewVariable({ s16.type, spv::StorageClassPrivate },
                             UtilString::Format("s%d", 16));
	m_sgprs.emplace(16, s16);
}

void GCNCompiler::emitDclStatusRegisters()
{
	SpirvVectorType u32Type;
	u32Type.ctype  = SpirvScalarType::Uint32;
	u32Type.ccount = 1;

	m_statusRegs.m0.type = u32Type;
	m_statusRegs.m0.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "m0");

	//m_statusRegs.scc.type = u32Type;
	//m_statusRegs.scc.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "scc");

	m_statusRegs.vcc.type = u32Type;
	m_statusRegs.vcc.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "vcc");

	m_statusRegs.vcc_hi.type = u32Type;
	m_statusRegs.vcc_hi.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "vcc_hi");

	// we assume thread_id = 1, so exec initialized to 1
	m_statusRegs.exec.type = u32Type;
	m_statusRegs.exec.id   = emitNewVariable({ u32Type, spv::StorageClass::StorageClassPrivate }, "exec", m_module.constu32(1));
}

void GCNCompiler::emitDclShaderResource(const GcnShaderResourceInstance& res)
{
	switch (res.usageType)
	{
	case kShaderInputUsageImmConstBuffer:
		emitDclImmConstBuffer(res);
		break;
	case kShaderInputUsageImmResource:
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
	[eudOffsetDw](const auto& resPair)
	{
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
	uint32_t arraySize               = vsharpBuffer->stride * vsharpBuffer->num_records / sizeof(uint32_t);

	uint32_t arrayId = m_module.defArrayTypeUnique(
		m_module.defFloatType(32),
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
	m_vs.m_uboId      = m_module.newVar(uboPtrId, spv::StorageClassUniform);

	m_module.decorateDescriptorSet(m_vs.m_uboId, 0);

	// Note:
	// The calculated bindingId is not "correct", it's a dummy value.
	// We'll remap binding id before compiling pipeline in GveShader class.
	uint32_t bindingId = computeConstantBufferBinding(m_programInfo.shaderType(), res.res.startRegister);
	m_module.decorateBinding(m_vs.m_uboId, bindingId);

	m_module.setDebugName(m_vs.m_uboId, "ubo");

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
						  UtilString::Format("sampler%d", samplerId).c_str());

	m_module.decorateDescriptorSet(varId, 0);

	uint32_t bindingId = computeSamplerBinding(m_programInfo.shaderType(), res.res.startRegister);
	m_module.decorateBinding(varId, bindingId);

	SpirvSampler sampler;
	sampler.varId               = varId;
	sampler.typeId              = samplerType;
	m_ps.samplers.at(samplerId) = sampler;

	m_resourceSlots.push_back({ bindingId, VK_DESCRIPTOR_TYPE_SAMPLER });
}

void GCNCompiler::emitDclImmResource(const GcnShaderResourceInstance& res)
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
						  UtilString::Format("texture%d", registerId).c_str());

	m_module.decorateDescriptorSet(varId, 0);

	uint32_t bindingId = computeResBinding(m_programInfo.shaderType(), res.res.startRegister);
	m_module.decorateBinding(varId, bindingId);

	SpirvTexture texture;
	texture.imageInfo            = typeInfo;
	texture.varId                = varId;
	texture.imageTypeId          = imageTypeId;
	m_ps.textures.at(registerId) = texture;

	m_resourceSlots.push_back({ bindingId, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE });
}

// TODO:
// Wrap these using template.

SpirvRegisterValue GCNCompiler::emitValueLoad(const SpirvRegisterPointer& reg)
{
	uint32_t varId = m_module.opLoad(
		getVectorTypeId(reg.type),
		reg.id);
	return SpirvRegisterValue(reg.type, varId);
}

SpirvRegisterValue GCNCompiler::emitSgprLoad(uint32_t index, SpirvScalarType dstType /*= SpirvScalarType::Unknown*/)
{
	SpirvRegisterValue result;
	do 
	{
		auto& sgpr = m_sgprs[index];

		if (sgpr.id == InvalidSpvId)
		{
			// Some instructions will specify 0 as one of their SRCs,
			// here 0 can be treated as both s0 and an empty src.
			// An empty src means the instruction doesn't use this src, e.g. SRC2 for v3_mac_f32.
			// In such case, we shouldn't do anything.
			LOG_WARN("try to load uninitialized sgpr s%d", index);
			break;
		}

		if (dstType != SpirvScalarType::Unknown && dstType != sgpr.type.ctype)
		{
			emitUpdateSgprType(index, dstType);
		}

		result = emitValueLoad(sgpr);

	} while (false);
	return result;
}

SpirvRegisterValue GCNCompiler::emitVgprLoad(uint32_t index, SpirvScalarType dstType /*= SpirvScalarType::Unknown*/)
{
	SpirvRegisterValue result;
	do 
	{
		auto& vgpr = m_vgprs[index];

		if (vgpr.id == InvalidSpvId)
		{
			// Some instructions will specify 0 as one of their SRCs,
			// here 0 can be treated as both v0 and an empty src.
			// An empty src means the instruction doesn't use this src, e.g. SRC2 for v3_mac_f32.
			// In such case, we shouldn't do anything.
			LOG_WARN("try to load uninitialized vgpr v%d", index);
			break;
		}

		if (dstType != SpirvScalarType::Unknown && dstType != vgpr.type.ctype)
		{
			emitUpdateVgprType(index, dstType);
		}
		result = emitValueLoad(vgpr);
	} while (false);
	return result;
}

void GCNCompiler::emitUpdateSgprType(uint32_t sidx, SpirvScalarType dstType)
{
	LOG_ASSERT(!isDoubleWordType(dstType), "wide type not supported yet.");
	do 
	{
		auto& sgpr = m_sgprs[sidx];

		if (sgpr.type.ctype == dstType)
		{
			break;
		}

		LOG_ASSERT(sgpr.type.ctype != SpirvScalarType::Unknown, "sgpr s%d not initialized.", sidx);
		SpirvRegisterValue value = emitValueLoad(sgpr);
		SpirvRegisterValue castedValue = emitRegisterBitcast(value, dstType);

		auto debugName = UtilString::Format("s%d_%s", sidx, getTypeName(dstType));
		SpirvRegisterPointer newSgpr;
		newSgpr.type.ctype  = dstType;
		newSgpr.type.ccount = 1;
		newSgpr.id          = emitNewVariable({ newSgpr.type, spv::StorageClassPrivate }, debugName);
		
		emitValueStore(newSgpr, castedValue, 1);

		sgpr = newSgpr;
	} while (false);
}

void GCNCompiler::emitUpdateVgprType(uint32_t vidx, SpirvScalarType dstType)
{
	LOG_ASSERT(!isDoubleWordType(dstType), "wide type not supported yet.");
	do 
	{
		auto& vgpr = m_vgprs[vidx];

		if (vgpr.type.ctype == dstType)
		{
			break;
		}

		LOG_ASSERT(vgpr.type.ctype != SpirvScalarType::Unknown, "vgpr v%d not initialized.", vidx);

		SpirvRegisterValue value       = emitValueLoad(vgpr);
		SpirvRegisterValue castedValue = emitRegisterBitcast(value, dstType);

		auto debugName = UtilString::Format("v%d_%s", vidx, getTypeName(dstType));
		SpirvRegisterPointer newVgpr;
		newVgpr.type.ctype  = dstType;
		newVgpr.type.ccount = 1;
		newVgpr.id          = emitNewVariable({ newVgpr.type, spv::StorageClassPrivate }, debugName);

		emitValueStore(newVgpr, castedValue, 1);

		vgpr = newVgpr;
	} while (false);
}

void GCNCompiler::emitValueStore(
	const SpirvRegisterPointer& ptr,
	const SpirvRegisterValue& src,
	const GcnRegMask& writeMask)
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
	if (src.type.ccount == 1)
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

void GCNCompiler::emitSgprStore(uint32_t dstIdx,
								const SpirvRegisterValue& srcReg)
{
	auto& sgpr = m_sgprs[dstIdx];
	if (sgpr.id == InvalidSpvId)  // Not initialized
	{
		sgpr.type = srcReg.type;
		sgpr.id   = emitNewVariable({ sgpr.type, spv::StorageClassPrivate },
                                  UtilString::Format("s%d", dstIdx));
	}

	if (sgpr.type.ctype != srcReg.type.ctype)
	{
		emitUpdateSgprType(dstIdx, srcReg.type.ctype);
	}

	emitValueStore(sgpr, srcReg, 1);
}

void GCNCompiler::emitSgprArrayStore(uint32_t startIdx,
									 const SpirvRegisterValue* values, uint32_t count)
{
	for (uint32_t i = 0; i != count; ++i)
	{
		emitSgprStore(startIdx + i, values[i]);
	}
}

void GCNCompiler::emitVgprStore(uint32_t dstIdx,
								const SpirvRegisterValue& srcReg)
{
	auto& vgpr = m_vgprs[dstIdx];
	if (vgpr.id == InvalidSpvId)  // Not initialized
	{
		vgpr.type = srcReg.type;
		vgpr.id   = emitNewVariable({ vgpr.type, spv::StorageClassPrivate },
                                  UtilString::Format("v%d", dstIdx));
	}

	if (vgpr.type.ctype != srcReg.type.ctype)
	{
		emitUpdateVgprType(dstIdx, srcReg.type.ctype);
	}

	emitValueStore(vgpr, srcReg, 1);
}

void GCNCompiler::emitVgprArrayStore(uint32_t startIdx,
									 const SpirvRegisterValue* values, uint32_t count)
{
	for (uint32_t i = 0; i != count; ++i)
	{
		emitVgprStore(startIdx + i, values[i]);
	}
}

void GCNCompiler::emitVgprVectorStore(uint32_t startIdx, const SpirvRegisterValue& srcVec, const GcnRegMask& writeMask)
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

		emitVgprStore(startIdx + i, value);
	}
}

SpirvRegisterValue GCNCompiler::emitSgprPairLoad(uint32_t firstIndex)
{
	// The type of a SGPR pair will be only Uint64

	// Load the higher and lower sgpr
	auto high = emitSgprLoad(firstIndex, SpirvScalarType::Uint32);
	auto low  = emitSgprLoad(firstIndex + 1, SpirvScalarType::Uint32);

	// Convert the two Uint32 to a vec2
	auto vec2U32 = emitRegisterConcat(high, low);

	// Bitcast to Uint64
	return emitRegisterBitcast(vec2U32, SpirvScalarType::Uint64);
}

void GCNCompiler::emitSgprPairStore(uint32_t firstIndex, const SpirvRegisterValue& srcReg)
{
	// The type of a SGPR pair will be only Uint64

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
	high.id        = m_module.opCompositeExtract(u32TypeId, vec2U32.id, 1, &index);

	index		= 1;
	low.id		= m_module.opCompositeExtract(u32TypeId, vec2U32.id, 1, &index);

	// Store
	emitSgprStore(firstIndex, high);
	emitSgprStore(firstIndex + 1, low);
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

// Used with with 7 bits SDST, 8 bits SSRC or 9 bits SRC
// See table "SDST, SSRC and SRC Operands" in section 3.1 of GPU Shader Core ISA manual
SpirvRegisterValue GCNCompiler::emitLoadScalarOperand(
	uint32_t srcOperand, uint32_t regIndex,
	SpirvScalarType dstType, uint32_t literalConst /*= 0*/ )
{
	Instruction::OperandSRC src = static_cast<Instruction::OperandSRC>(srcOperand);
	SpirvRegisterValue operand;

	switch (src)
	{
	case Instruction::OperandSRC::SRCScalarGPRMin ... Instruction::OperandSRC::SRCScalarGPRMax:
		operand = dstType != SpirvScalarType::Uint64 ? 
			emitSgprLoad(regIndex, dstType) :
			emitSgprPairLoad(regIndex);
		break;
	case Instruction::OperandSRC::SRCVccLo:
		operand = emitValueLoad(m_statusRegs.vcc);
		break;
	case Instruction::OperandSRC::SRCVccHi:
		operand = emitValueLoad(m_statusRegs.vcc_hi);
		break;
	case Instruction::OperandSRC::SRCM0:
		operand = emitValueLoad(m_statusRegs.m0);
		break;
	case Instruction::OperandSRC::SRCExecLo:
		operand = emitValueLoad(m_statusRegs.exec);
		break;
	case Instruction::OperandSRC::SRCExecHi:
		operand = emitValueLoad(m_statusRegs.exec_hi);
		break;
	case Instruction::OperandSRC::SRCConstZero:
	case Instruction::OperandSRC::SRCSignedConstIntPosMin ... Instruction::OperandSRC::SRCSignedConstIntPosMax:
	case Instruction::OperandSRC::SRCSignedConstIntNegMin ... Instruction::OperandSRC::SRCSignedConstIntNegMax:
		operand = emitInlineConstantInteger(src);
		break;
	case Instruction::OperandSRC::SRCConstFloatPos_0_5 ... Instruction::OperandSRC::SRCConstFloatNeg_4_0:
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
	case Instruction::OperandSRC::SRCVectorGPRMin ... Instruction::OperandSRC::SRCVectorGPRMax:
		operand = emitVgprLoad(regIndex, dstType);
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
	return emitVgprLoad(index, dstType);
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
			emitSgprStore(regIndex, srcReg) : 
			emitSgprPairStore(regIndex, srcReg);
	}
		break;
	case Instruction::OperandSDST::SDSTVccLo:
		emitValueStore(m_statusRegs.vcc, srcReg, 1);
		break;
	case Instruction::OperandSDST::SDSTVccHi:
		emitValueStore(m_statusRegs.vcc_hi, srcReg, 1);
		break;
	case Instruction::OperandSDST::SDSTExecLo:
		emitValueStore(m_statusRegs.exec, srcReg, 1);
		break;
	case Instruction::OperandSDST::SDSTExecHi:
		emitValueStore(m_statusRegs.exec_hi, srcReg, 1);
		break;
	case Instruction::OperandSDST::SDSTM0:
		emitValueStore(m_statusRegs.m0, srcReg, 1);
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
	emitVgprStore(dstIndex, srcReg);
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
	default:
		break;
	}

	uint32_t valueId = m_module.constf32(value);
	return SpirvRegisterValue(SpirvScalarType::Float32, 1, valueId);
}

SpirvRegisterValue GCNCompiler::emitInlineConstantInteger(Instruction::OperandSRC src)
{
	int32_t value = 0;
	switch (src)
	{
	case Instruction::OperandSRC::SRCConstZero:
		value = 0;
		break;
	case Instruction::OperandSRC::SRCSignedConstIntPosMin ... Instruction::OperandSRC::SRCSignedConstIntPosMax:
		value = (int32_t)src - 128;
		break;
	case Instruction::OperandSRC::SRCSignedConstIntNegMin ... Instruction::OperandSRC::SRCSignedConstIntNegMax:
		value = 192 - (int32_t)src;
		break;
	default:
		break;
	}

	uint32_t valueId = m_module.consti32(value);
	return SpirvRegisterValue(SpirvScalarType::Sint32, 1, valueId);
}

uint32_t GCNCompiler::emitLoadSampledImage(const SpirvTexture& textureResource, const SpirvSampler& samplerResource)
{
	const uint32_t sampledImageType = m_module.defSampledImageType(textureResource.imageTypeId);

	return m_module.opSampledImage(sampledImageType,
								   m_module.opLoad(textureResource.imageTypeId, textureResource.varId),
								   m_module.opLoad(samplerResource.typeId, samplerResource.varId));
}

pssl::SpirvRegisterValue GCNCompiler::emitPackFloat16(const SpirvRegisterValue& v2floatVec)
{
	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Uint32;
	result.type.ccount = 1;

	const uint32_t u32Type = getVectorTypeId(result.type);
	result.id              = m_module.opPackHalf2x16(u32Type, v2floatVec.id);
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitUnpackFloat16(const SpirvRegisterValue& uiVec)
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
	const std::string& name /* = "" */,
	std::optional<uint32_t> initValueId /*= std::nullopt*/)
{
	const uint32_t ptrTypeId = getPointerTypeId(info);
	uint32_t varId           = InvalidSpvId;
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

uint32_t GCNCompiler::emitNewBuiltinVariable(const SpirvRegisterInfo& info, spv::BuiltIn builtIn, const char* name)
{
	const uint32_t varId = emitNewVariable(info);

	m_module.setDebugName(varId, name);
	m_module.decorateBuiltIn(varId, builtIn);

	if (m_programInfo.shaderType() == PixelShader
		&& info.atype.vtype.ctype != SpirvScalarType::Float32
		&& info.atype.vtype.ctype != SpirvScalarType::Bool
		&& info.sclass == spv::StorageClassInput)
	{
		m_module.decorate(varId, spv::DecorationFlat);
	}

	m_entryPointInterfaces.push_back(varId);
	return varId;
}

SpirvRegisterPointer GCNCompiler::emitArrayAccess(SpirvRegisterPointer pointer, spv::StorageClass sclass, uint32_t index)
{
	uint32_t ptrTypeId = m_module.defPointerType(
		getVectorTypeId(pointer.type), sclass);

	SpirvRegisterPointer result;
	result.type = pointer.type;
	result.id   = m_module.opAccessChain(
        ptrTypeId, pointer.id, 1, &index);
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVecf32(float x, float y, float z, float w, const GcnRegMask& writeMask)
{
	// TODO refactor these functions into one single template
	std::array<uint32_t, 4> ids = { 0, 0, 0, 0 };
	uint32_t componentIndex = 0;

	if (writeMask[0]) ids[componentIndex++] = m_module.constf32(x);
	if (writeMask[1]) ids[componentIndex++] = m_module.constf32(y);
	if (writeMask[2]) ids[componentIndex++] = m_module.constf32(z);
	if (writeMask[3]) ids[componentIndex++] = m_module.constf32(w);

	SpirvRegisterValue result;
	result.type.ctype = SpirvScalarType::Float32;
	result.type.ccount = componentIndex;
	result.id = componentIndex > 1
		? m_module.constComposite(
			getVectorTypeId(result.type),
			componentIndex, ids.data())
		: ids[0];
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVecu32(uint32_t x, uint32_t y, uint32_t z, uint32_t w, const GcnRegMask& writeMask)
{
	std::array<uint32_t, 4> ids = { 0, 0, 0, 0 };
	uint32_t componentIndex = 0;

	if (writeMask[0]) ids[componentIndex++] = m_module.constu32(x);
	if (writeMask[1]) ids[componentIndex++] = m_module.constu32(y);
	if (writeMask[2]) ids[componentIndex++] = m_module.constu32(z);
	if (writeMask[3]) ids[componentIndex++] = m_module.constu32(w);

	SpirvRegisterValue result;
	result.type.ctype = SpirvScalarType::Uint32;
	result.type.ccount = componentIndex;
	result.id = componentIndex > 1
		? m_module.constComposite(
			getVectorTypeId(result.type),
			componentIndex, ids.data())
		: ids[0];
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVeci32(int32_t x, int32_t y, int32_t z, int32_t w, const GcnRegMask& writeMask)
{
	std::array<uint32_t, 4> ids = { 0, 0, 0, 0 };
	uint32_t componentIndex = 0;

	if (writeMask[0]) ids[componentIndex++] = m_module.consti32(x);
	if (writeMask[1]) ids[componentIndex++] = m_module.consti32(y);
	if (writeMask[2]) ids[componentIndex++] = m_module.consti32(z);
	if (writeMask[3]) ids[componentIndex++] = m_module.consti32(w);

	SpirvRegisterValue result;
	result.type.ctype = SpirvScalarType::Sint32;
	result.type.ccount = componentIndex;
	result.id = componentIndex > 1
		? m_module.constComposite(
			getVectorTypeId(result.type),
			componentIndex, ids.data())
		: ids[0];
	return result;
}

pssl::SpirvRegisterValue GCNCompiler::emitBuildConstVecf64(double xy, double zw, const GcnRegMask& writeMask)
{
	std::array<uint32_t, 2> ids = { 0, 0 };
	uint32_t componentIndex = 0;

	if (writeMask[0] && writeMask[1]) ids[componentIndex++] = m_module.constf64(xy);
	if (writeMask[2] && writeMask[3]) ids[componentIndex++] = m_module.constf64(zw);

	SpirvRegisterValue result;
	result.type.ctype = SpirvScalarType::Float64;
	result.type.ccount = componentIndex;
	result.id = componentIndex > 1
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
	result.type.ctype = dstType;
	result.type.ccount = srcValue.type.ccount;

	if (isDoubleWordType(srcType)) result.type.ccount *= 2;
	if (isDoubleWordType(dstType)) result.type.ccount /= 2;

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
	case SpirvScalarType::Float32: value.id = m_module.opFAbs(typeId, value.id); break;
	case SpirvScalarType::Sint32:  value.id = m_module.opSAbs(typeId, value.id); break;
	default: LOG_WARN("GCNCompiler: Cannot get absolute value for given type");
	}

	return value;
}

SpirvRegisterValue GCNCompiler::emitRegisterNegate(SpirvRegisterValue value)
{
	const uint32_t typeId = getVectorTypeId(value.type);

	switch (value.type.ctype) 
	{
	case SpirvScalarType::Float32: value.id = m_module.opFNegate(typeId, value.id); break;
	case SpirvScalarType::Float64: value.id = m_module.opFNegate(typeId, value.id); break;
	case SpirvScalarType::Sint32:  value.id = m_module.opSNegate(typeId, value.id); break;
	case SpirvScalarType::Sint64:  value.id = m_module.opSNegate(typeId, value.id); break;
	default: LOG_WARN("GCNCompiler: Cannot negate given type");
	}

	return value;
}

SpirvRegisterValue GCNCompiler::emitRegisterZeroTest(SpirvRegisterValue value, SpirvZeroTest test)
{
	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Bool;
	result.type.ccount = 1;

	const uint32_t zeroId = m_module.constu32(0u);
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

SpirvRegisterValue GCNCompiler::emitRegisterComponentLoad(
	const SpirvRegisterPointer& srcVec,
	uint32_t compIndex,
	spv::StorageClass storageClass /* = spv::StorageClassPrivate */)
{
	uint32_t typeId           = getScalarTypeId(srcVec.type.ctype);
	uint32_t ptrTypeId        = m_module.defPointerType(typeId, storageClass);
	uint32_t compositeIndexId = m_module.constu32(compIndex);
	uint32_t compositePointer = m_module.opAccessChain(
		ptrTypeId,
		srcVec.id,
		1, &compositeIndexId);
	uint32_t valueId = m_module.opLoad(typeId, compositePointer);
	return SpirvRegisterValue(srcVec.type.ctype, 1, valueId);
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
	return type == SpirvScalarType::Sint64
		|| type == SpirvScalarType::Uint64
		|| type == SpirvScalarType::Float64;
}

SpirvScalarType GCNCompiler::getScalarType(Instruction::OperandType operandType)
{
	SpirvScalarType resultType = SpirvScalarType::Unknown;
	switch (operandType)
	{
	case Instruction::TypeB32:
	case Instruction::TypeU32: resultType = SpirvScalarType::Uint32; break;
	case Instruction::TypeB64:
	case Instruction::TypeU64: resultType = SpirvScalarType::Uint64; break;
	case Instruction::TypeF32: resultType = SpirvScalarType::Float32; break;
	case Instruction::TypeF64: resultType = SpirvScalarType::Float64; break;
	case Instruction::TypeI32: resultType = SpirvScalarType::Sint32; break;
	case Instruction::TypeI64: resultType = SpirvScalarType::Sint64; break;
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
	case SpirvScalarType::Uint32: name = "uint";	break;
	case SpirvScalarType::Uint64: name = "ulong";	break;
	case SpirvScalarType::Sint32: name = "int";		break;
	case SpirvScalarType::Sint64: name = "long";	break;
	case SpirvScalarType::Float32:name = "float";	break;
	case SpirvScalarType::Float64:name = "float64";	break;
	case SpirvScalarType::Bool:	  name = "bool";	break;
	}
	return name;
}

}  // namespace pssl