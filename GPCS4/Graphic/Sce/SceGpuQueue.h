#pragma once

#include "SceCommon.h"

#include <memory>

namespace gve
{;
class GveDevice;
class GveContext;
}  // namespace gve

class GnmCmdStream;
class GnmCommandBuffer;

namespace sce
{;

class SceVideoOut;
class ScePresenter;

enum class SceQueueType
{
	Graphics,
	Compute
};

struct SceGpuQueueDevice
{
	RcPtr<gve::GveDevice>         device;
	std::shared_ptr<ScePresenter> presenter;
	std::shared_ptr<SceVideoOut>  videoOut;
};

class SceGpuQueue
{
public:
	SceGpuQueue(
		const SceGpuQueueDevice& device,
		SceQueueType             type);
	~SceGpuQueue();

	void submit(void* cmd, uint32_t size);

private:
	void createQueue(SceQueueType type);

private:
	SceGpuQueueDevice      m_device;
	RcPtr<gve::GveContext> m_context;

	std::unique_ptr<GnmCmdStream>     m_cmdParser;
	std::unique_ptr<GnmCommandBuffer> m_cmdProcesser;
};


}  // namespace sce
