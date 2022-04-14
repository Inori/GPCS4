#include "VltSignal.h"

using namespace util;

namespace sce::vlt
{
	VltSignalTracker::VltSignalTracker()
	{
	}

	VltSignalTracker::~VltSignalTracker()
	{
	}

	void VltSignalTracker::add(const Rc<sync::Signal>& signal, uint64_t value)
	{
		m_signals.push_back({ signal, value });
	}

	void VltSignalTracker::notify()
	{
		for (const auto& pair : m_signals)
			pair.first->signal(pair.second);
	}

	void VltSignalTracker::reset()
	{
		m_signals.clear();
	}
}  // namespace sce::vlt