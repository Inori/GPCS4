#include "VltContext.h"

namespace sce::vlt
{
	VltContext::VltContext(VltDevice* device) :
		m_device(device)
	{
	}

	VltContext::~VltContext()
	{
	}

	void VltContext::beginRecording(const Rc<VltCommandList>& cmdList)
	{
		m_cmd = cmdList;
	}

	Rc<VltCommandList> VltContext::endRecording()
	{
		return std::exchange(m_cmd, nullptr);
	}

}  // namespace sce::vlt