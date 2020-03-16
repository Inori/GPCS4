#include "VltSubmissionQueue.h"

#include "VltDevice.h"
#include "VltCmdList.h"
#include "VltPresenter.h"

namespace vlt
{;


VltSubmissionQueue::VltSubmissionQueue(VltDevice* device):
	m_device(device)
{
}

VltSubmissionQueue::~VltSubmissionQueue()
{
}

void VltSubmissionQueue::submit(const VltSubmitInfo& submission)
{
	auto& cmdList = submission.cmdList;
	cmdList->submit(submission.waitSync, submission.wakeSync);
	m_submitQueue.push(std::move(cmdList));
}

void VltSubmissionQueue::present(const VltPresentInfo& presentation)
{
	do
	{
		auto& presenter = presentation.presenter;
		VkResult status = presenter->presentImage(presentation.waitSync);
		if (status != VK_SUCCESS)
		{
			break;
		}

		auto cmdList = m_submitQueue.front();
		m_submitQueue.pop();

		// Wait for command buffer submit finish.
		status = cmdList->synchronize();
		if (status != VK_SUCCESS)
		{
			break;
		}

		// After submit done, reset cmdlist to release resource.
		cmdList->reset();

		// Finally, recycle the cmdlist for next use.
		// Here we release the ownership of m_cmdList to emulate item poped from queue.
		m_device->recycleCommandList(std::exchange(cmdList, nullptr));

	} while (false);
}

}  // namespace vlt