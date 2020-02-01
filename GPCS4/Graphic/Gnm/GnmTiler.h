#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"

class GnmTexture;

namespace GpuAddress
{;

struct SurfaceRegion;

union SurfaceFlags 
{
	class
	{
	public:
		uint32_t m_colorTarget		: 1;
		uint32_t m_depthTarget		: 1;
		uint32_t m_stencilTarget	: 1;
		uint32_t m_texture			: 1;
		uint32_t m_cube				: 1;
		uint32_t m_volume			: 1;
		uint32_t m_fmask			: 1;
		uint32_t m_cubeAsArray		: 1;
		uint32_t m_overlay			: 1;
		uint32_t m_noStencil		: 1;
		uint32_t m_display			: 1;
		uint32_t m_prt				: 1;
		uint32_t m_pow2Pad			: 1;
		uint32_t m_texCompatible	: 1;
		uint32_t m_reserved			: 18; 
	};

	uint32_t m_value;
};

class SurfaceInfo
{
public:
	uint32_t m_pitch;
	uint32_t m_height;
	uint32_t m_depth;
	uint8_t m_reserved[4];
	uint64_t m_surfaceSize;
	ArrayMode m_arrayMode;
	uint32_t m_baseAlign;
	uint32_t m_pitchAlign;
	uint32_t m_heightAlign;
	uint32_t m_depthAlign;
	uint32_t m_bitsPerElement;
	struct
	{
		uint16_t m_isTexCompatible : 1;
		uint16_t m_unused : 15;
	};
	TileMode m_tileMode;
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

	int32_t initFromTexture(const GnmTexture* texture, uint32_t mipLevel, uint32_t arraySlice);
};


class Tiler
{
protected:
	GpuMode m_minGpuMode;
	TileMode m_tileMode;
	ArrayMode m_arrayMode;
	uint32_t m_linearWidth;   // in elements
	uint32_t m_linearHeight;  // in elements
	uint32_t m_linearDepth;   // in pixels
	uint32_t m_paddedWidth;   // = pitch, in elements
	uint32_t m_paddedHeight;  // in elements
	uint32_t m_paddedDepth;   // in pixels
	uint32_t m_bitsPerElement;
	uint64_t m_linearSizeBytes;
	uint64_t m_tiledSizeBytes;
};


class Tiler1d : public Tiler
{
public:
	Tiler1d();
	explicit Tiler1d(const TilingParameters* tp);
	~Tiler1d();

	int32_t init(const TilingParameters* tp);

	int32_t detileSurface(void* outUntiledPixels, const void* tiledPixels);

	int32_t detileSurfaceRegion(void* outUntiledPixels, const void* tiledPixels, const SurfaceRegion* srcRegion, uint32_t destPitch, uint32_t destSlicePitch);

	int32_t getTiledElementByteOffset(uint64_t* outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const;

	int32_t getTiledElementBitOffset(uint64_t* outTiledBitOffset, uint32_t x, uint32_t y, uint32_t z) const;

private:
	MicroTileMode m_microTileMode;
	uint32_t m_tileThickness;
	uint32_t m_tileBytes;
	uint32_t m_tilesPerRow;
	uint32_t m_tilesPerSlice;
};

}  // namespace GpuAddress