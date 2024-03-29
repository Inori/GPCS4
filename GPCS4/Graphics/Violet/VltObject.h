#pragma once

#include "VltCommon.h"
#include "VltGpuEvent.h"
#include "VltMemory.h"
#include "VltPipeManager.h"
#include "VltUnbound.h"

namespace sce::vlt
{
	class VltDevice;

	class VltObjects
	{
	public:
		VltObjects(VltDevice* device) :
			m_device(device),
			m_memoryManager(device),
			m_pipelineManager(device),
			m_eventPool(device),
			//m_queryPool(device),
			m_dummyResources(device)
		{
		}

		VltMemoryAllocator& memoryManager()
		{
			return m_memoryManager;
		}

		VltPipelineManager& pipelineManager()
		{
			return m_pipelineManager;
		}

		VltGpuEventPool& eventPool()
		{
			return m_eventPool;
		}

		//DxvkGpuQueryPool& queryPool()
		//{
		//	return m_queryPool;
		//}

		VltUnboundResources& dummyResources()
		{
			return m_dummyResources;
		}

	private:
		VltDevice* m_device;

		VltMemoryAllocator m_memoryManager;
		VltPipelineManager m_pipelineManager;
		VltGpuEventPool    m_eventPool;
		//DxvkGpuQueryPool m_queryPool;

		VltUnboundResources m_dummyResources;
	};
}  // namespace sce::vlt