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

	/**
	 * \brief Binds a shader to a given state
	 *
	 * \param [in] stage Target shader stage
	 * \param [in] shader The shader to bind
	 */
	void bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader);

	/**
	 * \brief Binds index buffer
	 *
	 * The index buffer will be used when
	 * issuing \c drawIndexed commands.
	 * \param [in] buffer New index buffer
	 * \param [in] indexType Index type
	 */
	void bindIndexBuffer(const GveBuffer& buffer, VkIndexType indexType);

	/**
	 * \brief Binds vertex buffer
	 *
	 * \param [in] binding Vertex buffer binding
	 * \param [in] buffer New vertex buffer
	 * \param [in] stride Stride between vertices
	 */
	void bindVertexBuffer(uint32_t binding, const GveBuffer& buffer, uint32_t stride);

	/**
	 * \brief Binds image sampler
	 *
	 * Binds a sampler that can be used together with
	 * an image in order to read from a texture.
	 * \param [in] slot Resource binding slot
	 * \param [in] sampler Sampler view to bind
	 */
	void bindSampler(uint32_t slot, const RcPtr<GveSampler>& sampler);


	void bindImage(uint32_t slot, const RcPtr<GveImage>& image);



private:
	RcPtr<GveDevice> m_device;
	RcPtr<GveCommandBuffer> m_cmd;

	GveRenderState m_state;
	GveContextFlag m_flag;
};


} // namespace gve
