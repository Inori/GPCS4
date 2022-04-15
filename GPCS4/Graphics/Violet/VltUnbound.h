#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	class VltDevice;
	class VltContext;
	class VltBuffer;
	class VltBufferView;
	class VltImage;
	class VltImageView;
	class VltSampler;

	/**
     * \brief Unbound resources
     * 
     * Creates dummy resources that will be used
     * for descriptor sets when the client API did
     * not bind a compatible resource to a slot.
     */
	class VltUnboundResources
	{

	public:
		VltUnboundResources(VltDevice* dev);
		~VltUnboundResources();

		/**
         * \brief Dummy buffer handle
         * 
         * Returns a handle to a buffer filled
         * with zeroes. Use for unbound vertex
         * and index buffers.
         * \returns Dummy buffer handle
         */
		VkBuffer bufferHandle() const;

		/**
         * \brief Dummy buffer descriptor
         * 
         * Points to a small buffer filled with zeroes.
         * Do not write to this buffer, and do not use
         * it if out-of-bounds read access is possible.
         * \returns Dummy buffer descriptor
         */
		VkDescriptorBufferInfo bufferDescriptor() const;

		/**
         * \brief Dummy buffer view
         * 
         * Returns an \c R32_UINT view into the
         * dummy buffer, which will contain one
         * element with undefined value.
         * \returns Dummy buffer view
         */
		VkBufferView bufferViewDescriptor() const;

		/**
         * \brief Dummy sampler descriptor
         * 
         * Points to a sampler which was created with
         * reasonable default values. Client APIs may
         * still require different behaviour.
         * \returns Dummy sampler descriptor
         */
		VkDescriptorImageInfo samplerDescriptor() const;

		/**
         * \brief Dummy combined image sampler descriptor
         * 
         * Contains both an image view and a sampler
         * descriptor for the given image view type.
         * \param [in] type Image view type
         * \returns Dummy image view descriptor
         */
		VkDescriptorImageInfo imageSamplerDescriptor(VkImageViewType type) const;

		/**
         * \brief Dummy image view descriptor
         * 
         * Points to an image view which, instead of
         * reading image data, will return zeroes for
         * all components unconditionally.
         * \param [in] type Image view type
         * \param [in] sampled Format selector
         * \returns Dummy image view descriptor
         */
		VkDescriptorImageInfo imageViewDescriptor(VkImageViewType type, bool sampled) const;

		/**
         * \brief Clears the resources
         * 
         * Initializes all images and buffers to zero.
         * \param [in] dev The DXVK device handle
         */
		void clearResources(VltDevice* dev);

	private:
		struct UnboundViews
		{
			Rc<VltImageView> view1D;
			Rc<VltImageView> view1DArr;
			Rc<VltImageView> view2D;
			Rc<VltImageView> view2DArr;
			Rc<VltImageView> viewCube;
			Rc<VltImageView> viewCubeArr;
			Rc<VltImageView> view3D;
		};

		Rc<VltSampler> m_sampler;

		Rc<VltBuffer>     m_buffer;
		Rc<VltBufferView> m_bufferView;

		Rc<VltImage> m_image1D;
		Rc<VltImage> m_image2D;
		Rc<VltImage> m_image3D;

		UnboundViews m_viewsSampled;
		UnboundViews m_viewsStorage;

		Rc<VltSampler> createSampler(VltDevice* dev);

		Rc<VltBuffer> createBuffer(VltDevice* dev);

		Rc<VltBufferView> createBufferView(
			VltDevice*           dev,
			const Rc<VltBuffer>& buffer);

		Rc<VltImage> createImage(
			VltDevice*  dev,
			VkImageType type,
			uint32_t    layers);

		Rc<VltImageView> createImageView(
			VltDevice*          dev,
			const Rc<VltImage>& image,
			VkFormat            format,
			VkImageViewType     type,
			uint32_t            layers);

		UnboundViews createImageViews(
			VltDevice* dev,
			VkFormat   format);

		const VltImageView* getImageView(
			VkImageViewType type,
			bool            sampled) const;

		void clearBuffer(
			const Rc<VltContext>& ctx,
			const Rc<VltBuffer>&  buffer);

		void clearImage(
			const Rc<VltContext>& ctx,
			const Rc<VltImage>&   image);
	};
}  // namespace sce::vlt