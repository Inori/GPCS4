#include "GnmInitializer.h"

#include "GnmBuffer.h"
#include "GnmTexture.h"

#include "Violet/VltDevice.h"
#include "Violet/VltContext.h"
#include "PlatMemory.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gnm.GnmInitializer);

namespace sce::Gnm
{
	GnmInitializer::GnmInitializer(vlt::VltDevice* device, vlt::VltQueueType queueType) :
		m_device(device),
		m_context(m_device->createContext())
	{
		m_context->beginRecording(
			m_device->createCommandList(queueType));
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

	void GnmInitializer::initBuffer(
		const Rc<VltBuffer>& buffer, const Buffer* vsharp)
	{
		VkMemoryPropertyFlags memFlags = buffer->memFlags();

		(memFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			? initHostVisibleBuffer(buffer, vsharp)
			: initDeviceLocalBuffer(buffer, vsharp);
	}

	void GnmInitializer::initTexture(
		const Rc<VltImage>& image, const Texture* tsharp)
	{
		VkMemoryPropertyFlags memFlags = image->memFlags();
		(memFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			? initHostVisibleTexture(image, tsharp)
			: initDeviceLocalTexture(image, tsharp);
	}

	void GnmInitializer::initDeviceLocalBuffer(
		const Rc<VltBuffer>& buffer, const Buffer* vsharp)
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
		const Rc<VltBuffer>& buffer, const Buffer* vsharp)
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
		const Rc<VltImage>& image, const Texture* tsharp)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		auto     formatInfo      = imageFormatInfo(image->info().format);
		auto     textureFormat   = tsharp->getDataFormat();
		uint32_t bytesPerElement = textureFormat.getTotalBytesPerElement();
		bool     isCompressed    = textureFormat.isBlockCompressedFormat();
		auto     tileMode        = tsharp->getTileMode();
		uint8_t* textureMem      = reinterpret_cast<uint8_t*>(tsharp->getBaseAddress());

		if (image->info().extent.width == 1024 && image->info().extent.height == 256)
		{
			__debugbreak();
		}

		bool isTiled = tileMode != kTileModeDisplay_LinearAligned &&
					   tileMode != kTileModeDisplay_LinearGeneral;

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

				uint32_t pitch = 0;
				uint32_t height = 0;
				if (isTiled)
				{
					computeUntiledSize(&pitch, &height, &params);
				}
				else
				{
					pitch  = tsharp->getPitch() >> level;
					height = tsharp->getHeight() >> level;
				}

				uint32_t elementPerRow = isCompressed ? pitch / 4 : pitch;
				uint32_t pitchPerRow   = elementPerRow * bytesPerElement;
				uint32_t pitchPerLayer = pitchPerRow * height;

				uint64_t surfaceOffset = 0;
				uint64_t surfaceSize   = 0;
				GpuAddress::computeTextureSurfaceOffsetAndSize(
					&surfaceOffset, &surfaceSize, tsharp, level, layer);
				void* memory = textureMem + surfaceOffset;

				if (isTiled)
				{
					// allocate enough memory
					uint64_t      untiledSize = 0;
					AlignmentType align       = 0;
					GpuAddress::computeUntiledSurfaceSize(&untiledSize, &align, &params);
					void* untiled = plat::aligned_malloc(align, untiledSize);
					// detail surface
					// TODO:
					// Should be done on GPU using compute shader
					detileSurface(untiled, memory, &params);
					memory = untiled;
				}

				if (formatInfo->aspectMask != (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
				{
					m_context->uploadImage(
						image, subresourceLayers,
						memory,
						pitchPerRow,
						pitchPerLayer);
				}
				else
				{
					LOG_ASSERT(false, "TODO");
					// m_context->updateDepthStencilImage(
					//	image, subresourceLayers,
					//	VkOffset2D{ mipLevelOffset.x, mipLevelOffset.y },
					//	VkExtent2D{ mipLevelExtent.width, mipLevelExtent.height },
					//	pInitialData[id].pSysMem,
					//	pInitialData[id].SysMemPitch,
					//	pInitialData[id].SysMemSlicePitch,
					//	packedFormat);
				}

				if (isTiled)
				{
					plat::aligned_free(memory);
				}
				
				m_transferCommands += 1;
				m_transferMemory += pitchPerLayer;
			}
		}

		flushImplicit();
	}

	void GnmInitializer::initHostVisibleTexture(
		const Rc<VltImage>& image, const Texture* tsharp)
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

	void GnmInitializer::computeUntiledSize(
		uint32_t*                           outWidth,
		uint32_t*                           outHeight,
		const GpuAddress::TilingParameters* tp)
	{
		uint32_t paddedWidth  = tp->m_linearWidth;
		uint32_t paddedHeight = tp->m_linearHeight;
		if (tp->m_isBlockCompressed)
		{
			switch (tp->m_bitsPerFragment)
			{
				case 1:
					paddedWidth = std::max((paddedWidth + 7ULL) / 8ULL, 1ULL);
					break;
				case 4:
				case 8:
					paddedWidth  = std::max((paddedWidth + 3ULL) / 4ULL, 1ULL);
					paddedHeight = std::max((paddedHeight + 3ULL) / 4ULL, 1ULL);
					break;
				case 16:
					// TODO
					break;
				default:
					LOG_ASSERT(!tp->m_isBlockCompressed, "Unknown bit depth for block-compressed format");
					break;
			}
		}

		if (outWidth)
		{
			*outWidth = paddedWidth;
		}
		if (outHeight)
		{
			*outHeight = paddedHeight;
		}
	}

}  // namespace sce::Gnm