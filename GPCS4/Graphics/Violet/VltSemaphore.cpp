#include "VltSemaphore.h"
#include "VltDevice.h"

namespace sce::vlt
{
	VltSemaphore::VltSemaphore(VltDevice*                    device,
							   const VltSemaphoreCreateInfo& info) :
		m_device(device)
	{
		VkSemaphoreTypeCreateInfo typeInfo;
		typeInfo.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
		typeInfo.pNext         = nullptr;
		typeInfo.semaphoreType = info.semaphoreType;
		typeInfo.initialValue  = info.initialValue;

		VkSemaphoreCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext = &typeInfo;
		createInfo.flags = 0;

		VkResult status = vkCreateSemaphore(m_device->handle(),
											&createInfo, nullptr, &m_handle);
		if (status != VK_SUCCESS)
		{
			Logger::exception("create semaphore failed.");
		}
	}

	VltSemaphore::~VltSemaphore()
	{
		if (m_handle != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(m_device->handle(), m_handle, nullptr);
			m_handle = VK_NULL_HANDLE;
		}
	}

	void VltSemaphore::wait(uint64_t value)
	{
		VkSemaphoreWaitInfo waitInfo;
		waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
		waitInfo.pNext = nullptr;
		waitInfo.flags = 0;
		waitInfo.semaphoreCount = 1;
		waitInfo.pSemaphores    = &m_handle;
		waitInfo.pValues        = &value;

		VkResult status = VK_TIMEOUT;

		while (status == VK_TIMEOUT)
		{
			status = vkWaitSemaphores(
				m_device->handle(), &waitInfo, 1'000'000'000ull);
		}
	}

	VltSemaphoreTracker::VltSemaphoreTracker()
	{
	}

	VltSemaphoreTracker::~VltSemaphoreTracker()
	{
	}

	void VltSemaphoreTracker::trackSemaphore(const Rc<VltSemaphore>& sema)
	{
		// Semaphores may be queued to signal or wait,
		// so only track it with the first operation.
		auto handle = sema->handle();
		if (m_semaphores.find(handle) == m_semaphores.end())
		{
			m_semaphores.emplace(handle, sema);
		}
	}

	void VltSemaphoreTracker::reset()
	{
		m_semaphores.clear();
	}

}  // namespace sce::vlt