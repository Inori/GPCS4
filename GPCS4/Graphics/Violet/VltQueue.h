#pragma once

#include "VltCommon.h"
#include "VltCmdList.h"

namespace sce
{
	class ScePresenter;

	namespace vlt
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
			Rc<ScePresenter> presenter;
			uint32_t         imageIndex;
		};

		/**
         * \brief A submission queue to submit cmdlist asynchronously.
         *
         * TODO:
         * Currently this is a dummy queue, which uses synchronous submit.
	     * This exists just for future compatibility when we really need a asynchronous queue.
	     * I didn't do that because synchronous queue is more easier to debug.
         */
		class VltSubmissionQueue
		{
		public:
			VltSubmissionQueue(VltDevice* device);
			~VltSubmissionQueue();

			void submit(
				const VltSubmitInfo& submission);

			void present(
				const VltPresentInfo& presentInfo);

			void synchronize();

		private:
			VltDevice* m_device;
		};
	} // namespace vlt
}  // namespace sce