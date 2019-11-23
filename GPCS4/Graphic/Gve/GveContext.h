#pragma once

#include "GveCommon.h"
#include "GvePipelineState.h"
#include "GveGraphicsPipeline.h"
#include "GveFrameBuffer.h"
#include "../Pssl/PsslBindingCalculator.h"

#include <array>

namespace gve
{;

class GveDevice;
class GveCmdList;
class GveShader;
class GveBuffer;
class GveBufferView;
class GveImage;
class GveImageView;
class GveSampler;
class GvePipelineManager;
class GveResourceManager;
class GveRenderPass;


/**
 * \brief Graphics pipeline state flags
 *
 * Stores some information on which state
 * of the graphics and compute pipelines
 * has changed and/or needs to be updated.
 */
enum class GveContextFlag : uint32_t
{
	GpRenderPassBound,          ///< Render pass is currently bound
	GpCondActive,               ///< Conditional rendering is enabled
	GpXfbActive,                ///< Transform feedback is enabled
	GpClearRenderTargets,       ///< Render targets need to be cleared
	GpDirtyFramebuffer,         ///< Framebuffer binding is out of date
	GpDirtyPipeline,            ///< Graphics pipeline binding is out of date
	GpDirtyPipelineState,       ///< Graphics pipeline needs to be recompiled
	GpDirtyResources,           ///< Graphics pipeline resource bindings are out of date
	GpDirtyDescriptorBinding,   ///< Graphics descriptor set needs to be rebound
	GpDirtyVertexBuffers,       ///< Vertex buffer bindings are out of date
	GpDirtyIndexBuffer,         ///< Index buffer binding are out of date
	GpDirtyXfbBuffers,          ///< Transform feedback buffer bindings are out of date
	GpDirtyXfbCounters,         ///< Counter buffer values are dirty
	GpDirtyBlendConstants,      ///< Blend constants have changed
	GpDirtyDepthBias,           ///< Depth bias has changed
	GpDirtyDepthBounds,         ///< Depth bounds have changed
	GpDirtyStencilRef,          ///< Stencil reference has changed
	GpDirtyViewport,            ///< Viewport state has changed
	GpDirtyPredicate,           ///< Predicate has changed
	GpDynamicBlendConstants,    ///< Blend constants are dynamic
	GpDynamicDepthBias,         ///< Depth bias is dynamic
	GpDynamicDepthBounds,       ///< Depth bounds are dynamic
	GpDynamicStencilRef,        ///< Stencil reference is dynamic

	CpDirtyPipeline,            ///< Compute pipeline binding are out of date
	CpDirtyPipelineState,       ///< Compute pipeline needs to be recompiled
	CpDirtyResources,           ///< Compute pipeline resource bindings are out of date
	CpDirtyDescriptorBinding,   ///< Compute descriptor set needs to be rebound

	DirtyDrawBuffer,            ///< Indirect argument buffer is dirty
	DirtyPushConstants,         ///< Push constant data has changed
};

using GveContextFlags = Flags<GveContextFlag>;


struct GveShaderResourceSlot
{
	RcPtr<GveSampler> sampler;
	RcPtr<GveImageView> imageView;
	RcPtr<GveBuffer> buffer;
	RcPtr<GveBufferView> bufferView;
};


struct GveGraphicsPipelineState
{
	GveGraphicsPipelineShaders   shader;
	GveGraphicsPipelineStateInfo state;
	GveGraphicsPipeline*         pipeline = nullptr;
};

struct GveComputePipelineState
{
	GveComputePipelineStateInfo state;
};



struct GveContextState
{
	GveGraphicsPipelineState gp;
	GveComputePipelineState cp;
};

// This is our render context.
// Just like GfxContext in PS4, one GveContex should be bound to one display buffer.

class GveContex : public RcObject
{
public:
	GveContex(const RcPtr<GveDevice>& device);
	~GveContex();

	void beginRecording(const RcPtr<GveCmdList>& commandBuffer);

	void endRecording();

	void setViewport(const VkViewport& viewport, const VkRect2D& scissorRect);

	void setViewports(uint32_t viewportCount,
		const VkViewport* viewports, const VkRect2D* scissorRects);

	void setInputLayout(
		uint32_t								 attributeCount,
		const VkVertexInputAttributeDescription* attributes,
		uint32_t								 bindingCount,
		const VkVertexInputBindingDescription*   bindings);

	void setPrimitiveType(VkPrimitiveTopology topology);

	//void setRasterizerState(const GveRasterizeInfo& state);

	//void setMultiSampleState(const GveMultisampleState& state);

	//void setBlendControl(const GveBlendControl& blendCtl);

	void bindRenderTargets(const GveRenderTargets& target);

	void bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader);

	void bindIndexBuffer(const RcPtr<GveBuffer>& buffer, VkIndexType indexType);

	void bindVertexBuffer(uint32_t binding, const RcPtr<GveBuffer>& buffer, uint32_t stride);

	void bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler);

	void bindResourceBuffer(uint32_t regSlot, const RcPtr<GveBuffer>& buffer);

	void bindResourceView(uint32_t regSlot, 
		const RcPtr<GveImageView>& imageView, 
		const RcPtr<GveBufferView>& bufferView);

	void drawIndex(uint32_t indexCount, uint32_t firstIndex);

	void copyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size);

	void copyBufferToImage(VkBuffer buffer, VkImage image, 
		uint32_t width, uint32_t height);

	void updateBuffer(const RcPtr<GveBuffer>& buffer, 
		VkDeviceSize offset, VkDeviceSize size, const void* data);

	void updateImage(const RcPtr<GveImage>& buffer,
		VkDeviceSize offset, VkDeviceSize size, const void* data);

	void transitionImageLayout(VkImage image, VkFormat format, 
		VkImageLayout oldLayout, VkImageLayout newLayout);

private:
	RcPtr<GveDevice> m_device;

	RcPtr<GveCmdList> m_cmd;

	std::array<GveShaderResourceSlot, pssl::PsslBindingIndexMax> m_res;
};


} // namespace gve
