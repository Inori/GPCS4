#pragma once

#include "SceCommon.h"
#include "UtilFlag.h"

#include "Gnm/GnmBuffer.h"
#include "Gnm/GnmTexture.h"
#include "Gnm/GnmRenderTarget.h"
#include "Gnm/GnmDepthRenderTarget.h"

#include "Violet/VltRc.h"
#include "UtilSync.h"

#include <map>
#include <variant>

namespace sce
{
	namespace vlt
	{
		class VltBuffer;
		class VltBufferView;
		class VltImage;
		class VltImageView;
	}  // namespace vlt

	
	enum class SceResourceType
	{
		Buffer            = 0,
		Texture           = 1,
		RenderTarget      = 2,
		DepthRenderTarget = 3,
	};

	using SceResourceTypeFlags = util::Flags<SceResourceType>;


	struct SceBuffer
	{
		Gnm::Buffer                 gnmBuffer;
		vlt::Rc<vlt::VltBuffer>     buffer;
		vlt::Rc<vlt::VltBufferView> bufferView;

		void* cpuMemory() const;

		void* gpuMemory() const;

		size_t memorySize() const;
	};

	struct SceTexture
	{
		Gnm::Texture               texture;
		vlt::Rc<vlt::VltImage>     image;
		vlt::Rc<vlt::VltImageView> imageView;

		void* cpuMemory() const;

		void* gpuMemory() const;

		size_t memorySize() const;
	};

	// TODO:
	// Gnm::RenderTarget and Gnm::DepthRenderTarget
	// may contains multiple meta buffer together.
	// We should deal with them.

	struct SceRenderTarget
	{
		Gnm::RenderTarget          renderTarget;
		vlt::Rc<vlt::VltImage>     image;
		vlt::Rc<vlt::VltImageView> imageView;

		void* cpuMemory() const;

		void* gpuMemory() const;

		size_t memorySize() const;
	};

	struct SceDepthRenderTarget
	{
		Gnm::DepthRenderTarget     depthRenderTarget;
		vlt::Rc<vlt::VltImage>     image;
		vlt::Rc<vlt::VltImageView> imageView;

		void* cpuMemory() const;

		void* gpuMemory() const;

		size_t memorySize() const;
	};

	/**
	 * \brief Represent a Gnm resource with backing memory.
	 * 
	 * The resource can be different type 
	 * or proper combination of them.
	 * But must with same backing memory.
	 * 
	 */
	class SceResource
	{
	public:
		SceResource(const SceBuffer& buffer);
		SceResource(const SceTexture& texture);
		SceResource(const SceRenderTarget& renderTarget);
		SceResource(const SceDepthRenderTarget& depthRenderTarget);
		~SceResource();

		/**
		 * \brief Vulkan object backing memory
		 */
		void* gpuMemory() const
		{
			return m_gpuMemory;
		}

		/**
		 * \brief Emulated unified GPU memory
		 * 
		 * It's actually CPU memory allocate by our emulator.
		 */
		void* cpuMemory() const
		{
			return m_cpuMemory;
		}

		/**
		 * \brief Memory size
		 */
		size_t size() const
		{
			return m_memSize;
		}

		/**
		 * \brief Type flags
		 * 
		 * A resource can be both buffer and image
		 * at the same time with the same memory backed.
		 */
		SceResourceTypeFlags type() const
		{
			return m_type;
		}

		/**
		 * \brief Treat the resource as buffer
		 * 
		 * Only valid when SceResourceType::Buffer is set
		 */
		const SceBuffer& buffer() const
		{
			return m_buffer;
		}

		void setBuffer(const SceBuffer& buffer);

		/**
		 * \brief Treat the resource as Texture
		 * 
		 * Only valid when SceResourceType::Texture is set
		 */
		const SceTexture& texture() const
		{
			return m_texture;
		}

		void setTexture(const SceTexture& texture);

		/**
		 * \brief Treat the resource as RenderTarget
		 * 
		 * Only valid when SceResourceType::RenderTarget is set
		 */
		const SceRenderTarget& renderTarget() const
		{
			return std::get<SceRenderTarget>(m_target);
		}

		void setRenderTarget(const SceRenderTarget& renderTarget);

		/**
		 * \brief Treat the resource as DepthRenderTarget
		 * 
		 * Only valid when SceResourceType::DepthRenderTarget is set
		 */
		const SceDepthRenderTarget& depthRenderTarget() const
		{
			return std::get<SceDepthRenderTarget>(m_target);
		}

		void setDepthRenderTarget(const SceDepthRenderTarget& depthTarget);

	private:
		// vulkan memory
		void* m_gpuMemory = nullptr;
		// emulated GPU memory
		void* m_cpuMemory = nullptr;
		// memory size
		size_t m_memSize = 0;

		SceResourceTypeFlags m_type;

		SceBuffer  m_buffer;
		SceTexture m_texture;
		std::variant<SceRenderTarget, SceDepthRenderTarget> m_target;
	};


	/**
	 * \brief Global resource tracker.
	 * 
	 * Use to query vulkan object by Gnm resource memory.
	 * It's thread safe.
	 * 
	 */
	class SceResourceTracker
	{
		using SceResourceMap = std::map<void*, SceResource>;

	public:
		SceResourceTracker();
		~SceResourceTracker();

		/**
		 * \brief Track a sce resource type.
		 */
		template <class ResType>
		std::pair<SceResourceMap::iterator, bool>
		track(ResType&& arg)
		{
			std::lock_guard<util::sync::Spinlock> guard(m_lock);

			void* cpuMem = arg.cpuMemory();
			return m_resources.emplace(cpuMem, std::forward<ResType>(arg));
		}

		/**
		 * \brief Find resource object by memory pointer
		 * 
		 * The memory is not limited to the start address of a object,
		 * it can be any address 
		 * from start to end(not included) within the object memory.
		 */
		SceResource* find(void* mem);

		/**
		 * \brief Clear all information in the tracker
		 */
		void reset();
		
	private:
		util::sync::Spinlock m_lock;
		SceResourceMap       m_resources;
	};
}  // namespace sce