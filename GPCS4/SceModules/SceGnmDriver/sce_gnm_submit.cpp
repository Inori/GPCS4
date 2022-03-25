#include "sce_gnmdriver.h"
#include "Emulator/Emulator.h"
#include "VirtualGPU.h"
#include "Sce/SceGnmDriver.h"
#include "Sce/SceVideoOut.h"

LOG_CHANNEL(SceModules.SceDriver.GnmSubmit);

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
	auto& gnmDriver = GPU().gnmDriver();
	return gnmDriver.submitAndFlipCommandBuffers(count,
		dcbGpuAddrs, dcbSizesInBytes,
		ccbGpuAddrs, ccbSizesInBytes,
		videoOutHandle, displayBufferIndex,
		flipMode, flipArg);
}


int PS4API sceGnmSubmitAndFlipCommandBuffersForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes, 
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes)
{
	LOG_SCE_GRAPHIC("count %d", count);
	auto& gnmDriver = GPU().gnmDriver();
	return gnmDriver.submitCommandBuffers(count,
		dcbGpuAddrs, dcbSizesInBytes,
		ccbGpuAddrs, ccbSizesInBytes);
}


int PS4API sceGnmSubmitCommandBuffersForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitDone(void)
{
	LOG_SCE_GRAPHIC("");
	auto& gnmDriver = GPU().gnmDriver();
	return gnmDriver.sceGnmSubmitDone();
}

