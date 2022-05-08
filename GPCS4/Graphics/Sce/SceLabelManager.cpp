#include "SceLabelManager.h"

#include "Gnm/GnmGpuLabel.h"
#include "Violet/VltDevice.h"

#include <mutex>

LOG_CHANNEL(Graphic.Gnm.SceLabelManager);

namespace sce
{
	using namespace Gnm;

	SceLabelManager::SceLabelManager(vlt::VltDevice* device) :
		m_device(device)
	{
	}

	SceLabelManager::~SceLabelManager()
	{
	}

	GnmGpuLabel* SceLabelManager::getLabel(void* labelAddress)
	{
		LOG_ASSERT(labelAddress != nullptr, "null label address passed.");

		std::lock_guard<util::sync::Spinlock> guard(m_lock);
		GnmGpuLabel* label = nullptr;

		auto iter = m_labels.find(labelAddress);
		if (iter == m_labels.end())
		{
			auto pair = m_labels.emplace(std::piecewise_construct,
										 std::tuple(labelAddress),
										 std::tuple(m_device, labelAddress));
			label = &pair.first->second;
		}
		else 
		{
			label = &iter->second;
		}
		return label;
	}
}  // namespace sce