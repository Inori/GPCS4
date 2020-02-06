#include "GveFrameBuffer.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GveFrameBuffer);

namespace gve
{;

GveFrameBuffer::GveFrameBuffer(const RcPtr<GveDevice>& device, 
	const GveRenderTargets& renderTargets, 
	GveRenderPass* renderPass, 
	const GveFramebufferSize& defaultSize):
	m_device(device),
	m_renderTargets(renderTargets),
	m_renderPass(renderPass)
{
	bool ret = createFrameBuffer(renderTargets, renderPass, defaultSize);
	LOG_ASSERT(ret == true, "create framebuffer failed.");
}

GveFrameBuffer::~GveFrameBuffer()
{
	vkDestroyFramebuffer(*m_device, m_frameBuffer, nullptr);
}

VkFramebuffer GveFrameBuffer::handle() const
{
	return m_frameBuffer;
}

GveRenderPass* GveFrameBuffer::getRenderPass() const
{
	return m_renderPass;
}

VkRenderPass GveFrameBuffer::getDefaultRenderPassHandle() const
{
	return m_renderPass->getDefaultHandle();
}

VkRenderPass GveFrameBuffer::getRenderPassHandle(const GveRenderPassOps& ops) const
{
	return m_renderPass->getHandle(ops);
}

VkExtent2D GveFrameBuffer::getRenderExtent() const
{
	VkExtent2D extent = {};
	extent.width = m_renderSize.width;
	extent.height = m_renderSize.height;
	return extent;
}

bool GveFrameBuffer::matchColorTargets(const GveAttachment* color, uint32_t count)
{
	bool match = true;
	do 
	{
		if (!color || !count)
		{
			break;
		}

		for (uint32_t i = 0; i != count; ++i)
		{
			if (m_renderTargets.color[i].view != color[i].view ||
				m_renderTargets.color[i].layout != color[i].layout)
			{
				match = false;
				break;
			}
		}

	} while (false);
	return match;
}

bool GveFrameBuffer::matchDepthTarget(const GveAttachment& depth)
{
	return (m_renderTargets.depth.view == depth.view) && 
		   (m_renderTargets.depth.layout == depth.layout);
}

bool GveFrameBuffer::matchRenderTargets(const GveRenderTargets& renderTargets)
{
	return matchColorTargets(renderTargets.color, MaxNumRenderTargets) &&
		   matchDepthTarget(renderTargets.depth);
}

GveRenderPassFormat GveFrameBuffer::getRenderPassFormat(const GveRenderTargets& renderTargets)
{
	GveRenderPassFormat format = {};

	for (uint32_t i = 0; i < MaxNumRenderTargets; i++) 
	{
		if (renderTargets.color[i].view == nullptr) 
		{
			continue;
		}

		format.sampleCount = renderTargets.color[i].view->imageInfo().sampleCount;
		format.color[i].format = renderTargets.color[i].view->info().format;
		format.color[i].layout = renderTargets.color[i].layout;
	}

	if (renderTargets.depth.view != nullptr) 
	{
		format.sampleCount = renderTargets.depth.view->imageInfo().sampleCount;
		format.depth.format = renderTargets.depth.view->info().format;
		format.depth.layout = renderTargets.depth.layout;
	}

	return format;
}

bool GveFrameBuffer::createFrameBuffer(const GveRenderTargets& renderTargets, 
	GveRenderPass* renderPass, 
	const GveFramebufferSize& defaultSize)
{
	bool ret = false;
	do
	{
		if (!renderPass)
		{
			break;
		}

		m_renderSize = computeRenderSize(defaultSize);

		std::array<VkImageView, MaxNumRenderTargets + 1> attachmentViews;
		
		for (uint32_t i = 0; i != MaxNumRenderTargets; ++i)
		{
			if (renderTargets.color[i].view == nullptr)
			{
				continue;
			}

			attachmentViews[m_attachmentCount] = renderTargets.color[i].view->handle();
			m_attachments[m_attachmentCount] = &renderTargets.color[i];
			++m_attachmentCount;
		}

		if (renderTargets.depth.view != nullptr)
		{
			attachmentViews[m_attachmentCount] = renderTargets.depth.view->handle();
			m_attachments[m_attachmentCount] = &renderTargets.depth;
			++m_attachmentCount;
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_renderPass->getDefaultHandle();
		framebufferInfo.pAttachments = attachmentViews.data();
		framebufferInfo.attachmentCount = m_attachmentCount;
		framebufferInfo.width = m_renderSize.width;
		framebufferInfo.height = m_renderSize.height;
		framebufferInfo.layers = m_renderSize.layers;

		if (vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &m_frameBuffer) != VK_SUCCESS)
		{
			LOG_ERR("failed to create framebuffer!");
			break;
		}
		ret  = true;
	}while(false);
	return ret;
}

GveFramebufferSize GveFrameBuffer::computeRenderSize(const GveFramebufferSize& defaultSize) const
{
	// Some games bind render targets of a different size and
	// expect it to work, so we'll compute the minimum size
	GveFramebufferSize minSize = defaultSize;

	if (m_renderTargets.depth.view != nullptr) 
	{
		GveFramebufferSize depthSize = getRenderTargetSize(m_renderTargets.depth.view);
		minSize.width = std::min(minSize.width, depthSize.width);
		minSize.height = std::min(minSize.height, depthSize.height);
		minSize.layers = std::min(minSize.layers, depthSize.layers);
	}

	for (uint32_t i = 0; i < MaxNumRenderTargets; i++) 
	{
		if (m_renderTargets.color[i].view == nullptr) 
		{
			continue;
		}

		GveFramebufferSize colorSize = getRenderTargetSize(m_renderTargets.color[i].view);
		minSize.width = std::min(minSize.width, colorSize.width);
		minSize.height = std::min(minSize.height, colorSize.height);
		minSize.layers = std::min(minSize.layers, colorSize.layers);
	}

	return minSize;
}

GveFramebufferSize GveFrameBuffer::getRenderTargetSize(const RcPtr<GveImageView>& renderTarget) const
{
	auto extent = renderTarget->mipLevelExtent(0);
	auto layers = renderTarget->imageInfo().numLayers;
	return GveFramebufferSize{ extent.width, extent.height, layers };
}

} // namespace gve