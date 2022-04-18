#include "GnmResourceFactory.h"
#include "Emulator.h"
#include "VirtualGPU.h"
#include "GnmConverter.h"
#include "GnmDepthRenderTarget.h"
#include "Sce/SceResourceTracker.h"
#include "Violet/VltDevice.h"
#include "Violet/VltBuffer.h"
#include "Violet/VltImage.h"

using namespace sce::vlt;

namespace sce::Gnm
{
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
		imgInfo.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imgInfo.stages        = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		imgInfo.access        = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		imgInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout        = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

		VltImageViewCreateInfo viewInfo;
		viewInfo.type      = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format    = imgInfo.format;
		viewInfo.usage     = imgInfo.usage;
		viewInfo.aspect    = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.minLevel  = 0;
		viewInfo.numLevels = 1;
		viewInfo.minLayer  = 0;
		viewInfo.numLayers = 1;

		VltBufferCreateInfo info;
		info.size   = depthTarget->getZSizeAlign().m_size;
		info.usage  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		info.stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
		info.access = VK_ACCESS_TRANSFER_READ_BIT;

		depthImage.image = m_device->createImage(
			imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		depthImage.imageView = m_device->createImageView(
			depthImage.image, viewInfo);
		depthImage.depthRenderTarget = *depthTarget;

		return true;
	}

	bool GnmResourceFactory::createBuffer(
		const vlt::VltBufferCreateInfo& createinfo,
		VkMemoryPropertyFlags           memoryType,
		const Buffer*                   buffer,
		SceBuffer&                      sceBuffer)
	{
		sceBuffer.buffer     = m_device->createBuffer(createinfo, memoryType);
		sceBuffer.bufferView = nullptr;
		sceBuffer.gnmBuffer  = *buffer;
		return true;
	}

}  // namespace sce::Gnm