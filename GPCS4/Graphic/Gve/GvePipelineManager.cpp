#include "GvePipelineManager.h"

namespace gve
{;

GvePipelineManager::GvePipelineManager(GveDevice* device) :
	m_device(device)
{

}

GvePipelineManager::~GvePipelineManager()
{
}


GveGraphicsPipeline* GvePipelineManager::getGraphicsPipeline(const GveGraphicsPipelineShaders& shaders)
{
	GveGraphicsPipeline* pipeline = nullptr;

	auto iter = m_graphicsPipelines.find(shaders);
	if (iter != m_graphicsPipelines.end())
	{
		pipeline = &iter->second;
	}
	else
	{
		auto pair = m_graphicsPipelines.emplace(
			std::piecewise_construct,
			std::tuple(shaders),
			std::tuple(this, shaders));
		pipeline = &pair.first->second;
		
	}
	return pipeline;
}

}  // namespace gve