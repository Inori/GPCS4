#include "sce_gnmdriver.h"

// this file place functions used by both DrawCommandBuffer and DispatchCommandBuffer


int PS4API sceGnmInsertPushMarker(uint32_t* cmdBuffer, uint32_t numDwords, const char *debugString)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertPushColorMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertPopMarker(uint32_t* cmdBuffer, uint32_t numDwords)
{
	LOG_SCE_GRAPHIC("Not implemented");
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
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchDirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}




