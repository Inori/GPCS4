#include "VltFramebuffer.h"

#include "VltContext.h"
#include "VltImage.h"
#include "VltSampler.h"

namespace sce::vlt
{

	VltFramebufferInfo::VltFramebufferInfo()
	{
	}

	VltFramebufferInfo::VltFramebufferInfo(
		const VltRenderTargets&   renderTargets,
		const VltFramebufferSize& defaultSize) :
		m_renderTargets(renderTargets),
		m_renderSize(computeRenderSize(defaultSize))
	{

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (m_renderTargets.color[i].view != nullptr)
			{
				m_attachments[m_attachmentCount++] = i;
				m_sampleCount                      = m_renderTargets.color[i].view->imageInfo().sampleCount;
			}
		}

		if (m_renderTargets.depth.view != nullptr)
		{
			m_attachments[m_attachmentCount++] = -1;
			m_sampleCount                      = m_renderTargets.depth.view->imageInfo().sampleCount;
		}
	}

	VltFramebufferInfo::~VltFramebufferInfo()
	{
	}

	int32_t VltFramebufferInfo::findAttachment(const Rc<VltImageView>& view) const
	{
		for (uint32_t i = 0; i < m_attachmentCount; i++)
		{
			if (getAttachment(i).view->matchesView(view))
				return int32_t(i);
		}

		return -1;
	}

	bool VltFramebufferInfo::hasTargets(const VltRenderTargets& renderTargets)
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

	bool VltFramebufferInfo::isFullSize(const Rc<VltImageView>& view) const
	{
		return m_renderSize.width == view->mipLevelExtent(0).width &&
			   m_renderSize.height == view->mipLevelExtent(0).height &&
			   m_renderSize.layers == view->info().numLayers;
	}

	bool VltFramebufferInfo::isWritable(uint32_t attachmentIndex, VkImageAspectFlags aspects) const
	{
		VkImageAspectFlags writableAspects = 
			vutil::getWritableAspectsForLayout(getAttachment(attachmentIndex).layout);
		return (writableAspects & aspects) == aspects;
	}

	VltRtInfo VltFramebufferInfo::getRtInfo() const
	{
		VkFormat           depthStencilFormat          = VK_FORMAT_UNDEFINED;
		VkImageAspectFlags depthStencilReadOnlyAspects = 0;

		if (m_renderTargets.depth.view != nullptr)
		{
			depthStencilFormat          = m_renderTargets.depth.view->info().format;
			depthStencilReadOnlyAspects = m_renderTargets.depth.view->formatInfo()->aspectMask &
										  ~vutil::getWritableAspectsForLayout(m_renderTargets.depth.layout);
		}

		std::array<VkFormat, MaxNumRenderTargets> colorFormats = {};
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (m_renderTargets.color[i].view != nullptr)
				colorFormats[i] = m_renderTargets.color[i].view->info().format;
		}

		return VltRtInfo(MaxNumRenderTargets, colorFormats.data(),
						 depthStencilFormat, depthStencilReadOnlyAspects);
	}

	VltFramebufferSize VltFramebufferInfo::computeRenderSize(
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

	VltFramebufferSize VltFramebufferInfo::computeRenderTargetSize(
		const Rc<VltImageView>& renderTarget) const
	{
		auto extent = renderTarget->mipLevelExtent(0);
		auto layers = renderTarget->info().numLayers;
		return VltFramebufferSize{ extent.width, extent.height, layers };
	}
}  // namespace sce::vlt