#include "SceResource.h"

#include "Violet/VltBuffer.h"
#include "Violet/VltImage.h"
#include "Violet/VltHash.h"

using namespace sce::vlt;

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
	}

	SceResource::SceResource(const SceTexture& texture) :
		m_type(SceResourceType::Texture),
		m_texture(texture)
	{
		m_cpuMemory = texture.cpuMemory();
		m_gpuMemory = texture.gpuMemory();

		m_memSize = texture.memorySize();
	}

	SceResource::SceResource(const SceRenderTarget& renderTarget) :
		m_type(SceResourceType::RenderTarget),
		m_target(renderTarget)
	{
		m_cpuMemory = renderTarget.cpuMemory();
		m_gpuMemory = renderTarget.gpuMemory();

		m_memSize = renderTarget.memorySize();
	}

	SceResource::SceResource(const SceDepthRenderTarget& depthRenderTarget) :
		m_type(SceResourceType::DepthRenderTarget),
		m_target(depthRenderTarget)
	{
		m_cpuMemory = depthRenderTarget.cpuMemory();
		m_gpuMemory = depthRenderTarget.gpuMemory();

		m_memSize = depthRenderTarget.memorySize();

		// we do not check depth image size.
		// because the corresponding vulkan image must be created
		// with VK_IMAGE_TILING_OPTIMAL for a depth format.
		// the image is in optimized format so that image size is not width * height * bytesPerPixel
		// in such case the size of a gnm depth image and vulkan depth image will not match
		// we assume the game won't access the depth image for CPU side.
	}

	SceResource::~SceResource()
	{
	}

	void SceResource::setBuffer(const SceBuffer& buffer)
	{
		m_buffer = buffer;
		m_type.set(SceResourceType::Buffer);
	}

	void SceResource::setTexture(const SceTexture& texture)
	{
		m_texture = texture;
		m_type.set(SceResourceType::Texture);
	}

	void SceResource::setRenderTarget(const SceRenderTarget& renderTarget)
	{
		m_target = renderTarget;
		m_type.set(SceResourceType::RenderTarget);
	}

	void SceResource::setDepthRenderTarget(const SceDepthRenderTarget& depthTarget)
	{
		m_target = depthTarget;
		m_type.set(SceResourceType::DepthRenderTarget);
	}

}  // namespace sce