#include "VltCmdList.h"
#include "VltDevice.h"


namespace sce::vlt
{
	VltCommandList::VltCommandList(VltDevice* device):
		m_device(device)
	{
	}

	VltCommandList::~VltCommandList()
	{
	}
}  // namespace sce::vlt