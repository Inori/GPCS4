#pragma once

#include "VltCommon.h"
#include "VltRenderTarget.h"

#include <array>

namespace sce::vlt
{
	class VltBarrierSet;

	/**
	 * \brief Attachment 
	 *
	 * Stores the load/store ops 
	 * and layout of a single attachment.
	 */
	struct VltAttachmentOps
	{
		VkAttachmentLoadOp  loadOp     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		VkAttachmentStoreOp storeOp    = VK_ATTACHMENT_STORE_OP_STORE;
	};

	struct VltFrameBufferOps
	{
		VltAttachmentOps depthOps;
		VltAttachmentOps colorOps[MaxNumRenderTargets];
	};

	struct VltFrameBufferClearValues
	{
		VkClearValue colorValue[MaxNumRenderTargets];
		VkClearValue depthValue;
	};

	/**
     * \brief Framebuffer size
     * 
     * Stores the width, height and number of layers
     * of a framebuffer. This can be used in case a
     * framebuffer does not have any attachments.
     */
	struct VltFramebufferSize
	{
		uint32_t width;
		uint32_t height;
		uint32_t layers;
	};


	/**
     * \brief Framebuffer
     * 
	 * Unlike DXVK, we use dynamic rendering so there's no
	 * actual vulkan framebuffer objects.
	 * It's just a convenient wrapper of attachments. 
	 * 
     */
	class VltFramebuffer : public RcObject
	{
	public:
		VltFramebuffer(
			const VltRenderTargets&   renderTargets,
			const VltFramebufferSize& defaultSize);

		~VltFramebuffer();

		/**
         * \brief Framebuffer size
         * \returns Framebuffer size
         */
		VltFramebufferSize size() const
		{
			return m_renderSize;
		}

		/**
		 * \brief Number of color attachments
		 */
		uint32_t numColorAttachments() const
		{
			return m_colorAttachmentCount;
		}

		/**
		 * \brief Color target
		 *
		 * \param [in] id Target Index
		 * \returns The color target
		 */
		const VltAttachment& getColorTarget(uint32_t id) const
		{
			return m_renderTargets.color[id];
		}

		/**
         * \brief Depth-stencil target
         * \returns Depth-stencil target
         */
		const VltAttachment& getDepthTarget() const
		{
			return m_renderTargets.depth;
		}

		/**
		 * \brief Finds attachment index by view
		 *
		 * Color attachments start at 0
		 * \param [in] view Image view
		 * \returns Attachment index, -1 on fail
		 */
		int32_t findAttachment(const Rc<VltImageView>& view) const;

		/**
		 * \brief Retrieves attachment by index
		 *
		 * \param [in] id Framebuffer attachment ID
		 * \returns The framebuffer attachment
		 */
		const VltAttachment& getAttachment(uint32_t id) const;

		/**
		 * \brief Set attachment clear values
		 */
		void setAttachmentClearValues(const VltFrameBufferClearValues& values);

		/**
		 * \brief Set attachment ops
		 */
		void setAttachmentOps(const VltFrameBufferOps& ops);

		/**
         * \brief Retrieves color rendering attachments
         */
		const VkRenderingAttachmentInfo* colorAttachments() const
		{
			return m_colorAttachmentCount != 0 ?
				m_colorAttachments.data() : nullptr;
		}

		/**
         * \brief Retrieves depth rendering attachments
         */
		const VkRenderingAttachmentInfo* depthAttachment() const
		{
			return m_depthAttachment.imageView != VK_NULL_HANDLE ?
				&m_depthAttachment : nullptr;
		}

		/**
		 * \brief Transform attachment images to rendering-ready layout.
		 * 
		 */
		void prepareRenderingLayout(VltBarrierSet& barrier);

		/**
         * \brief Checks whether the framebuffer's targets match
         * 
         * \param [in] renderTargets Render targets to check
         * \returns \c true if the render targets are the same
         *          as the ones used for this framebuffer object.
         */
		bool matchTargets(const VltRenderTargets& renderTargets);

		/**
         * \brief Checks whether view and framebuffer sizes match
         *
         * Tests whether the size of the framebuffer is the same
         * as the size of one of its views. This may be \c false
         * when mixing attachments with mismatched dimensions.
         * \param [in] view Image view to test
         * \returns \c true if \c view has the same size as
         *          the framebuffer.
         */
		bool isFullSize(const Rc<VltImageView>& view) const;

	private:
		VltFramebufferSize computeRenderSize(
			const VltFramebufferSize& defaultSize) const;

		VltFramebufferSize computeRenderTargetSize(
			const Rc<VltImageView>& renderTarget) const;

	private:
		const VltRenderTargets   m_renderTargets;
		const VltFramebufferSize m_renderSize;

		uint32_t                                                   m_colorAttachmentCount = 0;
		std::array<VkRenderingAttachmentInfo, MaxNumRenderTargets> m_colorAttachments     = {};
		VkRenderingAttachmentInfo                                  m_depthAttachment      = {};
	};
}  // namespace sce::vlt