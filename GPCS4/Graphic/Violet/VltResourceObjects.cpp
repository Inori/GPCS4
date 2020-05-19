#include "VltResourceObjects.h"
#include "VltDevice.h"

namespace vlt
{;

VltResourceObjects::VltResourceObjects(VltDevice* device) :
	m_device(device),
	m_pipelineMgr(m_device),
	m_renderPassPool(m_device)
{

}

VltResourceObjects::~VltResourceObjects()
{
}

VltPipelineManager& VltResourceObjects::pipelineManager()
{
	return m_pipelineMgr;
}

VltRenderPassPool& VltResourceObjects::renderPassPool()
{
	return m_renderPassPool;
}


}  // namespace vlt