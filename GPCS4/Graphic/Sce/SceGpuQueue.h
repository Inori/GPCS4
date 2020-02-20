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

	/**
	 * \brief Record command buffer.
	 * 
	 * Convert Gnm command buffer to vulkan command buffer.
	 * \param cmd Gnm command buffer.
	 * \param displayBufferIndex Current display buffer index, 
	 *                           using to index render target.
	 * \returns bool
	 */
	bool record(const SceGpuCommand& cmd, uint32_t displayBufferIndex);

	/**
	 * \brief Submit vulkan command list.
	 * 
	 * \param sync synchronization objects to wait and signal.
	 * \returns bool
	 */
	bool submit(const SceGpuSync& sync);

	/**
	 * \brief Synchronize command buffer.
	 * 
	 * Wait command buffer submit finish.
	 * This must be called after submit, or will block CPU.
	 * \returns VkResult
	 */
	VkResult synchronize();

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
