#pragma once

#include "VltCommon.h"
#include "VltCmdList.h"

#include <queue>

namespace sce::vlt
{
	/**
     * \brief Queue submission info
     * 
     * Stores parameters used to submit
     * a command buffer to the device.
     */
	struct VltSubmitInfo
	{
		Rc<VltCommandList> cmdList;
		VkSemaphore        waitSync;
		VkSemaphore        wakeSync;
	};

	/**
     * \brief Present info
     *
     * Stores parameters used to present
     * a swap chain image on the device.
     */
	struct VltPresentInfo
	{
		uint32_t placeHolder;
	};


	/**
     * \brief A submission queue to submit cmdlist asynchronously.
     *
     * TODO:
     * Currently this is a dummy queue, which uses synchronous submit.
	 * This exists just for future compatibility when we really need a asynchronous queue.
	 * Actually it's easy to implement a simple version,
	 * just use lambda to pack the cmdlist, and then push to std::async
	 * I didn't do that because synchronous queue is more easy to debug.
     */
	class VltSubmissionQueue
	{
	public:
		VltSubmissionQueue(VltDevice* device);
		~VltSubmissionQueue();

		void submit(const VltSubmitInfo& submission);

		void present(const VltPresentInfo& presentation);

	private:
		VltDevice*                     m_device;
		std::queue<Rc<VltCommandList>> m_submitQueue;
	};

}  // namespace sce::vlt