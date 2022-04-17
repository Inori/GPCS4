#pragma once

#include "SceCommon.h"
#include "SceResource.h"

#include "Violet/VltRc.h"

namespace sce
{
	namespace vlt
	{
		class VltDevice;
		class VltContext;
	}  // namespace vlt

	class ScePresenter;
	class SceSwapchainBlitter;
	class SceVideoOut;
	struct PresenterDesc;

	struct SceSwapchainDevice
	{
		VkPhysicalDevice adapter;
		vlt::VltDevice*  device;
		SceVideoOut*     videoOut;
	};

	class SceSwapchain
	{
	public:
		SceSwapchain(
			const SceSwapchainDevice& device,
			const PresenterDesc&      desc);
		~SceSwapchain();

		/**
		 * \brief Get render target image.
		 */
		const SceRenderTarget& getImage(uint32_t index);

		/**
		 * \brief Present 
		 * 
		 * Draw the image specified by index to swapchain.
		 */
		void present(uint32_t index);

	private:
		void createPresenter(
			const PresenterDesc& desc);

		void createRenderTargets();

		void createSwapImageViews();

	private:
		SceSwapchainDevice m_device;

		vlt::Rc<vlt::VltContext> m_context;
		vlt::Rc<ScePresenter>    m_presenter;

		std::vector<vlt::Rc<vlt::VltImageView>> m_imageViews;
		vlt::Rc<SceSwapchainBlitter>            m_blitter;

		std::vector<SceRenderTarget> m_renderTargets;
	};

}  // namespace sce