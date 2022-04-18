#pragma once

#include "VltCommon.h"
#include "VltLimit.h"

#include <vector>

namespace sce::vlt
{
	class VltImageView;

	/**
     * \brief Rendering attachment
     * 
     * Stores an attachment, as well as the image layout
     * that will be used for rendering to the attachment.
     */
	struct VltAttachment
	{
		Rc<VltImageView> view   = nullptr;
		VkImageLayout    layout = VK_IMAGE_LAYOUT_UNDEFINED;
	};

	/**
	 * \brief Rendering attachment format
	 * 
	 * Stores all attachments' format used for
	 * creating pipelines.
	 */
	struct VltAttachmentFormat
	{
		VkFormat depth                      = VK_FORMAT_UNDEFINED;
		VkFormat color[MaxNumRenderTargets] = { VK_FORMAT_UNDEFINED };

		bool eq(const VltAttachmentFormat& other) const;

		uint32_t colorCount() const;
	};

	/**
     * \brief Render targets
     * 
     * Stores all depth-stencil and color
     * attachments attached to a framebuffer.
     */
	struct VltRenderTargets
	{
		VltAttachment depth;
		VltAttachment color[MaxNumRenderTargets];

		VltAttachmentFormat generateAttachmentFormat() const;
	};

}  // namespace sce::vlt