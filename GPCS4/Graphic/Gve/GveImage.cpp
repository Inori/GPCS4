#include "GveImage.h"
#include "GveDevice.h"
#include "GveUtil.h"
#include "UtilMath.h"

LOG_CHANNEL(Graphic.Gve.GveImage);

namespace gve
{;


GveImage::GveImage(
	const RcPtr<GveDevice>& device, 
	const GveImageCreateInfo& createInfo, 
	GveMemoryAllocator*  memAlloc,
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

	if (vkCreateImage(*device, &info, nullptr, &m_image) != VK_SUCCESS)
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
	memReqInfo.image = m_image;
	memReqInfo.pNext = VK_NULL_HANDLE;

	VkMemoryDedicatedAllocateInfoKHR dedMemoryAllocInfo;
	dedMemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR;
	dedMemoryAllocInfo.pNext = VK_NULL_HANDLE;
	dedMemoryAllocInfo.buffer = VK_NULL_HANDLE;
	dedMemoryAllocInfo.image = m_image;

	vkGetImageMemoryRequirements2(*device, &memReqInfo, &memReq);

	if (info.tiling != VK_IMAGE_TILING_LINEAR)
	{
		memReq.memoryRequirements.size      = ::util::alignRound(memReq.memoryRequirements.size, memAlloc->bufferImageGranularity());
		memReq.memoryRequirements.alignment = ::util::alignRound(memReq.memoryRequirements.alignment, memAlloc->bufferImageGranularity());
	}

	// Use high memory priority for GPU-writable resources
	bool isGpuWritable = (info.usage & (
		VK_IMAGE_USAGE_STORAGE_BIT |
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)) != 0;

	float priority = isGpuWritable ? 1.0f : 0.5f;

	// Ask driver whether we should be using a dedicated allocation
	m_memory = memAlloc->alloc(&memReq.memoryRequirements,
		dedicatedRequirements, dedMemoryAllocInfo, memFlags, priority);

	// Try to bind the allocated memory slice to the image
	if (vkBindImageMemory(*device, m_image, m_memory.memory(), m_memory.offset()) != VK_SUCCESS)
	{
		LOG_ERR("DxvkImage::DxvkImage: Failed to bind device memory");
	}
}

GveImage::GveImage(
	const RcPtr<GveDevice>& device, 
	const GveImageCreateInfo& info,
	VkImage image):
	m_device(device),
	m_info(info),
	m_image(image)
{

}

GveImage::~GveImage()
{
	vkDestroyImage(*m_device, m_image, nullptr);
}

VkImage GveImage::handle() const
{
	return m_image;
}

const GveImageCreateInfo& GveImage::info() const
{
	return m_info;
}

uint32_t GveImage::getWidth() const
{
	return m_info.extent.width;
}

uint32_t GveImage::getHeight() const
{
	return m_info.extent.height;
}

VkFormat GveImage::getFormat() const
{
	return m_info.format;
}

VkImageLayout GveImage::getLayout() const
{
	return m_info.layout;
}

///

GveImageView::GveImageView(const RcPtr<GveDevice>& device, 
	const GveImageViewCreateInfo& createInfo, 
	const RcPtr<GveImage>& image):
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
		viewInfo.format = m_image->getFormat();
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

GveImageView::~GveImageView()
{
}

const GveImageViewCreateInfo& GveImageView::info() const
{
	return m_info;
}

uint32_t GveImageView::getWidth() const
{
	return m_image->getWidth();
}

uint32_t GveImageView::getHeight() const
{
	return m_image->getHeight();
}

VkImageView GveImageView::handle() const
{
	return m_imageView;
}

RcPtr<gve::GveImage> GveImageView::getImage()
{
	return m_image;
}


const gve::GveImageCreateInfo& GveImageView::imageInfo() const
{
	return m_image->info();
}

GveDescriptorInfo GveImageView::getDescriptor(VkImageViewType type, VkImageLayout layout) const
{
	GveDescriptorInfo descInfo;

	descInfo.image.sampler = VK_NULL_HANDLE;
	descInfo.image.imageView = m_imageView;
	descInfo.image.imageLayout = layout;
	
	return descInfo;
}

VkExtent3D GveImageView::mipLevelExtent(uint32_t mipLevel) const
{
	return util::computeMipLevelExtent(m_image->info().extent, mipLevel);
}

}  // namespace gve

