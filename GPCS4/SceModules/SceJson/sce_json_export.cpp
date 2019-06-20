#include "sce_json.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceJson_libSceJson2_FunctionTable[] =
{
	{ 0x0B1C32EF01EAE09D, "_ZN3sce4Json11Initializer10initializeEPKNS0_13InitParameterE", (void*)_ZN3sce4Json11Initializer10initializeEPKNS0_13InitParameterE },
	{ 0x70AE9B6077FF4391, "_ZN3sce4Json11InitializerC1Ev", (void*)_ZN3sce4Json11InitializerC1Ev },
	{ 0x46E8D4C5BAF785A3, "_ZN3sce4Json11InitializerD1Ev", (void*)_ZN3sce4Json11InitializerD1Ev },
	{ 0x8BF5F0663C3438E6, "_ZN3sce4Json12MemAllocator11notifyErrorEimPv", (void*)_ZN3sce4Json12MemAllocator11notifyErrorEimPv },
	{ 0xFE125171EF309F55, "_ZN3sce4Json12MemAllocatorC2Ev", (void*)_ZN3sce4Json12MemAllocatorC2Ev },
	{ 0x39C0203F172AE559, "_ZN3sce4Json12MemAllocatorD2Ev", (void*)_ZN3sce4Json12MemAllocatorD2Ev },
	{ 0xCD0B4B453A9C78C6, "_ZN3sce4Json5Array9push_backERKNS0_5ValueE", (void*)_ZN3sce4Json5Array9push_backERKNS0_5ValueE },
	{ 0x24FFCFB4A3222351, "_ZN3sce4Json5ArrayC1Ev", (void*)_ZN3sce4Json5ArrayC1Ev },
	{ 0x1C9F06A514F56A2C, "_ZN3sce4Json5ArrayD1Ev", (void*)_ZN3sce4Json5ArrayD1Ev },
	{ 0x7450A986A9DDF9AE, "_ZN3sce4Json5Value3setERKNS0_6ObjectE", (void*)_ZN3sce4Json5Value3setERKNS0_6ObjectE },
	{ 0x47B1435AD70DE9FF, "_ZN3sce4Json5Value9serializeERNS0_6StringE", (void*)_ZN3sce4Json5Value9serializeERNS0_6StringE },
	{ 0x8997987CEC6D3118, "_ZN3sce4Json5ValueC1ERKNS0_5ArrayE", (void*)_ZN3sce4Json5ValueC1ERKNS0_5ArrayE },
	{ 0xB1922831118EFA39, "_ZN3sce4Json5ValueC1ERKNS0_6StringE", (void*)_ZN3sce4Json5ValueC1ERKNS0_6StringE },
	{ 0x51EB964FEC8D7424, "_ZN3sce4Json5ValueC1Eb", (void*)_ZN3sce4Json5ValueC1Eb },
	{ 0xD252CAF3E903AA61, "_ZN3sce4Json5ValueC1El", (void*)_ZN3sce4Json5ValueC1El },
	{ 0xC7801475B869441D, "_ZN3sce4Json5ValueC1Em", (void*)_ZN3sce4Json5ValueC1Em },
	{ 0xA81323AB2067DCE3, "_ZN3sce4Json5ValueC1Ev", (void*)_ZN3sce4Json5ValueC1Ev },
	{ 0x593B587FE70D9D72, "_ZN3sce4Json5ValueD1Ev", (void*)_ZN3sce4Json5ValueD1Ev },
	{ 0xE33AE6E95AE0200C, "_ZN3sce4Json5ValueaSERKS1_", (void*)_ZN3sce4Json5ValueaSERKS1_ },
	{ 0x3893D3A27A9D8342, "_ZN3sce4Json6ObjectC1Ev", (void*)_ZN3sce4Json6ObjectC1Ev },
	{ 0xE499B366DF2DC00A, "_ZN3sce4Json6ObjectD1Ev", (void*)_ZN3sce4Json6ObjectD1Ev },
	{ 0x111B9FF72D0363CE, "_ZN3sce4Json6ObjectixERKNS0_6StringE", (void*)_ZN3sce4Json6ObjectixERKNS0_6StringE },
	{ 0x4B9271427A061771, "_ZN3sce4Json6Parser5parseERNS0_5ValueEPKcm", (void*)_ZN3sce4Json6Parser5parseERNS0_5ValueEPKcm },
	{ 0xF4A5191632352310, "_ZN3sce4Json6StringC1EPKc", (void*)_ZN3sce4Json6StringC1EPKc },
	{ 0xA929AA2D75C23DAB, "_ZN3sce4Json6StringC1Ev", (void*)_ZN3sce4Json6StringC1Ev },
	{ 0x706D551361CC97A7, "_ZN3sce4Json6StringD1Ev", (void*)_ZN3sce4Json6StringD1Ev },
	{ 0xB27E07342B4D4736, "_ZNK3sce4Json5Value11getUIntegerEv", (void*)_ZNK3sce4Json5Value11getUIntegerEv },
	{ 0x7A927AC762D5D245, "_ZNK3sce4Json5Value9getStringEv", (void*)_ZNK3sce4Json5Value9getStringEv },
	{ 0x1F00EDE650FD05FA, "_ZNK3sce4Json5ValueixEPKc", (void*)_ZNK3sce4Json5ValueixEPKc },
	{ 0x2F52809185A697F3, "_ZNK3sce4Json6String5c_strEv", (void*)_ZNK3sce4Json6String5c_strEv },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceJson_LibTable[] =
{
	{ "libSceJson2", g_pSceJson_libSceJson2_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceJson =
{
	"libSceJson",
	g_pSceJson_LibTable
};


