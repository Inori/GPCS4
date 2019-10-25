#pragma once
#include "GveCommon.h"
#include "GveGraphicsPipeline.h"
#include "GveHash.h"

#include <unordered_map>

namespace gve
{;

class GvePipelineManager
{
	friend class GveGraphicsPipeline;
public:
	GvePipelineManager(GveDevice* device);
	~GvePipelineManager();

	GveGraphicsPipeline* getGraphicsPipeline(const GveGraphicsPipelineShaders& shaders);

private:
	GveDevice* m_device;
	std::unordered_map<GveGraphicsPipelineShaders, GveGraphicsPipeline,
		GveHash, GveEqual> m_graphicsPipelines;
};



}  // namespace gve