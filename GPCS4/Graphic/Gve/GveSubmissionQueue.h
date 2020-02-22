#pragma once

#include "GveCommon.h"

namespace gve
{;

struct GveSubmitInfo;
struct GvePresentInfo;

class GveDevice;
class GveCmdList;

// A submission queue to submit cmdlist asynchronously.

// TODO:
// Currently this is a dummy queue, which uses synchronous submit.
// This exists just for future compatibility when we really need a asynchronous queue. 

class GveSubmissionQueue
{
public:
	GveSubmissionQueue(GveDevice* device);
	~GveSubmissionQueue();

	void submit(const GveSubmitInfo& submission);

	void present(const GvePresentInfo& presentation);

private:
	GveDevice*        m_device;
	RcPtr<GveCmdList> m_cmdList;
};


}  // namespace gve