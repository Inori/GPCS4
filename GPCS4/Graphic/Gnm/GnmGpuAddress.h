#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmRegInfo.h"
#include "GnmTexture.h"

#include <algorithm>

enum Status
{
	kStatusSuccess = 0,
	kStatusInvalidArgument = 0x80D30000,
	kStatusInternalTilingError = 0x80D30001,
};

class GpuAddress
{
public:

	static bool isMacroTiled(TileMode tileMode)
	{
		const uint64_t macroTiledMask = 0
			| (1 << kTileModeDepth_2dThin_64)
			| (1 << kTileModeDepth_2dThin_128)
			| (1 << kTileModeDepth_2dThin_256)
			| (1 << kTileModeDepth_2dThin_512)
			| (1 << kTileModeDepth_2dThin_1K)
			| (0 << kTileModeDepth_1dThin)
			| (1 << kTileModeDepth_2dThinPrt_256)
			| (1 << kTileModeDepth_2dThinPrt_1K)
			| (0 << kTileModeDisplay_LinearAligned)
			| (0 << kTileModeDisplay_1dThin)
			| (1 << kTileModeDisplay_2dThin)
			| (1 << kTileModeDisplay_ThinPrt)
			| (1 << kTileModeDisplay_2dThinPrt)
			| (0 << kTileModeThin_1dThin)
			| (1 << kTileModeThin_2dThin)
			| (1 << kTileModeThin_3dThin)
			| (1 << kTileModeThin_ThinPrt)
			| (1 << kTileModeThin_2dThinPrt)
			| (1 << kTileModeThin_3dThinPrt)
			| (0 << kTileModeThick_1dThick)
			| (1 << kTileModeThick_2dThick)
			| (1 << kTileModeThick_3dThick)
			| (1 << kTileModeThick_ThickPrt)
			| (1 << kTileModeThick_2dThickPrt)
			| (1 << kTileModeThick_3dThickPrt)
			| (1 << kTileModeThick_2dXThick)
			| (1 << kTileModeThick_3dXThick)
			| (0 << kTileModeDisplay_LinearGeneral)
			;
		return (macroTiledMask & (1ULL << tileMode)) != 0;
	}

	static bool isPartiallyResidentTexture(ArrayMode arrayMode)
	{
		const uint64_t prtTiledMask = 0
			| (0 << kArrayModeLinearGeneral)
			| (0 << kArrayModeLinearAligned)
			| (0 << kArrayMode1dTiledThin)
			| (0 << kArrayMode1dTiledThick)
			| (0 << kArrayMode2dTiledThin)
			| (1 << kArrayModeTiledThinPrt)
			| (1 << kArrayMode2dTiledThinPrt)
			| (0 << kArrayMode2dTiledThick)
			| (0 << kArrayMode2dTiledXThick)
			| (1 << kArrayModeTiledThickPrt)
			| (1 << kArrayMode2dTiledThickPrt)
			| (1 << kArrayMode3dTiledThinPrt)
			| (0 << kArrayMode3dTiledThin)
			| (0 << kArrayMode3dTiledThick)
			| (0 << kArrayMode3dTiledXThick)
			| (1 << kArrayMode3dTiledThickPrt)
			;
		return (prtTiledMask & (1ULL << arrayMode)) != 0;
	}

	static int32_t getMicroTileMode(MicroTileMode *outMicroTileMode, TileMode tileMode)
	{
		*outMicroTileMode = (MicroTileMode)SCE_GNM_GET_FIELD(s_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, MICRO_TILE_MODE_NEW);
		return kStatusSuccess;
	}

	static int32_t getArrayMode(ArrayMode *outArrayMode, TileMode tileMode)
	{
		*outArrayMode = (ArrayMode)SCE_GNM_GET_FIELD(s_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, ARRAY_MODE);
		return kStatusSuccess;
	}

	static int32_t getPipeConfig(PipeConfig *outPipeConfig, TileMode tileMode)
	{
		*outPipeConfig = (PipeConfig)SCE_GNM_GET_FIELD(s_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, PIPE_CONFIG);
		return kStatusSuccess;
	}

	static int32_t getTileSplit(TileSplit *outTileSplit, TileMode tileMode)
	{
		*outTileSplit = (TileSplit)SCE_GNM_GET_FIELD(s_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, TILE_SPLIT);
		return kStatusSuccess;
	}

	static int32_t getSampleSplit(SampleSplit *outSampleSplit, TileMode tileMode)
	{
		*outSampleSplit = (SampleSplit)SCE_GNM_GET_FIELD(s_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, SAMPLE_SPLIT);
		return kStatusSuccess;
	}

	static uint32_t getMicroTileThickness(ArrayMode arrayMode)
	{
		uint32_t thickness = 1;
		switch (arrayMode)
		{
		case kArrayMode1dTiledThick:
		case kArrayMode2dTiledThick:
		case kArrayMode3dTiledThick:
		case kArrayModeTiledThickPrt:
		case kArrayMode2dTiledThickPrt:
		case kArrayMode3dTiledThickPrt:
			thickness = 4;
			break;
		case kArrayMode2dTiledXThick:
		case kArrayMode3dTiledXThick:
			thickness = 8;
			break;
		case kArrayModeLinearGeneral:
		case kArrayModeLinearAligned:
		case kArrayMode1dTiledThin:
		case kArrayMode2dTiledThin:
		case kArrayModeTiledThinPrt:
		case kArrayMode2dTiledThinPrt:
		case kArrayMode3dTiledThinPrt:
		case kArrayMode3dTiledThin:
			break;
		default:
			break;
		}
		return thickness;
	}

	static int32_t computeSurfaceMacroTileMode(MacroTileMode *outMacroTileMode, TileMode tileMode,
		uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
	{
		int32_t status = -1;
		do 
		{
			ArrayMode arrayMode;
			int32_t status = getArrayMode(&arrayMode, tileMode);
			if (status != kStatusSuccess)
			{
				break;
			}

			MicroTileMode microTileMode;
			SampleSplit sampleSplitHw;
			TileSplit tileSplitHw;
			status = getMicroTileMode(&microTileMode, tileMode);
			status = getSampleSplit(&sampleSplitHw, tileMode);
			status = getTileSplit(&tileSplitHw, tileMode);

			uint32_t tileThickness = getMicroTileThickness(arrayMode);
			uint32_t tileBytes1x = bitsPerElement * kMicroTileWidth*kMicroTileHeight*tileThickness / 8;
			uint32_t sampleSplit = 1 << sampleSplitHw;
			uint32_t colorTileSplit = std::max(256U, sampleSplit*tileBytes1x);
			uint32_t tileSplit = (microTileMode == kMicroTileModeDepth) ? (64UL << tileSplitHw) : colorTileSplit;
			uint32_t tileSplitC = std::min(kDramRowSize, tileSplit);
			uint32_t tileBytes = std::min(tileSplitC, numFragmentsPerPixel*tileBytes1x);
			uint32_t mtmIndex = fastIntLog2(tileBytes / 64);
			*outMacroTileMode = (MacroTileMode)(isPartiallyResidentTexture(arrayMode) ? (mtmIndex + 8) : mtmIndex);
		} while (false);
		return status;
	}
	/*
	static int32_t computeTotalTiledTextureSize(uint64_t *outSize, AlignmentType *outAlign, const GnmTexture *texture)
	{
		
		const auto isCubemap = (texture->getTextureType() == kTextureTypeCubemap);
		const auto isVolume = (texture->getTextureType() == kTextureType3d);
		auto arraySliceCount = texture->getTotalArraySliceCount();
		if (isCubemap)
			arraySliceCount *= 6; // cube maps store six faces per array slice
		else if (isVolume)
			arraySliceCount = 1; // volume textures can't be arrays
		if (texture->isPaddedToPow2())
			arraySliceCount = nextPowerOfTwo((uint32_t)arraySliceCount); // array slice counts are padded to a power of two

		switch (texture->getTextureType())
		{
		case kTextureType2d:
		case kTextureType2dArray:
		case kTextureTypeCubemap:
			if ((texture->getTileMode() == kTileModeThin_1dThin)
				&& isPowerOfTwo(texture->getWidth())
				&& (texture->getWidth() == texture->getHeight())
				)
			{
				const auto xy = fastIntLog2(texture->getWidth());
				const auto f = fastIntLog2(texture->getDataFormat().getTotalBytesPerElement()) + (texture->getDataFormat().isBlockCompressedFormat() ? 2 : 0);
				if (g_pitches[xy][f] == texture->getPitch())
				{
					const auto mipLevel = texture->getLastMipLevel();
					const auto offset = uint64_t(g_offsetSizes[xy][f][mipLevel][0]) << 8;
					const auto size = uint64_t(g_offsetSizes[xy][f][mipLevel][1]) << 8;
					*outSize = (offset + size) * arraySliceCount;
					*outAlign = 256;
					return kStatusSuccess;
				}
			}
			break;
		default:
			break;
		}

		*outSize = 0;
		*outAlign = (AlignmentType)1;
		TilingParameters tpBase;
		tpBase.initFromTexture(texture, 0, 0);
		TilingParameters tpCopy; // so we can modify the mipLevel field inside the loop
		memcpy(&tpCopy, &tpBase, sizeof(tpCopy));
		uint64_t mipLevelCount = texture->getLastMipLevel() + 1;
		for (uint32_t iMip = 0; iMip < mipLevelCount; ++iMip)
		{
			tpCopy.m_linearWidth = std::max(tpBase.m_linearWidth >> iMip, 1U);
			tpCopy.m_linearHeight = std::max(tpBase.m_linearHeight >> iMip, 1U);
			tpCopy.m_linearDepth = std::max(tpBase.m_linearDepth >> iMip, 1U);
			tpCopy.m_baseTiledPitch = texture->getPitch();
			tpCopy.m_mipLevel = iMip;
			uint64_t mipSize;
			AlignmentType mipAlign;
			int32_t mipStatus = computeTiledSurfaceSize(&mipSize, &mipAlign, &tpCopy);
			if (mipStatus != kStatusSuccess)
				return mipStatus;
			*outSize += arraySliceCount * mipSize; // TODO: special-case for cubemaps, where mips >0 have numSlices padded?
			*outAlign = std::max(*outAlign, mipAlign);
			if (tpCopy.m_linearWidth == 1 && tpCopy.m_linearHeight == 1 && tpCopy.m_linearDepth == 1)
				break; // early-out if we've hit the smallest mip
		}

		return kStatusSuccess;
		
	}
	*/

	static int32_t getNumBanks(NumBanks *outNumBanks, TileMode tileMode,
		uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
	{
		MacroTileMode macroTileMode;
		int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
		if (status == kStatusSuccess)
		{
			*outNumBanks = (NumBanks)SCE_GNM_GET_FIELD(s_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, NUM_BANKS);
		}
			
		return status;
	}

	static int32_t getAltNumBanks(NumBanks *outAltNumBanks, TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
	{
		MacroTileMode macroTileMode;
		int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
		if (status == kStatusSuccess) 
		{
			*outAltNumBanks = (NumBanks)SCE_GNM_GET_FIELD(s_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, ALT_NUM_BANKS);
		}
		return status;
	}

	// floor(log2(i))
	inline static uint32_t fastIntLog2(uint32_t i)
	{
		return 31 - __builtin_clz(i | 1);
	}

private:

	union SurfaceFlags
	{
		class
		{
		public:
			uint32_t m_colorTarget : 1;
			uint32_t m_depthTarget : 1;
			uint32_t m_stencilTarget : 1;
			uint32_t m_texture : 1;
			uint32_t m_cube : 1;
			uint32_t m_volume : 1;
			uint32_t m_fmask : 1;
			uint32_t m_cubeAsArray : 1;
			uint32_t m_overlay : 1;
			uint32_t m_noStencil : 1;
			uint32_t m_display : 1;
			uint32_t m_prt : 1;
			uint32_t m_pow2Pad : 1;
			uint32_t m_texCompatible : 1;
			uint32_t m_reserved : 18;
		};
		uint32_t m_value;
	};

	class TilingParameters
	{
	public:
		TileMode m_tileMode;
		GpuMode m_minGpuMode;

		uint32_t m_linearWidth;
		uint32_t m_linearHeight;
		uint32_t m_linearDepth;
		uint32_t m_numFragmentsPerPixel;
		uint32_t m_baseTiledPitch;

		uint32_t m_mipLevel;
		uint32_t m_arraySlice;
		SurfaceFlags m_surfaceFlags;
		uint32_t m_bitsPerFragment;
		bool m_isBlockCompressed;
		uint8_t m_tileSwizzleMask;
	};

private:
	static constexpr uint32_t kDramRowSize = 0x400;
	static constexpr uint32_t kNumLogicalBanks = 16;
	static constexpr uint32_t kPipeInterleaveBytes = 256;
	static constexpr uint32_t kBankInterleave = 1;
	static constexpr uint32_t kMicroTileWidth = 8;
	static constexpr uint32_t kMicroTileHeight = 8;
	static constexpr uint32_t kNumMicroTilePixels = kMicroTileWidth * kMicroTileHeight;
	static constexpr uint32_t kCmaskCacheBits = 0x400;
	static constexpr uint32_t kHtileCacheBits = 0x4000;

	static constexpr uint32_t s_tileModeDefaultsAsInts[32] =
	{
		0x90800310,
		0x90800B10,
		0x90801310,
		0x90801B10,
		0x90802310,
		0x90800308,
		0x90801318,
		0x90802318,
		0x90000304,
		0x90000308,
		0x92000310,
		0x92000294,
		0x92000318,
		0x90400308,
		0x92400310,
		0x924002B0,
		0x92400294,
		0x92400318,
		0x9240032C,
		0x9100030C,
		0x9100031C,
		0x910002B4,
		0x910002A4,
		0x91000328,
		0x910002BC,
		0x91000320,
		0x910002B8,
		0x90C00308,
		0x92C00310,
		0x92C00294,
		0x92C00318,
		0x00000000,
	};

	static constexpr uint32_t s_macroTileModeDefaultsAsInts[16] =
	{
		0x26E8,
		0x26D4,
		0x21D0,
		0x21D0,
		0x2080,
		0x2040,
		0x1000,
		0x0000,
		0x36EC,
		0x26E8,
		0x21D4,
		0x20D0,
		0x1080,
		0x1040,
		0x0000,
		0x0000,
	};
};