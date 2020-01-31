#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"

namespace GpuAddress
{;

class TilingParameters;

//////////////////////////////////////////////////////////////////////////

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
};

//////////////////////////////////////////////////////////////////////////

bool isMacroTiled(TileMode tileMode);

int32_t getAltNumBanks(
	NumBanks* outAltNumBanks, 
	TileMode tileMode, 
	uint32_t bitsPerElement, 
	uint32_t numFragmentsPerPixel);

int32_t getNumBanks(
	NumBanks* outNumBanks, 
	TileMode tileMode,
	uint32_t bitsPerElement, 
	uint32_t numFragmentsPerPixel);

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