#include "GnmCommandProxy.h"

namespace sce::Gnm
{
	const std::array<const std::array<GnmCommandProxy::ProxyFunction, 88>, 259> GnmCommandProxy::m_proxyTable = { {
		// IT_UNKNOWN_0
		{ nullptr },
		// IT_UNKNOWN_1
		{ nullptr },
		// IT_UNKNOWN_2
		{ nullptr },
		// IT_UNKNOWN_3
		{ nullptr },
		// IT_UNKNOWN_4
		{ nullptr },
		// IT_UNKNOWN_5
		{ nullptr },
		// IT_UNKNOWN_6
		{ nullptr },
		// IT_UNKNOWN_7
		{ nullptr },
		// IT_UNKNOWN_8
		{ nullptr },
		// IT_UNKNOWN_9
		{ nullptr },
		// IT_UNKNOWN_A
		{ nullptr },
		// IT_UNKNOWN_B
		{ nullptr },
		// IT_UNKNOWN_C
		{ nullptr },
		// IT_UNKNOWN_D
		{ nullptr },
		// IT_UNKNOWN_E
		{ nullptr },
		// IT_UNKNOWN_F
		{ nullptr },
		// IT_NOP
		{
			&GnmCommandProxy::sub_7FFC805A03D0,
			&GnmCommandProxy::setVsharpInUserData,
			&GnmCommandProxy::setTsharpInUserData,
			&GnmCommandProxy::setSsharpInUserData,
			&GnmCommandProxy::setUserDataRegion,
			&GnmCommandProxy::insertNop,
			&GnmCommandProxy::sub_7FFC805A03E0,
			&GnmCommandProxy::setVsharpInUserData2,
			&GnmCommandProxy::setTsharpInUserData2,
			&GnmCommandProxy::setSsharpInUserData2,
			&GnmCommandProxy::setUserDataRegion2,
			nullptr,
		},

		// IT_SET_BASE
		{
			&GnmCommandProxy::setBaseIndirectArgs,
			nullptr,
		},

		// IT_CLEAR_STATE
		{ nullptr },
		// IT_INDEX_BUFFER_SIZE
		{
			&GnmCommandProxy::setIndexCount,
			nullptr,
		},

		// IT_UNKNOWN_14
		{ nullptr },
		// IT_DISPATCH_DIRECT
		{
			&GnmCommandProxy::dispatchWithOrderedAppend,
			nullptr,
		},

		// IT_DISPATCH_INDIRECT
		{
			&GnmCommandProxy::dispatchIndirectWithOrderedAppend,
			&GnmCommandProxy::dispatchIndirectWithOrderedAppend2,
			nullptr,
		},

		// IT_INDIRECT_BUFFER_END
		{ nullptr },
		// IT_UNKNOWN_18
		{ nullptr },
		// IT_INDIRECT_BUFFER_CNST_END
		{ nullptr },
		// IT_UNKNOWN_1A
		{ nullptr },
		// IT_UNKNOWN_1B
		{ nullptr },
		// IT_UNKNOWN_1C
		{ nullptr },
		// IT_ATOMIC_GDS
		{ nullptr },
		// IT_ATOMIC_MEM
		{ nullptr },
		// IT_OCCLUSION_QUERY
		{ nullptr },
		// IT_SET_PREDICATION
		{
			&GnmCommandProxy::setZPassPredicationEnable,
			&GnmCommandProxy::setZPassPredicationDisable,
			nullptr,
		},

		// IT_REG_RMW
		{ nullptr },
		// IT_COND_EXEC
		{
			&GnmCommandProxy::setPredication,
			nullptr,
		},

		// IT_PRED_EXEC
		{ nullptr },
		// IT_DRAW_INDIRECT
		{
			&GnmCommandProxy::drawIndirect,
			nullptr,
		},

		// IT_DRAW_INDEX_INDIRECT
		{
			&GnmCommandProxy::drawIndexIndirect,
			nullptr,
		},

		// IT_INDEX_BASE
		{
			&GnmCommandProxy::setIndexBuffer,
			nullptr,
		},

		// IT_DRAW_INDEX_2
		{
			&GnmCommandProxy::drawIndexInline2,
			&GnmCommandProxy::drawIndex,
			nullptr,
		},

		// IT_CONTEXT_CONTROL
		{
			&GnmCommandProxy::initializeDefaultHardwareState,
			&GnmCommandProxy::initializeToDefaultContextState,
			&GnmCommandProxy::initializeToDefaultContextState2,
			&GnmCommandProxy::initializeToDefaultContextState3,
			nullptr,
		},

		// IT_UNKNOWN_29
		{ nullptr },
		// IT_INDEX_TYPE
		{
			&GnmCommandProxy::setIndexSize,
			nullptr,
		},

		// IT_UNKNOWN_2B
		{ nullptr },
		// IT_DRAW_INDIRECT_MULTI
		{
			&GnmCommandProxy::drawIndirectMulti,
			nullptr,
		},

		// IT_DRAW_INDEX_AUTO
		{
			&GnmCommandProxy::drawOpaqueAuto,
			&GnmCommandProxy::drawIndexAuto,
			nullptr,
		},

		// IT_UNKNOWN_2E
		{
			&GnmCommandProxy::drawIndexInline,
			nullptr,
		},

		// IT_NUM_INSTANCES
		{
			&GnmCommandProxy::setNumInstances,
			nullptr,
		},

		// IT_DRAW_INDEX_MULTI_AUTO
		{ nullptr },
		// IT_UNKNOWN_31
		{ nullptr },
		// IT_INDIRECT_BUFFER_PRIV
		{ nullptr },
		// IT_INDIRECT_BUFFER_CNST
		{
			&GnmCommandProxy::chainCommandBuffer2,
			&GnmCommandProxy::callCommandBuffer,
			nullptr,
		},

		// IT_STRMOUT_BUFFER_UPDATE
		{
			&GnmCommandProxy::writeStreamoutBufferUpdate,
			nullptr,
		},

		// IT_DRAW_INDEX_OFFSET_2
		{
			&GnmCommandProxy::drawIndexOffset,
			nullptr,
		},

		// IT_DRAW_PREAMBLE
		{ nullptr },
		// IT_WRITE_DATA
		{
			&GnmCommandProxy::prepareFlip2,
			&GnmCommandProxy::requestVideoFlip,
			&GnmCommandProxy::writeDataInline,
			&GnmCommandProxy::writeDataInlineThroughL2,
			&GnmCommandProxy::writeResume,
			nullptr,
		},

		// IT_DRAW_INDEX_INDIRECT_MULTI
		{
			&GnmCommandProxy::drawIndexIndirectMulti,
			nullptr,
		},

		// IT_MEM_SEMAPHORE
		{
			&GnmCommandProxy::signalSemaphore,
			&GnmCommandProxy::waitSemaphore,
			nullptr,
		},

		// IT_DRAW_INDEX_MULTI_INST
		{
			&GnmCommandProxy::drawIndexMultiInstanced,
			nullptr,
		},

		// IT_COPY_DW
		{ nullptr },
		// IT_WAIT_REG_MEM
		{
			&GnmCommandProxy::waitOnAddressAndStallCommandBufferParser,
			&GnmCommandProxy::waitOnRegister,
			&GnmCommandProxy::waitOnAddress,
			&GnmCommandProxy::waitForResume,
			&GnmCommandProxy::writeWaitMemCmd,
			nullptr,
		},

		// IT_UNKNOWN_3D
		{ nullptr },
		// IT_UNKNOWN_3E
		{ nullptr },
		// IT_INDIRECT_BUFFER
		{
			&GnmCommandProxy::prepareFlip,
			&GnmCommandProxy::chainCommandBuffer,
			&GnmCommandProxy::callCommandBuffer2,
			nullptr,
		},

		// IT_COPY_DATA
		{ nullptr },
		// IT_CP_DMA
		{ nullptr },
		// IT_PFP_SYNC_ME
		{
			&GnmCommandProxy::stallCommandBufferParser,
			nullptr,
		},

		// IT_SURFACE_SYNC
		{ nullptr },
		// IT_ME_INITIALIZE
		{ nullptr },
		// IT_COND_WRITE
		{ nullptr },
		// IT_EVENT_WRITE
		{
			&GnmCommandProxy::flushStreamout,
			&GnmCommandProxy::writeOcclusionQuery2,
			&GnmCommandProxy::writeEventStats,
			&GnmCommandProxy::RttvThreadTraceMarker,
			&GnmCommandProxy::RttvThreadTraceMarker2,
			&GnmCommandProxy::triggerEvent,
			nullptr,
		},

		// IT_EVENT_WRITE_EOP
		{
			&GnmCommandProxy::writeAtEndOfPipe,
			&GnmCommandProxy::writeAtEndOfPipeWithInterrupt,
			&GnmCommandProxy::triggerEndOfPipeInterrupt,
			nullptr,
		},

		// IT_EVENT_WRITE_EOS
		{
			&GnmCommandProxy::readDataFromGds,
			&GnmCommandProxy::writeAtEndOfShader,
			nullptr,
		},

		// IT_RELEASE_MEM
		{
			&GnmCommandProxy::readDataFromGds2,
			&GnmCommandProxy::triggerReleaseMemEventInterrupt,
			&GnmCommandProxy::writeResumeEventWithInterrupt,
			nullptr,
		},

		// IT_PREAMBLE_CNTL
		{ nullptr },
		// IT_UNKNOWN_4B
		{ nullptr },
		// IT_DRAW_RESERVED0
		{ nullptr },
		// IT_DRAW_RESERVED1
		{ nullptr },
		// IT_DRAW_RESERVED2
		{ nullptr },
		// IT_DRAW_RESERVED3
		{ nullptr },
		// IT_DMA_DATA
		{
			&GnmCommandProxy::prefetchIntoL2,
			&GnmCommandProxy::writeOcclusionQuery,
			&GnmCommandProxy::dmaData,
			&GnmCommandProxy::prefetchIntoL22,
			nullptr,
		},

		// IT_CONTEXT_REG_RMW
		{ nullptr },
		// IT_GFX_CNTX_UPDATE
		{ nullptr },
		// IT_BLK_CNTX_UPDATE
		{ nullptr },
		// IT_UNKNOWN_54
		{ nullptr },
		// IT_INCR_UPDT_STATE
		{ nullptr },
		// IT_UNKNOWN_56
		{ nullptr },
		// IT_UNKNOWN_57
		{ nullptr },
		// IT_ACQUIRE_MEM
		{
			&GnmCommandProxy::flushShaderCachesAndWait,
			&GnmCommandProxy::waitForGraphicsWrites,
			&GnmCommandProxy::flushShaderCachesAndWait2,
			&GnmCommandProxy::waitForGraphicsWrites2,
			nullptr,
		},

		// IT_REWIND
		{
			&GnmCommandProxy::pause,
			nullptr,
		},

		// IT_INTERRUPT
		{ nullptr },
		// IT_GEN_PDEPTE
		{ nullptr },
		// IT_INDIRECT_BUFFER_PASID
		{ nullptr },
		// IT_PRIME_UTCL2
		{ nullptr },
		// IT_LOAD_UCONFIG_REG
		{ nullptr },
		// IT_LOAD_SH_REG
		{ nullptr },
		// IT_LOAD_CONFIG_REG
		{ nullptr },
		// IT_LOAD_CONTEXT_REG
		{ nullptr },
		// IT_LOAD_COMPUTE_STATE
		{ nullptr },
		// IT_LOAD_SH_REG_INDEX
		{ nullptr },
		// IT_UNKNOWN_64
		{ nullptr },
		// IT_UNKNOWN_65
		{ nullptr },
		// IT_UNKNOWN_66
		{ nullptr },
		// IT_UNKNOWN_67
		{ nullptr },
		// IT_SET_CONFIG_REG
		{
			&GnmCommandProxy::setIndexOffset2,
			&GnmCommandProxy::setConfigRegister,
			&GnmCommandProxy::setConfigRegisterRange,
			nullptr,
		},

		// IT_SET_CONTEXT_REG
		{
			&GnmCommandProxy::setupEsGsRingRegisters,
			&GnmCommandProxy::setupGsVsRingRegisters,
			&GnmCommandProxy::setStreamoutBufferDimensions,
			&GnmCommandProxy::setStreamoutMapping,
			&GnmCommandProxy::setGraphicsScratchSize,
			&GnmCommandProxy::setViewportTransformControl,
			&GnmCommandProxy::setClipControl,
			&GnmCommandProxy::setUserClipPlane,
			&GnmCommandProxy::setClipRectangle,
			&GnmCommandProxy::setClipRectangleRule,
			&GnmCommandProxy::setPrimitiveSetup,
			&GnmCommandProxy::setPrimitiveResetIndexEnable,
			&GnmCommandProxy::setPrimitiveResetIndex,
			&GnmCommandProxy::setVertexQuantization,
			&GnmCommandProxy::setWindowOffset,
			&GnmCommandProxy::setScreenScissor,
			&GnmCommandProxy::setWindowScissor,
			&GnmCommandProxy::setGenericScissor,
			&GnmCommandProxy::setViewportScissor,
			&GnmCommandProxy::setViewport,
			&GnmCommandProxy::setScanModeControl,
			&GnmCommandProxy::setAaSampleCount,
			&GnmCommandProxy::setAaSampleLocationControl,
			&GnmCommandProxy::setPsShaderRate,
			&GnmCommandProxy::setAaSampleMask,
			&GnmCommandProxy::setAaSampleLocations,
			&GnmCommandProxy::setCentroidPriority,
			&GnmCommandProxy::setLineWidth,
			&GnmCommandProxy::setPointSize,
			&GnmCommandProxy::setPointMinMax,
			&GnmCommandProxy::setPolygonOffsetClamp,
			&GnmCommandProxy::setPolygonOffsetZFormat,
			&GnmCommandProxy::setPolygonOffsetFront,
			&GnmCommandProxy::setPolygonOffsetBack,
			&GnmCommandProxy::setHardwareScreenOffset,
			&GnmCommandProxy::setGuardBandClip,
			&GnmCommandProxy::setGuardBandDiscard,
			&GnmCommandProxy::setInstanceStepRate,
			&GnmCommandProxy::setPsShaderUsage,
			&GnmCommandProxy::setRenderOverrideControl,
			&GnmCommandProxy::setRenderOverride2Control,
			&GnmCommandProxy::setActiveShaderStages,
			&GnmCommandProxy::setGsMode,
			&GnmCommandProxy::setGsOnChipControl,
			&GnmCommandProxy::setBorderColorTableAddr,
			&GnmCommandProxy::setRenderTarget,
			&GnmCommandProxy::setRenderTarget2,
			&GnmCommandProxy::setRenderTarget3,
			&GnmCommandProxy::setRenderTarget4,
			&GnmCommandProxy::setDepthRenderTarget,
			&GnmCommandProxy::setDepthRenderTarget2,
			&GnmCommandProxy::setDepthClearValue,
			&GnmCommandProxy::setStencilClearValue,
			&GnmCommandProxy::setCmaskClearColor,
			&GnmCommandProxy::setRenderTargetMask,
			&GnmCommandProxy::setBlendControl,
			&GnmCommandProxy::setBlendColor,
			&GnmCommandProxy::setStencilSeparate,
			&GnmCommandProxy::setAlphaToMaskControl,
			&GnmCommandProxy::setHtileStencil0,
			&GnmCommandProxy::setHtileStencil1,
			&GnmCommandProxy::setCbControl,
			&GnmCommandProxy::setDepthStencilDisable,
			&GnmCommandProxy::setDepthBoundsRange,
			&GnmCommandProxy::setStencilOpControl,
			&GnmCommandProxy::setDbRenderControl,
			&GnmCommandProxy::setDbCountControl,
			&GnmCommandProxy::setDepthEqaaControl,
			&GnmCommandProxy::setPrimitiveIdEnable,
			&GnmCommandProxy::setVgtControlForBase,
			&GnmCommandProxy::setVgtControlForNeo,
			&GnmCommandProxy::setVertexReuseEnable,
			&GnmCommandProxy::setIndexOffset,
			&GnmCommandProxy::setDispatchDrawIndexDeallocationMask,
			&GnmCommandProxy::setVsShaderStreamoutEnable,
			&GnmCommandProxy::setupDrawOpaqueParameters,
			&GnmCommandProxy::setGuardBands,
			&GnmCommandProxy::setObjectIdMode,
			&GnmCommandProxy::setDrawPayloadControl,
			&GnmCommandProxy::resetFoveatedWindow,
			&GnmCommandProxy::setFoveatedWindow,
			&GnmCommandProxy::setPsShaderSampleExclusionMask,
			&GnmCommandProxy::setScaledResolutionGrid,
			&GnmCommandProxy::setTessellationDistributionThresholds,
			&GnmCommandProxy::setContextRegisterWithIndex,
			&GnmCommandProxy::setContextRegister,
			&GnmCommandProxy::setContextRegisterRange,
			nullptr,
		},

		// IT_SET_CONTEXT_REG_INDEX
		{ nullptr },
		// IT_UNKNOWN_6B
		{ nullptr },
		// IT_UNKNOWN_6C
		{ nullptr },
		// IT_UNKNOWN_6D
		{ nullptr },
		// IT_UNKNOWN_6E
		{ nullptr },
		// IT_UNKNOWN_6F
		{ nullptr },
		// IT_UNKNOWN_70
		{ nullptr },
		// IT_SET_VGPR_REG_DI_MULTI
		{ nullptr },
		// IT_SET_SH_REG_DI
		{ nullptr },
		// IT_SET_CONTEXT_REG_INDIRECT
		{ nullptr },
		// IT_SET_SH_REG_DI_MULTI
		{ nullptr },
		// IT_GFX_PIPE_LOCK
		{ nullptr },
		// IT_SET_SH_REG
		{
			&GnmCommandProxy::setPointerInUserData,
			&GnmCommandProxy::setUserData,
			&GnmCommandProxy::setComputeShaderControl,
			&GnmCommandProxy::setGraphicsShaderControl,
			&GnmCommandProxy::setGraphicsShaderControl2,
			&GnmCommandProxy::setGraphicsShaderControl3,
			&GnmCommandProxy::setGraphicsShaderControl4,
			&GnmCommandProxy::setGraphicsShaderControl5,
			&GnmCommandProxy::setGraphicsShaderControl6,
			&GnmCommandProxy::setGraphicsShaderControl7,
			&GnmCommandProxy::setComputeResourceManagement,
			&GnmCommandProxy::setComputeResourceManagement2,
			&GnmCommandProxy::setComputeResourceManagementForNeo,
			&GnmCommandProxy::setComputeResourceManagementForBase,
			&GnmCommandProxy::setComputeScratchSize,
			&GnmCommandProxy::setPsShader,
			&GnmCommandProxy::setPsShader2,
			&GnmCommandProxy::setPsShader3,
			&GnmCommandProxy::setVsShader,
			&GnmCommandProxy::setEsShader,
			&GnmCommandProxy::setGsShader,
			&GnmCommandProxy::setCsShader,
			&GnmCommandProxy::setLsShader,
			&GnmCommandProxy::setHsShader,
			&GnmCommandProxy::updatePsShader,
			&GnmCommandProxy::updateVsShader,
			&GnmCommandProxy::updateGsShader,
			&GnmCommandProxy::updateHsShader,
			&GnmCommandProxy::setPersistentRegister,
			&GnmCommandProxy::setPersistentRegisterRange,
			&GnmCommandProxy::initializeDefaultHardwareState2,
			&GnmCommandProxy::setPointerInUserData2,
			&GnmCommandProxy::setUserData2,
			&GnmCommandProxy::setScratchSize,
			nullptr,
		},

		// IT_SET_SH_REG_OFFSET
		{ nullptr },
		// IT_SET_QUEUE_REG
		{
			&GnmCommandProxy::setQueuePriority,
			&GnmCommandProxy::enableQueueQuantumTimer,
			&GnmCommandProxy::disableQueueQuantumTimer,
			&GnmCommandProxy::setQueueRegister,
			nullptr,
		},

		// IT_SET_UCONFIG_REG
		{
			&GnmCommandProxy::setVgtControlForNeo2,
			&GnmCommandProxy::setPrimitiveType,
			&GnmCommandProxy::disableOrderedAppendAllocationCounter,
			&GnmCommandProxy::enableOrderedAppendAllocationCounter,
			&GnmCommandProxy::setObjectId,
			&GnmCommandProxy::setTextureGradientFactors,
			&GnmCommandProxy::setVgtNumInstances,
			&GnmCommandProxy::setUserConfigRegisterWithIndex,
			&GnmCommandProxy::setUserConfigRegister,
			&GnmCommandProxy::setUserConfigRegisterRange,
			&GnmCommandProxy::waitOnAddress2,
			&GnmCommandProxy::enableOrderedAppendAllocationCounter2,
			&GnmCommandProxy::insertRttvDingDongMarker,
			&GnmCommandProxy::RttvThreadTraceMarker3,
			nullptr,
		},

		// IT_SET_UCONFIG_REG_INDEX
		{ nullptr },
		// IT_UNKNOWN_7B
		{ nullptr },
		// IT_FORWARD_HEADER
		{ nullptr },
		// IT_SCRATCH_RAM_WRITE
		{ nullptr },
		// IT_SCRATCH_RAM_READ
		{ nullptr },
		// IT_UNKNOWN_7F
		{ nullptr },
		// IT_LOAD_CONST_RAM
		{ nullptr },
		// IT_WRITE_CONST_RAM
		{
			&GnmCommandProxy::writeToCpRam,
			nullptr,
		},

		// IT_UNKNOWN_82
		{ nullptr },
		// IT_DUMP_CONST_RAM
		{
			&GnmCommandProxy::cpRamDump,
			nullptr,
		},

		// IT_INCREMENT_CE_COUNTER
		{
			&GnmCommandProxy::incrementCeCounter,
			&GnmCommandProxy::incrementCeCounterForDispatchDraw,
			nullptr,
		},

		// IT_INCREMENT_DE_COUNTER
		{
			&GnmCommandProxy::incrementDeCounter,
			nullptr,
		},

		// IT_WAIT_ON_CE_COUNTER
		{
			&GnmCommandProxy::waitOnCe,
			nullptr,
		},

		// IT_UNKNOWN_87
		{ nullptr },
		// IT_WAIT_ON_DE_COUNTER_DIFF
		{
			&GnmCommandProxy::waitOnDeCounterDiff,
			nullptr,
		},

		// IT_UNKNOWN_89
		{ nullptr },
		// IT_UNKNOWN_8A
		{ nullptr },
		// IT_SWITCH_BUFFER
		{ nullptr },
		// IT_DISPATCH_DRAW_PREAMBLE__GFX09
		{
			&GnmCommandProxy::waitForSetupDispatchDrawKickRingBuffer,
			&GnmCommandProxy::setupDispatchDrawKickRingBuffer,
			nullptr,
		},

		// IT_UNKNOWN_8D
		{
			&GnmCommandProxy::dispatchDraw,
			&GnmCommandProxy::dispatchDraw2,
			nullptr,
		},

		// IT_GET_LOD_STATS__GFX09
		{
			&GnmCommandProxy::requestMipStatsReportAndReset,
			nullptr,
		},

		// IT_DRAW_MULTI_PREAMBLE__GFX09
		{ nullptr },
		// IT_FRAME_CONTROL
		{ nullptr },
		// IT_INDEX_ATTRIBUTES_INDIRECT
		{ nullptr },
		// IT_UNKNOWN_92
		{ nullptr },
		// IT_WAIT_REG_MEM64
		{ nullptr },
		// IT_COND_PREEMPT
		{ nullptr },
		// IT_HDP_FLUSH
		{ nullptr },
		// IT_UNKNOWN_96
		{ nullptr },
		// IT_UNKNOWN_97
		{ nullptr },
		// IT_INVALIDATE_TLBS
		{ nullptr },
		// IT_UNKNOWN_99
		{ nullptr },
		// IT_DMA_DATA_FILL_MULTI
		{ nullptr },
		// IT_SET_SH_REG_INDEX
		{ nullptr },
		// IT_DRAW_INDIRECT_COUNT_MULTI
		{
			&GnmCommandProxy::drawIndirectCountMulti,
			nullptr,
		},

		// IT_DRAW_INDEX_INDIRECT_COUNT_MULTI
		{
			&GnmCommandProxy::drawIndexIndirectCountMulti,
			nullptr,
		},

		// IT_DUMP_CONST_RAM_OFFSET
		{ nullptr },
		// IT_LOAD_CONTEXT_REG_INDEX
		{ nullptr },
		// IT_SET_RESOURCES
		{ nullptr },
		// IT_MAP_PROCESS
		{ nullptr },
		// IT_MAP_QUEUES
		{ nullptr },
		// IT_UNMAP_QUEUES
		{ nullptr },
		// IT_QUERY_STATUS
		{ nullptr },
		// IT_RUN_LIST
		{ nullptr },
		// IT_MAP_PROCESS_VM
		{ nullptr },
		// IT_UNKNOWN_A7
		{ nullptr },
		// IT_UNKNOWN_A8
		{ nullptr },
		// IT_UNKNOWN_A9
		{ nullptr },
		// IT_UNKNOWN_AA
		{ nullptr },
		// IT_UNKNOWN_AB
		{ nullptr },
		// IT_UNKNOWN_AC
		{ nullptr },
		// IT_UNKNOWN_AD
		{ nullptr },
		// IT_UNKNOWN_AE
		{ nullptr },
		// IT_UNKNOWN_AF
		{ nullptr },
		// IT_UNKNOWN_B0
		{ nullptr },
		// IT_UNKNOWN_B1
		{ nullptr },
		// IT_UNKNOWN_B2
		{ nullptr },
		// IT_UNKNOWN_B3
		{ nullptr },
		// IT_UNKNOWN_B4
		{ nullptr },
		// IT_UNKNOWN_B5
		{ nullptr },
		// IT_UNKNOWN_B6
		{ nullptr },
		// IT_UNKNOWN_B7
		{ nullptr },
		// IT_UNKNOWN_B8
		{ nullptr },
		// IT_UNKNOWN_B9
		{ nullptr },
		// IT_UNKNOWN_BA
		{ nullptr },
		// IT_UNKNOWN_BB
		{ nullptr },
		// IT_UNKNOWN_BC
		{ nullptr },
		// IT_UNKNOWN_BD
		{ nullptr },
		// IT_UNKNOWN_BE
		{ nullptr },
		// IT_UNKNOWN_BF
		{ nullptr },
		// IT_UNKNOWN_C0
		{ nullptr },
		// IT_UNKNOWN_C1
		{ nullptr },
		// IT_UNKNOWN_C2
		{ nullptr },
		// IT_UNKNOWN_C3
		{ nullptr },
		// IT_UNKNOWN_C4
		{ nullptr },
		// IT_UNKNOWN_C5
		{ nullptr },
		// IT_UNKNOWN_C6
		{ nullptr },
		// IT_UNKNOWN_C7
		{ nullptr },
		// IT_UNKNOWN_C8
		{ nullptr },
		// IT_UNKNOWN_C9
		{ nullptr },
		// IT_UNKNOWN_CA
		{ nullptr },
		// IT_UNKNOWN_CB
		{ nullptr },
		// IT_UNKNOWN_CC
		{ nullptr },
		// IT_UNKNOWN_CD
		{ nullptr },
		// IT_UNKNOWN_CE
		{ nullptr },
		// IT_UNKNOWN_CF
		{ nullptr },
		// IT_UNKNOWN_D0
		{ nullptr },
		// IT_UNKNOWN_D1
		{ nullptr },
		// IT_UNKNOWN_D2
		{ nullptr },
		// IT_UNKNOWN_D3
		{ nullptr },
		// IT_UNKNOWN_D4
		{ nullptr },
		// IT_UNKNOWN_D5
		{ nullptr },
		// IT_UNKNOWN_D6
		{ nullptr },
		// IT_UNKNOWN_D7
		{ nullptr },
		// IT_UNKNOWN_D8
		{ nullptr },
		// IT_UNKNOWN_D9
		{ nullptr },
		// IT_UNKNOWN_DA
		{ nullptr },
		// IT_UNKNOWN_DB
		{ nullptr },
		// IT_UNKNOWN_DC
		{ nullptr },
		// IT_UNKNOWN_DD
		{ nullptr },
		// IT_UNKNOWN_DE
		{ nullptr },
		// IT_UNKNOWN_DF
		{ nullptr },
		// IT_UNKNOWN_E0
		{ nullptr },
		// IT_UNKNOWN_E1
		{ nullptr },
		// IT_UNKNOWN_E2
		{ nullptr },
		// IT_UNKNOWN_E3
		{ nullptr },
		// IT_UNKNOWN_E4
		{ nullptr },
		// IT_UNKNOWN_E5
		{ nullptr },
		// IT_UNKNOWN_E6
		{ nullptr },
		// IT_UNKNOWN_E7
		{ nullptr },
		// IT_UNKNOWN_E8
		{ nullptr },
		// IT_UNKNOWN_E9
		{ nullptr },
		// IT_UNKNOWN_EA
		{ nullptr },
		// IT_UNKNOWN_EB
		{ nullptr },
		// IT_UNKNOWN_EC
		{ nullptr },
		// IT_UNKNOWN_ED
		{ nullptr },
		// IT_UNKNOWN_EE
		{ nullptr },
		// IT_UNKNOWN_EF
		{ nullptr },
		// IT_UNKNOWN_F0
		{ nullptr },
		// IT_UNKNOWN_F1
		{ nullptr },
		// IT_UNKNOWN_F2
		{ nullptr },
		// IT_UNKNOWN_F3
		{ nullptr },
		// IT_UNKNOWN_F4
		{ nullptr },
		// IT_UNKNOWN_F5
		{ nullptr },
		// IT_UNKNOWN_F6
		{ nullptr },
		// IT_UNKNOWN_F7
		{ nullptr },
		// IT_UNKNOWN_F8
		{ nullptr },
		// IT_UNKNOWN_F9
		{ nullptr },
		// IT_UNKNOWN_FA
		{ nullptr },
		// IT_UNKNOWN_FB
		{ nullptr },
		// IT_UNKNOWN_FC
		{ nullptr },
		// IT_UNKNOWN_FD
		{ nullptr },
		// IT_UNKNOWN_FE
		{ nullptr },
		// IT_UNKNOWN_FF
		{ nullptr },
		// IT_UNKNOWN_100
		{ nullptr },
		// IT_UNKNOWN_101
		{ nullptr },
		// IT_UNKNOWN_102
		{
			&GnmCommandProxy::insertNop2,
			nullptr,
		},
	} };
}  // namespace sce::Gnm