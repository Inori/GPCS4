#include "GveFrameBuffer.h"

namespace gve
{;

GveFrameBuffer::GveFrameBuffer(const RcPtr<GveDevice>& device, VkRenderPass renderPass,
	VkImageView imageView, VkExtent2D& extent):
	m_device(device),
	m_renderPass(renderPass)
{
	bool ret = createFrameBuffer(imageView, extent);
	LOG_ASSERT(ret == true, "FrameBuffer init failed.");
}

GveFrameBuffer::~GveFrameBuffer()
{
	vkDestroyFramebuffer(*m_device, m_frameBuffer, nullptr);
}

VkFramebuffer GveFrameBuffer::handle() const
{
	return m_frameBuffer;
}

VkRenderPass GveFrameBuffer::renderPassHandle()
{

}

bool GveFrameBuffer::createFrameBuffer(VkImageView imageView, VkExtent2D& extent)
{
	bool ret = false;
	do 
	{
		VkImageView attachments[] = { imageView };

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &m_frameBuffer) != VK_SUCCESS)
		{
			LOG_ERR("failed to create framebuffer!");
			break;
		}
		ret = true;
	} while (false);
	return ret;
}

} // namespace gve