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

		// simulate async
		m_submitQueue.push(std::move(cmdList));
	}

	void VltSubmissionQueue::present(
		const VltPresentInfo& presentInfo,
		VltSubmitStatus*      status)
	{
		auto& presenter = presentInfo.presenter;
		presenter->presentImage();

		// simulate async
		auto cmdList = m_submitQueue.front();
		m_submitQueue.pop();

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
}  // namespace sce::vlt