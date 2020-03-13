#pragma once

#include "GnmCommon.h"
#include "GnmRegInfo.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"
#include "GpuAddress/GnmGpuAddress.h"

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

class GnmRenderTarget
{
public:
	enum
	{
		kCbColorBase       = mmCB_COLOR0_BASE - mmCB_COLOR0_BASE,
		kCbColorPitch      = mmCB_COLOR0_PITCH - mmCB_COLOR0_BASE,
		kCbColorSlice      = mmCB_COLOR0_SLICE - mmCB_COLOR0_BASE,
		kCbColorView       = mmCB_COLOR0_VIEW - mmCB_COLOR0_BASE,
		kCbColorInfo       = mmCB_COLOR0_INFO - mmCB_COLOR0_BASE,
		kCbColorAttrib     = mmCB_COLOR0_ATTRIB - mmCB_COLOR0_BASE,
		kCbColorDccControl = mmCB_COLOR0_DCC_CONTROL - mmCB_COLOR0_BASE,
		kCbColorCmask      = mmCB_COLOR0_CMASK - mmCB_COLOR0_BASE,
		kCbColorCmaskSlice = mmCB_COLOR0_CMASK_SLICE - mmCB_COLOR0_BASE,
		kCbColorFmask      = mmCB_COLOR0_FMASK - mmCB_COLOR0_BASE,
		kCbColorFmaskSlice = mmCB_COLOR0_FMASK_SLICE - mmCB_COLOR0_BASE,
		kCbColorClearWord0 = mmCB_COLOR0_CLEAR_WORD0 - mmCB_COLOR0_BASE,
		kCbColorClearWord1 = mmCB_COLOR0_CLEAR_WORD1 - mmCB_COLOR0_BASE,
		kCbColorDccBase    = mmCB_COLOR0_DCC_BASE - mmCB_COLOR0_BASE,
		// 14: unused
		kCbWidthHeight = 15,  // not a GPU register. width in [15:0], height in [31:16].
		kNumCbRegisters
	};

	GnmRenderTarget& operator = (const GnmRenderTarget& other)
	{
		std::memcpy(m_regs, other.m_regs, sizeof(m_regs));
		return *this;
	}

	SizeAlign getColorSizeAlign(void) const
	{
		// TODO:
		// For render target which represents display buffer, we don't need the buffer size,
		// since we use swapchain image instead.
		// but for other render targets, e.g. G-Buffers, the size maybe required,
		// better to implement this.

		SizeAlign sizeAlign = {};
		sizeAlign.m_size    = 0;  // Note: this should be equal to SceVideoOut::calculateBufferSize
		sizeAlign.m_align   = 64;
		return sizeAlign;
	}

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
		// From IDA
		return DataFormat::build(
			(RenderTargetFormat)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, FORMAT),
			(RenderTargetChannelType)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, NUMBER_TYPE),
			(RenderTargetChannelOrder)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, COMP_SWAP));
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
		// From IDA.
		uint32_t   base       = SCE_GNM_GET_FIELD(m_regs[kCbColorBase], CB_COLOR0_BASE, BASE_256B);
		DataFormat dataFormat = getDataFormat();
		TileMode   tileMode   = getTileMode();

		if (dataFormat.m_asInt && GpuAddress::isMacroTiled(tileMode))
		{
			uint8_t alt = SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, ALT_TILE_MODE);
			uint32_t bitsPerElement = dataFormat.getTotalBitsPerElement();
			uint32_t numFragments   = 1 << SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, NUM_FRAGMENTS);
			NumBanks numBanks       = {};
			uint32_t shift          = 0;
			if (alt)
			{
				GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
				shift = 4;
			}
			else
			{
				GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
				shift = 3;
			}
			base &= ~(((1 << (numBanks + 1)) - 1) << shift);
		}

		return base;
	}

	void *getBaseAddress() const
	{
		return (void *)(uintptr_t(getBaseAddress256ByteBlocks()) << 8);
	}


	uint32_t m_regs[16];
};
