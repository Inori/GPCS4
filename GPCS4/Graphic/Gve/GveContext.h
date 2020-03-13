#pragma once

#include "GveCommon.h"
#include "GvePipelineState.h"
#include "GveContextState.h"

#include <array>
#include <memory>

namespace gve
{;

struct GveAttachment;

class GveDevice;
class GveCmdList;
class GveShader;
class GveBuffer;
class GveBufferView;
class GveImage;
class GveImageView;
class GveSampler;
class GvePipelineManager;
class GveRenderPass;
class GveResourceObjects;
class GveDescriptorPool;
class GveStagingBufferAllocator;


struct GveShaderResourceSlot
{
	RcPtr<GveSampler>    sampler;
	RcPtr<GveImageView>  imageView;
	GveBufferSlice       buffer;
	RcPtr<GveBufferView> bufferView;
};

struct GvePipelineContext
{
	VkPipeline pipeline = VK_NULL_HANDLE;
	VkDescriptorSet descSet = VK_NULL_HANDLE;
};


// This is our render context.
// Just like GfxContext in PS4, one GveContex should be bound to one display buffer.

class GveContext : public RcObject
{
public:
	GveContext(const RcPtr<GveDevice>& device);
	~GveContext();

	void beginRecording(const RcPtr<GveCmdList>& commandBuffer);

	RcPtr<GveCmdList> endRecording();


	///

	void setViewport(const VkViewport& viewport, const VkRect2D& scissorRect);

	void setViewports(uint32_t viewportCount,
		const VkViewport* viewports, const VkRect2D* scissorRects);

	void setVertexInputState(const GveVertexInputInfo& viState);

	void setInputAssemblyState(const GveInputAssemblyInfo& iaState);

	void setRasterizerState(const GveRasterizationInfo& rsState);

	void setMultiSampleState(const GveMultisampleInfo& msState);

	void setDepthStencilState(const GveDepthStencilInfo& dsState);

	void setColorBlendState(const GveColorBlendInfo& blendCtl);

	void bindRenderTargets(const GveRenderTargets& renderTargets);

	void bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader);

	void bindIndexBuffer(const GveBufferSlice& buffer, VkIndexType indexType);

	void bindVertexBuffer(uint32_t binding, const GveBufferSlice& buffer, uint32_t stride);

	void bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler);

	void bindResourceBuffer(uint32_t regSlot, const GveBufferSlice& buffer);

	void bindResourceView(uint32_t regSlot, 
		const RcPtr<GveImageView>& imageView, 
		const RcPtr<GveBufferView>& bufferView);

	void draw(
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstance);

	void drawIndexed(
		uint32_t                indexCount,
		uint32_t                instanceCount,
		uint32_t                firstIndex,
		uint32_t                vertexOffset,
		uint32_t                firstInstance);

	/**
	 * \brief Clear render target
	 *
	 * Both depth and color target are supported.
	 * \param targetView Render target image view
	 * \param value Clear value
	 * \returns void
	 */
	void clearRenderTarget(
		const RcPtr<GveImageView>& targetView,
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
		const RcPtr<GveBuffer>& dstBuffer,
		VkDeviceSize            dstOffset,
		const RcPtr<GveBuffer>& srcBuffer,
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
		const RcPtr<GveImage>&   dstImage,
		VkImageSubresourceLayers dstSubresource,
		VkOffset3D               dstOffset,
		VkExtent3D               dstExtent,
		const RcPtr<GveBuffer>&  srcBuffer,
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
		const RcPtr<GveBuffer>& buffer,
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
		const RcPtr<GveImage>&          image,
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
		const RcPtr<GveBuffer>& buffer,
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
		const RcPtr<GveImage>&          image,
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
		const GveRenderTargets& renderTargets,
		GveRenderPassOps&       renderPassOps);

	VkDescriptorSet allocateDescriptorSet(VkDescriptorSetLayout layout);

	template <VkPipelineBindPoint BindPoint>
	void updateShaderResources(
		const GvePipelineLayout* pipelineLayout,
		VkDescriptorSet&         set);

	template <VkPipelineBindPoint BindPoint>
	void updateShaderDescriptorSetBinding(
		const GvePipelineLayout* layout,
		VkDescriptorSet          set);

	/// Graphics
	template <bool Indexed, bool Indirect>
	void commitGraphicsState();
	void updateGraphicsShaderResources();
	void updateGraphicsPipeline();
	void updateGraphicsPipelineStates();
	/// Compute
	void commitComputeState();
	void updateComputeDescriptorLayout();
	void updateComputePipeline();
	void updateComputePipelineStates();

	void enterRenderPassScope();
	void leaveRenderPassScope();

	void renderPassBindFramebuffer(const RcPtr<GveFrameBuffer>& framebuffer,
								   const GveRenderPassOps&      ops,
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
	RcPtr<GveDevice>    m_device;
	GveResourceObjects* m_objects;

	RcPtr<GveCmdList> m_cmd;

	RcPtr<GveDescriptorPool>         m_descPool;
	RcPtr<GveStagingBufferAllocator> m_staging;

	GveContextFlags m_flags;
	GveContextState m_state;

	GvePipelineContext m_gpCtx;
	GvePipelineContext m_cpCtx;

	std::array<GveShaderResourceSlot, pssl::PsslBindingIndexMax> m_res;

	
};



} // namespace gve
