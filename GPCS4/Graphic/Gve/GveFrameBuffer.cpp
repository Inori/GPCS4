#include "GveFrameBuffer.h"
#include "GveDevice.h"

namespace gve
{;

GveFrameBuffer::GveFrameBuffer(const RcPtr<GveDevice>& device, VkRenderPass renderPass,
	VkImageView imageView, VkExtent2D& extent):
	m_device(device),
	m_renderPass(renderPass),
	m_extent(extent)
{
	bool ret = createFrameBuffer(imageView);
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

VkExtent2D GveFrameBuffer::extent() const
{
	return m_extent;
}

VkRenderPass GveFrameBuffer::renderPassHandle()
{

}

bool GveFrameBuffer::createFrameBuffer(VkImageView imageView)
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
		framebufferInfo.width = m_extent.width;
		framebufferInfo.height = m_extent.height;
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