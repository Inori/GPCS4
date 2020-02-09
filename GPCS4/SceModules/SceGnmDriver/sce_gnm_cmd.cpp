#include "sce_gnmdriver.h"
#include "Graphic/Gnm/GnmOpCode.h"
#include "Graphic/Gnm/GnmStructure.h"
#include <cassert>

LOG_CHANNEL(SceModules.SceGnmDriver.GnmCmd);

// this file place functions used by both DrawCommandBuffer and DispatchCommandBuffer


int PS4API sceGnmInsertPushMarker(uint32_t* cmdBuffer, uint32_t numDwords, const char *debugString)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d str %p", cmdBuffer, numDwords, debugString);
	GnmCmdPushMarker* param = (GnmCmdPushMarker*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(numDwords, IT_GNM_PRIVATE, OP_PRIV_PUSH_MARKER);
	uint32_t strLen = strlen(debugString);
	if ((numDwords - 1) * sizeof(uint32_t) > strLen)
	{
		strcpy(param->debugString, debugString);
	}
	return SCE_OK;
}


int PS4API sceGnmInsertPushColorMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertPopMarker(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d", cmdBuffer, numDwords);
	const uint32_t paramSize = sizeof(GnmCmdPopMarker) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdPopMarker* param = (GnmCmdPopMarker*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_POP_MARKER);
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}

int PS4API sceGnmInsertSetColorMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertSetMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertThreadTraceMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetCsShaderWithModifier(uint32_t* cmdBuffer, uint32_t numDwords,
	const pssl::CsStageRegisters *csRegs, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d cs %p mod %d", cmdBuffer, numDwords, csRegs, shaderModifier);
	const uint32_t paramSize = sizeof(GnmCmdCSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdCSShader* param = (GnmCmdCSShader*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_SET_CS_SHADER);
	param->modifier = shaderModifier;
	memcpy(&param->csRegs, csRegs, sizeof(pssl::CsStageRegisters));
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDispatchDirect(uint32_t* cmdBuffer, uint32_t numDwords, 
	uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, uint32_t pred)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d x %d y %d z %d", cmdBuffer, numDwords, threadGroupX, threadGroupY, threadGroupZ);
	const uint32_t paramSize = sizeof(GnmCmdDispatchDirect) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdDispatchDirect* param = (GnmCmdDispatchDirect*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_DISPATCH_DIRECT);
	param->threadGroupX = threadGroupX;
	param->threadGroupY = threadGroupY;
	param->threadGroupZ = threadGroupZ;
	param->pred = pred;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDispatchIndirect(uint32_t *cmdBuffer,
								  uint32_t numDwords,
								  uint32_t dataOffsetInBytes,
								  uint32_t flag)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d dataOffsetInBytes %d, flag: %d", cmdBuffer,
					numDwords, dataOffsetInBytes, flag);

	const uint32_t paramSize = sizeof(GnmCmdDispatchIndirect) / sizeof(uint32_t);
	assert(paramSize == numDwords);

	auto param = (GnmCmdDispatchIndirect*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_GNM_PRIVATE, OP_PRIV_DISPATCH_INDIRECT);
	param->dataOffsetInBytes      = dataOffsetInBytes;
	param->flag                   = flag;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	
	return SCE_OK;
}




