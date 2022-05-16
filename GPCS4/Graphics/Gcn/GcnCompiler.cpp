#include "GcnCompiler.h"

#include "GcnAnalysis.h"
#include "GcnHeader.h"
#include "GcnUtil.h"
#include "PlatFile.h"

#include "Gnm/GnmConstant.h"

#include <algorithm>

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

using namespace sce::Gnm;
using namespace sce::vlt;

namespace sce::gcn
{
	constexpr uint32_t PerVertex_Position = 0;
	constexpr uint32_t PerVertex_CullDist = 1;
	constexpr uint32_t PerVertex_ClipDist = 2;

	GcnCompiler::GcnCompiler(
		const std::string&     fileName,
		const GcnModuleInfo&   moduleInfo,
		const GcnProgramInfo&  programInfo,
		const GcnHeader&       header,
		const GcnShaderMeta&   meta,
		const GcnAnalysisInfo& analysis) :
		m_moduleInfo(moduleInfo),
		m_programInfo(programInfo),
		m_header(&header),
		m_meta(meta),
		m_analysis(&analysis),
		m_module(spvVersion(1, 5)),
		m_state({
			{ this, "exec" },  
			{ this, "vcc" },
		}),
		m_controlFlowBlocks(m_analysis->controlFlowBlocks)
	{
		// Declare an entry point ID. We'll need it during the
		// initialization phase where the execution mode is set.
		m_entryPointId = m_module.allocateId();

		// Set the shader name so that we recognize it in renderdoc
		m_module.setDebugSource(
			spv::SourceLanguageUnknown, 0,
			m_module.addDebugString(fileName.c_str()),
			nullptr);

		// Set the memory model. This is the same for all shaders.
		m_module.setMemoryModel(
			spv::AddressingModelLogical,
			spv::MemoryModelGLSL450);

		this->emitInit();
	}

	GcnCompiler::~GcnCompiler()
	{
	}

	void GcnCompiler::processInstruction(
		const GcnShaderInstruction& ins)
	{
		emitBranchLabel();

		compileInstruction(ins);

		advanceProgramCounter(ins);
	}

	void GcnCompiler::compileInstruction(
		const GcnShaderInstruction& ins)
	{
		auto catecory = ins.category;
		switch (catecory)
		{
		case GcnInstCategory::ScalarALU:
			this->emitScalarALU(ins);
			break;
		case GcnInstCategory::ScalarMemory:
			this->emitScalarMemory(ins);
			break;
		case GcnInstCategory::VectorALU:
			this->emitVectorALU(ins);
			break;
		case GcnInstCategory::VectorMemory:
			this->emitVectorMemory(ins);
			break;
		case GcnInstCategory::FlowControl:
			this->emitFlowControl(ins);
			break;
		case GcnInstCategory::DataShare:
			this->emitDataShare(ins);
			break;
		case GcnInstCategory::VectorInterpolation:
			this->emitVectorInterpolation(ins);
			break;
		case GcnInstCategory::Export:
			this->emitExport(ins);
			break;
		case GcnInstCategory::DebugProfile:
			this->emitDebugProfile(ins);
			break;
		case GcnInstCategory::Undefined:
			LOG_GCN_UNHANDLED_INST();
			break;
		}
	}

	Rc<VltShader> GcnCompiler::finalize()
	{
		// Depending on the shader type, this will prepare
		// input registers, call various shader functions
		// and write back the output registers.
		switch (m_programInfo.type()) 
		{
		  case GcnProgramType::VertexShader:   this->emitVsFinalize(); break;
		  case GcnProgramType::HullShader:     this->emitHsFinalize(); break;
		  case GcnProgramType::DomainShader:   this->emitDsFinalize(); break;
		  case GcnProgramType::GeometryShader: this->emitGsFinalize(); break;
		  case GcnProgramType::PixelShader:    this->emitPsFinalize(); break;
		  case GcnProgramType::ComputeShader:  this->emitCsFinalize(); break;
		}

		// Declare the entry point, we now have all the
		// information we need, including the interfaces
		m_module.addEntryPoint(m_entryPointId,
							   m_programInfo.executionModel(), "main",
							   m_entryPointInterfaces.size(),
							   m_entryPointInterfaces.data());
		m_module.setDebugName(m_entryPointId, "main");

		// Options is not used currently, pass a dummy value.
		VltShaderOptions shaderOptions = {};

		// Create the shader module object
		return new VltShader(
			m_programInfo.shaderStage(),
			m_resourceSlots,
			m_interfaceSlots,
			m_module.compile(),
			shaderOptions,
			std::move(m_immConstData));
	}

	void GcnCompiler::emitInit()
	{
		// Set up common capabilities for all shaders
		m_module.enableCapability(spv::CapabilityShader);
		m_module.enableCapability(spv::CapabilityImageQuery);
		m_module.enableCapability(spv::CapabilityGroupNonUniform);
		m_module.enableCapability(spv::CapabilityGroupNonUniformBallot);

		// Declare sgpr/vgpr array.
		this->emitDclGprArray();
		// Declare hardware state register.
		this->emitDclStateRegister();
		// Declare shader resource and input interfaces
		this->emitDclInputSlots();
		// Declare export parameters
		this->emitDclExport();

		// Initialize the shader module with capabilities
		// etc. Each shader type has its own peculiarities.

		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:	emitVsInit(); break;
			case GcnProgramType::HullShader:	emitHsInit(); break;
			case GcnProgramType::DomainShader:	emitDsInit(); break;
			case GcnProgramType::GeometryShader:emitGsInit(); break;
			case GcnProgramType::PixelShader:	emitPsInit(); break;
			case GcnProgramType::ComputeShader:	emitCsInit(); break;
		}

	}

	void GcnCompiler::emitFunctionBegin(
		uint32_t entryPoint,
		uint32_t returnType,
		uint32_t funcType)
	{
		this->emitFunctionEnd();

		m_module.functionBegin(
			returnType, entryPoint, funcType,
			spv::FunctionControlMaskNone);

		m_insideFunction = true;
	}

	void GcnCompiler::emitFunctionEnd()
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

		m_insideFunction = false;
	}

	void GcnCompiler::emitFunctionLabel()
	{
		m_module.opLabel(m_module.allocateId());
		m_insideBlock = true;
	}

	void GcnCompiler::emitMainFunctionBegin()
	{
		this->emitFunctionBegin(
			m_entryPointId,
			m_module.defVoidType(),
			m_module.defFunctionType(
				m_module.defVoidType(), 0, nullptr));
		this->emitFunctionLabel();
	}

	void GcnCompiler::emitVsInit()
	{
		//m_module.enableCapability(spv::CapabilityClipDistance);
		//m_module.enableCapability(spv::CapabilityCullDistance);
		m_module.enableCapability(spv::CapabilityDrawParameters);

		m_module.enableExtension("SPV_KHR_shader_draw_parameters");

		// Declare the per-vertex output block. This is where
		// the vertex shader will write the vertex position.
		const uint32_t perVertexStruct  = this->getPerVertexBlockId();
		const uint32_t perVertexPointer = m_module.defPointerType(
			perVertexStruct, spv::StorageClassOutput);

		m_perVertexOut = m_module.newVar(
			perVertexPointer, spv::StorageClassOutput);
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

	void GcnCompiler::emitHsInit()
	{
	}

	void GcnCompiler::emitDsInit()
	{
	}

	void GcnCompiler::emitGsInit()
	{
	}

	void GcnCompiler::emitPsInit()
	{
		m_module.enableCapability(spv::CapabilityDerivativeControl);

		m_module.setExecutionMode(m_entryPointId,
								  spv::ExecutionModeOriginUpperLeft);

		// Declare inputs from vertex stage.
		this->emitDclPsInput();

		// Main function of the pixel shader
		m_ps.functionId = m_module.allocateId();
		m_module.setDebugName(m_ps.functionId, "ps_main");

		this->emitFunctionBegin(
			m_ps.functionId,
			m_module.defVoidType(),
			m_module.defFunctionType(
				m_module.defVoidType(), 0, nullptr));
		this->emitFunctionLabel();

		// TODO:
		// Support discard
		
		//if (m_analysis->usesKill && m_moduleInfo.options.useDemoteToHelperInvocation)
		//{
		//	// This extension basically implements D3D-style discard
		//	m_module.enableExtension("SPV_EXT_demote_to_helper_invocation");
		//	m_module.enableCapability(spv::CapabilityDemoteToHelperInvocationEXT);
		//}
		//else if (m_analysis->usesKill && m_analysis->usesDerivatives)
		//{
		//	// We may have to defer kill operations to the end of
		//	// the shader in order to keep derivatives correct.
		//	m_ps.killState = m_module.newVarInit(
		//		m_module.defPointerType(m_module.defBoolType(), spv::StorageClassPrivate),
		//		spv::StorageClassPrivate, m_module.constBool(false));

		//	m_module.setDebugName(m_ps.killState, "ps_kill");
		//}
	}

	void GcnCompiler::emitCsInit()
	{
		// Declare local size
		this->emitDclThreadGroup();
		// Declare LDS
		this->emitDclThreadGroupSharedMemory(m_meta.cs.ldsSize);

		// Main function of the compute shader
		m_cs.functionId = m_module.allocateId();
		m_module.setDebugName(m_cs.functionId, "cs_main");

		this->emitFunctionBegin(
			m_cs.functionId,
			m_module.defVoidType(),
			m_module.defFunctionType(
				m_module.defVoidType(), 0, nullptr));
		this->emitFunctionLabel();
	}

	void GcnCompiler::emitVsFinalize()
	{
		this->emitMainFunctionBegin();
		// Initialize system defined SGPR/VGPR values.
		this->emitInputSetup();
		// Some vertex shaders do not have fetch shader.
		if (m_vs.fetchFuncId)
		{
			// call fetch shader
			m_module.opFunctionCall(
				m_module.defVoidType(),
				m_vs.fetchFuncId, 0, nullptr);
		}

		// call vs_main
		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_vs.functionId, 0, nullptr);
		
		this->emitFunctionEnd();
	}

	void GcnCompiler::emitHsFinalize()
	{
	}

	void GcnCompiler::emitDsFinalize()
	{
	}

	void GcnCompiler::emitGsFinalize()
	{
	}

	void GcnCompiler::emitPsFinalize()
	{
		this->emitMainFunctionBegin();
		this->emitInputSetup();
	
		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_ps.functionId, 0, nullptr);

		//if (m_ps.killState != 0)
		//{
		//	DxbcConditional cond;
		//	cond.labelIf  = m_module.allocateId();
		//	cond.labelEnd = m_module.allocateId();

		//	uint32_t killTest = m_module.opLoad(m_module.defBoolType(), m_ps.killState);

		//	m_module.opSelectionMerge(cond.labelEnd, spv::SelectionControlMaskNone);
		//	m_module.opBranchConditional(killTest, cond.labelIf, cond.labelEnd);

		//	m_module.opLabel(cond.labelIf);
		//	m_module.opKill();

		//	m_module.opLabel(cond.labelEnd);
		//}

		this->emitFunctionEnd();
	}

	void GcnCompiler::emitCsFinalize()
	{
		this->emitMainFunctionBegin();
		this->emitInputSetup();

		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_cs.functionId, 0, nullptr);

		this->emitFunctionEnd();
	}

	void GcnCompiler::emitDclInputSlots()
	{
		// Declare resource and input interfaces in input usage slot

		// Get the flattened resource table.
		const auto& resouceTable = m_header->getShaderResourceTable();
		for (const auto& res : resouceTable)
		{
			ShaderInputUsageType usage = (ShaderInputUsageType)res.usage;
			switch (usage)
			{
				case kShaderInputUsageImmConstBuffer:
				{
					// ImmConstBuffer is different from D3D11's ImmediateConstantBuffer
					// It's not constant data embedded into the shader, it's just a simple buffer binding.
					this->emitDclBuffer(res);
				}
					break;
				case kShaderInputUsageImmResource:
				case kShaderInputUsageImmRwResource:
				{
					if (res.type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
					{
						this->emitDclBuffer(res);
					}
					else
					{
						this->emitDclTexture(res);
					}
				}
					break;
				case kShaderInputUsageImmSampler:
				{
					this->emitDclSampler(res);
				}
					break;
				case kShaderInputUsagePtrVertexBufferTable:
				{
					LOG_ASSERT(hasFetchShader() == true, "no fetch shader found while vertex buffer table exist.");
					// Declare vertex input
					this->emitDclVertexInput();
					// Emulate fetch shader with a function
					this->emitFetchInput();
				}
					break;
				case kShaderInputUsageImmAluFloatConst:
				case kShaderInputUsageImmAluBool32Const:
				case kShaderInputUsageImmGdsCounterRange:
				case kShaderInputUsageImmGdsMemoryRange:
				case kShaderInputUsageImmGwsBase:
				case kShaderInputUsageImmLdsEsGsSize:
				case kShaderInputUsageImmVertexBuffer:
					LOG_ASSERT(false, "TODO: usage type %d not supported.", usage);
					break;
			}
		}
	}

	void GcnCompiler::emitDclBuffer(
		const GcnShaderResource& res)
	{
		uint32_t regIdx = res.startRegister;

		const bool asSsbo =
			(res.type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

		std::string name = util::str::formatex(asSsbo ? "sb" : "cb", regIdx);
		// Declare the uniform buffer as max supported size.
		// Will this produce bad impact?
		constexpr uint32_t MaxUniformBufferSize = 65536;
		uint32_t           numConstants         = asSsbo ? 0 : MaxUniformBufferSize / 16;

		uint32_t arrayType = 0;
		if (!asSsbo)
		{
			// std140 layout uniform buffer data is stored as a fixed-size array
			// of 4x32-bit vectors. SPIR-V requires explicit strides.
			arrayType = m_module.defArrayTypeUnique(
				getVectorTypeId({ GcnScalarType::Float32, 4 }),
				m_module.constu32(numConstants));
			m_module.decorateArrayStride(arrayType, 16);
		}
		else
		{
			arrayType = m_module.defRuntimeArrayTypeUnique(
				getScalarTypeId(GcnScalarType::Uint32));
			m_module.decorateArrayStride(arrayType, 4);
		}

		// SPIR-V requires us to put that array into a
		// struct and decorate that struct as a block.
		const uint32_t structType = m_module.defStructTypeUnique(1, &arrayType);

		m_module.decorate(structType, asSsbo
										  ? spv::DecorationBufferBlock
										  : spv::DecorationBlock);
		m_module.memberDecorateOffset(structType, 0, 0);

		m_module.setDebugName(structType, util::str::formatex(name.c_str(), "_t").c_str());
		m_module.setDebugMemberName(structType, 0, "m");

		// Variable that we'll use to access the buffer
		const uint32_t varId = m_module.newVar(
			m_module.defPointerType(structType, spv::StorageClassUniform),
			spv::StorageClassUniform);

		m_module.setDebugName(varId, name.c_str());

		// Compute the VLT binding slot index for the buffer.
		// Gnm needs to bind the actual buffers to this slot.
		uint32_t bindingId = asSsbo ? computeResourceBinding(
										  m_programInfo.type(), regIdx)
									: computeConstantBufferBinding(
										  m_programInfo.type(), regIdx);

		m_module.decorateDescriptorSet(varId, 0);
		m_module.decorateBinding(varId, bindingId);

		if (res.usage == kShaderInputUsageImmResource)
			m_module.decorate(varId, spv::DecorationNonWritable);

		// Record the buffer so that we can use it
		// while compiling buffer instructions.
		GcnBuffer buf;
		buf.varId            = varId;
		buf.size             = numConstants;
		buf.asSsbo           = asSsbo;
		m_buffers.at(regIdx) = buf;

		// Store descriptor info for the shader interface
		VltResourceSlot resource;
		resource.slot   = bindingId;
		resource.type   = asSsbo
							  ? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
							  : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		resource.view   = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		resource.access = 
			res.usage == kShaderInputUsageImmResource ? 
			VK_ACCESS_SHADER_READ_BIT : 
			(asSsbo ? 
				VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT :
				VK_ACCESS_UNIFORM_READ_BIT);
		m_resourceSlots.push_back(resource);
	}

	void GcnCompiler::emitDclTexture(
		const GcnShaderResource& res)
	{
		const uint32_t registerId = res.startRegister;
		
		const auto& textureMetaTable = getTextureMetaTable();
		const auto& textureInfo      = textureMetaTable[registerId];

		const bool isStorage = (res.type == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);

		if (isStorage)
		{
			m_module.enableCapability(spv::CapabilityStorageImageReadWithoutFormat);
			m_module.enableCapability(spv::CapabilityStorageImageWriteWithoutFormat);
		}

		Gnm::TextureChannelType channelType = textureInfo.channelType;
		// Declare the actual sampled type
		const GcnScalarType sampledType = [channelType]
		{
			// clang-format off
			switch (channelType)
			{
				// FIXME do we have to manually clamp writes to SNORM/UNORM resources?
				case Gnm::kTextureChannelTypeSNorm: return GcnScalarType::Float32;
				case Gnm::kTextureChannelTypeUNorm: return GcnScalarType::Float32;
				case Gnm::kTextureChannelTypeFloat: return GcnScalarType::Float32;
				case Gnm::kTextureChannelTypeSrgb:  return GcnScalarType::Float32;
				case Gnm::kTextureChannelTypeSInt:  return GcnScalarType::Sint32;
				case Gnm::kTextureChannelTypeUInt:  return GcnScalarType::Uint32;
				default: Logger::exception(util::str::formatex("GcnCompiler: Invalid sampled type: ", channelType));
			}
			// clang-format on
		}();

		// Declare the resource type
		Gnm::TextureType   textureType   = textureInfo.textureType;
		const uint32_t     sampledTypeId = getScalarTypeId(sampledType);
		const GcnImageInfo typeInfo      = getImageType(
				 textureType, isStorage, textureInfo.isDepth);

		// Declare additional capabilities if necessary
		switch (textureType)
		{
			case Gnm::kTextureType1d:
			case Gnm::kTextureType1dArray:
				m_module.enableCapability(isStorage
											  ? spv::CapabilityImage1D
											  : spv::CapabilitySampled1D);
				break;
			default:
				// No additional capabilities required
				break;
		}

		spv::ImageFormat imageFormat = spv::ImageFormatUnknown;

		// If the read-without-format capability is not set and this
		// image is access via a typed load, or if atomic operations
		// are used,, we must define the image format explicitly.
		//if (isStorage)
		//{
		//	if ((m_analysis->uavInfos[registerId].accessAtomicOp) || 
		//		(m_analysis->uavInfos[registerId].accessTypedLoad && 
		//			!m_moduleInfo.options.useStorageImageReadWithoutFormat))
		//		imageFormat = getScalarImageFormat(sampledType);
		//}

		// We do not know whether the image is going to be used as
		// a color image or a depth image yet, but we can pick the
		// correct type when creating a sampled image object.
		const uint32_t imageTypeId = m_module.defImageType(sampledTypeId,
														   typeInfo.dim,
														   typeInfo.depth,
														   typeInfo.array,
														   typeInfo.ms,
														   typeInfo.sampled,
														   imageFormat);

		// We'll declare the texture variable with the color type
		// and decide which one to use when the texture is sampled.
		const uint32_t resourcePtrType = m_module.defPointerType(
			imageTypeId, spv::StorageClassUniformConstant);

		const uint32_t varId = m_module.newVar(resourcePtrType,
											   spv::StorageClassUniformConstant);

		m_module.setDebugName(varId,
							  util::str::formatex(isStorage ? "r" : "t", registerId).c_str());

		// Compute the VLT binding slot index for the resource.
		// Gnm needs to bind the actual resource to this slot.
		uint32_t bindingId = computeResourceBinding(m_programInfo.type(), registerId);

		m_module.decorateDescriptorSet(varId, 0);
		m_module.decorateBinding(varId, bindingId);

		GcnTexture tex;
		tex.imageInfo     = typeInfo;
		tex.varId         = varId;
		tex.sampledType   = sampledType;
		tex.sampledTypeId = sampledTypeId;
		tex.imageTypeId   = imageTypeId;
		tex.colorTypeId   = imageTypeId;
		tex.depthTypeId   = 0;

		if (textureInfo.isDepth &&
			(sampledType == GcnScalarType::Float32) &&
			(textureType == Gnm::kTextureType2d ||
			 textureType == Gnm::kTextureType2dArray ||
			 textureType == Gnm::kTextureTypeCubemap))
		{
			tex.depthTypeId = m_module.defImageType(sampledTypeId,
													typeInfo.dim, 1, typeInfo.array, typeInfo.ms, typeInfo.sampled,
													spv::ImageFormatUnknown);
		}

		m_textures.at(registerId) = tex;

		// Store descriptor info for the shader interface
		VltResourceSlot resource;
		resource.slot   = bindingId;
		resource.view   = typeInfo.vtype;
		resource.access = VK_ACCESS_SHADER_READ_BIT;

		if (isStorage)
		{
			resource.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			resource.access |= VK_ACCESS_SHADER_WRITE_BIT;
		}
		else
		{
			resource.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		}

		m_resourceSlots.push_back(resource);
	}

	void GcnCompiler::emitDclSampler(
		const GcnShaderResource& res)
	{
		const uint32_t samplerId = res.startRegister;

		// The sampler type is opaque, but we still have to
		// define a pointer and a variable in oder to use it
		const uint32_t samplerType    = m_module.defSamplerType();
		const uint32_t samplerPtrType = m_module.defPointerType(
			samplerType, spv::StorageClassUniformConstant);

		// Define the sampler variable
		const uint32_t varId = m_module.newVar(samplerPtrType,
											   spv::StorageClassUniformConstant);
		m_module.setDebugName(varId,
							  util::str::formatex("sampler", samplerId).c_str());

		m_samplers.at(samplerId).varId  = varId;
		m_samplers.at(samplerId).typeId = samplerType;

		// Compute binding slot index for the sampler
		uint32_t bindingId = computeSamplerBinding(
			m_programInfo.type(), samplerId);

		m_module.decorateDescriptorSet(varId, 0);
		m_module.decorateBinding(varId, bindingId);

		// Store descriptor info for the shader interface
		VltResourceSlot resource;
		resource.slot   = bindingId;
		resource.type   = VK_DESCRIPTOR_TYPE_SAMPLER;
		resource.view   = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		resource.access = 0;
		m_resourceSlots.push_back(resource);
	}

	void GcnCompiler::emitDclGprArray()
	{
		// Define sgpr array.
		emitDclGprArray(m_sArray, "s");

		// Define vgpr array.
		emitDclGprArray(m_vArray, "v");
	}

	void GcnCompiler::emitDclGprArray(GcnGprArray&       arrayInfo,
									  const std::string& name)
	{
		uint32_t typeId = getScalarTypeId(GcnScalarType::Float32);

		// Note that mutable arrays will be compiled to
		// registers by GPU driver, so we should make array
		// length as small as possible, or there will be
		// many useless registers and instructions emitted.

		// Define vgpr array.
		arrayInfo.arrayLength      = 1;
		arrayInfo.arrayLengthId    = m_module.lateConst32(getScalarTypeId(GcnScalarType::Uint32));
		const uint32_t arrayTypeId = m_module.defArrayType(typeId, arrayInfo.arrayLengthId);
		const uint32_t ptrTypeId   = m_module.defPointerType(
			  arrayTypeId, spv::StorageClassPrivate);

		arrayInfo.arrayId = m_module.newVar(
			ptrTypeId, spv::StorageClassPrivate);
		m_module.setDebugName(arrayInfo.arrayId, name.c_str());
	}

	void GcnCompiler::emitDclInput(uint32_t             regIdx,
								   GcnInterpolationMode im)
	{
		const GcnVectorType regType = getInputRegType(regIdx);
      
		GcnRegisterInfo info;
		info.type.ctype   = regType.ctype;
		info.type.ccount  = regType.ccount;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassInput;

		const uint32_t varId = emitNewVariable(info);

		m_module.decorateLocation(varId, regIdx);
		m_module.setDebugName(varId, util::str::formatex("i", regIdx).c_str());

		// Record the input so that we can
		// use it in fetch shader.
		GcnRegisterPointer input;
		input.type.ctype          = info.type.ctype;
		input.type.ccount         = info.type.ccount;
		input.id                  = varId;
		m_inputs[regIdx]          = input;

		m_entryPointInterfaces.push_back(varId);

		// Interpolation mode, used in pixel shaders
		if (im == GcnInterpolationMode::Constant)
			m_module.decorate(varId, spv::DecorationFlat);

		if (im == GcnInterpolationMode::LinearCentroid || 
			im == GcnInterpolationMode::LinearNoPerspectiveCentroid)
			m_module.decorate(varId, spv::DecorationCentroid);

		if (im == GcnInterpolationMode::LinearNoPerspective || 
			im == GcnInterpolationMode::LinearNoPerspectiveCentroid || 
			im == GcnInterpolationMode::LinearNoPerspectiveSample)
			m_module.decorate(varId, spv::DecorationNoPerspective);

		if (im == GcnInterpolationMode::LinearSample || 
			im == GcnInterpolationMode::LinearNoPerspectiveSample)
		{
			m_module.enableCapability(spv::CapabilitySampleRateShading);
			m_module.decorate(varId, spv::DecorationSample);
		}

		// Declare the input slot as defined
		m_interfaceSlots.inputSlots |= 1u << regIdx;
	}

	void GcnCompiler::emitDclVertexInput()
	{
		auto table = getSemanticTable();
		
		for (uint32_t i = 0; i != table.second; ++i)
		{
			auto& sema = table.first[i];
			this->emitDclInput(sema.m_semantic,
							   GcnInterpolationMode::Undefined);
		}
	}

	void GcnCompiler::emitDclPsInput()
	{
		uint32_t inputCount = m_meta.ps.inputSemanticCount;
		for (uint32_t i = 0; i != inputCount ; ++i)
		{
			this->emitDclInput(m_meta.ps.semanticMapping[i].m_outIndex,
							   GcnInterpolationMode::Undefined);
		}
	}

	void GcnCompiler::emitDclThreadGroup()
	{
		m_cs.workgroupSizeX = m_meta.cs.computeNumThreadX;
		m_cs.workgroupSizeY = m_meta.cs.computeNumThreadY;
		m_cs.workgroupSizeZ = m_meta.cs.computeNumThreadZ;

		m_module.setLocalSize(m_entryPointId,
							  m_cs.workgroupSizeX,
							  m_cs.workgroupSizeY,
							  m_cs.workgroupSizeZ);
	}

	void GcnCompiler::emitDclThreadGroupSharedMemory(uint32_t size)
	{
		do 
		{
			if (size == 0)
			{
				break;
			}

			GcnRegisterInfo varInfo;
			varInfo.type.ctype   = GcnScalarType::Uint32;
			varInfo.type.ccount  = 1;
			varInfo.type.alength = size / sizeof(uint32_t);
			varInfo.sclass       = spv::StorageClassWorkgroup;

			m_lds = emitNewVariable(varInfo);

			m_module.setDebugName(m_lds, "lds");
		} while (false);
	}

	void GcnCompiler::emitDclOutput(uint32_t        regIdx,
									GcnExportTarget target)
	{
		const GcnVectorType regType = getOutputRegType(regIdx);

		GcnRegisterInfo info;
		info.type.ctype   = regType.ctype;
		info.type.ccount  = regType.ccount;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassOutput;

		const uint32_t varId = this->emitNewVariable(info);
		m_module.setDebugName(varId, util::str::formatex("o", regIdx).c_str());

		if (info.sclass == spv::StorageClassOutput)
		{
			m_module.decorateLocation(varId, regIdx);
			m_entryPointInterfaces.push_back(varId);

			// Add index decoration for potential dual-source blending
			if (m_programInfo.type() == GcnProgramType::PixelShader)
				m_module.decorateIndex(varId, 0);
		}

		switch (target)
		{
			case GcnExportTarget::Mrt:
				m_mrts.at(regIdx) = { regType, varId };
				break;
			case GcnExportTarget::Param:
				m_params.at(regIdx) = { regType, varId };
				break;
			case GcnExportTarget::MrtZ:
			case GcnExportTarget::Null:
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		// Declare the output slot as defined
		m_interfaceSlots.outputSlots |= 1u << regIdx;
	}

	void GcnCompiler::emitDclExport()
	{
		// Declare param outputs
		uint32_t paramCount = m_analysis->exportInfo.paramCount;
		for (uint32_t i = 0; i != paramCount; ++i)
		{
			this->emitDclOutput(i, GcnExportTarget::Param);
		}

		// Declare mrt outputs
		uint32_t mrtCount = m_analysis->exportInfo.mrtCount;
		for (uint32_t i = 0; i != mrtCount; ++i)
		{
			this->emitDclOutput(i, GcnExportTarget::Mrt);
		}
	}

	void GcnCompiler::emitDclStateRegister()
	{
		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassPrivate;

		// M0
		m_state.m0.type.ctype  = GcnScalarType::Uint32;
		m_state.m0.type.ccount = 1;
		m_state.m0.id          = this->emitNewVariable(info);
		m_module.setDebugName(m_state.m0.id, "m0");
		// SCC
		info.type.ctype         = GcnScalarType::Bool;
		m_state.scc.type.ctype  = GcnScalarType::Bool;
		m_state.scc.type.ccount = 1;
		m_state.scc.id          = this->emitNewVariable(info);
		m_module.setDebugName(m_state.scc.id, "scc");
	}

	void GcnCompiler::emitInputSetup()
	{
		m_module.setLateConst(m_vArray.arrayLengthId, &m_vArray.arrayLength);
		m_module.setLateConst(m_sArray.arrayLengthId, &m_sArray.arrayLength);

		GcnRegisterValue ballot;
		ballot.type.ctype  = GcnScalarType::Uint32;
		ballot.type.ccount = 4;
		ballot.id          = m_module.opGroupNonUniformBallot(getVectorTypeId(ballot.type),
															  m_module.constu32(spv::ScopeSubgroup),
															  m_module.constBool(true));
		// Set hardware state register values.
		if (m_moduleInfo.options.separateSubgroup)
		{
			auto low = emitRegisterExtract(ballot, GcnRegMask::select(0));
			// Set high 32 lanes to be inactive.
			m_state.exec.init(low.id, m_module.constu32(0));
		}
		else
		{
			auto low  = emitRegisterExtract(ballot, GcnRegMask::select(0));
			auto high = emitRegisterExtract(ballot, GcnRegMask::select(1));
			m_state.exec.init(low.id, high.id);
		}

		// Init vcc to 0
		m_state.vcc.init(m_module.constu32(0), m_module.constu32(0));

		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:  this->emitVsInputSetup(); break;
			case GcnProgramType::PixelShader:   this->emitPsInputSetup(); break;
			case GcnProgramType::ComputeShader: this->emitCsInputSetup(); break;
			default:
				Logger::exception("shader stage not supported.");
				break;
		}
	}

	void GcnCompiler::emitUserDataInit()
	{
		// Typically, up to 16 user data registers 
		// are used to pass resource descriptors (V#, T# etc.)
		// we don't need to initialize them since we
		// use register id to index resource
	}

	void GcnCompiler::emitVsInputSetup()
	{
		// Initialize SGPR
		emitUserDataInit();
		uint32_t userDataCount = m_meta.vs.userSgprCount;

		
		// Initialize VGPR
		
		// Build a dummy register to index vgpr.
		GcnInstOperand reg = {};
		reg.field          = GcnOperandField::VectorGPR;
		reg.type           = GcnScalarType::Float32;
		
		// v0 stores index of current vertex within vertex buffer
		GcnRegisterValue value = emitVsSystemValueLoad(GcnSystemValue::VertexId, GcnRegMask());
		reg.code               = 0;
		emitVgprStore(reg, value);
	}

	void GcnCompiler::emitPsInputSetup()
	{
		// Build a dummy register to index gpr.
		GcnInstOperand reg = {};
		reg.field          = GcnOperandField::VectorGPR;
		reg.type           = GcnScalarType::Float32;

		// Initialize SGPR
		emitUserDataInit();
		uint32_t userDataCount = m_meta.ps.userSgprCount;

		// Initialize VGPR
		uint32_t vIndex = 0;

		// TODO:
		// What is sample position and what is center position?
		// What's the difference between screen position and them?
		if (m_meta.ps.perspSampleEn)
		{
			GcnRegisterValue value;
			value = emitPsSystemValueLoad(GcnSystemValue::Position, GcnRegMask::select(0));
			reg.code               = vIndex++;
			emitVgprStore(reg, value);

			value = emitPsSystemValueLoad(GcnSystemValue::Position, GcnRegMask::select(1));
			reg.code               = vIndex++;
			emitVgprStore(reg, value);
		}

		if (m_meta.ps.perspCenterEn)
		{
			GcnRegisterValue value;
			value    = emitPsSystemValueLoad(GcnSystemValue::Position, GcnRegMask::select(0));
			reg.code = vIndex++;
			emitVgprStore(reg, value);

			value    = emitPsSystemValueLoad(GcnSystemValue::Position, GcnRegMask::select(1));
			reg.code = vIndex++;
			emitVgprStore(reg, value);
		}

		LOG_ASSERT(m_meta.ps.perspCentroidEn == false, "TODO");
		LOG_ASSERT(m_meta.ps.perspPullModelEn == false, "TODO");
		LOG_ASSERT(m_meta.ps.linearSampleEn == false, "TODO");
		LOG_ASSERT(m_meta.ps.linearCenterEn == false, "TODO");
		LOG_ASSERT(m_meta.ps.linearCentroidEn == false, "TODO");
		if (m_meta.ps.posXEn)
		{
			GcnRegisterValue value = emitPsSystemValueLoad(GcnSystemValue::Position, GcnRegMask::select(0));
			reg.code = vIndex++;
			emitVgprStore(reg, value);
		}
		if (m_meta.ps.posYEn)
		{
			GcnRegisterValue value = emitPsSystemValueLoad(GcnSystemValue::Position, GcnRegMask::select(1));
			reg.code = vIndex++;
			emitVgprStore(reg, value);
		}
	}

	void GcnCompiler::emitCsInputSetup()
	{
		// Initialize SGPR
		emitUserDataInit();
		uint32_t userDataCount = m_meta.cs.userSgprCount;
		uint32_t sIndex        = userDataCount;

		// Build a dummy register to index gpr.
		GcnInstOperand reg;
		reg.field = GcnOperandField::ScalarGPR;
		reg.type  = GcnScalarType::Float32;

		if (m_meta.cs.enableTgidX)
		{
			auto value = emitCsSystemValueLoad(
				GcnSystemValue::WorkgroupId, GcnRegMask::select(0));

			reg.code = sIndex++;
			emitSgprStore(reg, value);
		}

		if (m_meta.cs.enableTgidY)
		{
			auto value = emitCsSystemValueLoad(
				GcnSystemValue::WorkgroupId, GcnRegMask::select(1));

			reg.code = sIndex++;
			emitSgprStore(reg, value);
		}

		if (m_meta.cs.enableTgidZ)
		{
			auto value = emitCsSystemValueLoad(
				GcnSystemValue::WorkgroupId, GcnRegMask::select(2));

			reg.code = sIndex++;
			emitSgprStore(reg, value);
		}

		// Initialize VGPR
		uint32_t vIndex = 0;
		reg.field       = GcnOperandField::VectorGPR;


		// v0 stores gl_LocalInvocationID.x
		GcnRegisterValue value = emitCsSystemValueLoad(
			GcnSystemValue::LocalInvocationId, GcnRegMask::select(0));

		reg.code  = vIndex++;
		emitVgprStore(reg, value);

		if (m_meta.cs.threadIdInGroupCount >= 1)
		{
			value = emitCsSystemValueLoad(
				GcnSystemValue::LocalInvocationId, GcnRegMask::select(1));

			reg.code = vIndex++;
			emitVgprStore(reg, value);
		}
		if (m_meta.cs.threadIdInGroupCount >= 2)
		{
			value = emitCsSystemValueLoad(
				GcnSystemValue::LocalInvocationId, GcnRegMask::select(2));

			reg.code = vIndex++;
			emitVgprStore(reg, value);
		}
	}

	void GcnCompiler::emitFetchInput()
	{
		// Emulate fetch shader,
		// load vertex input into destination vgprs.
		m_vs.fetchFuncId = m_module.allocateId();
		m_module.setDebugName(m_vs.fetchFuncId, "vs_fetch");

		this->emitFunctionBegin(
			m_vs.fetchFuncId,
			m_module.defVoidType(),
			m_module.defFunctionType(
				m_module.defVoidType(), 0, nullptr));
		this->emitFunctionLabel();

		auto tablePair = getSemanticTable();
		const VertexInputSemantic* semaTable = tablePair.first;
		uint32_t                   semaCount = tablePair.second;
		for (uint32_t i = 0; i != semaCount; ++i)
		{
			auto& sema = semaTable[i];

			auto           value = emitValueLoad(m_inputs[sema.m_semantic]);
			GcnInstOperand reg   = {};
			reg.field            = GcnOperandField::VectorGPR;
			reg.code             = sema.m_vgpr;
			this->emitVgprArrayStore(
				reg, value, GcnRegMask::firstN(sema.m_sizeInElements));
		}
	}

	uint32_t GcnCompiler::emitNewVariable(
		const GcnRegisterInfo& info)
	{
		const uint32_t ptrTypeId = this->getPointerTypeId(info);
		return m_module.newVar(ptrTypeId, info.sclass);
	}

	uint32_t GcnCompiler::emitNewBuiltinVariable(
		const GcnRegisterInfo& info,
		spv::BuiltIn           builtIn,
		const char*            name)
	{
		const uint32_t varId = emitNewVariable(info);

		m_module.setDebugName(varId, name);
		m_module.decorateBuiltIn(varId, builtIn);

		if (m_programInfo.type() == GcnProgramType::PixelShader &&
			info.type.ctype != GcnScalarType::Float32 &&
			info.type.ctype != GcnScalarType::Bool &&
			info.sclass == spv::StorageClassInput)
		{
			m_module.decorate(varId, spv::DecorationFlat);
		}
		
		m_entryPointInterfaces.push_back(varId);
		return varId;
	}

	GcnRegisterValue GcnCompiler::emitCommonSystemValueLoad(GcnSystemValue sv, GcnRegMask mask)
	{
		switch (sv)
		{
			case GcnSystemValue::SubgroupEqMask:
			{
				if (m_common.subgroupEqMask == 0)
				{
					m_common.subgroupEqMask = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 4, 0 },
																	   spv::StorageClassInput },
																	 spv::BuiltInSubgroupEqMask,
																	 "subgroup_eq_mask");
				}

				GcnRegisterPointer ptrMask;
				ptrMask.type = { GcnScalarType::Uint32, 4 };
				ptrMask.id   = m_common.subgroupEqMask;

				GcnRegisterValue eqMask = emitValueLoad(ptrMask);

				return emitRegisterExtract(eqMask, mask);
			}
				break;
			case GcnSystemValue::SubgroupGeMask:		
			case GcnSystemValue::SubgroupGtMask:	
			case GcnSystemValue::SubgroupLeMask:
			case GcnSystemValue::SubgroupLtMask:
			case GcnSystemValue::SubgroupSize:
			case GcnSystemValue::SubgroupInvocationID:
			default:
				Logger::exception(util::str::formatex(
					"GcnCompiler: Unhandled Common SV input: ", (uint32_t)sv));
				break;
		}
	}

	GcnRegisterValue GcnCompiler::emitVsSystemValueLoad(
		GcnSystemValue sv,
		GcnRegMask     mask)
	{
		switch (sv)
		{
			case GcnSystemValue::VertexId:
			{
				const uint32_t typeId = getScalarTypeId(GcnScalarType::Uint32);

				if (m_vs.builtinVertexId == 0)
				{
					m_vs.builtinVertexId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																	spv::StorageClassInput },
																  spv::BuiltInVertexIndex,
																  "vs_vertex_index");
				}

				if (m_vs.builtinBaseVertex == 0)
				{
					m_vs.builtinBaseVertex = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																	  spv::StorageClassInput },
																	spv::BuiltInBaseVertex,
																	"vs_base_vertex");
				}

				GcnRegisterValue result;
				result.type.ctype  = GcnScalarType::Uint32;
				result.type.ccount = 1;
				result.id          = m_module.opISub(typeId,
													 m_module.opLoad(typeId, m_vs.builtinVertexId),
													 m_module.opLoad(typeId, m_vs.builtinBaseVertex));
				return result;
			}
			break;

			case GcnSystemValue::InstanceId:
			{
				const uint32_t typeId = getScalarTypeId(GcnScalarType::Uint32);

				if (m_vs.builtinInstanceId == 0)
				{
					m_vs.builtinInstanceId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																	  spv::StorageClassInput },
																	spv::BuiltInInstanceIndex,
																	"vs_instance_index");
				}

				if (m_vs.builtinBaseInstance == 0)
				{
					m_vs.builtinBaseInstance = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																		spv::StorageClassInput },
																	  spv::BuiltInBaseInstance,
																	  "vs_base_instance");
				}

				GcnRegisterValue result;
				result.type.ctype  = GcnScalarType::Uint32;
				result.type.ccount = 1;
				result.id          = m_module.opISub(typeId,
													 m_module.opLoad(typeId, m_vs.builtinInstanceId),
													 m_module.opLoad(typeId, m_vs.builtinBaseInstance));
				return result;
			}
			break;

			default:
				Logger::exception(util::str::formatex(
					"GcnCompiler: Unhandled VS SV input: ", (uint32_t)sv));
		}
	}

	GcnRegisterValue GcnCompiler::emitPsSystemValueLoad(
		GcnSystemValue sv,
		GcnRegMask     mask)
	{
		switch (sv)
		{
			case GcnSystemValue::Position:
			{
				if (m_ps.builtinFragCoord == 0)
				{
					m_ps.builtinFragCoord = emitNewBuiltinVariable({ { GcnScalarType::Float32, 4, 0 },
																	 spv::StorageClassInput },
																   spv::BuiltInFragCoord,
																   "ps_frag_coord");
				}

				GcnRegisterPointer ptrIn;
				ptrIn.type = { GcnScalarType::Float32, 4 };
				ptrIn.id   = m_ps.builtinFragCoord;

				// The X, Y and Z components of the SV_POSITION semantic
				// are identical to Vulkan's FragCoord builtin, but we
				// need to compute the reciprocal of the W component.
				GcnRegisterValue fragCoord = emitValueLoad(ptrIn);

				uint32_t componentIndex = 3;
				uint32_t t_f32          = m_module.defFloatType(32);
				uint32_t v_wComp        = m_module.opCompositeExtract(t_f32, fragCoord.id, 1, &componentIndex);
				v_wComp                 = m_module.opFDiv(t_f32, m_module.constf32(1.0f), v_wComp);

				fragCoord.id = m_module.opCompositeInsert(
					getVectorTypeId(fragCoord.type),
					v_wComp, fragCoord.id,
					1, &componentIndex);

				return emitRegisterExtract(fragCoord, mask);
			}
			break;

			case GcnSystemValue::IsFrontFace:
			{
				if (m_ps.builtinIsFrontFace == 0)
				{
					m_ps.builtinIsFrontFace = emitNewBuiltinVariable({ { GcnScalarType::Bool, 1, 0 },
																	   spv::StorageClassInput },
																	 spv::BuiltInFrontFacing,
																	 "ps_is_front_face");
				}

				GcnRegisterValue result;
				result.type.ctype  = GcnScalarType::Uint32;
				result.type.ccount = 1;
				result.id          = m_module.opSelect(
							 getVectorTypeId(result.type),
							 m_module.opLoad(
								 m_module.defBoolType(),
								 m_ps.builtinIsFrontFace),
							 m_module.constu32(0xFFFFFFFF),
							 m_module.constu32(0x00000000));
				return result;
			}
			break;

			case GcnSystemValue::PrimitiveId:
			{
				if (m_primitiveIdIn == 0)
				{
					m_module.enableCapability(spv::CapabilityGeometry);

					m_primitiveIdIn = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
															   spv::StorageClassInput },
															 spv::BuiltInPrimitiveId,
															 "ps_primitive_id");
				}

				GcnRegisterPointer ptrIn;
				ptrIn.type = { GcnScalarType::Uint32, 1 };
				ptrIn.id   = m_primitiveIdIn;

				return emitValueLoad(ptrIn);
			}
			break;

			case GcnSystemValue::SampleIndex:
			{
				if (m_ps.builtinSampleId == 0)
				{
					m_module.enableCapability(spv::CapabilitySampleRateShading);

					m_ps.builtinSampleId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																	spv::StorageClassInput },
																  spv::BuiltInSampleId,
																  "ps_sample_id");
				}

				GcnRegisterPointer ptrIn;
				ptrIn.type.ctype  = GcnScalarType::Uint32;
				ptrIn.type.ccount = 1;
				ptrIn.id          = m_ps.builtinSampleId;

				return emitValueLoad(ptrIn);
			}
			break;

			case GcnSystemValue::RenderTargetId:
			{
				if (m_ps.builtinLayer == 0)
				{
					m_module.enableCapability(spv::CapabilityGeometry);

					m_ps.builtinLayer = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																 spv::StorageClassInput },
															   spv::BuiltInLayer,
															   "v_layer");
				}

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Uint32;
				ptr.type.ccount = 1;
				ptr.id          = m_ps.builtinLayer;

				return emitValueLoad(ptr);
			}
			break;

			case GcnSystemValue::ViewportId:
			{
				if (m_ps.builtinViewportId == 0)
				{
					m_module.enableCapability(spv::CapabilityMultiViewport);

					m_ps.builtinViewportId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																	  spv::StorageClassInput },
																	spv::BuiltInViewportIndex,
																	"v_viewport");
				}

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Uint32;
				ptr.type.ccount = 1;
				ptr.id          = m_ps.builtinViewportId;

				return emitValueLoad(ptr);
			}
			break;

			default:
				Logger::exception(util::str::formatex(
					"GcnCompiler: Unhandled PS SV input: ", (uint32_t)sv));
		}
	}

	GcnRegisterValue GcnCompiler::emitCsSystemValueLoad(
		GcnSystemValue sv, GcnRegMask mask)
	{
		switch (sv)
		{
			case GcnSystemValue::GlobalInvocationId:
			{
				if (m_cs.builtinGlobalInvocationId == 0)
				{
					m_cs.builtinGlobalInvocationId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 3, 0 },
																			  spv::StorageClassInput },
																			spv::BuiltInGlobalInvocationId,
																			"vThreadId");
				}

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Uint32;
				ptr.type.ccount = 3;
				ptr.id          = m_cs.builtinGlobalInvocationId;
				auto value      = emitValueLoad(ptr);
				return emitRegisterExtract(value, mask);
			}
				break;
			case GcnSystemValue::WorkgroupId:
			{
				if (m_cs.builtinWorkgroupId == 0)
				{
					m_cs.builtinWorkgroupId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 3, 0 },
																	   spv::StorageClassInput },
																	 spv::BuiltInWorkgroupId,
																	 "vGroupId");
				}

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Uint32;
				ptr.type.ccount = 3;
				ptr.id          = m_cs.builtinWorkgroupId;
				auto value      = emitValueLoad(ptr);
				return emitRegisterExtract(value, mask);
			}
				break;
			case GcnSystemValue::LocalInvocationId:
			{
				if (m_cs.builtinLocalInvocationId == 0)
				{
					m_cs.builtinLocalInvocationId = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 3, 0 },
																			 spv::StorageClassInput },
																		   spv::BuiltInLocalInvocationId,
																		   "vThreadIdInGroup");
				}

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Uint32;
				ptr.type.ccount = 3;
				ptr.id          = m_cs.builtinLocalInvocationId;
				auto value      = emitValueLoad(ptr);
				return emitRegisterExtract(value, mask);
			}
				break;
			case GcnSystemValue::LocalInvocationIndex:
			{
				if (m_cs.builtinLocalInvocationIndex == 0)
				{
					m_cs.builtinLocalInvocationIndex = emitNewBuiltinVariable({ { GcnScalarType::Uint32, 1, 0 },
																				spv::StorageClassInput },
																			  spv::BuiltInLocalInvocationIndex,
																			  "vThreadIndexInGroup");
				}

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Uint32;
				ptr.type.ccount = 1;
				ptr.id          = m_cs.builtinLocalInvocationIndex;
				return emitValueLoad(ptr);
			}
				break;
			default:
				Logger::exception(util::str::formatex(
					"GcnCompiler: Unhandled CS SV input: ", (uint32_t)sv));
				break;
		}
	}


	void GcnCompiler::emitVsSystemValueStore(
		GcnSystemValue          sv,
		GcnRegMask              mask,
		const GcnRegisterValue& value)
	{
		switch (sv)
		{
			case GcnSystemValue::Position:
			{
				const uint32_t memberId = m_module.consti32(PerVertex_Position);

				GcnRegisterPointer ptr;
				ptr.type.ctype  = GcnScalarType::Float32;
				ptr.type.ccount = 4;

				ptr.id = m_module.opAccessChain(
					m_module.defPointerType(
						getVectorTypeId(ptr.type),
						spv::StorageClassOutput),
					m_perVertexOut, 1, &memberId);

				emitValueStore(ptr, value, mask);
			}
				break;

			default:
				Logger::warn(util::str::formatex("GcnCompiler: Unhandled VS SV output: ", (uint32_t)sv));
		}
	}

	void GcnCompiler::emitPsSystemValueStore(
		GcnSystemValue          sv,
		GcnRegMask              mask,
		const GcnRegisterValue& value)
	{
	}

	template <bool IsVgpr>
	GcnRegisterPointer sce::gcn::GcnCompiler::emitGetGprPtr(
		const GcnInstOperand& reg)
	{
		GcnGprArray* arrayPtr = nullptr;
		if constexpr (IsVgpr)
		{
			arrayPtr = &m_vArray;
		}
		else
		{
			arrayPtr = &m_sArray;
		}

		uint32_t arrayId      = arrayPtr->arrayId;
		arrayPtr->arrayLength = std::max(arrayPtr->arrayLength, reg.code + 1);

		GcnRegisterPointer result;
		result.type.ctype  = GcnScalarType::Float32;
		result.type.ccount = 1;

		GcnRegisterInfo info;
		info.type.ctype   = result.type.ctype;
		info.type.ccount  = result.type.ccount;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassPrivate;

		uint32_t indexId = m_module.constu32(reg.code);
		result.id        = m_module.opAccessChain(
				   getPointerTypeId(info), arrayId,
				   1, &indexId);

		return result;
	}

	template <bool IsVgpr>
	GcnRegisterValue GcnCompiler::emitGprLoad(
		const GcnInstOperand& reg)
	{
		auto ptr = emitGetGprPtr<IsVgpr>(reg);
		return this->emitValueLoad(ptr);
	}

	template <bool IsVgpr>
	void GcnCompiler::emitGprStore(
		const GcnInstOperand&   reg,
		const GcnRegisterValue& value)
	{
		if (IsVgpr &&
			m_analysis->laneVgprs.find(reg.code) != m_analysis->laneVgprs.end())
		{
			emitLaneVgprStore(reg, value);
		}
		else
		{
			auto ptr = emitGetGprPtr<IsVgpr>(reg);
			this->emitValueStore(ptr, value, GcnRegMask::select(0));
		}
	}

	void GcnCompiler::emitLaneVgprStore(
		const GcnInstOperand&   reg,
		const GcnRegisterValue& value)
	{
		const uint32_t typeId = getScalarTypeId(GcnScalarType::Uint32);
		// For vgprs accessed by lane instructions,
		// we need to make sure the value is set exactly
		// for specific lanes.
		uint32_t condition = 0;
		if (m_moduleInfo.options.separateSubgroup)
		{
			// We only test low 32 lanes,
			// leave high 32 lanes for next neighbor subgroup.
			auto laneMask = emitCommonSystemValueLoad(GcnSystemValue::SubgroupEqMask, GcnRegMask::select(0));
			auto exec     = m_state.exec.emitLoad(GcnRegMask::select(0));

			GcnRegisterValue value;
			value.type  = laneMask.type;
			value.id    = m_module.opBitwiseAnd(typeId, laneMask.id, exec.low.id);
			auto result = emitRegisterZeroTest(value, GcnZeroTest::TestNz);
			condition   = result.id;
		}
		else
		{
			auto lowMask   = emitCommonSystemValueLoad(GcnSystemValue::SubgroupEqMask, GcnRegMask::select(0));
			auto highMask  = emitCommonSystemValueLoad(GcnSystemValue::SubgroupEqMask, GcnRegMask::select(1));
			auto exec      = m_state.exec.emitLoad(GcnRegMask::firstN(2));
			auto lowValue  = m_module.opBitwiseAnd(typeId, lowMask.id, exec.low.id);
			auto highValue = m_module.opBitwiseAnd(typeId, highMask.id, exec.high.id);
			auto value     = m_module.opBitwiseOr(typeId, lowValue, highValue);
			condition      = m_module.opINotEqual(m_module.defBoolType(), value, m_module.constu32(0));
		}

		// Only store vgpr if current invocation is active in exec mask.
		// Keep value unchanged otherwise.
		uint32_t labelBegin = m_module.allocateId();
		uint32_t labelEnd   = m_module.allocateId();
		m_module.opSelectionMerge(labelEnd, spv::SelectionControlMaskNone);
		m_module.opBranchConditional(condition, labelBegin, labelEnd);
		m_module.opLabel(labelBegin);

		auto ptr = emitGetGprPtr<true>(reg);
		this->emitValueStore(ptr, value, GcnRegMask::select(0));

		m_module.opBranch(labelEnd);
		m_module.opLabel(labelEnd);
	}

	template <bool IsVgpr>
	GcnRegisterValue GcnCompiler::emitGprArrayLoad(
		const GcnInstOperand& start,
		const GcnRegMask&     mask)
	{
		std::array<uint32_t, 4> gpr;
		uint32_t                componentCount = mask.popCount();
		for (uint32_t i = 0; i != componentCount; ++i)
		{
			GcnInstOperand reg = start;
			reg.code += i;
			gpr[i] = emitGprLoad<IsVgpr>(reg).id;
		}

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Float32;
		result.type.ccount = componentCount;
		result.id          = m_module.opCompositeConstruct(getVectorTypeId(result.type),
														   componentCount,
														   gpr.data());
		return result;
	}

	template <bool IsVgpr>
	void GcnCompiler::emitGprArrayStore(
		const GcnInstOperand&   start,
		const GcnRegisterValue& value,
		const GcnRegMask&       mask)
	{
		// store values in a vector into vgpr array,
		// e.g. vec3 -> v[4:6]

		uint32_t gprIdx = 0;
		for (uint32_t i = 0; i != value.type.ccount; ++i)
		{
			if (!mask[i])
			{
				continue;
			}

			uint32_t typeId = this->getScalarTypeId(value.type.ctype);
			uint32_t valueId = 0;
			if (mask.popCount() == 1)
			{
				valueId = value.id;
			}
			else
			{
				valueId = m_module.opCompositeExtract(
					typeId, value.id, 1, &i);
			}


			GcnInstOperand reg = start;
			reg.code += (gprIdx++);

			GcnRegisterValue val;
			val.type.ctype  = value.type.ctype;
			val.type.ccount = 1;
			val.id          = valueId;

			this->emitGprStore<IsVgpr>(reg, val);
		}
	}

	GcnRegisterValue GcnCompiler::emitVgprLoad(
		const GcnInstOperand& reg)
	{
		return this->emitGprLoad<true>(reg);
	}

	void GcnCompiler::emitVgprStore(
		const GcnInstOperand&   reg,
		const GcnRegisterValue& value)
	{
		this->emitGprStore<true>(reg, value);
	}

	GcnRegisterValue GcnCompiler::emitVgprArrayLoad(
		const GcnInstOperand& start,
		const GcnRegMask&     mask)
	{
		return this->emitGprArrayLoad<true>(start, mask);
	}

	void GcnCompiler::emitVgprArrayStore(
		const GcnInstOperand&   start,
		const GcnRegisterValue& value,
		const GcnRegMask&       mask)
	{
		this->emitGprArrayStore<true>(start, value, mask);
	}

	GcnRegisterValue GcnCompiler::emitSgprLoad(
		const GcnInstOperand& reg)
	{
		return this->emitGprLoad<false>(reg);
	}

	void GcnCompiler::emitSgprStore(
		const GcnInstOperand&   reg,
		const GcnRegisterValue& value)
	{
		this->emitGprStore<false>(reg, value);
	}

	GcnRegisterValue GcnCompiler::emitSgprArrayLoad(
		const GcnInstOperand& start,
		const GcnRegMask&     mask)
	{
		return this->emitGprArrayLoad<false>(start, mask);
	}

	void GcnCompiler::emitSgprArrayStore(
		const GcnInstOperand&   start,
		const GcnRegisterValue& value,
		const GcnRegMask&       mask)
	{
		this->emitGprArrayStore<false>(start, value, mask);
	}

	GcnRegisterValue GcnCompiler::emitValueLoad(
		GcnRegisterPointer ptr)
	{
		GcnRegisterValue result;
		result.type = ptr.type;
		result.id   = m_module.opLoad(
			  getVectorTypeId(result.type),
			  ptr.id);
		return result;
	}

	void GcnCompiler::emitValueStore(
		GcnRegisterPointer ptr,
		GcnRegisterValue   value,
		GcnRegMask         writeMask)
	{
		// If the component types are not compatible,
		// we need to bit-cast the source variable.
		if (value.type.ctype != ptr.type.ctype)
			value = emitRegisterBitcast(value, ptr.type.ctype);

		// If the source value consists of only one component,
		// it is stored in all components of the destination.
		if (value.type.ccount == 1)
			value = emitRegisterExtend(value, writeMask.popCount());

		if (ptr.type.ccount == writeMask.popCount())
		{
			// Simple case: We write to the entire register
			m_module.opStore(ptr.id, value.id);
		}
		else
		{
			// We only write to part of the destination
			// register, so we need to load and modify it
			GcnRegisterValue tmp = emitValueLoad(ptr);
			tmp                  = emitRegisterInsert(tmp, value, writeMask);

			m_module.opStore(ptr.id, tmp.id);
		}
	}

	GcnRegisterValuePair GcnCompiler::emitRegisterLoad(
		const GcnInstOperand& reg)
	{
		GcnRegisterValuePair result = {};

		LOG_ASSERT(reg.type != GcnScalarType::Float64, "TODO: support float64.");
		bool is64BitsType = this->isDoubleType(reg.type);

		auto field = reg.field;
		switch (field)
		{
			case GcnOperandField::ScalarGPR:
			{
				result.low = this->emitSgprLoad(reg);
				if (is64BitsType)
				{
					GcnInstOperand highReg = reg;
					highReg.code += 1;
					result.high = this->emitSgprLoad(highReg);
				}
			}
				break;
			case GcnOperandField::VccLo:
				result = m_state.vcc.emitLoad(
					GcnRegMask::firstN(is64BitsType ? 2 : 1));
				break;
			case GcnOperandField::VccHi:
				result = m_state.vcc.emitLoad(
					GcnRegMask::select(1));
				break;
			case GcnOperandField::M0:
				result.low = emitValueLoad(m_state.m0);
				break;
			case GcnOperandField::ExecLo:
				result = m_state.exec.emitLoad(
					GcnRegMask::firstN(is64BitsType ? 2 : 1));
				break;
			case GcnOperandField::ExecHi:
				result = m_state.exec.emitLoad(
					GcnRegMask::select(1));
				break;
			case GcnOperandField::ConstZero:
			case GcnOperandField::SignedConstIntPos:
			case GcnOperandField::SignedConstIntNeg:
			case GcnOperandField::ConstFloatPos_0_5:
			case GcnOperandField::ConstFloatNeg_0_5:
			case GcnOperandField::ConstFloatPos_1_0:
			case GcnOperandField::ConstFloatNeg_1_0:
			case GcnOperandField::ConstFloatPos_2_0:
			case GcnOperandField::ConstFloatNeg_2_0:
			case GcnOperandField::ConstFloatPos_4_0:
			case GcnOperandField::ConstFloatNeg_4_0:
				result = emitBuildInlineConst(reg);
				break;
			case GcnOperandField::VccZ:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::ExecZ:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::Scc:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::LdsDirect:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::LiteralConst:
				result = emitBuildLiteralConst(reg);
				break;
			case GcnOperandField::VectorGPR:
			{
				result.low = this->emitVgprLoad(reg);
				if (is64BitsType)
				{
					GcnInstOperand highReg = reg;
					highReg.code += 1;
					result.high = this->emitVgprLoad(highReg);
				}
			}
				break;
			case GcnOperandField::Undefined:
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		result = emitRegisterBitcast(result, reg.type);

		result.low = emitInputModifiers(result.low, reg.inputModifier);

		return result;
	}

	void GcnCompiler::emitRegisterStore(
		const GcnInstOperand&       reg,
		const GcnRegisterValuePair& value)
	{
		// Apply output modifier first
		GcnRegisterValuePair src = value;
		src.low                  = emitOutputModifiers(src.low, reg.outputModifier);

		bool is64BitsType = this->isDoubleType(reg.type);

		auto field = reg.field;
		switch (field)
		{
			case GcnOperandField::ScalarGPR:
			{
				emitSgprStore(reg, src.low);
				if (is64BitsType)
				{
					GcnInstOperand highReg = reg;
					highReg.code += 1;
					emitSgprStore(highReg, src.high);
				}
			}
			break;
			case GcnOperandField::VccLo:
				m_state.vcc.emitStore(src,
									  GcnRegMask::firstN(is64BitsType ? 2 : 1));
				break;
			case GcnOperandField::VccHi:
				m_state.vcc.emitStore(src,
									  GcnRegMask::select(1));
				break;
			case GcnOperandField::M0:
				emitValueStore(m_state.m0,
							   value.low,
							   GcnRegMask::select(0));
				break;
			case GcnOperandField::ExecLo:
				m_state.exec.emitStore(src,
									   GcnRegMask::firstN(is64BitsType ? 2 : 1));
				break;
			case GcnOperandField::ExecHi:
				m_state.exec.emitStore(src,
									   GcnRegMask::select(1));
				break;
			case GcnOperandField::VccZ:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::ExecZ:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::Scc:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::LdsDirect:
				LOG_GCN_UNHANDLED_INST();
				break;
			case GcnOperandField::VectorGPR:
			{
				emitVgprStore(reg, src.low);
				if (is64BitsType)
				{
					GcnInstOperand highReg = reg;
					highReg.code += 1;
					emitVgprStore(highReg, src.high);
				}
			}
				break;
			case GcnOperandField::Undefined:
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	GcnRegisterPointer GcnCompiler::emitVectorAccess(
		GcnRegisterPointer pointer,
		spv::StorageClass  sclass,
		const GcnRegMask&  mask)
	{
		// Create a pointer into a vector object
		LOG_ASSERT(mask.popCount() == 1, "mask can only select one component");

		GcnRegisterPointer result;
		result.type.ctype  = pointer.type.ctype;
		result.type.ccount = mask.popCount();

		uint32_t ptrTypeId = m_module.defPointerType(
			getVectorTypeId(result.type), sclass);

		uint32_t indexId = m_module.constu32(mask.firstSet());
		result.id        = m_module.opAccessChain(
				   ptrTypeId, pointer.id, 1, &indexId);
		return result;
	}

	GcnRegisterValue GcnCompiler::emitIndexLoad(
		const GcnRegIndex& index)
	{
		GcnRegisterValue result;
		if (index.relReg != nullptr)
		{
			result = emitRegisterLoad(*index.relReg).low;

			if (index.offset != 0)
			{
				result.id = m_module.opIAdd(
					getVectorTypeId(result.type), result.id,
					m_module.consti32(index.offset));
			}
		}
		else
		{
			result.type.ctype  = GcnScalarType::Uint32;
			result.type.ccount = 1;
			result.id          = m_module.constu32(index.offset);
		}
		return result;
	}

	std::vector<GcnRegisterPointer>
	GcnCompiler::emitUniformBufferAccess(
		uint32_t bufferId, uint32_t baseId, uint32_t count)
	{
		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Float32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassUniform;

		uint32_t uintTypeId = getScalarTypeId(GcnScalarType::Uint32);
		uint32_t ptrTypeId  = getPointerTypeId(info);

		std::vector<GcnRegisterPointer> result;
		result.reserve(count);
		for (uint32_t i = 0; i != count; ++i)
		{
			// Uniform buffers are vec4 arrays
			
			// offset in bytes
			uint32_t offsetId    = m_module.opIAdd(uintTypeId,
												   baseId,
												   m_module.constu32(i * 4));
			uint32_t vec4Id      = m_module.opUDiv(uintTypeId,
												   offsetId,
												   m_module.constu32(16));
			uint32_t componentId = m_module.opUDiv(uintTypeId,
												   m_module.opUMod(uintTypeId,
																   offsetId,
																   m_module.constu32(16)),
												   m_module.constu32(4));

			const std::array<uint32_t, 3> indices = { { m_module.consti32(0), vec4Id, componentId } };

			uint32_t componentPtr = m_module.opAccessChain(ptrTypeId,
														   bufferId,
														   indices.size(), indices.data());
			GcnRegisterPointer ptr;
			ptr.type.ctype  = info.type.ctype;
			ptr.type.ccount = 1;
			ptr.id          = componentPtr;
			result.push_back(ptr);
		}
		return result;
	}

	std::vector<GcnRegisterPointer>
	GcnCompiler::emitStorageBufferAccess(
		uint32_t bufferId, uint32_t baseId, uint32_t count)
	{
		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassUniform;

		uint32_t uintTypeId = getScalarTypeId(GcnScalarType::Uint32);
		uint32_t ptrTypeId  = getPointerTypeId(info);

		std::vector<GcnRegisterPointer> result;
		result.reserve(count);
		for (uint32_t i = 0; i != count; ++i)
		{
			// Storage buffers are uint arrays
			uint32_t offsetId = m_module.opIAdd(uintTypeId,
												baseId,
												m_module.constu32(i * 4));

			uint32_t uintIdx = m_module.opUDiv(uintTypeId,
											   offsetId,
											   m_module.constu32(4));

			const std::array<uint32_t, 2> indices = { { m_module.constu32(0), uintIdx } };

			uint32_t componentPtr = m_module.opAccessChain(ptrTypeId,
														   bufferId,
														   indices.size(), indices.data());

			GcnRegisterPointer ptr;
			ptr.type.ctype  = info.type.ctype;
			ptr.type.ccount = 1;
			ptr.id          = componentPtr;
			result.push_back(ptr);
		}
		return result;
	}

	void GcnCompiler::emitConstantBufferLoad(
		const GcnRegIndex&    index,
		const GcnInstOperand& dst,
		uint32_t              count)
	{
		uint32_t         regId  = index.regIdx;
		GcnRegisterValue baseId = emitIndexLoad(index);

		uint32_t fpTypeId = getScalarTypeId(GcnScalarType::Float32);



		auto ptrList = emitUniformBufferAccess(m_buffers.at(regId).varId,
											   baseId.id,
											   count);

		for (uint32_t i = 0; i != count ; ++i)
		{
			const auto&          ptr   = ptrList[i];
			GcnRegisterValuePair value = {};
			value.low.type             = ptr.type;
			value.low.id               = m_module.opLoad(fpTypeId,
														 ptr.id);

			GcnInstOperand reg = dst;
			reg.code += i;
			emitRegisterStore(reg, value);
		}
	}

	GcnRegisterValue GcnCompiler::emitCalcTexCoord(
		GcnRegisterValue    coordVector,
		const GcnImageInfo& imageInfo)
	{
		const uint32_t dim = getTexCoordDim(imageInfo);

		if (dim != coordVector.type.ccount)
		{
			coordVector = emitRegisterExtract(
				coordVector, GcnRegMask::firstN(dim));
		}

		return coordVector;
	}

	GcnRegisterValue GcnCompiler::emitLoadTexCoord(
		const GcnInstOperand& coordReg,
		const GcnImageInfo&   imageInfo)
	{
		uint32_t dim         = getTexCoordDim(imageInfo);
		auto     coordVector = emitVgprArrayLoad(coordReg,
												 GcnRegMask::firstN(dim));

		auto result = emitCalcTexCoord(coordVector, imageInfo);

		if (imageInfo.dim == spv::DimCube)
		{
			// Why do we need recover?
			// See comments in emitCubeCalculate.
			result = emitRecoverCubeCoord(result);
		}
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRecoverCubeCoord(
		const GcnRegisterValue& coord)
	{
		LOG_ASSERT(coord.type.ccount == 3, "cube coordinate must be vec3.");
		auto s = emitRegisterExtract(coord, GcnRegMask::select(0));
		auto t = emitRegisterExtract(coord, GcnRegMask::select(1));
		auto z = emitRegisterExtract(coord, GcnRegMask::select(2));

		const uint32_t typeId = getScalarTypeId(GcnScalarType::Float32);

		// We need to fix x and y coordinate,
		// because the s and t coordinate will be scaled and plus 1.5
		// by v_madak_f32.
		// We already force the scale value to be 1.0 when handling v_cubema_f32,
		// here we subtract 1.5 to recover the original value. 
		auto x = m_module.opFSub(typeId, s.id, m_module.constf32(1.5));
		auto y = m_module.opFSub(typeId, t.id, m_module.constf32(1.5));

		std::array<uint32_t, 3> direction = { x, y, z.id };

		GcnRegisterValue result;
		result.type = coord.type;
		result.id   = m_module.opCompositeConstruct(getVectorTypeId(result.type),
													direction.size(),
													direction.data());
		return result;
	}

	uint32_t GcnCompiler::emitLoadSampledImage(
		const GcnTexture& textureResource,
		const GcnSampler& samplerResource,
		bool              isDepthCompare)
	{
		const uint32_t sampledImageType = isDepthCompare
											  ? m_module.defSampledImageType(textureResource.depthTypeId)
											  : m_module.defSampledImageType(textureResource.colorTypeId);

		return m_module.opSampledImage(sampledImageType,
									   m_module.opLoad(textureResource.imageTypeId, textureResource.varId),
									   m_module.opLoad(samplerResource.typeId, samplerResource.varId));
	}

	void GcnCompiler::emitTextureSample(const GcnShaderInstruction& ins)
	{
		auto                  mimg        = gcnInstructionAs<GcnShaderInstMIMG>(ins);
		const GcnInstOperand& texCoordReg = mimg.vaddr;
		const GcnInstOperand& textureReg  = mimg.srsrc;
		const GcnInstOperand& samplerReg  = mimg.ssamp;

		// Texture and sampler register IDs
		// These registers are 4-GPR aligned, so multiplied by 4
		const uint32_t textureId = textureReg.code * 4;
		const uint32_t samplerId = samplerReg.code * 4;

		// Image type, which stores the image dimensions etc.
		const GcnImageInfo imageType = m_textures.at(textureId).imageInfo;

		// Load the texture coordinates. SPIR-V allows these
		// to be float4 even if not all components are used.
		GcnRegisterValue coord = emitLoadTexCoord(texCoordReg, imageType);
		
		// Accumulate additional image operands. These are
		// not part of the actual operand token in SPIR-V.
		SpirvImageOperands imageOperands = {};

		auto op             = ins.opcode;
		bool isDepthCompare = op == GcnOpcode::IMAGE_SAMPLE_C ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_CL ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_D ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_D_CL ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_L ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_B ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_B_CL ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_LZ ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_CL_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_D_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_D_CL_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_L_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_B_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_B_CL_O ||
							  op == GcnOpcode::IMAGE_SAMPLE_C_LZ_O;

		// Combine the texture and the sampler into a sampled image
		const uint32_t sampledImageId = emitLoadSampledImage(
			m_textures.at(textureId), m_samplers.at(samplerId),
			isDepthCompare);

		// Sampling an image always returns a four-component
		// vector, whereas depth-compare ops return a scalar.
		GcnRegisterValue result;
		result.type.ctype  = m_textures.at(textureId).sampledType;
		result.type.ccount = isDepthCompare ? 1 : 4;

		switch (op)
		{
			// Simple image sample operation
			case GcnOpcode::IMAGE_SAMPLE:
			{
				result.id = m_module.opImageSampleImplicitLod(
					getVectorTypeId(result.type),
					sampledImageId, coord.id,
					imageOperands);
			}
			break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		auto colorMask = GcnRegMask(mimg.control.dmask);
		result         = emitRegisterExtract(result, colorMask);
		emitVgprArrayStore(mimg.vdata,
						   result,
						   colorMask);
	}


	GcnRegisterValue GcnCompiler::emitBuildConstVecf32(
		float             x,
		float             y,
		float             z,
		float             w,
		const GcnRegMask& writeMask)
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

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Float32;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstVecu32(
		uint32_t          x,
		uint32_t          y,
		uint32_t          z,
		uint32_t          w,
		const GcnRegMask& writeMask)
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

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstVeci32(
		int32_t           x,
		int32_t           y,
		int32_t           z,
		int32_t           w,
		const GcnRegMask& writeMask)
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

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Sint32;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstVecf64(
		double            xy,
		double            zw,
		const GcnRegMask& writeMask)
	{
		std::array<uint32_t, 2> ids            = { 0, 0 };
		uint32_t                componentIndex = 0;

		if (writeMask[0] && writeMask[1])
			ids[componentIndex++] = m_module.constf64(xy);
		if (writeMask[2] && writeMask[3])
			ids[componentIndex++] = m_module.constf64(zw);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Float64;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstValue(
		size_t value, GcnScalarType type)
	{
		GcnRegisterValue result;
		result.type.ctype  = type;
		result.type.ccount = 1;
		switch (type)
		{
			case sce::gcn::GcnScalarType::Uint32:
				result.id = m_module.constu32(static_cast<uint32_t>(value));
				break;
			case sce::gcn::GcnScalarType::Sint32:
				result.id = m_module.consti32(static_cast<int32_t>(value));
				break;
			case sce::gcn::GcnScalarType::Float32:
				result.id = m_module.constf32(static_cast<float>(value));
				break;
			case sce::gcn::GcnScalarType::Bool:
				result.id = m_module.constBool(static_cast<bool>(value));
				break;
			default:
				LOG_ASSERT(false, "type not supported");
				break;
		}
		return result;
	}


	GcnRegisterValuePair GcnCompiler::emitBuildLiteralConst(
		const GcnInstOperand& reg)
	{
		LOG_ASSERT(!isDoubleType(reg.type), "TODO: support 64 bis types.");

		GcnRegisterValuePair result = {};
		result.low.type.ctype       = reg.type;
		result.low.type.ccount      = 1;

		switch (reg.type)
		{
			case GcnScalarType::Uint32:
				result.low.id = m_module.constu32(reg.literalConst);
				break;
			case GcnScalarType::Sint32:
				result.low.id = m_module.consti32(reg.literalConst);
				break;
			case GcnScalarType::Float32:
				result.low.id = m_module.constf32(*reinterpret_cast<const float*>(&reg.literalConst));
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
		return result;
	}

	GcnRegisterValuePair GcnCompiler::emitBuildInlineConst(const GcnInstOperand& reg)
	{
		GcnRegisterValuePair result = {};
		result.low.type.ctype       = (reg.type == GcnScalarType::Uint64 ||
									   reg.type == GcnScalarType::Sint64) ? getHalfType(reg.type) : reg.type;
		result.low.type.ccount      = 1;
		result.high.type            = result.low.type;
		
		bool doubleType = isDoubleType(reg.type);
		auto field = reg.field;
		switch (field)
		{
			case GcnOperandField::ConstZero:
			{
				result.low.id         = m_module.consti32(0);
				result.low.type.ctype = GcnScalarType::Sint32;
				if (doubleType)
				{
					result.high.id         = m_module.consti32(0);
					result.high.type.ctype = GcnScalarType::Sint32;
				}
			}
				break;
			case GcnOperandField::SignedConstIntPos:
			{
				constexpr int32_t InlineConstZero = 128;
				int32_t           value           = reg.code - InlineConstZero;
				result.low.id                     = m_module.consti32(value);
				result.low.type.ctype             = GcnScalarType::Sint32;
				if (doubleType)
				{
					result.high.id = m_module.consti32(0);
					result.high.type.ctype = GcnScalarType::Sint32;
				}
			}
				break;
			case GcnOperandField::SignedConstIntNeg:
			{
				constexpr int32_t InlineConst64 = 192;
				int32_t           value         = InlineConst64 - reg.code;
				result.low.id                   = m_module.consti32(value);
				result.low.type.ctype           = GcnScalarType::Sint32;
				if (doubleType)
				{
					result.high.id = m_module.consti32(0);
					result.high.type.ctype = GcnScalarType::Sint32;
				}
			}
				break;
			case GcnOperandField::ConstFloatPos_0_5:
				result.low.id = doubleType ? m_module.constf64(0.5f) : m_module.constf32(0.5f);
				break;
			case GcnOperandField::ConstFloatNeg_0_5:
				result.low.id = doubleType ? m_module.constf64(-0.5f) : m_module.constf32(-0.5f);
				break;
			case GcnOperandField::ConstFloatPos_1_0:
				result.low.id = doubleType ? m_module.constf64(1.0f) : m_module.constf32(1.0f);
				break;
			case GcnOperandField::ConstFloatNeg_1_0:
				result.low.id = doubleType ? m_module.constf64(-1.0f) : m_module.constf32(-1.0f);
				break;
			case GcnOperandField::ConstFloatPos_2_0:
				result.low.id = doubleType ? m_module.constf64(2.0f) : m_module.constf32(2.0f);
				break;
			case GcnOperandField::ConstFloatNeg_2_0:
				result.low.id = doubleType ? m_module.constf64(-2.0f) : m_module.constf32(-2.0f);
				break;
			case GcnOperandField::ConstFloatPos_4_0:
				result.low.id = doubleType ? m_module.constf64(4.0f) : m_module.constf32(4.0f);
				break;
			case GcnOperandField::ConstFloatNeg_4_0:
				result.low.id = doubleType ? m_module.constf64(-4.0f) : m_module.constf32(-4.0f);
				break;
		}

		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterBitcast(
		GcnRegisterValue srcValue,
		GcnScalarType    dstType)
	{
		GcnScalarType srcType = srcValue.type.ctype;

		if (srcType == dstType)
			return srcValue;

		GcnRegisterValue result;
		result.type.ctype  = dstType;
		result.type.ccount = srcValue.type.ccount;

		if (isDoubleType(srcType))
			result.type.ccount *= 2;
		if (isDoubleType(dstType))
			result.type.ccount /= 2;

		result.id = m_module.opBitcast(
			getVectorTypeId(result.type),
			srcValue.id);
		return result;
	}

	GcnRegisterValuePair GcnCompiler::emitRegisterBitcast(
		GcnRegisterValuePair srcValue,
		GcnScalarType        dstType)
	{
		// Cast gpr pair

		GcnRegisterValuePair result = srcValue;

		LOG_ASSERT(dstType != GcnScalarType::Float64, "TODO: support float64");
		bool castHighPart = isDoubleType(dstType);

		GcnScalarType resultType = dstType;
		if (castHighPart)
		{
			if (dstType == GcnScalarType::Uint64)
			{
				resultType = GcnScalarType::Uint32;
			}
			else if (dstType == GcnScalarType::Sint64)
			{
				resultType = GcnScalarType::Sint32;
			}
			else
			{
				LOG_ASSERT(false, "not supported.");
			}
		}

		result.low = emitRegisterBitcast(result.low, resultType);
		if (castHighPart)
		{
			result.high = emitRegisterBitcast(result.high, resultType);
		}
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterSwizzle(
		GcnRegisterValue value,
		GcnRegSwizzle    swizzle,
		GcnRegMask       writeMask)
	{
		if (value.type.ccount == 1)
			return emitRegisterExtend(value, writeMask.popCount());

		std::array<uint32_t, 4> indices;

		uint32_t dstIndex = 0;

		for (uint32_t i = 0; i < 4; i++)
		{
			if (writeMask[i])
				indices[dstIndex++] = swizzle[i];
		}

		// If the swizzle combined with the mask can be reduced
		// to a no-op, we don't need to insert any instructions.
		bool isIdentitySwizzle = dstIndex == value.type.ccount;

		for (uint32_t i = 0; i < dstIndex && isIdentitySwizzle; i++)
			isIdentitySwizzle &= indices[i] == i;

		if (isIdentitySwizzle)
			return value;

		// Use OpCompositeExtract if the resulting vector contains
		// only one component, and OpVectorShuffle if it is a vector.
		GcnRegisterValue result;
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

	GcnRegisterValue GcnCompiler::emitRegisterExtract(
		GcnRegisterValue value,
		GcnRegMask       mask)
	{
		return emitRegisterSwizzle(value,
								   GcnRegSwizzle(0, 1, 2, 3), mask);
	}

	GcnRegisterValue GcnCompiler::emitRegisterInsert(
		GcnRegisterValue dstValue,
		GcnRegisterValue srcValue,
		GcnRegMask       srcMask)
	{
		GcnRegisterValue result;
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
			uint32_t                srcComponentId = dstValue.type.ccount;

			for (uint32_t i = 0; i < dstValue.type.ccount; i++)
				components.at(i) = srcMask[i] ? srcComponentId++ : i;

			result.id = m_module.opVectorShuffle(
				typeId, dstValue.id, srcValue.id,
				dstValue.type.ccount, components.data());
		}

		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterConcat(
		GcnRegisterValue value1,
		GcnRegisterValue value2)
	{
		std::array<uint32_t, 2> ids = { { value1.id, value2.id } };

		GcnRegisterValue result;
		result.type.ctype  = value1.type.ctype;
		result.type.ccount = value1.type.ccount + value2.type.ccount;
		result.id          = m_module.opCompositeConstruct(
					 getVectorTypeId(result.type),
					 ids.size(), ids.data());
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterExtend(
		GcnRegisterValue value,
		uint32_t         size)
	{
		if (size == 1)
			return value;

		std::array<uint32_t, 4> ids = { {
			value.id,
			value.id,
			value.id,
			value.id,
		} };

		GcnRegisterValue result;
		result.type.ctype  = value.type.ctype;
		result.type.ccount = size;
		result.id          = m_module.opCompositeConstruct(
					 getVectorTypeId(result.type),
					 size, ids.data());
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterAbsolute(
		GcnRegisterValue value)
	{
		const uint32_t typeId = getVectorTypeId(value.type);

		switch (value.type.ctype)
		{
		case GcnScalarType::Float32:
			value.id = m_module.opFAbs(typeId, value.id);
			break;
		case GcnScalarType::Sint32:
			value.id = m_module.opSAbs(typeId, value.id);
			break;
		default:
			Logger::warn("GcnCompiler: Cannot get absolute value for given type");
		}

		return value;
	}

	GcnRegisterValue GcnCompiler::emitRegisterNegate(
		GcnRegisterValue value)
	{
		const uint32_t typeId = getVectorTypeId(value.type);

		switch (value.type.ctype)
		{
		case GcnScalarType::Float32:
			value.id = m_module.opFNegate(typeId, value.id);
			break;
		case GcnScalarType::Float64:
			value.id = m_module.opFNegate(typeId, value.id);
			break;
		case GcnScalarType::Sint32:
			value.id = m_module.opSNegate(typeId, value.id);
			break;
		case GcnScalarType::Sint64:
			value.id = m_module.opSNegate(typeId, value.id);
			break;
		default:
			Logger::warn("GcnCompiler: Cannot negate given type");
		}

		return value;
	}

	GcnRegisterValue GcnCompiler::emitRegisterZeroTest(
		GcnRegisterValue value,
		GcnZeroTest      test)
	{
		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Bool;
		result.type.ccount = 1;

		const uint32_t zeroId = m_module.constu32(0u);
		const uint32_t typeId = getVectorTypeId(result.type);

		result.id = test == GcnZeroTest::TestZ
						? m_module.opIEqual(typeId, value.id, zeroId)
						: m_module.opINotEqual(typeId, value.id, zeroId);
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterMaskBits(
		GcnRegisterValue value,
		uint32_t         mask)
	{
		GcnRegisterValue maskVector = emitBuildConstVecu32(
			mask, mask, mask, mask, GcnRegMask::firstN(value.type.ccount));

		GcnRegisterValue result;
		result.type = value.type;
		result.id   = m_module.opBitwiseAnd(
			  getVectorTypeId(result.type),
			  value.id, maskVector.id);
		return result;
	}

	GcnRegisterValue GcnCompiler::emitInputModifiers(
		GcnRegisterValue  value,
		GcnInputModifiers modifiers)
	{
		if (modifiers.test(GcnInputModifier::Abs))
			value = emitRegisterAbsolute(value);

		if (modifiers.test(GcnInputModifier::Neg))
			value = emitRegisterNegate(value);
		return value;
	}

	GcnRegisterValue GcnCompiler::emitOutputModifiers(
		GcnRegisterValue   value,
		GcnOutputModifiers modifiers)
	{
		const uint32_t typeId = getVectorTypeId(value.type);

		// Output modifier only makes sense on floats
		if (isFloatType(value.type.ctype))
		{
			if (!std::isnan(modifiers.multiplier))
			{
				uint32_t multiplierId = m_module.constf32(modifiers.multiplier);
				value.id              = m_module.opFMul(typeId, value.id, multiplierId);
			}

			if (modifiers.clamp)
			{
				const GcnRegMask       mask = GcnRegMask::firstN(value.type.ccount);
				const GcnRegisterValue vec0 = emitBuildConstVecf32(0.0f, 0.0f, 0.0f, 0.0f, mask);
				const GcnRegisterValue vec1 = emitBuildConstVecf32(1.0f, 1.0f, 1.0f, 1.0f, mask);

				value.id = m_module.opNClamp(typeId, value.id, vec0.id, vec1.id);
			}
		}

		return value;
	}

	GcnRegisterValue GcnCompiler::emitPackHalf2x16(
		GcnRegisterValuePair src)
	{
		const uint32_t t_u32   = getVectorTypeId({ GcnScalarType::Uint32, 1 });
		const uint32_t t_f32v2 = getVectorTypeId({ GcnScalarType::Float32, 2 });

		const std::array<uint32_t, 2> packIds = { { src.low.id, src.high.id } };

		uint32_t uintId = m_module.opPackHalf2x16(t_u32,
												  m_module.opCompositeConstruct(t_f32v2,
																				packIds.size(),
																				packIds.data()));

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = 1;
		result.id          = uintId;
		return result;
	}

	GcnRegisterValuePair GcnCompiler::emitUnpackHalf2x16(
		GcnRegisterValue src)
	{
		const uint32_t t_f32   = getVectorTypeId({ GcnScalarType::Float32, 1 });
		const uint32_t t_f32v2 = getVectorTypeId({ GcnScalarType::Float32, 2 });

		const uint32_t idxZero = 0;
		const uint32_t idxOne  = 1;

		GcnRegisterValuePair result;
		result.low.type.ctype  = GcnScalarType::Float32;
		result.low.type.ccount = 1;
		result.high.type       = result.low.type;

		uint32_t vec2Id = m_module.opUnpackHalf2x16(t_f32v2, src.id);
		result.low.id   = m_module.opCompositeExtract(t_f32,
													  vec2Id,
													  1, &idxZero);
		result.high.id  = m_module.opCompositeExtract(t_f32,
													  vec2Id,
													  1, &idxOne);
		return result;
	}


	uint32_t GcnCompiler::getScalarTypeId(GcnScalarType type)
	{
		if (type == GcnScalarType::Float64)
			m_module.enableCapability(spv::CapabilityFloat64);

		if (type == GcnScalarType::Sint64 || type == GcnScalarType::Uint64)
			m_module.enableCapability(spv::CapabilityInt64);

		switch (type)
		{
		case GcnScalarType::Uint32:
			return m_module.defIntType(32, 0);
		case GcnScalarType::Uint64:
			return m_module.defIntType(64, 0);
		case GcnScalarType::Sint32:
			return m_module.defIntType(32, 1);
		case GcnScalarType::Sint64:
			return m_module.defIntType(64, 1);
		case GcnScalarType::Float32:
			return m_module.defFloatType(32);
		case GcnScalarType::Float64:
			return m_module.defFloatType(64);
		case GcnScalarType::Bool:
			return m_module.defBoolType();
		}

		LOG_ASSERT(false, "GcnCompiler: Invalid scalar type");
	}

	uint32_t GcnCompiler::getVectorTypeId(const GcnVectorType& type)
	{
		uint32_t typeId = this->getScalarTypeId(type.ctype);

		if (type.ccount > 1)
			typeId = m_module.defVectorType(typeId, type.ccount);

		return typeId;
	}

	uint32_t GcnCompiler::getArrayTypeId(const GcnArrayType& type)
	{
		GcnVectorType vtype;
		vtype.ctype  = type.ctype;
		vtype.ccount = type.ccount;

		uint32_t typeId = this->getVectorTypeId(vtype);

		if (type.alength != 0)
		{
			typeId = m_module.defArrayType(typeId,
										   m_module.constu32(type.alength));
		}

		return typeId;
	}

	uint32_t GcnCompiler::getPointerTypeId(const GcnRegisterInfo& type)
	{
		return m_module.defPointerType(
			this->getArrayTypeId(type.type),
			type.sclass);
	}

	uint32_t GcnCompiler::getPerVertexBlockId()
	{
		uint32_t t_f32    = m_module.defFloatType(32);
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

	bool GcnCompiler::isDoubleType(
		GcnScalarType type) const
	{
		return type == GcnScalarType::Sint64 ||
			   type == GcnScalarType::Uint64 ||
			   type == GcnScalarType::Float64;
	}

	bool GcnCompiler::isFloatType(GcnScalarType type) const
	{
		return type == GcnScalarType::Float16 ||
			   type == GcnScalarType::Float32 ||
			   type == GcnScalarType::Float64;
	}

	GcnScalarType GcnCompiler::getHalfType(GcnScalarType type) const
	{
		GcnScalarType result;
		switch (type)
		{
			case GcnScalarType::Uint64:
				result = GcnScalarType::Uint32;
				break;
			case GcnScalarType::Sint64:
				result = GcnScalarType::Sint32;
				break;
		}
		return result;
	}

	GcnScalarType GcnCompiler::getDestinationType(GcnScalarType type) const
	{
		GcnScalarType dstType = type;

		if (type == GcnScalarType::Uint64 || 
			type == GcnScalarType::Sint64)
		{
			dstType = getHalfType(type);
		}

		return dstType;
	}

	uint32_t GcnCompiler::getUserSgprCount() const
	{
		uint32_t count = 0;
		auto     type  = m_programInfo.type();
		// clang-format off
		switch (type)
		{
		case GcnProgramType::VertexShader:	count = m_meta.vs.userSgprCount; break;
		case GcnProgramType::PixelShader:	count = m_meta.ps.userSgprCount; break;
		case GcnProgramType::ComputeShader:	count = m_meta.cs.userSgprCount; break;
		case GcnProgramType::GeometryShader:count = m_meta.gs.userSgprCount; break;
		case GcnProgramType::HullShader:	count = m_meta.hs.userSgprCount; break;
		case GcnProgramType::DomainShader:	count = m_meta.ds.userSgprCount; break;
		}
		// clang-format on
		return count;
	}

	bool GcnCompiler::hasFetchShader() const
	{
		auto& resTable = m_header->getShaderResourceTable();
		auto  iter     = std::find_if(resTable.begin(), resTable.end(), 
			[](const GcnShaderResource& res) 
			{
				return res.usage == kShaderInputUsageSubPtrFetchShader;
			});
		return iter != resTable.end();
	}

	std::pair<const VertexInputSemantic*, uint32_t> 
		GcnCompiler::getSemanticTable() const
	{
		const VertexInputSemantic* semanticTable = nullptr;
		uint32_t                   semanticCount = 0;

		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:
			{
				semanticTable = m_meta.vs.inputSemanticTable.data();
				semanticCount = m_meta.vs.inputSemanticCount;
			}
			break;
			default:
				LOG_ASSERT(false, "program type not supported, please support it.");
				break;
		}

		return std::make_pair(semanticTable, semanticCount);
	}

	const std::array<GcnTextureMeta, 128>&
		GcnCompiler::getTextureMetaTable()
	{
		switch (m_programInfo.type())
		{
			case GcnProgramType::PixelShader:	return m_meta.ps.textureInfos;
			case GcnProgramType::ComputeShader:	return m_meta.cs.textureInfos;
		}

		LOG_ASSERT(false, "program type not supported, please support it.");
	}

	GcnBufferInfo GcnCompiler::getBufferType(
		const GcnInstOperand& reg)
	{
		uint32_t regIdx = reg.code << 2;

		GcnBufferMeta* meta = nullptr;
		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:	meta = &m_meta.vs.bufferInfos[regIdx]; break;
			case GcnProgramType::PixelShader:	meta = &m_meta.ps.bufferInfos[regIdx]; break;
			case GcnProgramType::ComputeShader: meta = &m_meta.cs.bufferInfos[regIdx]; break;
			case GcnProgramType::GeometryShader:meta = &m_meta.gs.bufferInfos[regIdx]; break;
			case GcnProgramType::HullShader:	meta = &m_meta.hs.bufferInfos[regIdx]; break;
			case GcnProgramType::DomainShader:	meta = &m_meta.ds.bufferInfos[regIdx]; break;
		}

		auto& buffer = m_buffers[regIdx];

		GcnBufferInfo result = {};
		result.varId         = buffer.varId;
		result.isSsbo        = buffer.asSsbo;
		result.buffer        = *meta;
		result.image         = GcnImageInfo();

		return result;
	}

	GcnImageInfo GcnCompiler::getImageType(
		Gnm::TextureType textureType,
		bool             isStorage,
		bool             isDepth) const
	{
		uint32_t     depth    = isDepth ? 1u : 0u;
		uint32_t     sampled  = isStorage ? 2u : 1u;
		GcnImageInfo typeInfo = [textureType, depth, sampled]() -> GcnImageInfo
		{
			switch (textureType)
			{
				case Gnm::kTextureType1d:
					return { spv::Dim1D, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_1D };
				case Gnm::kTextureType2d:
					return { spv::Dim2D, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_2D };
				case Gnm::kTextureType3d:
					return { spv::Dim3D, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_3D };
				case Gnm::kTextureTypeCubemap:
					return { spv::DimCube, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_CUBE };
				case Gnm::kTextureType1dArray:
					return { spv::Dim1D, depth, 1, 0, sampled, VK_IMAGE_VIEW_TYPE_1D_ARRAY };
				case Gnm::kTextureType2dArray:
					return { spv::Dim2D, depth, 1, 0, sampled, VK_IMAGE_VIEW_TYPE_2D_ARRAY };
				case Gnm::kTextureType2dMsaa:
					return { spv::Dim2D, depth, 0, 1, sampled, VK_IMAGE_VIEW_TYPE_2D };
				case Gnm::kTextureType2dArrayMsaa:
					return { spv::Dim2D, depth, 1, 1, sampled, VK_IMAGE_VIEW_TYPE_2D_ARRAY };
				default:
					Logger::exception(util::str::formatex("GcnCompiler: Unsupported resource type: ", textureType));
			}
		}();

		return typeInfo;
	}

	GcnVectorType GcnCompiler::getInputRegType(uint32_t regIdx) const
	{
		GcnVectorType result;
		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:
			{
				const auto table = getSemanticTable();
				LOG_ASSERT(regIdx < table.second, "reg index exceed semantic table count.");
				auto&      sema  = table.first[regIdx];
				result.ctype     = GcnScalarType::Float32;
				// The count value is fixed when parsing V# in CommandBufferDraw
				result.ccount    = sema.m_sizeInElements;
			}
				break;
			default:
			{
				result.ctype  = GcnScalarType::Float32;
				result.ccount = 4;
			}
				break;
		}
		return result;
	}

	GcnVectorType GcnCompiler::getOutputRegType(uint32_t paramIdx) const
	{
		GcnVectorType result;
		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:
			{
				result.ctype  = GcnScalarType::Float32;
				result.ccount = m_analysis->exportInfo.params[paramIdx].popCount();
			}
				break;
			case GcnProgramType::PixelShader:
			case GcnProgramType::HullShader:
			default:
			{
				result.ctype  = GcnScalarType::Float32;
				result.ccount = 4;
			}
		}
		return result;
	}

	uint32_t GcnCompiler::getTexLayerDim(const GcnImageInfo& imageType) const
	{
		switch (imageType.dim)
		{
			case spv::DimBuffer:	return 1;
			case spv::Dim1D:		return 1;
			case spv::Dim2D:		return 2;
			case spv::Dim3D:		return 3;
			case spv::DimCube:		return 3;
			default: Logger::exception("DxbcCompiler: getTexLayerDim: Unsupported image dimension");
		}
	}

	uint32_t GcnCompiler::getTexCoordDim(const GcnImageInfo& imageType) const
	{
		return getTexLayerDim(imageType) + imageType.array;
	}



}  // namespace sce::gcn