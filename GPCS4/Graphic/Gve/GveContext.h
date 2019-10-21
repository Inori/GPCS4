#pragma once

#include "GveCommon.h"
#include "GveRenderState.h"

namespace gve
{;

class GveDevice;
class GveCommandBuffer;
class GveFrameBuffer;
class GveShader;
class GveBuffer;
class GveImage;
class GveImageView;
class GveSampler;

// This is our render context.
// Just like GfxContext in PS4, one GveContex should be bound to one display buffer.

class GveContex : public RcObject
{
public:
	GveContex(const RcPtr<GveDevice>& device);
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

	void bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader);

	void bindIndexBuffer(const GveBuffer& buffer, VkIndexType indexType);

	void bindVertexBuffer(uint32_t binding, const GveBuffer& buffer, uint32_t stride);

	void bindSampler(uint32_t startRegister, const RcPtr<GveSampler>& sampler);

	void bindImage(uint32_t startRegister, const RcPtr<GveImageView>& image);



private:
	RcPtr<GveDevice> m_device;
	RcPtr<GveCommandBuffer> m_cmd;

	GveRenderState m_state;
	GveContextFlag m_flag;
};


} // namespace gve
