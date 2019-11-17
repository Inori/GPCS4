#include "GveResourceObjects.h"
#include "GveDevice.h"

namespace gve
{;

GveResourceObjects::GveResourceObjects(GveDevice* device) :
	m_device(device),
	m_memAllocator(m_device),
	m_pipelineMgr(m_device),
	m_resourceMgr(m_device, &m_memAllocator),
	m_renderPassPool(m_device)
{

}

GveResourceObjects::~GveResourceObjects()
{
}

GvePipelineManager& GveResourceObjects::pipelineManager()
{
	return m_pipelineMgr;
}

GveResourceManager& GveResourceObjects::resourceManager()
{
	return m_resourceMgr;
}

GveRenderPassPool& GveResourceObjects::renderPassPool()
{
	return m_renderPassPool;
}

}  // namespace gve