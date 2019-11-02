#pragma once

#include "GveCommon.h"
#include "GveRenderState.h"
#include "GveGraphicsPipeline.h"
#include "GveFrameBuffer.h"
#include "../Pssl/PsslBindingCalculator.h"

#include <array>

namespace gve
{;

class GveDevice;
class GveCommandBuffer;
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


struct GveContextParam
{
	GvePipelineManager* pipeMgr = nullptr;
	RcPtr<GveRenderPass> renderPass;
};

// This is our render context.
// Just like GfxContext in PS4, one GveContex should be bound to one display buffer.

class GveContex : public RcObject
{
public:
	GveContex(const RcPtr<GveDevice>& device, const GveContextParam& param);
	~GveContex();

	void beginRecording(const RcPtr<GveCommandBuffer>& commandBuffer);

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

	void setRasterizerState(const GveRasterizerState& state);

	void setMultiSampleState(const GveMultisampleState& state);

	void setBlendControl(const GveBlendControl& blendCtl);

	void bindRenderTargets(const GveRenderTarget& target);

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
	GvePipelineManager* m_pipeMgr;
	GveResourceManager* m_resMgr;
	RcPtr<GveRenderPass> m_renderPass;

	RcPtr<GveCommandBuffer> m_cmd;

	GveGraphicsPipelineShaders m_shaders;
	GveRenderState m_state;
	GveContextFlag m_flag;

	GveRenderTarget m_renderTarget;

	std::array<GveShaderResourceSlot, pssl::PsslBindingIndexMax> m_res;

	static VkPipeline s_pipeline;
	static GvePipelineLayout* s_layout;
	static RcPtr<GveDescriptorPool> s_descPool;
	VkDescriptorSet m_descSet = VK_NULL_HANDLE;
};


} // namespace gve
