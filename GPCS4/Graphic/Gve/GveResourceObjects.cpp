#include "GveResourceObjects.h"
#include "GveDevice.h"

namespace gve
{;

GveResourceObjects::GveResourceObjects(GveDevice* device) :
	m_device(device),
	m_pipelineMgr(m_device),
	m_renderPassPool(m_device),
	m_sharpResMgr(device)
{

}

GveResourceObjects::~GveResourceObjects()
{
}

GvePipelineManager& GveResourceObjects::pipelineManager()
{
	return m_pipelineMgr;
}

GveRenderPassPool& GveResourceObjects::renderPassPool()
{
	return m_renderPassPool;
}

GveSharpResourceManager& GveResourceObjects::getSharpResManager()
{
	return m_sharpResMgr;
}

}  // namespace gve