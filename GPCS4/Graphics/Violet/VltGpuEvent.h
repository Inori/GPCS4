#pragma once

#include "UtilSync.h"
#include "VltCommon.h"
#include "VltResource.h"

namespace sce::vlt
{
	class VltDevice;
	class VltGpuEventPool;

	/**
	 * \brief Event status
	 *
	 * Reports whether the event is in
	 * a signaled or unsignaled state.
	 */
	enum class VltGpuEventStatus : uint32_t
	{
		Invalid  = 0,
		Pending  = 1,
		Signaled = 2,
	};

	/**
	 * \brief Event handle
	 *
	 * Stores the event handle itself as well
	 * as a pointer to the pool that the event
	 * was allocated from.
	 */
	struct VltGpuEventHandle
	{
		VltGpuEventPool* pool  = nullptr;
		VkEvent          event = VK_NULL_HANDLE;
	};

	/**
	 * \brief GPU event
	 *
	 * An event managed by the GPU which allows
	 * the application to check whether a specific
	 * command has completed execution.
	 */
	class VltGpuEvent : public VltResource
	{

	public:
		VltGpuEvent(VltDevice* device);
		~VltGpuEvent();

		/**
		 * \brief Retrieves event status
		 *
		 * Only valid after the event has been
		 * recorded intro a command buffer.
		 * \returns Event status
		 */
		VltGpuEventStatus test() const;

		/**
		 * \brief Resets event
		 *
		 * Assigns a new Vulkan event to this event
		 * object and replaces the old one. The old
		 * event should be freed as soon as the GPU
		 * stops using it.
		 * \param [in] handle New GPU event handle
		 * \returns Old GPU event handle
		 */
		VltGpuEventHandle reset(VltGpuEventHandle handle);

	private:
		VltDevice*        m_device;
		VltGpuEventHandle m_handle;
	};

	/**
	 * \brief Event pool
	 *
	 * Thread-safe event allocator that provides
	 * a way to create and recycle Vulkan events.
	 */
	class VltGpuEventPool
	{

	public:
		VltGpuEventPool(VltDevice* device);
		~VltGpuEventPool();

		/**
		 * \brief Allocates an event
		 *
		 * Either returns a recycled event, or
		 * creates a new one if necessary. The
		 * state of the event is undefined.
		 * \returns An event handle
		 */
		VltGpuEventHandle allocEvent();

		/**
		 * \brief Recycles an event
		 *
		 * \param [in] handle Event to free
		 */
		void freeEvent(VkEvent event);

	private:
		VltDevice*           m_device;
		util::sync::Spinlock m_mutex;
		std::vector<VkEvent> m_events;
	};

	/**
	 * \brief GPU event tracker
	 *
	 * Stores events currently accessed by the
	 * GPU, and returns them to the event pool
	 * once they are no longer in use.
	 */
	class VltGpuEventTracker
	{

	public:
		VltGpuEventTracker();
		~VltGpuEventTracker();

		/**
		 * \brief Tracks an event
		 * \param [in] handle Event to track
		 */
		void trackEvent(VltGpuEventHandle handle);

		/**
		 * \brief Resets event tracker
		 *
		 * Releases all tracked events back
		 * to the respective event pool
		 */
		void reset();

	private:
		std::vector<VltGpuEventHandle> m_handles;
	};

}  // namespace sce::vlt