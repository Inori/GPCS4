#include "VltFramebuffer.h"

#include "VltBarrier.h"
#include "VltImage.h"

namespace sce::vlt
{
	VltFramebuffer::VltFramebuffer(
		const VltRenderTargets&   renderTargets,
		const VltFramebufferSize& defaultSize) :
		m_renderTargets(renderTargets),
		m_renderSize(computeRenderSize(defaultSize))
	{
		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color                    = { { 0.0f, 0.0f, 0.0f, 0.0f } };
		clearValues[1].depthStencil             = { 0.0f, 0 };

		for (uint32_t i = 0; i != MaxNumRenderTargets; ++i)
		{
			auto& colorView = m_renderTargets.color[i].view;
			if (colorView != nullptr)
			{
				m_colorAttachments[m_colorAttachmentCount].sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
				m_colorAttachments[m_colorAttachmentCount].pNext              = nullptr;
				m_colorAttachments[m_colorAttachmentCount].imageView          = colorView->handle();
				m_colorAttachments[m_colorAttachmentCount].imageLayout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				m_colorAttachments[m_colorAttachmentCount].resolveMode        = VK_RESOLVE_MODE_NONE;
				m_colorAttachments[m_colorAttachmentCount].resolveImageView   = VK_NULL_HANDLE;
				m_colorAttachments[m_colorAttachmentCount].resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				m_colorAttachments[m_colorAttachmentCount].loadOp             = VK_ATTACHMENT_LOAD_OP_LOAD;
				m_colorAttachments[m_colorAttachmentCount].storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
				m_colorAttachments[m_colorAttachmentCount].clearValue         = clearValues[0];
				++m_colorAttachmentCount;
			}
		}

		auto& depthView = m_renderTargets.depth.view;
		if (depthView != nullptr)
		{
			m_depthAttachment.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			m_depthAttachment.pNext              = nullptr;
			m_depthAttachment.imageView          = depthView->handle();
			m_depthAttachment.imageLayout        = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			m_depthAttachment.resolveMode        = VK_RESOLVE_MODE_NONE;
			m_depthAttachment.resolveImageView   = VK_NULL_HANDLE;
			m_depthAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			m_depthAttachment.loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR;
			m_depthAttachment.storeOp            = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			m_depthAttachment.clearValue         = clearValues[1];
		}
	}

	VltFramebuffer::~VltFramebuffer()
	{
	}

	bool VltFramebuffer::matchTargets(const VltRenderTargets& renderTargets)
	{
		bool eq = m_renderTargets.depth.view == renderTargets.depth.view &&
				  m_renderTargets.depth.layout == renderTargets.depth.layout;

		for (uint32_t i = 0; i < MaxNumRenderTargets && eq; i++)
		{
			eq &= m_renderTargets.color[i].view == renderTargets.color[i].view &&
				  m_renderTargets.color[i].layout == renderTargets.color[i].layout;
		}

		return eq;
	}

	bool VltFramebuffer::isFullSize(const Rc<VltImageView>& view) const
	{
		return m_renderSize.width == view->mipLevelExtent(0).width &&
			   m_renderSize.height == view->mipLevelExtent(0).height &&
			   m_renderSize.layers == view->info().numLayers;
	}

	VltFramebufferSize VltFramebuffer::computeRenderSize(
		const VltFramebufferSize& defaultSize) const
	{
		// Some games bind render targets of a different size and
		// expect it to work, so we'll compute the minimum size
		VltFramebufferSize minSize = defaultSize;

		if (m_renderTargets.depth.view != nullptr)
		{
			VltFramebufferSize depthSize = this->computeRenderTargetSize(m_renderTargets.depth.view);
			minSize.width                = std::min(minSize.width, depthSize.width);
			minSize.height               = std::min(minSize.height, depthSize.height);
			minSize.layers               = std::min(minSize.layers, depthSize.layers);
		}

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (m_renderTargets.color[i].view != nullptr)
			{
				VltFramebufferSize colorSize = this->computeRenderTargetSize(m_renderTargets.color[i].view);
				minSize.width                = std::min(minSize.width, colorSize.width);
				minSize.height               = std::min(minSize.height, colorSize.height);
				minSize.layers               = std::min(minSize.layers, colorSize.layers);
			}
		}

		return minSize;
	}

	VltFramebufferSize VltFramebuffer::computeRenderTargetSize(
		const Rc<VltImageView>& renderTarget) const
	{
		auto extent = renderTarget->mipLevelExtent(0);
		auto layers = renderTarget->info().numLayers;
		return VltFramebufferSize{ extent.width, extent.height, layers };
	}

	void VltFramebuffer::prepareRenderingLayout(VltBarrierSet& barrier)
	{
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			auto& colorView = m_renderTargets.color[i].view;
			if (colorView != nullptr)
			{
				auto& colorImage = colorView->image();
				if (colorImage->info().layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
				{
					continue;
				}

				VkImageSubresourceRange subresources;
				subresources.aspectMask     = colorImage->formatInfo()->aspectMask;
				subresources.baseArrayLayer = 0;
				subresources.baseMipLevel   = 0;
				subresources.layerCount     = colorImage->info().numLayers;
				subresources.levelCount     = colorImage->info().mipLevels;

				barrier.accessImage(
					colorImage,
					subresources,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					0,
					VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
			}
		}

		auto& depthView = m_renderTargets.depth.view;
		if (depthView != nullptr)
		{
			auto& depthImage = depthView->image();
			if (depthImage->info().layout != VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
			{
				VkImageSubresourceRange subresources;
				subresources.aspectMask     = depthImage->formatInfo()->aspectMask;
				subresources.baseArrayLayer = 0;
				subresources.baseMipLevel   = 0;
				subresources.layerCount     = depthImage->info().numLayers;
				subresources.levelCount     = depthImage->info().mipLevels;

				barrier.accessImage(
					depthImage,
					subresources,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
					0,
					VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
			}
		}
	}

	int32_t VltFramebuffer::findAttachment(
		const Rc<VltImageView>& view) const
	{
		for (uint32_t i = 0; i < m_colorAttachmentCount; i++)
		{
			if (m_colorAttachments[i].imageView == view->handle())
				return int32_t(i);
		}

		if (m_depthAttachment.imageView == view->handle())
		{
			return m_colorAttachmentCount;
		}

		return -1;
	}
	
	const sce::vlt::VltAttachment& VltFramebuffer::getAttachment(uint32_t id) const
	{
		if (id == m_colorAttachmentCount)
		{
			return m_renderTargets.depth;
		}
		else
		{
			return m_renderTargets.color[id];
		}
	}

	void VltFramebuffer::setAttachmentClearValues(const VltFrameBufferClearValues& values)
	{
		m_depthAttachment.clearValue.depthStencil = values.depthValue.depthStencil;

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			m_colorAttachments[i].clearValue.color = values.colorValue[i].color;
		}
	}

	void VltFramebuffer::setAttachmentOps(const VltFrameBufferOps& ops)
	{
		m_depthAttachment.loadOp  = ops.depthOps.loadOp;
		m_depthAttachment.storeOp = ops.depthOps.storeOp;

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			m_colorAttachments[i].loadOp  = ops.colorOps[i].loadOp;
			m_colorAttachments[i].storeOp = ops.colorOps[i].storeOp;
		}
	}

}  // namespace sce::vlt