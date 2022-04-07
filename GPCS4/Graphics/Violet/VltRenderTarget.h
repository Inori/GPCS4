#pragma once

#include "VltCommon.h"
#include "VltLimit.h"

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
     * \brief Render targets
     * 
     * Stores all depth-stencil and color
     * attachments attached to a framebuffer.
     */
	struct VltRenderTargets
	{
		VltAttachment depth;
		VltAttachment color[MaxNumRenderTargets];
	};

}  // namespace sce::vlt