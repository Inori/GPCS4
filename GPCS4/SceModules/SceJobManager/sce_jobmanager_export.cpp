#include "sce_jobmanager.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceJobManager_libSceJobManager_FunctionTable[] =
{
	{ 0x192F4349BCC2F8C6, "_import_192F4349BCC2F8C6", (void*)_import_192F4349BCC2F8C6 },
	{ 0x366D81840C183459, "_import_366D81840C183459", (void*)_import_366D81840C183459 },
	{ 0x55329ECDF62EFEFE, "_import_55329ECDF62EFEFE", (void*)_import_55329ECDF62EFEFE },
	{ 0x5BFC9781099AA335, "_import_5BFC9781099AA335", (void*)_import_5BFC9781099AA335 },
	{ 0x721C67BECC717C16, "_import_721C67BECC717C16", (void*)_import_721C67BECC717C16 },
	{ 0x730333CDD7AC2E56, "_import_730333CDD7AC2E56", (void*)_import_730333CDD7AC2E56 },
	{ 0xE20D5E7B7E13307F, "_import_E20D5E7B7E13307F", (void*)_import_E20D5E7B7E13307F },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceJobManager_LibTable[] =
{
	{ "libSceJobManager", g_pSceJobManager_libSceJobManager_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceJobManager =
{
	"libSceJobManager",
	g_pSceJobManager_LibTable
};


