#pragma once

#include "VltCommon.h"
#include "VltLimit.h"

namespace sce::vlt
{
	/**
	 * \brief Color attachment transitions
	 *
	 * Stores the load/store ops and the initial
	 * and final layout of a single attachment.
	 */
	struct VltColorAttachmentOps
	{
		VkAttachmentLoadOp loadOp      = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		VkImageLayout      loadLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageLayout      storeLayout = VK_IMAGE_LAYOUT_GENERAL;
		VkClearColorValue  clearValue  = VkClearColorValue();
	};

	/**
	 * \brief Depth attachment transitions
	 *
	 * Stores the load/store ops and the initial and
	 * final layout of the depth-stencil attachment.
	 */
	struct VltDepthAttachmentOps
	{
		VkAttachmentLoadOp       loadOpD     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		VkAttachmentLoadOp       loadOpS     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		VkImageLayout            loadLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageLayout            storeLayout = VK_IMAGE_LAYOUT_GENERAL;
		VkClearDepthStencilValue clearValue  = VkClearDepthStencilValue();
	};

	/**
	 * \brief Render pass transitions
	 *
	 * Stores transitions for all depth and color attachments.
	 * This is used to select a specific render pass object
	 * from a group of render passes with the same format.
	 */
	struct VltRenderPassOps
	{
		VltDepthAttachmentOps depthOps;
		VltColorAttachmentOps colorOps[MaxNumRenderTargets];
	};

}  // namespace sce::vlt