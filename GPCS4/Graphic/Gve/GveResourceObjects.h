#pragma once

#include "GveCommon.h"
#include "GvePipelineManager.h"
#include "GveSharpResourceManager.h"
#include "GveRenderPass.h"
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

	GveRenderPassPool& renderPassPool();

	GveSharpResourceManager& getSharpResManager();

private:
	GveDevice* m_device;

	GvePipelineManager m_pipelineMgr;
	GveRenderPassPool m_renderPassPool;
	GveSharpResourceManager m_sharpResMgr;
};

}  // namespace gve