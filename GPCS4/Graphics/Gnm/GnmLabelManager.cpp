#include "GnmLabelManager.h"
#include "GnmGpuLabel.h"
#include "Violet/VltDevice.h"

#include <mutex>

LOG_CHANNEL(Graphic.Gnm.SceLabelManager);

namespace sce::Gnm
{
	GnmLabelManager::GnmLabelManager(vlt::VltDevice* device) :
		m_device(device)
	{
	}

	GnmLabelManager::~GnmLabelManager()
	{
	}

	GnmGpuLabel* GnmLabelManager::getLabel(void* labelAddress)
	{
		LOG_ASSERT(labelAddress != nullptr, "null label address passed.");

		std::lock_guard<util::sync::Spinlock> guard(m_lock);
		GnmGpuLabel* label = nullptr;

		auto iter = m_labels.find(labelAddress);
		if (iter == m_labels.end())
		{
			// auto pair = m_labels.emplace(std::piecewise_construct,
			// 							 std::tuple(labelAddress),
			// 							 std::tuple(m_device, labelAddress));
			auto pair = m_labels.emplace(labelAddress,
										 std::make_shared<Gnm::GnmGpuLabel>(m_device, labelAddress));
			label = pair.first->second.get();
		}
		else 
		{
			label = iter->second.get();
		}
		return label;
	}

	void GnmLabelManager::reset()
	{
		std::lock_guard<util::sync::Spinlock> guard(m_lock);
		m_labels.clear();
	}

}  // namespace sce