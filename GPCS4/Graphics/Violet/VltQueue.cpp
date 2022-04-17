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

		// TODO:
		// Calling synchronize will block the CPU waiting for the submission done on GPU,
		// thus will block the submit thread, slow down the submit speed and drop FPS.
		// DXVK's solution is to use another thread dedicated for cmdlist submission
		// thus overcome the performance problem,
		// see https://github.com/doitsujin/dxvk/blob/master/src/dxvk/dxvk_queue.cpp
		//
		// This can be easily implemented, but for now, our bottleneck is not here obviously.
		// So I keep it single threaded to make debugging easier.

		// Wait for command buffer submit finish.
		cmdList->synchronize();

		// After submit done, reset cmdlist to release resource.
		cmdList->reset();

		// Finally, recycle the cmdlist for next use.
		m_device->recycleCommandList(cmdList);
	}

	void VltSubmissionQueue::present(
		const VltPresentInfo& presentInfo)
	{
		auto& presenter = presentInfo.presenter;
		presenter->presentImage();
	}

	void VltSubmissionQueue::synchronize()
	{
		// Since currently we don't use another thread to submit command buffer,
		// we don't need to really synchronize.
		// This need to be implemented after we support asynchronous submit.
	}

}  // namespace sce::vlt