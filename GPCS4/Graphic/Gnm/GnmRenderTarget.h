#pragma once

#include "GnmCommon.h"
#include "GnmRegInfo.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"

union RenderTargetInitFlags
{
	struct
	{
		uint32_t enableCmaskFastClear : 1;
		uint32_t enableFmaskCompression : 1;
		uint32_t enableColorTextureWithoutDecompress : 1;
		uint32_t enableFmaskTextureWithoutDecompress : 1;
		uint32_t enableDccCompression : 1;
		uint32_t reserved : 27;
	};
	int32_t asInt;
};

class RenderTarget
{
public:
	enum
	{
		kCbColorBase = mmCB_COLOR0_BASE - mmCB_COLOR0_BASE,
		kCbColorPitch = mmCB_COLOR0_PITCH - mmCB_COLOR0_BASE,
		kCbColorSlice = mmCB_COLOR0_SLICE - mmCB_COLOR0_BASE,
		kCbColorView = mmCB_COLOR0_VIEW - mmCB_COLOR0_BASE,
		kCbColorInfo = mmCB_COLOR0_INFO - mmCB_COLOR0_BASE,
		kCbColorAttrib = mmCB_COLOR0_ATTRIB - mmCB_COLOR0_BASE,
		kCbColorDccControl = mmCB_COLOR0_DCC_CONTROL - mmCB_COLOR0_BASE,
		kCbColorCmask = mmCB_COLOR0_CMASK - mmCB_COLOR0_BASE,
		kCbColorCmaskSlice = mmCB_COLOR0_CMASK_SLICE - mmCB_COLOR0_BASE,
		kCbColorFmask = mmCB_COLOR0_FMASK - mmCB_COLOR0_BASE,
		kCbColorFmaskSlice = mmCB_COLOR0_FMASK_SLICE - mmCB_COLOR0_BASE,
		kCbColorClearWord0 = mmCB_COLOR0_CLEAR_WORD0 - mmCB_COLOR0_BASE,
		kCbColorClearWord1 = mmCB_COLOR0_CLEAR_WORD1 - mmCB_COLOR0_BASE,
		kCbColorDccBase = mmCB_COLOR0_DCC_BASE - mmCB_COLOR0_BASE,
		// 14: unused
		kCbWidthHeight = 15, // not a GPU register. width in [15:0], height in [31:16].
		kNumCbRegisters
	};


	uint32_t getWidth() const
	{
		return m_regs[kCbWidthHeight] & 0x0000FFFF;
	}

	uint32_t getHeight() const
	{
		return (m_regs[kCbWidthHeight] >> 16) & 0x0000FFFF;
	}

	uint32_t getPitchDiv8Minus1() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kCbColorPitch], CB_COLOR0_PITCH, TILE_MAX);
	}

	uint32_t getPitch() const
	{
		return (getPitchDiv8Minus1() + 1) * 8;
	}

	DataFormat getDataFormat() const
	{
		// TODO:
	}

	TileMode getTileMode(void) const
	{
		return (TileMode)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, TILE_MODE_INDEX);
	}

	NumSamples getNumSamples() const
	{
		return (NumSamples)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, NUM_SAMPLES); // cannot be greater than 4
	}

	NumFragments getNumFragments() const
	{
		return (NumFragments)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, NUM_FRAGMENTS); // cannot be greater than NUM_SAMPLES
	}

	GpuMode getMinimumGpuMode(void) const
	{
		// TODO:
	}

	uint32_t getBaseAddress256ByteBlocks() const
	{
		// TODO:
	}

	void *getBaseAddress() const
	{
		return (void *)(uintptr_t(getBaseAddress256ByteBlocks()) << 8);
	}


	uint32_t m_regs[16];
};
