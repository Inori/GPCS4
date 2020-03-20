#pragma once

#include "SceCommon.h"

#include <memory>

namespace vlt
{;
class VltDevice;
class VltContext;
class VltPresenter;
class VltCmdList;
}  // namespace vlt

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
	RcPtr<vlt::VltDevice>        device;
	RcPtr<vlt::VltPresenter>     presenter;
	std::shared_ptr<SceVideoOut> videoOut;
};

struct SceGpuCommand
{
	const void* buffer = nullptr;
	uint32_t    size   = 0;
};

struct SceGpuSubmission
{
	RcPtr<vlt::VltCmdList> cmdList;
	VkSemaphore            wait;
	VkSemaphore            wake;
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
	 * Convert Gnm command buffer to Violet command list.
	 * \param cmd Gnm command buffer.
	 * \param displayBufferIndex Current display buffer index, 
	 *                           using to index render target.
	 * \returns The Violet command list recorded.
	 */
	RcPtr<vlt::VltCmdList> record(
		const SceGpuCommand& cmd,
		uint32_t             displayBufferIndex);

	/**
	 * \brief Submit vulkan command list.
	 * 
	 * \param sync synchronization objects to wait and signal.
	 */
	void submit(const SceGpuSubmission& submission);

private:
	void createQueue(SceQueueType type);

private:
	SceGpuQueueDevice      m_device;
	RcPtr<vlt::VltContext> m_context;

	std::unique_ptr<GnmCmdStream>     m_cmdParser;
	std::unique_ptr<GnmCommandBuffer> m_cmdProcesser;
};


}  // namespace sce
