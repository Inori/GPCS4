#include "VltBarrier.h"

#include "VltBuffer.h"
#include "VltImage.h"
#include "VltUtil.h"

namespace sce::vlt
{

	VltBarrierSet::VltBarrierSet(VltCmdType cmdBuffer) :
		m_cmdBuffer(cmdBuffer)
	{
	}

	VltBarrierSet::~VltBarrierSet()
	{
	}

	void VltBarrierSet::accessMemory(
		VkPipelineStageFlags2 srcStages,
		VkAccessFlags2        srcAccess,
		VkPipelineStageFlags2 dstStages,
		VkAccessFlags2        dstAccess)
	{
		// should we use a list of VkMemoryBarrier2 like for buffer and image,
		// or a single one?
		m_memBarrier.srcStageMask |= srcStages;
		m_memBarrier.dstStageMask |= dstStages;

		m_memBarrier.srcAccessMask |= srcAccess;
		m_memBarrier.dstAccessMask |= dstAccess;
	}

	void VltBarrierSet::accessBuffer(
		const VltBufferSliceHandle& bufSlice,
		VkPipelineStageFlags2       srcStages,
		VkAccessFlags2              srcAccess,
		VkPipelineStageFlags2       dstStages,
		VkAccessFlags2              dstAccess)
	{
		VltAccessFlags access = vutil::getAccessTypes(srcAccess);

		if (srcStages == VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT ||
			dstStages == VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT)
			access.set(VltAccess::Write);

		m_memBarrier.srcStageMask |= srcStages;
		m_memBarrier.dstStageMask |= dstStages;

		m_memBarrier.srcAccessMask |= srcAccess;
		m_memBarrier.dstAccessMask |= dstAccess;

		m_bufSlices.push_back({ bufSlice, access });
	}

	void VltBarrierSet::accessImage(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& subresources,
		VkImageLayout                  srcLayout,
		VkPipelineStageFlags2          srcStages,
		VkAccessFlags2                 srcAccess,
		VkImageLayout                  dstLayout,
		VkPipelineStageFlags2          dstStages,
		VkAccessFlags2                 dstAccess)
	{
		VltAccessFlags access = vutil::getAccessTypes(srcAccess);

		if (srcStages == VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT ||
			dstStages == VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT ||
			srcLayout != dstLayout)
			access.set(VltAccess::Write);

		m_memBarrier.srcStageMask |= srcStages;
		m_memBarrier.dstStageMask |= dstStages;

		if (srcLayout == dstLayout)
		{
			m_memBarrier.srcAccessMask |= srcAccess;
			m_memBarrier.dstAccessMask |= dstAccess;
		}
		else
		{
			VkImageMemoryBarrier2 barrier;
			barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
			barrier.pNext                       = nullptr;
			barrier.srcStageMask                = srcStages;
			barrier.srcAccessMask               = srcAccess;
			barrier.dstStageMask                = dstStages;
			barrier.dstAccessMask               = dstAccess;
			barrier.oldLayout                   = srcLayout;
			barrier.newLayout                   = dstLayout;
			barrier.srcQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
			barrier.image                       = image->handle();
			barrier.subresourceRange            = subresources;
			barrier.subresourceRange.aspectMask = image->formatInfo()->aspectMask;
			m_imgBarriers.push_back(barrier);
		}

		m_imgSlices.push_back({ image.ptr(), subresources, access });
	}

	void VltBarrierSet::releaseBuffer(
		VltBarrierSet&              acquire,
		const VltBufferSliceHandle& bufSlice,
		uint32_t                    srcQueue,
		VkPipelineStageFlags2       srcStages,
		VkAccessFlags2              srcAccess,
		uint32_t                    dstQueue,
		VkPipelineStageFlags2       dstStages,
		VkAccessFlags2              dstAccess)
	{
		auto& release = *this;

		release.m_memBarrier.srcStageMask |= srcStages;
		acquire.m_memBarrier.dstStageMask |= dstStages;

		VkBufferMemoryBarrier2 barrier;
		barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
		barrier.pNext               = nullptr;
		barrier.srcStageMask        = srcStages;
		barrier.srcAccessMask       = srcAccess;
		barrier.dstStageMask        = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
		barrier.dstAccessMask       = 0;
		barrier.srcQueueFamilyIndex = srcQueue;
		barrier.dstQueueFamilyIndex = dstQueue;
		barrier.buffer              = bufSlice.handle;
		barrier.offset              = bufSlice.offset;
		barrier.size                = bufSlice.length;
		release.m_bufBarriers.push_back(barrier);

		barrier.srcStageMask  = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
		barrier.srcAccessMask = 0;
		barrier.dstStageMask  = dstStages;
		barrier.dstAccessMask = dstAccess;
		acquire.m_bufBarriers.push_back(barrier);

		VltAccessFlags access(VltAccess::Read, VltAccess::Write);
		release.m_bufSlices.push_back({ bufSlice, access });
		acquire.m_bufSlices.push_back({ bufSlice, access });
	}

	void VltBarrierSet::releaseImage(
		VltBarrierSet&                 acquire,
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& subresources,
		uint32_t                       srcQueue,
		VkImageLayout                  srcLayout,
		VkPipelineStageFlags2          srcStages,
		VkAccessFlags2                 srcAccess,
		uint32_t                       dstQueue,
		VkImageLayout                  dstLayout,
		VkPipelineStageFlags2          dstStages,
		VkAccessFlags2                 dstAccess)
	{
		auto& release = *this;

		release.m_memBarrier.srcStageMask |= srcStages;
		acquire.m_memBarrier.dstStageMask |= dstStages;

		VkImageMemoryBarrier2 barrier;
		barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
		barrier.pNext                       = nullptr;
		barrier.srcStageMask                = srcStages;
		barrier.srcAccessMask               = srcAccess;
		barrier.dstStageMask                = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
		barrier.dstAccessMask               = 0;
		barrier.oldLayout                   = srcLayout;
		barrier.newLayout                   = dstLayout;
		barrier.srcQueueFamilyIndex         = srcQueue;
		barrier.dstQueueFamilyIndex         = dstQueue;
		barrier.image                       = image->handle();
		barrier.subresourceRange            = subresources;
		barrier.subresourceRange.aspectMask = image->formatInfo()->aspectMask;
		release.m_imgBarriers.push_back(barrier);

		if (srcQueue == dstQueue)
			barrier.oldLayout = dstLayout;

		barrier.srcStageMask  = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
		barrier.srcAccessMask = 0;
		barrier.dstStageMask  = dstStages;
		barrier.dstAccessMask = dstAccess;
		acquire.m_imgBarriers.push_back(barrier);

		VltAccessFlags access(VltAccess::Read, VltAccess::Write);
		release.m_imgSlices.push_back({ image.ptr(), subresources, access });
		acquire.m_imgSlices.push_back({ image.ptr(), subresources, access });
	}

	bool VltBarrierSet::isBufferDirty(
		const VltBufferSliceHandle& bufSlice,
		VltAccessFlags              bufAccess)
	{
		bool result = false;

		for (uint32_t i = 0; i < m_bufSlices.size() && !result; i++)
		{
			const VltBufferSliceHandle& dstSlice = m_bufSlices[i].slice;

			result = (bufSlice.handle == dstSlice.handle) &&
					 (bufAccess | m_bufSlices[i].access).test(VltAccess::Write) &&
					 isBufferOverlapped(bufSlice, dstSlice);
		}

		return result;
	}

	bool VltBarrierSet::isImageDirty(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& imgSubres,
		VltAccessFlags                 imgAccess)
	{
		bool result = false;

		for (uint32_t i = 0; i < m_imgSlices.size() && !result; i++)
		{
			const VkImageSubresourceRange& dstSubres = m_imgSlices[i].subres;

			result = (image == m_imgSlices[i].image) &&
					 (imgAccess | m_imgSlices[i].access).test(VltAccess::Write) &&
					 isImageOverlapped(imgSubres, dstSubres);
		}

		return result;
	}

	VltAccessFlags VltBarrierSet::getBufferAccess(
		const VltBufferSliceHandle& bufSlice)
	{
		VltAccessFlags access;

		for (uint32_t i = 0; i < m_bufSlices.size(); i++)
		{
			const VltBufferSliceHandle& dstSlice = m_bufSlices[i].slice;

			if ((bufSlice.handle == dstSlice.handle) && (bufSlice.offset + bufSlice.length > dstSlice.offset) && (bufSlice.offset < dstSlice.offset + dstSlice.length))
				access = access | m_bufSlices[i].access;
		}

		return access;
	}

	VltAccessFlags VltBarrierSet::getImageAccess(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& imgSubres)
	{
		VltAccessFlags access;

		for (uint32_t i = 0; i < m_imgSlices.size(); i++)
		{
			const VkImageSubresourceRange& dstSubres = m_imgSlices[i].subres;

			if ((image == m_imgSlices[i].image) &&
				(imgSubres.baseArrayLayer < dstSubres.baseArrayLayer + dstSubres.layerCount) &&
				(imgSubres.baseArrayLayer + imgSubres.layerCount > dstSubres.baseArrayLayer) &&
				(imgSubres.baseMipLevel < dstSubres.baseMipLevel + dstSubres.levelCount) &&
				(imgSubres.baseMipLevel + imgSubres.levelCount > dstSubres.baseMipLevel))
				access = access | m_imgSlices[i].access;
		}

		return access;
	}

	void VltBarrierSet::recordCommands(const Rc<VltCommandList>& commandList)
	{
		if (m_memBarrier.srcStageMask | m_memBarrier.dstStageMask)
		{
			if (!m_memBarrier.srcStageMask)
				m_memBarrier.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
			if (!m_memBarrier.dstStageMask)
				m_memBarrier.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;

			VkDependencyInfo dependency;
			dependency.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
			dependency.pNext                    = nullptr;
			dependency.dependencyFlags          = 0;
			dependency.memoryBarrierCount       = 1;
			dependency.pMemoryBarriers          = &m_memBarrier;
			dependency.bufferMemoryBarrierCount = m_bufBarriers.size();
			dependency.pBufferMemoryBarriers    = m_bufBarriers.data();
			dependency.imageMemoryBarrierCount  = m_imgBarriers.size();
			dependency.pImageMemoryBarriers     = m_imgBarriers.data();

			commandList->cmdPipelineBarrier2(m_cmdBuffer, &dependency);

			this->reset();
		}
	}

	void VltBarrierSet::reset()
	{
		m_memBarrier = { VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
						 nullptr,
						 VK_PIPELINE_STAGE_2_NONE,
						 VK_ACCESS_2_NONE,
						 VK_PIPELINE_STAGE_2_NONE,
						 VK_ACCESS_2_NONE };

		m_bufBarriers.resize(0);
		m_imgBarriers.resize(0);

		m_bufSlices.resize(0);
		m_imgSlices.resize(0);
	}

	bool VltBarrierSet::isBufferOverlapped(
		const VltBufferSliceHandle& lhs,
		const VltBufferSliceHandle& rhs)
	{
		return (lhs.offset + lhs.length > rhs.offset) &&
			   (lhs.offset < rhs.offset + rhs.length);
	}

	bool VltBarrierSet::isImageOverlapped(
		const VkImageSubresourceRange& lhs,
		const VkImageSubresourceRange& rhs)
	{
		return (lhs.baseArrayLayer < rhs.baseArrayLayer + rhs.layerCount) &&
			   (lhs.baseArrayLayer + lhs.layerCount > rhs.baseArrayLayer) &&
			   (lhs.baseMipLevel < rhs.baseMipLevel + rhs.levelCount) &&
			   (lhs.baseMipLevel + lhs.levelCount > rhs.baseMipLevel);
	}

}  // namespace sce::vlt