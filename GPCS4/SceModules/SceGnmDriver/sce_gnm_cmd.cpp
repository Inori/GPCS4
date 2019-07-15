#include "sce_gnmdriver.h"
#include "Graphic/Gnm/GnmOpCode.h"
#include "Graphic/Gnm/GnmStructure.h"
#include <cassert>


// this file place functions used by both DrawCommandBuffer and DispatchCommandBuffer


int PS4API sceGnmInsertPushMarker(uint32_t* cmdBuffer, uint32_t numDwords, const char *debugString)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d str %p", cmdBuffer, numDwords, debugString);
	GnmPushMarker* param = (GnmPushMarker*)cmdBuffer;
	param->opcode = OPCODE_BUILD(numDwords, OP_TYPE_PRIV_SHARED, OP_INFO_PUSH_MARKER);
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
	const uint32_t paramSize = sizeof(GnmPopMarker) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmPopMarker* param = (GnmPopMarker*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_SHARED, OP_INFO_POP_MARKER);
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
	const CsStageRegisters *csRegs, uint32_t shaderModifier)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d cs %p mod %d", cmdBuffer, numDwords, csRegs, shaderModifier);
	const uint32_t paramSize = sizeof(GnmCSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCSShader* param = (GnmCSShader*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_SHARED, OP_INFO_SET_CS_SHADER);
	param->modifier = shaderModifier;
	memcpy(&param->csRegs, csRegs, sizeof(CsStageRegisters));
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDispatchDirect(uint32_t* cmdBuffer, uint32_t numDwords, 
	uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, uint32_t pred)
{
	LOG_SCE_GRAPHIC("cmd %p numdw %d x %d y %d z %d", cmdBuffer, numDwords, threadGroupX, threadGroupY, threadGroupZ);
	const uint32_t paramSize = sizeof(GnmDispatchDirect) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmDispatchDirect* param = (GnmDispatchDirect*)cmdBuffer;
	param->opcode = OPCODE_BUILD(paramSize, OP_TYPE_PRIV_SHARED, OP_INFO_DISPATCH_DIRECT);
	param->threadGroupX = threadGroupX;
	param->threadGroupY = threadGroupY;
	param->threadGroupZ = threadGroupZ;
	param->pred = pred;
	memset(param->reserved, 0, sizeof(param->reserved) * sizeof(uint32_t));
	return SCE_OK;
}


int PS4API sceGnmDispatchIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}




