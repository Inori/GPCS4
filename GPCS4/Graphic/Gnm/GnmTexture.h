#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmStructure.h"
#include "GnmRegInfo.h"
#include "GnmDataFormat.h"
#include "GnmSharpBuffer.h"
#include "GnmGpuAddress.h"

class GnmTexture
{
public:
	enum
	{
		kSqImgRsrcWord0 = 0,
		kSqImgRsrcWord1 = 1,
		kSqImgRsrcWord2 = 2,
		kSqImgRsrcWord3 = 3,
		kSqImgRsrcWord4 = 4,
		kSqImgRsrcWord5 = 5,
		kSqImgRsrcWord6 = 6,
		kSqImgRsrcWord7 = 7,
		kNumSqImgRsrcRegisters
	};

	const TSharpBuffer& getTsharp() const
	{
		return m_tsharp;
	}

	SizeAlign getSizeAlign(void) const
	{
		uint64_t size       = 0;
		AlignmentType align = 0;
		auto status         = GpuAddress::computeTotalTiledTextureSize(&size, &align, this);

		SizeAlign result = {0};
		if (status == GpuAddress::kStatusSuccess)
		{
			result.m_size  = size;
			result.m_align = align;
		}
		return result;
	}

	SizeAlign getMetadataSizeAlign(void) const
	{
		// TODO
	}

	uint8_t getMipStatsCounterIndex() const
	{
		return (uint8_t)(SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, COUNTER_BANK_ID));
	}

	bool isMipStatsEnabled() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, LOD_HDW_CNT_EN) ? true : false;
	}

	TextureType getTextureType() const
	{
		return (TextureType)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, TYPE);
	}

	// From IDA
	uint32_t getBaseAddress256ByteBlocks() const
	{
		uint32_t baseAddr256 = 0;
		TileMode tileMode;
		uint32_t numFragPerPixel = 0;
		NumBanks numBanks;
		uint32_t bitsPerElement = 0;
	
		DataFormat format = getDataFormat();

		bool isMacroTile = GpuAddress::isMacroTiled((TileMode)m_tsharp.tiling_idx);

		baseAddr256 = m_tsharp.baseaddr256;

		if (isMacroTile)
		{
			bitsPerElement = format.getTotalBitsPerElement();
			tileMode = (TileMode)m_tsharp.tiling_idx;
			numFragPerPixel = 1 << m_tsharp.last_level;
			
			if (m_regs[3] <= 0xDFFFFFFF)
			{
				numFragPerPixel = 1;
			}
			
			uint8_t shift = 0;
			if (m_regs[6] & 0x1000000)
			{
				GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
				shift = 4;
			}
			else
			{
				GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
				shift = 3;
			}
			baseAddr256 &= ~(((1 << (numBanks + 1)) - 1) << shift);
		}
	
		return baseAddr256;
	}

	void *getBaseAddress() const
	{
		return (void *)(uintptr_t(getBaseAddress256ByteBlocks()) << 8);
	}

	SamplerModulationFactor getSamplerModulationFactor(void) const
	{
		return (SamplerModulationFactor)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord2], SQ_IMG_RSRC_WORD2, PERF_MOD);
	}

	ResourceMemoryType getResourceMemoryType() const
	{

	}

	TileMode getTileMode(void) const
	{
		return (TileMode)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, TILING_INDEX);
	}

	bool getUseAltTileMode(void) const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, ALT_TILE_MODE) != 0; // [vi]
	}

	uint32_t getMinLodClamp() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord1], SQ_IMG_RSRC_WORD1, MIN_LOD);
	}

	uint32_t getPitchMinus1() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord4], SQ_IMG_RSRC_WORD4, PITCH);
	}

	uint32_t getWidthMinus1() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord2], SQ_IMG_RSRC_WORD2, WIDTH);
	}

	uint32_t getHeightMinus1() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord2], SQ_IMG_RSRC_WORD2, HEIGHT);
	}

	uint32_t getDepthMinus1() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord4], SQ_IMG_RSRC_WORD4, DEPTH);
	}

	uint32_t getTotalArraySliceCount() const
	{
		return (getTextureType() == kTextureType3d) ? 1 : getDepth();
	}

	SurfaceFormat getSurfaceFormat() const
	{
		return (SurfaceFormat)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord1], SQ_IMG_RSRC_WORD1, DATA_FORMAT);
	}

	TextureChannelType getTextureChannelType() const
	{
		return (TextureChannelType)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord1], SQ_IMG_RSRC_WORD1, NUM_FORMAT);
	}

	DataFormat getDataFormat() const
	{
		return DataFormat::build((SurfaceFormat)m_tsharp.dfmt, (TextureChannelType)m_tsharp.nfmt,
			(TextureChannel)m_tsharp.dst_sel_x,
			(TextureChannel)m_tsharp.dst_sel_y,
			(TextureChannel)m_tsharp.dst_sel_z,
			(TextureChannel)m_tsharp.dst_sel_w);
	}

	uint32_t getBaseArraySliceIndex() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord5], SQ_IMG_RSRC_WORD5, BASE_ARRAY);
	}

	uint32_t getLastArraySliceIndex() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord5], SQ_IMG_RSRC_WORD5, LAST_ARRAY); // For cubemaps, must be BASE_ARRAY + (numCubemaps*6)-1
	}

	uint32_t getBaseMipLevel() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, BASE_LEVEL);
	}

	uint32_t getLastMipLevel() const
	{
		return (getTextureType() == kTextureType2dMsaa || getTextureType() == kTextureType2dArrayMsaa) ? 0 : SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL);
	}

	bool isPaddedToPow2() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, POW2_PAD) != 0;
	}

	NumFragments getNumFragments() const
	{
		return  (getTextureType() == kTextureType2dMsaa || getTextureType() == kTextureType2dArrayMsaa) ? (NumFragments)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL) : kNumFragments1;
	}

	uint32_t getWidth() const
	{
		return getWidthMinus1() + 1;
	}

	uint32_t getHeight() const
	{
		return getHeightMinus1() + 1;
	}

	uint32_t getDepth() const
	{
		return getDepthMinus1() + 1;
	}

	uint32_t getPitch() const
	{
		return (getPitchMinus1() + 1);
	}

	bool isTexture(void) const
	{
		uint32_t textureType = SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, TYPE);
		return ((textureType & 0x8) != 0); // values of 8-15 represent valid texture types. Anything else is invalid!
	}

	uint8_t getTileSwizzleMask(void) const
	{
		// TODO:
		// Complete this function.

		// For Base mode or MacroTile, mask is always 0.
		return 0;
	}

	uint32_t getMetadataAddress256ByteBlocks() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord7], SQ_IMG_RSRC_WORD7, META_DATA_ADDRESS); // [vi]
	}

	void *getMetadataAddress(void) const
	{
		return (void *)(uintptr_t(getMetadataAddress256ByteBlocks()) << 8);
	}

	bool getMetadataCompressionEnable(void) const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, COMPRESSION_EN) != 0; // [vi]
	}

	TextureMetadataType getTextureMetadataType(void) const
	{

	}

	bool getDccAlphaOnMsb(void) const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, ALPHA_IS_ON_MSB) != 0; // [vi]
	}

	DccColorTransform getDccColorTransform(void) const
	{
		return (DccColorTransform)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, COLOR_TRANSFORM); // [vi]
	}

	GpuMode getMinimumGpuMode(void) const
	{
		// IDA
		return static_cast<GpuMode>(*((uint8_t*)this + 27) & 1);
	}

	union
	{
		uint32_t m_regs[8];
		TSharpBuffer m_tsharp;
	};
	
};
