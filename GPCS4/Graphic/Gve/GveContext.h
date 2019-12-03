#pragma once

#include "GveCommon.h"
#include "GvePipelineState.h"
#include "GveContextState.h"

#include <array>

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
class GveResourceManager;
class GveRenderPass;
class GveResourceObjects;
class GveDescriptorPool;


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

class GveContex : public RcObject
{
public:
	GveContex(const RcPtr<GveDevice>& device);
	~GveContex();

	void beginRecording(const RcPtr<GveCmdList>& commandBuffer);

	RcPtr<GveCmdList> endRecording();

	void setViewport(const VkViewport& viewport, const VkRect2D& scissorRect);

	void setViewports(uint32_t viewportCount,
		const VkViewport* viewports, const VkRect2D* scissorRects);

	// alias for vertex input state
	void setVertexInputLayout(const GveVertexInputInfo& viState);

	void setInputAssemblyState(const GveInputAssemblyInfo& iaState);

	void setRasterizerState(const GveRasterizationInfo& rsState);

	void setMultiSampleState(const GveMultisampleInfo& msState);

	void setDepthStencilState(const GveDepthStencilInfo& dsState);

	void setColorBlendState(const GveColorBlendInfo& blendCtl);

	void bindRenderTargets(const GveAttachment* color, uint32_t count);

	void bindDepthRenderTarget(const GveAttachment& depth);

	void bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader);

	void bindIndexBuffer(const GveBufferSlice& buffer, VkIndexType indexType);

	void bindVertexBuffer(uint32_t binding, const GveBufferSlice& buffer, uint32_t stride);

	void bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler);

	void bindResourceBuffer(uint32_t regSlot, const GveBufferSlice& buffer);

	void bindResourceView(uint32_t regSlot, 
		const RcPtr<GveImageView>& imageView, 
		const RcPtr<GveBufferView>& bufferView);

	void drawIndex(uint32_t indexCount, uint32_t firstIndex);

	void copyBuffer(GveBufferSlice& dstBuffer, GveBufferSlice& srcBuffer, VkDeviceSize size);

	void copyBufferToImage(
		RcPtr<GveImage>& dstImage,
		GveBufferSlice& srcBuffer,
		uint32_t width, uint32_t height);

	void updateBuffer(const RcPtr<GveBuffer>& buffer, 
		VkDeviceSize offset, VkDeviceSize size, const void* data);

	void updateImage(const RcPtr<GveImage>& buffer,
		VkDeviceSize offset, VkDeviceSize size, const void* data);

	void transitionImageLayout(VkImage image, VkFormat format, 
		VkImageLayout oldLayout, VkImageLayout newLayout);

private:

	void updateFrameBuffer();

	void beginRenderPass();
	
	void endRenderPass();

	void updateVertexBindings();

	void updateIndexBinding();

	template <VkPipelineBindPoint BindPoint>
	void updateShaderResources();

	template <VkPipelineBindPoint BindPoint>
	void updateDescriptorLayout(const GvePipelineLayout* layout, VkDescriptorSet set);
	void updateGraphicsDescriptorLayout();
	void updateComputeDescriptorLayout();

	void updateGraphicsPipeline();
	void updateGraphicsPipelineStates();
	
	void updateComputePipeline();
	void updateComputePipelineStates();

	void commitGraphicsState();

	void commitComputeState();

	void setupRenderPassOps();

private:
	RcPtr<GveDevice> m_device;
	GveResourceObjects* m_objects;

	RcPtr<GveCmdList> m_cmd;

	RcPtr<GveDescriptorPool> m_descPool;

	GveContextFlags m_flags;
	GveContextState m_state;

	GvePipelineContext m_gpCtx;
	GvePipelineContext m_cpCtx;

	

	VkPipeline m_activePipeline = VK_NULL_HANDLE;

	std::array<GveShaderResourceSlot, pssl::PsslBindingIndexMax> m_res;

	
};



} // namespace gve
