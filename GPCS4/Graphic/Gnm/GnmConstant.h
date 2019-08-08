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
