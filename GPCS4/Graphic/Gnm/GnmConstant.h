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