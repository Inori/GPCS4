#pragma once

#include "SceCommon.h"

#include <memory>


namespace sce
{;

namespace Gnm
{

class GnmCommandProcessor;
class GnmCommandBuffer;

}  // namespace Gnm

class SceVideoOut;

enum class SceQueueType
{
	Graphics = 0,
	Compute  = 1
};

struct SceGpuQueueDevice
{
	uint32_t placeHolder;
};

struct SceGpuCommand
{
	const void* buffer = nullptr;
	uint32_t    size   = 0;
};

struct SceGpuSubmission
{
	//RcPtr<vlt::VltCmdList> cmdList;
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
	void record(const SceGpuCommand& cmd);

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

	std::unique_ptr<Gnm::GnmCommandProcessor> m_cp;
	std::unique_ptr<Gnm::GnmCommandBuffer>    m_cmdProducer;
};


}  // namespace sce
