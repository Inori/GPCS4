#pragma once

#include "SceCommon.h"
#include "UtilSync.h"

#include <unordered_map>

namespace sce
{
	namespace vlt
	{
		class VltDevice;
	}  // namespace vlt

	namespace Gnm
	{
		class GnmGpuLabel;
	}  // namespace Gnm

	class SceLabelManager
	{
	public:
		SceLabelManager(vlt::VltDevice* device);
		~SceLabelManager();

		Gnm::GnmGpuLabel* getLabel(void* labelAddress);

		void reset();

	private:
		vlt::VltDevice*                             m_device;
		std::unordered_map<void*, Gnm::GnmGpuLabel> m_labels;
		util::sync::Spinlock                        m_lock;
	};
}  // namespace sce