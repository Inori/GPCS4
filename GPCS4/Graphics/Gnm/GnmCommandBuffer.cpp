#include "GnmCommandBuffer.h"
#include "PlatProcess.h"

namespace sce::Gnm
{

GnmCommandBuffer::GnmCommandBuffer()
{
}

GnmCommandBuffer::~GnmCommandBuffer()
{
}


void GnmCommandBuffer::emuWriteGpuLabel(EventWriteSource selector, void* label, uint64_t value)
{
	do 
	{
		if (!label)
		{
			break;
		}

		if (selector == kEventWriteSource32BitsImmediate)
		{
			*(uint32_t*)label = value;
		}
		else if (selector == kEventWriteSource64BitsImmediate)
		{
			*(uint64_t*)label = value;
		}
		else
		{
			*(uint64_t*)label = pprocess::GetProcessTimeCounter();
		}

	} while (false);
}

}  // namespace sce::Gnm