#pragma once

#include "SceCommon.h"

#include "Violet/VltRc.h"

namespace sce
{

	namespace vlt
	{
		class VltDevice;
		class VltContext;
		class VltShader;
		class VltImageView;
		class VltSampler;
	}  // namespace vlt

	/**
     * \brief Swap chain blitter
     *
     * Provides common rendering code for blitting
     * rendered images to a swap chain image.
     */
	class SceSwapchainBlitter : public vlt::RcObject
	{

	public:
		SceSwapchainBlitter(vlt::VltDevice* device);
		~SceSwapchainBlitter();

		/**
         * \brief Records presentation commands
         *
         * \param [in] ctx Context
         * \param [in] dstView Swap chain image view
         * \param [in] srcView Image to present
         * \param [in] dstRect Destination rectangle
         * \param [in] srcRect Back buffer rectangle
         */
		void presentImage(
			vlt::VltContext*                  ctx,
			const vlt::Rc<vlt::VltImageView>& dstView,
			VkRect2D                          dstRect,
			const vlt::Rc<vlt::VltImageView>& srcView,
			VkRect2D                          srcRect);

	private:
		enum BindingIds : uint32_t
		{
			Image = 0,
		};

		struct PresenterArgs
		{
			VkOffset2D srcOffset;
			union
			{
				VkExtent2D srcExtent;
				VkOffset2D dstOffset;
			};
		};

		vlt::VltDevice* m_device;

		vlt::Rc<vlt::VltShader> m_vs;
		vlt::Rc<vlt::VltShader> m_fsCopy;
		vlt::Rc<vlt::VltShader> m_fsBlit;

		vlt::Rc<vlt::VltSampler> m_samplerPresent;

		void draw(
			vlt::VltContext*                  ctx,
			const vlt::Rc<vlt::VltShader>&    fs,
			const vlt::Rc<vlt::VltImageView>& dstView,
			VkRect2D                          dstRect,
			const vlt::Rc<vlt::VltImageView>& srcView,
			VkRect2D                          srcRect);

		void createSampler();

		void createShaders();
	};
}  // namespace sce