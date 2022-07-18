#include "SceResourceTracker.h"
#include "Violet/VltDevice.h"
#include "Violet/VltContext.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Sce.SceResourceTracker);

namespace sce
{

	SceResourceTracker::SceResourceTracker()
	{
	}

	SceResourceTracker::~SceResourceTracker()
	{
	}

	SceResource* SceResourceTracker::find(void* mem)
	{
		std::lock_guard<util::sync::Spinlock> guard(m_lock);

		SceResource* result = nullptr;
		auto iter = m_resources.lower_bound(mem);
		if (iter != m_resources.end())
		{
			// detect if mem is within a resource's memory range
			auto& res = iter->second;
			void*       start = res.cpuMemory();
			void*       end   = reinterpret_cast<uint8_t*>(start) + res.size();
			if (mem	>= start && mem < end)
			{
				result = &res;
			}
		}

		return result;
	}

	void SceResourceTracker::transform(VltContext* context)
	{
		for (auto& res : m_resources)
		{
			auto type      = res.second.type();
			auto transform = res.second.transform();

			if (transform.test(SceTransformFlag::GpuUpload))
			{
				Rc<VltImage> dstImage = nullptr;
				if (type.test(SceResourceType::Texture))
				{
					dstImage = res.second.texture().image;
				}
				else if (type.test(SceResourceType::RenderTarget))
				{
					dstImage = res.second.renderTarget().image;
				}
				else if (type.test(SceResourceType::DepthRenderTarget))
				{
					dstImage = res.second.depthRenderTarget().image;
				}

				VkExtent3D               imageExtent       = dstImage->mipLevelExtent(0);
				VkImageSubresourceLayers subresourceLayers = 
				{
					dstImage->formatInfo()->aspectMask, 0, 0, 1
				};

				auto& srcBuffer = res.second.buffer().buffer;
				context->copyBufferToImage(dstImage,
										   subresourceLayers,
										   VkOffset3D{ 0, 0, 0 },
										   imageExtent,
										   srcBuffer, 0, { 0u, 0u });
			}

			if (transform.test(SceTransformFlag::GpuDownload))
			{
				// TODO
			}

			res.second.clearTransform();
		}
	}

	void SceResourceTracker::download(vlt::VltContext* context)
	{
		for (auto& res : m_resources)
		{
			auto type      = res.second.type();
			auto transform = res.second.transform();

			if (type.test(SceResourceType::Buffer) &&
				!type.any(SceResourceType::RenderTarget,
						  SceResourceType::DepthRenderTarget))
			{
				auto& buffer   = res.second.buffer().buffer;
				void* data     = res.second.cpuMemory();
				auto  memFlags = buffer->memFlags();
				if (memFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				{
					if (!(memFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
					{
						// vkInvalidateMappedMemoryRanges()
					}
					std::memcpy(data, buffer->mapPtr(0), buffer->info().size);
				}
				else
				{
					context->downloadBuffer(buffer, data);
				}
			}
		}
	}

	void SceResourceTracker::reset()
	{
		std::lock_guard<util::sync::Spinlock> guard(m_lock);

		m_resources.clear();
	}

}  // namespace sce