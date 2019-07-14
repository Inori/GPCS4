#include "sce_gnmdriver.h"
#include "Graphic/GraphicShared.h"
#include "Graphic/SceVideoOut.h"
#include "Graphic/SceGnmDriver.h"



int PS4API sceGnmAreSubmitsAllowed(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitAndFlipCommandBuffers(uint32_t count,
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
	uint32_t videoOutHandle, uint32_t displayBufferIndex,
	uint32_t flipMode, int64_t flipArg)
{
	LOG_SCE_GRAPHIC("displayBuffIdx %d", displayBufferIndex);
	
	return SCE_OK;
}


int PS4API sceGnmSubmitAndFlipCommandBuffersForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitCommandBuffers(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitCommandBuffersForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitDone(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

