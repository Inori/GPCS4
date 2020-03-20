#include "VltGraphicsPipeline.h"
#include "VltDevice.h"
#include "VltPipelineManager.h"
#include "VltPipelineLayout.h"

#include <mutex>

LOG_CHANNEL(Graphic.Violet.VltGraphicsPipeline);

namespace vlt
{;

VltGraphicsPipelineInstance::VltGraphicsPipelineInstance(VkPipeline pipeline,
	const VltGraphicsPipelineStateInfo& state,
	const VltRenderPass& rp) :
	m_pipeline(pipeline),
	m_state(state),
	m_renderPass(&rp)
{

}

VltGraphicsPipelineInstance::~VltGraphicsPipelineInstance()
{
}

VkPipeline VltGraphicsPipelineInstance::pipeline()
{
	return m_pipeline;
}

bool VltGraphicsPipelineInstance::isCompatible(const VltGraphicsPipelineStateInfo& state, const VltRenderPass& rp) const
{
	return (m_state == state) && (m_renderPass == &rp);
}

///


VltGraphicsPipeline::VltGraphicsPipeline(VltPipelineManager* pipeMgr, const VltGraphicsPipelineShaders& shaders):
	m_pipelineManager(pipeMgr),
	m_shaders(shaders)
{
	shaders.vs->defineResourceSlots(m_resSlotMap);
	shaders.fs->defineResourceSlots(m_resSlotMap);

	m_layout = new VltPipelineLayout(pipeMgr->m_device, m_resSlotMap, VK_PIPELINE_BIND_POINT_GRAPHICS);
}

VltGraphicsPipeline::~VltGraphicsPipeline()
{
}


VkPipeline VltGraphicsPipeline::getPipelineHandle(const VltGraphicsPipelineStateInfo& state, const VltRenderPass& rp)
{
	VkPipeline pipeline = VK_NULL_HANDLE;
	
	do 
	{
		std::lock_guard<Spinlock> lock(m_mutex);

		auto instance = findInstance(state, rp);
		if (instance)
		{
			pipeline = instance->pipeline();
			break;
		}

		instance = createInstance(state, rp);
		if (!instance)
		{
			break;
		}
		pipeline = instance->pipeline();

	} while (false);
	return pipeline;
}

VltPipelineLayout* VltGraphicsPipeline::getLayout() const
{
	return m_layout;
}

VltGraphicsPipelineInstance* VltGraphicsPipeline::findInstance(const VltGraphicsPipelineStateInfo& state, const VltRenderPass& rp)
{
	VltGraphicsPipelineInstance* instance = nullptr;
	for (auto& pipeInst : m_pipelines)
	{
		if (pipeInst.isCompatible(state, rp))
		{
			instance = &pipeInst;
			break;
		}
	}
	return instance;
}


VltGraphicsPipelineInstance* VltGraphicsPipeline::createInstance(const VltGraphicsPipelineStateInfo& state, const VltRenderPass& rp)
{
	VltGraphicsPipelineInstance* instance = nullptr;
	do 
	{
		auto vsModule = m_shaders.vs->createShaderModule(m_pipelineManager->m_device, m_resSlotMap);
		auto vsStage  = vsModule.stageInfo(nullptr);

		auto fsModule = m_shaders.fs->createShaderModule(m_pipelineManager->m_device, m_resSlotMap);
		auto fsStage  = fsModule.stageInfo(nullptr);

		VkPipelineShaderStageCreateInfo shaderStages[] = { vsStage, fsStage };

		std::vector<VkVertexInputBindingDescription>   vertexBindings;
		std::vector<VkVertexInputAttributeDescription> vertexAttributes;
		auto viState = state.vi.state(vertexBindings, vertexAttributes);

		std::vector<VkPipelineColorBlendAttachmentState> colorAttachments;
		auto cbState = state.cb.state(colorAttachments);

		std::vector<VkDynamicState> dynStateArray;
		auto dyState = state.dy.state(dynStateArray);

		auto vpState = state.dy.viewportState();
		auto iaState = state.ia.state();
		auto rsState = state.rs.state();
		auto msState = state.ms.state(VkSampleCountFlagBits(state.rs.sampleCount()), VK_FALSE, 0.0);
		auto dsState = state.ds.state();

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount                   = 2;
		pipelineInfo.pStages                      = shaderStages;
		pipelineInfo.pVertexInputState            = &viState;
		pipelineInfo.pInputAssemblyState          = &iaState;
		pipelineInfo.pViewportState               = &vpState;
		pipelineInfo.pRasterizationState          = &rsState;
		pipelineInfo.pMultisampleState            = &msState;
		pipelineInfo.pDepthStencilState           = &dsState;
		pipelineInfo.pColorBlendState             = &cbState;
		pipelineInfo.pDynamicState                = &dyState;
		pipelineInfo.layout                       = m_layout->pipelineLayout();
		pipelineInfo.renderPass                   = rp.getDefaultHandle();
		pipelineInfo.subpass                      = 0;
		pipelineInfo.basePipelineHandle           = VK_NULL_HANDLE;

		VkDevice   device   = *(m_pipelineManager->m_device);
		VkPipeline pipeline = VK_NULL_HANDLE;
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			LOG_ERR("failed to create graphics pipeline!");
			break;
		}

		m_pipelines.emplace_back(pipeline, state, rp);
		instance = &m_pipelines.back();
		
	} while (false);
	return instance;
}

}  // namespace vlt