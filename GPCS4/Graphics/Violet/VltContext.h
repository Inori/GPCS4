#pragma once

#include "VltCommon.h"
#include "VltCmdList.h"
#include "VltBarrier.h"
#include "VltContextState.h"

namespace sce::vlt
{
	class VltDevice;
	class VltImage;

	/**
     * \brief DXVk context
     * 
     * Tracks pipeline state and records command lists.
     * This is where the actual rendering commands are
     * recorded.
     */
	class VltContext : public RcObject
	{
	public:
		VltContext(VltDevice* device);
		virtual ~VltContext();

	    /**
         * \brief Begins command buffer recording
         * 
         * Begins recording a command list. This does
         * not alter any context state other than the
         * active command list.
         * \param [in] cmdList Target command list
         */
		void beginRecording(
			const Rc<VltCommandList>& cmdList);

		/**
         * \brief Ends command buffer recording
         * 
         * Finishes recording the active command list.
         * The command list can then be submitted to
         * the device.
         * 
         * This will not change any context state
         * other than the active command list.
         * \returns Active command list
         */
		Rc<VltCommandList> endRecording();

        /**
         * \brief Transforms image subresource layouts
         * 
         * \param [in] dstImage Image to transform
         * \param [in] dstSubresources Subresources
         * \param [in] srcLayout Current layout
         * \param [in] dstLayout Desired layout
         */
		void transformImage(
			const Rc<VltImage>&            dstImage,
			const VkImageSubresourceRange& dstSubresources,
			VkImageLayout                  srcLayout,
			VkImageLayout                  dstLayout);


    private:
		bool updateGraphicsPipeline();

		template <bool Indexed, bool Indirect>
		bool commitGraphicsState();

	private:
		VltDevice* m_device;

        Rc<VltCommandList> m_cmd;

        VltContextFlags m_flags;
		VltContextState m_state;

        VltBarrierSet m_execBarriers;
	};


}  // namespace sce::vlt