#include "VltPipelineManager.h"

namespace vlt
{;

VltPipelineManager::VltPipelineManager(VltDevice* device) :
	m_device(device)
{

}

VltPipelineManager::~VltPipelineManager()
{
}


VltGraphicsPipeline* VltPipelineManager::createGraphicsPipeline(const VltGraphicsPipelineShaders& shaders)
{
	VltGraphicsPipeline* pipeline = nullptr;

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


VltComputePipeline* VltPipelineManager::createComputePipeline(
	const VltComputePipelineShaders& shaders)
{
	VltComputePipeline* pipeline = nullptr;

	auto iter = m_computePipelines.find(shaders);
	if (iter != m_computePipelines.end())
	{
		pipeline = &iter->second;
	}
	else
	{
		auto pair = m_computePipelines.emplace(
			std::piecewise_construct,
			std::tuple(shaders),
			std::tuple(this, shaders));
		pipeline = &pair.first->second;
	}
	return pipeline;
}

}  // namespace vlt