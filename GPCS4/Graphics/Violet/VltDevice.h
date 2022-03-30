#pragma once

#include "VltCommon.h"
#include "UtilSync.h"
#include "VltInstance.h"
#include "VltAdapter.h"
#include "VltContext.h"
#include "VltCmdList.h"
#include "VltRecycler.h"
#include "VltQueue.h"


namespace sce::vlt
{
	class VltInstance;

	/**
     * \brief Device queue
     * 
     * Stores a Vulkan queue and the
     * queue family that it belongs to.
     */
	struct VltDeviceQueue
	{
		VkQueue  queueHandle = VK_NULL_HANDLE;
		uint32_t queueFamily = 0;
		uint32_t queueIndex  = 0;
	};

	/**
     * \brief Device queue infos
     */
	struct VltDeviceQueueSet
	{
		VltDeviceQueue graphics;
		VltDeviceQueue compute;
		VltDeviceQueue transfer;
	};

	/**
     * \brief Violet device
     * 
     * Device object. This is responsible for resource creation,
     * memory allocation, command submission and state tracking.
     * Rendering commands are recorded into command lists using
     * contexts. Multiple contexts can be created for a device.
     */
	class VltDevice : public RcObject
	{
	public:
		VltDevice(
			const Rc<VltInstance>&     instance,
			const Rc<VltAdapter>&      adapter,
			VkDevice                   device,
			const VltDeviceExtensions& extensions,
			const VltDeviceFeatures&   features);

		~VltDevice();

		/**
         * \brief Logical device handle
         * \returns The device handle
         */
		VkDevice handle() const
		{
			return m_device;
		}

		/**
         * \brief Queue handles
         * 
         * Handles and queue family indices
         * of all known device queues.
         * \returns Device queue infos
         */
		const VltDeviceQueueSet& queues() const
		{
			return m_queues;
		}

		/**
         * \brief Tests whether a dedicated transfer queue is available
         * \returns \c true if an transfer queue is supported by the device
         */
		bool hasDedicatedTransferQueue() const
		{
			return m_queues.transfer.queueHandle != 
                m_queues.graphics.queueHandle;
		}

		/**
         * \brief Tests whether a dedicated compute queue is available
         * \returns \c true if an compute queue is supported by the device
         */
		bool hasDedicatedComputeQueue() const
		{
			return m_queues.compute.queueHandle !=
				   m_queues.graphics.queueHandle;
		}

		/**
         * \brief The instance
         * 
         * The VLT instance that created this device.
         * \returns Instance
         */
		Rc<VltInstance> instance() const
		{
			return m_instance;
		}

		/**
         * \brief The adapter
         * 
         * The physical device that the
         * device has been created for.
         * \returns Adapter
         */
		Rc<VltAdapter> adapter() const
		{
			return m_adapter;
		}

		/**
         * \brief Enabled device extensions
         * \returns Enabled device extensions
         */
		const VltDeviceExtensions& extensions() const
		{
			return m_extensions;
		}

		/**
         * \brief Enabled device features
         * \returns Enabled features
         */
		const VltDeviceFeatures& features() const
		{
			return m_features;
		}

		/**
         * \brief Device properties
         * \returns Device properties
         */
		const VltDeviceInfo& properties() const
		{
			return m_properties;
		}

		/**
         * \brief Checks whether this is a UMA system
         *
         * Basically tests whether all heaps are device-local.
         * Can be used for various optimizations in client APIs.
         * \returns \c true if the system has unified memory.
         */
		bool isUnifiedMemoryArchitecture() const;

		/**
        * \brief Queries supported shader stages
        * \returns Supported shader pipeline stages
        */
		VkPipelineStageFlags getShaderPipelineStages() const;

		    
		/**
        * \brief Creates a command list
        * \returns The command list
        */
		Rc<VltCommandList> createCommandList();

		/**
        * \brief Creates a context
        * 
        * Creates a context object that can
        * be used to record command buffers.
        * \returns The context object
        */
		Rc<VltContext> createContext();

		/**
		 * \brief Submits a command list
		 * 
		 * Submits the given command list to the device using
		 * the given set of optional synchronization primitives.
		 * \param [in] commandList The command list to submit
		 * \param [in] waitSync (Optional) Semaphore to wait on
		 * \param [in] wakeSync (Optional) Semaphore to notify
		 */
		void submitCommandList(
			const Rc<VltCommandList>& commandList,
			VkSemaphore               waitSync,
			VkSemaphore               wakeSync);

		/**
        * \brief Waits until the device becomes idle
        * 
        * Waits for the GPU to complete the execution of all
        * previously submitted command buffers. This may be
        * used to ensure that resources that were previously
        * used by the GPU can be safely destroyed.
        */
		void waitForIdle();

	private:
		void recycleCommandList(
			const Rc<VltCommandList>& cmdList);

		VltDeviceQueue getQueue(
			uint32_t family,
			uint32_t index) const;

	private:
		VkDevice m_device;

		Rc<VltInstance>     m_instance;
		Rc<VltAdapter>      m_adapter;
		VltDeviceExtensions m_extensions;

		VltDeviceFeatures m_features;
		VltDeviceInfo     m_properties;

		VltDeviceQueueSet m_queues;

		VltSubmissionQueue m_submissionQueue;

		VltRecycler<VltCommandList, 16> m_recycledCommandLists;
	};

}  // namespace sce::vlt