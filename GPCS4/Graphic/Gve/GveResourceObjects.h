#pragma once

#include "GveCommon.h"
#include "GvePipelineManager.h"
#include "GveRenderPass.h"
#include "GveResourceManager.h"
#include "GveDescriptor.h"
#include "GveMemory.h"


namespace gve
{;

class GveDevice;

class GveResourceObjects
{
public:
	GveResourceObjects(GveDevice* device);
	~GveResourceObjects();

	GvePipelineManager& pipelineManager();

	GveResourceManager& resourceManager();

	GveRenderPassPool& renderPassPool();

private:
	GveDevice* m_device;

	GveMemoryAllocator m_memAllocator;
	GvePipelineManager m_pipelineMgr;
	GveResourceManager m_resourceMgr;
	GveRenderPassPool m_renderPassPool;
};

}  // namespace gve