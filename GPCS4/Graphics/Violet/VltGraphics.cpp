#include "VltGraphics.h"

#include "VltDevice.h"
#include "VltPipeManager.h"
#include "VltShader.h"

namespace sce::vlt
{

	VltGraphicsPipeline::VltGraphicsPipeline(
		VltPipelineManager*        pipeMgr,
		VltGraphicsPipelineShaders shaders) :
		m_device(pipeMgr->m_device),
		m_pipeMgr(pipeMgr),
		m_shaders(std::move(shaders))
	{
		if (m_shaders.vs != nullptr)
			m_shaders.vs->defineResourceSlots(m_slotMapping);
		if (m_shaders.tcs != nullptr)
			m_shaders.tcs->defineResourceSlots(m_slotMapping);
		if (m_shaders.tes != nullptr)
			m_shaders.tes->defineResourceSlots(m_slotMapping);
		if (m_shaders.gs != nullptr)
			m_shaders.gs->defineResourceSlots(m_slotMapping);
		if (m_shaders.fs != nullptr)
			m_shaders.fs->defineResourceSlots(m_slotMapping);

		m_vsIn  = m_shaders.vs != nullptr ? m_shaders.vs->interfaceSlots().inputSlots : 0;
		m_fsOut = m_shaders.fs != nullptr ? m_shaders.fs->interfaceSlots().outputSlots : 0;

		m_layout = new VltPipelineLayout(m_device,
										 m_slotMapping, VK_PIPELINE_BIND_POINT_GRAPHICS);
	}

	VltGraphicsPipeline::~VltGraphicsPipeline()
	{
		for (const auto& instance : m_pipelines)
			this->destroyPipeline(instance.pipeline());
	}

	Rc<VltShader> VltGraphicsPipeline::getShader(
		VkShaderStageFlagBits stage) const
	{
		switch (stage)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			return m_shaders.vs;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return m_shaders.gs;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return m_shaders.tcs;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return m_shaders.tes;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return m_shaders.fs;
		default:
			return nullptr;
		}
	}

	VkPipeline VltGraphicsPipeline::getPipelineHandle(
		const VltGraphicsPipelineStateInfo& state)
	{
		VltGraphicsPipelineInstance* instance = nullptr;

		{
			std::lock_guard<util::sync::Spinlock> lock(m_mutex);

			instance = this->findInstance(state);

			if (instance)
				return instance->pipeline();

			instance = this->createInstance(state);
		}

		if (!instance)
			return VK_NULL_HANDLE;

		return instance->pipeline();
	}

	void VltGraphicsPipeline::compilePipeline(
		const VltGraphicsPipelineStateInfo& state)
	{
		std::lock_guard<util::sync::Spinlock> lock(m_mutex);

		if (!this->findInstance(state))
			this->createInstance(state);
	}

	VltGraphicsPipelineInstance* VltGraphicsPipeline::createInstance(
		const VltGraphicsPipelineStateInfo& state)
	{
		// If the pipeline state vector is invalid, don't try
		// to create a new pipeline, it won't work anyway.
		if (!this->validatePipelineState(state))
			return nullptr;

		VkPipeline newPipelineHandle = this->createPipeline(state);

		m_pipeMgr->m_numGraphicsPipelines += 1;
		return &m_pipelines.emplace_back(state, newPipelineHandle);
	}

	VltGraphicsPipelineInstance* VltGraphicsPipeline::findInstance(
		const VltGraphicsPipelineStateInfo& state)
	{
		for (auto& instance : m_pipelines)
		{
			if (instance.isCompatible(state))
				return &instance;
		}

		return nullptr;
	}

	VkPipeline VltGraphicsPipeline::createPipeline(
		const VltGraphicsPipelineStateInfo& state) const
	{
		if (Logger::logLevel() <= LogLevel::Debug)
		{
			Logger::debug("Compiling graphics pipeline...");
			this->logPipelineState(LogLevel::Debug, state);
		}

		// Set up dynamic states as needed
		std::array<VkDynamicState, 6> dynamicStates;
		uint32_t                      dynamicStateCount = 0;

		dynamicStates[dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
		dynamicStates[dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;

		if (state.useDynamicDepthBias())
			dynamicStates[dynamicStateCount++] = VK_DYNAMIC_STATE_DEPTH_BIAS;

		if (state.useDynamicDepthBounds())
			dynamicStates[dynamicStateCount++] = VK_DYNAMIC_STATE_DEPTH_BOUNDS;

		if (state.useDynamicBlendConstants())
			dynamicStates[dynamicStateCount++] = VK_DYNAMIC_STATE_BLEND_CONSTANTS;

		if (state.useDynamicStencilRef())
			dynamicStates[dynamicStateCount++] = VK_DYNAMIC_STATE_STENCIL_REFERENCE;

		// Figure out the actual sample count to use
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;

		if (state.ms.sampleCount())
			sampleCount = VkSampleCountFlagBits(state.ms.sampleCount());
		else if (state.rs.sampleCount())
			sampleCount = VkSampleCountFlagBits(state.rs.sampleCount());

		auto vsm  = createShaderModule(m_shaders.vs, state);
		auto tcsm = createShaderModule(m_shaders.tcs, state);
		auto tesm = createShaderModule(m_shaders.tes, state);
		auto gsm  = createShaderModule(m_shaders.gs, state);
		auto fsm  = createShaderModule(m_shaders.fs, state);

		std::vector<VkPipelineShaderStageCreateInfo> stages;
		if (vsm)
			stages.push_back(vsm.stageInfo(nullptr));
		if (tcsm)
			stages.push_back(tcsm.stageInfo(nullptr));
		if (tesm)
			stages.push_back(tesm.stageInfo(nullptr));
		if (gsm)
			stages.push_back(gsm.stageInfo(nullptr));
		if (fsm)
			stages.push_back(fsm.stageInfo(nullptr));

		// Fix up color write masks using the component mappings
		std::array<VkPipelineColorBlendAttachmentState, MaxNumRenderTargets> omBlendAttachments;

		const VkColorComponentFlags fullMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			omBlendAttachments[i] = state.cbBlend[i].state();

			if (omBlendAttachments[i].colorWriteMask != fullMask)
			{
				omBlendAttachments[i].colorWriteMask = vutil::remapComponentMask(
					state.cbBlend[i].colorWriteMask(), state.cbSwizzle[i].mapping());
			}

			if ((m_fsOut & (1 << i)) == 0)
				omBlendAttachments[i].colorWriteMask = 0;
		}

		// Generate per-instance attribute divisors
		std::array<VkVertexInputBindingDivisorDescriptionEXT, MaxNumVertexBindings> viDivisorDesc;
		uint32_t                                                                    viDivisorCount = 0;

		for (uint32_t i = 0; i < state.il.bindingCount(); i++)
		{
			if (state.ilBindings[i].inputRate() == VK_VERTEX_INPUT_RATE_INSTANCE && state.ilBindings[i].divisor() != 1)
			{
				const uint32_t id = viDivisorCount++;

				viDivisorDesc[id].binding = i; /* see below */
				viDivisorDesc[id].divisor = state.ilBindings[i].divisor();
			}
		}

		int32_t rasterizedStream = m_shaders.gs != nullptr
									   ? m_shaders.gs->shaderOptions().rasterizedStream
									   : 0;

		// Compact vertex bindings so that we can more easily update vertex buffers
		std::array<VkVertexInputAttributeDescription, MaxNumVertexAttributes> viAttribs;
		std::array<VkVertexInputBindingDescription, MaxNumVertexBindings>     viBindings;
		std::array<uint32_t, MaxNumVertexBindings>                            viBindingMap = {};

		for (uint32_t i = 0; i < state.il.bindingCount(); i++)
		{
			viBindings[i]                               = state.ilBindings[i].description();
			viBindings[i].binding                       = i;
			viBindingMap[state.ilBindings[i].binding()] = i;
		}

		for (uint32_t i = 0; i < state.il.attributeCount(); i++)
		{
			viAttribs[i]         = state.ilAttributes[i].description();
			viAttribs[i].binding = viBindingMap[state.ilAttributes[i].binding()];
		}

		VkPipelineVertexInputDivisorStateCreateInfoEXT viDivisorInfo;
		viDivisorInfo.sType                     = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
		viDivisorInfo.pNext                     = nullptr;
		viDivisorInfo.vertexBindingDivisorCount = viDivisorCount;
		viDivisorInfo.pVertexBindingDivisors    = viDivisorDesc.data();

		VkPipelineVertexInputStateCreateInfo viInfo;
		viInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		viInfo.pNext                           = &viDivisorInfo;
		viInfo.flags                           = 0;
		viInfo.vertexBindingDescriptionCount   = state.il.bindingCount();
		viInfo.pVertexBindingDescriptions      = viBindings.data();
		viInfo.vertexAttributeDescriptionCount = state.il.attributeCount();
		viInfo.pVertexAttributeDescriptions    = viAttribs.data();

		if (viDivisorCount == 0)
			viInfo.pNext = viDivisorInfo.pNext;

		// TODO remove this once the extension is widely supported
		if (!m_pipeMgr->m_device->features().extVertexAttributeDivisor.vertexAttributeInstanceRateDivisor)
			viInfo.pNext = viDivisorInfo.pNext;

		VkPipelineInputAssemblyStateCreateInfo iaInfo;
		iaInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		iaInfo.pNext                  = nullptr;
		iaInfo.flags                  = 0;
		iaInfo.topology               = state.ia.primitiveTopology();
		iaInfo.primitiveRestartEnable = state.ia.primitiveRestart();

		VkPipelineTessellationStateCreateInfo tsInfo;
		tsInfo.sType              = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		tsInfo.pNext              = nullptr;
		tsInfo.flags              = 0;
		tsInfo.patchControlPoints = state.ia.patchVertexCount();

		VkPipelineViewportStateCreateInfo vpInfo;
		vpInfo.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vpInfo.pNext         = nullptr;
		vpInfo.flags         = 0;
		vpInfo.viewportCount = state.rs.viewportCount();
		vpInfo.pViewports    = nullptr;
		vpInfo.scissorCount  = state.rs.viewportCount();
		vpInfo.pScissors     = nullptr;

		VkPipelineRasterizationStateStreamCreateInfoEXT xfbStreamInfo;
		xfbStreamInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT;
		xfbStreamInfo.pNext               = nullptr;
		xfbStreamInfo.flags               = 0;
		xfbStreamInfo.rasterizationStream = uint32_t(rasterizedStream);

		VkPipelineRasterizationDepthClipStateCreateInfoEXT rsDepthClipInfo;
		rsDepthClipInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT;
		rsDepthClipInfo.pNext           = nullptr;
		rsDepthClipInfo.flags           = 0;
		rsDepthClipInfo.depthClipEnable = state.rs.depthClipEnable();

		VkPipelineRasterizationStateCreateInfo rsInfo;
		rsInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rsInfo.pNext                   = &rsDepthClipInfo;
		rsInfo.flags                   = 0;
		rsInfo.depthClampEnable        = VK_TRUE;
		rsInfo.rasterizerDiscardEnable = rasterizedStream < 0;
		rsInfo.polygonMode             = state.rs.polygonMode();
		rsInfo.cullMode                = state.rs.cullMode();
		rsInfo.frontFace               = state.rs.frontFace();
		rsInfo.depthBiasEnable         = state.rs.depthBiasEnable();
		rsInfo.depthBiasConstantFactor = 0.0f;
		rsInfo.depthBiasClamp          = 0.0f;
		rsInfo.depthBiasSlopeFactor    = 0.0f;
		rsInfo.lineWidth               = 1.0f;

		if (rasterizedStream > 0)
			rsDepthClipInfo.pNext = &xfbStreamInfo;

		if (!m_pipeMgr->m_device->features().extDepthClipEnable.depthClipEnable)
		{
			rsInfo.pNext            = rsDepthClipInfo.pNext;
			rsInfo.depthClampEnable = !state.rs.depthClipEnable();
		}

		uint32_t sampleMask = state.ms.sampleMask();

		VkPipelineMultisampleStateCreateInfo msInfo;
		msInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		msInfo.pNext                 = nullptr;
		msInfo.flags                 = 0;
		msInfo.rasterizationSamples  = sampleCount;
		msInfo.sampleShadingEnable   = m_common.msSampleShadingEnable;
		msInfo.minSampleShading      = m_common.msSampleShadingFactor;
		msInfo.pSampleMask           = &sampleMask;
		msInfo.alphaToCoverageEnable = state.ms.enableAlphaToCoverage();
		msInfo.alphaToOneEnable      = VK_FALSE;

		VkPipelineDepthStencilStateCreateInfo dsInfo;
		dsInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		dsInfo.pNext                 = nullptr;
		dsInfo.flags                 = 0;
		dsInfo.depthTestEnable       = state.ds.enableDepthTest();
		dsInfo.depthWriteEnable      = state.ds.enableDepthWrite();  // &&!vutil::isDepthReadOnlyLayout(passFormat.depth.layout);
		dsInfo.depthCompareOp        = state.ds.depthCompareOp();
		dsInfo.depthBoundsTestEnable = state.ds.enableDepthBoundsTest();
		dsInfo.stencilTestEnable     = state.ds.enableStencilTest();
		dsInfo.front                 = state.dsFront.state();
		dsInfo.back                  = state.dsBack.state();
		dsInfo.minDepthBounds        = 0.0f;
		dsInfo.maxDepthBounds        = 1.0f;

		VkPipelineColorBlendStateCreateInfo cbInfo;
		cbInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		cbInfo.pNext           = nullptr;
		cbInfo.flags           = 0;
		cbInfo.logicOpEnable   = state.cb.enableLogicOp();
		cbInfo.logicOp         = state.cb.logicOp();
		cbInfo.attachmentCount = VltLimits::MaxNumRenderTargets;
		cbInfo.pAttachments    = omBlendAttachments.data();

		for (uint32_t i = 0; i < 4; i++)
			cbInfo.blendConstants[i] = 0.0f;

		VkPipelineDynamicStateCreateInfo dyInfo;
		dyInfo.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dyInfo.pNext             = nullptr;
		dyInfo.flags             = 0;
		dyInfo.dynamicStateCount = dynamicStateCount;
		dyInfo.pDynamicStates    = dynamicStates.data();

		VkGraphicsPipelineCreateInfo info;
		info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		info.pNext               = nullptr;
		info.flags               = 0;
		info.stageCount          = stages.size();
		info.pStages             = stages.data();
		info.pVertexInputState   = &viInfo;
		info.pInputAssemblyState = &iaInfo;
		info.pTessellationState  = &tsInfo;
		info.pViewportState      = &vpInfo;
		info.pRasterizationState = &rsInfo;
		info.pMultisampleState   = &msInfo;
		info.pDepthStencilState  = &dsInfo;
		info.pColorBlendState    = &cbInfo;
		info.pDynamicState       = &dyInfo;
		info.layout              = m_layout->pipelineLayout();
		info.renderPass          = VK_NULL_HANDLE;
		info.subpass             = 0;
		info.basePipelineHandle  = VK_NULL_HANDLE;
		info.basePipelineIndex   = -1;

		if (tsInfo.patchControlPoints == 0)
			info.pTessellationState = nullptr;

		VkPipeline pipeline = VK_NULL_HANDLE;
		if (vkCreateGraphicsPipelines(m_device->handle(),
									  VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS)
		{
			Logger::err("DxvkGraphicsPipeline: Failed to compile pipeline");
			this->logPipelineState(LogLevel::Error, state);
			return VK_NULL_HANDLE;
		}

		return pipeline;
	}

	void VltGraphicsPipeline::destroyPipeline(VkPipeline pipeline) const
	{
		vkDestroyPipeline(m_device->handle(), pipeline, nullptr);
	}

	VltShaderModule VltGraphicsPipeline::createShaderModule(
		const Rc<VltShader>&                shader,
		const VltGraphicsPipelineStateInfo& state) const
	{
		if (shader == nullptr)
			return VltShaderModule();

		VltShaderModuleCreateInfo info;

		// Fix up fragment shader outputs for dual-source blending
		if (shader->stage() == VK_SHADER_STAGE_FRAGMENT_BIT)
		{
			info.fsDualSrcBlend = state.cbBlend[0].blendEnable() && (vutil::isDualSourceBlendFactor(state.cbBlend[0].srcColorBlendFactor()) ||
																	 vutil::isDualSourceBlendFactor(state.cbBlend[0].dstColorBlendFactor()) ||
																	 vutil::isDualSourceBlendFactor(state.cbBlend[0].srcAlphaBlendFactor()) ||
																	 vutil::isDualSourceBlendFactor(state.cbBlend[0].dstAlphaBlendFactor()));
		}

		// Deal with undefined shader inputs
		uint32_t consumedInputs = shader->interfaceSlots().inputSlots;
		uint32_t providedInputs = 0;

		if (shader->stage() == VK_SHADER_STAGE_VERTEX_BIT)
		{
			for (uint32_t i = 0; i < state.il.attributeCount(); i++)
				providedInputs |= 1u << state.ilAttributes[i].location();
		}
		else if (shader->stage() != VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
		{
			auto prevStage = getPrevStageShader(shader->stage());
			providedInputs = prevStage->interfaceSlots().outputSlots;
		}
		else
		{
			// Technically not correct, but this
			// would need a lot of extra care
			providedInputs = consumedInputs;
		}

		info.undefinedInputs = (providedInputs & consumedInputs) ^ consumedInputs;
		return shader->createShaderModule(m_device, m_slotMapping, info);
	}

	Rc<VltShader> VltGraphicsPipeline::getPrevStageShader(VkShaderStageFlagBits stage) const
	{
		if (stage == VK_SHADER_STAGE_VERTEX_BIT)
			return nullptr;

		if (stage == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
			return m_shaders.tcs;

		Rc<VltShader> result = m_shaders.vs;

		if (stage == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT)
			return result;

		if (m_shaders.tes != nullptr)
			result = m_shaders.tes;

		if (stage == VK_SHADER_STAGE_GEOMETRY_BIT)
			return result;

		if (m_shaders.gs != nullptr)
			result = m_shaders.gs;

		return result;
	}

	bool VltGraphicsPipeline::validatePipelineState(
		const VltGraphicsPipelineStateInfo& state) const
	{
		// Tessellation shaders and patches must be used together
		bool hasPatches = state.ia.primitiveTopology() == VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

		bool hasTcs = m_shaders.tcs != nullptr;
		bool hasTes = m_shaders.tes != nullptr;

		if (hasPatches != hasTcs || hasPatches != hasTes)
			return false;

		// Filter out undefined primitive topologies
		if (state.ia.primitiveTopology() == VK_PRIMITIVE_TOPOLOGY_MAX_ENUM)
			return false;

		// Prevent unintended out-of-bounds access to the IL arrays
		if (state.il.attributeCount() > VltLimits::MaxNumVertexAttributes ||
			state.il.bindingCount() > VltLimits::MaxNumVertexBindings)
			return false;

		// No errors
		return true;
	}

	void VltGraphicsPipeline::logPipelineState(
		LogLevel                            level,
		const VltGraphicsPipelineStateInfo& state) const
	{
		if (m_shaders.vs != nullptr)
			Logger::log(level, util::str::formatex("  vs  : ", m_shaders.vs->debugName()));
		if (m_shaders.tcs != nullptr)
			Logger::log(level, util::str::formatex("  tcs : ", m_shaders.tcs->debugName()));
		if (m_shaders.tes != nullptr)
			Logger::log(level, util::str::formatex("  tes : ", m_shaders.tes->debugName()));
		if (m_shaders.gs != nullptr)
			Logger::log(level, util::str::formatex("  gs  : ", m_shaders.gs->debugName()));
		if (m_shaders.fs != nullptr)
			Logger::log(level, util::str::formatex("  fs  : ", m_shaders.fs->debugName()));

		for (uint32_t i = 0; i < state.il.attributeCount(); i++)
		{
			const auto& attr = state.ilAttributes[i];
			Logger::log(level, util::str::formatex("  attr ", i, " : location ", attr.location(), ", binding ", attr.binding(), ", format ", attr.format(), ", offset ", attr.offset()));
		}
		for (uint32_t i = 0; i < state.il.bindingCount(); i++)
		{
			const auto& bind = state.ilBindings[i];
			Logger::log(level, util::str::formatex("  binding ", i, " : binding ", bind.binding(), ", stride ", bind.stride(), ", rate ", bind.inputRate(), ", divisor ", bind.divisor()));
		}

		// TODO log more pipeline state
	}

}  // namespace sce::vlt