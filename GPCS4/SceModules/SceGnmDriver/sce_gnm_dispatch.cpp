#include "sce_gnmdriver.h"
#include "Graphic/Gnm/GnmOpCode.h"
#include <cassert>

uint32_t PS4API sceGnmDispatchInitDefaultHardwareState(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint initCmdSize = sizeof(GnmDispatchInitDefaultHardwareState) / sizeof(uint32_t);
	assert(numDwords >= initCmdSize);
	GnmDispatchInitDefaultHardwareState* initParam = (GnmDispatchInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode = OPCODE_BUILD(initCmdSize, OP_TYPE_PRIV_DISPATCH, OP_INFO_DISPATCH_INITIALIZE_DEFAULT_HARDWARE_STATE);
	memset(initParam->reserved, 0, sizeof(initParam->reserved) * sizeof(uint32_t));
	return initCmdSize;
}


int PS4API sceGnmDisableMipStatsReport(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchIndirectOnMec(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmComputeWaitOnAddress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmComputeWaitSemaphore(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertDingDongMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


