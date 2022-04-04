#pragma once

#include "VltCommon.h"
#include "VltMemory.h"

namespace sce::vlt
{
	class VltDevice;

	class VltObjects
	{
	public:
		VltObjects(VltDevice* device) :
			m_device(device),
			m_memoryManager(device)
		//m_renderPassPool(device),
		//m_pipelineManager(device, &m_renderPassPool),
		//m_eventPool(device),
		//m_queryPool(device),
		//m_dummyResources(device)
		{
		}

		VltMemoryAllocator& memoryManager()
		{
			return m_memoryManager;
		}

		//DxvkRenderPassPool& renderPassPool()
		//{
		//	return m_renderPassPool;
		//}

		//DxvkPipelineManager& pipelineManager()
		//{
		//	return m_pipelineManager;
		//}

		//DxvkGpuEventPool& eventPool()
		//{
		//	return m_eventPool;
		//}

		//DxvkGpuQueryPool& queryPool()
		//{
		//	return m_queryPool;
		//}

		//DxvkUnboundResources& dummyResources()
		//{
		//	return m_dummyResources;
		//}

	private:
		VltDevice* m_device;

		VltMemoryAllocator m_memoryManager;
		//DxvkRenderPassPool  m_renderPassPool;
		//DxvkPipelineManager m_pipelineManager;

		//DxvkGpuEventPool m_eventPool;
		//DxvkGpuQueryPool m_queryPool;

		//DxvkUnboundResources m_dummyResources;
	};
}  // namespace sce::vlt