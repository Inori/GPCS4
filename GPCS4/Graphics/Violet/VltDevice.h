#pragma once

#include "UtilSync.h"
#include "VltAdapter.h"
#include "VltCommon.h"

namespace sce::vlt
{
	class VltInstance;

	/**
   * \brief Device options
   */
	struct VltDeviceOptions
	{
		uint32_t maxNumDynamicUniformBuffers = 0;
		uint32_t maxNumDynamicStorageBuffers = 0;
	};

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
   * \brief DXVK device
   * 
   * Device object. This is responsible for resource creation,
   * memory allocation, command submission and state tracking.
   * Rendering commands are recorded into command lists using
   * contexts. Multiple contexts can be created for a device.
   */
	class VltDevice : public RcObject
	{
		//friend class DxvkContext;
		//friend class DxvkSubmissionQueue;
		//friend class DxvkDescriptorPoolTracker;

	public:
		VltDevice(
			const Rc<VltInstance>&     instance,
			const Rc<VltAdapter>&      adapter,
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
        * \returns \c true if an SDMA queue is supported by the device
        */
		bool hasDedicatedTransferQueue() const
		{
			return m_queues.transfer.queueHandle != m_queues.graphics.queueHandle;
		}

		/**
        * \brief The instance
        * 
        * The DXVK instance that created this device.
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
        * \brief Get device status
        * 
        * This may report device loss in
        * case a submission failed.
        * \returns Device status
        */
		//VkResult getDeviceStatus() const
		//{
		//	return m_submissionQueue.getLastError();
		//}

		/**
        * \brief Checks whether this is a UMA system
        *
        * Basically tests whether all heaps are device-local.
        * Can be used for various optimizations in client APIs.
        * \returns \c true if the system has unified memory.
        */
		bool isUnifiedMemoryArchitecture() const;

		/**
        * \brief Queries default framebuffer size
        * \returns Default framebuffer size
        */
		//DxvkFramebufferSize getDefaultFramebufferSize() const;

		/**
        * \brief Queries supported shader stages
        * \returns Supported shader pipeline stages
        */
		VkPipelineStageFlags getShaderPipelineStages() const;

		/**
        * \brief Retrieves device options
        * \returns Device options
        */
		VltDeviceOptions options() const;

		/**
        * \brief Creates a command list
        * \returns The command list
        */
		//Rc<DxvkCommandList> createCommandList();

		/**
        * \brief Creates a descriptor pool
        * 
        * Returns a previously recycled pool, or creates
        * a new one if necessary. The context should take
        * ownership of the returned pool.
        * \returns Descriptor pool
        */
		//Rc<DxvkDescriptorPool> createDescriptorPool();

		/**
        * \brief Creates a context
        * 
        * Creates a context object that can
        * be used to record command buffers.
        * \returns The context object
        */
		//Rc<DxvkContext> createContext();

		/**
        * \brief Creates a GPU event
        * \returns New GPU event
        */
		//Rc<DxvkGpuEvent> createGpuEvent();

		/**
        * \brief Creates a query
        * 
        * \param [in] type Query type
        * \param [in] flags Query flags
        * \param [in] index Query index
        * \returns New query
        */
		//Rc<DxvkGpuQuery> createGpuQuery(
		//	VkQueryType         type,
		//	VkQueryControlFlags flags,
		//	uint32_t            index);

		/**
        * \brief Creates framebuffer for a set of render targets
        * 
        * \param [in] info Framebuffer info
        * \returns The framebuffer object
        */
		//Rc<DxvkFramebuffer> createFramebuffer(
		//	const DxvkFramebufferInfo& info);

		/**
        * \brief Creates a buffer object
        * 
        * \param [in] createInfo Buffer create info
        * \param [in] memoryType Memory type flags
        * \returns The buffer object
        */
		//Rc<DxvkBuffer> createBuffer(
		//	const DxvkBufferCreateInfo& createInfo,
		//	VkMemoryPropertyFlags       memoryType);

		/**
        * \brief Creates a buffer view
        * 
        * \param [in] buffer The buffer to view
        * \param [in] createInfo Buffer view properties
        * \returns The buffer view object
        */
		//Rc<DxvkBufferView> createBufferView(
		//	const Rc<DxvkBuffer>&           buffer,
		//	const DxvkBufferViewCreateInfo& createInfo);

		/**
        * \brief Creates an image object
        * 
        * \param [in] createInfo Image create info
        * \param [in] memoryType Memory type flags
        * \returns The image object
        */
		//Rc<DxvkImage> createImage(
		//	const DxvkImageCreateInfo& createInfo,
		//	VkMemoryPropertyFlags      memoryType);

		/**
        * \brief Creates an image object for an existing VkImage
        * 
        * \param [in] createInfo Image create info
        * \param [in] image Vulkan image to wrap
        * \returns The image object
        */
		//Rc<DxvkImage> createImageFromVkImage(
		//	const DxvkImageCreateInfo& createInfo,
		//	VkImage                    image);

		/**
        * \brief Creates an image view
        * 
        * \param [in] image The image to create a view for
        * \param [in] createInfo Image view create info
        * \returns The image view
        */
		//Rc<DxvkImageView> createImageView(
		//	const Rc<DxvkImage>&           image,
		//	const DxvkImageViewCreateInfo& createInfo);

		/**
        * \brief Creates a sampler object
        * 
        * \param [in] createInfo Sampler parameters
        * \returns Newly created sampler object
        */
		//Rc<DxvkSampler> createSampler(
		//	const DxvkSamplerCreateInfo& createInfo);

		/**
        * \brief Creates a shader module
        * 
        * \param [in] stage Shader stage
        * \param [in] slotCount Resource slot count
        * \param [in] slotInfos Resource slot descriptions
        * \param [in] iface Inter-stage interface slots
        * \param [in] code Shader code
        * \returns New shader module
        */
		//Rc<DxvkShader> createShader(
		//	VkShaderStageFlagBits     stage,
		//	uint32_t                  slotCount,
		//	const DxvkResourceSlot*   slotInfos,
		//	const DxvkInterfaceSlots& iface,
		//	const SpirvCodeBuffer&    code);

		/**
        * \brief Retrieves stat counters
        * 
        * Can be used by the HUD to display some
        * internal information, such as memory
        * usage, draw calls, etc.
        */
		//DxvkStatCounters getStatCounters();

		/**
        * \brief Retrieves memors statistics
        *
        * \param [in] heap Memory heap index
        * \returns Memory stats for this heap
        */
		//DxvkMemoryStats getMemoryStats(uint32_t heap);

		/**
        * \brief Retreves current frame ID
        * \returns Current frame ID
        */
		uint32_t getCurrentFrameId() const;

		/**
        * \brief Initializes dummy resources
        * 
        * Should be called after creating the device in
        * case the device initialization was successful
        * and the device is usable.
        */
		void initResources();

		/**
        * \brief Registers a shader
        * \param [in] shader Newly compiled shader
        */
		//void registerShader(
		//	const Rc<DxvkShader>& shader);

		/**
        * \brief Presents a swap chain image
        * 
        * Invokes the presenter's \c presentImage method on
        * the submission thread. The status of this operation
        * can be retrieved with \ref waitForSubmission.
        * \param [in] presenter The presenter
        * \param [out] status Present status
        */
		//void presentImage(
		//	const Rc<vk::Presenter>& presenter,
		//	DxvkSubmitStatus*        status);

		/**
        * \brief Submits a command list
        * 
        * Submits the given command list to the device using
        * the given set of optional synchronization primitives.
        * \param [in] commandList The command list to submit
        * \param [in] waitSync (Optional) Semaphore to wait on
        * \param [in] wakeSync (Optional) Semaphore to notify
        */
		//void submitCommandList(
		//	const Rc<DxvkCommandList>& commandList,
		//	VkSemaphore                waitSync,
		//	VkSemaphore                wakeSync);

		/**
        * \brief Locks submission queue
        * 
        * Since Vulkan queues are only meant to be accessed
        * from one thread at a time, external libraries need
        * to lock the queue before submitting command buffers.
        */
		//void lockSubmission()
		//{
		//	m_submissionQueue.synchronize();
		//	m_submissionQueue.lockDeviceQueue();
		//}

		/**
        * \brief Unlocks submission queue
        * 
        * Releases the Vulkan queues again so that DXVK
        * itself can use them for submissions again.
        */
		//void unlockSubmission()
		//{
		//	m_submissionQueue.unlockDeviceQueue();
		//}

		/**
        * \brief Number of pending submissions
        * 
        * A return value of 0 indicates
        * that the GPU is currently idle.
        * \returns Pending submission count
        */
		//uint32_t pendingSubmissions() const
		//{
		//	return m_submissionQueue.pendingSubmissions();
		//}

		/**
        * \brief Increments a given stat counter
        *
        * \param [in] counter Stat counter to increment
        * \param [in] value Increment value
        */
		//void addStatCtr(DxvkStatCounter counter, uint64_t value)
		//{
		//	std::lock_guard<sync::Spinlock> lock(m_statLock);
		//	m_statCounters.addCtr(counter, value);
		//}

		/**
        * \brief Waits for a given submission
        * 
        * \param [in,out] status Submission status
        * \returns Result of the submission
        */
		//VkResult waitForSubmission(DxvkSubmitStatus* status);

		/**
        * \brief Waits for resource to become idle
        *
        * \param [in] resource Resource to wait for
        * \param [in] access Access mode to check
        */
		//void waitForResource(const Rc<DxvkResource>& resource, DxvkAccess access);

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
		VkDevice m_device;

		Rc<VltInstance>     m_instance;
		Rc<VltAdapter>      m_adapter;
		VltDeviceExtensions m_extensions;

		VltDeviceFeatures m_features;
		VltDeviceInfo     m_properties;

		// VltObjects         m_objects;

		util::sync::Spinlock m_statLock;
		//DxvkStatCounters m_statCounters;

		VltDeviceQueueSet m_queues;

		//DxvkRecycler<DxvkCommandList, 16>    m_recycledCommandLists;
		//DxvkRecycler<DxvkDescriptorPool, 16> m_recycledDescriptorPools;

		//DxvkSubmissionQueue m_submissionQueue;

		//void recycleCommandList(
		//	const Rc<DxvkCommandList>& cmdList);

		//void recycleDescriptorPool(
		//	const Rc<DxvkDescriptorPool>& pool);

		VltDeviceQueue getQueue(
			uint32_t family,
			uint32_t index) const;
	};

}  // namespace sce::vlt