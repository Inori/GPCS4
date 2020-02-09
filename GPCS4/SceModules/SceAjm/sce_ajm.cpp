#include "sce_ajm.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceAjm);

//////////////////////////////////////////////////////////////////////////
// library: libSceAjm
//////////////////////////////////////////////////////////////////////////

int PS4API sceAjmInitialize(int64_t iReserved, SceAjmContextId * const pContext)
{
	LOG_FIXME("Not implemented");
	*pContext = 0x123;
	return SCE_OK;
}


int PS4API sceAjmFinalize(const SceAjmContextId uiContext)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmModuleRegister(const SceAjmContextId uiContext, const SceAjmCodecType uiCodec, int64_t iReserved)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmModuleUnregister(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmBatchJobControlBufferRa(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmBatchJobRunSplitBufferRa(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmBatchStartBuffer(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmBatchWait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmInstanceCreate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceAjmInstanceDestroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}






