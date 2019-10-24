#include "GveGraphicsPipeline.h"

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


GveGraphicsPipeline::GveGraphicsPipeline(GvePipelineManager* pipeMgr, const GveGraphicsPipelineShaders& shaders)
{

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

}

}  // namespace gve