#pragma once

#include "GnmCommon.h"
#include "UtilSync.h"

#include <unordered_map>
#include <memory>

namespace sce::vlt
{
	class VltDevice;
}  // namespace vlt

namespace sce::Gnm
{
	class GnmGpuLabel;

	class GnmLabelManager
	{
	public:
		GnmLabelManager(vlt::VltDevice* device);
		~GnmLabelManager();

		GnmGpuLabel* getLabel(void* labelAddress);

		void reset();

	private:
		vlt::VltDevice*                             m_device;
		std::unordered_map<void*, std::shared_ptr<Gnm::GnmGpuLabel>> m_labels;
		util::sync::Spinlock                        m_lock;
	};
}  // namespace sce