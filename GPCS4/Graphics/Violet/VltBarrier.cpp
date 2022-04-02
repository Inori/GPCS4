#include "VltBarrier.h"

#include "VltBuffer.h"
#include "VltImage.h"

namespace sce::vlt
{

	VltBarrierSet::VltBarrierSet(VltCmdBuffer cmdBuffer) :
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
		m_memBarrier.srcStageMask |= srcStages;
		m_memBarrier.dstStageMask |= dstStages;

		m_memBarrier.srcAccessMask |= srcAccess;
		m_memBarrier.dstAccessMask |= dstAccess;
	}

	//void DxvkBarrierSet::accessBuffer(
	//	const DxvkBufferSliceHandle& bufSlice,
	//	VkPipelineStageFlags2         srcStages,
	//	VkAccessFlags2                srcAccess,
	//	VkPipelineStageFlags2         dstStages,
	//	VkAccessFlags2                dstAccess)
	//{
	//	VltAccessFlags access = this->getAccessTypes(srcAccess);

	//	if (srcStages == VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT || dstStages == VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
	//		access.set(VltAccess::Write);

	//	m_srcStages |= srcStages;
	//	m_dstStages |= dstStages;

	//	m_srcAccess |= srcAccess;
	//	m_dstAccess |= dstAccess;

	//	m_bufSlices.push_back({ bufSlice, access });
	//}

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
		VltAccessFlags access = this->getAccessTypes(srcAccess);

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

	//void DxvkBarrierSet::releaseBuffer(
	//	DxvkBarrierSet&              acquire,
	//	const DxvkBufferSliceHandle& bufSlice,
	//	uint32_t                     srcQueue,
	//	VkPipelineStageFlags2         srcStages,
	//	VkAccessFlags2                srcAccess,
	//	uint32_t                     dstQueue,
	//	VkPipelineStageFlags2         dstStages,
	//	VkAccessFlags2                dstAccess)
	//{
	//	auto& release = *this;

	//	release.m_srcStages |= srcStages;
	//	acquire.m_dstStages |= dstStages;

	//	VkBufferMemoryBarrier barrier;
	//	barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	//	barrier.pNext               = nullptr;
	//	barrier.srcAccessMask       = srcAccess;
	//	barrier.dstAccessMask       = 0;
	//	barrier.srcQueueFamilyIndex = srcQueue;
	//	barrier.dstQueueFamilyIndex = dstQueue;
	//	barrier.buffer              = bufSlice.handle;
	//	barrier.offset              = bufSlice.offset;
	//	barrier.size                = bufSlice.length;
	//	release.m_bufBarriers.push_back(barrier);

	//	barrier.srcAccessMask = 0;
	//	barrier.dstAccessMask = dstAccess;
	//	acquire.m_bufBarriers.push_back(barrier);

	//	VltAccessFlags access(VltAccess::Read, VltAccess::Write);
	//	release.m_bufSlices.push_back({ bufSlice, access });
	//	acquire.m_bufSlices.push_back({ bufSlice, access });
	//}

	void VltBarrierSet::releaseImage(
		VltBarrierSet&                acquire,
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
		barrier.dstStageMask                = 0;
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

		barrier.srcStageMask  = 0;
		barrier.srcAccessMask = 0;
		barrier.dstStageMask  = dstStages;
		barrier.dstAccessMask = dstAccess;
		acquire.m_imgBarriers.push_back(barrier);

		VltAccessFlags access(VltAccess::Read, VltAccess::Write);
		release.m_imgSlices.push_back({ image.ptr(), subresources, access });
		acquire.m_imgSlices.push_back({ image.ptr(), subresources, access });
	}

	//bool DxvkBarrierSet::isBufferDirty(
	//	const DxvkBufferSliceHandle& bufSlice,
	//	VltAccessFlags               bufAccess)
	//{
	//	bool result = false;

	//	for (uint32_t i = 0; i < m_bufSlices.size() && !result; i++)
	//	{
	//		const DxvkBufferSliceHandle& dstSlice = m_bufSlices[i].slice;

	//		result = (bufSlice.handle == dstSlice.handle) && (bufAccess | m_bufSlices[i].access).test(VltAccess::Write) && (bufSlice.offset + bufSlice.length > dstSlice.offset) && (bufSlice.offset < dstSlice.offset + dstSlice.length);
	//	}

	//	return result;
	//}

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
					 isImageOverlaps(imgSubres, dstSubres);
		}

		return result;
	}

	//VltAccessFlags DxvkBarrierSet::getBufferAccess(
	//	const DxvkBufferSliceHandle& bufSlice)
	//{
	//	VltAccessFlags access;

	//	for (uint32_t i = 0; i < m_bufSlices.size(); i++)
	//	{
	//		const DxvkBufferSliceHandle& dstSlice = m_bufSlices[i].slice;

	//		if ((bufSlice.handle == dstSlice.handle) && (bufSlice.offset + bufSlice.length > dstSlice.offset) && (bufSlice.offset < dstSlice.offset + dstSlice.length))
	//			access = access | m_bufSlices[i].access;
	//	}

	//	return access;
	//}

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
		m_memBarrier = { VK_STRUCTURE_TYPE_MEMORY_BARRIER_2, nullptr, 0, 0, 0, 0 };

		m_bufBarriers.resize(0);
		m_imgBarriers.resize(0);

		m_bufSlices.resize(0);
		m_imgSlices.resize(0);
	}

	VltAccessFlags VltBarrierSet::getAccessTypes(VkAccessFlags2 flags) const
	{
		const VkAccessFlags2 rflags =
			VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT |
			VK_ACCESS_2_INDEX_READ_BIT |
			VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT |
			VK_ACCESS_2_UNIFORM_READ_BIT |
			VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT |
			VK_ACCESS_2_SHADER_READ_BIT |
			VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT |
			VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
			VK_ACCESS_2_TRANSFER_READ_BIT |
			VK_ACCESS_2_HOST_READ_BIT |
			VK_ACCESS_2_MEMORY_READ_BIT |
			VK_ACCESS_2_SHADER_SAMPLED_READ_BIT |
			VK_ACCESS_2_SHADER_STORAGE_READ_BIT |
			VK_ACCESS_2_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT |
			VK_ACCESS_2_CONDITIONAL_RENDERING_READ_BIT_EXT |
			VK_ACCESS_2_COMMAND_PREPROCESS_READ_BIT_NV |
			VK_ACCESS_2_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR |
			VK_ACCESS_2_SHADING_RATE_IMAGE_READ_BIT_NV |
			VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_KHR |
			VK_ACCESS_2_ACCELERATION_STRUCTURE_READ_BIT_NV |
			VK_ACCESS_2_FRAGMENT_DENSITY_MAP_READ_BIT_EXT |
			VK_ACCESS_2_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT |
			VK_ACCESS_2_INVOCATION_MASK_READ_BIT_HUAWEI;

		const VkAccessFlags2 wflags =
			VK_ACCESS_2_SHADER_WRITE_BIT |
			VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT |
			VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
			VK_ACCESS_2_TRANSFER_WRITE_BIT |
			VK_ACCESS_2_HOST_WRITE_BIT |
			VK_ACCESS_2_MEMORY_WRITE_BIT |
			VK_ACCESS_2_SHADER_STORAGE_WRITE_BIT |
			VK_ACCESS_2_TRANSFORM_FEEDBACK_WRITE_BIT_EXT |
			VK_ACCESS_2_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT |
			VK_ACCESS_2_COMMAND_PREPROCESS_WRITE_BIT_NV |
			VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_KHR |
			VK_ACCESS_2_ACCELERATION_STRUCTURE_WRITE_BIT_NV;

		VltAccessFlags result;
		if (flags & rflags)
			result.set(VltAccess::Read);
		if (flags & wflags)
			result.set(VltAccess::Write);
		return result;
	}

	bool VltBarrierSet::isBufferOverlaps()
	{
	}

	bool VltBarrierSet::isImageOverlaps(
		const VkImageSubresourceRange& lhs, 
		const VkImageSubresourceRange& rhs)
	{
		return (lhs.baseArrayLayer < rhs.baseArrayLayer + rhs.layerCount) &&
			   (lhs.baseArrayLayer + lhs.layerCount > rhs.baseArrayLayer) &&
			   (lhs.baseMipLevel < rhs.baseMipLevel + rhs.levelCount) &&
			   (lhs.baseMipLevel + lhs.levelCount > rhs.baseMipLevel);
	}

}  // namespace sce::vlt