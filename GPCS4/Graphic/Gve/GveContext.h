#pragma once

#include "GveCommon.h"
#include "GvePipelineState.h"
#include "GveGraphicsPipeline.h"
#include "GveFrameBuffer.h"
#include "GveContextState.h"
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



struct GveShaderResourceSlot
{
	RcPtr<GveSampler> sampler;
	RcPtr<GveImageView> imageView;
	RcPtr<GveBuffer> buffer;
	RcPtr<GveBufferView> bufferView;
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

	void updateFrameBuffer();

	void beginRenderPass();
	
	void endRenderPass();

	void updateVertexInput();

	void updateIndexBuffer();

	void updateShaderResources();

	void updateDescriptorLayout();

	template <VkPipelineBindPoint BindPoint>
	void updatePipelineStates();

	template <VkPipelineBindPoint BindPoint>
	void updatePipeline();

	void commitGraphicsState();

	void commitComputeState();

private:
	RcPtr<GveDevice> m_device;
	RcPtr<GveCmdList> m_cmd;

	GveContextFlags m_flags;
	GveContextState m_state;

	std::array<GveShaderResourceSlot, pssl::PsslBindingIndexMax> m_res;
};



} // namespace gve
