#pragma once

#include "SceCommon.h"

#include "Violet/VltRc.h"

#include <memory>

namespace sce
{
	namespace vlt
	{
		class VltDevice;
		class VltCommandList;
	}  // namespace vlt

	namespace Gnm
	{
		class GnmCommandProcessor;
		class GnmCommandBuffer;
	}  // namespace Gnm

	enum class SceQueueType
	{
		Graphics = 0,
		Compute  = 1
	};

	struct SceGpuCommand
	{
		const void* buffer = nullptr;
		uint32_t    size   = 0;
	};

	struct SceGpuSubmission
	{
		vlt::Rc<vlt::VltCommandList> cmdList;
		VkSemaphore wait;
		VkSemaphore wake;
	};

	class SceGpuQueue
	{
	public:
		SceGpuQueue(
			vlt::VltDevice* device,
			SceQueueType    type);
		~SceGpuQueue();

		/**
	     * \brief Record command list.
	     * 
	     * Convert Gnm command buffer to Violet command list.
	     * \param cmd Gnm command buffer.
	     * \param displayBufferIndex Current display buffer index, 
	     *                           using to index render target.
	     * \returns The Violet command list recorded.
	     */
		vlt::Rc<vlt::VltCommandList>
			record(const SceGpuCommand& cmd);

		/**
	     * \brief Submit vulkan command list to device.
	     * 
	     * \param sync synchronization objects to wait and signal.
	     */
		void submit(const SceGpuSubmission& submission);

	private:
		void createQueue(SceQueueType type);

	private:
		vlt::VltDevice* m_device;

		std::unique_ptr<Gnm::GnmCommandProcessor> m_cp;
		std::unique_ptr<Gnm::GnmCommandBuffer>    m_cmdProducer;
	};

}  // namespace sce
