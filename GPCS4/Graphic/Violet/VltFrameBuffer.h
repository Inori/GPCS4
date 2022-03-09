#pragma once

#include "VltCommon.h"
#include "VltLimit.h"
#include "VltRenderPass.h"
#include "VltImage.h"

namespace vlt
{;

class VltDevice;
class VltFrameBuffer;

/**
 * \brief Framebuffer size
 *
 * Stores the width, height and number of layers
 * of a framebuffer. This can be used in case a
 * framebuffer does not have any attachments.
 */
struct VltFramebufferSize
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
struct VltAttachment 
{
	RcPtr<VltImageView> view = nullptr;
	VkImageLayout       layout = VK_IMAGE_LAYOUT_UNDEFINED;
};


/**
 * \brief Render targets
 *
 * Stores all depth-stencil and color
 * attachments attached to a framebuffer.
 */
struct VltRenderTargets
{
	VltAttachment color[MaxNumRenderTargets];
	VltAttachment depth;
};


class VltFrameBuffer : public VltGpuResource
{
public:
	VltFrameBuffer(const RcPtr<VltDevice>&   device,
				   const VltRenderTargets&   renderTargets,
				   VltRenderPass*            renderPass,
				   const VltFramebufferSize& defaultSize);
	~VltFrameBuffer();

	/**
     * \brief Framebuffer handle
     * \returns VkFramebuffer handle
     */
	VkFramebuffer handle() const;

	/**
	 * \brief Get FrameBuffer size
	 * 
	 * \returns VltFramebufferSize
	 */
	const VltFramebufferSize& size() const;

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
	VltRenderPass* getRenderPass() const;

	/**
     * \brief Retrieves render pass handle
     * 
     * Retrieves a render pass handle that can
     * be used to begin a render pass instance.
     * \param [in] ops Render pass ops
     * \returns The render pass handle
     */
	VkRenderPass getRenderPassHandle(const VltRenderPassOps& ops) const;

	/**
     * \brief Depth-stencil target
     * \returns Depth-stencil target
     */
	const VltAttachment& getDepthTarget() const;

	/**
     * \brief Color target
     * 
     * \param [in] id Target Index
     * \returns The color target
     */
	const VltAttachment& getColorTarget(uint32_t id) const;

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
	const VltAttachment& getAttachment(uint32_t id) const;

	/**
     * \brief Finds attachment index by view
     * 
     * Color attachments start at 0
     * \param [in] view Image view
     * \returns Attachment index
     */
	int32_t findAttachment(const RcPtr<VltImageView>& view) const;

	/**
     * \brief Checks whether the framebuffer's targets match
     * 
     * \param [in] renderTargets Render targets to check
     * \returns \c true if the render targets are the same
     *          as the ones used for this framebuffer object.
     */
	bool matchTargets(const VltRenderTargets& renderTargets) const;


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
	bool isFullSize(const RcPtr<VltImageView>& view) const;


	static VltRenderPassFormat getRenderPassFormat(const VltRenderTargets& renderTargets);

private:
	bool createFrameBuffer(
		const VltFramebufferSize& defaultSize);

	VltFramebufferSize computeRenderSize(
		const VltFramebufferSize& defaultSize) const;

	VltFramebufferSize getRenderTargetSize(
		const RcPtr<VltImageView>& renderTarget) const;

	bool matchColors(const VltAttachment* color, uint32_t count) const;

	bool matchDepthStencil(const VltAttachment& depthStencil) const;

private:
	RcPtr<VltDevice> m_device;
	VltRenderTargets m_renderTargets;
	VltRenderPass*   m_renderPass;

	VkFramebuffer      m_frameBuffer = VK_NULL_HANDLE;
	VltFramebufferSize m_renderSize;

	uint32_t                                                  m_attachmentCount = 0;
	std::array<const VltAttachment*, MaxNumRenderTargets + 1> m_attachments;
};



} // namespace vlt
