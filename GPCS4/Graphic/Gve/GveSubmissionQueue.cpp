#include "GveSubmissionQueue.h"

#include "GveDevice.h"
#include "GveCmdList.h"
#include "GvePresenter.h"

namespace gve
{;


GveSubmissionQueue::GveSubmissionQueue(GveDevice* device):
	m_device(device),
	m_cmdList(nullptr)
{
}

GveSubmissionQueue::~GveSubmissionQueue()
{
}

void GveSubmissionQueue::submit(const GveSubmitInfo& submission)
{
	m_cmdList = submission.cmdList;
	m_cmdList->submit(submission.waitSync, submission.wakeSync);
}

void GveSubmissionQueue::present(const GvePresentInfo& presentation)
{
	do
	{
		auto& presenter = presentation.presenter;
		VkResult status = presenter->presentImage(presentation.waitSync);
		if (status != VK_SUCCESS)
		{
			break;
		}

		// Wait for command buffer submit finish.
		status = m_cmdList->synchronize();
		if (status != VK_SUCCESS)
		{
			break;
		}

		// After submit done, reset cmdlist to release resource.
		m_cmdList->reset();

		// Finally, recycle the cmdlist for next use.
		// Here we release the ownership of m_cmdList to emulate item poped from queue.
		m_device->recycleCommandList(std::exchange(m_cmdList, nullptr));

	} while (false);
}

}  // namespace gve