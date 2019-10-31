#include "GveGraphicsPipeline.h"
#include "GveDevice.h"
#include "GvePipelineManager.h"
#include "GvePipelineLayout.h"

#include <mutex>

namespace gve
{;

GvePipelineInstance::GvePipelineInstance(VkPipeline pipeline,
	const GveRenderState& state,
	GveRenderPass* rp) :
	m_pipeline(pipeline),
	m_state(state),
	m_renderPass(rp)
{

}

GvePipelineInstance::~GvePipelineInstance()
{
}

VkPipeline GvePipelineInstance::pipeline()
{
	return m_pipeline;
}

bool GvePipelineInstance::isCompatible(const GveRenderState& state, const GveRenderPass& rp) const
{
	// TODO:
	return true;
}

///


GveGraphicsPipeline::GveGraphicsPipeline(GvePipelineManager* pipeMgr, const GveGraphicsPipelineShaders& shaders):
	m_pipelineManager(pipeMgr),
	m_shaders(shaders)
{
	shaders.vs->fillResourceSlots(m_resSlotMap);
	shaders.fs->fillResourceSlots(m_resSlotMap);

	m_layout = new GvePipelineLayout(pipeMgr->m_device, m_resSlotMap, VK_PIPELINE_BIND_POINT_GRAPHICS);
}

GveGraphicsPipeline::~GveGraphicsPipeline()
{
}


VkPipeline GveGraphicsPipeline::getPipelineHandle(const GveRenderState& state, GveRenderPass& rp)
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

GvePipelineLayout* GveGraphicsPipeline::getLayout() const
{
	return m_layout;
}

GvePipelineInstance* GveGraphicsPipeline::findInstance(const GveRenderState& state, GveRenderPass& rp)
{
	GvePipelineInstance* instance = nullptr;
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

GvePipelineInstance* GveGraphicsPipeline::createInstance(const GveRenderState& state, GveRenderPass& rp)
{
	GvePipelineInstance* instance = nullptr;
	do 
	{
		auto vsModule = m_shaders.vs->createShaderModule(m_pipelineManager->m_device, m_resSlotMap);
		auto vsStage = vsModule.stageInfo(nullptr);

		auto fsModule = m_shaders.fs->createShaderModule(m_pipelineManager->m_device, m_resSlotMap);
		auto fsStage = fsModule.stageInfo(nullptr);

		VkPipelineShaderStageCreateInfo shaderStages[] = { vsStage, fsStage };

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = state.viewports.size();
		viewportState.pViewports = state.viewports.data();
		viewportState.scissorCount = state.scissors.size();
		viewportState.pScissors = state.scissors.data();


		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &state.vi.state;
		pipelineInfo.pInputAssemblyState = &state.ia;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &state.rs;
		pipelineInfo.pMultisampleState = &state.ms;
		pipelineInfo.pColorBlendState = &state.cb;
		pipelineInfo.layout = m_layout->pipelineLayout();
		pipelineInfo.renderPass = rp.handle();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		VkDevice device = *(m_pipelineManager->m_device);
		VkPipeline pipeline = VK_NULL_HANDLE;
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			LOG_ERR("failed to create graphics pipeline!");
			break;
		}

		instance = new GvePipelineInstance(pipeline, state, &rp);
	} while (false);
	return instance;
}

}  // namespace gve