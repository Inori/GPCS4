#include "VltQueue.h"
#include "VltDevice.h"
#include "Sce/ScePresenter.h"

namespace sce::vlt
{

	VltSubmissionQueue::VltSubmissionQueue(VltDevice* device) :
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

	void VltSubmissionQueue::present(
		const VltPresentInfo& presentInfo,
		VltSubmitStatus*      status)
	{

		auto& presenter = presentInfo.presenter;
		presenter->presentImage();


		auto cmdList = m_submitQueue.front();
		m_submitQueue.pop();

		// Wait for command buffer submit finish.
		cmdList->synchronize();

		// After submit done, reset cmdlist to release resource.
		cmdList->reset();

		// Finally, recycle the cmdlist for next use.
		m_device->recycleCommandList(cmdList);
	}
}  // namespace sce::vlt