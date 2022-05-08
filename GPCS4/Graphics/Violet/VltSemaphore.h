#pragma once

#include "VltCommon.h"

#include <unordered_map>

namespace sce::vlt
{
	class VltDevice;
	class VltSemaphore;
	
	struct VltSemaphoreSubmission
	{
		Rc<VltSemaphore>      semaphore;
		uint64_t              value;
		VkPipelineStageFlags2 stageMask;
	};

	/**
	 * \brief Semaphore create info
	 */
	struct VltSemaphoreCreateInfo
	{
		VkSemaphoreType semaphoreType;
		uint64_t        initialValue;
	};

	class VltSemaphore : public RcObject
	{
	public:
		VltSemaphore(VltDevice*                    device,
					 const VltSemaphoreCreateInfo& info);
		~VltSemaphore();

		/**
		 * \brief Vulkan semaphore handle
		 */
		VkSemaphore handle() const
		{
			return m_handle;
		}

		/**
		 * \brief Wait on host.
		 * 
		 * Only valid when type it is a
		 * timeline semaphore.
		 */
		void wait(uint64_t value);

	private:
		VltDevice*  m_device;
		VkSemaphore m_handle = VK_NULL_HANDLE;
	};


	/**
	 * \brief GPU semaphore tracker
	 *
	 */
	class VltSemaphoreTracker
	{

	public:
		VltSemaphoreTracker();
		~VltSemaphoreTracker();

		/**
		 * \brief Tracks an semaphore
		 */
		void trackSemaphore(const Rc<VltSemaphore>& sema);

		/**
		 * \brief Resets semaphore tracker
		 *
		 * Releases all tracked semaphores
		 */
		void reset();

	private:
		std::unordered_map<
			VkSemaphore, Rc<VltSemaphore>> m_semaphores;
	};

}  // namespace sce::vlt