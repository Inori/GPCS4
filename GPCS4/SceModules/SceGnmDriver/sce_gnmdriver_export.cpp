#include "sce_gnmdriver.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceGnmDriver_libSceGnmDriver_FunctionTable[] =
{
	{ 0x145559702BB7CD65, "_import_145559702BB7CD65", (void*)_import_145559702BB7CD65 },
	{ 0x8A6D99B88B5A6EEE, "_import_8A6D99B88B5A6EEE", (void*)_import_8A6D99B88B5A6EEE },
	{ 0xB616CF706EC4EEA9, "_import_B616CF706EC4EEA9", (void*)_import_B616CF706EC4EEA9 },
	{ 0x6F4C729659D563F2, "sceGnmAddEqEvent", (void*)sceGnmAddEqEvent },
	{ 0x6F4F0082D3E51CF8, "sceGnmAreSubmitsAllowed", (void*)sceGnmAreSubmitsAllowed },
	{ 0x8A1C6B6ECA122967, "sceGnmBeginWorkload", (void*)sceGnmBeginWorkload },
	{ 0xE6E7409BEE9BA158, "sceGnmCreateWorkloadStream", (void*)sceGnmCreateWorkloadStream },
	{ 0xAA91884F33C4F997, "sceGnmDebugHardwareStatus", (void*)sceGnmDebugHardwareStatus },
	{ 0x3D54FE7EEA12F605, "sceGnmDeleteEqEvent", (void*)sceGnmDeleteEqEvent },
	{ 0x52D39B0D142219BB, "sceGnmDestroyWorkloadStream", (void*)sceGnmDestroyWorkloadStream },
	{ 0x6D7E486D1BC40979, "sceGnmDingDong", (void*)sceGnmDingDong },
	{ 0x6F25E5AAEA5DF1C1, "sceGnmDingDongForWorkload", (void*)sceGnmDingDongForWorkload },
	{ 0x1C7A3504096364EF, "sceGnmDisableMipStatsReport", (void*)sceGnmDisableMipStatsReport },
	{ 0xD01CCB1A58DCC01A, "sceGnmDispatchDirect", (void*)sceGnmDispatchDirect },
	{ 0x678DEF2A9E64EEBD, "sceGnmDispatchIndirect", (void*)sceGnmDispatchIndirect },
	{ 0xC040F8657085253D, "sceGnmDispatchIndirectOnMec", (void*)sceGnmDispatchIndirectOnMec },
	{ 0x9C5E9B1515014405, "sceGnmDispatchInitDefaultHardwareState", (void*)sceGnmDispatchInitDefaultHardwareState },
	{ 0x1E54CFA19FE863B6, "sceGnmDrawIndex", (void*)sceGnmDrawIndex },
	{ 0x186B27EE3313C70E, "sceGnmDrawIndexAuto", (void*)sceGnmDrawIndexAuto },
	{ 0x103F7F163AFC4DAE, "sceGnmDrawIndexIndirect", (void*)sceGnmDrawIndexIndirect },
	{ 0xE6AF79ADABE7B9E8, "sceGnmDrawIndexIndirectMulti", (void*)sceGnmDrawIndexIndirectMulti },
	{ 0x8C774FBC8CE5A4A7, "sceGnmDrawIndexMultiInstanced", (void*)sceGnmDrawIndexMultiInstanced },
	{ 0xA1833E6337C29B66, "sceGnmDrawIndexOffset", (void*)sceGnmDrawIndexOffset },
	{ 0xE2FFA8B4821D8EA8, "sceGnmDrawIndirect", (void*)sceGnmDrawIndirect },
	{ 0x7140A8F0EBC0AEBC, "sceGnmDrawIndirectCountMulti", (void*)sceGnmDrawIndirectCountMulti },
	{ 0x7F94102E9F6BCC69, "sceGnmDrawIndirectMulti", (void*)sceGnmDrawIndirectMulti },
	{ 0xC9BD9C4616A00F52, "sceGnmDrawInitDefaultHardwareState350", (void*)sceGnmDrawInitDefaultHardwareState350 },
	{ 0xB2D0D2616D920553, "sceGnmDrawOpaqueAuto", (void*)sceGnmDrawOpaqueAuto },
	{ 0x4CB5789ACC226780, "sceGnmDriverCaptureInProgress", (void*)sceGnmDriverCaptureInProgress },
	{ 0x47ACF5C4CDE95BFC, "sceGnmDriverTraceInProgress", (void*)sceGnmDriverTraceInProgress },
	{ 0x77CF1A9EB80DA0A6, "sceGnmDriverTriggerCapture", (void*)sceGnmDriverTriggerCapture },
	{ 0x15ADF1EF938E2D10, "sceGnmEndWorkload", (void*)sceGnmEndWorkload },
	{ 0x881B7739ED342AF7, "sceGnmFlushGarlic", (void*)sceGnmFlushGarlic },
	{ 0x528618D0358C0B45, "sceGnmGetEqEventType", (void*)sceGnmGetEqEventType },
	{ 0x1FBFDF82F12EB4CD, "sceGnmGetEqTimeStamp", (void*)sceGnmGetEqTimeStamp },
	{ 0xA0BE21188D4F329C, "sceGnmGetGpuBlockStatus", (void*)sceGnmGetGpuBlockStatus },
	{ 0xB590922F9BA59C1E, "sceGnmGetGpuInfoStatus", (void*)sceGnmGetGpuInfoStatus },
	{ 0x8858AB173818B2FC, "sceGnmGetLastWaitedAddress", (void*)sceGnmGetLastWaitedAddress },
	{ 0x2A795D44E5245896, "sceGnmGetNumTcaUnits", (void*)sceGnmGetNumTcaUnits },
	{ 0x7F325D122853155E, "sceGnmGetProtectionFaultTimeStamp", (void*)sceGnmGetProtectionFaultTimeStamp },
	{ 0x9C4C856D8525A083, "sceGnmGetShaderProgramBaseAddress", (void*)sceGnmGetShaderProgramBaseAddress },
	{ 0x93B8864EF0D03CB4, "sceGnmGetShaderStatus", (void*)sceGnmGetShaderStatus },
	{ 0x967DF7CE306B7E39, "sceGnmGetTheTessellationFactorRingBufferBaseAddress", (void*)sceGnmGetTheTessellationFactorRingBufferBaseAddress },
	{ 0x35FBCEACDCF2EAC9, "sceGnmInsertDingDongMarker", (void*)sceGnmInsertDingDongMarker },
	{ 0xEEA65536012EF926, "sceGnmInsertPopMarker", (void*)sceGnmInsertPopMarker },
	{ 0x68F2192535C2F9C5, "sceGnmInsertPushColorMarker", (void*)sceGnmInsertPushColorMarker },
	{ 0x5B512D8FF8E55BB6, "sceGnmInsertPushMarker", (void*)sceGnmInsertPushMarker },
	{ 0x6A3DCBFE26859B29, "sceGnmInsertSetColorMarker", (void*)sceGnmInsertSetColorMarker },
	{ 0x8E222DCD2EBEDB68, "sceGnmInsertSetMarker", (void*)sceGnmInsertSetMarker },
	{ 0x5110E025C5E140EB, "sceGnmInsertThreadTraceMarker", (void*)sceGnmInsertThreadTraceMarker },
	{ 0xD6A5CB1C8A5138F1, "sceGnmInsertWaitFlipDone", (void*)sceGnmInsertWaitFlipDone },
	{ 0x8E0DF7AC428B7D5B, "sceGnmIsUserPaEnabled", (void*)sceGnmIsUserPaEnabled },
	{ 0xDBA3CCE4CCE5F337, "sceGnmLogicalCuIndexToPhysicalCuIndex", (void*)sceGnmLogicalCuIndexToPhysicalCuIndex },
	{ 0x454EF891E93F3747, "sceGnmLogicalCuMaskToPhysicalCuMask", (void*)sceGnmLogicalCuMaskToPhysicalCuMask },
	{ 0xDBDA0ABCA5F3119A, "sceGnmMapComputeQueue", (void*)sceGnmMapComputeQueue },
	{ 0x03EB86ABEDCA16D4, "sceGnmMapComputeQueueWithPriority", (void*)sceGnmMapComputeQueueWithPriority },
	{ 0x80E6CE0E58BF387F, "sceGnmRequestFlipAndSubmitDone", (void*)sceGnmRequestFlipAndSubmitDone },
	{ 0xE98447861E661C2B, "sceGnmRequestFlipAndSubmitDoneForWorkload", (void*)sceGnmRequestFlipAndSubmitDoneForWorkload },
	{ 0x7FCE68AE3C7BAADB, "sceGnmRequestMipStatsReportAndReset", (void*)sceGnmRequestMipStatsReportAndReset },
	{ 0x31846D621A2329D0, "sceGnmResetVgtControl", (void*)sceGnmResetVgtControl },
	{ 0x2B1FE1FE759027C0, "sceGnmSetCsShaderWithModifier", (void*)sceGnmSetCsShaderWithModifier },
	{ 0x5FD3A6C3D770BF93, "sceGnmSetEmbeddedPsShader", (void*)sceGnmSetEmbeddedPsShader },
	{ 0xF8016F3845EB2899, "sceGnmSetEmbeddedVsShader", (void*)sceGnmSetEmbeddedVsShader },
	{ 0x1541C6F2C437479F, "sceGnmSetEsShader", (void*)sceGnmSetEsShader },
	{ 0x8ED92A5E900E63AC, "sceGnmSetGsRingSizes", (void*)sceGnmSetGsRingSizes },
	{ 0x509C0DB8C05C5009, "sceGnmSetGsShader", (void*)sceGnmSetGsShader },
	{ 0x54936316DAA2179C, "sceGnmSetHsShader", (void*)sceGnmSetHsShader },
	{ 0xBDC91DCDB438E922, "sceGnmSetLsShader", (void*)sceGnmSetLsShader },
	{ 0xE6E14A7248896113, "sceGnmSetPsShader350", (void*)sceGnmSetPsShader350 },
	{ 0x7050A9D0D5FCC1FD, "sceGnmSetVgtControl", (void*)sceGnmSetVgtControl },
	{ 0x8008429FA5225386, "sceGnmSetVsShader", (void*)sceGnmSetVsShader },
	{ 0x5E2CB3359F49E274, "sceGnmSetWaveLimitMultipliers", (void*)sceGnmSetWaveLimitMultipliers },
	{ 0xFB1B8387195644F8, "sceGnmSetupMipStatsReport", (void*)sceGnmSetupMipStatsReport },
	{ 0xC5BC4D6AD6B0A217, "sceGnmSubmitAndFlipCommandBuffers", (void*)sceGnmSubmitAndFlipCommandBuffers },
	{ 0x19AEABEC7E98D112, "sceGnmSubmitAndFlipCommandBuffersForWorkload", (void*)sceGnmSubmitAndFlipCommandBuffersForWorkload },
	{ 0xCF0634615F754D32, "sceGnmSubmitCommandBuffers", (void*)sceGnmSubmitCommandBuffers },
	{ 0x8D1708F157204F3E, "sceGnmSubmitCommandBuffersForWorkload", (void*)sceGnmSubmitCommandBuffersForWorkload },
	{ 0xCAF67BDEE414AAB9, "sceGnmSubmitDone", (void*)sceGnmSubmitDone },
	{ 0x02B4A0FD31A29E19, "sceGnmUnmapComputeQueue", (void*)sceGnmUnmapComputeQueue },
	{ 0x9CB3368B6FBAE610, "sceGnmUpdateGsShader", (void*)sceGnmUpdateGsShader },
	{ 0x18D971FB2EF13DD1, "sceGnmUpdateHsShader", (void*)sceGnmUpdateHsShader },
	{ 0x98B54BECDEC15418, "sceGnmUpdatePsShader350", (void*)sceGnmUpdatePsShader350 },
	{ 0x577D55D3552249C6, "sceGnmUpdateVsShader", (void*)sceGnmUpdateVsShader },
	{ 0xBBD60AA514477BF3, "sceRazorCaptureImmediate", (void*)sceRazorCaptureImmediate },
	{ 0x7F7DCEAEBB9061B3, "sceRazorIsLoaded", (void*)sceRazorIsLoaded },
	{ 0x75EC17C39AE82ECD, "_import_75EC17C39AE82ECD", (void*)_import_75EC17C39AE82ECD },
	{ 0x7DFACD40EB21A30B, "sceGnmComputeWaitOnAddress", (void*)sceGnmComputeWaitOnAddress },
	{ 0x1096A9365DBEA605, "sceGnmComputeWaitSemaphore", (void*)sceGnmComputeWaitSemaphore },
	{ 0x2A5D19DCB1F4ED02, "sceGnmLogicalTcaUnitToPhysical", (void*)sceGnmLogicalTcaUnitToPhysical },
	{ 0xE0CBFD397CA9046F, "sceGnmFindResourcesPublic", (void*)sceGnmFindResourcesPublic },
	{ 0x4098CF8E598F00BD, "sceGnmGetOwnerName", (void*)sceGnmGetOwnerName },
	{ 0xE0F2A76173A1731E, "sceGnmGetResourceBaseAddressAndSizeInBytes", (void*)sceGnmGetResourceBaseAddressAndSizeInBytes },
	{ 0x3B44BDE989C3D385, "sceGnmGetResourceName", (void*)sceGnmGetResourceName },
	{ 0x501BFB16455FCDC4, "sceGnmGetResourceShaderGuid", (void*)sceGnmGetResourceShaderGuid },
	{ 0x6DDA9DBC890B3C85, "sceGnmGetResourceType", (void*)sceGnmGetResourceType },
	{ 0x52806E58084A93B5, "sceGnmGetResourceUserData", (void*)sceGnmGetResourceUserData },
	{ 0xE8831BA51EE74F30, "sceGnmQueryResourceRegistrationUserMemoryRequirements", (void*)sceGnmQueryResourceRegistrationUserMemoryRequirements },
	{ 0xF4CBFAD4768C85F0, "sceGnmRegisterGdsResource", (void*)sceGnmRegisterGdsResource },
	{ 0x645A8A165DB768C7, "sceGnmRegisterOwner", (void*)sceGnmRegisterOwner },
	{ 0x9EF1307D8008993B, "sceGnmRegisterResource", (void*)sceGnmRegisterResource },
	{ 0xABFAA10F13FAEC78, "sceGnmSetResourceRegistrationUserMemory", (void*)sceGnmSetResourceRegistrationUserMemory },
	{ 0x2B704A04160A5121, "sceGnmSetResourceUserData", (void*)sceGnmSetResourceUserData },
	{ 0xCA11429DACF975AC, "sceGnmUnregisterAllResourcesForOwner", (void*)sceGnmUnregisterAllResourcesForOwner },
	{ 0x7E12B0095563F679, "sceGnmUnregisterOwnerAndResources", (void*)sceGnmUnregisterOwnerAndResources },
	{ 0x93C11792120FFA53, "sceGnmUnregisterResource", (void*)sceGnmUnregisterResource },
	{ 0x060337B772EF70D9, "_import_060337B772EF70D9", (void*)_import_060337B772EF70D9 },
	{ 0x497C387591248290, "sceGnmValidateDisableDiagnostics", (void*)sceGnmValidateDisableDiagnostics },
	{ 0xA863FBE13E4E5897, "sceGnmValidateDispatchCommandBuffers", (void*)sceGnmValidateDispatchCommandBuffers },
	{ 0x86C64F7F594E37B1, "sceGnmValidateDrawCommandBuffers", (void*)sceGnmValidateDrawCommandBuffers },
	{ 0x457ED708D49A2FA2, "sceGnmValidateGetDiagnosticInfo", (void*)sceGnmValidateGetDiagnosticInfo },
	{ 0xE521C63702D7055E, "sceGnmValidateGetDiagnostics", (void*)sceGnmValidateGetDiagnostics },
	{ 0x1F330DEC036A6047, "sceGnmValidateGetVersion", (void*)sceGnmValidateGetVersion },
	{ 0xAD3215D759CC42E3, "sceGnmValidateOnSubmitEnabled", (void*)sceGnmValidateOnSubmitEnabled },
	{ 0x30131AE8416EE0AA, "sceGnmValidateResetState", (void*)sceGnmValidateResetState },
	{ 0x43BB7854460B69F2, "sceGnmValidationRegisterMemoryCheckCallback", (void*)sceGnmValidationRegisterMemoryCheckCallback },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceGnmDriver_LibTable[] =
{
	{ "libSceGnmDriver", g_pSceGnmDriver_libSceGnmDriver_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceGnmDriver =
{
	"libSceGnmDriver",
	g_pSceGnmDriver_LibTable
};


