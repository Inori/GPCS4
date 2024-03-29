#pragma once

#include "GnmCommon.h"
#include "GnmOpCode.h"

#include <array>

namespace sce::Gnm
{
	class GnmCommandProxy
	{
	public:
		enum PacketCount : uint32_t
		{
			kPacketCountSub_7FFC805A03D0                         = 0,
			kPacketCountPrepareFlip2                             = 3,
			kPacketCountSetVsharpInUserData                      = 2,
			kPacketCountSetTsharpInUserData                      = 2,
			kPacketCountSetSsharpInUserData                      = 2,
			kPacketCountSetUserDataRegion                        = 2,
			kPacketCountSetPointerInUserData                     = 1,
			kPacketCountSetUserData                              = 1,
			kPacketCountPrefetchIntoL2                           = 0,
			kPacketCountRequestVideoFlip                         = 2,
			kPacketCountInitializeDefaultHardwareState           = 0,
			kPacketCountInitializeToDefaultContextState          = 0,
			kPacketCountInitializeToDefaultContextState2         = 0,
			kPacketCountInitializeToDefaultContextState3         = 0,
			kPacketCountSetupEsGsRingRegisters                   = 1,
			kPacketCountSetupGsVsRingRegisters                   = 4,
			kPacketCountFlushStreamout                           = 5,
			kPacketCountSetStreamoutBufferDimensions             = 1,
			kPacketCountSetStreamoutMapping                      = 1,
			kPacketCountWriteStreamoutBufferUpdate               = 1,
			kPacketCountSetComputeShaderControl                  = 1,
			kPacketCountSetGraphicsShaderControl                 = 7,
			kPacketCountSetGraphicsShaderControl2                = 1,
			kPacketCountSetGraphicsShaderControl3                = 1,
			kPacketCountSetGraphicsShaderControl4                = 1,
			kPacketCountSetGraphicsShaderControl5                = 1,
			kPacketCountSetGraphicsShaderControl6                = 1,
			kPacketCountSetGraphicsShaderControl7                = 1,
			kPacketCountSetComputeResourceManagement             = 2,
			kPacketCountSetComputeResourceManagement2            = 2,
			kPacketCountSetComputeResourceManagementForNeo       = 1,
			kPacketCountSetComputeResourceManagementForBase      = 1,
			kPacketCountSetGraphicsScratchSize                   = 1,
			kPacketCountSetComputeScratchSize                    = 1,
			kPacketCountSetViewportTransformControl              = 1,
			kPacketCountSetClipControl                           = 1,
			kPacketCountSetUserClipPlane                         = 1,
			kPacketCountSetClipRectangle                         = 1,
			kPacketCountSetClipRectangleRule                     = 1,
			kPacketCountSetPrimitiveSetup                        = 1,
			kPacketCountSetPrimitiveResetIndexEnable             = 1,
			kPacketCountSetPrimitiveResetIndex                   = 1,
			kPacketCountSetVertexQuantization                    = 1,
			kPacketCountSetWindowOffset                          = 1,
			kPacketCountSetScreenScissor                         = 1,
			kPacketCountSetWindowScissor                         = 1,
			kPacketCountSetGenericScissor                        = 1,
			kPacketCountSetViewportScissor                       = 1,
			kPacketCountSetViewport                              = 2,
			kPacketCountSetScanModeControl                       = 1,
			kPacketCountSetAaSampleCount                         = 1,
			kPacketCountSetAaSampleLocationControl               = 2,
			kPacketCountSetPsShaderRate                          = 1,
			kPacketCountSetAaSampleMask                          = 2,
			kPacketCountSetAaSampleLocations                     = 1,
			kPacketCountSetCentroidPriority                      = 1,
			kPacketCountSetLineWidth                             = 1,
			kPacketCountSetPointSize                             = 1,
			kPacketCountSetPointMinMax                           = 1,
			kPacketCountSetPolygonOffsetClamp                    = 1,
			kPacketCountSetPolygonOffsetZFormat                  = 1,
			kPacketCountSetPolygonOffsetFront                    = 1,
			kPacketCountSetPolygonOffsetBack                     = 1,
			kPacketCountSetHardwareScreenOffset                  = 1,
			kPacketCountSetGuardBandClip                         = 2,
			kPacketCountSetGuardBandDiscard                      = 2,
			kPacketCountSetInstanceStepRate                      = 1,
			kPacketCountSetPsShaderUsage                         = 1,
			kPacketCountSetRenderOverrideControl                 = 1,
			kPacketCountSetRenderOverride2Control                = 1,
			kPacketCountSetActiveShaderStages                    = 1,
			kPacketCountSetGsMode                                = 1,
			kPacketCountSetGsOnChipControl                       = 1,
			kPacketCountSetBorderColorTableAddr                  = 1,
			kPacketCountSetPsShader                              = 3,
			kPacketCountSetPsShader2                             = 4,
			kPacketCountSetPsShader3                             = 9,
			kPacketCountSetVsShader                              = 6,
			kPacketCountSetEsShader                              = 3,
			kPacketCountSetGsShader                              = 6,
			kPacketCountSetCsShader                              = 4,
			kPacketCountSetLsShader                              = 4,
			kPacketCountSetHsShader                              = 6,
			kPacketCountUpdatePsShader                           = 9,
			kPacketCountUpdateVsShader                           = 6,
			kPacketCountUpdateGsShader                           = 6,
			kPacketCountUpdateHsShader                           = 6,
			kPacketCountWaitOnCe                                 = 1,
			kPacketCountIncrementDeCounter                       = 1,
			kPacketCountReadDataFromGds                          = 1,
			kPacketCountSetRenderTarget                          = 3,
			kPacketCountSetRenderTarget2                         = 2,
			kPacketCountSetRenderTarget3                         = 2,
			kPacketCountSetRenderTarget4                         = 1,
			kPacketCountSetDepthRenderTarget                     = 6,
			kPacketCountSetDepthRenderTarget2                    = 2,
			kPacketCountSetDepthClearValue                       = 1,
			kPacketCountSetStencilClearValue                     = 1,
			kPacketCountSetCmaskClearColor                       = 1,
			kPacketCountSetRenderTargetMask                      = 1,
			kPacketCountSetBlendControl                          = 1,
			kPacketCountSetBlendColor                            = 1,
			kPacketCountSetStencilSeparate                       = 0,
			kPacketCountSetAlphaToMaskControl                    = 1,
			kPacketCountSetHtileStencil0                         = 1,
			kPacketCountSetHtileStencil1                         = 1,
			kPacketCountSetCbControl                             = 1,
			kPacketCountSetDepthStencilDisable                   = 0,
			kPacketCountSetDepthBoundsRange                      = 1,
			kPacketCountSetStencilOpControl                      = 1,
			kPacketCountSetDbRenderControl                       = 1,
			kPacketCountSetDbCountControl                        = 1,
			kPacketCountSetDepthEqaaControl                      = 1,
			kPacketCountSetPrimitiveIdEnable                     = 1,
			kPacketCountSetVgtControlForBase                     = 1,
			kPacketCountSetVgtControlForNeo                      = 1,
			kPacketCountSetVgtControlForNeo2                     = 1,
			kPacketCountSetVertexReuseEnable                     = 1,
			kPacketCountSetPrimitiveType                         = 1,
			kPacketCountSetIndexSize                             = 1,
			kPacketCountSetNumInstances                          = 1,
			kPacketCountSetIndexOffset                           = 1,
			kPacketCountSetIndexOffset2                          = 1,
			kPacketCountDrawOpaqueAuto                           = 0,
			kPacketCountDrawIndexAuto                            = 0,
			kPacketCountDrawIndexInline                          = 0,
			kPacketCountDrawIndexInline2                         = 0,
			kPacketCountDrawIndex                                = 0,
			kPacketCountSetIndexBuffer                           = 1,
			kPacketCountSetIndexCount                            = 1,
			kPacketCountDrawIndexOffset                          = 0,
			kPacketCountSetBaseIndirectArgs                      = 1,
			kPacketCountDrawIndirect                             = 0,
			kPacketCountDrawIndirectMulti                        = 0,
			kPacketCountDrawIndirectCountMulti                   = 0,
			kPacketCountDrawIndexIndirect                        = 0,
			kPacketCountDrawIndexIndirectMulti                   = 0,
			kPacketCountDrawIndexIndirectCountMulti              = 0,
			kPacketCountSetDispatchDrawIndexDeallocationMask     = 1,
			kPacketCountWaitForSetupDispatchDrawKickRingBuffer   = 1,
			kPacketCountDispatchDraw                             = 0,
			kPacketCountDispatchWithOrderedAppend                = 0,
			kPacketCountDispatchIndirectWithOrderedAppend        = 0,
			kPacketCountDrawIndexMultiInstanced                  = 0,
			kPacketCountWriteOcclusionQuery                      = 4,
			kPacketCountWriteOcclusionQuery2                     = 2,
			kPacketCountSetZPassPredicationEnable                = 1,
			kPacketCountSetZPassPredicationDisable               = 1,
			kPacketCountWriteDataInline                          = 1,
			kPacketCountWriteDataInlineThroughL2                 = 1,
			kPacketCountWriteAtEndOfPipe                         = 1,
			kPacketCountWriteAtEndOfPipeWithInterrupt            = 1,
			kPacketCountTriggerEndOfPipeInterrupt                = 1,
			kPacketCountWriteAtEndOfShader                       = 1,
			kPacketCountStallCommandBufferParser                 = 1,
			kPacketCountWaitOnAddressAndStallCommandBufferParser = 1,
			kPacketCountWaitOnRegister                           = 1,
			kPacketCountFlushShaderCachesAndWait                 = 1,
			kPacketCountSignalSemaphore                          = 1,
			kPacketCountWaitSemaphore                            = 1,
			kPacketCountWriteEventStats                          = 1,
			kPacketCountDmaData                                  = 1,
			kPacketCountDisableOrderedAppendAllocationCounter    = 0,
			kPacketCountEnableOrderedAppendAllocationCounter     = 0,
			kPacketCountSetVsShaderStreamoutEnable               = 1,
			kPacketCountSetupDrawOpaqueParameters                = 3,
			kPacketCountRequestMipStatsReportAndReset            = 1,
			kPacketCountSetGuardBands                            = 1,
			kPacketCountSetPredication                           = 1,
			kPacketCountPrepareFlip                              = 7,
			kPacketCountSetObjectId                              = 1,
			kPacketCountSetObjectIdMode                          = 1,
			kPacketCountSetDrawPayloadControl                    = 1,
			kPacketCountResetFoveatedWindow                      = 1,
			kPacketCountSetFoveatedWindow                        = 1,
			kPacketCountSetPsShaderSampleExclusionMask           = 1,
			kPacketCountSetScaledResolutionGrid                  = 1,
			kPacketCountSetTessellationDistributionThresholds    = 1,
			kPacketCountSetTextureGradientFactors                = 1,
			kPacketCountSetVgtNumInstances                       = 1,
			kPacketCountRttvThreadTraceMarker                    = 2,
			kPacketCountRttvThreadTraceMarker2                   = 1,
			kPacketCountSetContextRegisterWithIndex              = 1,
			kPacketCountSetContextRegister                       = 1,
			kPacketCountSetConfigRegister                        = 1,
			kPacketCountSetUserConfigRegisterWithIndex           = 1,
			kPacketCountSetUserConfigRegister                    = 1,
			kPacketCountSetPersistentRegister                    = 1,
			kPacketCountSetContextRegisterRange                  = 1,
			kPacketCountSetConfigRegisterRange                   = 1,
			kPacketCountSetUserConfigRegisterRange               = 1,
			kPacketCountSetPersistentRegisterRange               = 1,
			kPacketCountChainCommandBuffer                       = 1,
			kPacketCountPause                                    = 1,
			kPacketCountTriggerEvent                             = 1,
			kPacketCountWaitOnAddress                            = 1,
			kPacketCountWaitForGraphicsWrites                    = 1,
			kPacketCountInsertNop                                = 1,
			kPacketCountInsertNop2                               = 1,
			kPacketCountIncrementCeCounter                       = 1,
			kPacketCountIncrementCeCounterForDispatchDraw        = 1,
			kPacketCountWaitOnDeCounterDiff                      = 1,
			kPacketCountWriteToCpRam                             = 1,
			kPacketCountCpRamDump                                = 1,
			kPacketCountChainCommandBuffer2                      = 1,
			kPacketCountCallCommandBuffer                        = 1,
			kPacketCountInitializeDefaultHardwareState2          = 0,
			kPacketCountSetQueuePriority                         = 1,
			kPacketCountDispatchIndirectWithOrderedAppend2       = 0,
			kPacketCountSub_7FFC805A03E0                         = 0,
			kPacketCountSetVsharpInUserData2                     = 2,
			kPacketCountSetTsharpInUserData2                     = 2,
			kPacketCountSetSsharpInUserData2                     = 2,
			kPacketCountSetUserDataRegion2                       = 2,
			kPacketCountSetPointerInUserData2                    = 1,
			kPacketCountSetUserData2                             = 1,
			kPacketCountPrefetchIntoL22                          = 0,
			kPacketCountReadDataFromGds2                         = 1,
			kPacketCountWaitOnAddress2                           = 3,
			kPacketCountDispatchDraw2                            = 0,
			kPacketCountFlushShaderCachesAndWait2                = 1,
			kPacketCountEnableOrderedAppendAllocationCounter2    = 0,
			kPacketCountSetupDispatchDrawKickRingBuffer          = 2,
			kPacketCountSetScratchSize                           = 1,
			kPacketCountTriggerReleaseMemEventInterrupt          = 1,
			kPacketCountWaitForResume                            = 0,
			kPacketCountWriteResume                              = 1,
			kPacketCountEnableQueueQuantumTimer                  = 1,
			kPacketCountDisableQueueQuantumTimer                 = 1,
			kPacketCountInsertRttvDingDongMarker                 = 6,
			kPacketCountRttvThreadTraceMarker3                   = 1,
			kPacketCountSetQueueRegister                         = 1,
			kPacketCountCallCommandBuffer2                       = 1,
			kPacketCountWaitForGraphicsWrites2                   = 1,
			kPacketCountWriteResumeEventWithInterrupt            = 1,
			kPacketCountWriteWaitMemCmd                          = 1,
		};
	public:
		GnmCommandProxy();
		~GnmCommandProxy();

	private:
		uint32_t sub_7FFC805A03D0(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t prepareFlip2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVsharpInUserData(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setTsharpInUserData(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setSsharpInUserData(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserDataRegion(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPointerInUserData(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserData(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t prefetchIntoL2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t requestVideoFlip(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t initializeDefaultHardwareState(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t initializeToDefaultContextState(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t initializeToDefaultContextState2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t initializeToDefaultContextState3(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setupEsGsRingRegisters(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setupGsVsRingRegisters(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t flushStreamout(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setStreamoutBufferDimensions(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setStreamoutMapping(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeStreamoutBufferUpdate(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setComputeShaderControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl3(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl4(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl5(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl6(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsShaderControl7(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setComputeResourceManagement(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setComputeResourceManagement2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setComputeResourceManagementForNeo(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setComputeResourceManagementForBase(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGraphicsScratchSize(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setComputeScratchSize(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setViewportTransformControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setClipControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserClipPlane(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setClipRectangle(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setClipRectangleRule(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPrimitiveSetup(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPrimitiveResetIndexEnable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPrimitiveResetIndex(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVertexQuantization(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setWindowOffset(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setScreenScissor(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setWindowScissor(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGenericScissor(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setViewportScissor(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setViewport(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setScanModeControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setAaSampleCount(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setAaSampleLocationControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPsShaderRate(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setAaSampleMask(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setAaSampleLocations(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setCentroidPriority(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setLineWidth(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPointSize(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPointMinMax(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPolygonOffsetClamp(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPolygonOffsetZFormat(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPolygonOffsetFront(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPolygonOffsetBack(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setHardwareScreenOffset(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGuardBandClip(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGuardBandDiscard(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setInstanceStepRate(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPsShaderUsage(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderOverrideControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderOverride2Control(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setActiveShaderStages(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGsMode(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGsOnChipControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setBorderColorTableAddr(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPsShader2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPsShader3(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setEsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setCsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setLsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setHsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t updatePsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t updateVsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t updateGsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t updateHsShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitOnCe(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t incrementDeCounter(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t readDataFromGds(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderTarget(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderTarget2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderTarget3(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderTarget4(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDepthRenderTarget(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDepthRenderTarget2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDepthClearValue(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setStencilClearValue(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setCmaskClearColor(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setRenderTargetMask(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setBlendControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setBlendColor(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setStencilSeparate(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setAlphaToMaskControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setHtileStencil0(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setHtileStencil1(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setCbControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDepthStencilDisable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDepthBoundsRange(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setStencilOpControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDbRenderControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDbCountControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDepthEqaaControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPrimitiveIdEnable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVgtControlForBase(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVgtControlForNeo(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVgtControlForNeo2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVertexReuseEnable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPrimitiveType(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setIndexSize(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setNumInstances(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setIndexOffset(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setIndexOffset2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawOpaqueAuto(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexAuto(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexInline(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexInline2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndex(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setIndexBuffer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setIndexCount(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexOffset(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setBaseIndirectArgs(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndirect(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndirectMulti(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndirectCountMulti(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexIndirect(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexIndirectMulti(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexIndirectCountMulti(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDispatchDrawIndexDeallocationMask(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitForSetupDispatchDrawKickRingBuffer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t dispatchDraw(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t dispatchWithOrderedAppend(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t dispatchIndirectWithOrderedAppend(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t drawIndexMultiInstanced(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeOcclusionQuery(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeOcclusionQuery2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setZPassPredicationEnable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setZPassPredicationDisable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeDataInline(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeDataInlineThroughL2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeAtEndOfPipe(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeAtEndOfPipeWithInterrupt(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t triggerEndOfPipeInterrupt(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeAtEndOfShader(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t stallCommandBufferParser(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitOnAddressAndStallCommandBufferParser(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitOnRegister(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t flushShaderCachesAndWait(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t signalSemaphore(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitSemaphore(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeEventStats(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t dmaData(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t disableOrderedAppendAllocationCounter(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t enableOrderedAppendAllocationCounter(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVsShaderStreamoutEnable(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setupDrawOpaqueParameters(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t requestMipStatsReportAndReset(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setGuardBands(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPredication(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t prepareFlip(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setObjectId(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setObjectIdMode(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setDrawPayloadControl(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t resetFoveatedWindow(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setFoveatedWindow(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPsShaderSampleExclusionMask(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setScaledResolutionGrid(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setTessellationDistributionThresholds(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setTextureGradientFactors(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVgtNumInstances(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t RttvThreadTraceMarker(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t RttvThreadTraceMarker2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setContextRegisterWithIndex(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setContextRegister(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setConfigRegister(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserConfigRegisterWithIndex(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserConfigRegister(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPersistentRegister(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setContextRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setConfigRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserConfigRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPersistentRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t chainCommandBuffer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t pause(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t triggerEvent(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitOnAddress(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitForGraphicsWrites(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t insertNop(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t insertNop2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t incrementCeCounter(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t incrementCeCounterForDispatchDraw(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitOnDeCounterDiff(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeToCpRam(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t cpRamDump(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t chainCommandBuffer2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t callCommandBuffer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t initializeDefaultHardwareState2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setQueuePriority(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t dispatchIndirectWithOrderedAppend2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t sub_7FFC805A03E0(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setVsharpInUserData2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setTsharpInUserData2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setSsharpInUserData2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserDataRegion2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setPointerInUserData2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setUserData2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t prefetchIntoL22(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t readDataFromGds2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitOnAddress2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t dispatchDraw2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t flushShaderCachesAndWait2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t enableOrderedAppendAllocationCounter2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setupDispatchDrawKickRingBuffer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setScratchSize(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t triggerReleaseMemEventInterrupt(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitForResume(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeResume(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t enableQueueQuantumTimer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t disableQueueQuantumTimer(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t insertRttvDingDongMarker(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t RttvThreadTraceMarker3(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t setQueueRegister(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t callCommandBuffer2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t waitForGraphicsWrites2(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeResumeEventWithInterrupt(PPM4_TYPE_3_HEADER pm4Hdr);
		uint32_t writeWaitMemCmd(PPM4_TYPE_3_HEADER pm4Hdr);

	private:
		using ProxyFunction = uint32_t (GnmCommandProxy::*)(PPM4_TYPE_3_HEADER);
		static const std::array<
			const std::array<ProxyFunction, 88>, 259> m_proxyTable;
	};


}  // namespace sce::Gnm