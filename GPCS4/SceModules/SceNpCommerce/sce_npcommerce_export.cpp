#include "sce_npcommerce.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpCommerce_libSceNpCommerce_FunctionTable[] =
{
	{ 0xAF8D9B59C41BB596, "sceNpCommerceDialogGetResult", (void*)sceNpCommerceDialogGetResult },
	{ 0xD1A4766969906A5E, "sceNpCommerceDialogInitialize", (void*)sceNpCommerceDialogInitialize },
	{ 0x0DF4820D10371236, "sceNpCommerceDialogOpen", (void*)sceNpCommerceDialogOpen },
	{ 0x9BF23DD806F9D16F, "sceNpCommerceDialogTerminate", (void*)sceNpCommerceDialogTerminate },
	{ 0x2D1E5CC0530C0951, "sceNpCommerceDialogUpdateStatus", (void*)sceNpCommerceDialogUpdateStatus },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpCommerce_LibTable[] =
{
	{ "libSceNpCommerce", g_pSceNpCommerce_libSceNpCommerce_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpCommerce =
{
	"libSceNpCommerce",
	g_pSceNpCommerce_LibTable
};


