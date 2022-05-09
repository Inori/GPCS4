#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "Violet/VltRc.h"

#include <unordered_map>
#include <future>

namespace sce::vlt
{
	class VltDevice;
	class VltContext;
	class VltSemaphore;
}  // namespace sce::vlt

namespace sce::Gnm
{
    class GnmGpuLabel
	{
	public:
		GnmGpuLabel(
			vlt::VltDevice* device,
			void*           label);
		~GnmGpuLabel();

		void set(uint64_t value);

		void write(
			vlt::VltContext*      context,
			VkPipelineStageFlags2 stage,
			EventWriteSource      srcSelector,
			uint64_t              immValue);

		void writeWithInterrupt(
			vlt::VltContext*      context,
			VkPipelineStageFlags2 stage,
			EventWriteSource      srcSelector,
			uint64_t              immValue);

		void wait(
			vlt::VltContext* context,
			uint32_t         mask,
			WaitCompareFunc  compareFunc,
			uint32_t         refValue);
		
	private:
		vlt::VltDevice* m_device;
		void*           m_label;

		vlt::Rc<vlt::VltSemaphore> m_semaphore;
		std::future<void>          m_future;
	};

}  // namespace sce::Gnm