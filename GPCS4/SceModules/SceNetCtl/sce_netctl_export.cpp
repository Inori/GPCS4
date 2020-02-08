#include "sce_netctl.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNetCtl_libSceNetCtl_FunctionTable[] =
{
	{ 0x824CB4FA868D3389, "sceNetCtlInit", (void*)sceNetCtlInit },
	{ 0x890C378903E1BD44, "sceNetCtlCheckCallback", (void*)sceNetCtlCheckCallback },
	{ 0xD1C06076E3D147E3, "sceNetCtlGetResult", (void*)sceNetCtlGetResult },
	{ 0x509F99ED0FB8724D, "sceNetCtlRegisterCallback", (void*)sceNetCtlRegisterCallback },
	{ 0x46A9B63A764C0B3D, "sceNetCtlUnregisterCallback", (void*)sceNetCtlUnregisterCallback },
	{ 0xA1BBB17538B0905F, "sceNetCtlGetInfo", (void*)sceNetCtlGetInfo },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNetCtl_LibTable[] =
{
	{ "libSceNetCtl", g_pSceNetCtl_libSceNetCtl_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNetCtl =
{
	"libSceNetCtl",
	g_pSceNetCtl_LibTable
};


