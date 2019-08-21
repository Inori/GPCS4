#include "SceGnmDriver.h"
#include "sce_errors.h"


namespace sce
{;

SceGnmDriver::SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut):
	m_videoOut(videoOut)
{
}

SceGnmDriver::~SceGnmDriver()
{
}

int SceGnmDriver::submitAndFlipCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
	uint32_t videoOutHandle, uint32_t displayBufferIndex, 
	uint32_t flipMode, int64_t flipArg)
{
	int err = SCE_GNM_ERROR_UNKNOWN;
	do 
	{
		if (count != 1)
		{
			LOG_FIXME("Currently only support only 1 cmdbuff.");
			break;
		}

	
		err = SCE_OK;
	} while (false);
	return err;
}

int SceGnmDriver::sceGnmSubmitDone(void)
{
	return SCE_OK;
}

}  //sce