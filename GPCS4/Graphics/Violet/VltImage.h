#pragma once

#include "VltCommon.h"
#include "VltMemory.h"
#include "VltResource.h"
#include "VltFormat.h"

namespace sce::vlt
{
	class VltDevice;
	class VltImageView;

	/**
     * \brief Image create info
     * 
     * The properties of an image that are
     * passed to \ref DxvkDevice::createImage
     */
	struct VltImageCreateInfo
	{
		/// Image dimension
		VkImageType type;

		/// Pixel format
		VkFormat format;

		/// Flags
		VkImageCreateFlags flags;

		/// Sample count for MSAA
		VkSampleCountFlagBits sampleCount;

		/// Image size, in texels
		VkExtent3D extent;

		/// Number of image array layers
		uint32_t numLayers;

		/// Number of mip levels
		uint32_t mipLevels;

		/// Image usage flags
		VkImageUsageFlags usage;

		/// Pipeline stages that can access
		/// the contents of the image
		VkPipelineStageFlags stages;

		/// Allowed access pattern
		VkAccessFlags access;

		/// Image tiling mode
		VkImageTiling tiling;

		/// Common image layout
		VkImageLayout layout;

		// Initial image layout
		VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		// Image view formats that can
		// be used with this image
		uint32_t        viewFormatCount = 0;
		const VkFormat* viewFormats     = nullptr;
	};

	/**
     * \brief Image create info
     * 
     * The properties of an image view that are
     * passed to \ref DxvkDevice::createImageView
     */
	struct VltImageViewCreateInfo
	{
		/// Image view dimension
		VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D;

		/// Pixel format
		VkFormat format = VK_FORMAT_UNDEFINED;

		/// Image view usage flags
		VkImageUsageFlags usage = 0;

		/// Subresources to use in the view
		VkImageAspectFlags aspect = 0;

		uint32_t minLevel  = 0;
		uint32_t numLevels = 0;
		uint32_t minLayer  = 0;
		uint32_t numLayers = 0;

		/// Component mapping. Defaults to identity.
		VkComponentMapping swizzle = {
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
		};
	};

	/**
     * \brief Stores an image and its memory slice.
     */
	struct VltPhysicalImage
	{
		VkImage   image = VK_NULL_HANDLE;
		VltMemory memory;
	};

	/**
     * \brief VLT image
     * 
     * An image resource consisting of various subresources.
     * Can be accessed by the host if allocated on a suitable
     * memory type and if created with the linear tiling option.
     */
	class VltImage : public VltResource
	{
		friend class VltContext;
		friend class VltImageView;

	public:
		VltImage(
			VltDevice*                 device,
			const VltImageCreateInfo& createInfo,
			VltMemoryAllocator&        memAlloc,
			VkMemoryPropertyFlags      memFlags);

		/**
         * \brief Creates image object from existing image
         * 
         * This can be used to create an image object for
         * an implementation-managed image. Make sure to
         * provide the correct image properties, since
         * otherwise some image operations may fail.
         */
		VltImage(
			VltDevice*                 device,
			const VltImageCreateInfo& info,
			VkImage                    image);

		/**
         * \brief Destroys image
         * 
         * If this is an implementation-managed image,
         * this will not destroy the Vulkan image.
         */
		virtual ~VltImage();

		/**
         * \brief Image handle
         * 
         * Internal use only.
         * \returns Image handle
         */
		VkImage handle() const
		{
			return m_image.image;
		}

		/**
         * \brief Image properties
         * 
         * The image create info structure.
         * \returns Image properties
         */
		const VltImageCreateInfo& info() const
		{
			return m_info;
		}

		/**
         * \brief Memory type flags
         * 
         * Use this to determine whether a
         * buffer is mapped to host memory.
         * \returns Vulkan memory flags
         */
		VkMemoryPropertyFlags memFlags() const
		{
			return m_memFlags;
		}

		/**
         * \brief Image format info
         * \returns Image format info
         */
		const VltFormatInfo* formatInfo() const
		{
			return imageFormatInfo(m_info.format);
		}

	private:
		VltDevice*            m_device;
		VltImageCreateInfo   m_info;
		VkMemoryPropertyFlags m_memFlags;
		VltPhysicalImage     m_image;

		std::vector<VkFormat> m_viewFormats;
	};

	/**
     * \brief VLT image view
     */
	class VltImageView : public VltResource
	{
		friend class VltImage;
		constexpr static uint32_t ViewCount = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY + 1;

	public:
		VltImageView(
			VltDevice*                     device,
			const Rc<VltImage>&           image,
			const VltImageViewCreateInfo& info);

		virtual ~VltImageView();

	private:
		VltDevice*    m_device;
		Rc<VltImage> m_image;

		VltImageViewCreateInfo m_info;
		VkImageView             m_views[ViewCount];

		uint64_t m_cookie;

		static std::atomic<uint64_t> s_cookie;
	};

}  // namespace sce::vlt