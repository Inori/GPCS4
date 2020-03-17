#include "VltFrameBuffer.h"

#include "VltDevice.h"

LOG_CHANNEL(Graphic.Violet.VltFrameBuffer);

namespace vlt
{;

VltFrameBuffer::VltFrameBuffer(const RcPtr<VltDevice>&   device,
							   const VltRenderTargets&   renderTargets,
							   VltRenderPass*            renderPass,
							   const VltFramebufferSize& defaultSize) :
	m_device(device),
	m_renderTargets(renderTargets),
	m_renderPass(renderPass)
{
	bool ret = createFrameBuffer(defaultSize);
	LOG_ASSERT(ret == true, "create framebuffer failed.");
}

VltFrameBuffer::~VltFrameBuffer()
{
	vkDestroyFramebuffer(*m_device, m_frameBuffer, nullptr);
}

bool VltFrameBuffer::createFrameBuffer(const VltFramebufferSize& defaultSize)
{
	bool ret = false;
	do
	{
		if (!m_renderPass)
		{
			break;
		}

		m_renderSize = computeRenderSize(defaultSize);

		std::array<VkImageView, MaxNumRenderTargets + 1> attachmentViews;

		for (uint32_t i = 0; i != MaxNumRenderTargets; ++i)
		{
			if (m_renderTargets.color[i].view == nullptr)
			{
				continue;
			}

			attachmentViews[m_attachmentCount] = m_renderTargets.color[i].view->handle();
			m_attachments[m_attachmentCount]   = &m_renderTargets.color[i];
			++m_attachmentCount;
		}

		if (m_renderTargets.depth.view != nullptr)
		{
			attachmentViews[m_attachmentCount] = m_renderTargets.depth.view->handle();
			m_attachments[m_attachmentCount]   = &m_renderTargets.depth;
			++m_attachmentCount;
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass              = m_renderPass->getDefaultHandle();
		framebufferInfo.pAttachments            = attachmentViews.data();
		framebufferInfo.attachmentCount         = m_attachmentCount;
		framebufferInfo.width                   = m_renderSize.width;
		framebufferInfo.height                  = m_renderSize.height;
		framebufferInfo.layers                  = m_renderSize.layers;

		if (vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &m_frameBuffer) != VK_SUCCESS)
		{
			LOG_ERR("failed to create framebuffer!");
			break;
		}
		ret = true;
	} while (false);
	return ret;
}

VkFramebuffer VltFrameBuffer::handle() const
{
	return m_frameBuffer;
}

const VltFramebufferSize& VltFrameBuffer::size() const
{
	return m_renderSize;
}

VltRenderPass* VltFrameBuffer::getRenderPass() const
{
	return m_renderPass;
}

VkRenderPass VltFrameBuffer::getDefaultRenderPassHandle() const
{
	return m_renderPass->getDefaultHandle();
}

VkRenderPass VltFrameBuffer::getRenderPassHandle(const VltRenderPassOps& ops) const
{
	return m_renderPass->getHandle(ops);
}

uint32_t VltFrameBuffer::numAttachments() const
{
	return m_attachmentCount;
}

int32_t VltFrameBuffer::findAttachment(const RcPtr<VltImageView>& view) const
{
	int32_t attachmentIndex = -1;

	for (int32_t i = 0; i != m_attachmentCount; ++i)
	{
		if (m_attachments[i]->view != view)
		{
			continue;
		}

		attachmentIndex = i;
		break;
	}

	return attachmentIndex;
}

bool VltFrameBuffer::matchTargets(const VltRenderTargets& renderTargets) const
{
	return matchColors(renderTargets.color, MaxNumRenderTargets) &&
		   matchDepthStencil(renderTargets.depth);
}

bool VltFrameBuffer::matchColors(const VltAttachment* color, uint32_t count) const
{
	bool eq = true; 
	for (uint32_t i = 0; i < count; i++)
	{
		eq &=
			m_renderTargets.color[i].view == color[i].view &&
			m_renderTargets.color[i].layout == color[i].layout;
	}
	return eq;
}

bool VltFrameBuffer::matchDepthStencil(const VltAttachment& depthStencil) const
{
	bool eq =
		m_renderTargets.depth.view == depthStencil.view &&
		m_renderTargets.depth.layout == depthStencil.layout;
	return eq;
}

bool VltFrameBuffer::isFullSize(const RcPtr<VltImageView>& view) const
{
	return 
		m_renderSize.width == view->mipLevelExtent(0).width && 
		m_renderSize.height == view->mipLevelExtent(0).height && 
		m_renderSize.layers == view->info().numLayers;
}

const VltAttachment& VltFrameBuffer::getAttachment(uint32_t id) const
{
	return *m_attachments[id];
}

const VltAttachment& VltFrameBuffer::getColorTarget(uint32_t id) const
{
	return m_renderTargets.color[id];
}

const VltAttachment& VltFrameBuffer::getDepthTarget() const
{
	return m_renderTargets.depth;
}

VltRenderPassFormat VltFrameBuffer::getRenderPassFormat(const VltRenderTargets& renderTargets)
{
	VltRenderPassFormat format = {};

	for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
	{
		if (renderTargets.color[i].view == nullptr)
		{
			continue;
		}

		format.sampleCount     = renderTargets.color[i].view->imageInfo().sampleCount;
		format.color[i].format = renderTargets.color[i].view->info().format;
		format.color[i].layout = renderTargets.color[i].layout;
	}

	if (renderTargets.depth.view != nullptr)
	{
		format.sampleCount  = renderTargets.depth.view->imageInfo().sampleCount;
		format.depth.format = renderTargets.depth.view->info().format;
		format.depth.layout = renderTargets.depth.layout;
	}

	return format;
}

VltFramebufferSize VltFrameBuffer::computeRenderSize(const VltFramebufferSize& defaultSize) const
{
	// Some games bind render targets of a different size and
	// expect it to work, so we'll compute the minimum size
	VltFramebufferSize minSize = defaultSize;

	if (m_renderTargets.depth.view != nullptr)
	{
		VltFramebufferSize depthSize = getRenderTargetSize(m_renderTargets.depth.view);
		minSize.width                = std::min(minSize.width, depthSize.width);
		minSize.height               = std::min(minSize.height, depthSize.height);
		minSize.layers               = std::min(minSize.layers, depthSize.layers);
	}

	for (uint32_t i = 0; i < MaxNumRenderTargets; ++i)
	{
		if (m_renderTargets.color[i].view == nullptr)
		{
			continue;
		}

		VltFramebufferSize colorSize = getRenderTargetSize(m_renderTargets.color[i].view);
		minSize.width                = std::min(minSize.width, colorSize.width);
		minSize.height               = std::min(minSize.height, colorSize.height);
		minSize.layers               = std::min(minSize.layers, colorSize.layers);
	}

	return minSize;
}

VltFramebufferSize VltFrameBuffer::getRenderTargetSize(const RcPtr<VltImageView>& renderTarget) const
{
	auto extent = renderTarget->mipLevelExtent(0);
	auto layers = renderTarget->imageInfo().numLayers;
	return VltFramebufferSize{ extent.width, extent.height, layers };
}


}  // namespace vlt