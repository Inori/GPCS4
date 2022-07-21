#pragma once

#include "GnmCommon.h"

#include "Violet/VltRc.h"

#include <mutex>

namespace sce::vlt
{
	class VltDevice;
	class VltContext;
	class VltBuffer;
	class VltImage;
	enum class VltQueueType : uint32_t;
}  // namespace sce::vlt

namespace sce::GpuAddress
{
	class TilingParameters;
}  // namespace sce::GpuAddress

namespace sce::Gnm
{
	class Buffer;
	class Texture;

	class GnmInitializer
	{
		constexpr static size_t MaxTransferMemory   = 32 * 1024 * 1024;
		constexpr static size_t MaxTransferCommands = 512;

	public:
		GnmInitializer(
			vlt::VltDevice*   device,
			vlt::VltQueueType queueType);
		~GnmInitializer();

		void flush();

		void initBuffer(
			const vlt::Rc<vlt::VltBuffer>& buffer,
			const Buffer*                  vsharp);

		void initTexture(
			const vlt::Rc<vlt::VltImage>& image,
			const Texture*                tsharp);

	private:
		void initDeviceLocalBuffer(
			const vlt::Rc<vlt::VltBuffer>& buffer,
			const Buffer*                  vsharp);

		void initHostVisibleBuffer(
			const vlt::Rc<vlt::VltBuffer>& buffer,
			const Buffer*                  vsharp);

		void initDeviceLocalTexture(
			const vlt::Rc<vlt::VltImage>& image,
			const Texture*                tsharp);

		void initHostVisibleTexture(
			const vlt::Rc<vlt::VltImage>& image,
			const Texture*                tsharp);

		void computeUntiledSize(
			uint32_t*                           outWidth,
			uint32_t*                           outHeight,
			const GpuAddress::TilingParameters* tp);

		void flushImplicit();
		void flushInternal();

	private:
		std::mutex m_mutex;

		vlt::VltDevice*          m_device;
		vlt::Rc<vlt::VltContext> m_context;

		size_t m_transferCommands = 0;
		size_t m_transferMemory   = 0;
	};

}  // namespace sce::Gnm