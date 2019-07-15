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