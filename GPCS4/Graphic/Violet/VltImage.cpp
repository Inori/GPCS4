#include "VltImage.h"
#include "VltDevice.h"
#include "VltUtil.h"
#include "VltFormat.h"
#include "UtilMath.h"

LOG_CHANNEL(Graphic.Violet.VltImage);

namespace vlt
{;


VltImage::VltImage(
	const RcPtr<VltDevice>& device, 
	const VltImageCreateInfo& createInfo, 
	VltMemoryAllocator*  memAlloc,
	VkMemoryPropertyFlags memFlags):
	m_device(device),
	m_info(createInfo)
{
	VkImageCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.imageType = VK_IMAGE_TYPE_2D;
	info.extent.width = m_info.extent.width;
	info.extent.height = m_info.extent.height;
	info.extent.depth = m_info.extent.depth;
	info.mipLevels = m_info.mipLevels;
	info.arrayLayers = 1;
	info.format = m_info.format;
	info.tiling = m_info.tiling;
	info.initialLayout = m_info.initialLayout;
	info.usage = m_info.usage;
	info.samples = VK_SAMPLE_COUNT_1_BIT;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(*device, &info, nullptr, &m_image.image) != VK_SUCCESS)
	{
		LOG_ERR("create image failed.");
	}

	// Get memory requirements for the image. We may enforce strict
	// alignment on non-linear images in order not to violate the
	// bufferImageGranularity limit, which may be greater than the
	// required resource memory alignment on some GPUs.
	VkMemoryDedicatedRequirementsKHR dedicatedRequirements;
	dedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR;
	dedicatedRequirements.pNext = VK_NULL_HANDLE;
	dedicatedRequirements.prefersDedicatedAllocation = VK_FALSE;
	dedicatedRequirements.requiresDedicatedAllocation = VK_FALSE;

	VkMemoryRequirements2 memReq;
	memReq.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR;
	memReq.pNext = &dedicatedRequirements;

	VkImageMemoryRequirementsInfo2 memReqInfo;
	memReqInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2_KHR;
	memReqInfo.image = m_image.image;
	memReqInfo.pNext = VK_NULL_HANDLE;

	VkMemoryDedicatedAllocateInfoKHR dedMemoryAllocInfo;
	dedMemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR;
	dedMemoryAllocInfo.pNext = VK_NULL_HANDLE;
	dedMemoryAllocInfo.buffer = VK_NULL_HANDLE;
	dedMemoryAllocInfo.image  = m_image.image;

	vkGetImageMemoryRequirements2(*device, &memReqInfo, &memReq);

	if (info.tiling != VK_IMAGE_TILING_LINEAR)
	{
		memReq.memoryRequirements.size      = ::util::align(memReq.memoryRequirements.size, memAlloc->bufferImageGranularity());
		memReq.memoryRequirements.alignment = ::util::align(memReq.memoryRequirements.alignment, memAlloc->bufferImageGranularity());
	}

	// Use high memory priority for GPU-writable resources
	bool isGpuWritable = (info.usage & (
		VK_IMAGE_USAGE_STORAGE_BIT |
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)) != 0;

	float priority = isGpuWritable ? 1.0f : 0.5f;

	// Ask driver whether we should be using a dedicated allocation
	m_image.memory = memAlloc->alloc(&memReq.memoryRequirements,
		dedicatedRequirements, dedMemoryAllocInfo, memFlags, priority);

	// Try to bind the allocated memory slice to the image
	if (vkBindImageMemory(*device, m_image.image, m_image.memory.memory(), m_image.memory.offset()) != VK_SUCCESS)
	{
		LOG_ERR("DxvkImage::DxvkImage: Failed to bind device memory");
	}
}

VltImage::VltImage(
	const RcPtr<VltDevice>& device, 
	const VltImageCreateInfo& info,
	VkImage image):
	m_device(device),
	m_info(info)
{
	m_image.image = image;
}

VltImage::~VltImage()
{
	if (m_image.memory.memory() != VK_NULL_HANDLE)
	{
		vkDestroyImage(*m_device, m_image.image, nullptr);
	}
}

VkImage VltImage::handle() const
{
	return m_image.image;
}

const VltImageCreateInfo& VltImage::info() const
{
	return m_info;
}

const VltFormatInfo* VltImage::formatInfo() const
{
	return imageFormatInfo(m_info.format);
}

VkImageLayout VltImage::pickLayout(VkImageLayout target) const
{
	VkImageLayout layout = target;
	if (m_info.layout == VK_IMAGE_LAYOUT_GENERAL)
	{
		layout = m_info.layout;
	}
	return layout;
}

///

VltImageView::VltImageView(const RcPtr<VltDevice>& device, 
	const VltImageViewCreateInfo& createInfo, 
	const RcPtr<VltImage>& image):
	m_device(device),
	m_info(createInfo),
	m_image(image)
{
	do 
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_image->handle();
		viewInfo.viewType = createInfo.type;
		viewInfo.format = m_image->info().format;
		viewInfo.subresourceRange.aspectMask = createInfo.aspect;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(*m_device, &viewInfo, nullptr, &m_imageView) != VK_SUCCESS) 
		{
			LOG_ERR("failed to create texture image view!");
			break;
		}
	} while (false);
}

VltImageView::~VltImageView()
{
	vkDestroyImageView(*m_device, m_imageView, nullptr);
}

const VltImageViewCreateInfo& VltImageView::info() const
{
	return m_info;
}

VkImageView VltImageView::handle() const
{
	return m_imageView;
}

const RcPtr<VltImage>& VltImageView::image() const
{
	return m_image;
}


const vlt::VltImageCreateInfo& VltImageView::imageInfo() const
{
	return m_image->info();
}

VltDescriptorInfo VltImageView::getDescriptor(VkImageViewType type, VkImageLayout layout) const
{
	VltDescriptorInfo descInfo;

	descInfo.image.sampler = VK_NULL_HANDLE;
	descInfo.image.imageView = m_imageView;
	descInfo.image.imageLayout = layout;
	
	return descInfo;
}

VkExtent3D VltImageView::mipLevelExtent(uint32_t mipLevel) const
{
	return util::computeMipLevelExtent(m_image->info().extent, mipLevel);
}

VkImageLayout VltImageView::pickLayout(VkImageLayout target) const
{
	return m_image->pickLayout(target);
}

}  // namespace vlt

