#pragma once

#include "GveCommon.h"
#include "GveMemory.h"
#include "GveDescriptor.h"
#include "../Gnm/GnmTexture.h"


namespace gve
{;

class GveDevice;

struct GveImageCreateInfo
{
	GnmTexture texture;
};

struct GveImageViewCreateInfo
{
	GnmTexture texture;
};


class GveImage : public RcObject
{
public:
	GveImage(const RcPtr<GveDevice>& device,
		const GveImageCreateInfo& createInfo,
		GveMemoryAllocator& memAlloc,
		VkMemoryPropertyFlags memFlags);

	~GveImage();

	VkImage handle() const;

	VkFormat getFormat() const;

	VkImageLayout getLayout() const;

	const GnmTexture* getGnmTexture() const;


private:
	void convertCreateInfo(const GveImageCreateInfo& gveInfo,
		VkImageCreateInfo& vkInfo);

private:
	RcPtr<GveDevice> m_device;
	GveImageCreateInfo m_info;

	VkImage m_image = VK_NULL_HANDLE;
	GveMemory m_memory;

};


///

class GveImageView : public RcObject
{
public:
	GveImageView(const RcPtr<GveDevice>& device,
		const GveImageViewCreateInfo& createInfo,
		const RcPtr<GveImage>& image);
	~GveImageView();

	VkImageView handle() const;

	RcPtr<GveImage> getImage();

	GveDescriptorInfo getDescriptor(VkImageViewType type, VkImageLayout layout) const;

private:
	RcPtr<GveDevice> m_device;
	GveImageViewCreateInfo m_info;
	RcPtr<GveImage> m_image;

	VkImageView m_imageView;
};


}  // namespace gve