#include "GnmTextureCache.h"

#include "GnmTexture.h"
#include "GnmRenderTarget.h"
#include "GnmDepthRenderTarget.h"
#include "GnmConvertor.h"

#include "../Violet/VltDevice.h"
#include "../Violet/VltContext.h"
#include "../Violet/VltImage.h"
#include "../Violet/VltPresenter.h"
#include "../Sce/SceGpuQueue.h"
#include "../Sce/SceVideoOut.h"

LOG_CHANNEL(Graphic.Gnm.GnmTextureCache);

using namespace vlt;
using namespace sce;
using namespace pssl;

GnmTextureCache::GnmTextureCache(const sce::SceGpuQueueDevice* device,
								 vlt::VltContext*              context,
								 GnmMemoryMonitor*             monitor) :
	m_device(device),
	m_context(context),
	m_monitor(monitor)
{
	collectRenderTargets();
}

GnmTextureCache::~GnmTextureCache()
{

}

GnmTextureInstance* GnmTextureCache::grabTexture(const GnmTextureCreateInfo& desc)
{
	GnmTextureInstance* texture = nullptr;

	GnmMemoryRange range = extractMemoryRange(desc);

	auto iter = m_textureMap.find(range);
	if (iter == m_textureMap.end())
	{
		auto instance = createTexture(desc);
		auto [iter, inserted] = m_textureMap.emplace(std::make_pair(range, instance));
		texture = &iter->second;
	}
	else
	{
		texture = &iter->second;
	}

	if (texture->memory.pendingSync())
	{
		upload(*texture);
	}

	return texture;
}

void GnmTextureCache::flush(const GnmMemoryRange& range)
{
	// Flush memory from Vulkan object to it's backing Gnm CPU memory.
}

void GnmTextureCache::invalidate(const GnmMemoryRange& range)
{
	// Mark the vulkan object as invalid
}


void GnmTextureCache::sync()
{
	for (auto& [range, texture] : m_textureMap)
	{
		if (texture.memory.pendingSync())
		{
			upload(texture);
		}
	}
}

GnmMemoryRange GnmTextureCache::extractMemoryRange(const GnmTextureCreateInfo& desc)
{
	GnmMemoryRange range = {};

	if (desc.stages & VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT || 
		desc.stages & VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT)
	{
		// Depth image

		// TODO:
		// Take stencil image into consideration.
		range.start = desc.depthRenderTarget->getZReadAddress();
		range.size  = desc.depthRenderTarget->getZSizeAlign().m_size;
	}
	else if (desc.stages & VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
	{
		// Render target
		range.start = desc.renderTarget->getBaseAddress();
		range.size  = desc.renderTarget->getColorSizeAlign().m_size;
	}
	else
	{
		// Other image
		range.start = desc.texture->getBaseAddress();
		range.size  = desc.texture->getSizeAlign().m_size;
	}

	return range;
}

GnmTextureInstance GnmTextureCache::createTexture(const GnmTextureCreateInfo& desc)
{
	GnmTextureInstance texture = {};

	VkFormat          format = {};
	VkImageUsageFlags usage  = {};
	VkAccessFlags     access = {};
	VkImageLayout     layout = {};
	VkExtent3D        extent = {};

	do 
	{
		if (desc.stages & VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT ||
			desc.stages & VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT)
		{
			// Depth image

			ZFormat zfmt = desc.depthRenderTarget->getZFormat();
			if (zfmt == kZFormatInvalid)
			{
				// kZFormatInvalid is an legal value,
				// it is used to disable Z buffer
				break;
			}

			format = cvt::convertZFormatToVkFormat(zfmt);  // TODO: Should check format support
			usage  = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			extent.width  = desc.depthRenderTarget->getWidth();
			extent.height = desc.depthRenderTarget->getHeight();
			extent.depth  = 1;
		}
		else if (desc.stages & VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
		{
			// Render target

			// TODO:
			// For future development, a game may use "render to texture" technique,
			// and set a render target which is not a swapchain image.
			// we should check whether "target" is the display buffer or not
			// and support extra render target,
			// but currently I just use the default one.
			LOG_FIXME("Not implemented.");
		}
		else
		{
			// Other image
			format = cvt::convertDataFormatToVkFormat(desc.texture->getDataFormat());
			usage  = VK_IMAGE_USAGE_SAMPLED_BIT;
			access = VK_ACCESS_SHADER_READ_BIT;
			layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			extent.width  = desc.texture->getWidth();
			extent.height = desc.texture->getHeight();
			extent.depth  = desc.texture->getDepth();
		}

		if (format == VK_FORMAT_UNDEFINED)
		{
			LOG_ERR("unknown format %d", format);
			break;
		}

		// TODO:
		// Fill these details from desc.
		VltImageCreateInfo imgInfo = {};
		imgInfo.type               = VK_IMAGE_TYPE_2D;
		imgInfo.format             = format;
		imgInfo.flags              = 0;
		imgInfo.sampleCount        = VK_SAMPLE_COUNT_1_BIT;
		imgInfo.extent             = extent;
		imgInfo.numLayers          = 1;
		imgInfo.mipLevels          = 1;
		imgInfo.usage              = usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imgInfo.stages             = desc.stages;
		imgInfo.access             = access;
		imgInfo.tiling             = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout             = layout;
		imgInfo.initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED;

		auto image = m_device->device->createImage(imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		if (!image)
		{
			LOG_ERR("create depth image failed.");
			break;
		}

		VltImageViewCreateInfo viewInfo = {};
		viewInfo.type                   = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format                 = imgInfo.format;
		viewInfo.usage                  = imgInfo.usage;
		viewInfo.aspect                 = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.numLayers              = imgInfo.numLayers;
		viewInfo.numLevels              = imgInfo.mipLevels;
		auto view                       = m_device->device->createImageView(image, viewInfo);
		if (!view)
		{
			LOG_ERR("create depth image view failed.");
			break;
		}

		GnmMemoryRange range = extractMemoryRange(desc);

		texture.image              = image;
		texture.view               = view;
		texture.memory             = GnmResourceMemory(
            range,
            desc.isGpuWritable ? GnmMemoryProtect::GpuReadWrite : GnmMemoryProtect::GpuReadOnly);

		// We need to sync the texture memory upon creation.
		texture.memory.setPendingSync(true);
	} while (false);

	return texture;
}

void GnmTextureCache::upload(GnmTextureInstance& texture)
{
	texture.memory.setPendingSync(false);
}

void GnmTextureCache::collectRenderTargets()
{
	// TODO:
	// For render targets, the correct way should be like this:
	// instead of using internal swapchain images, we need to
	// create real images used as render targets and then present
	// the images into swapchain like what dxvk does.

	// Insert
	// display buffer - swapchain image
	// pair to the image map.
	uint32_t numDisplayBuffer = m_device->videoOut->numDisplayBuffer();
	for (uint32_t i = 0; i != numDisplayBuffer; ++i)
	{
		SceDisplayBuffer bufferInfo = m_device->videoOut->getDisplayBuffer(i);
		GnmMemoryRange   range      = {};
		range.start                 = const_cast<void*>(bufferInfo.address);
		range.size                  = bufferInfo.size;

		PresenterImage     swapImage    = m_device->presenter->getImage(i);
		GnmTextureInstance renderTarget = {};
		renderTarget.image              = swapImage.image;
		renderTarget.view               = swapImage.view;
		m_textureMap.insert(std::make_pair(range, renderTarget));
	}
}
