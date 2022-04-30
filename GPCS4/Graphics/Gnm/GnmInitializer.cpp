#include "GnmInitializer.h"
#include "GnmBuffer.h"
#include "GnmTexture.h"
#include "Violet/VltDevice.h"
#include "Violet/VltContext.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gnm.GnmInitializer);

namespace sce::Gnm
{
	GnmInitializer::GnmInitializer(vlt::VltDevice* device):
		m_device(device),
		m_context(m_device->createContext())
	{
		m_context->beginRecording(
			m_device->createCommandList());
	}

	GnmInitializer::~GnmInitializer()
	{
	}

	void GnmInitializer::flush()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_transferCommands != 0)
			flushInternal();
	}

	void GnmInitializer::initBuffer(Rc<VltBuffer> buffer, const Buffer* vsharp)
	{
		VkMemoryPropertyFlags memFlags = buffer->memFlags();

		(memFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			? initHostVisibleBuffer(buffer, vsharp)
			: initDeviceLocalBuffer(buffer, vsharp);
	}

	void GnmInitializer::initTexture(Rc<VltImage> image, const Texture* tsharp)
	{
		VkMemoryPropertyFlags memFlags = image->memFlags();
		(memFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			? initHostVisibleTexture(image, tsharp)
			: initDeviceLocalTexture(image, tsharp);
	}

	void GnmInitializer::initDeviceLocalBuffer(
		vlt::Rc<vlt::VltBuffer> buffer, const Buffer* vsharp)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		VltBufferSlice bufferSlice(buffer);

		m_transferMemory += bufferSlice.length();
		m_transferCommands += 1;

		m_context->uploadBuffer(
			bufferSlice.buffer(),
			vsharp->getBaseAddress());

		flushImplicit();
	}

	void GnmInitializer::initHostVisibleBuffer(
		vlt::Rc<vlt::VltBuffer> buffer, const Buffer* vsharp)
	{
		// If the buffer is mapped, we can write data directly
		// to the mapped memory region instead of doing it on
		// the GPU. Same goes for zero-initialization.
		VltBufferSlice bufferSlice(buffer);

		if (vsharp != nullptr && vsharp->getBaseAddress())
		{
			std::memcpy(
				bufferSlice.mapPtr(0),
				vsharp->getBaseAddress(),
				vsharp->getSize());
		}
		else
		{
			std::memset(
				bufferSlice.mapPtr(0), 0,
				bufferSlice.length());
		}
	}

	void GnmInitializer::initDeviceLocalTexture(
		vlt::Rc<vlt::VltImage> image, const Texture* tsharp)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		auto           formatInfo      = imageFormatInfo(image->info().format);
		uint32_t       bytesPerElement = tsharp->getDataFormat().getBytesPerElement();
		const uint8_t* textureMem      = reinterpret_cast<uint8_t*>(tsharp->getBaseAddress());
		
		for (uint32_t layer = 0; layer < image->info().numLayers; layer++)
		{
			for (uint32_t level = 0; level < image->info().mipLevels; level++)
			{
				VkImageSubresourceLayers subresourceLayers;
				subresourceLayers.aspectMask     = formatInfo->aspectMask;
				subresourceLayers.mipLevel       = level;
				subresourceLayers.baseArrayLayer = layer;
				subresourceLayers.layerCount     = 1;

				GpuAddress::TilingParameters params;
				params.initFromTexture(tsharp, level, layer);
				GpuAddress::SurfaceInfo surfaceInfo;
				GpuAddress::computeSurfaceInfo(&surfaceInfo, &params);

				VkOffset3D mipLevelOffset = { 0, 0, 0 };
				VkExtent3D mipLevelExtent = image->mipLevelExtent(level);

				m_transferCommands += 1;
				m_transferMemory += vutil::computeImageDataSize(
					image->info().format, mipLevelExtent);

				uint32_t pitchInBytes = surfaceInfo.m_pitch * bytesPerElement;
				if (formatInfo->aspectMask != (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
				{
					uint64_t surfaceOffset = 0;
					uint64_t surfaceSize   = 0;
					GpuAddress::computeTextureSurfaceOffsetAndSize(&surfaceOffset, &surfaceSize, tsharp, level, layer);
					const void* memory = textureMem + surfaceOffset;
					m_context->uploadImage(
						image, subresourceLayers,
						memory,
						pitchInBytes,
						surfaceInfo.m_surfaceSize);
				}
				else
				{
					LOG_ASSERT(false, "TODO");
					//m_context->updateDepthStencilImage(
					//	image, subresourceLayers,
					//	VkOffset2D{ mipLevelOffset.x, mipLevelOffset.y },
					//	VkExtent2D{ mipLevelExtent.width, mipLevelExtent.height },
					//	pInitialData[id].pSysMem,
					//	pInitialData[id].SysMemPitch,
					//	pInitialData[id].SysMemSlicePitch,
					//	packedFormat);
				}
			}
		}

		flushImplicit();
	}

	void GnmInitializer::initHostVisibleTexture(
		vlt::Rc<vlt::VltImage> image, const Texture* tsharp)
	{
		LOG_ASSERT(false, "TODO:");
	}

	void GnmInitializer::flushImplicit()
	{
		if (m_transferCommands > MaxTransferCommands || 
			m_transferMemory > MaxTransferMemory)
			flushInternal();
	}

	void GnmInitializer::flushInternal()
	{
		m_context->flushCommandList();

		m_transferCommands = 0;
		m_transferMemory   = 0;
	}

}  // namespace sce::Gnm