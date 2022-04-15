#pragma once

#include "VltCommon.h"
#include "VltCmdList.h"
#include "VltBarrier.h"
#include "VltContextState.h"
#include "VltStaging.h"

namespace sce::vlt
{
	class VltDevice;
	class VltImage;
	class VltObjects;
	class VltGpuEvent;
	class VltSampler;
	class VltDescriptorPool;

	/**
     * \brief Bound shader resources
     * 
     * Stores the resources bound to a binding
     * slot in DXVK. These are used to create
     * descriptor sets.
     */
	struct VltShaderResourceSlot
	{
		Rc<VltSampler>    sampler;
		Rc<VltImageView>  imageView;
		Rc<VltBufferView> bufferView;
		VltBufferSlice    bufferSlice;
	};

	/**
     * \brief DXVk context
     * 
     * Tracks pipeline state and records command lists.
     * This is where the actual rendering commands are
     * recorded.
     */
	class VltContext : public RcObject
	{
	public:
		VltContext(VltDevice* device);
		virtual ~VltContext();

	    /**
         * \brief Begins command buffer recording
         * 
         * Begins recording a command list. This does
         * not alter any context state other than the
         * active command list.
         * \param [in] cmdList Target command list
         */
		void beginRecording(
			const Rc<VltCommandList>& cmdList);

		/**
         * \brief Ends command buffer recording
         * 
         * Finishes recording the active command list.
         * The command list can then be submitted to
         * the device.
         * 
         * This will not change any context state
         * other than the active command list.
         * \returns Active command list
         */
		Rc<VltCommandList> endRecording();


        /**
         * \brief Sets render targets
         * 
         * \param [in] slot Slot number
         * \param [in] targets Render targets to bind
         */
		void bindRenderTarget(
			uint32_t             slot,
			const VltAttachment& target);

        /**
         * \brief Sets depth render targets
         * 
         * \param [in] targets Render targets to bind
         */
		void bindDepthRenderTarget(
			const VltAttachment& depthTarget);


        /**
         * \brief Binds buffer as a shader resource
         * 
         * Can be used for uniform and storage buffers.
         * \param [in] slot Resource binding slot
         * \param [in] buffer Buffer to bind
         */
		void bindResourceBuffer(
			uint32_t              slot,
			const VltBufferSlice& buffer);
        /**
         * \brief Binds a shader to a given state
         * 
         * \param [in] stage Target shader stage
         * \param [in] shader The shader to bind
         */
		void bindShader(
			VkShaderStageFlagBits stage,
			const Rc<VltShader>&  shader);


        /**
         * \brief Starts compute jobs
         * 
         * \param [in] x Number of threads in X direction
         * \param [in] y Number of threads in Y direction
         * \param [in] z Number of threads in Z direction
         */
		void dispatch(
			uint32_t x,
			uint32_t y,
			uint32_t z);


        /**
         * \brief Uses transfer queue to initialize buffer
         * 
         * Only safe to use if the buffer is not in use by the GPU.
         * \param [in] buffer The buffer to initialize
         * \param [in] data The data to copy to the buffer
         */
		void uploadBuffer(
			const Rc<VltBuffer>& buffer,
			const void*          data);

        /**
         * \brief Uses transfer queue to initialize image
         * 
         * Only safe to use if the image is not in use by the GPU.
         * \param [in] image The image to initialize
         * \param [in] subresources Subresources to initialize
         * \param [in] data Source data
         * \param [in] pitchPerRow Row pitch of the source data
         * \param [in] pitchPerLayer Layer pitch of the source data
         */
		void uploadImage(
			const Rc<VltImage>&             image,
			const VkImageSubresourceLayers& subresources,
			const void*                     data,
			VkDeviceSize                    pitchPerRow,
			VkDeviceSize                    pitchPerLayer);

        /**
         * \brief Transforms image subresource layouts
         * 
         * \param [in] dstImage Image to transform
         * \param [in] dstSubresources Subresources
         * \param [in] srcLayout Current layout
         * \param [in] dstLayout Desired layout
         */
		void transformImage(
			const Rc<VltImage>&            dstImage,
			const VkImageSubresourceRange& dstSubresources,
			VkImageLayout                  srcLayout,
			VkImageLayout                  dstLayout);

        /**
         * \brief Transforms image subresource layouts
         */
		void transformImage(
			const Rc<VltImage>&            dstImage,
			const VkImageSubresourceRange& dstSubresources,
			VkImageLayout                  srcLayout,
			VkPipelineStageFlags2          srcStages,
			VkAccessFlags2                 srcAccess,
			VkImageLayout                  dstLayout,
			VkPipelineStageFlags2          dstStages,
			VkAccessFlags2                 dstAccess);


        /**
         * \brief Sets viewports
         * 
         * \param [in] viewportCount Number of viewports
         * \param [in] viewports The viewports
         * \param [in] scissorRects Schissor rectangles
         */
		void setViewports(
			uint32_t          viewportCount,
			const VkViewport* viewports);

        /**
         * \brief Sets viewports
         * 
         * \param [in] scissorCount Number of schissor rectangles
         * \param [in] scissorRects Schissor rectangles
         */
		void setScissors(
			uint32_t        scissorCount,
			const VkRect2D* scissorRects);

        /**
         * \brief Sets barrier control flags
         *
         * Barrier control flags can be used to control
         * implicit synchronization of compute shaders.
         * \param [in] control New barrier control flags
         */
		void setBarrierControl(
			VltBarrierControlFlags control);

        /**
         * \brief Signals a GPU event
         * \param [in] event The event
         */
		void signalGpuEvent(
			const Rc<VltGpuEvent>&  event,
			const VkDependencyInfo* dependencyInfo);

        /**
         * \brief Waits a GPU event
         * \param [in] event The event
         */
		void waitGpuEvent(
			const Rc<VltGpuEvent>&  event,
			const VkDependencyInfo* dependencyInfo);

        /**
         * \brief Queues a signal
         * 
         * The signal will be notified after all
         * previously submitted commands have
         * finished execution on the GPU.
         * \param [in] signal The signal
         * \param [in] value Signal value
         */
		void signal(
			const Rc<util::sync::Signal>& signal,
			uint64_t                      value);


    private:
		void beginRendering();

        void endRendering();

		bool updateGraphicsPipeline();

		bool updateComputePipeline();
		bool updateComputePipelineState();

        template <VkPipelineBindPoint BindPoint>
		void updatePushConstants();

        template <VkPipelineBindPoint BindPoint>
		void updateShaderResources(
			const VltPipelineLayout* layout);

		template <VkPipelineBindPoint BindPoint>
		void updateShaderDescriptorSetBinding(
			VkDescriptorSet          set,
			const VltPipelineLayout* layout);

        void updateComputeShaderResources();
		void updateGraphicsShaderResources();

		template <bool Indexed, bool Indirect>
		bool commitGraphicsState();
        bool commitComputeState();

        void commitComputePrevBarriers();
		void commitComputePostBarriers();

        VltGraphicsPipeline* lookupGraphicsPipeline(
			const VltGraphicsPipelineShaders& shaders);

        VltComputePipeline* lookupComputePipeline(
			const VltComputePipelineShaders& shaders);

        VkDescriptorSet allocateDescriptorSet(
			VkDescriptorSetLayout layout);

	private:
		VltDevice*  m_device;
		VltObjects* m_common;

        Rc<VltCommandList>    m_cmd;
		Rc<VltDescriptorPool> m_descPool;
        
        VltContextFlags m_flags;
		VltContextState m_state;

        VkPipeline m_gpActivePipeline = VK_NULL_HANDLE;
		VkPipeline m_cpActivePipeline = VK_NULL_HANDLE;

        VltBarrierSet m_execBarriers;
		VltBarrierSet m_transBarriers;
		VltBarrierSet m_initBarriers;
		VltBarrierSet m_transAcquires;
		VltBarrierControlFlags m_barrierControl;
		
        VkDescriptorSet m_gpSet = VK_NULL_HANDLE;
		VkDescriptorSet m_cpSet = VK_NULL_HANDLE;

        std::array<VltShaderResourceSlot, MaxNumResourceSlots> m_rc            = {};
		std::array<VltGraphicsPipeline*, 4096>                 m_gpLookupCache = {};
		std::array<VltComputePipeline*, 256>                   m_cpLookupCache = {};

        VltStagingDataAlloc m_staging;
	};



}  // namespace sce::vlt