#pragma once

#include "VltCommon.h"
#include "VltPipelineManager.h"
#include "VltRenderPass.h"
#include "VltDescriptor.h"
#include "VltMemory.h"


namespace vlt
{;

class VltDevice;

class VltResourceObjects
{
public:
	VltResourceObjects(VltDevice* device);
	~VltResourceObjects();

	VltPipelineManager& pipelineManager();

	VltRenderPassPool& renderPassPool();

private:
	VltDevice* m_device;

	VltPipelineManager m_pipelineMgr;
	VltRenderPassPool m_renderPassPool;
};

}  // namespace vlt