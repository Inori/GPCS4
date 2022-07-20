#include "GnmResourceFactory.h"
#include "Emulator.h"
#include "VirtualGPU.h"
#include "GnmConverter.h"
#include "GnmDepthRenderTarget.h"
#include "Sce/SceResourceTracker.h"
#include "Violet/VltDevice.h"
#include "Violet/VltBuffer.h"
#include "Violet/VltImage.h"
#include "Violet/VltSampler.h"
#include "fmt/format.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gnm.GnmResourceFactory);

namespace sce::Gnm
{
	std::atomic<size_t> GnmResourceFactory::s_objectId = 0;

	GnmResourceFactory::GnmResourceFactory(VltDevice* device) :
		m_device(device)
	{
	}

	GnmResourceFactory::~GnmResourceFactory()
	{
	}

	bool GnmResourceFactory::createDepthImage(
		const DepthRenderTarget* depthTarget,
		SceDepthRenderTarget&    depthImage)
	{
		VltImageCreateInfo imgInfo;
		imgInfo.type          = VK_IMAGE_TYPE_2D;
		imgInfo.format        = cvt::convertZFormat(depthTarget->getZFormat());
		imgInfo.flags         = 0;
		imgInfo.sampleCount   = cvt::convertNumFragments(depthTarget->getNumFragments());  // not really understand..
		imgInfo.extent.width  = depthTarget->getWidth();
		imgInfo.extent.height = depthTarget->getHeight();
		imgInfo.extent.depth  = 1;
		// NOTE: this slice count is only valid if the array view hasn't changed since initialization!
		imgInfo.numLayers     = depthTarget->getLastArraySliceIndex() - depthTarget->getBaseArraySliceIndex() + 1;
		imgInfo.mipLevels     = 1;

		imgInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
						VK_IMAGE_USAGE_TRANSFER_DST_BIT |
						VK_IMAGE_USAGE_SAMPLED_BIT;

		imgInfo.stages = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
						 VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT |
						 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		imgInfo.access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
						 VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout = optimizeLayout(imgInfo.usage);

		VltImageViewCreateInfo viewInfo;
		viewInfo.type      = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format    = imgInfo.format;
		viewInfo.usage     = imgInfo.usage;
		viewInfo.aspect    = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.minLevel  = 0;
		viewInfo.numLevels = 1;
		viewInfo.minLayer  = 0;
		viewInfo.numLayers = 1;
		if (imgInfo.format >= VK_FORMAT_D16_UNORM_S8_UINT)
		{
			viewInfo.aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}

		depthImage.image = m_device->createImage(
			imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		depthImage.imageView = m_device->createImageView(
			depthImage.image, viewInfo);
		depthImage.depthRenderTarget = *depthTarget;

		// Set debug name
		auto imageName = fmt::format("DepthTarget_{}_{}",
									 s_objectId++,
									 fmt::ptr(depthTarget->getZReadAddress()));
		auto viewName  = fmt::format("DepthView_{}_{}",
									 s_objectId++,
									 fmt::ptr(depthTarget->getZReadAddress()));
		
		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext        = nullptr;
		nameInfo.objectType   = VK_OBJECT_TYPE_IMAGE;
		nameInfo.pObjectName  = imageName.c_str();
		nameInfo.objectHandle = (uint64_t)depthImage.image->handle();
		setObjectName(&nameInfo);

		nameInfo.objectType   = VK_OBJECT_TYPE_IMAGE_VIEW;
		nameInfo.pObjectName  = viewName.c_str();
		nameInfo.objectHandle = (uint64_t)depthImage.imageView->handle();
		setObjectName(&nameInfo);

		return true;
	}
	
	bool GnmResourceFactory::createRenderTarget(
		const RenderTarget* target,
		SceRenderTarget&    targetImage)
	{

		VltImageCreateInfo imageInfo;
		imageInfo.type        = VK_IMAGE_TYPE_2D;
		imageInfo.format      = cvt::convertDataFormat(target->getDataFormat());
		imageInfo.flags       = 0;
		imageInfo.sampleCount = cvt::convertNumFragments(target->getNumFragments());  // Should we use getNumSamples?
		imageInfo.extent      = { target->getWidth(), target->getHeight(), 1 };
		imageInfo.numLayers   = 1;
		imageInfo.mipLevels   = 1;

		imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT |
						  VK_IMAGE_USAGE_STORAGE_BIT |
						  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
						  VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		imageInfo.stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
						   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
						   VK_PIPELINE_STAGE_TRANSFER_BIT;

		imageInfo.access = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
						   VK_ACCESS_SHADER_READ_BIT |
						   VK_ACCESS_TRANSFER_READ_BIT |
						   VK_ACCESS_TRANSFER_WRITE_BIT;

		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.layout = optimizeLayout(imageInfo.usage);

		VltImageViewCreateInfo viewInfo;
		viewInfo.type      = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format    = imageInfo.format;
		viewInfo.usage     = imageInfo.usage;
		viewInfo.aspect    = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.minLevel  = 0;
		viewInfo.numLevels = 1;
		viewInfo.minLayer  = 0;
		viewInfo.numLayers = 1;

		targetImage.image        = m_device->createImage(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		targetImage.imageView    = m_device->createImageView(targetImage.image, viewInfo);
		targetImage.renderTarget = *target;

		// Set debug name
		auto imageName = fmt::format("ColorTarget_{}_{}",
									 s_objectId++,
									 fmt::ptr(target->getBaseAddress()));
		auto viewName  = fmt::format("ColorView_{}_{}",
									 s_objectId++,
									 fmt::ptr(target->getBaseAddress()));

		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext        = nullptr;
		nameInfo.objectType   = VK_OBJECT_TYPE_IMAGE;
		nameInfo.pObjectName  = imageName.c_str();
		nameInfo.objectHandle = (uint64_t)targetImage.image->handle();
		setObjectName(&nameInfo);

		nameInfo.objectType   = VK_OBJECT_TYPE_IMAGE_VIEW;
		nameInfo.pObjectName  = viewName.c_str();
		nameInfo.objectHandle = (uint64_t)targetImage.imageView->handle();
		setObjectName(&nameInfo);

		return true;
	}

	bool GnmResourceFactory::createBuffer(
		const GnmBufferCreateInfo& createInfo,
		SceBuffer&                 sceBuffer)
	{
		auto                vsharp = createInfo.vsharp;
		VltBufferCreateInfo info   = {};
		info.size                  = vsharp->getSize();
		info.usage                 = createInfo.usage;
		info.stages                = createInfo.stage;
		info.access                = createInfo.access;

		sceBuffer.buffer     = m_device->createBuffer(info, createInfo.memoryType);
		sceBuffer.bufferView = nullptr;
		sceBuffer.gnmBuffer  = *vsharp;

		// Set debug name
		auto bufferName = fmt::format("Buffer_{}_{}",
									  s_objectId++,
									  fmt::ptr(vsharp->getBaseAddress()));

		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext        = nullptr;
		nameInfo.objectType   = VK_OBJECT_TYPE_BUFFER;
		nameInfo.pObjectName  = bufferName.c_str();
		nameInfo.objectHandle = (uint64_t)sceBuffer.buffer->getSliceHandle().handle;
		setObjectName(&nameInfo);

		return true;
	}

	bool GnmResourceFactory::createImage(
		const GnmImageCreateInfo& createInfo,
		SceTexture&               sceTexture)
	{
		auto        tsharp = createInfo.tsharp;
		TextureType textureType   = tsharp->getTextureType();

		auto tileMode = tsharp->getTileMode();

		VkImageCreateFlags flags;
		switch (textureType)
		{
			case kTextureType2dArray: flags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT; break;
			case kTextureTypeCubemap: flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT; break;
			default: flags = 0; break;
		}

		uint32_t mipLevelCount = tsharp->getLastMipLevel() -
								 tsharp->getBaseMipLevel() + 1;
		uint32_t sliceCount = tsharp->getLastArraySliceIndex() -
							  tsharp->getBaseArraySliceIndex() + 1;
		auto format = tsharp->getDataFormat();

		VltImageCreateInfo imageInfo;
		imageInfo.type        = cvt::convertTextureType(textureType);
		imageInfo.format      = cvt::convertDataFormat(format);
		imageInfo.flags       = flags;
		imageInfo.sampleCount = cvt::convertNumFragments(tsharp->getNumFragments());
		imageInfo.extent      = { tsharp->getWidth(), tsharp->getHeight(), tsharp->getDepth() };
		imageInfo.numLayers   = sliceCount;
		imageInfo.mipLevels   = mipLevelCount;
		imageInfo.usage       = createInfo.usage;
		imageInfo.stages      = createInfo.stage;
		imageInfo.access      = createInfo.access;
		imageInfo.tiling      = createInfo.tiling;
		imageInfo.layout      = optimizeLayout(imageInfo.usage);

		VltImageViewCreateInfo viewInfo;
		viewInfo.type      = cvt::convertTextureTypeView(textureType);
		viewInfo.format    = imageInfo.format;
		viewInfo.usage     = imageInfo.usage;
		viewInfo.aspect    = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.minLevel  = tsharp->getBaseMipLevel();
		viewInfo.numLevels = mipLevelCount;
		viewInfo.minLayer  = tsharp->getBaseArraySliceIndex();
		viewInfo.numLayers = sliceCount;
		viewInfo.swizzle   = calculateSwizzle(format);

		sceTexture.image     = m_device->createImage(imageInfo, createInfo.memoryType);
		sceTexture.imageView = m_device->createImageView(sceTexture.image, viewInfo);
		sceTexture.texture   = *tsharp;

		// Set debug name
		auto imageName = fmt::format("Texture_{}_{}",
									 s_objectId++,
									 fmt::ptr(tsharp->getBaseAddress()));
		auto viewName  = fmt::format("TextureView_{}_{}",
									 s_objectId++,
									 fmt::ptr(tsharp->getBaseAddress()));

		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext        = nullptr;
		nameInfo.objectType   = VK_OBJECT_TYPE_IMAGE;
		nameInfo.pObjectName  = imageName.c_str();
		nameInfo.objectHandle = (uint64_t)sceTexture.image->handle();
		setObjectName(&nameInfo);

		nameInfo.objectType   = VK_OBJECT_TYPE_IMAGE_VIEW;
		nameInfo.pObjectName  = viewName.c_str();
		nameInfo.objectHandle = (uint64_t)sceTexture.imageView->handle();
		setObjectName(&nameInfo);

		return true;
	}

	bool GnmResourceFactory::createSampler(
		const Sampler*  ssharp,
		Rc<VltSampler>& sampler)
	{
		sampler = m_samplerCache.getSampler(*ssharp);

		if (sampler == nullptr)
		{
			sampler = createSampler(ssharp);
			m_samplerCache.addSampler(*ssharp, sampler);
		}

		return true;
	}

	Rc<VltSampler> GnmResourceFactory::createSampler(const Sampler* ssharp)
	{
		DepthCompare depthComp = ssharp->getDepthCompareFunction();

		static const std::array<VkClearColorValue, 3> s_borderColors = { {
			{ { 0.0f, 0.0f, 0.0f, 0.0f } },
			{ { 0.0f, 0.0f, 0.0f, 1.0f } },
			{ { 1.0f, 1.0f, 1.0f, 1.0f } },
		} };

		VltSamplerCreateInfo samplerInfo;
		samplerInfo.magFilter      = cvt::convertFilterMode(ssharp->getMagFilterMode());
		samplerInfo.minFilter      = cvt::convertFilterMode(ssharp->getMinFilterMode());
		samplerInfo.mipmapMode     = cvt::convertMipFilterMode(ssharp->getMipFilterMode());
		samplerInfo.mipmapLodBias  = (float)ssharp->getLodBias();
		samplerInfo.mipmapLodMin   = (float)ssharp->getMinLod();
		samplerInfo.mipmapLodMax   = (float)ssharp->getMaxLod();
		samplerInfo.useAnisotropy  = ssharp->getAnisotropyRatio() != kAnisotropyRatio1;
		samplerInfo.maxAnisotropy  = (float)ssharp->getAnisotropyThreshold();
		samplerInfo.addressModeU   = cvt::convertWrapMode(ssharp->getWrapModeX());
		samplerInfo.addressModeV   = cvt::convertWrapMode(ssharp->getWrapModeY());
		samplerInfo.addressModeW   = cvt::convertWrapMode(ssharp->getWrapModeZ());
		samplerInfo.compareToDepth = VkBool32(depthComp != kDepthCompareNever);
		samplerInfo.compareOp      = cvt::convertDepthCompare(depthComp);
		samplerInfo.borderColor    = s_borderColors[ssharp->getBorderColor()];
		samplerInfo.usePixelCoord  = ssharp->getForceUnnormalized();

		Rc<VltSampler> result = m_device->createSampler(samplerInfo);

		// Set debug name
		auto samplerName = fmt::format("Sampler_{}", s_objectId++);

		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext        = nullptr;
		nameInfo.objectType   = VK_OBJECT_TYPE_SAMPLER;
		nameInfo.pObjectName  = samplerName.c_str();
		nameInfo.objectHandle = (uint64_t)result->handle();
		setObjectName(&nameInfo);

		return result;
	}

	VkImageLayout GnmResourceFactory::optimizeLayout(VkImageUsageFlags usage)
	{
		const VkImageUsageFlags usageFlags = usage;

		// Filter out unnecessary flags. Transfer operations
		// are handled by the backend in a transparent manner.
		usage &= ~(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
				   VK_IMAGE_USAGE_TRANSFER_SRC_BIT);

		// If the image is used only as an attachment, we never
		// have to transform the image back to a different layout
		if (usage == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		if (usage == VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		usage &= ~(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
				   VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		// If the image is used for reading but not as a storage
		// image, we can optimize the image for texture access
		if (usage == VK_IMAGE_USAGE_SAMPLED_BIT)
		{
			return usageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
					   ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
					   : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		// Otherwise, we have to stick with the default layout
		return VK_IMAGE_LAYOUT_GENERAL;
	}

	VkComponentMapping GnmResourceFactory::calculateSwizzle(DataFormat format)
	{
		VkComponentMapping result;

		auto selectComponent = [](TextureChannel channel)
		{
			switch (channel)
			{
				case Gnm::kTextureChannelConstant0: return VK_COMPONENT_SWIZZLE_ZERO;
				case Gnm::kTextureChannelConstant1: return VK_COMPONENT_SWIZZLE_ONE;
				case Gnm::kTextureChannelX: return VK_COMPONENT_SWIZZLE_R;
				case Gnm::kTextureChannelY: return VK_COMPONENT_SWIZZLE_G;
				case Gnm::kTextureChannelZ: return VK_COMPONENT_SWIZZLE_B;
				case Gnm::kTextureChannelW: return VK_COMPONENT_SWIZZLE_A;
			}
		};

		auto x   = static_cast<TextureChannel>(format.m_bits.m_channelX);
		result.r = x == Gnm::kTextureChannelX
					   ? VK_COMPONENT_SWIZZLE_IDENTITY
					   : selectComponent(x);

		auto y   = static_cast<TextureChannel>(format.m_bits.m_channelY);
		result.g = y == Gnm::kTextureChannelY
					   ? VK_COMPONENT_SWIZZLE_IDENTITY
					   : selectComponent(y);

		auto z   = static_cast<TextureChannel>(format.m_bits.m_channelZ);
		result.b = z == Gnm::kTextureChannelZ
					   ? VK_COMPONENT_SWIZZLE_IDENTITY
					   : selectComponent(z);

		auto w   = static_cast<TextureChannel>(format.m_bits.m_channelW);
		result.a = w == Gnm::kTextureChannelW
					   ? VK_COMPONENT_SWIZZLE_IDENTITY
					   : selectComponent(w);

		return result;
	}

	void GnmResourceFactory::setObjectName(VkDebugUtilsObjectNameInfoEXT* nameInfo)
	{
		m_device->vkd()->vkSetDebugUtilsObjectNameEXT(m_device->handle(), nameInfo);
	}

}  // namespace sce::Gnm