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
}  // namespace sce::vlt

namespace sce::Gnm
{
	class Buffer;
	class Texture;

	class GnmInitializer
	{
		constexpr static size_t MaxTransferMemory   = 32 * 1024 * 1024;
		constexpr static size_t MaxTransferCommands = 512;

	public:
		GnmInitializer(vlt::VltDevice* device);
		~GnmInitializer();

		void flush();

		void initBuffer(
			vlt::Rc<vlt::VltBuffer> buffer,
			const Buffer*           vsharp);

		void initTexture(
			vlt::Rc<vlt::VltImage> image,
			const Texture*         tsharp);

	private:

		void initDeviceLocalBuffer(
			vlt::Rc<vlt::VltBuffer> buffer,
			const Buffer*           vsharp);

		void initHostVisibleBuffer(
			vlt::Rc<vlt::VltBuffer> buffer,
			const Buffer*           vsharp);

		void initDeviceLocalTexture(
			vlt::Rc<vlt::VltImage> image,
			const Texture*         tsharp);

		void initHostVisibleTexture(
			vlt::Rc<vlt::VltImage> image,
			const Texture*         tsharp);

		void flushImplicit();
		void flushInternal();

	private:
		std::mutex m_mutex;

		vlt::VltDevice* m_device;
		vlt::Rc<vlt::VltContext> m_context;

		size_t m_transferCommands = 0;
		size_t m_transferMemory   = 0;
	};


}  // namespace sce::Gnm