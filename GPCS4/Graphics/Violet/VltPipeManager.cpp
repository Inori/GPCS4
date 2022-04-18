#include "VltPipeManager.h"
#include "VltImage.h"

namespace sce::vlt
{
	VltPipelineManager::VltPipelineManager(VltDevice* device) :
		m_device(device)
	{
	}

	VltPipelineManager::~VltPipelineManager()
	{
	}

	VltComputePipeline* VltPipelineManager::createComputePipeline(
		const VltComputePipelineShaders& shaders)
	{
		if (shaders.cs == nullptr)
			return nullptr;

		std::lock_guard<std::mutex> lock(m_mutex);

		auto pair = m_computePipelines.find(shaders);
		if (pair != m_computePipelines.end())
			return &pair->second;

		auto iter = m_computePipelines.emplace(
			std::piecewise_construct,
			std::tuple(shaders),
			std::tuple(this, shaders));
		return &iter.first->second;
	}

	VltGraphicsPipeline* VltPipelineManager::createGraphicsPipeline(
		const VltGraphicsPipelineShaders& shaders)
	{
		if (shaders.vs == nullptr)
			return nullptr;

		std::lock_guard<std::mutex> lock(m_mutex);

		auto pair = m_graphicsPipelines.find(shaders);
		if (pair != m_graphicsPipelines.end())
			return &pair->second;

		auto iter = m_graphicsPipelines.emplace(
			std::piecewise_construct,
			std::tuple(shaders),
			std::tuple(this, shaders));
		return &iter.first->second;
	}

	VltPipelineCount VltPipelineManager::getPipelineCount() const
	{
		VltPipelineCount result;
		result.numComputePipelines  = m_numComputePipelines.load();
		result.numGraphicsPipelines = m_numGraphicsPipelines.load();
		return result;
	}

}  // namespace sce::vlt