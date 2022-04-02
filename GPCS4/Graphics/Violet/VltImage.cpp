#include "VltImage.h"

namespace sce::vlt
{

	std::atomic<uint64_t> VltImageView::s_cookie = { 0ull };

	VltImage::VltImage(
		VltDevice*                device,
		const VltImageCreateInfo& createInfo,
		VltMemoryAllocator&       memAlloc,
		VkMemoryPropertyFlags     memFlags) :
		m_device(device),
		m_info(createInfo), m_memFlags(memFlags)
	{
	}

	VltImage::VltImage(
		VltDevice*                device,
		const VltImageCreateInfo& info,
		VkImage                   image) :
		m_device(device),
		m_info(info), m_image({ image })
	{
	}

	VltImage::~VltImage()
	{
	}

	VltImageView::VltImageView(
		VltDevice*                    device,
		const Rc<VltImage>&           image,
		const VltImageViewCreateInfo& info) :
		m_device(device),
		m_image(image), m_info(info), m_cookie(++s_cookie)
	{
	}

	VltImageView::~VltImageView()
	{
	}

}  // namespace sce::vlt