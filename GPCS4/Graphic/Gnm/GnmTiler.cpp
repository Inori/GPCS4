#include "GnmTiler.h"

namespace GpuAddress
{;

int32_t TilingParameters::initFromTexture(const GnmTexture* texture, uint32_t mipLevel, uint32_t arraySlice)
{
}

int32_t detileSurface(void* outUntiledPixels, const void* tiledPixels, const TilingParameters* tp)
{
}

int32_t detileSurfaceRegion(void* outUntiledPixels, const void* tiledPixels, const TilingParameters* tp, const SurfaceRegion* srcRegion, uint32_t destPitch, uint32_t destSlicePitchElems)
{
}


Tiler1d::Tiler1d()
{
}

Tiler1d::Tiler1d(const TilingParameters* tp)
{
}

Tiler1d::~Tiler1d()
{
}

int32_t Tiler1d::init(const TilingParameters* tp)
{
}

int32_t Tiler1d::detileSurface(void* outUntiledPixels, const void* tiledPixels)
{
}

int32_t Tiler1d::detileSurfaceRegion(void* outUntiledPixels, const void* tiledPixels, const SurfaceRegion* srcRegion, uint32_t destPitch, uint32_t destSlicePitch)
{
}

}  // namespace GpuAddress