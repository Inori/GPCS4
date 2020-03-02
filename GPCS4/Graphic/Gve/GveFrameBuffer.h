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
	GveFrameBuffer(const RcPtr<GveDevice>&   device,
				   const GveRenderTargets&   renderTargets,
				   GveRenderPass*            renderPass,
				   const GveFramebufferSize& defaultSize);
	~GveFrameBuffer();

	/**
     * \brief Framebuffer handle
     * \returns VkFramebuffer handle
     */
	VkFramebuffer handle() const;

	/**
	 * \brief Get FrameBuffer size
	 * 
	 * \returns GveFramebufferSize
	 */
	const GveFramebufferSize& size() const;

	/**
     * \brief Retrieves default render pass handle
     * 
     * Retrieves the render pass handle that was used
     * to create the Vulkan framebuffer object with,
     * and that should be used to create pipelines.
     * \returns The default render pass handle
     */
	VkRenderPass getDefaultRenderPassHandle() const;

	/**
     * \brief Retrieves render pass
     * \returns Render pass reference
     */
	GveRenderPass* getRenderPass() const;

	/**
     * \brief Retrieves render pass handle
     * 
     * Retrieves a render pass handle that can
     * be used to begin a render pass instance.
     * \param [in] ops Render pass ops
     * \returns The render pass handle
     */
	VkRenderPass getRenderPassHandle(const GveRenderPassOps& ops) const;

	/**
     * \brief Depth-stencil target
     * \returns Depth-stencil target
     */
	const GveAttachment& getDepthTarget() const;

	/**
     * \brief Color target
     * 
     * \param [in] id Target Index
     * \returns The color target
     */
	const GveAttachment& getColorTarget(uint32_t id) const;

	/**
     * \brief Number of framebuffer attachment
     * \returns Total attachment count
     */
	uint32_t numAttachments() const;

	/**
     * \brief Retrieves attachment by index
     * 
     * \param [in] id Framebuffer attachment ID
     * \returns The framebuffer attachment
     */
	const GveAttachment& getAttachment(uint32_t id) const;

	/**
     * \brief Finds attachment index by view
     * 
     * Color attachments start at 0
     * \param [in] view Image view
     * \returns Attachment index
     */
	int32_t findAttachment(const RcPtr<GveImageView>& view) const;

	/**
     * \brief Checks whether the framebuffer's targets match
     * 
     * \param [in] renderTargets Render targets to check
     * \returns \c true if the render targets are the same
     *          as the ones used for this framebuffer object.
     */
	bool matchTargets(const GveRenderTargets& renderTargets) const;


	/**
     * \brief Checks whether view and framebuffer sizes match
     *
     * Tests whether the size of the framebuffer is the same
     * as the size of one of its views. This may be \c false
     * when mixing attachments with mismatched dimensions.
     * \param [in] view Image view to test
     * \returns \c true if \c view has the same size as
     *          the framebuffer.
     */
	bool isFullSize(const RcPtr<GveImageView>& view) const;


	static GveRenderPassFormat getRenderPassFormat(const GveRenderTargets& renderTargets);

private:
	bool createFrameBuffer(
		const GveFramebufferSize& defaultSize);

	GveFramebufferSize computeRenderSize(
		const GveFramebufferSize& defaultSize) const;

	GveFramebufferSize getRenderTargetSize(
		const RcPtr<GveImageView>& renderTarget) const;

	bool matchColors(const GveAttachment* color, uint32_t count) const;

	bool matchDepthStencil(const GveAttachment& depthStencil) const;

private:
	RcPtr<GveDevice> m_device;
	GveRenderTargets m_renderTargets;
	GveRenderPass*   m_renderPass;

	VkFramebuffer      m_frameBuffer = VK_NULL_HANDLE;
	GveFramebufferSize m_renderSize;

	uint32_t                                                  m_attachmentCount = 0;
	std::array<const GveAttachment*, MaxNumRenderTargets + 1> m_attachments;
};



} // namespace gve
