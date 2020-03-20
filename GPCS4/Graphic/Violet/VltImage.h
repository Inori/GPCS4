#pragma once

#include "VltCommon.h"
#include "VltMemory.h"
#include "VltDescriptor.h"
#include "VltGpuResource.h"


namespace vlt
{;

struct VltFormatInfo;
class VltDevice;


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
};

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


class VltImage : public VltGpuResource
{
public:
	VltImage(
		const RcPtr<VltDevice>&    device,
		const VltImageCreateInfo&  createInfo,
		VltMemoryAllocator*        memAlloc,
		VkMemoryPropertyFlags      memFlags);

	// Creates image object from existing image
	// Typically implementation-managed, like swapchain image.
	VltImage(
		const RcPtr<VltDevice>&   device,
		const VltImageCreateInfo& info,
		VkImage                   image);

	~VltImage();

	VkImage handle() const;

	const VltImageCreateInfo& info() const;

	const VltFormatInfo* formatInfo() const;

	VkImageLayout pickLayout(VkImageLayout target) const;


private:
	RcPtr<VltDevice>   m_device;
	VltImageCreateInfo m_info;

	VltPhysicalImage m_image;
};


///

class VltImageView : public VltGpuResource
{
public:
	VltImageView(const RcPtr<VltDevice>&       device,
				 const VltImageViewCreateInfo& createInfo,
				 const RcPtr<VltImage>&        image);
	~VltImageView();

	VkImageView handle() const;

	RcPtr<VltImage> getImage();

	const VltImageViewCreateInfo& info() const;

	const VltImageCreateInfo& imageInfo() const;

	VltDescriptorInfo getDescriptor(VkImageViewType type, VkImageLayout layout) const;

	VkExtent3D mipLevelExtent(uint32_t mipLevel) const;

private:
	RcPtr<VltDevice>       m_device;
	VltImageViewCreateInfo m_info;
	RcPtr<VltImage>        m_image;

	VkImageView m_imageView;
};



}  // namespace vlt