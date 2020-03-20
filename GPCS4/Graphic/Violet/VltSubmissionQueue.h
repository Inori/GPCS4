#pragma once

#include "VltCommon.h"

#include <queue>

namespace vlt
{;

struct VltSubmitInfo;
struct VltPresentInfo;

class VltDevice;
class VltCmdList;

// A submission queue to submit cmdlist asynchronously.

// TODO:
// Currently this is a dummy queue, which uses synchronous submit.
// This exists just for future compatibility when we really need a asynchronous queue. 
// Actually it's easy to implement a simple version,
// just use lambda to pack the cmdlist, and then push to std::async
// I didn't do that because synchronous queue is more easy to debug.

class VltSubmissionQueue
{
public:
	VltSubmissionQueue(VltDevice* device);
	~VltSubmissionQueue();

	void submit(const VltSubmitInfo& submission);

	void present(const VltPresentInfo& presentation);

private:
	VltDevice*                    m_device;
	std::queue<RcPtr<VltCmdList>> m_submitQueue;
};


}  // namespace vlt