#include "sce_gnmdriver.h"
#include "Gnm/GnmOpCode.h"
#include <cassert>

LOG_CHANNEL(SceModules.SceGnmDriver.GnmDispatch);

using namespace sce::Gnm;

uint32_t PS4API sceGnmDispatchInitDefaultHardwareState(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint32_t initCmdSize = sizeof(GnmCmdDispatchInitDefaultHardwareState) / sizeof(uint32_t);
	assert(numDwords >= initCmdSize);
	GnmCmdDispatchInitDefaultHardwareState* initParam = (GnmCmdDispatchInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode = PM4_HEADER_BUILD(initCmdSize, IT_GNM_PRIVATE, OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE);
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


int PS4API sceGnmComputeWaitOnAddress(
	uint32_t* cmdBuffer,
	uint32_t  numDwords,
	void*     gpuAddr,
	uint32_t  mask,
	uint32_t  compareFunc,
	uint32_t  refValue)
{
	LOG_SCE_GRAPHIC("cmdbuff %p numDwords %d", cmdBuffer, numDwords);
	const uint32_t cmdSize = sizeof(GnmCmdComputeWaitOnAddress) / sizeof(uint32_t);
	assert(numDwords == cmdSize);
	GnmCmdComputeWaitOnAddress* param = (GnmCmdComputeWaitOnAddress*)cmdBuffer;
	param->opcode                     = PM4_HEADER_BUILD(cmdSize, IT_GNM_PRIVATE, OP_PRIV_COMPUTE_WAIT_ON_ADDRESS);
	param->gpuAddr                    = reinterpret_cast<uint64_t>(gpuAddr);
	param->mask                       = mask;
	param->compareFunc                = compareFunc;
	param->refValue                   = refValue;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
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


