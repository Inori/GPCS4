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
		const GcnProgramInfo&  programInfo,
		const GcnHeader&       header,
		const GcnShaderMeta&   meta,
		const GcnAnalysisInfo& analysis) :
		m_programInfo(programInfo),
		m_header(&header),
		m_meta(meta),
		m_analysis(&analysis),
		m_module(spvVersion(1, 3)),
		m_state({
			{ this, 1 },  // assume we have only one thread, and the thread id is 0
			{ this, 0 },
		})
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
		compileInstruction(ins);

		updateProgramCounter(ins);
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

		// clang-format off
		switch (m_programInfo.type()) 
		{
		  case GcnProgramType::VertexShader:   this->emitVsFinalize(); break;
		  case GcnProgramType::HullShader:     this->emitHsFinalize(); break;
		  case GcnProgramType::DomainShader:   this->emitDsFinalize(); break;
		  case GcnProgramType::GeometryShader: this->emitGsFinalize(); break;
		  case GcnProgramType::PixelShader:    this->emitPsFinalize(); break;
		  case GcnProgramType::ComputeShader:  this->emitCsFinalize(); break;
		}
		// clang-format on

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

		// Declare shader resource and input interfaces
		this->emitDclInputSlots();

		// Initialize the shader module with capabilities
		// etc. Each shader type has its own peculiarities.

		// clang-format off
		switch (m_programInfo.type()) 
		{
		  case GcnProgramType::VertexShader:   emitVsInit(); break;
		  case GcnProgramType::HullShader:     emitHsInit(); break;
		  case GcnProgramType::DomainShader:   emitDsInit(); break;
		  case GcnProgramType::GeometryShader: emitGsInit(); break;
		  case GcnProgramType::PixelShader:    emitPsInit(); break;
		  case GcnProgramType::ComputeShader:  emitCsInit(); break;
		}
		// clang-format on
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
		if (m_insideFunction)
		{
			m_module.opReturn();
			m_module.functionEnd();
		}

		m_insideFunction = false;
	}

	void GcnCompiler::emitFunctionLabel()
	{
		m_module.opLabel(m_module.allocateId());
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
		m_module.enableCapability(spv::CapabilityClipDistance);
		m_module.enableCapability(spv::CapabilityCullDistance);
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
	}

	void GcnCompiler::emitCsInit()
	{
	}

	void GcnCompiler::emitVsFinalize()
	{
		this->emitMainFunctionBegin();
		this->emitInputSetup();

		// call fetch shader
		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_vs.fetchFuncId, 0, nullptr);

		// call vs_main
		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_vs.functionId, 0, nullptr);
		this->emitOutputSetup();
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
	}

	void GcnCompiler::emitCsFinalize()
	{
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
				getScalarTypeId(GcnScalarType::Float32));
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
		
		const auto& textureInfoTable = getTextureInfoTable();
		const auto& textureInfo      = textureInfoTable[registerId];

		// TODO
		// Support storage image
		const bool isStorage = res.usage != kShaderInputUsageImmResource;

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
	
	void GcnCompiler::emitDclInput(
		const VertexInputSemantic& sema)
	{
		GcnRegisterInfo info;
		info.type.ctype = GcnScalarType::Float32;
		// the count value is fixed when parsing V# in CommandBufferDraw
		info.type.ccount  = sema.m_sizeInElements;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassInput;

		const uint32_t varId = emitNewVariable(info);

		m_module.decorateLocation(varId, sema.m_semantic);
		m_module.setDebugName(varId, util::str::formatex("vertex", sema.m_semantic).c_str());

		// Record the input so that we can
		// use it in fetch shader.
		GcnRegisterPointer input;
		input.type.ctype          = info.type.ctype;
		input.type.ccount         = info.type.ccount;
		input.id                  = varId;
		m_inputs[sema.m_semantic] = input;

		m_entryPointInterfaces.push_back(varId);

		// Declare the input slot as defined
		m_interfaceSlots.inputSlots |= 1u << sema.m_semantic;
	}

	void GcnCompiler::emitDclVertexInput()
	{
		auto table = getSemanticTable();
		
		for (uint32_t i = 0; i != table.second; ++i)
		{
			auto& sema = table.first[i];
			this->emitDclInput(sema);
		}
	}

	void GcnCompiler::emitInputSetup()
	{
		// The 16 user data registers is passed though push constants,
		// here we copy them into predefined user data array.

		// TODO
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
				reg, sema.m_sizeInElements, value);
		}
	}

	void GcnCompiler::emitOutputSetup()
	{
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

	template <bool IsVgpr>
	GcnRegisterValue GcnCompiler::emitGprLoad(
		const GcnInstOperand& reg)
	{
		GcnRegisterPointer* gpr = nullptr;
		if constexpr (IsVgpr)
		{
			uint32_t vgprIndex = reg.code - GcnCodeVGPR0;
			gpr                = &m_vgprs[vgprIndex];
		}
		else
		{
			gpr = &m_sgprs[reg.code];
		}

		// sgprs/vgprs are not allowed to load before created.
		// if this occurs, it is most likely a system value vgpr
		// which should be initialized in emitInputSetup,
		// please add it there.
		LOG_ASSERT(gpr->id != 0, "sgpr/vgpr is not initialized before load.");

		return this->emitValueLoad(*gpr);
	}

	template <bool IsVgpr>
	void GcnCompiler::emitGprStore(
		const GcnInstOperand&   reg,
		const GcnRegisterValue& value)
	{
		GcnRegisterPointer* gpr = nullptr;
		std::string         debugName;
		if constexpr (IsVgpr)
		{
			uint32_t vgprIndex = reg.code - GcnCodeVGPR0;
			gpr                = &m_vgprs[vgprIndex];
			debugName          = util::str::formatex("v", vgprIndex);
		}
		else
		{
			gpr       = &m_sgprs[reg.code];
			debugName = util::str::formatex("s", reg.code);
		}

		// If the vgpr has not been used, we create one.
		if (gpr->id == 0)
		{
			GcnRegisterInfo info;
			info.type.ctype   = GcnScalarType::Float32;
			info.type.ccount  = 1;
			info.type.alength = 0;
			info.sclass       = spv::StorageClassPrivate;

			uint32_t id = emitNewVariable(info);
			m_module.setDebugName(id, debugName.c_str());

			gpr->type.ctype  = info.type.ctype;
			gpr->type.ccount = info.type.ccount;
			gpr->id          = id;
		}

		return this->emitValueStore(*gpr, value, GcnRegMask(true, false, false, false));
	}

	template <bool IsVgpr>
	GcnRegisterValue GcnCompiler::emitGprArrayLoad(
		const GcnInstOperand& start,
		uint32_t              count)
	{
	}

	template <bool IsVgpr>
	void GcnCompiler::emitGprArrayStore(
		const GcnInstOperand&   start,
		uint32_t                count,
		const GcnRegisterValue& value)
	{
		// store values in a vector into vgpr array,
		// e.g. vec3 -> v[4:6]

		LOG_ASSERT(count <= value.type.ccount, "value component count is less than store count.");

		for (uint32_t i = 0; i != count; ++i)
		{
			GcnInstOperand reg = start;
			reg.code += i;

			uint32_t typeId = this->getScalarTypeId(value.type.ctype);
			uint32_t id     = m_module.opCompositeExtract(
					typeId, value.id, 1, &i);

			GcnRegisterValue val;
			val.type.ctype  = value.type.ctype;
			val.type.ccount = 1;
			val.id          = id;
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
		uint32_t              count)
	{
		return this->emitGprArrayLoad<true>(start, count);
	}

	void GcnCompiler::emitVgprArrayStore(
		const GcnInstOperand&   start,
		uint32_t                count,
		const GcnRegisterValue& value)
	{
		this->emitGprArrayStore<true>(start, count, value);
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
		uint32_t              count)
	{
		return this->emitGprArrayLoad<false>(start, count);
	}

	void GcnCompiler::emitSgprArrayStore(
		const GcnInstOperand&   start,
		uint32_t                count,
		const GcnRegisterValue& value)
	{
		this->emitGprArrayStore<false>(start, count, value);
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

		bool loadHighPart = reg.type == GcnScalarType::Uint64 ||
							reg.type == GcnScalarType::Sint64;

		auto field = reg.field;
		switch (field)
		{
			case GcnOperandField::ScalarGPR:
			{
				result.low = this->emitSgprLoad(reg);
				if (loadHighPart)
				{
					GcnInstOperand highReg = reg;
					highReg.code += 1;
					result.high = this->emitSgprLoad(highReg);
				}
			}
				break;
			case GcnOperandField::VccLo:
				break;
			case GcnOperandField::VccHi:
				break;
			case GcnOperandField::M0:
				break;
			case GcnOperandField::ExecLo:
				break;
			case GcnOperandField::ExecHi:
				break;
			case GcnOperandField::ConstZero:
				break;
			case GcnOperandField::SignedConstIntPos:
				break;
			case GcnOperandField::SignedConstIntNeg:
				break;
			case GcnOperandField::ConstFloatPos_0_5:
				break;
			case GcnOperandField::ConstFloatNeg_0_5:
				break;
			case GcnOperandField::ConstFloatPos_1_0:
				break;
			case GcnOperandField::ConstFloatNeg_1_0:
				break;
			case GcnOperandField::ConstFloatPos_2_0:
				break;
			case GcnOperandField::ConstFloatNeg_2_0:
				break;
			case GcnOperandField::ConstFloatPos_4_0:
				break;
			case GcnOperandField::ConstFloatNeg_4_0:
				break;
			case GcnOperandField::VccZ:
				break;
			case GcnOperandField::ExecZ:
				break;
			case GcnOperandField::Scc:
				break;
			case GcnOperandField::LdsDirect:
				break;
			case GcnOperandField::LiteralConst:
				break;
			case GcnOperandField::VectorGPR:
				break;
			case GcnOperandField::Undefined:
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		result = emitRegisterBitcast(result, reg.type);

		return result;
	}

	void GcnCompiler::emitRegisterStore(
		const GcnInstOperand&       reg,
		const GcnRegisterValuePair& value)
	{
		auto field = reg.field;
		switch (field)
		{
			case GcnOperandField::ScalarGPR:
				break;
			case GcnOperandField::VccLo:
				break;
			case GcnOperandField::VccHi:
				break;
			case GcnOperandField::M0:
				break;
			case GcnOperandField::ExecLo:
				break;
			case GcnOperandField::ExecHi:
				break;
			case GcnOperandField::VccZ:
				break;
			case GcnOperandField::ExecZ:
				break;
			case GcnOperandField::Scc:
				break;
			case GcnOperandField::LdsDirect:
				break;
			case GcnOperandField::LiteralConst:
				break;
			case GcnOperandField::VectorGPR:
				break;
			case GcnOperandField::Undefined:
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	GcnRegisterPointer GcnCompiler::emitCompositeAccess(
		GcnRegisterPointer pointer,
		spv::StorageClass  sclass,
		uint32_t           index)
	{
		// Create a pointer into a composite object

		uint32_t ptrTypeId = m_module.defPointerType(
			getVectorTypeId(pointer.type), sclass);

		GcnRegisterPointer result;
		result.type = pointer.type;
		result.id   = m_module.opAccessChain(
			  ptrTypeId, pointer.id, 1, &index);
		return result;
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

	// GcnRegisterValue GcnCompiler::emitSrcOperandModifiers(
	//	GcnRegisterValue value,
	//	GcnRegModifiers  modifiers)
	//{
	//	if (modifiers.test(GcnRegModifier::Abs))
	//		value = emitRegisterAbsolute(value);

	//	if (modifiers.test(GcnRegModifier::Neg))
	//		value = emitRegisterNegate(value);
	//	return value;
	//}

	// GcnRegisterValue GcnCompiler::emitDstOperandModifiers(
	//	GcnRegisterValue value,
	//	GcnOpModifiers   modifiers)
	//{
	//	const uint32_t typeId = getVectorTypeId(value.type);

	//	if (value.type.ctype == GcnScalarType::Float32)
	//	{
	//		// Saturating only makes sense on floats
	//		if (modifiers.saturate)
	//		{
	//			const GcnRegMask       mask = GcnRegMask::firstN(value.type.ccount);
	//			const GcnRegisterValue vec0 = emitBuildConstVecf32(0.0f, 0.0f, 0.0f, 0.0f, mask);
	//			const GcnRegisterValue vec1 = emitBuildConstVecf32(1.0f, 1.0f, 1.0f, 1.0f, mask);

	//			value.id = m_module.opNClamp(typeId, value.id, vec0.id, vec1.id);
	//		}
	//	}

	//	return value;
	//}

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
		GcnCompiler::getSemanticTable()
	{
		const VertexInputSemantic* semanticTable = nullptr;
		uint32_t                   semanticCount = 0;

		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader:
			{
				semanticTable = m_meta.vs.inputSemanticTable;
				semanticCount = m_meta.vs.inputSemanticCount;
			}
			break;
			default:
				LOG_ASSERT(false, "program type not supported, please support it.");
				break;
		}

		return std::make_pair(semanticTable, semanticCount);
	}

	const std::array<GcnTextureInfo, 128>&
		GcnCompiler::getTextureInfoTable()
	{
		switch (m_programInfo.type())
		{
			case GcnProgramType::PixelShader:
			{
				return m_meta.ps.textureInfos;
			}
		}

		LOG_ASSERT(false, "program type not supported, please support it.");
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

}  // namespace sce::gcn