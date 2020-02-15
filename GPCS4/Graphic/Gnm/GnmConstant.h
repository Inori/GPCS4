#pragma once

enum PrimitiveType
{
	kPrimitiveTypeNone = 0x00000000,
	kPrimitiveTypePointList = 0x00000001,
	kPrimitiveTypeLineList = 0x00000002,
	kPrimitiveTypeLineStrip = 0x00000003,
	kPrimitiveTypeTriList = 0x00000004,
	kPrimitiveTypeTriFan = 0x00000005,
	kPrimitiveTypeTriStrip = 0x00000006,
	kPrimitiveTypePatch = 0x00000009,
	kPrimitiveTypeLineListAdjacency = 0x0000000a,
	kPrimitiveTypeLineStripAdjacency = 0x0000000b,
	kPrimitiveTypeTriListAdjacency = 0x0000000c,
	kPrimitiveTypeTriStripAdjacency = 0x0000000d,
	kPrimitiveTypeRectList = 0x00000011,
	kPrimitiveTypeLineLoop = 0x00000012,
	kPrimitiveTypeQuadList = 0x00000013,
	kPrimitiveTypeQuadStrip = 0x00000014,
	kPrimitiveTypePolygon = 0x00000015,
};

enum ShaderStage
{
	kShaderStageCs = 0x00000000,
	kShaderStagePs = 0x00000001,
	kShaderStageVs = 0x00000002,
	kShaderStageGs = 0x00000003,
	kShaderStageEs = 0x00000004,
	kShaderStageHs = 0x00000005,
	kShaderStageLs = 0x00000006,
	kShaderStageCount											
};

enum VgtPartialVsWaveMode
{
	kVgtPartialVsWaveDisable = 0,
	kVgtPartialVsWaveEnable = 1,
};

enum WdSwitchOnlyOnEopMode
{
	kWdSwitchOnlyOnEopDisable = 0,
	kWdSwitchOnlyOnEopEnable = 1,
};

enum EmbeddedDataAlignment
{
	kEmbeddedDataAlignment4 = 0x2,
	kEmbeddedDataAlignment8 = 0x3,
	kEmbeddedDataAlignment16 = 0x4,
	kEmbeddedDataAlignment64 = 0x6,
	kEmbeddedDataAlignment128 = 0x7,
	kEmbeddedDataAlignment256 = 0x8,
};

enum DispatchOrderedAppendMode
{
	kDispatchOrderedAppendModeDisabled = 0,
	kDispatchOrderedAppendModeIndexPerWavefront = 1,
	kDispatchOrderedAppendModeIndexPerThreadgroup = 3,
};

enum GsMaxOutputPrimitiveDwordSize
{
	kGsMaxOutputPrimitiveDwordSize1024 = 0x00000000,
	kGsMaxOutputPrimitiveDwordSize512 = 0x00000001,
	kGsMaxOutputPrimitiveDwordSize256 = 0x00000002,
	kGsMaxOutputPrimitiveDwordSize128 = 0x00000003,
};

enum CacheAction
{
	kCacheActionNone = 0x00,
	kCacheActionWriteBackAndInvalidateL1andL2 = 0x38,
	kCacheActionWriteBackL2Volatile = 0x3B,
	kCacheActionWriteBackAndInvalidateL2Volatile = 0x3B,
	kCacheActionInvalidateL2Volatile = 0x33,
	kCacheActionInvalidateL1 = 0x10,
};

enum StallCommandBufferParserMode
{
	kStallCommandBufferParserEnable = 0x00000000,
	kStallCommandBufferParserDisable = 0x00000001,
};

enum ShaderEngine
{
	kShaderEngine0 = 0,
	kShaderEngine1 = 1,
	kShaderEngine2 = 2,
	kShaderEngine3 = 3,
};

enum EndOfPipeEventType
{
	kEopFlushCbDbCaches = 0x00000004,
	kEopFlushAndInvalidateCbDbCaches = 0x00000014,
	kEopCbDbReadsDone = 0x00000028,
	kEopCsDone = 0x00000028,
};

enum EventWriteDest
{
	kEventWriteDestMemory = 0x00,
	kEventWriteDestTcL2 = 0x01,
	kEventWriteDestTcL2Volatile = 0x11,
};

enum EventWriteSource
{
	kEventWriteSource32BitsImmediate = 0x1,
	kEventWriteSource64BitsImmediate = 0x2,
	kEventWriteSourceGlobalClockCounter = 0x3,
	kEventWriteSourceGpuCoreClockCounter = 0x4,
};

enum CachePolicy
{
	kCachePolicyLru = 0x0,
	kCachePolicyStream = 0x1,
	kCachePolicyBypass = 0x2,
};

enum EmbeddedVsShader
{
	kEmbeddedVsShaderFullScreen = 0,
	kNumEmbeddedVsShaders
};

// dfmt values
enum BufferFormat
{
	kBufferFormatInvalid =	0x00000000,  //No format (special case that disables buffer access)
	kBufferFormat8 =		0x00000001,  //8 bits, 1 component, bits [7:0]
	kBufferFormat16 =		0x00000002,  //16 bits, 1 component, bits [15:0]
	kBufferFormat8_8 =		0x00000003,  //16 bits, 2 components, bits [7:0], [15:8]
	kBufferFormat32 =		0x00000004,  //32 bits, 1 component, bits [31:0]
	kBufferFormat16_16 =	0x00000005,  //32 bits, 2 components, bits [15:0], [31:16]
	kBufferFormat10_11_11 = 0x00000006,  //32 bits, 3 components, bits [9:0], [20:10], [31:21]
	kBufferFormat11_11_10 = 0x00000007,  //32 bits, 3 components, bits [10:0], [21:11], [31:22]
	kBufferFormat10_10_10_2 =	0x00000008,  //32 bits, 4 components, bits [9:0], [19:10], [29:20],[31:30]
	kBufferFormat2_10_10_10 =	0x00000009,  //32 bits, 4 components, bits [1:0], [11:2], [21:12],[31:22]
	kBufferFormat8_8_8_8 =		0x0000000a,  //32 bits, 4 components, bits [7:0], [15:8], [23:16],[31:24]
	kBufferFormat32_32 =		0x0000000b,  //64 bits, 2 components, bits [31:0], [63:32]
	kBufferFormat16_16_16_16 =	0x0000000c,  //64 bits, 4 components, bits [15:0], [31:16], [47:32],[63:48]
	kBufferFormat32_32_32 =		0x0000000d,  //96 bits, 3 components, bits [31:0], [63:32], [95:64]
	kBufferFormat32_32_32_32 =	0x0000000e,  //128 bits, 4 components, bits [31:0], [63:32], [95:64],[127:96]
};

// nfmt values
enum RenderTargetChannelType
{
	kRenderTargetChannelTypeUNorm = 0x00000000,
	kRenderTargetChannelTypeSNorm = 0x00000001,
	kRenderTargetChannelTypeUInt = 0x00000004,
	kRenderTargetChannelTypeSInt = 0x00000005,
	kRenderTargetChannelTypeSrgb = 0x00000006,
	kRenderTargetChannelTypeFloat = 0x00000007,
};

enum TileMode
{

	kTileModeDepth_2dThin_64 = 0x00000000,
	kTileModeDepth_2dThin_128 = 0x00000001,
	kTileModeDepth_2dThin_256 = 0x00000002,
	kTileModeDepth_2dThin_512 = 0x00000003,
	kTileModeDepth_2dThin_1K = 0x00000004,
	kTileModeDepth_1dThin = 0x00000005,
	kTileModeDepth_2dThinPrt_256 = 0x00000006,
	kTileModeDepth_2dThinPrt_1K = 0x00000007,

	kTileModeDisplay_LinearAligned = 0x00000008,
	kTileModeDisplay_1dThin = 0x00000009,
	kTileModeDisplay_2dThin = 0x0000000A,
	kTileModeDisplay_ThinPrt = 0x0000000B,
	kTileModeDisplay_2dThinPrt = 0x0000000C,

	kTileModeThin_1dThin = 0x0000000D,
	kTileModeThin_2dThin = 0x0000000E,
	kTileModeThin_3dThin = 0x0000000F,
	kTileModeThin_ThinPrt = 0x00000010,
	kTileModeThin_2dThinPrt = 0x00000011,
	kTileModeThin_3dThinPrt = 0x00000012,

	kTileModeThick_1dThick = 0x00000013,
	kTileModeThick_2dThick = 0x00000014,
	kTileModeThick_3dThick = 0x00000015,
	kTileModeThick_ThickPrt = 0x00000016,
	kTileModeThick_2dThickPrt = 0x00000017,
	kTileModeThick_3dThickPrt = 0x00000018,
	kTileModeThick_2dXThick = 0x00000019,
	kTileModeThick_3dXThick = 0x0000001A,

	kTileModeDisplay_LinearGeneral = 0x0000001F,
};

enum RenderTargetFormat
{
	kRenderTargetFormatInvalid = 0x00000000,
	kRenderTargetFormat8 = 0x00000001,
	kRenderTargetFormat16 = 0x00000002,
	kRenderTargetFormat8_8 = 0x00000003,
	kRenderTargetFormat32 = 0x00000004,
	kRenderTargetFormat16_16 = 0x00000005,
	kRenderTargetFormat10_11_11 = 0x00000006,
	kRenderTargetFormat11_11_10 = 0x00000007,
	kRenderTargetFormat10_10_10_2 = 0x00000008,
	kRenderTargetFormat2_10_10_10 = 0x00000009,
	kRenderTargetFormat8_8_8_8 = 0x0000000a,
	kRenderTargetFormat32_32 = 0x0000000b,
	kRenderTargetFormat16_16_16_16 = 0x0000000c,
	kRenderTargetFormat32_32_32_32 = 0x0000000e,
	kRenderTargetFormat5_6_5 = 0x00000010,
	kRenderTargetFormat1_5_5_5 = 0x00000011,
	kRenderTargetFormat5_5_5_1 = 0x00000012,
	kRenderTargetFormat4_4_4_4 = 0x00000013,
	kRenderTargetFormat8_24 = 0x00000014,
	kRenderTargetFormat24_8 = 0x00000015,
	kRenderTargetFormatX24_8_32 = 0x00000016,
};

enum SurfaceFormat
{
	kSurfaceFormatInvalid = 0x00000000,
	kSurfaceFormat8 = 0x00000001,
	kSurfaceFormat16 = 0x00000002,
	kSurfaceFormat8_8 = 0x00000003,
	kSurfaceFormat32 = 0x00000004,
	kSurfaceFormat16_16 = 0x00000005,
	kSurfaceFormat10_11_11 = 0x00000006,
	kSurfaceFormat11_11_10 = 0x00000007,
	kSurfaceFormat10_10_10_2 = 0x00000008,
	kSurfaceFormat2_10_10_10 = 0x00000009,
	kSurfaceFormat8_8_8_8 = 0x0000000a,
	kSurfaceFormat32_32 = 0x0000000b,
	kSurfaceFormat16_16_16_16 = 0x0000000c,
	kSurfaceFormat32_32_32 = 0x0000000d,
	kSurfaceFormat32_32_32_32 = 0x0000000e,
	kSurfaceFormat5_6_5 = 0x00000010,
	kSurfaceFormat1_5_5_5 = 0x00000011,
	kSurfaceFormat5_5_5_1 = 0x00000012,
	kSurfaceFormat4_4_4_4 = 0x00000013,
	kSurfaceFormat8_24 = 0x00000014,
	kSurfaceFormat24_8 = 0x00000015,
	kSurfaceFormatX24_8_32 = 0x00000016,
	kSurfaceFormatGB_GR = 0x00000020,
	kSurfaceFormatBG_RG = 0x00000021,
	kSurfaceFormat5_9_9_9 = 0x00000022,
	kSurfaceFormatBc1 = 0x00000023,
	kSurfaceFormatBc2 = 0x00000024,
	kSurfaceFormatBc3 = 0x00000025,
	kSurfaceFormatBc4 = 0x00000026,
	kSurfaceFormatBc5 = 0x00000027,
	kSurfaceFormatBc6 = 0x00000028,
	kSurfaceFormatBc7 = 0x00000029,
	kSurfaceFormatFmask8_S2_F1 = 0x0000002C,
	kSurfaceFormatFmask8_S4_F1 = 0x0000002D,
	kSurfaceFormatFmask8_S8_F1 = 0x0000002E,
	kSurfaceFormatFmask8_S2_F2 = 0x0000002F,
	kSurfaceFormatFmask8_S4_F2 = 0x00000030,
	kSurfaceFormatFmask8_S4_F4 = 0x00000031,
	kSurfaceFormatFmask16_S16_F1 = 0x00000032,
	kSurfaceFormatFmask16_S8_F2 = 0x00000033,
	kSurfaceFormatFmask32_S16_F2 = 0x00000034,
	kSurfaceFormatFmask32_S8_F4 = 0x00000035,
	kSurfaceFormatFmask32_S8_F8 = 0x00000036,
	kSurfaceFormatFmask64_S16_F4 = 0x00000037,
	kSurfaceFormatFmask64_S16_F8 = 0x00000038,
	kSurfaceFormat4_4 = 0x00000039,
	kSurfaceFormat6_5_5 = 0x0000003A,
	kSurfaceFormat1 = 0x0000003B,
	kSurfaceFormat1Reversed = 0x0000003C,
};

enum TextureChannelType
{
	kTextureChannelTypeUNorm = 0x00000000,
	kTextureChannelTypeSNorm = 0x00000001,
	kTextureChannelTypeUScaled = 0x00000002,
	kTextureChannelTypeSScaled = 0x00000003,
	kTextureChannelTypeUInt = 0x00000004,
	kTextureChannelTypeSInt = 0x00000005,
	kTextureChannelTypeSNormNoZero = 0x00000006,
	kTextureChannelTypeFloat = 0x00000007,

	kTextureChannelTypeSrgb = 0x00000009,
	kTextureChannelTypeUBNorm = 0x0000000A,
	kTextureChannelTypeUBNormNoZero = 0x0000000B,
	kTextureChannelTypeUBInt = 0x0000000C,
	kTextureChannelTypeUBScaled = 0x0000000D,
};

enum TextureChannel
{
	kTextureChannelConstant0 = 0x00000000,
	kTextureChannelConstant1 = 0x00000001,
	kTextureChannelX = 0x00000004,
	kTextureChannelY = 0x00000005,
	kTextureChannelZ = 0x00000006,
	kTextureChannelW = 0x00000007,
};

enum RenderTargetChannelOrder
{
	kRenderTargetChannelOrderStandard = 0x00000000,
	kRenderTargetChannelOrderAlt = 0x00000001,
	kRenderTargetChannelOrderReversed = 0x00000002,
	kRenderTargetChannelOrderAltReversed = 0x00000003,
};

enum NumSamples
{
	kNumSamples1 = 0x0,
	kNumSamples2 = 0x1,
	kNumSamples4 = 0x2,
	kNumSamples8 = 0x3,
	kNumSamples16 = 0x4,
};

enum NumFragments
{
	kNumFragments1 = 0x0,
	kNumFragments2 = 0x1,
	kNumFragments4 = 0x2,
	kNumFragments8 = 0x3,
};

enum GpuMode
{
	kGpuModeBase = 0,
	kGpuModeNeo = 1,
};

enum ZFormat
{
	kZFormatInvalid = 0x00000000,
	kZFormat16 = 0x00000001,
	kZFormat32Float = 0x00000003,
};

enum StencilFormat
{
	kStencilInvalid = 0x00000000,
	kStencil8 = 0x00000001,
};

enum DepthControlZWrite
{
	kDepthControlZWriteDisable = 0, 
	kDepthControlZWriteEnable = 1,
};

enum CompareFunc
{
	kCompareFuncNever = 0x00000000,
	kCompareFuncLess = 0x00000001,
	kCompareFuncEqual = 0x00000002,
	kCompareFuncLessEqual = 0x00000003,
	kCompareFuncGreater = 0x00000004,
	kCompareFuncNotEqual = 0x00000005,
	kCompareFuncGreaterEqual = 0x00000006,
	kCompareFuncAlways = 0x00000007,
};

enum PrimitiveSetupCullFaceMode
{
	kPrimitiveSetupCullFaceNone = 0,
	kPrimitiveSetupCullFaceFront = 1,
	kPrimitiveSetupCullFaceBack = 2,
	kPrimitiveSetupCullFaceFrontAndBack = 3,
};

enum PrimitiveSetupFrontFace
{
	kPrimitiveSetupFrontFaceCw = 1,
	kPrimitiveSetupFrontFaceCcw = 0,
};

enum PrimitiveSetupPolygonMode
{
	kPrimitiveSetupPolygonModePoint = 0,
	kPrimitiveSetupPolygonModeLine = 1,
	kPrimitiveSetupPolygonModeFill = 2,
};

enum PrimitiveSetupPolygonOffsetMode
{
	kPrimitiveSetupPolygonOffsetEnable = 1,
	kPrimitiveSetupPolygonOffsetDisable = 0,
};

enum PrimitiveSetupProvokingVertexMode
{
	kPrimitiveSetupProvokingVertexFirst = 0,
	kPrimitiveSetupProvokingVertexLast = 1,
};

enum BlendMultiplier
{
	kBlendMultiplierZero = 0x00000000,
	kBlendMultiplierOne = 0x00000001,
	kBlendMultiplierSrcColor = 0x00000002,
	kBlendMultiplierOneMinusSrcColor = 0x00000003,
	kBlendMultiplierSrcAlpha = 0x00000004,
	kBlendMultiplierOneMinusSrcAlpha = 0x00000005,
	kBlendMultiplierDestAlpha = 0x00000006,
	kBlendMultiplierOneMinusDestAlpha = 0x00000007,
	kBlendMultiplierDestColor = 0x00000008,
	kBlendMultiplierOneMinusDestColor = 0x00000009,
	kBlendMultiplierSrcAlphaSaturate = 0x0000000a,
	kBlendMultiplierConstantColor = 0x0000000d,
	kBlendMultiplierOneMinusConstantColor = 0x0000000e,
	kBlendMultiplierSrc1Color = 0x0000000f,
	kBlendMultiplierInverseSrc1Color = 0x00000010,
	kBlendMultiplierSrc1Alpha = 0x00000011,
	kBlendMultiplierInverseSrc1Alpha = 0x00000012,
	kBlendMultiplierConstantAlpha = 0x00000013,
	kBlendMultiplierOneMinusConstantAlpha = 0x00000014,
};

enum BlendFunc
{
	kBlendFuncAdd = 0x00000000,
	kBlendFuncSubtract = 0x00000001,
	kBlendFuncMin = 0x00000002,
	kBlendFuncMax = 0x00000003,
	kBlendFuncReverseSubtract = 0x00000004,
};

enum ActiveShaderStages
{
	kActiveShaderStagesVsPs = 0x00000000,
	kActiveShaderStagesEsGsVsPs = 0x000000B0,
	kActiveShaderStagesLsHsVsPs = 0x00000045,
	kActiveShaderStagesOffChipLsHsVsPs = 0x00000145,
	kActiveShaderStagesLsHsEsGsVsPs = 0x000000AD,
	kActiveShaderStagesOffChipLsHsEsGsVsPs = 0x000001AD,
	kActiveShaderStagesDispatchDrawVsPs = 0x00000200,
};

enum IndexSize
{
	kIndexSize16 = 0x00000000,
	kIndexSize32 = 0x00000001,
	kIndexSize16ForDispatchDraw = 0x00000010,
	kIndexSize32ForDispatchDraw = 0x00000011,
};

enum ResourceMemoryType
{
	kResourceMemoryTypePV = 0x60,
	kResourceMemoryTypeGC = 0x6D,
	kResourceMemoryTypeSC = 0x6E,
	kResourceMemoryTypeUC = 0x6F,
	kResourceMemoryTypeRO = 0x10,
};

enum BufferSwizzleElementSize
{
	kBufferSwizzleElementSize2 = 0x0,
	kBufferSwizzleElementSize4 = 0x1,
	kBufferSwizzleElementSize8 = 0x2,
	kBufferSwizzleElementSize16 = 0x3,
};

enum BufferSwizzleStride
{
	kBufferSwizzleStride8 = 0x0,
	kBufferSwizzleStride16 = 0x1,
	kBufferSwizzleStride32 = 0x2,
	kBufferSwizzleStride64 = 0x3,
};

enum TextureType
{
	kTextureType1d = 0x00000008,
	kTextureType2d = 0x00000009,
	kTextureType3d = 0x0000000A,
	kTextureTypeCubemap = 0x0000000B,
	kTextureType1dArray = 0x0000000C,
	kTextureType2dArray = 0x0000000D,
	kTextureType2dMsaa = 0x0000000E,
	kTextureType2dArrayMsaa = 0x0000000F,
};

enum SamplerModulationFactor
{
	kSamplerModulationFactor0_0000 = 0x0,
	kSamplerModulationFactor0_1250 = 0x1,
	kSamplerModulationFactor0_3125 = 0x2,
	kSamplerModulationFactor0_4375 = 0x3,
	kSamplerModulationFactor0_5625 = 0x4,
	kSamplerModulationFactor0_6875 = 0x5,
	kSamplerModulationFactor0_8750 = 0x6,
	kSamplerModulationFactor1_0000 = 0x7,
};

enum TextureMetadataType
{
	kTextureMetadataTypeDcc = 0,
	kTextureMetadataTypeCmask = 1,
	kTextureMetadataTypeHtile = 2,
	kTextureMetadataTypeNone = 3,
};

enum DccColorTransform
{
	kDccColorTransformAuto = 0,
	kDccColorTransformNone = 1,
	kDccColorTransformAbgr = 2,
	kDccColorTransformBgra = 3,
};

enum WrapMode
{
	kWrapModeWrap = 0x00000000,
	kWrapModeMirror = 0x00000001,
	kWrapModeClampLastTexel = 0x00000002,
	kWrapModeMirrorOnceLastTexel = 0x00000003,
	kWrapModeClampHalfBorder = 0x00000004,
	kWrapModeMirrorOnceHalfBorder = 0x00000005,
	kWrapModeClampBorder = 0x00000006,
	kWrapModeMirrorOnceBorder = 0x00000007,
};

enum FilterMode
{
	kFilterModePoint = 0x00000000,
	kFilterModeBilinear = 0x00000001,
	kFilterModeAnisoPoint = 0x00000002,
	kFilterModeAnisoBilinear = 0x00000003,
};

enum ZFilterMode
{
	kZFilterModeNone = 0x00000000, 
	kZFilterModePoint = 0x00000001, 
	kZFilterModeLinear = 0x00000002,
};

enum MipFilterMode
{
	kMipFilterModeNone = 0x00000000,
	kMipFilterModePoint = 0x00000001,
	kMipFilterModeLinear = 0x00000002,
};

enum FilterReductionMode
{
	kFilterReductionModeWeightedAverage = 0x00000000,
	kFilterReductionModeMin = 0x00000001,
	kFilterReductionModeMax = 0x00000002,
};

enum AnisotropyRatio
{
	kAnisotropyRatio1 = 0x00000000,
	kAnisotropyRatio2 = 0x00000001,
	kAnisotropyRatio4 = 0x00000002,
	kAnisotropyRatio8 = 0x00000003,
	kAnisotropyRatio16 = 0x00000004,
};

enum DepthCompare
{
	kDepthCompareNever = 0x00000000,
	kDepthCompareLess = 0x00000001,
	kDepthCompareEqual = 0x00000002,
	kDepthCompareLessEqual = 0x00000003,
	kDepthCompareGreater = 0x00000004,
	kDepthCompareNotEqual = 0x00000005,
	kDepthCompareGreaterEqual = 0x00000006,
	kDepthCompareAlways = 0x00000007,
};

enum BorderColor
{
	kBorderColorTransBlack = 0x00000000,
	kBorderColorOpaqueBlack = 0x00000001,
	kBorderColorOpaqueWhite = 0x00000002,
	kBorderColorFromTable = 0x00000003,
};

enum ArrayMode
{
	kArrayModeLinearGeneral = 0x00000000,
	kArrayModeLinearAligned = 0x00000001,
	kArrayMode1dTiledThin = 0x00000002,
	kArrayMode1dTiledThick = 0x00000003,
	kArrayMode2dTiledThin = 0x00000004,
	kArrayModeTiledThinPrt = 0x00000005,
	kArrayMode2dTiledThinPrt = 0x00000006,
	kArrayMode2dTiledThick = 0x00000007,
	kArrayMode2dTiledXThick = 0x00000008,
	kArrayModeTiledThickPrt = 0x00000009,
	kArrayMode2dTiledThickPrt = 0x0000000a,
	kArrayMode3dTiledThinPrt = 0x0000000b,
	kArrayMode3dTiledThin = 0x0000000c,
	kArrayMode3dTiledThick = 0x0000000d,
	kArrayMode3dTiledXThick = 0x0000000e,
	kArrayMode3dTiledThickPrt = 0x0000000f,
};

enum MicroTileMode
{
	kMicroTileModeDisplay = 0x00000000,
	kMicroTileModeThin = 0x00000001,
	kMicroTileModeDepth = 0x00000002,
	kMicroTileModeRotated = 0x00000003,
	kMicroTileModeThick = 0x00000004,
};



// Internal usage

enum NumBanks
{
	kNumBanks2 = 0x00000000,
	kNumBanks4 = 0x00000001,
	kNumBanks8 = 0x00000002,
	kNumBanks16 = 0x00000003,
};

enum MacroTileMode
{
	kMacroTileMode_1x4_16 = 0x00000000,
	kMacroTileMode_1x2_16 = 0x00000001,
	kMacroTileMode_1x1_16 = 0x00000002,
	kMacroTileMode_1x1_16_dup = 0x00000003,
	kMacroTileMode_1x1_8 = 0x00000004,
	kMacroTileMode_1x1_4 = 0x00000005,
	kMacroTileMode_1x1_2 = 0x00000006,
	kMacroTileMode_1x1_2_dup = 0x00000007,
	kMacroTileMode_1x8_16 = 0x00000008,
	kMacroTileMode_1x4_16_dup = 0x00000009,
	kMacroTileMode_1x2_16_dup = 0x0000000A,
	kMacroTileMode_1x1_16_dup2 = 0x0000000B,
	kMacroTileMode_1x1_8_dup = 0x0000000C,
	kMacroTileMode_1x1_4_dup = 0x0000000D,
	kMacroTileMode_1x1_2_dup2 = 0x0000000E,
	kMacroTileMode_1x1_2_dup3 = 0x0000000F,
};

enum SampleSplit
{
	kSampleSplit1 = 0x00000000,
	kSampleSplit2 = 0x00000001,
	kSampleSplit4 = 0x00000002,
	kSampleSplit8 = 0x00000003,
};

enum TileSplit
{
	kTileSplit64B = 0x00000000,
	kTileSplit128B = 0x00000001,
	kTileSplit256B = 0x00000002,
	kTileSplit512B = 0x00000003,
	kTileSplit1KB = 0x00000004,
	kTileSplit2KB = 0x00000005,
	kTileSplit4KB = 0x00000006,
};

enum PipeConfig
{
	kPipeConfigP8_32x32_8x16 = 0x0000000a,
	kPipeConfigP8_32x32_16x16 = 0x0000000c,
	kPipeConfigP16 = 0x00000012,
};

// For GpuAddress

enum BankWidth
{
	kBankWidth1 = 0x00000000,
	kBankWidth2 = 0x00000001,
	kBankWidth4 = 0x00000002,
	kBankWidth8 = 0x00000003,
};

enum BankHeight
{
	kBankHeight1 = 0x00000000,
	kBankHeight2 = 0x00000001,
	kBankHeight4 = 0x00000002,
	kBankHeight8 = 0x00000003,
};

enum MacroTileAspect
{
	kMacroTileAspect1 = 0x00000000,
	kMacroTileAspect2 = 0x00000001,
	kMacroTileAspect4 = 0x00000002,
	kMacroTileAspect8 = 0x00000003,
};