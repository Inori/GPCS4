#pragma once
#include "VltCommon.h"
#include "VltGraphicsPipeline.h"
#include "VltHash.h"

#include <unordered_map>

namespace vlt
{;

class VltPipelineManager
{
	friend class VltGraphicsPipeline;
public:
	VltPipelineManager(VltDevice* device);
	~VltPipelineManager();

	VltGraphicsPipeline* getGraphicsPipeline(const VltGraphicsPipelineShaders& shaders);

private:
	VltDevice* m_device;
	std::unordered_map<VltGraphicsPipelineShaders, VltGraphicsPipeline,
		VltHash, VltEqual> m_graphicsPipelines;
};



}  // namespace vlt