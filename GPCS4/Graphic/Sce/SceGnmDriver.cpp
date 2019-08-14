#include "SceGnmDriver.h"
#include "sce_errors.h"


namespace sce
{;

SceGnmDriver::SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut):
	m_videoOut(videoOut)
{
	m_cb = std::make_shared<GnmCommandBufferDraw>(m_videoOut);
	m_cmdParser = std::make_shared<GnmCmdStream>(m_cb);
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

		uint32_t* cmdBuff = (uint32_t*)dcbGpuAddrs[0];
		uint32_t cmdSize = dcbSizesInBytes[0];
		if (!m_cmdParser->processCommandBuffer(cmdBuff, cmdSize))
		{
			break;
		}

		m_videoOut->flip(displayBufferIndex);

		err = SCE_OK;
	} while (false);
	return err;
}

int SceGnmDriver::sceGnmSubmitDone(void)
{
	return SCE_OK;
}

}  //sce