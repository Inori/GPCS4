#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmRegInfo.h"
#include "GnmStructure.h"
#include "GpuAddress/GnmGpuAddress.h"

union DepthRenderTargetInitFlags {
	struct
	{
		uint32_t enableHtileAcceleration : 1;
		uint32_t enableTextureWithoutDecompress : 1;
		uint32_t reserved : 30;
	};
	int32_t asInt;
};

class GnmDepthRenderTarget
{
public:
	enum
	{
		kDbZInfo            = 0,
		kDbStencilInfo      = 1,
		kDbZReadBase        = 2,
		kDbStencilReadBase  = 3,
		kDbZWriteBase       = 4,
		kDbStencilWriteBase = 5,
		kDbDepthSize        = 6,
		kDbDepthSlice       = 7,
		kDbDepthView        = 8,
		kDbHtileDataBase    = 9,
		kDbHtileSurface     = 10,
		kDbDepthInfo        = 11,
		kNumDbRegisters
	};

	GnmDepthRenderTarget& operator=(const GnmDepthRenderTarget& other)
	{
		std::memcpy(m_regs, other.m_regs, sizeof(m_regs));
		return *this;
	}

	SizeAlign getZSizeAlign(void) const
	{
		// TODO:
		SizeAlign sizeAlign = {};
		return sizeAlign;
	}

	SizeAlign getStencilSizeAlign(void) const
	{
	}

	SizeAlign getHtileSizeAlign(void) const
	{
	}

	uint32_t getPitchDiv8Minus1() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kDbDepthSize], DB_DEPTH_SIZE, PITCH_TILE_MAX);
	}

	uint32_t getPitch() const
	{
		return (getPitchDiv8Minus1() + 1) * 8;
	}

	uint32_t getWidth() const
	{
		return m_regs[12] & 0xFFFF;
	}

	uint32_t getHeight() const
	{
		return (m_regs[12] >> 16) & 0xFFFF;
	}

	ZFormat getZFormat() const
	{
		return (ZFormat)SCE_GNM_GET_FIELD(m_regs[kDbZInfo], DB_Z_INFO, FORMAT);
	}

	StencilFormat getStencilFormat() const
	{
		return (StencilFormat)SCE_GNM_GET_FIELD(m_regs[kDbStencilInfo], DB_STENCIL_INFO, FORMAT);
	}

	TileMode getTileMode(void) const
	{
		return (TileMode)SCE_GNM_GET_FIELD(m_regs[kDbZInfo], DB_Z_INFO, TILE_MODE_INDEX);
	}

	NumFragments getNumFragments() const
	{
		return (NumFragments)SCE_GNM_GET_FIELD(m_regs[kDbZInfo], DB_Z_INFO, NUM_SAMPLES);  // See illuminating comment in setNumFragments()
	}

	GpuMode getMinimumGpuMode(void) const
	{
		// TODO:
	}

	uint32_t getHtileAddress256ByteBlocks() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kDbHtileDataBase], DB_HTILE_DATA_BASE, BASE_256B);
	}

	void* getHtileAddress() const
	{
		return (void*)(uintptr_t(getHtileAddress256ByteBlocks()) << 8);
	}

	uint32_t getZReadAddress256ByteBlocks() const
	{
		// From IDA.
		uint32_t base     = SCE_GNM_GET_FIELD(m_regs[kDbZReadBase], DB_Z_READ_BASE, BASE_256B);
		ZFormat  zfmt     = (ZFormat)SCE_GNM_GET_FIELD(m_regs[kDbZInfo], DB_Z_INFO, FORMAT);
		TileMode tileMode = (TileMode)SCE_GNM_GET_FIELD(m_regs[kDbZInfo], DB_Z_INFO, TILE_MODE_INDEX);

		if (zfmt && GpuAddress::isMacroTiled(tileMode))
		{
			DataFormat dataFormat     = DataFormat::build(zfmt);
			uint32_t   bitsPerElement = dataFormat.getTotalBitsPerElement();
			uint32_t   numSamples     = 1 << SCE_GNM_GET_FIELD(m_regs[kDbZInfo], DB_Z_INFO, NUM_SAMPLES);

			NumBanks numBanks = {};
			uint32_t shift    = 0;

			PipeConfig pipeConfig = (PipeConfig)SCE_GNM_GET_FIELD(m_regs[kDbDepthInfo], DB_DEPTH_INFO, PIPE_CONFIG);
			if (pipeConfig == kPipeConfigP16)
			{
				GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numSamples);
				shift = 4;
			}
			else
			{
				GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numSamples);
				shift = 3;
			}
			base &= ~(((1 << (numBanks + 1)) - 1) << shift);
		}
		return base;
	}

	void* getZReadAddress() const
	{
		return (void*)(uintptr_t(getZReadAddress256ByteBlocks()) << 8);
	}

	uint32_t getStencilReadAddress256ByteBlocks() const
	{
		// TODO:
	}

	void* getStencilReadAddress() const
	{
		return (void*)(uintptr_t(getStencilReadAddress256ByteBlocks()) << 8);
	}

	uint32_t m_regs[13];
};