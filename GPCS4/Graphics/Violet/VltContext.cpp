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
	}

	Rc<VltCommandList> VltContext::endRecording()
	{
	}

}  // namespace sce::vlt