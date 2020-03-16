#include "GnmResourceFactory.h"

#include "GnmBuffer.h"
#include "GnmConvertor.h"
#include "GnmDepthRenderTarget.h"
#include "GnmRenderTarget.h"
#include "GnmSampler.h"
#include "GnmTexture.h"
#include "UtilBit.h"

#include "../Violet/VltBuffer.h"
#include "../Violet/VltDevice.h"
#include "../Violet/VltImage.h"
#include "../Violet/VltPresenter.h"
#include "../Violet/VltSampler.h"
#include "../Pssl/PsslShaderFileBinary.h"
#include "../Sce/SceGpuQueue.h"
#include "../Sce/SceVideoOut.h"
#include "Algorithm/MurmurHash2.h"

LOG_CHANNEL(Graphic.Gnm.GnmResourceFactory);

using namespace vlt;
using namespace sce;
using namespace pssl;

GnmResourceFactory::GnmResourceFactory(const sce::SceGpuQueueDevice* device) :
	m_device(device)
{
	collectRenderTargets();
}

GnmResourceFactory::~GnmResourceFactory()
{
}

RcPtr<VltBuffer> GnmResourceFactory::grabIndex(const GnmIndexBuffer& desc, bool* create /*= nullptr*/)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.buffer;
	entry.size             = desc.size;
	auto createFunc        = [this, &desc]() { return createIndex(desc); };
	return grabResource(entry, m_bufferMap, createFunc, create);
}

RcPtr<VltBuffer> GnmResourceFactory::grabBuffer(const GnmBufferCreateInfo& desc, bool* create /*= nullptr*/)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.buffer->getBaseAddress();
	entry.size             = desc.buffer->getSize();
	auto createFunc        = [this, &desc]() { return createBuffer(desc); };
	return grabResource(entry, m_bufferMap, createFunc, create);
}

GnmCombinedImageView GnmResourceFactory::grabImage(const GnmTextureCreateInfo& desc, bool* create /*= nullptr*/)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.texture->getBaseAddress();
	entry.size             = desc.texture->getSizeAlign().m_size;
	auto createFunc        = [this, &desc]() { return createImage(desc); };
	return grabResource(entry, m_imageMap, createFunc, create);
}

GnmCombinedImageView GnmResourceFactory::grabRenderTarget(const GnmRenderTarget& desc, bool* create /*= nullptr*/)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.getBaseAddress();
	entry.size             = desc.getColorSizeAlign().m_size;
	auto createFunc        = [this, &desc]() { return createRenderTarget(desc); };
	return grabResource(entry, m_imageMap, createFunc, create);
}

GnmCombinedImageView GnmResourceFactory::grabDepthRenderTarget(const GnmDepthRenderTarget& desc, bool* create /*= nullptr*/)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.getZReadAddress();
	entry.size             = desc.getZSizeAlign().m_size;
	auto createFunc        = [this, &desc]() { return createDepthRenderTarget(desc); };
	return grabResource(entry, m_imageMap, createFunc, create);
}

RcPtr<VltSampler> GnmResourceFactory::grabSampler(const GnmSampler& desc, bool* create /*= nullptr*/)
{
	uint64_t         hash  = algo::MurmurHash(desc.m_regs, sizeof(desc.m_regs));
	GnmResourceEntry entry = {};
	entry.memory           = reinterpret_cast<const void*>(bit::extract(hash, 32, 63));
	entry.size             = static_cast<uint32_t>(bit::extract(hash, 0, 31));
	auto createFunc        = [this, &desc]() { return createSampler(desc); };
	return grabResource(entry, m_samplerMap, createFunc, create);
}

template <typename MapType>
typename MapType::mapped_type GnmResourceFactory::grabResource(
	const GnmResourceEntry&                        entry,
	MapType&                                       map,
	std::function<typename MapType::mapped_type()> createFunc,
	bool*                                          create)
{
	typename MapType::mapped_type resource = {};

	bool isNew = false;
	auto                          iter     = map.find(entry);
	if (iter != map.end())
	{
		resource = iter->second;
		isNew = false;
	}
	else
	{
		resource = createFunc();
		map.insert(std::make_pair(entry, resource));
		isNew = true;
	}

	if (create)
	{
		*create = isNew;
	}
	return resource;
}

void GnmResourceFactory::collectRenderTargets()
{
	// Insert
	// display buffer - swapchain image
	// pair to the image map.
	uint32_t numDisplayBuffer = m_device->videoOut->numDisplayBuffer();
	for (uint32_t i = 0; i != numDisplayBuffer; ++i)
	{
		SceDisplayBuffer bufferInfo = m_device->videoOut->getDisplayBuffer(i);
		GnmResourceEntry entry      = {};
		entry.memory                = bufferInfo.address;
		entry.size                  = bufferInfo.size;

		PresenterImage       swapImage    = m_device->presenter->getImage(i);
		GnmCombinedImageView renderTarget = {};
		renderTarget.image                = swapImage.image;
		renderTarget.view                 = swapImage.view;
		m_imageMap.insert(std::make_pair(entry, renderTarget));
	}
}

RcPtr<VltBuffer> GnmResourceFactory::createIndex(const GnmIndexBuffer& desc)
{
	VltBufferCreateInfo info = {};
	info.size                = desc.size;
	info.usage               = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	info.stages              = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
	info.access              = VK_ACCESS_INDEX_READ_BIT;

	return m_device->device->createBuffer(
		info,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

RcPtr<VltBuffer> GnmResourceFactory::createBuffer(const GnmBufferCreateInfo& desc)
{
	VkBufferUsageFlags usage  = {};
	VkAccessFlags      access = {};

	ShaderInputUsageType inputUsageType = static_cast<ShaderInputUsageType>(desc.usageType);
	switch (inputUsageType)
	{
	case pssl::kShaderInputUsageImmConstBuffer:
	{
		usage  = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		access = VK_ACCESS_UNIFORM_READ_BIT;
	}
	break;
	case pssl::kShaderInputUsageImmVertexBuffer:
	{
		usage  = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		access = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
	}
	break;
	case pssl::kShaderInputUsageImmRwResource:
	case pssl::kShaderInputUsageImmResource:
	default:
		LOG_ASSERT(false, "unsupported buffer usage type %d", inputUsageType);
		break;
	}

	VltBufferCreateInfo info = {};
	info.size                = desc.buffer->getSize();
	info.usage               = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	info.stages              = desc.stages;
	info.access              = access;

	return m_device->device->createBuffer(info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

GnmCombinedImageView GnmResourceFactory::createImage(const GnmTextureCreateInfo& desc)
{

	VkImageUsageFlags usage  = {};
	VkAccessFlags     access = {};
	VkImageLayout     layout = {};

	ShaderInputUsageType inputUsageType = static_cast<ShaderInputUsageType>(desc.usageType);
	switch (inputUsageType)
	{
	case pssl::kShaderInputUsageImmResource:
	{
		usage  = VK_IMAGE_USAGE_SAMPLED_BIT;
		access = VK_ACCESS_SHADER_READ_BIT;
		layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}
		break;
	case pssl::kShaderInputUsageImmRwResource:
	default:
		LOG_ASSERT(false, "unsupported texture usage type %d", inputUsageType);
		break;
	}

	VkFormat format = cvt::convertDataFormatToVkFormat(desc.texture->getDataFormat());

	// TODO:
	// Fill these details from desc.
	VltImageCreateInfo imgInfo = {};
	imgInfo.type               = VK_IMAGE_TYPE_2D;
	imgInfo.format             = format;
	imgInfo.flags              = 0;
	imgInfo.sampleCount        = VK_SAMPLE_COUNT_1_BIT;
	imgInfo.extent.width       = desc.texture->getWidth();
	imgInfo.extent.height      = desc.texture->getHeight();
	imgInfo.extent.depth       = desc.texture->getDepth();
	imgInfo.numLayers          = 1;
	imgInfo.mipLevels          = 1;
	imgInfo.usage              = usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	imgInfo.stages             = desc.stages;
	imgInfo.access             = access;
	imgInfo.tiling             = VK_IMAGE_TILING_OPTIMAL;
	imgInfo.layout             = layout;
	imgInfo.initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED;
	auto image                 = m_device->device->createImage(imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VltImageViewCreateInfo viewInfo = {};
	viewInfo.type                   = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format                 = imgInfo.format;
	viewInfo.usage                  = imgInfo.usage;
	viewInfo.aspect                 = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.numLayers              = imgInfo.numLayers;
	viewInfo.numLevels              = imgInfo.mipLevels;
	auto view                       = m_device->device->createImageView(image, viewInfo);

	GnmCombinedImageView imageView = {};
	imageView.image                = image;
	imageView.view                 = view;
	return imageView;
}

GnmCombinedImageView GnmResourceFactory::createRenderTarget(const GnmRenderTarget& desc)
{
	// TODO:
	// For future development, a game may use "render to texture" technique,
	// and set a render target which is not a swapchain image.
	// we should check whether "target" is the display buffer or not
	// and support extra render target,
	// but currently I just use the default one.
	LOG_FIXME("Not implemented.");
}

GnmCombinedImageView GnmResourceFactory::createDepthRenderTarget(const GnmDepthRenderTarget& desc)
{
	GnmCombinedImageView depthImage = {};
	do
	{
		ZFormat zfmt = desc.getZFormat();
		if (zfmt == kZFormatInvalid)
		{
			// kZFormatInvalid is an legal value,
			// it is used to disable Z buffer
			break;
		}

		VkFormat format = cvt::convertZFormatToVkFormat(zfmt);  // TODO: Should check format support
		if (format == VK_FORMAT_UNDEFINED)
		{
			LOG_ERR("unknown zformat %d", zfmt);
			break;
		}

		// TODO:
		// Fill these details from desc.
		VltImageCreateInfo imgInfo = {};
		imgInfo.type               = VK_IMAGE_TYPE_2D;
		imgInfo.format             = format;
		imgInfo.flags              = 0;
		imgInfo.sampleCount        = VK_SAMPLE_COUNT_1_BIT;
		imgInfo.extent.width       = desc.getWidth();
		imgInfo.extent.height      = desc.getHeight();
		imgInfo.extent.depth       = 1;
		imgInfo.numLayers          = 1;
		imgInfo.mipLevels          = 1;
		imgInfo.usage              = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imgInfo.stages             = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		imgInfo.access             = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		imgInfo.tiling             = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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

		depthImage.image = image;
		depthImage.view  = view;

	} while (false);
	return depthImage;
}

RcPtr<VltSampler> GnmResourceFactory::createSampler(const GnmSampler& desc)
{
	// TODO:
	// Set create info according to desc.
	VltSamplerCreateInfo info = {};
	info.magFilter            = VK_FILTER_LINEAR;
	info.minFilter            = VK_FILTER_LINEAR;
	info.mipmapMode           = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	info.useAnisotropy        = VK_TRUE;
	info.maxAnisotropy        = 16;
	info.addressModeU         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeV         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeW         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.compareToDepth       = VK_FALSE;
	info.compareOp            = VK_COMPARE_OP_ALWAYS;
	info.borderColor          = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	info.usePixelCoord        = VK_FALSE;
	return m_device->device->createSampler(info);
}
