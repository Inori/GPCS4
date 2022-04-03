#include "SceResourceBank.h"
#include "Violet/VltBuffer.h"
#include "Violet/VltImage.h"

LOG_CHANNEL(Graphic.Sce.SceResourceBank);

namespace sce
{

	void* SceBuffer::cpuMemory() const
	{
		return this->gnmBuffer.getBaseAddress();
	}

	void* SceBuffer::gpuMemory() const
	{
		return this->buffer->mapPtr(0);
	}

	size_t SceBuffer::memorySize() const
	{
		return this->gnmBuffer.getSize();
	}

	void* SceTexture::cpuMemory() const
	{
		return this->texture.getBaseAddress();
	}

	void* SceTexture::gpuMemory() const
	{
		return this->image->mapPtr(0);
	}

	size_t SceTexture::memorySize() const
	{
		return this->texture.getSizeAlign().m_size;
	}

	void* SceRenderTarget::cpuMemory() const
	{
		return this->renderTarget.getBaseAddress();
	}

	void* SceRenderTarget::gpuMemory() const
	{
		return this->image->mapPtr(0);
	}

	size_t SceRenderTarget::memorySize() const
	{
		return this->renderTarget.getColorSizeAlign().m_size;
	}

	void* SceDepthRenderTarget::cpuMemory() const
	{
		return this->depthRenderTarget.getZReadAddress();
	}

	void* SceDepthRenderTarget::gpuMemory() const
	{
		return this->image->mapPtr(0);
	}

	size_t SceDepthRenderTarget::memorySize() const
	{
		return this->depthRenderTarget.getZSizeAlign().m_size;
	}

	/////////////////////////////////////////////////////////////////////

	SceResource::SceResource(const SceBuffer& buffer) :
		m_type(SceResourceType::Buffer),
		m_buffer(buffer)
	{
		m_cpuMemory = buffer.cpuMemory();
		m_gpuMemory = buffer.gpuMemory();

		m_memSize = buffer.memorySize();
		LOG_ASSERT(m_memSize == buffer.buffer->memSize(), "Gnm size not match vlt size");
	}

	SceResource::SceResource(const SceTexture& texture):
		m_type(SceResourceType::Texture),
		m_texture(texture)
	{
		m_cpuMemory = texture.cpuMemory();
		m_gpuMemory = texture.gpuMemory();

		m_memSize = texture.memorySize();
		LOG_ASSERT(m_memSize == texture.image->memSize(), "Gnm size not match vlt size");
	}

	SceResource::SceResource(const SceRenderTarget& renderTarget):
		m_type(SceResourceType::RenderTarget),
		m_target(renderTarget)
	{
		m_cpuMemory = renderTarget.cpuMemory();
		m_gpuMemory = renderTarget.gpuMemory();

		m_memSize = renderTarget.memorySize();
		LOG_ASSERT(m_memSize == renderTarget.image->memSize(), "Gnm size not match vlt size");
	}

	SceResource::SceResource(const SceDepthRenderTarget& depthRenderTarget):
		m_type(SceResourceType::DepthRenderTarget),
		m_target(depthRenderTarget)
	{
		m_cpuMemory = depthRenderTarget.cpuMemory();
		m_gpuMemory = depthRenderTarget.gpuMemory();

		m_memSize = depthRenderTarget.memorySize();
		LOG_ASSERT(m_memSize == depthRenderTarget.image->memSize(), "Gnm size not match vlt size");
	}

	SceResource::~SceResource()
	{
	}

	//////////////////////////////////////////////////////////////////////////

	SceResourceBank::SceResourceBank()
	{
	}

	SceResourceBank::~SceResourceBank()
	{
	}

	SceResource* SceResourceBank::find(void* mem)
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

	void SceResourceBank::clear()
	{
		std::lock_guard<util::sync::Spinlock> guard(m_lock);

		m_resources.clear();
	}

}  // namespace sce