#include "GnmGpuAddress.h"

#include "GnmRegInfo.h"
#include "GnmTexture.h"

#include <algorithm>

namespace GpuAddress
{;

constexpr uint32_t kDramRowSize         = 0x400;
constexpr uint32_t kNumLogicalBanks     = 16;
constexpr uint32_t kPipeInterleaveBytes = 256;
constexpr uint32_t kBankInterleave      = 1;
constexpr uint32_t kMicroTileWidth      = 8;
constexpr uint32_t kMicroTileHeight     = 8;
constexpr uint32_t kNumMicroTilePixels  = kMicroTileWidth * kMicroTileHeight;
constexpr uint32_t kCmaskCacheBits      = 0x400;
constexpr uint32_t kHtileCacheBits      = 0x4000;

constexpr uint32_t g_tileModeDefaultsAsInts[32] = 
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

constexpr uint32_t g_macroTileModeDefaultsAsInts[16] = 
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
	
bool isMacroTiled(TileMode tileMode)
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
		| (0 << kTileModeDisplay_LinearGeneral);
	return (macroTiledMask & (1ULL << tileMode)) != 0;
}

bool isPartiallyResidentTexture(ArrayMode arrayMode)
{
	const uint64_t prtTiledMask = 0 | (0 << kArrayModeLinearGeneral) | (0 << kArrayModeLinearAligned) | (0 << kArrayMode1dTiledThin) | (0 << kArrayMode1dTiledThick) | (0 << kArrayMode2dTiledThin) | (1 << kArrayModeTiledThinPrt) | (1 << kArrayMode2dTiledThinPrt) | (0 << kArrayMode2dTiledThick) | (0 << kArrayMode2dTiledXThick) | (1 << kArrayModeTiledThickPrt) | (1 << kArrayMode2dTiledThickPrt) | (1 << kArrayMode3dTiledThinPrt) | (0 << kArrayMode3dTiledThin) | (0 << kArrayMode3dTiledThick) | (0 << kArrayMode3dTiledXThick) | (1 << kArrayMode3dTiledThickPrt);
	return (prtTiledMask & (1ULL << arrayMode)) != 0;
}

int32_t getMicroTileMode(MicroTileMode* outMicroTileMode, TileMode tileMode)
{
	*outMicroTileMode = (MicroTileMode)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, MICRO_TILE_MODE_NEW);
	return kStatusSuccess;
}

int32_t getArrayMode(ArrayMode* outArrayMode, TileMode tileMode)
{
	*outArrayMode = (ArrayMode)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, ARRAY_MODE);
	return kStatusSuccess;
}

int32_t getPipeConfig(PipeConfig* outPipeConfig, TileMode tileMode)
{
	*outPipeConfig = (PipeConfig)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, PIPE_CONFIG);
	return kStatusSuccess;
}

int32_t getTileSplit(TileSplit* outTileSplit, TileMode tileMode)
{
	*outTileSplit = (TileSplit)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, TILE_SPLIT);
	return kStatusSuccess;
}

int32_t getSampleSplit(SampleSplit* outSampleSplit, TileMode tileMode)
{
	*outSampleSplit = (SampleSplit)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, SAMPLE_SPLIT);
	return kStatusSuccess;
}

uint32_t getMicroTileThickness(ArrayMode arrayMode)
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

inline static uint32_t fastIntLog2(uint32_t i)
{
	return 31 - __builtin_clz(i | 1);
}

int32_t computeSurfaceMacroTileMode(MacroTileMode* outMacroTileMode, TileMode tileMode,
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

		uint32_t tileThickness  = getMicroTileThickness(arrayMode);
		uint32_t tileBytes1x    = bitsPerElement * kMicroTileWidth * kMicroTileHeight * tileThickness / 8;
		uint32_t sampleSplit    = 1 << sampleSplitHw;
		uint32_t colorTileSplit = std::max(256U, sampleSplit * tileBytes1x);
		uint32_t tileSplit      = (microTileMode == kMicroTileModeDepth) ? (64UL << tileSplitHw) : colorTileSplit;
		uint32_t tileSplitC     = std::min(kDramRowSize, tileSplit);
		uint32_t tileBytes      = std::min(tileSplitC, numFragmentsPerPixel * tileBytes1x);
		uint32_t mtmIndex       = fastIntLog2(tileBytes / 64);
		*outMacroTileMode       = (MacroTileMode)(isPartiallyResidentTexture(arrayMode) ? (mtmIndex + 8) : mtmIndex);
	} while (false);
	return status;
}

int32_t getAltNumBanks(NumBanks* outAltNumBanks, TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
	{
		*outAltNumBanks = (NumBanks)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, ALT_NUM_BANKS);
	}
	return status;
}

int32_t getNumBanks(NumBanks* outNumBanks, TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
	{
		*outNumBanks = (NumBanks)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, NUM_BANKS);
	}

	return status;
}

}  // namespace GpuAddress