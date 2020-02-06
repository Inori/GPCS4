#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmTiler.h"
#include "GnmStructure.h"

namespace GpuAddress
{;

//////////////////////////////////////////////////////////////////////////

constexpr uint32_t kDramRowSize         = 0x400;
constexpr uint32_t kNumLogicalBanks     = 16;
constexpr uint32_t kPipeInterleaveBytes = 256;
constexpr uint32_t kBankInterleave      = 1;
constexpr uint32_t kMicroTileWidth      = 8;
constexpr uint32_t kMicroTileHeight     = 8;
constexpr uint32_t kNumMicroTilePixels  = kMicroTileWidth * kMicroTileHeight;
constexpr uint32_t kCmaskCacheBits      = 0x400;
constexpr uint32_t kHtileCacheBits      = 0x4000;

enum Status
{
	kStatusSuccess             = 0,
	kStatusInvalidArgument     = 0x80D30000,
	kStatusInternalTilingError = 0x80D30001,
};

struct SurfaceRegion
{
	uint32_t m_left;
	uint32_t m_top;
	uint32_t m_front;
	uint32_t m_right;
	uint32_t m_bottom;
	uint32_t m_back;

	bool operator==(const SurfaceRegion& other)
	{
		return !std::memcmp(this, &other, sizeof(SurfaceRegion));
	}
};

//////////////////////////////////////////////////////////////////////////

bool isMacroTiled(TileMode tileMode);

int32_t getAltNumBanks(
	NumBanks* outAltNumBanks, 
	TileMode tileMode, 
	uint32_t bitsPerElement, 
	uint32_t numFragmentsPerPixel);

int32_t getArrayMode(
	ArrayMode* outArrayMode, 
	TileMode tileMode);

int32_t getNumBanks(
	NumBanks* outNumBanks, 
	TileMode tileMode,
	uint32_t bitsPerElement, 
	uint32_t numFragmentsPerPixel);

int32_t getMicroTileMode(
	MicroTileMode* outMicroTileMode, 
	TileMode tileMode);

int32_t computeSurfaceMacroTileMode(
	MacroTileMode* outMacroTileMode, 
	TileMode tileMode,					
	uint32_t bitsPerElement, 
	uint32_t numFragmentsPerPixel);

int32_t computeSurfaceInfo(
	SurfaceInfo* infoOut, 
	const TilingParameters* tp);

int32_t computeTotalTiledTextureSize(
	uint64_t* outSize, 
	AlignmentType* outAlign, 
	const GnmTexture* texture);

int32_t adjustTileMode(
	GpuMode minGpuMode, 
	TileMode* outTileMode, 
	TileMode oldTileMode, 
	ArrayMode newArrayMode);

int32_t detileSurface(
	void* outUntiledPixels, 
	const void* tiledPixels, 
	const TilingParameters* tp);

int32_t detileSurfaceRegion(
	void* outUntiledPixels, 
	const void* tiledPixels, 
	const TilingParameters* tp,
	const SurfaceRegion* srcRegion, 
	uint32_t destPitch, 
	uint32_t destSlicePitchElems);

}  // namespace GpuAddress