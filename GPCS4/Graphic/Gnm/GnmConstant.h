#pragma once


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