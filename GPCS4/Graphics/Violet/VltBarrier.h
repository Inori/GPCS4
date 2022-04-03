#pragma once

#include "VltBuffer.h"
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
	class VltBarrierSet
	{
	public:
		VltBarrierSet(VltCmdBuffer cmdBuffer);
		~VltBarrierSet();

		void accessMemory(
			VkPipelineStageFlags2 srcStages,
			VkAccessFlags2        srcAccess,
			VkPipelineStageFlags2 dstStages,
			VkAccessFlags2        dstAccess);

		void accessBuffer(
			const VltBufferSliceHandle& bufSlice,
			VkPipelineStageFlags2       srcStages,
			VkAccessFlags2              srcAccess,
			VkPipelineStageFlags2       dstStages,
			VkAccessFlags2              dstAccess);

		void accessImage(
			const Rc<VltImage>&            image,
			const VkImageSubresourceRange& subresources,
			VkImageLayout                  srcLayout,
			VkPipelineStageFlags2          srcStages,
			VkAccessFlags2                 srcAccess,
			VkImageLayout                  dstLayout,
			VkPipelineStageFlags2          dstStages,
			VkAccessFlags2                 dstAccess);

		/**
		 * \brief Queue family ownership transfer
		 */
		void releaseBuffer(
			VltBarrierSet&              acquire,
			const VltBufferSliceHandle& bufSlice,
			uint32_t                    srcQueue,
			VkPipelineStageFlags2       srcStages,
			VkAccessFlags2              srcAccess,
			uint32_t                    dstQueue,
			VkPipelineStageFlags2       dstStages,
			VkAccessFlags2              dstAccess);

		/**
         * \brief Queue family ownership transfer
         */
		void releaseImage(
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
			VkAccessFlags2                 dstAccess);

		bool isBufferDirty(
			const VltBufferSliceHandle& bufSlice,
			VltAccessFlags              bufAccess);

		bool isImageDirty(
			const Rc<VltImage>&            image,
			const VkImageSubresourceRange& imgSubres,
			VltAccessFlags                 imgAccess);

		VltAccessFlags getBufferAccess(
			const VltBufferSliceHandle& bufSlice);

		VltAccessFlags getImageAccess(
			const Rc<VltImage>&            image,
			const VkImageSubresourceRange& imgSubres);

		VkPipelineStageFlags2 getSrcStages()
		{
			return m_memBarrier.srcStageMask;
		}

		void recordCommands(
			const Rc<VltCommandList>& commandList);

		void reset();

	private:
		bool isBufferOverlapped(
			const VltBufferSliceHandle& lhs,
			const VltBufferSliceHandle& rhs);

		bool isImageOverlapped(
			const VkImageSubresourceRange& lhs,
			const VkImageSubresourceRange& rhs);

	private:
		struct BufSlice
		{
			VltBufferSliceHandle slice;
			VltAccessFlags       access;
		};

		struct ImgSlice
		{
			VltImage*               image;
			VkImageSubresourceRange subres;
			VltAccessFlags          access;
		};

		VltCmdBuffer m_cmdBuffer;

		VkMemoryBarrier2 m_memBarrier = { VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
										  nullptr,
										  VK_PIPELINE_STAGE_2_NONE,
										  VK_ACCESS_2_NONE,
										  VK_PIPELINE_STAGE_2_NONE,
										  VK_ACCESS_2_NONE };

		std::vector<VkBufferMemoryBarrier2> m_bufBarriers;
		std::vector<VkImageMemoryBarrier2>  m_imgBarriers;

		std::vector<BufSlice> m_bufSlices;
		std::vector<ImgSlice> m_imgSlices;
	};
}  // namespace sce::vlt