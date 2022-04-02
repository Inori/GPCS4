#include "VltBarrier.h"
#include "VltBuffer.h"
#include "VltImage.h"

namespace sce::vlt
{

	DxvkBarrierSet::DxvkBarrierSet(VltCmdBuffer cmdBuffer) :
		m_cmdBuffer(cmdBuffer)
	{
	}

	DxvkBarrierSet::~DxvkBarrierSet()
	{
	}

	void DxvkBarrierSet::accessMemory(
		VkPipelineStageFlags srcStages,
		VkAccessFlags        srcAccess,
		VkPipelineStageFlags dstStages,
		VkAccessFlags        dstAccess)
	{
	}

	//void DxvkBarrierSet::accessBuffer(
	//	const DxvkBufferSliceHandle& bufSlice,
	//	VkPipelineStageFlags         srcStages,
	//	VkAccessFlags                srcAccess,
	//	VkPipelineStageFlags         dstStages,
	//	VkAccessFlags                dstAccess)
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

	void DxvkBarrierSet::accessImage(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& subresources,
		VkImageLayout                  srcLayout,
		VkPipelineStageFlags           srcStages,
		VkAccessFlags                  srcAccess,
		VkImageLayout                  dstLayout,
		VkPipelineStageFlags           dstStages,
		VkAccessFlags                  dstAccess)
	{
		VltAccessFlags access = this->getAccessTypes(srcAccess);

		if (srcStages == VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT || 
			dstStages == VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT || 
			srcLayout != dstLayout)
			access.set(VltAccess::Write);

		m_srcStages |= srcStages;
		m_dstStages |= dstStages;

		if (srcLayout == dstLayout)
		{
			m_srcAccess |= srcAccess;
			m_dstAccess |= dstAccess;
		}
		else
		{
			VkImageMemoryBarrier barrier;
			barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.pNext                       = nullptr;
			barrier.srcAccessMask               = srcAccess;
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
	//	VkPipelineStageFlags         srcStages,
	//	VkAccessFlags                srcAccess,
	//	uint32_t                     dstQueue,
	//	VkPipelineStageFlags         dstStages,
	//	VkAccessFlags                dstAccess)
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

	void DxvkBarrierSet::releaseImage(
		DxvkBarrierSet&                acquire,
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& subresources,
		uint32_t                       srcQueue,
		VkImageLayout                  srcLayout,
		VkPipelineStageFlags           srcStages,
		VkAccessFlags                  srcAccess,
		uint32_t                       dstQueue,
		VkImageLayout                  dstLayout,
		VkPipelineStageFlags           dstStages,
		VkAccessFlags                  dstAccess)
	{
		auto& release = *this;

		release.m_srcStages |= srcStages;
		acquire.m_dstStages |= dstStages;

		VkImageMemoryBarrier barrier;
		barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext                       = nullptr;
		barrier.srcAccessMask               = srcAccess;
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

		barrier.srcAccessMask = 0;
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

	bool DxvkBarrierSet::isImageDirty(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& imgSubres,
		VltAccessFlags                 imgAccess)
	{
		bool result = false;

		for (uint32_t i = 0; i < m_imgSlices.size() && !result; i++)
		{
			const VkImageSubresourceRange& dstSubres = m_imgSlices[i].subres;

			result = (image == m_imgSlices[i].image) && (imgAccess | m_imgSlices[i].access).test(VltAccess::Write) && (imgSubres.baseArrayLayer < dstSubres.baseArrayLayer + dstSubres.layerCount) && (imgSubres.baseArrayLayer + imgSubres.layerCount > dstSubres.baseArrayLayer) && (imgSubres.baseMipLevel < dstSubres.baseMipLevel + dstSubres.levelCount) && (imgSubres.baseMipLevel + imgSubres.levelCount > dstSubres.baseMipLevel);
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

	VltAccessFlags DxvkBarrierSet::getImageAccess(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& imgSubres)
	{
		VltAccessFlags access;

		for (uint32_t i = 0; i < m_imgSlices.size(); i++)
		{
			const VkImageSubresourceRange& dstSubres = m_imgSlices[i].subres;

			if ((image == m_imgSlices[i].image) && (imgSubres.baseArrayLayer < dstSubres.baseArrayLayer + dstSubres.layerCount) && (imgSubres.baseArrayLayer + imgSubres.layerCount > dstSubres.baseArrayLayer) && (imgSubres.baseMipLevel < dstSubres.baseMipLevel + dstSubres.levelCount) && (imgSubres.baseMipLevel + imgSubres.levelCount > dstSubres.baseMipLevel))
				access = access | m_imgSlices[i].access;
		}

		return access;
	}

	void DxvkBarrierSet::recordCommands(const Rc<VltCommandList>& commandList)
	{
		if (m_srcStages | m_dstStages)
		{
			VkPipelineStageFlags srcFlags = m_srcStages;
			VkPipelineStageFlags dstFlags = m_dstStages;

			if (!srcFlags)
				srcFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			if (!dstFlags)
				dstFlags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			VkMemoryBarrier memBarrier;
			memBarrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
			memBarrier.pNext         = nullptr;
			memBarrier.srcAccessMask = m_srcAccess;
			memBarrier.dstAccessMask = m_dstAccess;

			VkMemoryBarrier* pMemBarrier = nullptr;
			if (m_srcAccess | m_dstAccess)
				pMemBarrier = &memBarrier;

			commandList->cmdPipelineBarrier(
				m_cmdBuffer, srcFlags, dstFlags, 0,
				pMemBarrier ? 1 : 0, pMemBarrier,
				m_bufBarriers.size(),
				m_bufBarriers.data(),
				m_imgBarriers.size(),
				m_imgBarriers.data());

			this->reset();
		}
	}

	void DxvkBarrierSet::reset()
	{
		m_srcStages = 0;
		m_dstStages = 0;

		m_srcAccess = 0;
		m_dstAccess = 0;

		m_bufBarriers.resize(0);
		m_imgBarriers.resize(0);

		m_bufSlices.resize(0);
		m_imgSlices.resize(0);
	}

	VltAccessFlags DxvkBarrierSet::getAccessTypes(VkAccessFlags flags) const
	{
		const VkAccessFlags rflags =
			VK_ACCESS_INDIRECT_COMMAND_READ_BIT |
			VK_ACCESS_INDEX_READ_BIT |
			VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT |
			VK_ACCESS_UNIFORM_READ_BIT |
			VK_ACCESS_INPUT_ATTACHMENT_READ_BIT |
			VK_ACCESS_SHADER_READ_BIT |
			VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
			VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
			VK_ACCESS_TRANSFER_READ_BIT |
			VK_ACCESS_HOST_READ_BIT |
			VK_ACCESS_MEMORY_READ_BIT |
			VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;

		const VkAccessFlags wflags =
			VK_ACCESS_SHADER_WRITE_BIT |
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
			VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
			VK_ACCESS_TRANSFER_WRITE_BIT |
			VK_ACCESS_HOST_WRITE_BIT |
			VK_ACCESS_MEMORY_WRITE_BIT |
			VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT |
			VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;

		VltAccessFlags result;
		if (flags & rflags)
			result.set(VltAccess::Read);
		if (flags & wflags)
			result.set(VltAccess::Write);
		return result;
	}

}  // namespace sce::vlt