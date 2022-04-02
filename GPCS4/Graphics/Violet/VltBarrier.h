#pragma once

#include "VltCmdList.h"
#include "VltCommon.h"

namespace sce::vlt
{
	class VltImage;

	/**
     * \brief Barrier set
     * 
     * Accumulates memory barriers and provides a
     * method to record all those barriers into a
     * command buffer at once.
     */
	class DxvkBarrierSet
	{

	public:
		DxvkBarrierSet(VltCmdBuffer cmdBuffer);
		~DxvkBarrierSet();

		void accessMemory(
			VkPipelineStageFlags srcStages,
			VkAccessFlags        srcAccess,
			VkPipelineStageFlags dstStages,
			VkAccessFlags        dstAccess);

		//void accessBuffer(
		//	const DxvkBufferSliceHandle& bufSlice,
		//	VkPipelineStageFlags         srcStages,
		//	VkAccessFlags                srcAccess,
		//	VkPipelineStageFlags         dstStages,
		//	VkAccessFlags                dstAccess);

		void accessImage(
			const Rc<VltImage>&            image,
			const VkImageSubresourceRange& subresources,
			VkImageLayout                  srcLayout,
			VkPipelineStageFlags           srcStages,
			VkAccessFlags                  srcAccess,
			VkImageLayout                  dstLayout,
			VkPipelineStageFlags           dstStages,
			VkAccessFlags                  dstAccess);

		//void releaseBuffer(
		//	DxvkBarrierSet&              acquire,
		//	const DxvkBufferSliceHandle& bufSlice,
		//	uint32_t                     srcQueue,
		//	VkPipelineStageFlags         srcStages,
		//	VkAccessFlags                srcAccess,
		//	uint32_t                     dstQueue,
		//	VkPipelineStageFlags         dstStages,
		//	VkAccessFlags                dstAccess);

		void releaseImage(
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
			VkAccessFlags                  dstAccess);

		//bool isBufferDirty(
		//	const DxvkBufferSliceHandle& bufSlice,
		//	VltAccessFlags               bufAccess);

		bool isImageDirty(
			const Rc<VltImage>&            image,
			const VkImageSubresourceRange& imgSubres,
			VltAccessFlags                 imgAccess);

		//VltAccessFlags getBufferAccess(
		//	const DxvkBufferSliceHandle& bufSlice);

		VltAccessFlags getImageAccess(
			const Rc<VltImage>&            image,
			const VkImageSubresourceRange& imgSubres);

		VkPipelineStageFlags getSrcStages()
		{
			return m_srcStages;
		}

		void recordCommands(
			const Rc<VltCommandList>& commandList);

		void reset();

	private:
		struct BufSlice
		{
			//DxvkBufferSliceHandle slice;
			VltAccessFlags access;
		};

		struct ImgSlice
		{
			VltImage*               image;
			VkImageSubresourceRange subres;
			VltAccessFlags          access;
		};

		VltCmdBuffer m_cmdBuffer;

		VkPipelineStageFlags m_srcStages = 0;
		VkPipelineStageFlags m_dstStages = 0;

		VkAccessFlags m_srcAccess = 0;
		VkAccessFlags m_dstAccess = 0;

		std::vector<VkBufferMemoryBarrier> m_bufBarriers;
		std::vector<VkImageMemoryBarrier>  m_imgBarriers;

		std::vector<BufSlice> m_bufSlices;
		std::vector<ImgSlice> m_imgSlices;

		VltAccessFlags getAccessTypes(VkAccessFlags flags) const;
	};
}  // namespace sce::vlt