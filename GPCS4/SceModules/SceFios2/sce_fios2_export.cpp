#include "sce_fios2.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceFios2_libSceFios2_FunctionTable[] =
{
	{ 0x466FA18B0BD29F1C, "sceFiosDateToComponents", (void*)sceFiosDateToComponents },
	{ 0x2AC553734E2437D9, "sceFiosDeleteSync", (void*)sceFiosDeleteSync },
	{ 0x9D6BB36B465D7EA0, "sceFiosDirectoryCreateSync", (void*)sceFiosDirectoryCreateSync },
	{ 0x38EBAF1CA4EEE0E7, "sceFiosDirectoryExistsSync", (void*)sceFiosDirectoryExistsSync },
	{ 0x00EBA3486A94FA6B, "sceFiosFHCloseSync", (void*)sceFiosFHCloseSync },
	{ 0x6F8E1A9D5D83ECAD, "sceFiosFHOpenSync", (void*)sceFiosFHOpenSync },
	{ 0x067D991786637AE4, "sceFiosFHReadSync", (void*)sceFiosFHReadSync },
	{ 0xC5179279BC0A0290, "sceFiosFHSeek", (void*)sceFiosFHSeek },
	{ 0xC4FE397889ED122B, "sceFiosFHStatSync", (void*)sceFiosFHStatSync },
	{ 0x2A5FD36EB0D4F583, "sceFiosFHWriteSync", (void*)sceFiosFHWriteSync },
	{ 0x3703873113363E9C, "sceFiosFileExistsSync", (void*)sceFiosFileExistsSync },
	{ 0xCC5F3F091BD15E73, "sceFiosFileGetSizeSync", (void*)sceFiosFileGetSizeSync },
	{ 0x9153314A2603EAD7, "sceFiosFileReadSync", (void*)sceFiosFileReadSync },
	{ 0xC23FD4FA631BC803, "sceFiosFileWriteSync", (void*)sceFiosFileWriteSync },
	{ 0x8DACAF634EC2E5D9, "sceFiosStatSync", (void*)sceFiosStatSync },
	{ 0xFA67CEDBB6BF6C46, "sceFiosDeallocatePassthruFH", (void*)sceFiosDeallocatePassthruFH },
	{ 0xA1182C81F258DCC7, "sceFiosFilenoToFH", (void*)sceFiosFilenoToFH },
	{ 0xF1DCD75391B1B232, "sceFiosFHToFileno", (void*)sceFiosFHToFileno },
	{ 0xF081A3C2D9EF6302, "sceFiosIsValidHandle", (void*)sceFiosIsValidHandle },
	{ 0xC35DCE8E6ECE37DA, "sceFiosFHOpenWithModeSync", (void*)sceFiosFHOpenWithModeSync },
	{ 0x1BFDFD96C752817A, "sceFiosRenameSync", (void*)sceFiosRenameSync },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceFios2_LibTable[] =
{
	{ "libSceFios2", g_pSceFios2_libSceFios2_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceFios2 =
{
	"libSceFios2",
	g_pSceFios2_LibTable
};


