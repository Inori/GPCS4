#pragma once

#include "SceCommon.h"

#include <memory>

namespace gve
{;
class GveDevice;
class GveContext;
class GvePresenter;
class GveCmdList;
}  // namespace gve

class GnmCmdStream;
class GnmCommandBuffer;

namespace sce
{;

class SceVideoOut;

enum class SceQueueType
{
	Graphics = 0,
	Compute  = 1
};

struct SceGpuQueueDevice
{
	RcPtr<gve::GveDevice>        device;
	RcPtr<gve::GvePresenter>     presenter;
	std::shared_ptr<SceVideoOut> videoOut;
};

struct SceGpuCommand
{
	const void* buffer = nullptr;
	uint32_t    size   = 0;
};

struct SceGpuSync
{
	VkSemaphore wait;
	VkSemaphore wake;
};

class SceGpuQueue
{
public:
	SceGpuQueue(
		const SceGpuQueueDevice& device,
		SceQueueType             type);
	~SceGpuQueue();

	bool record(const SceGpuCommand& cmd, uint32_t displayBufferIndex);

	VkResult synchronize();

	bool submit(const SceGpuSync& sync);

private:
	void createQueue(SceQueueType type);

private:
	SceGpuQueueDevice      m_device;
	RcPtr<gve::GveContext> m_context;

	std::unique_ptr<GnmCmdStream>     m_cmdParser;
	std::unique_ptr<GnmCommandBuffer> m_cmdProcesser;
	RcPtr<gve::GveCmdList>            m_cmdList;
};


}  // namespace sce
