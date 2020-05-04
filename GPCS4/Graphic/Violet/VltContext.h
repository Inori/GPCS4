#pragma once

#include "VltCommon.h"
#include "VltPipelineState.h"
#include "VltContextState.h"

#include <array>
#include <memory>

namespace vlt
{;

struct VltAttachment;

class VltDevice;
class VltCmdList;
class VltShader;
class VltBuffer;
class VltBufferView;
class VltImage;
class VltImageView;
class VltSampler;
class VltPipelineManager;
class VltRenderPass;
class VltResourceObjects;
class VltDescriptorPool;
class VltStagingBufferAllocator;


struct VltShaderResourceSlot
{
	RcPtr<VltSampler>    sampler;
	RcPtr<VltImageView>  imageView;
	VltBufferSlice       buffer;
	RcPtr<VltBufferView> bufferView;
};

struct VltPipelineContext
{
	VkPipeline      pipeline      = VK_NULL_HANDLE;
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};


// This is our render context.
// Just like GfxContext in PS4, one VltContex should be bound to one display buffer.

class VltContext : public RcObject
{
public:
	VltContext(const RcPtr<VltDevice>& device);
	~VltContext();

	void beginRecording(
		const RcPtr<VltCmdList>& commandBuffer);

	RcPtr<VltCmdList> endRecording();

	///< Pipeline state setting methods.

	void setViewports(
		uint32_t          viewportCount,
		const VkViewport* viewports,
		const VkRect2D*   scissorRects);

	void setInputLayout(
		uint32_t                  bindingCount,
		const VltVertexBinding*   bindings,
		uint32_t                  attributeCount,
		const VltVertexAttribute* attributes);

	void setInputAssemblyState(
		const VltInputAssemblyInfo& iaState);

	void setRasterizerState(
		const VltRasterizationInfo& rsState);

	void setMultiSampleState(
		const VltMultisampleInfo& msState);

	void setDepthStencilState(
		const VltDepthStencilInfo& dsState);

	void setDepthBounds(
		VltDepthBounds depthBounds);

	void setLogicOpState(
		const VltLogicOp& lo);

	void setBlendMode(
		uint32_t                       attachment,
		const VltColorBlendAttachment& blendMode);

	void setBlendMask(
		uint32_t                     attachment,
		const VkColorComponentFlags& colorMask);

	/// Resource binding methods.

	void bindRenderTargets(
		const VltRenderTargets& renderTargets);

	void bindShader(
		VkShaderStageFlagBits   stage,
		const RcPtr<VltShader>& shader);

	void bindIndexBuffer(
		const VltBufferSlice& buffer,
		VkIndexType           indexType);

	void bindVertexBuffer(
		uint32_t              binding,
		const VltBufferSlice& buffer,
		uint32_t              stride);

	void bindSampler(
		uint32_t                 regSlot,
		const RcPtr<VltSampler>& sampler);

	void bindResourceBuffer(
		uint32_t              regSlot,
		const VltBufferSlice& buffer);

	void bindResourceView(
		uint32_t                    regSlot,
		const RcPtr<VltImageView>&  imageView,
		const RcPtr<VltBufferView>& bufferView);

	///< Draw calls

	void draw(
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstance);

	void drawIndexed(
		uint32_t indexCount,
		uint32_t instanceCount,
		uint32_t firstIndex,
		uint32_t vertexOffset,
		uint32_t firstInstance);

	void dispatch(
		uint32_t x,
		uint32_t y,
		uint32_t z);

	///< Resource updating methods.

	/**
	 * \brief Clear render target
	 *
	 * Both depth and color target are supported.
	 * \param targetView Render target image view
	 * \param value Clear value
	 * \returns void
	 */
	void clearRenderTarget(
		const RcPtr<VltImageView>& targetView,
		VkImageAspectFlags         clearAspects,
		const VkClearValue&        clearValue);
	 
	/**
     * \brief Copies data from one buffer to another
     * 
     * \param [in] dstBuffer Destination buffer
     * \param [in] dstOffset Destination data offset
     * \param [in] srcBuffer Source buffer
     * \param [in] srcOffset Source data offset
     * \param [in] numBytes Number of bytes to copy
     */
	void copyBuffer(
		const RcPtr<VltBuffer>& dstBuffer,
		VkDeviceSize            dstOffset,
		const RcPtr<VltBuffer>& srcBuffer,
		VkDeviceSize            srcOffset,
		VkDeviceSize            numBytes);

	/**
     * \brief Copies data from a buffer to an image
     * 
     * \param [in] dstImage Destination image
     * \param [in] dstSubresource Destination subresource
     * \param [in] dstOffset Destination area offset
     * \param [in] dstExtent Destination area size
     * \param [in] srcBuffer Source buffer
     * \param [in] srcOffset Source offset, in bytes
     * \param [in] srcExtent Source data extent
     */
	void copyBufferToImage(
		const RcPtr<VltImage>&   dstImage,
		VkImageSubresourceLayers dstSubresource,
		VkOffset3D               dstOffset,
		VkExtent3D               dstExtent,
		const RcPtr<VltBuffer>&  srcBuffer,
		VkDeviceSize             srcOffset,
		VkExtent2D               srcExtent);


	/**
     * \brief Updates a buffer
     * 
     * Copies data from the host into a buffer.
     * \param [in] buffer Destination buffer
     * \param [in] offset Offset of sub range to update
     * \param [in] size Length of sub range to update
     * \param [in] data Data to upload
     */
	void updateBuffer(
		const RcPtr<VltBuffer>& buffer,
		VkDeviceSize            offset,
		VkDeviceSize            size,
		const void*             data);

	/**
     * \brief Updates an image
     * 
     * Copies data from the host into an image.
     * \param [in] image Destination image
     * \param [in] subsresources Image subresources to update
     * \param [in] imageOffset Offset of the image area to update
     * \param [in] imageExtent Size of the image area to update
     * \param [in] data Source data
     * \param [in] size Source data size
     */
	void updateImage(
		const RcPtr<VltImage>&          image,
		const VkImageSubresourceLayers& subresources,
		VkOffset3D                      imageOffset,
		VkExtent3D                      imageExtent,
		const void*                     data,
		VkDeviceSize                    pitchPerRow,
		VkDeviceSize                    pitchPerLayer);

	/**
     * \brief Uses transfer queue to initialize buffer
     * 
     * Only safe to use if the buffer is not in use by the GPU.
     * \param [in] buffer The buffer to initialize
     * \param [in] data The data to copy to the buffer
     */
	void uploadBuffer(
		const RcPtr<VltBuffer>& buffer,
		const void*             data);

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
		const RcPtr<VltImage>&          image,
		const VkImageSubresourceLayers& subresources,
		const void*                     data,
		VkDeviceSize                    pitchPerRow,
		VkDeviceSize                    pitchPerLayer);
    

private:

	void updateFrameBuffer();

	void updateVertexBindings();

	void updateIndexBinding();

	void updateDynamicState();

	void resetRenderPassOps(
		const VltRenderTargets& renderTargets,
		VltRenderPassOps&       renderPassOps);

	VkDescriptorSet allocateDescriptorSet(VkDescriptorSetLayout layout);

	template <VkPipelineBindPoint BindPoint>
	void updateShaderResources(
		const VltPipelineLayout* pipelineLayout);

	template <VkPipelineBindPoint BindPoint>
	void updateShaderDescriptorSetBinding(
		const VltPipelineLayout* layout,
		VkDescriptorSet          set);

	/// Graphics
	template <bool Indexed, bool Indirect>
	bool commitGraphicsState();
	void updateGraphicsShaderResources();
	bool updateGraphicsPipeline();
	bool updateGraphicsPipelineState();

	/// Compute
	bool commitComputeState();
	void updateComputeShaderResources();
	bool updateComputePipeline();
	bool updateComputePipelineState();


	/// 
	void enterRenderPassScope();
	void leaveRenderPassScope();

	void renderPassBindFramebuffer(const RcPtr<VltFrameBuffer>& framebuffer,
								   const VltRenderPassOps&      ops,
								   uint32_t                     clearValueCount,
								   const VkClearValue*          clearValues);
	void renderPassUnbindFramebuffer();

	/// Helpers
	void transitionImageLayout(
		VkImage              image,
		VkPipelineStageFlags srcStage,
		VkPipelineStageFlags dstStage,
		VkAccessFlags        srcAccess,
		VkAccessFlags        dstAccess,
		VkImageLayout        oldLayout,
		VkImageLayout        newLayout);

	// Should be only used for debugging purpose.
	void fullPipelineBarrier();

private:
	RcPtr<VltDevice>    m_device;
	VltResourceObjects* m_objects;

	RcPtr<VltCmdList> m_cmd;

	RcPtr<VltDescriptorPool>         m_descPool;
	RcPtr<VltStagingBufferAllocator> m_staging;

	VltContextFlags m_flags;
	VltContextState m_state = {};

	VltPipelineContext m_gpCtx;
	VltPipelineContext m_cpCtx;

	std::array<VltShaderResourceSlot, pssl::PsslBindingIndexMax> m_res;

	
};



} // namespace vlt
