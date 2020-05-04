#pragma once
#include "VltCommon.h"
#include "VltGraphicsPipeline.h"
#include "VltComputePipeline.h"
#include "VltHash.h"

#include <unordered_map>

namespace vlt
{;

class VltPipelineManager
{
	friend class VltGraphicsPipeline;
	friend class VltComputePipeline;

public:
	VltPipelineManager(VltDevice* device);
	~VltPipelineManager();

	VltGraphicsPipeline* createGraphicsPipeline(
		const VltGraphicsPipelineShaders& shaders);

	VltComputePipeline* createComputePipeline(
		const VltComputePipelineShaders& shaders);

private:
	VltDevice* m_device;

	std::unordered_map<
		VltGraphicsPipelineShaders, 
		VltGraphicsPipeline, 
		VltHash, VltEqual> m_graphicsPipelines;

	std::unordered_map<
		VltComputePipelineShaders,
		VltComputePipeline,
		VltHash, VltEqual> m_computePipelines;
};



}  // namespace vlt