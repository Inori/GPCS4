#include "sce_ajm.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceAjm_libSceAjm_FunctionTable[] =
{
	{ 0x7660F26CDFFF167F, "sceAjmBatchJobControlBufferRa", (void*)sceAjmBatchJobControlBufferRa },
	{ 0xEE37405CAFB67CCA, "sceAjmBatchJobRunSplitBufferRa", (void*)sceAjmBatchJobRunSplitBufferRa },
	{ 0x7C5164934C5F196B, "sceAjmBatchStartBuffer", (void*)sceAjmBatchStartBuffer },
	{ 0xFEA2EC7C3032C086, "sceAjmBatchWait", (void*)sceAjmBatchWait },
	{ 0x307BABEAA0AC52EB, "sceAjmFinalize", (void*)sceAjmFinalize },
	{ 0x765FB87874B352EE, "sceAjmInitialize", (void*)sceAjmInitialize },
	{ 0x031A03AC8369E09F, "sceAjmInstanceCreate", (void*)sceAjmInstanceCreate },
	{ 0x45B2DBB8ABFCCE1A, "sceAjmInstanceDestroy", (void*)sceAjmInstanceDestroy },
	{ 0x43777216EC069FAE, "sceAjmModuleRegister", (void*)sceAjmModuleRegister },
	{ 0x5A2EC3B652D5F8A2, "sceAjmModuleUnregister", (void*)sceAjmModuleUnregister },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceAjm_LibTable[] =
{
	{ "libSceAjm", g_pSceAjm_libSceAjm_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceAjm =
{
	"libSceAjm",
	g_pSceAjm_LibTable
};


