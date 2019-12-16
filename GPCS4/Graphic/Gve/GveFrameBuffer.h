#pragma once

#include "GveCommon.h"
#include "GveLimit.h"
#include "GveRenderPass.h"
#include "GveImage.h"

namespace gve
{;

class GveDevice;
class GveFrameBuffer;

/**
 * \brief Framebuffer size
 *
 * Stores the width, height and number of layers
 * of a framebuffer. This can be used in case a
 * framebuffer does not have any attachments.
 */
struct GveFramebufferSize
{
	uint32_t width;
	uint32_t height;
	uint32_t layers;
};


/**
 * \brief Framebuffer attachment
 *
 * Stores an attachment, as well as the image layout
 * that will be used for rendering to the attachment.
 */
struct GveAttachment 
{
	RcPtr<GveImageView> view = nullptr;
	VkImageLayout       layout = VK_IMAGE_LAYOUT_UNDEFINED;
};


/**
 * \brief Render targets
 *
 * Stores all depth-stencil and color
 * attachments attached to a framebuffer.
 */
struct GveRenderTargets
{
	GveAttachment color[MaxNumRenderTargets];
	GveAttachment depth;
};


class GveFrameBuffer : public RcObject
{
public:
	GveFrameBuffer(const RcPtr<GveDevice>& device, 
		const GveRenderTargets& renderTargets,
		GveRenderPass* renderPass,
		const GveFramebufferSize& defaultSize);
	~GveFrameBuffer();

	VkFramebuffer handle() const;

	GveRenderPass* getRenderPass() const;

	VkRenderPass getDefaultRenderPassHandle() const;

	VkRenderPass getRenderPassHandle(const GveRenderPassOps& ops) const;

	VkExtent2D getRenderExtent() const;

	bool matchColorTargets(const GveAttachment* color, uint32_t count);

	bool matchDepthTarget(const GveAttachment& depth);

	bool matchRenderTargets(const GveRenderTargets& renderTargets);

	static GveRenderPassFormat getRenderPassFormat(const GveRenderTargets& renderTargets);

private:
	bool createFrameBuffer(const GveRenderTargets& renderTargets,
		GveRenderPass* renderPass,
		const GveFramebufferSize& defaultSize);

	GveFramebufferSize computeRenderSize(
		const GveFramebufferSize& defaultSize) const;

	GveFramebufferSize getRenderTargetSize(
		const RcPtr<GveImageView>& renderTarget) const;

private:
	RcPtr<GveDevice> m_device;
	GveRenderTargets m_renderTargets;
	GveRenderPass* m_renderPass;

	VkFramebuffer m_frameBuffer = VK_NULL_HANDLE;
	GveFramebufferSize m_renderSize;

	uint32_t                                                  m_attachmentCount = 0;
	std::array<const GveAttachment*, MaxNumRenderTargets + 1> m_attachments;
};



} // namespace gve
