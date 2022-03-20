#include "GnmGpuAddress.h"
#include "GnmGpuAddressInternal.h"
#include "GnmTilerSSE2.h"
#include "GnmRegsinfo.h"
#include "GnmRegsinfoPrivate.h"

#include "Gnm/GnmTexture.h"
#include "Gnm/GnmRenderTarget.h"
#include "Gnm/GnmDepthRenderTarget.h"

using namespace sce::GpuAddress;
using namespace sce;

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

LOG_CHANNEL("GpuAddress");

static inline int32_t getTiledElementByteOffset(const TilerLinear* tiler, uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t /*fragmentIndex*/) 
{
    return tiler->getTiledElementByteOffset(outTiledByteOffset, x, y, z);
}

static inline int32_t getTiledElementByteOffset(const Tiler1d* tiler, uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t /*fragmentIndex*/) 
{
    return tiler->getTiledElementByteOffset(outTiledByteOffset, x, y, z);
}

static inline int32_t getTiledElementByteOffset(const Tiler2d* tiler, uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex) 
{
    return tiler->getTiledElementByteOffset(outTiledByteOffset, x, y, z, fragmentIndex);
}

static MicroTileFunc getTileFuncSse2(const Gnm::MicroTileMode microTileMode, const uint32_t bitsPerElement)
{
	switch(microTileMode)
	{
	case Gnm::kMicroTileModeDisplay:
		if (bitsPerElement ==   8) return   tile8bppDisplaySse2;
		if (bitsPerElement ==  16) return  tile16bppDisplaySse2;
		if (bitsPerElement ==  32) return  tile32bppDisplaySse2;
		if (bitsPerElement ==  64) return  tile64bppDisplaySse2;
		return NULL;
	case Gnm::kMicroTileModeDepth:
	case Gnm::kMicroTileModeThin:
		if (bitsPerElement ==   8) return   tile8bppThinSse2;
		if (bitsPerElement ==  16) return  tile16bppThinSse2;
		if (bitsPerElement ==  32) return  tile32bppThinSse2;
		if (bitsPerElement ==  64) return  tile64bppThinSse2;
		if (bitsPerElement == 128) return tile128bppThinSse2;
		return NULL;
	case Gnm::kMicroTileModeRotated:
		return NULL;
	case Gnm::kMicroTileModeThick:
		if (bitsPerElement ==   8) return   tile8bppThickSse2;
		if (bitsPerElement ==  16) return  tile16bppThickSse2;
		if (bitsPerElement ==  32) return  tile32bppThickSse2;
		if (bitsPerElement ==  64) return  tile64bppThickSse2;
		if (bitsPerElement == 128) return tile128bppThickSse2;
		return NULL;
	default:
		SCE_GNM_ERROR("Unknown microTileMode %d", microTileMode);
		return NULL;
	}
}
static MicroTileFunc getDetileFuncSse2(const Gnm::MicroTileMode microTileMode, const uint32_t bitsPerElement)
{
	switch(microTileMode)
	{
	case Gnm::kMicroTileModeDisplay:
		if (bitsPerElement ==   8) return   detile8bppDisplaySse2;
		if (bitsPerElement ==  16) return  detile16bppDisplaySse2;
		if (bitsPerElement ==  32) return  detile32bppDisplaySse2;
		if (bitsPerElement ==  64) return  detile64bppDisplaySse2;
		return NULL;
	case Gnm::kMicroTileModeDepth:
	case Gnm::kMicroTileModeThin:
		if (bitsPerElement ==   8) return   detile8bppThinSse2;
		if (bitsPerElement ==  16) return  detile16bppThinSse2;
		if (bitsPerElement ==  32) return  detile32bppThinSse2;
		if (bitsPerElement ==  64) return  detile64bppThinSse2;
		if (bitsPerElement == 128) return detile128bppThinSse2;
		return NULL;
	case Gnm::kMicroTileModeRotated:
		return NULL;
	case Gnm::kMicroTileModeThick:
		if (bitsPerElement ==   8) return   detile8bppThickSse2;
		if (bitsPerElement ==  16) return  detile16bppThickSse2;
		if (bitsPerElement ==  32) return  detile32bppThickSse2;
		if (bitsPerElement ==  64) return  detile64bppThickSse2;
		if (bitsPerElement == 128) return detile128bppThickSse2;
		return NULL;
	default:
		SCE_GNM_ERROR("Unknown microTileMode %d", microTileMode);
		return NULL;
	}
}

// Only works for count=1,2,4,8,16
static inline void* small_memcpy(void *dest, const void *src, size_t count)
{
	switch(count)
	{
	case 1:
		*((uint8_t *)dest + 0) = *((uint8_t *)src + 0);
		break;
	case 2:
		*((uint16_t*)dest + 0) = *((uint16_t*)src + 0);
		break;
	case 4:
		*((uint32_t*)dest + 0) = *((uint32_t*)src + 0);
		break;
	case 8:
		*((uint64_t*)dest + 0) = *((uint64_t*)src + 0);
		break;
	case 16:
		*((uint64_t*)dest + 0) = *((uint64_t*)src + 0);
		*((uint64_t*)dest + 1) = *((uint64_t*)src + 1);
		break;
	default:
		SCE_GNM_ERROR("Unsupported byte count %lld", static_cast<long long>(count));
	}
	return dest;
}

static uint32_t getElementIndex(uint32_t x, uint32_t y, uint32_t z, uint32_t bitsPerElement, Gnm::MicroTileMode microTileMode, Gnm::ArrayMode arrayMode)
{
	uint32_t elem = 0;

	if (microTileMode == Gnm::kMicroTileModeDisplay)
	{
		switch(bitsPerElement)
		{
		case 8:
			elem |= ( (x>>0) & 0x1 ) << 0;
			elem |= ( (x>>1) & 0x1 ) << 1;
			elem |= ( (x>>2) & 0x1 ) << 2;
			elem |= ( (y>>1) & 0x1 ) << 3;
			elem |= ( (y>>0) & 0x1 ) << 4;
			elem |= ( (y>>2) & 0x1 ) << 5;
			break;
		case 16:
			elem |= ( (x>>0) & 0x1 ) << 0;
			elem |= ( (x>>1) & 0x1 ) << 1;
			elem |= ( (x>>2) & 0x1 ) << 2;
			elem |= ( (y>>0) & 0x1 ) << 3;
			elem |= ( (y>>1) & 0x1 ) << 4;
			elem |= ( (y>>2) & 0x1 ) << 5;
			break;
		case 32:
			elem |= ( (x>>0) & 0x1 ) << 0;
			elem |= ( (x>>1) & 0x1 ) << 1;
			elem |= ( (y>>0) & 0x1 ) << 2;
			elem |= ( (x>>2) & 0x1 ) << 3;
			elem |= ( (y>>1) & 0x1 ) << 4;
			elem |= ( (y>>2) & 0x1 ) << 5;
			break;
		case 64:
			elem |= ( (x>>0) & 0x1 ) << 0;
			elem |= ( (y>>0) & 0x1 ) << 1;
			elem |= ( (x>>1) & 0x1 ) << 2;
			elem |= ( (x>>2) & 0x1 ) << 3;
			elem |= ( (y>>1) & 0x1 ) << 4;
			elem |= ( (y>>2) & 0x1 ) << 5;
			break;
		default:
			SCE_GNM_ERROR("Unsupported bitsPerElement (%u) for displayable surface.", bitsPerElement);
		}
	}
	else if (microTileMode == Gnm::kMicroTileModeThin || microTileMode == Gnm::kMicroTileModeDepth)
	{
		elem |= ( (x>>0) & 0x1 ) << 0;
		elem |= ( (y>>0) & 0x1 ) << 1;
		elem |= ( (x>>1) & 0x1 ) << 2;
		elem |= ( (y>>1) & 0x1 ) << 3;
		elem |= ( (x>>2) & 0x1 ) << 4;
		elem |= ( (y>>2) & 0x1 ) << 5;
		// Use Z too, if the array mode is Thick/XThick
		switch(arrayMode)
		{
		case Gnm::kArrayMode2dTiledXThick:
		case Gnm::kArrayMode3dTiledXThick:
			elem |= ( (z>>2) & 0x1 ) << 8;
			// Intentional fall-through
		case Gnm::kArrayMode1dTiledThick:
		case Gnm::kArrayMode2dTiledThick:
		case Gnm::kArrayMode3dTiledThick:
		case Gnm::kArrayModeTiledThickPrt:
		case Gnm::kArrayMode2dTiledThickPrt:
		case Gnm::kArrayMode3dTiledThickPrt:
			elem |= ( (z>>0) & 0x1 ) << 6;
			elem |= ( (z>>1) & 0x1 ) << 7;
		default:
			break; // no other thick modes
		}
	}
	else if (microTileMode == Gnm::kMicroTileModeThick) // thick/xthick
	{
		switch(arrayMode)
		{
		case Gnm::kArrayMode2dTiledXThick:
		case Gnm::kArrayMode3dTiledXThick:
			elem |= ( (z>>2) & 0x1 ) << 8;
			// intentional fall-through
		case Gnm::kArrayMode1dTiledThick:
		case Gnm::kArrayMode2dTiledThick:
		case Gnm::kArrayMode3dTiledThick:
		case Gnm::kArrayModeTiledThickPrt:
		case Gnm::kArrayMode2dTiledThickPrt:
		case Gnm::kArrayMode3dTiledThickPrt:
			if (bitsPerElement == 8 || bitsPerElement == 16)
			{
				elem |= ( (x>>0) & 0x1 ) << 0;
				elem |= ( (y>>0) & 0x1 ) << 1;
				elem |= ( (x>>1) & 0x1 ) << 2;
				elem |= ( (y>>1) & 0x1 ) << 3;
				elem |= ( (z>>0) & 0x1 ) << 4;
				elem |= ( (z>>1) & 0x1 ) << 5;
				elem |= ( (x>>2) & 0x1 ) << 6;
				elem |= ( (y>>2) & 0x1 ) << 7;
			}
			else if (bitsPerElement == 32)
			{
				elem |= ( (x>>0) & 0x1 ) << 0;
				elem |= ( (y>>0) & 0x1 ) << 1;
				elem |= ( (x>>1) & 0x1 ) << 2;
				elem |= ( (z>>0) & 0x1 ) << 3;
				elem |= ( (y>>1) & 0x1 ) << 4;
				elem |= ( (z>>1) & 0x1 ) << 5;
				elem |= ( (x>>2) & 0x1 ) << 6;
				elem |= ( (y>>2) & 0x1 ) << 7;
			}
			else if (bitsPerElement == 64 || bitsPerElement == 128)
			{
				elem |= ( (x>>0) & 0x1 ) << 0;
				elem |= ( (y>>0) & 0x1 ) << 1;
				elem |= ( (z>>0) & 0x1 ) << 2;
				elem |= ( (x>>1) & 0x1 ) << 3;
				elem |= ( (y>>1) & 0x1 ) << 4;
				elem |= ( (z>>1) & 0x1 ) << 5;
				elem |= ( (x>>2) & 0x1 ) << 6;
				elem |= ( (y>>2) & 0x1 ) << 7;
			}
			else
			{
				SCE_GNM_ERROR("Invalid bitsPerElement (%u) for microTileMode=kMicroTileModeThick.", bitsPerElement);
			}
			break;
		default:
			SCE_GNM_ERROR("Invalid arrayMode (0x%02X) for thick/xthick microTileMode=kMicroTileModeThick.", arrayMode);
		}
	}
	// TODO: rotated

	return elem;
}
static uint32_t getPipeIndex(uint32_t x, uint32_t y, Gnm::PipeConfig pipeCfg)
{
	uint32_t pipe = 0;
	switch(pipeCfg)
	{
	case Gnm::kPipeConfigP8_32x32_8x16:
		pipe |= ( ((x>>4) ^ (y>>3) ^ (x>>5))	& 0x1 ) << 0;
		pipe |=	( ((x>>3) ^ (y>>4))				& 0x1 ) << 1;
		pipe |=	( ((x>>5) ^ (y>>5))				& 0x1 ) << 2;
		break;
	case Gnm::kPipeConfigP8_32x32_16x16:
		pipe |= ( ((x>>3) ^ (y>>3) ^ (x>>4))	& 0x1 ) << 0;
		pipe |=	( ((x>>4) ^ (y>>4))				& 0x1 ) << 1;
		pipe |=	( ((x>>5) ^ (y>>5))				& 0x1 ) << 2;
		break;
	case Gnm::kPipeConfigP16:
		pipe |= ( ((x>>3) ^ (y>>3) ^ (x>>4))	& 0x1 ) << 0;
		pipe |=	( ((x>>4) ^ (y>>4))				& 0x1 ) << 1;
		pipe |=	( ((x>>5) ^ (y>>5))				& 0x1 ) << 2;
		pipe |=	( ((x>>6) ^ (y>>5))				& 0x1 ) << 3;
		break;
	default:
		SCE_GNM_ERROR("Unsupported pipeCfg (0x%02X).", pipeCfg);
	}
	return pipe;
}

static uint32_t getBankIndex(uint32_t x, uint32_t y, uint32_t bank_width, uint32_t bank_height, uint32_t num_banks, uint32_t num_pipes)
{
	const uint32_t x_shift_offset = fastIntLog2(bank_width * num_pipes);
	const uint32_t y_shift_offset = fastIntLog2(bank_height);
	const uint32_t xs = x >> x_shift_offset;
	const uint32_t ys = y >> y_shift_offset;
	uint32_t bank = 0;
	switch(num_banks)
	{
	case 2:
		bank |= ( ((xs>>3) ^ (ys>>3))			& 0x1 ) << 0;
		break;
	case 4:
		bank |= ( ((xs>>3) ^ (ys>>4))			& 0x1 ) << 0;
		bank |= ( ((xs>>4) ^ (ys>>3))			& 0x1 ) << 1;
		break;
	case 8:
		bank |= ( ((xs>>3) ^ (ys>>5))			& 0x1 ) << 0;
		bank |=	( ((xs>>4) ^ (ys>>4) ^ (ys>>5))	& 0x1 ) << 1;
		bank |=	( ((xs>>5) ^ (ys>>3))			& 0x1 ) << 2;
		break;
	case 16:
		bank |= ( ((xs>>3) ^ (ys>>6))			& 0x1 ) << 0;
		bank |=	( ((xs>>4) ^ (ys>>5) ^ (ys>>6))	& 0x1 ) << 1;
		bank |=	( ((xs>>5) ^ (ys>>4))			& 0x1 ) << 2;
		bank |=	( ((xs>>6) ^ (ys>>3))			& 0x1 ) << 3;
		break;
	default:
		SCE_GNM_ERROR("invalid num_banks (%u) -- must be 2, 4, 8, or 16.", num_banks);
	}

	return bank;
}

sce::GpuAddress::TilerLinear::TilerLinear()
{
}
sce::GpuAddress::TilerLinear::TilerLinear(const TilingParameters *tp)
{
	int32_t status = init(tp);
	SCE_GNM_UNUSED(status);
	SCE_GNM_ASSERT_MSG(status == GpuAddress::kStatusSuccess, "TilerLinear initialization failed with error code %d", status);
}
sce::GpuAddress::TilerLinear::~TilerLinear()
{
}

int32_t sce::GpuAddress::TilerLinear::init(const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "computeSurfaceInfo() failed: %d\n", status);

	// derived inputs
	m_minGpuMode = tp->m_minGpuMode;
	m_tileMode = tp->m_tileMode;
	m_arrayMode = surfInfoOut.m_arrayMode;
	m_linearWidth = tp->m_linearWidth;
	m_linearHeight = tp->m_linearHeight;
	m_linearDepth = tp->m_linearDepth;
	m_bitsPerElement = tp->m_bitsPerFragment;
	m_paddedHeight = surfInfoOut.m_height;
	m_paddedDepth = surfInfoOut.m_depth;
	m_paddedWidth = surfInfoOut.m_pitch;
	// For compressed formats, each block is treated as an element for tiling purposes.
	// This affects a few of the above variables.
	if (tp->m_bitsPerFragment == 1)
	{
		m_bitsPerElement *= 8;
		m_linearWidth  = std::max((m_linearWidth+7)/8, 1U);
		m_paddedWidth  = std::max((m_paddedWidth+7)/8, 1U);
	}
	m_linearSizeBytes = (m_linearWidth * m_linearHeight * m_linearDepth * m_bitsPerElement + 7) / 8;
	m_tiledSizeBytes = static_cast<uint32_t>(surfInfoOut.m_surfaceSize);

	// These restrictions are enforced in computeSurfaceInfo(). If the asserts fire, it probably means
	// computeSurfaceInfo() isn't doing its job correctly.
	SCE_GNM_ASSERT_MSG( (m_paddedWidth % std::max(8UL, 64UL/(m_bitsPerElement/8UL))) == 0, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG( (m_paddedWidth * m_paddedHeight % std::max(64UL, kPipeInterleaveBytes / (m_bitsPerElement/8UL))) == 0, "internal consistency check failed.");
	return kStatusSuccess;
}

int32_t sce::GpuAddress::TilerLinear::getTiledElementByteOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledByteOffset != 0, kStatusInvalidArgument, "outTiledByteOffset must not be NULL.");
	uint64_t bitOffset = 0;
	int32_t status = getTiledElementBitOffset(&bitOffset, x, y, z);
	*outTiledByteOffset = bitOffset/8;
	return status;
}
int32_t sce::GpuAddress::TilerLinear::getTiledElementBitOffset(uint64_t *outTiledBitsOffset, uint32_t x, uint32_t y, uint32_t z) const
{
	uint64_t tiledRowSizeBits = m_bitsPerElement * m_paddedWidth;
	uint64_t tiledSliceBits = m_paddedWidth*m_paddedHeight*m_bitsPerElement;
	*outTiledBitsOffset  =  tiledSliceBits*z + tiledRowSizeBits*y  + m_bitsPerElement*x;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::TilerLinear::tileSurface(void *outTiledPixels, const void *untiledPixels)
{
	SurfaceRegion destRegion;
	destRegion.m_left   = destRegion.m_top = destRegion.m_front = 0;
	destRegion.m_right  = m_linearWidth;
	destRegion.m_bottom = m_linearHeight;
	destRegion.m_back   = m_linearDepth;
	return tileSurfaceRegion(outTiledPixels, untiledPixels, &destRegion, m_linearWidth, m_linearWidth*m_linearHeight);
}

static inline int width(const SurfaceRegion surfaceRegion)
{
	return surfaceRegion.m_right - surfaceRegion.m_left;
}
static inline int height(const SurfaceRegion surfaceRegion)
{
	return surfaceRegion.m_bottom - surfaceRegion.m_top;
}
static inline int depth(const SurfaceRegion surfaceRegion)
{
	return surfaceRegion.m_back - surfaceRegion.m_front;
}

#ifdef SCE_GNM_DEBUG
static inline int texels(const SurfaceRegion surfaceRegion)
{
    return width(surfaceRegion) * height(surfaceRegion) * depth(surfaceRegion);
}
#endif

static inline bool hasTexels(const SurfaceRegion surfaceRegion)
{
	return (width (surfaceRegion) > 0) 
		&& (height(surfaceRegion) > 0) 
		&& (depth (surfaceRegion) > 0);
}

static bool operator==(const SurfaceRegion a, const SurfaceRegion b)
{
    return memcmp(&a, &b, sizeof(a)) == 0;
}

#ifdef SCE_GNM_DEBUG
static inline bool intersect(const SurfaceRegion a, const SurfaceRegion b)
{
    return a.m_left < b.m_right &&
           a.m_right > b.m_left &&
           a.m_top < b.m_bottom &&
           a.m_bottom > b.m_top &&
           a.m_front < b.m_back &&
           a.m_back > b.m_front;
}
#endif

struct Regions
{
    SurfaceRegion m_aligned;
    SurfaceRegion m_unaligned[6];
    int m_unaligneds;
    void Init(const SurfaceRegion region, int tileThickness)
    {      
        const auto t = tileThickness - 1;
        m_aligned = region;
        m_aligned.m_left   = (m_aligned.m_left   + 7) & ~7;
        m_aligned.m_right  = (m_aligned.m_right     ) & ~7;
        m_aligned.m_top    = (m_aligned.m_top    + 7) & ~7;
        m_aligned.m_bottom = (m_aligned.m_bottom    ) & ~7;
        m_aligned.m_front  = (m_aligned.m_front  + t) & ~t;
        m_aligned.m_back   = (m_aligned.m_back      ) & ~t;

        m_unaligneds = 0;
        if(m_aligned == region)
            return;
        if(!hasTexels(m_aligned))
        {
            memset(&m_aligned, 0, sizeof(m_aligned));
            m_unaligned[0] = region;
            m_unaligneds = 1;
            return;
        }

        SurfaceRegion temp = region;

        if(temp.m_left < m_aligned.m_left)
        {
            m_unaligned[m_unaligneds] = temp;
            temp.m_left = m_unaligned[m_unaligneds].m_right = m_aligned.m_left;
            ++m_unaligneds;
        }
        if(temp.m_right > m_aligned.m_right)
        {
            m_unaligned[m_unaligneds] = temp;
            temp.m_right = m_unaligned[m_unaligneds].m_left = m_aligned.m_right;
            ++m_unaligneds;
        }

        if(temp.m_top < m_aligned.m_top)
        {
            m_unaligned[m_unaligneds] = temp;
            temp.m_top = m_unaligned[m_unaligneds].m_bottom = m_aligned.m_top;
            ++m_unaligneds;
        }
        if(temp.m_bottom > m_aligned.m_bottom)
        {
            m_unaligned[m_unaligneds] = temp;
            temp.m_bottom = m_unaligned[m_unaligneds].m_top = m_aligned.m_bottom;
            ++m_unaligneds;
        }

        if(temp.m_front < m_aligned.m_front)
        {
            m_unaligned[m_unaligneds] = temp;
            temp.m_front = m_unaligned[m_unaligneds].m_back = m_aligned.m_front;
            ++m_unaligneds;
        }
        if(temp.m_back > m_aligned.m_back)
        {
            m_unaligned[m_unaligneds] = temp;
            temp.m_back = m_unaligned[m_unaligneds].m_front = m_aligned.m_back;
            ++m_unaligneds;
        }

#ifdef SCE_GNM_DEBUG
        int total = texels(m_aligned);
        for(auto i = 0; i < m_unaligneds; ++i)
            total += texels(m_unaligned[i]);
        SCE_GNM_ASSERT(total == texels(region));
        for(auto i = 0; i < m_unaligneds; ++i)
            SCE_GNM_ASSERT(!intersect(m_aligned, m_unaligned[i]));
        for(auto i = 0; i < m_unaligneds; ++i)
            for(auto j = i+1; j < m_unaligneds; ++j)
                SCE_GNM_ASSERT(!intersect(m_unaligned[i], m_unaligned[j]));
#endif

    }
};

struct uint128_t { uint64_t m_data[2]; };
struct uint256_t { uint64_t m_data[4]; };

template <typename T, typename ELEMENT, typename DOUBLE_ELEMENT>
void slowTileOneFragment(const T* t, const SurfaceRegion region, const SurfaceRegion subRegion, int fragment, int srcPitch, int srcSlicePitch, uint8_t * __restrict out_bytes, const uint8_t *__restrict in_bytes)
{
    const int dx = subRegion.m_left    - region.m_left;
    const int dy = subRegion.m_top     - region.m_top;
    const int dz = subRegion.m_front   - region.m_front;
    for(auto z = 0; z < depth(subRegion); ++z)
	    for(auto y = 0; y < height(subRegion); ++y)
        {
    	    uint64_t linear_offset, tiled_offset;
            auto x = 0;
		    computeLinearElementByteOffset(&linear_offset, dx + x, dy + y, dz + z, 0, srcPitch, srcSlicePitch, sizeof(ELEMENT)*8, 1);
            if(subRegion.m_left & 1)
            {
			    getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
			    *static_cast<ELEMENT *>(static_cast<void *>(out_bytes + tiled_offset)) = *static_cast<const ELEMENT*>(static_cast<const void *>(in_bytes + linear_offset));
                linear_offset += sizeof(ELEMENT);
                ++x;
            }
		    for(;x + 1 < width(subRegion); x += 2)
		    {
			    getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
			    *static_cast<DOUBLE_ELEMENT *>(static_cast<void *>(out_bytes + tiled_offset)) = *static_cast<const DOUBLE_ELEMENT*>(static_cast<const void *>(in_bytes + linear_offset));
                linear_offset += sizeof(DOUBLE_ELEMENT);
		    }
            if(x < width(subRegion))
            {
			    getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
			    *static_cast<ELEMENT *>(static_cast<void *>(out_bytes + tiled_offset)) = *static_cast<const ELEMENT*>(static_cast<const void *>(in_bytes + linear_offset));
                linear_offset += sizeof(ELEMENT);
            }
        }
}


template <typename T>
void slowTileOneFragment(const T* t, const SurfaceRegion region, const SurfaceRegion subRegion, int fragment, int sourcePitch, int sourceSlicePitch, uint8_t * __restrict out_bytes, const uint8_t * __restrict in_bytes, int bytesPerElement)
{
    if(!hasTexels(subRegion))
        return;
    switch(bytesPerElement)
    {
        case 1:
            slowTileOneFragment<T,uint8_t,uint16_t>(t, region, subRegion, fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes);
            break;
        case 2:
            slowTileOneFragment<T,uint16_t,uint32_t>(t, region, subRegion, fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes);
            break;
        case 4:
            slowTileOneFragment<T,uint32_t,uint64_t>(t, region, subRegion, fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes);
            break;
        case 8:
            slowTileOneFragment<T,uint64_t,uint128_t>(t, region, subRegion, fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes);
            break;
        case 16:
            slowTileOneFragment<T,uint128_t,uint256_t>(t, region, subRegion, fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes);
            break;
        default:
            break;
    }
}

template <typename T, typename ELEMENT, typename DOUBLE_ELEMENT>
void slowDetileOneFragment(const T* t, const SurfaceRegion region, const SurfaceRegion subRegion, int fragment, int destPitch, int destSlicePitch, uint8_t * __restrict out_bytes, const uint8_t * __restrict in_bytes)
{
    const int dx = subRegion.m_left    - region.m_left;
    const int dy = subRegion.m_top     - region.m_top;
    const int dz = subRegion.m_front   - region.m_front;
    for(auto z = 0; z < depth(subRegion); ++z)
	    for(auto y = 0; y < height(subRegion); ++y)
        {
    	    uint64_t linear_offset, tiled_offset;
            auto x = 0;
		    computeLinearElementByteOffset(&linear_offset, dx + x, dy + y, dz + z, 0, destPitch, destSlicePitch, sizeof(ELEMENT)*8, 1);
            if(subRegion.m_left & 1)
            {
			    getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
			    *static_cast<ELEMENT *>(static_cast<void *>(out_bytes + linear_offset)) = *static_cast<const ELEMENT*>(static_cast<const void *>(in_bytes + tiled_offset));
                linear_offset += sizeof(ELEMENT);
                ++x;
            }
		    for(;x + 1 < width(subRegion); x += 2)
		    {
			    getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
			    *static_cast<DOUBLE_ELEMENT *>(static_cast<void *>(out_bytes + linear_offset)) = *static_cast<const DOUBLE_ELEMENT*>(static_cast<const void *>(in_bytes + tiled_offset));
                linear_offset += sizeof(DOUBLE_ELEMENT);
		    }
            if(x < width(subRegion))
            {
			    getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
			    *static_cast<ELEMENT *>(static_cast<void *>(out_bytes + linear_offset)) = *static_cast<const ELEMENT*>(static_cast<const void *>(in_bytes + tiled_offset));
                linear_offset += sizeof(ELEMENT);
            }
        }
}

template <typename T>
void slowDetileOneFragment(const T* t, const SurfaceRegion region, const SurfaceRegion subRegion, int fragment, int destPitch, int destSlicePitch, uint8_t * __restrict out_bytes, const uint8_t * __restrict in_bytes, int bytesPerElement)
{
    if(!hasTexels(subRegion))
        return;
    switch(bytesPerElement)
    {
        case 1:
            slowDetileOneFragment<T,uint8_t,uint16_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
            break;
        case 2:
            slowDetileOneFragment<T,uint16_t,uint32_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
            break;
        case 4:
            slowDetileOneFragment<T,uint32_t,uint64_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
            break;
        case 8:
            slowDetileOneFragment<T,uint64_t,uint128_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
            break;
        case 16:
            slowDetileOneFragment<T,uint128_t,uint256_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
            break;
        default:
            break;
    }
}

int32_t sce::GpuAddress::TilerLinear::tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch)
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledPixels != 0, kStatusInvalidArgument, "outTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inUntiledPixels != 0, kStatusInvalidArgument, "inUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(destRegion != 0, kStatusInvalidArgument, "destRegion must not be NULL.");
	const auto region = *destRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "destRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "destRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "destRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(sourcePitch     ), kStatusInvalidArgument, "destRegion width (%u) must not exceed sourcePitch (%u).",                  width(region),                 sourcePitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(sourceSlicePitch), kStatusInvalidArgument, "destRegion X*Y dimensions (%ux%u) must not exceed sourceSlicePitch (%u).", width(region), height(region), sourceSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inUntiledPixels);
	const auto out_bytes = static_cast<uint8_t*>(outTiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;
    slowTileOneFragment<TilerLinear>(this, region, region, 0, sourcePitch, sourceSlicePitch, out_bytes, in_bytes, bytesPerElement);
	return kStatusSuccess;
}

int32_t sce::GpuAddress::TilerLinear::detileSurface(void *outUntiledPixels, const void *inTiledPixels)
{
	SurfaceRegion srcRegion;
	srcRegion.m_left   = srcRegion.m_top = srcRegion.m_front = 0;
	srcRegion.m_right  = m_linearWidth;
	srcRegion.m_bottom = m_linearHeight;
	srcRegion.m_back   = m_linearDepth;
	return detileSurfaceRegion(outUntiledPixels, inTiledPixels, &srcRegion, m_linearWidth, m_linearWidth*m_linearHeight);
}

int32_t sce::GpuAddress::TilerLinear::detileSurfaceRegion(void *outUntiledPixels, const void *inTiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch)
{
	SCE_GNM_ASSERT_MSG_RETURN(outUntiledPixels != 0, kStatusInvalidArgument, "outUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inTiledPixels != 0, kStatusInvalidArgument, "inTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(srcRegion != 0, kStatusInvalidArgument, "srcRegion must not be NULL.");
	const auto region = *srcRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "srcRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "srcRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "srcRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(destPitch),      kStatusInvalidArgument, "srcRegion width (%u) must not exceed destPitch (%u).",                       width(region),                 destPitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(destSlicePitch), kStatusInvalidArgument, "srcRegion X*Y dimensions (%ux%u) must not exceed destSlicePitchElems (%u).", width(region), height(region), destSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inTiledPixels);
	const auto out_bytes = static_cast<uint8_t*>(outUntiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;
    slowDetileOneFragment<TilerLinear>(this, region, region, 0, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
	return kStatusSuccess;
}


sce::GpuAddress::Tiler1d::Tiler1d()
{
}

sce::GpuAddress::Tiler1d::Tiler1d(const TilingParameters *tp)
{
	int32_t status = init(tp);
	SCE_GNM_UNUSED(status);
	SCE_GNM_ASSERT_MSG(status == GpuAddress::kStatusSuccess, "Tiler1d initialization failed with error code %d", status);
}

sce::GpuAddress::Tiler1d::~Tiler1d()
{
}

int32_t sce::GpuAddress::Tiler1d::init(const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp->m_numFragmentsPerPixel == 1, kStatusInvalidArgument, "tp->m_numFragmentsPerPixel must be 1 for 1D-tiled surfaces.");
	// Use gpu_addr to come up with actual legal/padded surface parameters
	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "computeSurfaceInfo() failed: %d", status);

	// derived inputs
	m_minGpuMode = tp->m_minGpuMode;
	Gnm::MicroTileMode microTileMode;
	GpuAddress::getMicroTileMode(&microTileMode, tp->m_tileMode);
	m_tileMode      = tp->m_tileMode;
	m_arrayMode     = surfInfoOut.m_arrayMode;
	m_microTileMode = microTileMode;
	m_linearWidth  = tp->m_linearWidth;
	m_linearHeight = tp->m_linearHeight;
	m_linearDepth  = tp->m_linearDepth;
	m_bitsPerElement = tp->m_bitsPerFragment;
	m_paddedWidth = surfInfoOut.m_pitch;
	m_paddedHeight = surfInfoOut.m_height;
	m_paddedDepth = surfInfoOut.m_depth;

	// For multi-texel-per-element formats, each block is treated as an element for tiling purposes.
	// This affects a few of the above variables.
	if (tp->m_isBlockCompressed)
	{
		switch(tp->m_bitsPerFragment)
		{
		case 1:
			SCE_GNM_ASSERT_MSG_RETURN(m_microTileMode == Gnm::kMicroTileModeDisplay, kStatusInvalidArgument, "1bpp surfaces must use Gnm::kMicroTileModeDisplay");
			m_bitsPerElement *= 8;
			m_linearWidth  = std::max((m_linearWidth+7)/8, 1U);
			m_paddedWidth  = std::max((m_paddedWidth+7)/8, 1U);
			break;
		case 4:
		case 8:
			m_bitsPerElement *= 16;
			m_linearWidth  = std::max((m_linearWidth+3)/4, 1U);
			m_linearHeight = std::max((m_linearHeight+3)/4, 1U);
			m_paddedWidth  = std::max((m_paddedWidth+3)/4, 1U);
			m_paddedHeight = std::max((m_paddedHeight+3)/4, 1U);
			break;
		case 16:
			// TODO
			break;
		default:
			SCE_GNM_ASSERT_MSG_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", m_bitsPerElement);
			break;
		}
	}
	m_linearSizeBytes = (m_linearWidth * m_linearHeight * m_linearDepth * m_bitsPerElement + 7) / 8;
	m_tiledSizeBytes = static_cast<uint32_t>(surfInfoOut.m_surfaceSize);

	m_tileThickness = (m_arrayMode == Gnm::kArrayMode1dTiledThick) ? 4 : 1;
	m_tileBytes = (kMicroTileWidth * kMicroTileHeight * m_tileThickness * m_bitsPerElement + 7) / 8;
	m_tilesPerRow = m_paddedWidth / kMicroTileWidth;
	m_tilesPerSlice = std::max(m_tilesPerRow * (m_paddedHeight / kMicroTileHeight), 1U);

	// Verify 1D tiling restrictions
	// These restrictions should be addressed by the computeSurfaceInfo() function.  If any of these
	// asserts fire, it probably means computeSurfaceInfo() isn't doing its job correctly.
	SCE_GNM_ASSERT_MSG_RETURN(m_paddedWidth % kMicroTileWidth == 0, kStatusInternalTilingError, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG_RETURN(m_paddedHeight % kMicroTileHeight == 0, kStatusInternalTilingError, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG_RETURN(m_paddedDepth % m_tileThickness == 0, kStatusInternalTilingError, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG_RETURN(((m_paddedHeight * m_paddedWidth * m_tileThickness * m_bitsPerElement / 8) % kPipeInterleaveBytes) == 0, kStatusInternalTilingError, "internal consistency check failed.");
	return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler1d::getTiledElementByteOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledByteOffset != 0, kStatusInvalidArgument, "outTiledByteOffset must not be NULL.");
	uint64_t bitOffset = 0;
	int32_t status = getTiledElementBitOffset(&bitOffset, x, y, z);
	*outTiledByteOffset = bitOffset/8;
	return status;
}
int32_t sce::GpuAddress::Tiler1d::getTiledElementBitOffset(uint64_t *outTiledBitOffset, uint32_t x, uint32_t y, uint32_t z) const
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledBitOffset, kStatusInvalidArgument, "outTiledBitOffset must not be NULL.");
	uint64_t element_index = getElementIndex(x, y, z, m_bitsPerElement, m_microTileMode, m_arrayMode);

	uint64_t slice_offset = (z / m_tileThickness) * m_tilesPerSlice * m_tileBytes;

	uint64_t tile_row_index = y / kMicroTileHeight;
	uint64_t tile_column_index = x / kMicroTileWidth;
	uint64_t tile_offset = ((tile_row_index * m_tilesPerRow) + tile_column_index) * m_tileBytes;

	uint64_t element_offset = element_index * m_bitsPerElement;

	uint64_t final_offset = (slice_offset + tile_offset)*8 + element_offset;

	*outTiledBitOffset = final_offset;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler1d::tileSurface(void *outTiledPixels, const void *untiledPixels)
{
	SurfaceRegion destRegion;
	destRegion.m_left   = destRegion.m_top = destRegion.m_front = 0;
	destRegion.m_right  = m_linearWidth;
	destRegion.m_bottom = m_linearHeight;
	destRegion.m_back   = m_linearDepth;
	return tileSurfaceRegion(outTiledPixels, untiledPixels, &destRegion, m_linearWidth, m_linearWidth*m_linearHeight);
}

int32_t sce::GpuAddress::Tiler1d::tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch)
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledPixels != 0, kStatusInvalidArgument, "outTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inUntiledPixels != 0, kStatusInvalidArgument, "inUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(destRegion != 0, kStatusInvalidArgument, "destRegion must not be NULL.");
	const auto region = *destRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "destRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "destRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "destRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(sourcePitch),      kStatusInvalidArgument, "destRegion width (%u) must not exceed sourcePitch (%u).",                  width(region),                 sourcePitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(sourceSlicePitch), kStatusInvalidArgument, "destRegion X*Y dimensions (%ux%u) must not exceed sourceSlicePitch (%u).", width(region), height(region), sourceSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inUntiledPixels);
	const auto out_bytes = static_cast<uint8_t*>(outTiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;

	const auto tileFunc = getTileFuncSse2(m_microTileMode, m_bitsPerElement);
	if(tileFunc != nullptr && (intptr_t(out_bytes) % 16) == 0)
	{
        Regions regions;
        regions.Init(region, m_tileThickness);
        if(hasTexels(regions.m_aligned))
        {
            const auto dx = regions.m_aligned.m_left  - destRegion->m_left;
            const auto dy = regions.m_aligned.m_top   - destRegion->m_top;
            const auto dz = regions.m_aligned.m_front - destRegion->m_front;
		    for(auto z = 0; z < depth(regions.m_aligned); z += m_tileThickness)
			    for(auto y = 0; y < height(regions.m_aligned); y += kMicroTileHeight)
                {
            		uint64_t inBaseOffset = 0, outBaseOffset = 0;
                    computeLinearElementByteOffset(&inBaseOffset, dx + 0, dy + y, dz + z, 0, sourcePitch, sourceSlicePitch, m_bitsPerElement, 1);
		            getTiledElementByteOffset(&outBaseOffset, regions.m_aligned.m_left + 0, regions.m_aligned.m_top + y, regions.m_aligned.m_front + z);
			        for(auto x = 0; x < width(regions.m_aligned); x += kMicroTileWidth)
			        {
					    tileFunc(out_bytes + outBaseOffset, in_bytes + inBaseOffset, sourcePitch, sourceSlicePitch);
                        inBaseOffset += kMicroTileWidth * bytesPerElement;
                        outBaseOffset += m_tileBytes;
				    }
                }
            for(auto i = 0; i < regions.m_unaligneds; ++i)
			{
				slowTileOneFragment<Tiler1d>(this, region, regions.m_unaligned[i], 0, sourcePitch, sourceSlicePitch, out_bytes, in_bytes, bytesPerElement);
			}
                
        	return kStatusSuccess;
        }
    }
    slowTileOneFragment<Tiler1d>(this, region, region, 0, sourcePitch, sourceSlicePitch, out_bytes, in_bytes, bytesPerElement);
	return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler1d::detileSurface(void *outUntiledPixels, const void *inTiledPixels)
{
	SurfaceRegion srcRegion;
	srcRegion.m_left   = srcRegion.m_top = srcRegion.m_front = 0;
	srcRegion.m_right  = m_linearWidth;
	srcRegion.m_bottom = m_linearHeight;
	srcRegion.m_back   = m_linearDepth;
	return detileSurfaceRegion(outUntiledPixels, inTiledPixels, &srcRegion, m_linearWidth, m_linearWidth*m_linearHeight);
}

int32_t sce::GpuAddress::Tiler1d::detileSurfaceRegion(void *outUntiledPixels, const void *inTiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch)
{
	SCE_GNM_ASSERT_MSG_RETURN(outUntiledPixels != 0, kStatusInvalidArgument, "outUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inTiledPixels != 0, kStatusInvalidArgument, "inTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(srcRegion != 0, kStatusInvalidArgument, "srcRegion must not be NULL.");
	const auto region = *srcRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "srcRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "srcRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "srcRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(destPitch),      kStatusInvalidArgument, "srcRegion width (%u) must not exceed destPitch (%u).",                  width(region),                 destPitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(destSlicePitch), kStatusInvalidArgument, "srcRegion X*Y dimensions (%ux%u) must not exceed destSlicePitch (%u).", width(region), height(region), destSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inTiledPixels);
	const auto out_bytes = static_cast<uint8_t*>(outUntiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;

	const auto detileFunc = getDetileFuncSse2(m_microTileMode, m_bitsPerElement);
	if(nullptr != detileFunc && (intptr_t(in_bytes) % 16) == 0)
	{
        Regions regions;
        regions.Init(region, m_tileThickness);
        if(hasTexels(regions.m_aligned))
        {
            const auto dx = regions.m_aligned.m_left  - srcRegion->m_left;
            const auto dy = regions.m_aligned.m_top   - srcRegion->m_top;
            const auto dz = regions.m_aligned.m_front - srcRegion->m_front;
		    for(auto z = 0; z < depth(regions.m_aligned); z += m_tileThickness)
			    for(auto y = 0; y < height(regions.m_aligned); y += kMicroTileHeight)
                {
            		uint64_t inBaseOffset = 0, outBaseOffset = 0;
		            getTiledElementByteOffset(&inBaseOffset, regions.m_aligned.m_left + 0, regions.m_aligned.m_top + y, regions.m_aligned.m_front + z);
                    computeLinearElementByteOffset(&outBaseOffset, dx + 0, dy + y, dz + z, 0, destPitch, destSlicePitch, m_bitsPerElement, 1);
			        for(auto x = 0; x < width(regions.m_aligned); x += kMicroTileWidth)
			        {
					    detileFunc(out_bytes + outBaseOffset, in_bytes + inBaseOffset, destPitch, destSlicePitch);
                        inBaseOffset += m_tileBytes;
                        outBaseOffset += kMicroTileWidth * bytesPerElement;
				    }
                }
            for(auto i = 0; i < regions.m_unaligneds; ++i)
                slowDetileOneFragment<Tiler1d>(this, region, regions.m_unaligned[i], 0, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
            return kStatusSuccess;
        }
	}
    slowDetileOneFragment<Tiler1d>(this, region, region, 0, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
	return kStatusSuccess;
}

sce::GpuAddress::Tiler2d::Tiler2d()
{
}
sce::GpuAddress::Tiler2d::Tiler2d(const TilingParameters *tp)
{
	int32_t status = init(tp);
	SCE_GNM_UNUSED(status);
	SCE_GNM_ASSERT_MSG(status == GpuAddress::kStatusSuccess, "Tiler2d initialization failed with error code %d", status);
}
sce::GpuAddress::Tiler2d::~Tiler2d()
{
}

int32_t sce::GpuAddress::Tiler2d::init(const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	// Use gpu_addr to come up with actual legal/padded surface parameters
	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "computeSurfaceInfo() failed: %d", status);

	// derived inputs
	m_minGpuMode = tp->m_minGpuMode;
	m_tileMode = tp->m_tileMode;
	m_arrayMode = surfInfoOut.m_arrayMode;
	getMicroTileMode(&m_microTileMode, tp->m_tileMode);

	// other constants
	switch(m_arrayMode)
	{
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayModeTiledThinPrt:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
		m_tileThickness = 1;
		break;
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayModeTiledThickPrt:
	case Gnm::kArrayMode2dTiledThickPrt:
	case Gnm::kArrayMode3dTiledThickPrt:
		m_tileThickness = 4;
		break;
	case Gnm::kArrayMode2dTiledXThick:
	case Gnm::kArrayMode3dTiledXThick:
		m_tileThickness = 8;
		break;
	default:
		break;
	}

	m_linearWidth = tp->m_linearWidth; // unpadded
	m_linearHeight = tp->m_linearHeight; // unpadded
	m_linearDepth = tp->m_linearDepth; // unpadded
	m_bitsPerElement = tp->m_bitsPerFragment;
	m_paddedWidth = surfInfoOut.m_pitch; // padded
	m_paddedHeight = surfInfoOut.m_height;
	m_paddedDepth = surfInfoOut.m_depth;
	m_numFragmentsPerPixel       = tp->m_numFragmentsPerPixel;

	// For multi-texel-per-element formats, each block is treated as an element for tiling purposes.
	// This affects a few of the above variables.
	if (tp->m_isBlockCompressed)
	{
		switch(tp->m_bitsPerFragment)
		{
		case 1:
			SCE_GNM_ASSERT_MSG_RETURN(m_microTileMode == Gnm::kMicroTileModeDisplay, kStatusInvalidArgument, "1bpp surfaces must use Gnm::kMicroTileModeDisplay");
			m_bitsPerElement *= 8;
			m_linearWidth  = std::max((m_linearWidth+7)/8, 1U);
			m_paddedWidth  = std::max((m_paddedWidth+7)/8, 1U);
			break;
		case 4:
		case 8:
			m_bitsPerElement *= 16;
			m_linearWidth  = std::max((m_linearWidth+3)/4, 1U);
			m_linearHeight = std::max((m_linearHeight+3)/4, 1U);
			m_paddedWidth  = std::max((m_paddedWidth+3)/4, 1U);
			m_paddedHeight = std::max((m_paddedHeight+3)/4, 1U);
			break;
		case 16:
			// TODO
			break;
		default:
			SCE_GNM_ASSERT_MSG_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", m_bitsPerElement);
			break;
		}
	}
	m_linearSizeBytes = (m_linearWidth * m_linearHeight * m_linearDepth * m_bitsPerElement * m_numFragmentsPerPixel + 7) / 8;
	m_tiledSizeBytes = surfInfoOut.m_surfaceSize;

	Gnm::BankWidth bankWidthHW;
	Gnm::BankHeight bankHeightHW;
	Gnm::MacroTileAspect macroAspectHW;
	Gnm::NumBanks numBanksHW;
	if (tp->m_minGpuMode == Gnm::kGpuModeNeo)
	{
		getAltPipeConfig(&m_pipeConfig, m_tileMode);
		getAllAltMacroTileData(m_tileMode, m_bitsPerElement, m_numFragmentsPerPixel,
			&bankWidthHW, &bankHeightHW, &macroAspectHW, &numBanksHW);
	}
	else
	{
		getPipeConfig(&m_pipeConfig, m_tileMode);
		getAllMacroTileData(m_tileMode, m_bitsPerElement, m_numFragmentsPerPixel,
			&bankWidthHW, &bankHeightHW, &macroAspectHW, &numBanksHW);
	}
	m_bankWidth        = 1  << bankWidthHW;
	m_bankHeight       = 1  << bankHeightHW;
	m_numBanks         = 2  << numBanksHW;
	m_macroTileAspect  = 1  << macroAspectHW;
	uint32_t tileBytes1x = (m_tileThickness*m_bitsPerElement*kMicroTileWidth*kMicroTileHeight + 7)/8;
	Gnm::SampleSplit sampleSplitHw;
	Gnm::TileSplit tileSplitHw;
	GpuAddress::getSampleSplit(&sampleSplitHw, tp->m_tileMode);
	GpuAddress::getTileSplit(&tileSplitHw, tp->m_tileMode);
	uint32_t sampleSplit = 1 << sampleSplitHw;
	uint32_t tileSplitC   = (m_microTileMode == Gnm::kMicroTileModeDepth)
		? (64 << tileSplitHw) // depth modes store tile split directly
		: std::max(256U, tileBytes1x*sampleSplit); // other modes store a sample split multiplier
	m_tileSplitBytes = std::min(kDramRowSize, tileSplitC);
	// Hardware constants -- see GB_ADDR_CONFIG register
	m_pipeInterleaveBytes = kPipeInterleaveBytes;
	m_numPipes = getPipeCount(m_pipeConfig);
	m_pipeInterleaveBits = fastIntLog2(m_pipeInterleaveBytes);
	m_pipeInterleaveMask = (1 << (m_pipeInterleaveBits)) - 1;
	m_pipeBits = fastIntLog2(m_numPipes);
	m_bankBits = fastIntLog2(m_numBanks);
	m_pipeMask = (m_numPipes-1) << m_pipeInterleaveBits;
	m_bankSwizzleMask = tp->m_tileSwizzleMask;
	m_pipeSwizzleMask = 0; // not currently used
	m_macroTileWidth  = (kMicroTileWidth  * m_bankWidth  * m_numPipes) * m_macroTileAspect;
	m_macroTileHeight = (kMicroTileHeight * m_bankHeight * m_numBanks) / m_macroTileAspect;

	m_arraySlice = tp->m_arraySlice;

	// Verify 2D tiled addressing restrictions
	// These restrictions should be addressed by the computeSurfaceInfo() function.  If any of these
	// asserts fire, it probably means computeSurfaceInfo() isn't doing its job correctly.
	SCE_GNM_ASSERT_MSG_RETURN(m_paddedWidth % m_macroTileWidth == 0, kStatusInternalTilingError, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG_RETURN(m_paddedHeight % m_macroTileHeight == 0, kStatusInternalTilingError, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG_RETURN(m_numBanks * m_numPipes >= 4, kStatusInternalTilingError, "internal consistency check failed.");
	return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler2d::getTiledElementByteOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex) const
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledByteOffset != 0, kStatusInvalidArgument, "outTiledByteOffset must not be NULL.");
	uint64_t bitOffset = 0;
	int32_t status = getTiledElementBitOffset(&bitOffset, x, y, z, fragmentIndex);
	*outTiledByteOffset = bitOffset/8;
	return status;
}
int32_t sce::GpuAddress::Tiler2d::getTiledElementBitOffset(uint64_t *outTiledBitOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex) const
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledBitOffset, kStatusInvalidArgument, "outTiledBitOffset must not be NULL.");

	uint64_t element_index = getElementIndex(x, y, z, m_bitsPerElement, m_microTileMode, m_arrayMode);

	// kArrayTileThinPrt and kArrayModeTiledThickPrt array modes do not use x/y bits beyond the macro tile size
	// to calculate the bank and pipe indices. This is to ensure that multiple virtual texture pages can alias the
	// same physical texture page; otherwise, the tiling of the physical page would be different based on the location
	// of the virtual page.
	uint32_t xh = x, yh = y;
	if (m_arrayMode == Gnm::kArrayModeTiledThinPrt || m_arrayMode == Gnm::kArrayModeTiledThickPrt)
	{
		xh %= m_macroTileWidth;
		yh %= m_macroTileHeight;
	}
	uint64_t pipe = getPipeIndex(xh, yh, m_pipeConfig);
	uint64_t bank = getBankIndex(xh, yh, m_bankWidth, m_bankHeight, m_numBanks, m_numPipes);

	// possibly overwritten below, or else we could hoist outside the loop.  Nevertheless, all uses between here and there should use
	// this value, not new value written ~10 lines below.
	uint32_t tile_bytes = (kMicroTileWidth * kMicroTileHeight * m_tileThickness * m_bitsPerElement * m_numFragmentsPerPixel + 7) / 8;

	uint64_t element_offset = 0;
	if (m_microTileMode == Gnm::kMicroTileModeDepth) // depth surface
	{
		uint64_t pixel_offset = element_index * m_bitsPerElement * m_numFragmentsPerPixel;
		element_offset = pixel_offset + (fragmentIndex * m_bitsPerElement);
	}
	else // color/texture surface
	{
		uint64_t fragment_offset = fragmentIndex * (tile_bytes / m_numFragmentsPerPixel) * 8;
		element_offset = fragment_offset + (element_index * m_bitsPerElement);
	}

	// If a tile is too large, it will need to be split across multiple slices.
	uint64_t slices_per_tile = 1;
	uint64_t tile_split_slice = 0;
	if (tile_bytes > m_tileSplitBytes && m_tileThickness == 1)
	{
		slices_per_tile = tile_bytes / m_tileSplitBytes;
		tile_split_slice = element_offset / (m_tileSplitBytes*8);
		element_offset %= (m_tileSplitBytes*8);
		tile_bytes = m_tileSplitBytes;
	}

	// Verify 2D tiled addressing restrictions
	// These restrictions should be addressed by the computeSurfaceInfo() function.  If any of these
	// asserts fire, it probably means computeSurfaceInfo() isn't doing its job correctly.
	SCE_GNM_ASSERT_MSG_RETURN(kBankInterleave * m_pipeInterleaveBytes <= std::min(m_tileSplitBytes, tile_bytes) * m_bankWidth * m_bankHeight, kStatusInternalTilingError, "internal consistency check failed.");
	SCE_GNM_ASSERT_MSG_RETURN(kBankInterleave * m_pipeInterleaveBytes <= m_numPipes * m_bankWidth * m_macroTileAspect * std::min(m_tileSplitBytes, tile_bytes), kStatusInternalTilingError, "internal consistency check failed.");

	uint64_t macro_tile_bytes = (m_macroTileWidth/kMicroTileWidth) * (m_macroTileHeight/kMicroTileHeight) * tile_bytes / (m_numPipes * m_numBanks);
	uint64_t macro_tiles_per_row = m_paddedWidth / m_macroTileWidth;
	uint64_t macro_tile_row_index = y / m_macroTileHeight;
	uint64_t macro_tile_column_index = x / m_macroTileWidth;
	uint64_t macro_tile_index = (macro_tile_row_index * macro_tiles_per_row) + macro_tile_column_index;
	uint64_t macro_tile_offset = macro_tile_index * macro_tile_bytes;
	uint64_t macro_tiles_per_slice = macro_tiles_per_row * (m_paddedHeight / m_macroTileHeight);
	uint64_t slice_bytes = macro_tiles_per_slice * macro_tile_bytes;

	SCE_GNM_ASSERT_MSG_RETURN(z==0 || m_arraySlice == 0, kStatusInvalidArgument, "arrays of volume textures aren't supported.");
	uint32_t slice = z;
	// Cube maps and texture arrays have faces are rotated the same way as Z slices, but only AFTER the slice_offset
	// has been calculated.
	uint64_t slice_offset = (tile_split_slice + slices_per_tile * slice / m_tileThickness) * slice_bytes;
	if (m_arraySlice != 0)
	{
		slice = m_arraySlice;
	}

	uint64_t tile_row_index = (y / kMicroTileHeight) % m_bankHeight;
	uint64_t tile_column_index = ((x / kMicroTileWidth) / m_numPipes) % m_bankWidth;
	uint64_t tile_index = (tile_row_index * m_bankWidth) + tile_column_index;
	uint64_t tile_offset = tile_index * tile_bytes;

	// Bank and pipe rotation/swizzling.
	uint64_t bank_swizzle = m_bankSwizzleMask; //(tex->m_baseAddress & m_bankMask) >> (m_bankInterleaveBits + m_pipeInterleaveBits + m_pipeBits);
	uint64_t pipe_swizzle = m_pipeSwizzleMask; //(tex->m_baseAddress & m_pipeMask) >> m_pipeInterleaveBits;

	uint64_t pipe_slice_rotation = 0;
	switch(m_arrayMode)
	{
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayMode3dTiledXThick:
		pipe_slice_rotation = std::max(1UL, (m_numPipes/2UL)-1UL) * (slice/m_tileThickness);
		break;
	default:
		break;
	}
	pipe_swizzle += pipe_slice_rotation;
	pipe_swizzle &= (m_numPipes - 1);
	pipe = pipe ^ pipe_swizzle;

	uint32_t slice_rotation = 0;
	switch(m_arrayMode)
	{
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode2dTiledXThick:
		slice_rotation = ((m_numBanks/2)-1) * (slice/m_tileThickness);
		break;
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayMode3dTiledXThick:
		slice_rotation = std::max(1UL, (m_numPipes/2UL)-1UL) * (slice/m_tileThickness) / m_numPipes;
		break;
	default:
		break;
	}
	uint64_t tile_split_slice_rotation = 0;
	switch(m_arrayMode)
	{
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
		tile_split_slice_rotation = ((m_numBanks/2)+1) * tile_split_slice;
		break;
	default:
		break;
	}
	bank ^= bank_swizzle + slice_rotation;
	bank ^= tile_split_slice_rotation;
	bank &= (m_numBanks - 1);

	uint64_t total_offset = (slice_offset + macro_tile_offset + tile_offset)*8 + element_offset;
	uint64_t bitOffset = total_offset & 0x7;
	total_offset /= 8;

	uint64_t pipe_interleave_offset = total_offset & m_pipeInterleaveMask;
	uint64_t offset = total_offset >> m_pipeInterleaveBits;

	uint64_t finalByteOffset = pipe_interleave_offset |
		(pipe   << (m_pipeInterleaveBits)) |
		(bank   << (m_pipeInterleaveBits + m_pipeBits)) |
		(offset << (m_pipeInterleaveBits + m_pipeBits + m_bankBits));
	*outTiledBitOffset = (finalByteOffset << 3) | bitOffset;

	return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler2d::tileSurface(void *outTiledPixels, const void *inUntiledPixels)
{
	SurfaceRegion destRegion;
	destRegion.m_left   = destRegion.m_top = destRegion.m_front = 0;
	destRegion.m_right  = m_linearWidth;
	destRegion.m_bottom = m_linearHeight;
	destRegion.m_back   = m_linearDepth;
	return tileSurfaceRegion(outTiledPixels, inUntiledPixels, &destRegion, m_linearWidth, m_linearWidth*m_linearHeight);
}


int32_t sce::GpuAddress::Tiler2d::tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch)
{
    if(1 == m_numFragmentsPerPixel)
        return tileSurfaceRegionOneFragment(outTiledPixels, inUntiledPixels, destRegion, sourcePitch, sourceSlicePitch, 0);

	SCE_GNM_ASSERT_MSG_RETURN(outTiledPixels != 0, kStatusInvalidArgument, "outTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inUntiledPixels != 0, kStatusInvalidArgument, "inUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(destRegion != 0, kStatusInvalidArgument, "destRegion must not be NULL.");
	const auto region = *destRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "destRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "destRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "destRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(sourcePitch),     kStatusInvalidArgument, "destRegion width (%u) must not exceed sourcePitch (%u).",                   width(region),                 sourcePitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(sourceSlicePitch), kStatusInvalidArgument, "destRegion X*Y dimensions (%ux%u) must not exceed sourceSlicePitch (%u).", width(region), height(region), sourceSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inUntiledPixels);
	const auto out_bytes = static_cast<uint8_t*>(outTiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;
	for(auto z=0; z<depth(region); ++z)
		for(auto y=0; y<height(region); ++y)
			for(auto x=0; x<width(region); ++x)
				for(uint32_t fragment=0; fragment < m_numFragmentsPerPixel; ++fragment)
				{
					uint64_t linear_offset, tiled_offset;
					getTiledElementByteOffset(&tiled_offset, region.m_left+x, region.m_top+y, region.m_front+z, fragment);
					computeLinearElementByteOffset(&linear_offset, x,y,z,fragment, sourcePitch, sourceSlicePitch, m_bitsPerElement, m_numFragmentsPerPixel);
					small_memcpy(out_bytes + tiled_offset, in_bytes + linear_offset, bytesPerElement);
				}
	return kStatusSuccess;
}

struct Offset
{
    uint8_t m_x;
    uint8_t m_y;
    uint8_t m_z;
    uint8_t m_w;
};

struct Offsets
{
    uint32_t m_cacheLinesPerFragment;
    Offset m_offset[16];
};

static const Offsets g_offsetOfCacheLine[][5] =
{
    { // DISPLAY
        { 1, {{0,0}}                                           }, //  8bpp
        { 2, {{0,0},{0,4}}                                     }, // 16bpp
        { 4, {{0,0},{0,2},{0,4},{0,6}}                         }, // 32bpp
        { 8, {{0,0},{4,0},{0,2},{4,2},{0,4},{4,4},{0,6},{4,6}} }, // 64bpp
    },
    { // THIN
        { 1, {{0,0}}                                           }, //   8bpp
        { 2, {{0,0},{0,4}}                                     }, //  16bpp
        { 4, {{0,0},{4,0},{0,4},{4,4}}                         }, //  32bpp
        { 8, {{0,0},{0,2},{4,0},{4,2},{0,4},{0,6},{4,4},{4,6}} }, //  64bpp
        {16, {{0,0},{2,0},{0,2},{2,2},{4,0},{6,0},{4,2},{6,2},
              {0,4},{2,4},{0,6},{2,6},{4,4},{6,4},{4,6},{6,6}} }, // 128bpp
    },
    { // DEPTH
        { 1, {{0,0}}                                           }, //   8bpp
        { 2, {{0,0},{0,4}}                                     }, //  16bpp
        { 4, {{0,0},{4,0},{0,4},{4,4}}                         }, //  32bpp
        { 8, {{0,0},{0,2},{4,0},{4,2},{0,4},{0,6},{4,4},{4,6}} }, //  64bpp
        {16, {{0,0},{2,0},{0,2},{2,2},{4,0},{6,0},{4,2},{6,2},
              {0,4},{2,4},{0,6},{2,6},{4,4},{6,4},{4,6},{6,6}} }, // 128bpp
    },
};

int32_t sce::GpuAddress::Tiler2d::tileSurfaceRegionOneFragment(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch, uint32_t fragment)
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledPixels != 0, kStatusInvalidArgument, "outTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inUntiledPixels != 0, kStatusInvalidArgument, "inUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(destRegion != 0, kStatusInvalidArgument, "destRegion must not be NULL.");
    SCE_GNM_ASSERT_MSG_RETURN(fragment < m_numFragmentsPerPixel, kStatusInvalidArgument, "fragment (%u) must be < m_numFragmentsPerPixel (%u).", fragment, m_numFragmentsPerPixel);
	const auto region = *destRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "destRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "destRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "destRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int32_t(sourcePitch),      kStatusInvalidArgument, "destRegion width (%u) must not exceed sourcePitch (%u).",                       width(region), sourcePitch);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int32_t(sourceSlicePitch), kStatusInvalidArgument, "destRegion X*Y dimensions (%ux%u) must not exceed sourceSlicePitchElems (%u).", width(region), height(region), sourceSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inUntiledPixels);
    const auto out_bytes = static_cast<uint8_t*>(outTiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;

	if(m_microTileMode == Gnm::kMicroTileModeDepth && m_numFragmentsPerPixel > 1)
	{
		for(auto z = 0; z < depth(region); ++z)
			for(auto y = 0; y < height(region); ++y)
			{
				uint64_t linear_offset;
				computeLinearElementByteOffset(&linear_offset,0,y,z,0,sourcePitch,sourceSlicePitch,m_bitsPerElement,1);
				for(auto x = 0; x < width(region); ++x)
				{
					uint64_t tiled_offset;
					getTiledElementByteOffset(&tiled_offset, region.m_left + x, region.m_top + y, region.m_front + z, fragment);
					small_memcpy(out_bytes + tiled_offset, in_bytes + linear_offset, bytesPerElement);
					linear_offset += bytesPerElement;
				}
			}
		return kStatusSuccess;
	}

    bool canTakeFastPath = true;
    if(m_microTileMode >= sizeof(g_offsetOfCacheLine)/sizeof(g_offsetOfCacheLine[0]))
        canTakeFastPath = false;
    if(canTakeFastPath)
    {
        Regions regions;
        regions.Init(region, m_tileThickness);
        if(hasTexels(regions.m_aligned))
        {   
            const auto microTileFunc = getTileFuncSse2(m_microTileMode, m_bitsPerElement);
            SCE_GNM_ASSERT_MSG_RETURN(nullptr != microTileFunc, kStatusInvalidArgument, "Can't find SSE2 tiling function for micro tilemode %d.", m_microTileMode);
            const auto offsetOfCacheLine = &g_offsetOfCacheLine[m_microTileMode][fastIntLog2(bytesPerElement)];
            const int dx = regions.m_aligned.m_left   - region.m_left;
            const int dy = regions.m_aligned.m_top    - region.m_top;
            const int dz = regions.m_aligned.m_front  - region.m_front;
	        for(auto z = 0; z < depth(regions.m_aligned); z += m_tileThickness)
		        for(auto y = 0; y < height(regions.m_aligned); y += kMicroTileHeight)
			        for(auto x = 0; x < width(regions.m_aligned); x += kMicroTileWidth)
                    {
                        // Due to tile split, the cache lines of a microtile may be stored non-contiguously.
                        // But to use the optimized microtile detiler, all cache lines of a microtile must be stored contiguously.
                        // Therefore, here we gather all the cache lines together into a temporary buffer before proceeding...
    			        uint64_t linear_offset;
				        computeLinearElementByteOffset(&linear_offset, dx + x, dy + y, dz + z, 0, sourcePitch, sourceSlicePitch, m_bitsPerElement, 1);
                        uint8_t contiguous[16][64];
                        microTileFunc(contiguous, in_bytes + linear_offset, sourcePitch, sourceSlicePitch);
                        for(auto cacheLine = 0U; cacheLine < offsetOfCacheLine->m_cacheLinesPerFragment; ++cacheLine) 
                        {
                            const auto cacheLineX = regions.m_aligned.m_left  + x + offsetOfCacheLine->m_offset[cacheLine].m_x;
                            const auto cacheLineY = regions.m_aligned.m_top   + y + offsetOfCacheLine->m_offset[cacheLine].m_y;
                            const auto cacheLineZ = regions.m_aligned.m_front + z + offsetOfCacheLine->m_offset[cacheLine].m_z;
					        uint64_t tiled_offset;
					        getTiledElementByteOffset(&tiled_offset, cacheLineX, cacheLineY, cacheLineZ, fragment);
                            memcpy(out_bytes + tiled_offset, contiguous[cacheLine], 64);
                        }
			        }
            for(auto i = 0; i < regions.m_unaligneds; ++i)
                slowTileOneFragment<Tiler2d>(this, region, regions.m_unaligned[i], fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes, bytesPerElement);
            return kStatusSuccess;
        }
    }
    slowTileOneFragment<Tiler2d>(this, region, region, fragment, sourcePitch, sourceSlicePitch, out_bytes, in_bytes, bytesPerElement);
    return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler2d::detileSurface(void *outUntiledPixels, const void *tiledPixels)
{
	SurfaceRegion srcRegion;
	srcRegion.m_left   = srcRegion.m_top = srcRegion.m_front = 0;
	srcRegion.m_right  = m_linearWidth;
	srcRegion.m_bottom = m_linearHeight;
	srcRegion.m_back   = m_linearDepth;
	return detileSurfaceRegion(outUntiledPixels, tiledPixels, &srcRegion, m_linearWidth, m_linearWidth*m_linearHeight);
}

int32_t sce::GpuAddress::Tiler2d::detileSurfaceRegion(void *outUntiledPixels, const void *inTiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch)
{
    if(1 == m_numFragmentsPerPixel)
        return detileSurfaceRegionOneFragment(outUntiledPixels, inTiledPixels, srcRegion, destPitch, destSlicePitch, 0);

	SCE_GNM_ASSERT_MSG_RETURN(outUntiledPixels != 0, kStatusInvalidArgument, "outUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inTiledPixels != 0, kStatusInvalidArgument, "inTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(srcRegion != 0, kStatusInvalidArgument, "srcRegion must not be NULL.");
	const auto region = *srcRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "srcRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "srcRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "srcRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(destPitch),      kStatusInvalidArgument, "srcRegion width (%u) must not exceed destPitch (%u).",		                width(region),                 destPitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(destSlicePitch), kStatusInvalidArgument, "srcRegion X*Y dimensions (%ux%u) must not exceed destSlicePitchElems (%u).", width(region), height(region), destSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inTiledPixels);
	const auto out_bytes = static_cast<uint8_t*>(outUntiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;
	for(auto z=0; z<depth(region); ++z)
		for(auto y=0; y<height(region); ++y)
			for(auto x=0; x<width(region); ++x)
				for(uint32_t fragment=0; fragment < m_numFragmentsPerPixel; ++fragment)
				{
					uint64_t linear_offset, tiled_offset;
					getTiledElementByteOffset(&tiled_offset, region.m_left+x, region.m_top+y, region.m_front+z, fragment);
					computeLinearElementByteOffset(&linear_offset, x,y,z,fragment, destPitch,destSlicePitch, m_bitsPerElement, m_numFragmentsPerPixel);
					small_memcpy(out_bytes + linear_offset, in_bytes + tiled_offset, bytesPerElement);
				}
	return kStatusSuccess;
}

int32_t sce::GpuAddress::Tiler2d::detileSurfaceRegionOneFragment(void *outUntiledPixels, const void *inTiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch, uint32_t fragment)
{
	SCE_GNM_ASSERT_MSG_RETURN(outUntiledPixels != 0, kStatusInvalidArgument, "outUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inTiledPixels != 0, kStatusInvalidArgument, "inTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(srcRegion != 0, kStatusInvalidArgument, "srcRegion must not be NULL.");
    SCE_GNM_ASSERT_MSG_RETURN(fragment < m_numFragmentsPerPixel, kStatusInvalidArgument, "fragment (%u) must be < m_numFragmentsPerPixel (%u).", fragment, m_numFragmentsPerPixel);
	const auto region = *srcRegion;
	if(!hasTexels(region))
		return kStatusSuccess; // Zero-area region; nothing to do.
	SCE_GNM_ASSERT_MSG_RETURN(region.m_right  <= m_linearWidth,  kStatusInvalidArgument, "srcRegion m_right (%u) must not exceed destination surface's width (%u).",   region.m_right,  m_linearWidth);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "srcRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	SCE_GNM_ASSERT_MSG_RETURN(region.m_back   <= m_linearDepth,  kStatusInvalidArgument, "srcRegion m_back (%u) must not exceed destination surface's depth (%u).",    region.m_back,   m_linearDepth);
	SCE_GNM_ASSERT_MSG_RETURN(width(region)                <= int(destPitch),     kStatusInvalidArgument, "srcRegion width (%u) must not exceed destPitch (%u).",                        width(region),                 destPitch     );
	SCE_GNM_ASSERT_MSG_RETURN(width(region)*height(region) <= int(destSlicePitch), kStatusInvalidArgument, "srcRegion X*Y dimensions (%ux%u) must not exceed destSlicePitchElems (%u).", width(region), height(region), destSlicePitch);

	const auto in_bytes = static_cast<const uint8_t*>(inTiledPixels);
   	const auto out_bytes = static_cast<uint8_t*>(outUntiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;

	if(m_microTileMode == Gnm::kMicroTileModeDepth && m_numFragmentsPerPixel > 1)
	{
		for(auto z = 0; z < depth(region); ++z)
			for(auto y = 0; y < height(region); ++y)
			{
				uint64_t linear_offset;
				computeLinearElementByteOffset(&linear_offset, 0, y, z, 0, destPitch, destSlicePitch, m_bitsPerElement, 1);
				for(auto x = 0; x < width(region); ++x)
				{
					uint64_t tiled_offset;
					getTiledElementByteOffset(&tiled_offset, region.m_left + x, region.m_top + y, region.m_front + z, fragment);
					small_memcpy(out_bytes + linear_offset, in_bytes + tiled_offset, bytesPerElement);
					linear_offset += bytesPerElement;
				}
			}
		return kStatusSuccess;
	}

    bool canTakeFastPath = true;
    if(m_microTileMode >= sizeof(g_offsetOfCacheLine)/sizeof(g_offsetOfCacheLine[0]))
        canTakeFastPath = false;
    if(canTakeFastPath)
    {
        Regions regions;
        regions.Init(region, m_tileThickness);
        if(hasTexels(regions.m_aligned))
        {
            const auto microTileFunc = getDetileFuncSse2(m_microTileMode, m_bitsPerElement);
            SCE_GNM_ASSERT_MSG_RETURN(nullptr != microTileFunc, kStatusInvalidArgument, "Can't find SSE2 detiling function for micro tilemode %d.", m_microTileMode);
            const auto offsetOfCacheLine = &g_offsetOfCacheLine[m_microTileMode][fastIntLog2(bytesPerElement)];
            const int dx = regions.m_aligned.m_left   - region.m_left;
            const int dy = regions.m_aligned.m_top    - region.m_top;
            const int dz = regions.m_aligned.m_front  - region.m_front;
	        for(auto z = 0; z < depth(regions.m_aligned); z += m_tileThickness)
		        for(auto y = 0; y < height(regions.m_aligned); y += kMicroTileHeight)
			        for(auto x = 0; x < width(regions.m_aligned); x += kMicroTileWidth)
                    {
                        // Due to tile split, the cache lines of a microtile may be stored non-contiguously.
                        // But to use the optimized microtile detiler, all cache lines of a microtile must be stored contiguously.
                        // Therefore, here we gather all the cache lines together into a temporary buffer before proceeding...
                        uint8_t contiguous[16][64];
                        for(auto cacheLine = 0U; cacheLine < offsetOfCacheLine->m_cacheLinesPerFragment; ++cacheLine) 
                        {
                            const auto cacheLineX = regions.m_aligned.m_left  + x + offsetOfCacheLine->m_offset[cacheLine].m_x;
                            const auto cacheLineY = regions.m_aligned.m_top   + y + offsetOfCacheLine->m_offset[cacheLine].m_y;
                            const auto cacheLineZ = regions.m_aligned.m_front + z + offsetOfCacheLine->m_offset[cacheLine].m_z;
					        uint64_t tiled_offset;
					        getTiledElementByteOffset(&tiled_offset, cacheLineX, cacheLineY, cacheLineZ, fragment);
                            memcpy(contiguous[cacheLine], in_bytes + tiled_offset, 64);
                        }
                        // Now that we have one contiguous microtile, we can pass it to the optimized microtile detiler...
    			        uint64_t linear_offset;
				        computeLinearElementByteOffset(&linear_offset, dx + x, dy + y, dz + z, 0, destPitch, destSlicePitch, m_bitsPerElement, 1);
                        microTileFunc(out_bytes + linear_offset, contiguous, destPitch, destSlicePitch);
			        }
            for(auto i = 0; i < regions.m_unaligneds; ++i)
                slowDetileOneFragment<Tiler2d>(this, region, regions.m_unaligned[i], fragment, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
            return kStatusSuccess;
        }
    }
    slowDetileOneFragment<Tiler2d>(this, region, region, fragment, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
    return kStatusSuccess;
}

int32_t sce::GpuAddress::TilingParameters::initFromTexture(const Gnm::Texture *texture, uint32_t mipLevel, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(texture != 0, kStatusInvalidArgument, "texture must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(mipLevel <= texture->getLastMipLevel(), kStatusInvalidArgument, "mipLevel (%u) is out of range for texture; last level is %u", mipLevel, texture->getLastMipLevel());
	bool isCubemap = (texture->getTextureType() == Gnm::kTextureTypeCubemap);
	bool isVolume = (texture->getTextureType() == Gnm::kTextureType3d);
	// Building surface flags manually is error-prone, but we don't know exactly what type of texture this is.
	m_surfaceFlags.m_value = 0;
	Gnm::MicroTileMode microTileMode;
	int32_t status = getMicroTileMode(&microTileMode, texture->getTileMode());
	if (status != kStatusSuccess)
		return status;
	m_surfaceFlags.m_depthTarget   = (!isVolume && (microTileMode == Gnm::kMicroTileModeDepth) && (texture->getDataFormat().getZFormat()       != Gnm::kZFormatInvalid)) ? 1 : 0;
	m_surfaceFlags.m_stencilTarget = (!isVolume && (microTileMode == Gnm::kMicroTileModeDepth) && (texture->getDataFormat().getStencilFormat() != Gnm::kStencilInvalid)) ? 1 : 0;
	m_surfaceFlags.m_cube = isCubemap ? 1 : 0;
	m_surfaceFlags.m_volume = isVolume ? 1 : 0;
	m_surfaceFlags.m_pow2Pad = texture->isPaddedToPow2() ? 1 : 0;
	if (texture->getMinimumGpuMode() == Gnm::kGpuModeNeo)
	{
		m_surfaceFlags.m_texCompatible = 1;
	}
	m_tileMode = texture->getTileMode(); // see below, though
	m_minGpuMode = texture->getMinimumGpuMode();
	Gnm::DataFormat dataFormat = texture->getDataFormat();
	m_bitsPerFragment = dataFormat.getTotalBitsPerElement() / dataFormat.getTexelsPerElement();
	m_isBlockCompressed = (dataFormat.getTexelsPerElement() > 1);
	m_tileSwizzleMask = texture->getTileSwizzleMask();
	m_linearWidth = std::max(texture->getWidth() >> mipLevel, 1U);
	m_linearHeight = std::max(texture->getHeight() >> mipLevel, 1U);
	m_linearDepth = m_surfaceFlags.m_volume ? std::max(texture->getDepth() >> mipLevel, 1U) : 1;
	m_numFragmentsPerPixel = 1 << texture->getNumFragments();
	m_baseTiledPitch = texture->getPitch();
	m_mipLevel = mipLevel;
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice == 0 || !m_surfaceFlags.m_volume, kStatusInvalidArgument, "for volume textures, arraySlice must be 0."); // volume textures can't be arrays
	uint32_t arraySliceCount = texture->getTotalArraySliceCount();
	if (isCubemap)
		arraySliceCount *= 6; // Cube maps store 6 faces per array slice
	else if (isVolume)
		arraySliceCount = 1;
	if (texture->isPaddedToPow2())
		arraySliceCount = nextPowerOfTwo(arraySliceCount); // array slice counts are padded to a power of two as well
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice < arraySliceCount, kStatusInvalidArgument, "arraySlice (%u) is out of range for texture (0x%p) with %u slices.", arraySlice, texture, arraySliceCount);
	m_arraySlice = arraySlice;
	// Use computeSurfaceInfo() to determine what array mode we REALLY need to use, since it's occasionally not the one the Texture uses.
	// (e.g. for a 2D-tiled texture, the smaller mip levels will implicitly use a 1D array mode to cut down on wasted padding space)
	SurfaceInfo surfInfoOut = {0};
	status = computeSurfaceInfo(&surfInfoOut, this);
	if (status != kStatusSuccess)
		return status;
	status = adjustTileMode(m_minGpuMode, &m_tileMode, m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::TilingParameters::initFromRenderTarget(const Gnm::RenderTarget *target, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(target != 0, kStatusInvalidArgument, "target must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice <= target->getLastArraySliceIndex(), kStatusInvalidArgument, "arraySlice (%u) is out of range for target; last slice is %u", arraySlice, target->getLastArraySliceIndex());
	m_tileMode = target->getTileMode(); // see below, though
	m_minGpuMode = target->getMinimumGpuMode();
	Gnm::DataFormat dataFormat = target->getDataFormat();
	m_bitsPerFragment = dataFormat.getTotalBitsPerElement() / dataFormat.getTexelsPerElement();
	m_isBlockCompressed = (dataFormat.getTexelsPerElement() > 1);
	m_tileSwizzleMask = target->getTileSwizzleMask();
	m_linearWidth = target->getWidth();
	m_linearHeight = target->getHeight();
	m_linearDepth = 1;
	m_numFragmentsPerPixel = 1 << target->getNumFragments();
	m_baseTiledPitch = target->getPitch();
	m_mipLevel = 0; // unused for render targets
	m_arraySlice = arraySlice;
	Gnm::MicroTileMode microTileMode;
	int32_t status = getMicroTileMode(&microTileMode, target->getTileMode());
	if (status != kStatusSuccess)
		return status;
	status = getFlagsForSurfaceType(m_minGpuMode, &m_surfaceFlags, (microTileMode == Gnm::kMicroTileModeDisplay) ? kSurfaceTypeColorTargetDisplayable : kSurfaceTypeColorTarget, kSurfaceMipmapDisable);
	if (m_minGpuMode == Gnm::kGpuModeNeo)
	{
		bool independentDccBlocks = SCE_GNM_GET_FIELD(target->m_regs[Gnm::RenderTarget::kCbColorDccControl], CB_COLOR0_DCC_CONTROL, INDEPENDENT_64B_BLOCKS) != 0; // [vi]
		Gnm::DccBlockSize maxUncompressedBlockSize = (Gnm::DccBlockSize)SCE_GNM_GET_FIELD(target->m_regs[Gnm::RenderTarget::kCbColorDccControl], CB_COLOR0_DCC_CONTROL, MAX_COMPRESSED_BLOCK_SIZE); // [vi]
		bool isDccEnabled = target->getDccCompressionEnable();
		if (isDccEnabled && (!independentDccBlocks || maxUncompressedBlockSize>Gnm::kDccBlockSize64))
			m_surfaceFlags.m_texCompatible = 0;
		else
			m_surfaceFlags.m_texCompatible = 1;
	}
	if (status != kStatusSuccess)
		return status;

	// Use computeSurfaceInfo() to determine what array mode we REALLY need to use, since it's occasionally not the one the Texture uses.
	// (e.g. for a 2D-tiled texture, the smaller mip levels will implicitly use a 1D array mode to cut down on wasted padding space)
	SurfaceInfo surfInfoOut = {0};
	status = computeSurfaceInfo(&surfInfoOut, this);
	if (status != kStatusSuccess)
		return status;
	status = adjustTileMode(m_minGpuMode, &m_tileMode, m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::TilingParameters::initFromDepthSurface(const Gnm::DepthRenderTarget *target, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(target != 0, kStatusInvalidArgument, "target must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice <= target->getLastArraySliceIndex(), kStatusInvalidArgument, "arraySlice (%u) is out of range for target; last slice is %u", arraySlice, target->getLastArraySliceIndex());
	m_tileMode = target->getTileMode(); // see below, though
	m_minGpuMode = target->getMinimumGpuMode();
	Gnm::DataFormat dataFormat = Gnm::DataFormat::build(target->getZFormat());
	m_bitsPerFragment = dataFormat.getTotalBitsPerElement() / dataFormat.getTexelsPerElement();
	m_isBlockCompressed = (dataFormat.getTexelsPerElement() > 1);
	m_tileSwizzleMask = target->getZReadTileSwizzleMask();
	m_linearWidth = target->getWidth();
	m_linearHeight = target->getHeight();
	m_linearDepth = 1;
	m_numFragmentsPerPixel = 1 << target->getNumFragments();
	m_baseTiledPitch = target->getPitch();
	m_mipLevel = 0; // unused for render targets
	m_arraySlice = arraySlice;
	int32_t status = getFlagsForSurfaceType(m_minGpuMode, &m_surfaceFlags, kSurfaceTypeDepthOnlyTarget, kSurfaceMipmapDisable);
	if (m_minGpuMode == Gnm::kGpuModeNeo)
	{
		m_surfaceFlags.m_texCompatible = 1;
	}
	if (status != kStatusSuccess)
		return status;

	// Use computeSurfaceInfo() to determine what array mode we REALLY need to use, since it's occasionally not the one the Texture uses.
	// (e.g. for a 2D-tiled texture, the smaller mip levels will implicitly use a 1D array mode to cut down on wasted padding space)
	SurfaceInfo surfInfoOut = {0};
	status = computeSurfaceInfo(&surfInfoOut, this);
	if (status != kStatusSuccess)
		return status;
	status = adjustTileMode(m_minGpuMode, &m_tileMode, m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::TilingParameters::initFromStencilSurface(const Gnm::DepthRenderTarget *target, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(target != 0, kStatusInvalidArgument, "target must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice <= target->getLastArraySliceIndex(), kStatusInvalidArgument, "arraySlice (%u) is out of range for target; last slice is %u", arraySlice, target->getLastArraySliceIndex());
	m_tileMode = target->getTileMode(); // see below, though
	m_minGpuMode = target->getMinimumGpuMode();
	Gnm::DataFormat dataFormat = Gnm::DataFormat::build(target->getStencilFormat(), Gnm::kTextureChannelTypeFloat);
	m_bitsPerFragment = dataFormat.getTotalBitsPerElement() / dataFormat.getTexelsPerElement();
	m_isBlockCompressed = (dataFormat.getTexelsPerElement() > 1);
	m_tileSwizzleMask = target->getStencilReadTileSwizzleMask();
	m_linearWidth = target->getWidth();
	m_linearHeight = target->getHeight();
	m_linearDepth = 1;
	m_numFragmentsPerPixel = 1 << target->getNumFragments();
	m_baseTiledPitch = target->getPitch();
	m_mipLevel = 0; // unused for render targets
	m_arraySlice = arraySlice;
	int32_t status = getFlagsForSurfaceType(m_minGpuMode, &m_surfaceFlags, kSurfaceTypeStencilOnlyTarget, kSurfaceMipmapDisable);
	if (m_minGpuMode == Gnm::kGpuModeNeo)
	{
		m_surfaceFlags.m_texCompatible = 1;
	}
	if (status != kStatusSuccess)
		return status;

	// Use computeSurfaceInfo() to determine what array mode we REALLY need to use, since it's occasionally not the one the Texture uses.
	// (e.g. for a 2D-tiled texture, the smaller mip levels will implicitly use a 1D array mode to cut down on wasted padding space)
	SurfaceInfo surfInfoOut = {0};
	status = computeSurfaceInfo(&surfInfoOut, this);
	if (status != kStatusSuccess)
		return status;
	status = adjustTileMode(m_minGpuMode, &m_tileMode, m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeTiledSurfaceSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(outSize != NULL, kStatusInvalidArgument, "outSize must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(outAlign != NULL, kStatusInvalidArgument, "outAlign must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp->m_linearDepth == 1 || tp->m_surfaceFlags.m_volume, kStatusInvalidArgument, "for non-volume textures, tp->m_linearDepth must be 1.");
	// Use gpu_addr to come up with actual legal/padded surface parameters
	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	if (status != kStatusSuccess)
		return status;

	*outSize = surfInfoOut.m_surfaceSize;
	*outAlign = (Gnm::AlignmentType)surfInfoOut.m_baseAlign;
	return kStatusSuccess;
}
int32_t sce::GpuAddress::computeUntiledSurfaceSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(outSize != NULL, kStatusInvalidArgument, "outSize must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(outAlign != NULL, kStatusInvalidArgument, "outAlign must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp->m_linearDepth == 1 || tp->m_surfaceFlags.m_volume, kStatusInvalidArgument, "for non-volume textures, tp->m_linearDepth must be 1.");
	uint32_t bitsPerElement = tp->m_bitsPerFragment;
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(bitsPerElement) && bitsPerElement > 0 && bitsPerElement <= 128, kStatusInvalidArgument, "bitsPerFragment (%u) must be a power of 2 in the range [1..128].", bitsPerElement);
	// Can't use gpu_addr here, as even linear-general surfaces get padded/aligned.  This function should calculate the bare minimum tightly-packed size.
	uint64_t paddedWidth  = tp->m_linearWidth;
	uint64_t paddedHeight = tp->m_linearHeight;
	if (tp->m_isBlockCompressed)
	{
		switch(tp->m_bitsPerFragment)
		{
		case 1:
			bitsPerElement = 8;
			paddedWidth = std::max((paddedWidth+7ULL)/8ULL, 1ULL);
			break;
		case 4:
		case 8:
			bitsPerElement *= 16;
			paddedWidth  = std::max((paddedWidth+3ULL)/4ULL, 1ULL);
			paddedHeight = std::max((paddedHeight+3ULL)/4ULL, 1ULL);
			break;
		case 16:
			// TODO
			break;
		default:
			SCE_GNM_ASSERT_MSG_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", bitsPerElement);
			break;
		}
	}
	uint64_t totalBits = paddedWidth * paddedHeight * tp->m_linearDepth * tp->m_numFragmentsPerPixel * bitsPerElement;

	*outSize = (totalBits+7) / 8;
	*outAlign = (Gnm::AlignmentType)1;
	return kStatusSuccess;
}

static const uint16_t g_pitches[15][7] =
{
  { // xy=1
    32, // f=0 [Gnm::kDataFormatR8Uint]
    16, // f=1 [Gnm::kDataFormatR16Uint]
    8, // f=2 [Gnm::kDataFormatR32Uint]
    8, // f=3 [Gnm::kDataFormatR32G32Uint]
    8, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    32, // f=5 [Gnm::kDataFormatBc1Unorm]
    32, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=2
    32, // f=0 [Gnm::kDataFormatR8Uint]
    16, // f=1 [Gnm::kDataFormatR16Uint]
    8, // f=2 [Gnm::kDataFormatR32Uint]
    8, // f=3 [Gnm::kDataFormatR32G32Uint]
    8, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    32, // f=5 [Gnm::kDataFormatBc1Unorm]
    32, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=4
    32, // f=0 [Gnm::kDataFormatR8Uint]
    16, // f=1 [Gnm::kDataFormatR16Uint]
    8, // f=2 [Gnm::kDataFormatR32Uint]
    8, // f=3 [Gnm::kDataFormatR32G32Uint]
    8, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    32, // f=5 [Gnm::kDataFormatBc1Unorm]
    32, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=8
    32, // f=0 [Gnm::kDataFormatR8Uint]
    16, // f=1 [Gnm::kDataFormatR16Uint]
    8, // f=2 [Gnm::kDataFormatR32Uint]
    8, // f=3 [Gnm::kDataFormatR32G32Uint]
    8, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    32, // f=5 [Gnm::kDataFormatBc1Unorm]
    32, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=16
    16, // f=0 [Gnm::kDataFormatR8Uint]
    16, // f=1 [Gnm::kDataFormatR16Uint]
    16, // f=2 [Gnm::kDataFormatR32Uint]
    16, // f=3 [Gnm::kDataFormatR32G32Uint]
    16, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    32, // f=5 [Gnm::kDataFormatBc1Unorm]
    32, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=32
    32, // f=0 [Gnm::kDataFormatR8Uint]
    32, // f=1 [Gnm::kDataFormatR16Uint]
    32, // f=2 [Gnm::kDataFormatR32Uint]
    32, // f=3 [Gnm::kDataFormatR32G32Uint]
    32, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    32, // f=5 [Gnm::kDataFormatBc1Unorm]
    32, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=64
    64, // f=0 [Gnm::kDataFormatR8Uint]
    64, // f=1 [Gnm::kDataFormatR16Uint]
    64, // f=2 [Gnm::kDataFormatR32Uint]
    64, // f=3 [Gnm::kDataFormatR32G32Uint]
    64, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    64, // f=5 [Gnm::kDataFormatBc1Unorm]
    64, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=128
    128, // f=0 [Gnm::kDataFormatR8Uint]
    128, // f=1 [Gnm::kDataFormatR16Uint]
    128, // f=2 [Gnm::kDataFormatR32Uint]
    128, // f=3 [Gnm::kDataFormatR32G32Uint]
    128, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    128, // f=5 [Gnm::kDataFormatBc1Unorm]
    128, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=256
    256, // f=0 [Gnm::kDataFormatR8Uint]
    256, // f=1 [Gnm::kDataFormatR16Uint]
    256, // f=2 [Gnm::kDataFormatR32Uint]
    256, // f=3 [Gnm::kDataFormatR32G32Uint]
    256, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    256, // f=5 [Gnm::kDataFormatBc1Unorm]
    256, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=512
    512, // f=0 [Gnm::kDataFormatR8Uint]
    512, // f=1 [Gnm::kDataFormatR16Uint]
    512, // f=2 [Gnm::kDataFormatR32Uint]
    512, // f=3 [Gnm::kDataFormatR32G32Uint]
    512, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    512, // f=5 [Gnm::kDataFormatBc1Unorm]
    512, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=1024
    1024, // f=0 [Gnm::kDataFormatR8Uint]
    1024, // f=1 [Gnm::kDataFormatR16Uint]
    1024, // f=2 [Gnm::kDataFormatR32Uint]
    1024, // f=3 [Gnm::kDataFormatR32G32Uint]
    1024, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    1024, // f=5 [Gnm::kDataFormatBc1Unorm]
    1024, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=2048
    2048, // f=0 [Gnm::kDataFormatR8Uint]
    2048, // f=1 [Gnm::kDataFormatR16Uint]
    2048, // f=2 [Gnm::kDataFormatR32Uint]
    2048, // f=3 [Gnm::kDataFormatR32G32Uint]
    2048, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    2048, // f=5 [Gnm::kDataFormatBc1Unorm]
    2048, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=4096
    4096, // f=0 [Gnm::kDataFormatR8Uint]
    4096, // f=1 [Gnm::kDataFormatR16Uint]
    4096, // f=2 [Gnm::kDataFormatR32Uint]
    4096, // f=3 [Gnm::kDataFormatR32G32Uint]
    4096, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    4096, // f=5 [Gnm::kDataFormatBc1Unorm]
    4096, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=8192
    8192, // f=0 [Gnm::kDataFormatR8Uint]
    8192, // f=1 [Gnm::kDataFormatR16Uint]
    8192, // f=2 [Gnm::kDataFormatR32Uint]
    8192, // f=3 [Gnm::kDataFormatR32G32Uint]
    8192, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    8192, // f=5 [Gnm::kDataFormatBc1Unorm]
    8192, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
  { // xy=16384
    16384, // f=0 [Gnm::kDataFormatR8Uint]
    16384, // f=1 [Gnm::kDataFormatR16Uint]
    16384, // f=2 [Gnm::kDataFormatR32Uint]
    16384, // f=3 [Gnm::kDataFormatR32G32Uint]
    16384, // f=4 [Gnm::kDataFormatR32G32B32A32Uint]
    16384, // f=5 [Gnm::kDataFormatBc1Unorm]
    16384, // f=6 [Gnm::kDataFormatBc7Unorm]
  },
};

static const uint32_t g_offsetSizes[15][7][15][2] =
{
  { // xy=1
    { // f=0 (R8)
      {0, 1}, // m=0
    },
    { // f=1 (R16)
      {0, 1}, // m=0
    },
    { // f=2 (R32)
      {0, 1}, // m=0
    },
    { // f=3 (R32G32)
      {0, 2}, // m=0
    },
    { // f=4 (R32G32B32A32)
      {0, 4}, // m=0
    },
    { // f=5 (BC1)
      {0, 2}, // m=0
    },
    { // f=6 (BC7)
      {0, 4}, // m=0
    },
  },
  { // xy=2
    { // f=0 (R8)
      {0, 1}, // m=0
      {1, 1}, // m=1
    },
    { // f=1 (R16)
      {0, 1}, // m=0
      {1, 1}, // m=1
    },
    { // f=2 (R32)
      {0, 1}, // m=0
      {1, 1}, // m=1
    },
    { // f=3 (R32G32)
      {0, 2}, // m=0
      {2, 2}, // m=1
    },
    { // f=4 (R32G32B32A32)
      {0, 4}, // m=0
      {4, 4}, // m=1
    },
    { // f=5 (BC1)
      {0, 2}, // m=0
      {2, 2}, // m=1
    },
    { // f=6 (BC7)
      {0, 4}, // m=0
      {4, 4}, // m=1
    },
  },
  { // xy=4
    { // f=0 (R8)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
    },
    { // f=1 (R16)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
    },
    { // f=2 (R32)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
    },
    { // f=3 (R32G32)
      {0, 2}, // m=0
      {2, 2}, // m=1
      {4, 2}, // m=2
    },
    { // f=4 (R32G32B32A32)
      {0, 4}, // m=0
      {4, 4}, // m=1
      {8, 4}, // m=2
    },
    { // f=5 (BC1)
      {0, 2}, // m=0
      {2, 2}, // m=1
      {4, 2}, // m=2
    },
    { // f=6 (BC7)
      {0, 4}, // m=0
      {4, 4}, // m=1
      {8, 4}, // m=2
    },
  },
  { // xy=8
    { // f=0 (R8)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
      {3, 1}, // m=3
    },
    { // f=1 (R16)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
      {3, 1}, // m=3
    },
    { // f=2 (R32)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
      {3, 1}, // m=3
    },
    { // f=3 (R32G32)
      {0, 2}, // m=0
      {2, 2}, // m=1
      {4, 2}, // m=2
      {6, 2}, // m=3
    },
    { // f=4 (R32G32B32A32)
      {0, 4}, // m=0
      {4, 4}, // m=1
      {8, 4}, // m=2
      {12, 4}, // m=3
    },
    { // f=5 (BC1)
      {0, 2}, // m=0
      {2, 2}, // m=1
      {4, 2}, // m=2
      {6, 2}, // m=3
    },
    { // f=6 (BC7)
      {0, 4}, // m=0
      {4, 4}, // m=1
      {8, 4}, // m=2
      {12, 4}, // m=3
    },
  },
  { // xy=16
    { // f=0 (R8)
      {0, 1}, // m=0
      {1, 1}, // m=1
      {2, 1}, // m=2
      {3, 1}, // m=3
      {4, 1}, // m=4
    },
    { // f=1 (R16)
      {0, 2}, // m=0
      {2, 1}, // m=1
      {3, 1}, // m=2
      {4, 1}, // m=3
      {5, 1}, // m=4
    },
    { // f=2 (R32)
      {0, 4}, // m=0
      {4, 1}, // m=1
      {5, 1}, // m=2
      {6, 1}, // m=3
      {7, 1}, // m=4
    },
    { // f=3 (R32G32)
      {0, 8}, // m=0
      {8, 2}, // m=1
      {10, 2}, // m=2
      {12, 2}, // m=3
      {14, 2}, // m=4
    },
    { // f=4 (R32G32B32A32)
      {0, 16}, // m=0
      {16, 4}, // m=1
      {20, 4}, // m=2
      {24, 4}, // m=3
      {28, 4}, // m=4
    },
    { // f=5 (BC1)
      {0, 2}, // m=0
      {2, 2}, // m=1
      {4, 2}, // m=2
      {6, 2}, // m=3
      {8, 2}, // m=4
    },
    { // f=6 (BC7)
      {0, 4}, // m=0
      {4, 4}, // m=1
      {8, 4}, // m=2
      {12, 4}, // m=3
      {16, 4}, // m=4
    },
  },
  { // xy=32
    { // f=0 (R8)
      {0, 4}, // m=0
      {4, 1}, // m=1
      {5, 1}, // m=2
      {6, 1}, // m=3
      {7, 1}, // m=4
      {8, 1}, // m=5
    },
    { // f=1 (R16)
      {0, 8}, // m=0
      {8, 2}, // m=1
      {10, 1}, // m=2
      {11, 1}, // m=3
      {12, 1}, // m=4
      {13, 1}, // m=5
    },
    { // f=2 (R32)
      {0, 16}, // m=0
      {16, 4}, // m=1
      {20, 1}, // m=2
      {21, 1}, // m=3
      {22, 1}, // m=4
      {23, 1}, // m=5
    },
    { // f=3 (R32G32)
      {0, 32}, // m=0
      {32, 8}, // m=1
      {40, 2}, // m=2
      {42, 2}, // m=3
      {44, 2}, // m=4
      {46, 2}, // m=5
    },
    { // f=4 (R32G32B32A32)
      {0, 64}, // m=0
      {64, 16}, // m=1
      {80, 4}, // m=2
      {84, 4}, // m=3
      {88, 4}, // m=4
      {92, 4}, // m=5
    },
    { // f=5 (BC1)
      {0, 2}, // m=0
      {2, 2}, // m=1
      {4, 2}, // m=2
      {6, 2}, // m=3
      {8, 2}, // m=4
      {10, 2}, // m=5
    },
    { // f=6 (BC7)
      {0, 4}, // m=0
      {4, 4}, // m=1
      {8, 4}, // m=2
      {12, 4}, // m=3
      {16, 4}, // m=4
      {20, 4}, // m=5
    },
  },
  { // xy=64
    { // f=0 (R8)
      {0, 16}, // m=0
      {16, 4}, // m=1
      {20, 1}, // m=2
      {21, 1}, // m=3
      {22, 1}, // m=4
      {23, 1}, // m=5
      {24, 1}, // m=6
    },
    { // f=1 (R16)
      {0, 32}, // m=0
      {32, 8}, // m=1
      {40, 2}, // m=2
      {42, 1}, // m=3
      {43, 1}, // m=4
      {44, 1}, // m=5
      {45, 1}, // m=6
    },
    { // f=2 (R32)
      {0, 64}, // m=0
      {64, 16}, // m=1
      {80, 4}, // m=2
      {84, 1}, // m=3
      {85, 1}, // m=4
      {86, 1}, // m=5
      {87, 1}, // m=6
    },
    { // f=3 (R32G32)
      {0, 128}, // m=0
      {128, 32}, // m=1
      {160, 8}, // m=2
      {168, 2}, // m=3
      {170, 2}, // m=4
      {172, 2}, // m=5
      {174, 2}, // m=6
    },
    { // f=4 (R32G32B32A32)
      {0, 256}, // m=0
      {256, 64}, // m=1
      {320, 16}, // m=2
      {336, 4}, // m=3
      {340, 4}, // m=4
      {344, 4}, // m=5
      {348, 4}, // m=6
    },
    { // f=5 (BC1)
      {0, 8}, // m=0
      {8, 2}, // m=1
      {10, 2}, // m=2
      {12, 2}, // m=3
      {14, 2}, // m=4
      {16, 2}, // m=5
      {18, 2}, // m=6
    },
    { // f=6 (BC7)
      {0, 16}, // m=0
      {16, 4}, // m=1
      {20, 4}, // m=2
      {24, 4}, // m=3
      {28, 4}, // m=4
      {32, 4}, // m=5
      {36, 4}, // m=6
    },
  },
  { // xy=128
    { // f=0 (R8)
      {0, 64}, // m=0
      {64, 16}, // m=1
      {80, 4}, // m=2
      {84, 1}, // m=3
      {85, 1}, // m=4
      {86, 1}, // m=5
      {87, 1}, // m=6
      {88, 1}, // m=7
    },
    { // f=1 (R16)
      {0, 128}, // m=0
      {128, 32}, // m=1
      {160, 8}, // m=2
      {168, 2}, // m=3
      {170, 1}, // m=4
      {171, 1}, // m=5
      {172, 1}, // m=6
      {173, 1}, // m=7
    },
    { // f=2 (R32)
      {0, 256}, // m=0
      {256, 64}, // m=1
      {320, 16}, // m=2
      {336, 4}, // m=3
      {340, 1}, // m=4
      {341, 1}, // m=5
      {342, 1}, // m=6
      {343, 1}, // m=7
    },
    { // f=3 (R32G32)
      {0, 512}, // m=0
      {512, 128}, // m=1
      {640, 32}, // m=2
      {672, 8}, // m=3
      {680, 2}, // m=4
      {682, 2}, // m=5
      {684, 2}, // m=6
      {686, 2}, // m=7
    },
    { // f=4 (R32G32B32A32)
      {0, 1024}, // m=0
      {1024, 256}, // m=1
      {1280, 64}, // m=2
      {1344, 16}, // m=3
      {1360, 4}, // m=4
      {1364, 4}, // m=5
      {1368, 4}, // m=6
      {1372, 4}, // m=7
    },
    { // f=5 (BC1)
      {0, 32}, // m=0
      {32, 8}, // m=1
      {40, 2}, // m=2
      {42, 2}, // m=3
      {44, 2}, // m=4
      {46, 2}, // m=5
      {48, 2}, // m=6
      {50, 2}, // m=7
    },
    { // f=6 (BC7)
      {0, 64}, // m=0
      {64, 16}, // m=1
      {80, 4}, // m=2
      {84, 4}, // m=3
      {88, 4}, // m=4
      {92, 4}, // m=5
      {96, 4}, // m=6
      {100, 4}, // m=7
    },
  },
  { // xy=256
    { // f=0 (R8)
      {0, 256}, // m=0
      {256, 64}, // m=1
      {320, 16}, // m=2
      {336, 4}, // m=3
      {340, 1}, // m=4
      {341, 1}, // m=5
      {342, 1}, // m=6
      {343, 1}, // m=7
      {344, 1}, // m=8
    },
    { // f=1 (R16)
      {0, 512}, // m=0
      {512, 128}, // m=1
      {640, 32}, // m=2
      {672, 8}, // m=3
      {680, 2}, // m=4
      {682, 1}, // m=5
      {683, 1}, // m=6
      {684, 1}, // m=7
      {685, 1}, // m=8
    },
    { // f=2 (R32)
      {0, 1024}, // m=0
      {1024, 256}, // m=1
      {1280, 64}, // m=2
      {1344, 16}, // m=3
      {1360, 4}, // m=4
      {1364, 1}, // m=5
      {1365, 1}, // m=6
      {1366, 1}, // m=7
      {1367, 1}, // m=8
    },
    { // f=3 (R32G32)
      {0, 2048}, // m=0
      {2048, 512}, // m=1
      {2560, 128}, // m=2
      {2688, 32}, // m=3
      {2720, 8}, // m=4
      {2728, 2}, // m=5
      {2730, 2}, // m=6
      {2732, 2}, // m=7
      {2734, 2}, // m=8
    },
    { // f=4 (R32G32B32A32)
      {0, 4096}, // m=0
      {4096, 1024}, // m=1
      {5120, 256}, // m=2
      {5376, 64}, // m=3
      {5440, 16}, // m=4
      {5456, 4}, // m=5
      {5460, 4}, // m=6
      {5464, 4}, // m=7
      {5468, 4}, // m=8
    },
    { // f=5 (BC1)
      {0, 128}, // m=0
      {128, 32}, // m=1
      {160, 8}, // m=2
      {168, 2}, // m=3
      {170, 2}, // m=4
      {172, 2}, // m=5
      {174, 2}, // m=6
      {176, 2}, // m=7
      {178, 2}, // m=8
    },
    { // f=6 (BC7)
      {0, 256}, // m=0
      {256, 64}, // m=1
      {320, 16}, // m=2
      {336, 4}, // m=3
      {340, 4}, // m=4
      {344, 4}, // m=5
      {348, 4}, // m=6
      {352, 4}, // m=7
      {356, 4}, // m=8
    },
  },
  { // xy=512
    { // f=0 (R8)
      {0, 1024}, // m=0
      {1024, 256}, // m=1
      {1280, 64}, // m=2
      {1344, 16}, // m=3
      {1360, 4}, // m=4
      {1364, 1}, // m=5
      {1365, 1}, // m=6
      {1366, 1}, // m=7
      {1367, 1}, // m=8
      {1368, 1}, // m=9
    },
    { // f=1 (R16)
      {0, 2048}, // m=0
      {2048, 512}, // m=1
      {2560, 128}, // m=2
      {2688, 32}, // m=3
      {2720, 8}, // m=4
      {2728, 2}, // m=5
      {2730, 1}, // m=6
      {2731, 1}, // m=7
      {2732, 1}, // m=8
      {2733, 1}, // m=9
    },
    { // f=2 (R32)
      {0, 4096}, // m=0
      {4096, 1024}, // m=1
      {5120, 256}, // m=2
      {5376, 64}, // m=3
      {5440, 16}, // m=4
      {5456, 4}, // m=5
      {5460, 1}, // m=6
      {5461, 1}, // m=7
      {5462, 1}, // m=8
      {5463, 1}, // m=9
    },
    { // f=3 (R32G32)
      {0, 8192}, // m=0
      {8192, 2048}, // m=1
      {10240, 512}, // m=2
      {10752, 128}, // m=3
      {10880, 32}, // m=4
      {10912, 8}, // m=5
      {10920, 2}, // m=6
      {10922, 2}, // m=7
      {10924, 2}, // m=8
      {10926, 2}, // m=9
    },
    { // f=4 (R32G32B32A32)
      {0, 16384}, // m=0
      {16384, 4096}, // m=1
      {20480, 1024}, // m=2
      {21504, 256}, // m=3
      {21760, 64}, // m=4
      {21824, 16}, // m=5
      {21840, 4}, // m=6
      {21844, 4}, // m=7
      {21848, 4}, // m=8
      {21852, 4}, // m=9
    },
    { // f=5 (BC1)
      {0, 512}, // m=0
      {512, 128}, // m=1
      {640, 32}, // m=2
      {672, 8}, // m=3
      {680, 2}, // m=4
      {682, 2}, // m=5
      {684, 2}, // m=6
      {686, 2}, // m=7
      {688, 2}, // m=8
      {690, 2}, // m=9
    },
    { // f=6 (BC7)
      {0, 1024}, // m=0
      {1024, 256}, // m=1
      {1280, 64}, // m=2
      {1344, 16}, // m=3
      {1360, 4}, // m=4
      {1364, 4}, // m=5
      {1368, 4}, // m=6
      {1372, 4}, // m=7
      {1376, 4}, // m=8
      {1380, 4}, // m=9
    },
  },
  { // xy=1024
    { // f=0 (R8)
      {0, 4096}, // m=0
      {4096, 1024}, // m=1
      {5120, 256}, // m=2
      {5376, 64}, // m=3
      {5440, 16}, // m=4
      {5456, 4}, // m=5
      {5460, 1}, // m=6
      {5461, 1}, // m=7
      {5462, 1}, // m=8
      {5463, 1}, // m=9
      {5464, 1}, // m=10
    },
    { // f=1 (R16)
      {0, 8192}, // m=0
      {8192, 2048}, // m=1
      {10240, 512}, // m=2
      {10752, 128}, // m=3
      {10880, 32}, // m=4
      {10912, 8}, // m=5
      {10920, 2}, // m=6
      {10922, 1}, // m=7
      {10923, 1}, // m=8
      {10924, 1}, // m=9
      {10925, 1}, // m=10
    },
    { // f=2 (R32)
      {0, 16384}, // m=0
      {16384, 4096}, // m=1
      {20480, 1024}, // m=2
      {21504, 256}, // m=3
      {21760, 64}, // m=4
      {21824, 16}, // m=5
      {21840, 4}, // m=6
      {21844, 1}, // m=7
      {21845, 1}, // m=8
      {21846, 1}, // m=9
      {21847, 1}, // m=10
    },
    { // f=3 (R32G32)
      {0, 32768}, // m=0
      {32768, 8192}, // m=1
      {40960, 2048}, // m=2
      {43008, 512}, // m=3
      {43520, 128}, // m=4
      {43648, 32}, // m=5
      {43680, 8}, // m=6
      {43688, 2}, // m=7
      {43690, 2}, // m=8
      {43692, 2}, // m=9
      {43694, 2}, // m=10
    },
    { // f=4 (R32G32B32A32)
      {0, 65536}, // m=0
      {65536, 16384}, // m=1
      {81920, 4096}, // m=2
      {86016, 1024}, // m=3
      {87040, 256}, // m=4
      {87296, 64}, // m=5
      {87360, 16}, // m=6
      {87376, 4}, // m=7
      {87380, 4}, // m=8
      {87384, 4}, // m=9
      {87388, 4}, // m=10
    },
    { // f=5 (BC1)
      {0, 2048}, // m=0
      {2048, 512}, // m=1
      {2560, 128}, // m=2
      {2688, 32}, // m=3
      {2720, 8}, // m=4
      {2728, 2}, // m=5
      {2730, 2}, // m=6
      {2732, 2}, // m=7
      {2734, 2}, // m=8
      {2736, 2}, // m=9
      {2738, 2}, // m=10
    },
    { // f=6 (BC7)
      {0, 4096}, // m=0
      {4096, 1024}, // m=1
      {5120, 256}, // m=2
      {5376, 64}, // m=3
      {5440, 16}, // m=4
      {5456, 4}, // m=5
      {5460, 4}, // m=6
      {5464, 4}, // m=7
      {5468, 4}, // m=8
      {5472, 4}, // m=9
      {5476, 4}, // m=10
    },
  },
  { // xy=2048
    { // f=0 (R8)
      {0, 16384}, // m=0
      {16384, 4096}, // m=1
      {20480, 1024}, // m=2
      {21504, 256}, // m=3
      {21760, 64}, // m=4
      {21824, 16}, // m=5
      {21840, 4}, // m=6
      {21844, 1}, // m=7
      {21845, 1}, // m=8
      {21846, 1}, // m=9
      {21847, 1}, // m=10
      {21848, 1}, // m=11
    },
    { // f=1 (R16)
      {0, 32768}, // m=0
      {32768, 8192}, // m=1
      {40960, 2048}, // m=2
      {43008, 512}, // m=3
      {43520, 128}, // m=4
      {43648, 32}, // m=5
      {43680, 8}, // m=6
      {43688, 2}, // m=7
      {43690, 1}, // m=8
      {43691, 1}, // m=9
      {43692, 1}, // m=10
      {43693, 1}, // m=11
    },
    { // f=2 (R32)
      {0, 65536}, // m=0
      {65536, 16384}, // m=1
      {81920, 4096}, // m=2
      {86016, 1024}, // m=3
      {87040, 256}, // m=4
      {87296, 64}, // m=5
      {87360, 16}, // m=6
      {87376, 4}, // m=7
      {87380, 1}, // m=8
      {87381, 1}, // m=9
      {87382, 1}, // m=10
      {87383, 1}, // m=11
    },
    { // f=3 (R32G32)
      {0, 131072}, // m=0
      {131072, 32768}, // m=1
      {163840, 8192}, // m=2
      {172032, 2048}, // m=3
      {174080, 512}, // m=4
      {174592, 128}, // m=5
      {174720, 32}, // m=6
      {174752, 8}, // m=7
      {174760, 2}, // m=8
      {174762, 2}, // m=9
      {174764, 2}, // m=10
      {174766, 2}, // m=11
    },
    { // f=4 (R32G32B32A32)
      {0, 262144}, // m=0
      {262144, 65536}, // m=1
      {327680, 16384}, // m=2
      {344064, 4096}, // m=3
      {348160, 1024}, // m=4
      {349184, 256}, // m=5
      {349440, 64}, // m=6
      {349504, 16}, // m=7
      {349520, 4}, // m=8
      {349524, 4}, // m=9
      {349528, 4}, // m=10
      {349532, 4}, // m=11
    },
    { // f=5 (BC1)
      {0, 8192}, // m=0
      {8192, 2048}, // m=1
      {10240, 512}, // m=2
      {10752, 128}, // m=3
      {10880, 32}, // m=4
      {10912, 8}, // m=5
      {10920, 2}, // m=6
      {10922, 2}, // m=7
      {10924, 2}, // m=8
      {10926, 2}, // m=9
      {10928, 2}, // m=10
      {10930, 2}, // m=11
    },
    { // f=6 (BC7)
      {0, 16384}, // m=0
      {16384, 4096}, // m=1
      {20480, 1024}, // m=2
      {21504, 256}, // m=3
      {21760, 64}, // m=4
      {21824, 16}, // m=5
      {21840, 4}, // m=6
      {21844, 4}, // m=7
      {21848, 4}, // m=8
      {21852, 4}, // m=9
      {21856, 4}, // m=10
      {21860, 4}, // m=11
    },
  },
  { // xy=4096
    { // f=0 (R8)
      {0, 65536}, // m=0
      {65536, 16384}, // m=1
      {81920, 4096}, // m=2
      {86016, 1024}, // m=3
      {87040, 256}, // m=4
      {87296, 64}, // m=5
      {87360, 16}, // m=6
      {87376, 4}, // m=7
      {87380, 1}, // m=8
      {87381, 1}, // m=9
      {87382, 1}, // m=10
      {87383, 1}, // m=11
      {87384, 1}, // m=12
    },
    { // f=1 (R16)
      {0, 131072}, // m=0
      {131072, 32768}, // m=1
      {163840, 8192}, // m=2
      {172032, 2048}, // m=3
      {174080, 512}, // m=4
      {174592, 128}, // m=5
      {174720, 32}, // m=6
      {174752, 8}, // m=7
      {174760, 2}, // m=8
      {174762, 1}, // m=9
      {174763, 1}, // m=10
      {174764, 1}, // m=11
      {174765, 1}, // m=12
    },
    { // f=2 (R32)
      {0, 262144}, // m=0
      {262144, 65536}, // m=1
      {327680, 16384}, // m=2
      {344064, 4096}, // m=3
      {348160, 1024}, // m=4
      {349184, 256}, // m=5
      {349440, 64}, // m=6
      {349504, 16}, // m=7
      {349520, 4}, // m=8
      {349524, 1}, // m=9
      {349525, 1}, // m=10
      {349526, 1}, // m=11
      {349527, 1}, // m=12
    },
    { // f=3 (R32G32)
      {0, 524288}, // m=0
      {524288, 131072}, // m=1
      {655360, 32768}, // m=2
      {688128, 8192}, // m=3
      {696320, 2048}, // m=4
      {698368, 512}, // m=5
      {698880, 128}, // m=6
      {699008, 32}, // m=7
      {699040, 8}, // m=8
      {699048, 2}, // m=9
      {699050, 2}, // m=10
      {699052, 2}, // m=11
      {699054, 2}, // m=12
    },
    { // f=4 (R32G32B32A32)
      {0, 1048576}, // m=0
      {1048576, 262144}, // m=1
      {1310720, 65536}, // m=2
      {1376256, 16384}, // m=3
      {1392640, 4096}, // m=4
      {1396736, 1024}, // m=5
      {1397760, 256}, // m=6
      {1398016, 64}, // m=7
      {1398080, 16}, // m=8
      {1398096, 4}, // m=9
      {1398100, 4}, // m=10
      {1398104, 4}, // m=11
      {1398108, 4}, // m=12
    },
    { // f=5 (BC1)
      {0, 32768}, // m=0
      {32768, 8192}, // m=1
      {40960, 2048}, // m=2
      {43008, 512}, // m=3
      {43520, 128}, // m=4
      {43648, 32}, // m=5
      {43680, 8}, // m=6
      {43688, 2}, // m=7
      {43690, 2}, // m=8
      {43692, 2}, // m=9
      {43694, 2}, // m=10
      {43696, 2}, // m=11
      {43698, 2}, // m=12
    },
    { // f=6 (BC7)
      {0, 65536}, // m=0
      {65536, 16384}, // m=1
      {81920, 4096}, // m=2
      {86016, 1024}, // m=3
      {87040, 256}, // m=4
      {87296, 64}, // m=5
      {87360, 16}, // m=6
      {87376, 4}, // m=7
      {87380, 4}, // m=8
      {87384, 4}, // m=9
      {87388, 4}, // m=10
      {87392, 4}, // m=11
      {87396, 4}, // m=12
    },
  },
  { // xy=8192
    { // f=0 (R8)
      {0, 262144}, // m=0
      {262144, 65536}, // m=1
      {327680, 16384}, // m=2
      {344064, 4096}, // m=3
      {348160, 1024}, // m=4
      {349184, 256}, // m=5
      {349440, 64}, // m=6
      {349504, 16}, // m=7
      {349520, 4}, // m=8
      {349524, 1}, // m=9
      {349525, 1}, // m=10
      {349526, 1}, // m=11
      {349527, 1}, // m=12
      {349528, 1}, // m=13
    },
    { // f=1 (R16)
      {0, 524288}, // m=0
      {524288, 131072}, // m=1
      {655360, 32768}, // m=2
      {688128, 8192}, // m=3
      {696320, 2048}, // m=4
      {698368, 512}, // m=5
      {698880, 128}, // m=6
      {699008, 32}, // m=7
      {699040, 8}, // m=8
      {699048, 2}, // m=9
      {699050, 1}, // m=10
      {699051, 1}, // m=11
      {699052, 1}, // m=12
      {699053, 1}, // m=13
    },
    { // f=2 (R32)
      {0, 1048576}, // m=0
      {1048576, 262144}, // m=1
      {1310720, 65536}, // m=2
      {1376256, 16384}, // m=3
      {1392640, 4096}, // m=4
      {1396736, 1024}, // m=5
      {1397760, 256}, // m=6
      {1398016, 64}, // m=7
      {1398080, 16}, // m=8
      {1398096, 4}, // m=9
      {1398100, 1}, // m=10
      {1398101, 1}, // m=11
      {1398102, 1}, // m=12
      {1398103, 1}, // m=13
    },
    { // f=3 (R32G32)
      {0, 2097152}, // m=0
      {2097152, 524288}, // m=1
      {2621440, 131072}, // m=2
      {2752512, 32768}, // m=3
      {2785280, 8192}, // m=4
      {2793472, 2048}, // m=5
      {2795520, 512}, // m=6
      {2796032, 128}, // m=7
      {2796160, 32}, // m=8
      {2796192, 8}, // m=9
      {2796200, 2}, // m=10
      {2796202, 2}, // m=11
      {2796204, 2}, // m=12
      {2796206, 2}, // m=13
    },
    { // f=4 (R32G32B32A32)
      {0, 4194304}, // m=0
      {4194304, 1048576}, // m=1
      {5242880, 262144}, // m=2
      {5505024, 65536}, // m=3
      {5570560, 16384}, // m=4
      {5586944, 4096}, // m=5
      {5591040, 1024}, // m=6
      {5592064, 256}, // m=7
      {5592320, 64}, // m=8
      {5592384, 16}, // m=9
      {5592400, 4}, // m=10
      {5592404, 4}, // m=11
      {5592408, 4}, // m=12
      {5592412, 4}, // m=13
    },
    { // f=5 (BC1)
      {0, 131072}, // m=0
      {131072, 32768}, // m=1
      {163840, 8192}, // m=2
      {172032, 2048}, // m=3
      {174080, 512}, // m=4
      {174592, 128}, // m=5
      {174720, 32}, // m=6
      {174752, 8}, // m=7
      {174760, 2}, // m=8
      {174762, 2}, // m=9
      {174764, 2}, // m=10
      {174766, 2}, // m=11
      {174768, 2}, // m=12
      {174770, 2}, // m=13
    },
    { // f=6 (BC7)
      {0, 262144}, // m=0
      {262144, 65536}, // m=1
      {327680, 16384}, // m=2
      {344064, 4096}, // m=3
      {348160, 1024}, // m=4
      {349184, 256}, // m=5
      {349440, 64}, // m=6
      {349504, 16}, // m=7
      {349520, 4}, // m=8
      {349524, 4}, // m=9
      {349528, 4}, // m=10
      {349532, 4}, // m=11
      {349536, 4}, // m=12
      {349540, 4}, // m=13
    },
  },
  { // xy=16384
    { // f=0 (R8)
      {0, 1048576}, // m=0
      {1048576, 262144}, // m=1
      {1310720, 65536}, // m=2
      {1376256, 16384}, // m=3
      {1392640, 4096}, // m=4
      {1396736, 1024}, // m=5
      {1397760, 256}, // m=6
      {1398016, 64}, // m=7
      {1398080, 16}, // m=8
      {1398096, 4}, // m=9
      {1398100, 1}, // m=10
      {1398101, 1}, // m=11
      {1398102, 1}, // m=12
      {1398103, 1}, // m=13
      {1398104, 1}, // m=14
    },
    { // f=1 (R16)
      {0, 2097152}, // m=0
      {2097152, 524288}, // m=1
      {2621440, 131072}, // m=2
      {2752512, 32768}, // m=3
      {2785280, 8192}, // m=4
      {2793472, 2048}, // m=5
      {2795520, 512}, // m=6
      {2796032, 128}, // m=7
      {2796160, 32}, // m=8
      {2796192, 8}, // m=9
      {2796200, 2}, // m=10
      {2796202, 1}, // m=11
      {2796203, 1}, // m=12
      {2796204, 1}, // m=13
      {2796205, 1}, // m=14
    },
    { // f=2 (R32)
      {0, 4194304}, // m=0
      {4194304, 1048576}, // m=1
      {5242880, 262144}, // m=2
      {5505024, 65536}, // m=3
      {5570560, 16384}, // m=4
      {5586944, 4096}, // m=5
      {5591040, 1024}, // m=6
      {5592064, 256}, // m=7
      {5592320, 64}, // m=8
      {5592384, 16}, // m=9
      {5592400, 4}, // m=10
      {5592404, 1}, // m=11
      {5592405, 1}, // m=12
      {5592406, 1}, // m=13
      {5592407, 1}, // m=14
    },
    { // f=3 (R32G32)
      {0, 8388608}, // m=0
      {8388608, 2097152}, // m=1
      {10485760, 524288}, // m=2
      {11010048, 131072}, // m=3
      {11141120, 32768}, // m=4
      {11173888, 8192}, // m=5
      {11182080, 2048}, // m=6
      {11184128, 512}, // m=7
      {11184640, 128}, // m=8
      {11184768, 32}, // m=9
      {11184800, 8}, // m=10
      {11184808, 2}, // m=11
      {11184810, 2}, // m=12
      {11184812, 2}, // m=13
      {11184814, 2}, // m=14
    },
    { // f=4 (R32G32B32A32)
      {0, 16777216}, // m=0
      {16777216, 4194304}, // m=1
      {20971520, 1048576}, // m=2
      {22020096, 262144}, // m=3
      {22282240, 65536}, // m=4
      {22347776, 16384}, // m=5
      {22364160, 4096}, // m=6
      {22368256, 1024}, // m=7
      {22369280, 256}, // m=8
      {22369536, 64}, // m=9
      {22369600, 16}, // m=10
      {22369616, 4}, // m=11
      {22369620, 4}, // m=12
      {22369624, 4}, // m=13
      {22369628, 4}, // m=14
    },
    { // f=5 (BC1)
      {0, 524288}, // m=0
      {524288, 131072}, // m=1
      {655360, 32768}, // m=2
      {688128, 8192}, // m=3
      {696320, 2048}, // m=4
      {698368, 512}, // m=5
      {698880, 128}, // m=6
      {699008, 32}, // m=7
      {699040, 8}, // m=8
      {699048, 2}, // m=9
      {699050, 2}, // m=10
      {699052, 2}, // m=11
      {699054, 2}, // m=12
      {699056, 2}, // m=13
      {699058, 2}, // m=14
    },
    { // f=6 (BC7)
      {0, 1048576}, // m=0
      {1048576, 262144}, // m=1
      {1310720, 65536}, // m=2
      {1376256, 16384}, // m=3
      {1392640, 4096}, // m=4
      {1396736, 1024}, // m=5
      {1397760, 256}, // m=6
      {1398016, 64}, // m=7
      {1398080, 16}, // m=8
      {1398096, 4}, // m=9
      {1398100, 4}, // m=10
      {1398104, 4}, // m=11
      {1398108, 4}, // m=12
      {1398112, 4}, // m=13
      {1398116, 4}, // m=14
    },
  },
};

int32_t sce::GpuAddress::computeTotalTiledTextureSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const Gnm::Texture *texture)
{
	SCE_GNM_ASSERT_MSG_RETURN(outSize != NULL, kStatusInvalidArgument, "outSize must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(outAlign != NULL, kStatusInvalidArgument, "outAlign must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(texture != 0, kStatusInvalidArgument, "texture must not be NULL.");

	const auto isCubemap = (texture->getTextureType() == Gnm::kTextureTypeCubemap);
	const auto isVolume = (texture->getTextureType() == Gnm::kTextureType3d);
	auto arraySliceCount = texture->getTotalArraySliceCount();
	if(isCubemap)
		arraySliceCount *= 6; // cube maps store six faces per array slice
	else if(isVolume)
		arraySliceCount = 1; // volume textures can't be arrays
	if (texture->isPaddedToPow2())
		arraySliceCount = nextPowerOfTwo((uint32_t)arraySliceCount); // array slice counts are padded to a power of two

    switch(texture->getTextureType())
    {
        case Gnm::kTextureType2d:
        case Gnm::kTextureType2dArray:
        case Gnm::kTextureTypeCubemap:
            if( (texture->getTileMode() == Gnm::kTileModeThin_1dThin) 
             && isPowerOfTwo(texture->getWidth()) 
             && (texture->getWidth() == texture->getHeight()) 
            )
            {
                const auto xy = fastIntLog2(texture->getWidth());
                const auto f = fastIntLog2(texture->getDataFormat().getTotalBytesPerElement()) + (texture->getDataFormat().isBlockCompressedFormat() ? 2 : 0);
                if(g_pitches[xy][f] == texture->getPitch())
                {
                    const auto mipLevel = texture->getLastMipLevel();
                    const auto offset = uint64_t(g_offsetSizes[xy][f][mipLevel][0]) << 8;
                    const auto size   = uint64_t(g_offsetSizes[xy][f][mipLevel][1]) << 8;
                    *outSize  = (offset + size) * arraySliceCount;
                    *outAlign = 256;
                    return kStatusSuccess;
                }
            }
            break;
        default:
            break;
    }

	*outSize = 0;
	*outAlign = (Gnm::AlignmentType)1;
	TilingParameters tpBase;
	tpBase.initFromTexture(texture, 0, 0);
	TilingParameters tpCopy; // so we can modify the mipLevel field inside the loop
	memcpy(&tpCopy, &tpBase, sizeof(tpCopy));
	uint64_t mipLevelCount = texture->getLastMipLevel()+1;
	for(uint32_t iMip=0; iMip<mipLevelCount; ++iMip)
	{
		tpCopy.m_linearWidth  = std::max(tpBase.m_linearWidth  >> iMip, 1U);
		tpCopy.m_linearHeight = std::max(tpBase.m_linearHeight >> iMip, 1U);
		tpCopy.m_linearDepth  = std::max(tpBase.m_linearDepth  >> iMip, 1U);
		tpCopy.m_baseTiledPitch = texture->getPitch();
		tpCopy.m_mipLevel = iMip;
		uint64_t mipSize;
		Gnm::AlignmentType mipAlign;
		int32_t mipStatus = computeTiledSurfaceSize(&mipSize, &mipAlign, &tpCopy);
		if (mipStatus != kStatusSuccess)
			return mipStatus;
		*outSize += arraySliceCount*mipSize; // TODO: special-case for cubemaps, where mips >0 have numSlices padded?
		*outAlign = std::max(*outAlign, mipAlign);
		if (tpCopy.m_linearWidth == 1 && tpCopy.m_linearHeight == 1 && tpCopy.m_linearDepth == 1)
			break; // early-out if we've hit the smallest mip
	}

	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeTotalUntiledTextureSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const Gnm::Texture *texture)
{
	SCE_GNM_ASSERT_MSG_RETURN(outSize != NULL, kStatusInvalidArgument, "outSize must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(outAlign != NULL, kStatusInvalidArgument, "outAlign must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(texture != 0, kStatusInvalidArgument, "texture must not be NULL.");
	TilingParameters tpBase;
	tpBase.initFromTexture(texture, 0, 0);
	*outSize = 0;
	*outAlign = (Gnm::AlignmentType)1;
	TilingParameters tpCopy; // so we can modify the mipLevel field inside the loop
	memcpy(&tpCopy, &tpBase, sizeof(tpCopy));
	uint64_t arraySliceCount = texture->getTotalArraySliceCount();
	if (texture->getTextureType() == Gnm::kTextureTypeCubemap)
		arraySliceCount *= 6; // cube maps store six faces per array slice
	else if (texture->getTextureType() == Gnm::kTextureType3d)
		arraySliceCount = 1; // volume textures can't be arrays
	uint64_t mipLevelCount = texture->getLastMipLevel()+1;
	for(uint32_t iMip=0; iMip<mipLevelCount; ++iMip)
	{
		tpCopy.m_linearWidth  = std::max(tpBase.m_linearWidth  >> iMip, 1U);
		tpCopy.m_linearHeight = std::max(tpBase.m_linearHeight >> iMip, 1U);
		tpCopy.m_linearDepth  = std::max(tpBase.m_linearDepth  >> iMip, 1U);
		tpCopy.m_baseTiledPitch = texture->getPitch();
		tpCopy.m_mipLevel = iMip;
		uint64_t mipSize;
		Gnm::AlignmentType mipAlign;
		int32_t mipStatus = computeUntiledSurfaceSize(&mipSize, &mipAlign, &tpCopy);
		if (mipStatus != kStatusSuccess)
			return mipStatus;
		*outSize += arraySliceCount*mipSize; // TODO: special-case for cubemaps, where mips >0 have numSlices padded?
		*outAlign = std::max(*outAlign, mipAlign);
		if (tpCopy.m_linearWidth == 1 && tpCopy.m_linearHeight == 1 && tpCopy.m_linearDepth == 1)
			break; // early-out if we've hit the smallest mip
	}
	return kStatusSuccess;
}

static uint32_t getMaximumMipLevelCount(const Gnm::Texture *texture)
{
	switch(texture->getTextureType())
	{
	case Gnm::kTextureType2dMsaa:
	case Gnm::kTextureType2dArrayMsaa:
		return 1; // MSAA textures can't have mips
	case Gnm::kTextureType3d:
		return 1+::fastIntLog2(std::max(std::max(texture->getWidth(), texture->getHeight()), texture->getDepth()));
	default:
		return 1+::fastIntLog2(         std::max(texture->getWidth(), texture->getHeight())                      );
	}
}

int32_t sce::GpuAddress::computeTextureSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::Texture *texture, uint32_t mipLevel, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(texture != 0, kStatusInvalidArgument, "texture must not be NULL.");
	uint32_t maxMipLevel = getMaximumMipLevelCount(texture)-1;
    mipLevel = std::min(mipLevel, maxMipLevel); // The hardware does not issue an error when mipLevel is too high; it simply clamps to the highest logically possible mipLevel.

	const auto isCubemap = (texture->getTextureType() == Gnm::kTextureTypeCubemap);
	const auto isVolume = (texture->getTextureType() == Gnm::kTextureType3d);
	auto arraySliceCount = texture->getTotalArraySliceCount();
	if (isCubemap)
		arraySliceCount *= 6; // cube maps store six faces per array slice
	else if (isVolume)
		arraySliceCount = 1; // volume textures can't be arrays
	if (texture->isPaddedToPow2())
		arraySliceCount = nextPowerOfTwo(arraySliceCount); // array slice counts are padded to powers of two as well

	SCE_GNM_ASSERT_MSG_RETURN(arraySlice < arraySliceCount, kStatusInvalidArgument, "arraySlice (%u) is out of range for texture (0x%p) with %u slices.", arraySlice, texture, arraySliceCount);

    switch(texture->getTextureType())
    {
        case Gnm::kTextureType2d:
        case Gnm::kTextureType2dArray:
        case Gnm::kTextureTypeCubemap:
            if( (texture->getTileMode() == Gnm::kTileModeThin_1dThin) 
             && isPowerOfTwo(texture->getWidth()) 
             && (texture->getWidth() == texture->getHeight()) 
            )
            {
                const auto xy = fastIntLog2(texture->getWidth());
                const auto f = fastIntLog2(texture->getDataFormat().getTotalBytesPerElement()) + (texture->getDataFormat().isBlockCompressedFormat() ? 2 : 0);
                if(g_pitches[xy][f] == texture->getPitch())
                {
                    const auto offset = uint64_t(g_offsetSizes[xy][f][mipLevel][0]) << 8;
                    const auto size   = uint64_t(g_offsetSizes[xy][f][mipLevel][1]) << 8;
                    if(nullptr != outSurfaceOffset)
                        *outSurfaceOffset = offset * arraySliceCount + size * arraySlice;
                    if(nullptr != outSurfaceSize)
                        *outSurfaceSize   = size;
                    return kStatusSuccess;
                }
            }
            break;
        default:
            break;
    }

	const uint32_t baseWidth = texture->getWidth();
	const uint32_t baseHeight = texture->getHeight();
	const uint32_t baseDepth = isVolume ? texture->getDepth() : 1;
	TilingParameters tp;
	tp.initFromTexture(texture, 0, arraySlice);
	SurfaceInfo surfInfoOut = {0};

	uint64_t finalSurfaceOffset = 0;
	uint64_t finalSurfaceSize = 0;
	for(uint32_t iMip=0; iMip<=mipLevel; ++iMip)
	{
		finalSurfaceOffset += (uint64_t)arraySliceCount * finalSurfaceSize; // Add all faces from the previous slices/mips
		tp.m_linearWidth  = std::max((baseWidth  >> iMip), 1U);
		tp.m_linearHeight = std::max((baseHeight >> iMip), 1U);
		tp.m_linearDepth  = std::max((baseDepth  >> iMip), 1U);
		tp.m_baseTiledPitch = texture->getPitch();
		tp.m_mipLevel = iMip;
		computeSurfaceInfo(&surfInfoOut, &tp);
		finalSurfaceSize = surfInfoOut.m_surfaceSize;
	}
	finalSurfaceOffset += surfInfoOut.m_surfaceSize*(uint64_t)arraySlice;

	if (outSurfaceOffset != NULL)
		*outSurfaceOffset = finalSurfaceOffset;
	if (outSurfaceSize != NULL)
		*outSurfaceSize = finalSurfaceSize;

	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeRenderTargetSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::RenderTarget *target, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(target != 0, kStatusInvalidArgument, "target must not be NULL.");
	TilingParameters tp;
	tp.initFromRenderTarget(target, arraySlice);
	SurfaceInfo surfInfoOut = {0};

	uint64_t finalSurfaceSize = 0;
	uint64_t finalSurfaceOffset = 0;
	uint32_t arraySliceCount = target->getLastArraySliceIndex() + 1;
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice < arraySliceCount, kStatusInvalidArgument, "arraySlice (%u) is out of range for RenderTarget (0x%p) with %u slices.", arraySlice, target, arraySliceCount);
	computeSurfaceInfo(&surfInfoOut, &tp);
	finalSurfaceSize = surfInfoOut.m_surfaceSize;
	finalSurfaceOffset += surfInfoOut.m_surfaceSize*(uint64_t)arraySlice;

	if (outSurfaceOffset != NULL)
		*outSurfaceOffset = finalSurfaceOffset;
	if (outSurfaceSize != NULL)
		*outSurfaceSize = finalSurfaceSize;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeDepthSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::DepthRenderTarget *depthTarget, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(depthTarget != 0, kStatusInvalidArgument, "depthTarget must not be NULL.");
	TilingParameters tp;
	tp.initFromDepthSurface(depthTarget, arraySlice);
	SurfaceInfo surfInfoOut = {0};

	uint64_t finalSurfaceSize = 0;
	uint64_t finalSurfaceOffset = 0;
	uint32_t arraySliceCount = depthTarget->getLastArraySliceIndex() + 1;
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice < arraySliceCount, kStatusInvalidArgument, "arraySlice (%u) is out of range for DepthRenderTarget (0x%p) with %u slices.", arraySlice, depthTarget, arraySliceCount);
	computeSurfaceInfo(&surfInfoOut, &tp);
	finalSurfaceSize = surfInfoOut.m_surfaceSize;
	finalSurfaceOffset += surfInfoOut.m_surfaceSize*(uint64_t)arraySlice;

	if (outSurfaceOffset != NULL)
		*outSurfaceOffset = finalSurfaceOffset;
	if (outSurfaceSize != NULL)
		*outSurfaceSize = finalSurfaceSize;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeStencilSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::DepthRenderTarget *depthTarget, uint32_t arraySlice)
{
	SCE_GNM_ASSERT_MSG_RETURN(depthTarget != 0, kStatusInvalidArgument, "depthTarget must not be NULL.");
	TilingParameters tp;
	tp.initFromStencilSurface(depthTarget, arraySlice);
	SurfaceInfo surfInfoOut = {0};

	uint64_t finalSurfaceSize = 0;
	uint64_t finalSurfaceOffset = 0;
	uint32_t arraySliceCount = depthTarget->getLastArraySliceIndex() + 1;
	SCE_GNM_ASSERT_MSG_RETURN(arraySlice < arraySliceCount, kStatusInvalidArgument, "arraySlice (%u) is out of range for DepthRenderTarget (0x%p) with %u slices.", arraySlice, depthTarget, arraySliceCount);
	computeSurfaceInfo(&surfInfoOut, &tp);
	finalSurfaceSize = surfInfoOut.m_surfaceSize;
	finalSurfaceOffset += surfInfoOut.m_surfaceSize*(uint64_t)arraySlice;

	if (outSurfaceOffset != NULL)
		*outSurfaceOffset = finalSurfaceOffset;
	if (outSurfaceSize != NULL)
		*outSurfaceSize = finalSurfaceSize;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const TilingParameters *tp, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch)
{
	SCE_GNM_ASSERT_MSG_RETURN(outTiledPixels != 0, kStatusInvalidArgument, "outTiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(inUntiledPixels != 0, kStatusInvalidArgument, "inUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(destRegion != 0, kStatusInvalidArgument, "destRegion must not be NULL."); // more extensive region validation in the lower-level tileSurface() functions.

	// Use gpu_addr to come up with actual legal/padded surface parameters
	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	if (status != kStatusSuccess)
		return status;

	// Sometimes the output tilemode is different from the input!
	// e.g. the smallest mipmap levels are treated as 1D-tiled surfaces, to avoid excessive padding.
	// Therefore, we tile based on the surfInfoOut's tileMode instead of the one passed in tp.
	TilingParameters correctedTP;
	memcpy(&correctedTP, tp, sizeof(correctedTP));
	status = adjustTileMode(correctedTP.m_minGpuMode, &correctedTP.m_tileMode, tp->m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	Gnm::ArrayMode arrayMode;
	status = getArrayMode(&arrayMode, correctedTP.m_tileMode);
	if (status != kStatusSuccess)
		return status;
	switch(arrayMode)
	{
	case Gnm::kArrayModeLinearGeneral:
		// Just memcpy
		SCE_GNM_ASSERT((surfInfoOut.m_surfaceSize & 0xFFFFFFFF00000000LL) == 0LL); // Make sure the size fit in 32b -> for the memcpy
		memcpy(outTiledPixels, inUntiledPixels, static_cast<size_t>(surfInfoOut.m_surfaceSize));
		return kStatusSuccess;
	case Gnm::kArrayModeLinearAligned:
		{
			TilerLinear tiler(&correctedTP);
			return tiler.tileSurfaceRegion(outTiledPixels, inUntiledPixels, destRegion, sourcePitch, sourceSlicePitch);
		}
	case Gnm::kArrayMode1dTiledThin:
	case Gnm::kArrayMode1dTiledThick:
		{
			Tiler1d tiler(&correctedTP);
			return tiler.tileSurfaceRegion(outTiledPixels, inUntiledPixels, destRegion, sourcePitch, sourceSlicePitch);
		}
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode2dTiledXThick:
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayMode3dTiledXThick:
	case Gnm::kArrayModeTiledThinPrt:
	case Gnm::kArrayModeTiledThickPrt:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode2dTiledThickPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThickPrt:
		{
			Tiler2d tiler(&correctedTP);
			return tiler.tileSurfaceRegion(outTiledPixels, inUntiledPixels, destRegion, sourcePitch, sourceSlicePitch);
		}
	default:
		// Unsupported tiling mode
		SCE_GNM_ERROR("Invalid corrected tile mode (0x%02X).", correctedTP.m_tileMode);
		return kStatusInvalidArgument;
	}
}
int32_t sce::GpuAddress::tileSurface(void *outTiledPixels, const void *untiledPixels, const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(tp != NULL, kStatusInvalidArgument, "tp must not be NULL.");
	SurfaceRegion destRegion;
	destRegion.m_left   = destRegion.m_top = destRegion.m_front = 0;
	// Convert tp image dimensions (in pixels) to region dimensions (in elements)
	destRegion.m_right  = tp->m_linearWidth;
	destRegion.m_bottom = tp->m_linearHeight;
	destRegion.m_back   = tp->m_linearDepth;
	uint32_t elemWidth  = tp->m_linearWidth;
	uint32_t elemHeight = tp->m_linearHeight;
	if (tp->m_isBlockCompressed)
	{
		switch(tp->m_bitsPerFragment)
		{
		case 1:
			destRegion.m_left   = (destRegion.m_left+7)   / 8;
			destRegion.m_right  = (destRegion.m_right+7)  / 8;
			elemWidth           = (elemWidth+7)           / 8;
			break;
		case 4:
		case 8:
			destRegion.m_left   = (destRegion.m_left+3)   / 4;
			destRegion.m_top    = (destRegion.m_top+3)    / 4;
			destRegion.m_right  = (destRegion.m_right+3)  / 4;
			destRegion.m_bottom = (destRegion.m_bottom+3) / 4;
			elemWidth           = (elemWidth+3)           / 4;
			elemHeight          = (elemHeight+3)          / 4;
			// destRegion.m_back doesn't need to be rescaled; BCn blocks don't have thickness.
			break;
		case 16:
			// TODO
			break;
		default:
			SCE_GNM_ASSERT_MSG_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", tp->m_bitsPerFragment);
			break;
		}
	}
	return tileSurfaceRegion(outTiledPixels, untiledPixels, tp, &destRegion, elemWidth, elemWidth*elemHeight);
}

int32_t sce::GpuAddress::detileSurfaceRegion(void *outUntiledPixels, const void *tiledPixels, const TilingParameters *tp, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitchElems)
{
	SCE_GNM_ASSERT_MSG_RETURN(outUntiledPixels != 0, kStatusInvalidArgument, "outUntiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tiledPixels != 0, kStatusInvalidArgument, "tiledPixels must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(srcRegion != 0, kStatusInvalidArgument, "srcRegion must not be NULL."); // more extensive region validation in the lower-level tileSurface() functions.

	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	if (status != kStatusSuccess)
		return status;

	// Sometimes the output tilemode is different from the input!
	// e.g. the smallest mipmap levels are treated as 1D-tiled surfaces, to avoid excessive padding.
	// Therefore, we tile based on the surfInfoOut's tileMode instead of the one passed in tp.
	TilingParameters correctedTP;
	memcpy(&correctedTP, tp, sizeof(correctedTP));
	status = adjustTileMode(correctedTP.m_minGpuMode, &correctedTP.m_tileMode, tp->m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	Gnm::ArrayMode arrayMode;
	status = getArrayMode(&arrayMode, correctedTP.m_tileMode);
	if (status != kStatusSuccess)
		return status;
	switch(arrayMode)
	{
	case Gnm::kArrayModeLinearGeneral:
		// Just memcpy
		SCE_GNM_ASSERT((surfInfoOut.m_surfaceSize & 0xFFFFFFFF00000000LL) == 0LL); // Make sure the size fit in 32b -> for the memcpy
		memcpy(outUntiledPixels, tiledPixels, static_cast<size_t>(surfInfoOut.m_surfaceSize));
		return kStatusSuccess;
	case Gnm::kArrayModeLinearAligned:
		{
			TilerLinear tiler(&correctedTP);
			return tiler.detileSurfaceRegion(outUntiledPixels, tiledPixels, srcRegion, destPitch, destSlicePitchElems);
		}
	case Gnm::kArrayMode1dTiledThin:
	case Gnm::kArrayMode1dTiledThick:
		{
			Tiler1d tiler(&correctedTP);
			return tiler.detileSurfaceRegion(outUntiledPixels, tiledPixels, srcRegion, destPitch, destSlicePitchElems);
		}
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode2dTiledXThick:
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayMode3dTiledXThick:
	case Gnm::kArrayModeTiledThinPrt:
	case Gnm::kArrayModeTiledThickPrt:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode2dTiledThickPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThickPrt:
		{
			Tiler2d tiler(&correctedTP);
			return tiler.detileSurfaceRegion(outUntiledPixels, tiledPixels, srcRegion, destPitch, destSlicePitchElems);
		}
	default:
		// Unsupported tiling mode
		SCE_GNM_ERROR("Invalid corrected tile mode (0x%02X).", correctedTP.m_tileMode);
		return kStatusInvalidArgument;
	}
}
int32_t sce::GpuAddress::detileSurface(void *outUntiledPixels, const void *tiledPixels, const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(tp != NULL, kStatusInvalidArgument, "tp must not be NULL.");
	SurfaceRegion srcRegion;
	srcRegion.m_left    = srcRegion.m_top = srcRegion.m_front = 0;
	srcRegion.m_right   = tp->m_linearWidth;
	srcRegion.m_bottom  = tp->m_linearHeight;
	srcRegion.m_back    = tp->m_linearDepth;
	uint32_t elemWidth  = tp->m_linearWidth;
	uint32_t elemHeight = tp->m_linearHeight;
	if (tp->m_isBlockCompressed)
	{
		switch(tp->m_bitsPerFragment)
		{
		case 1:
			srcRegion.m_left   = (srcRegion.m_left+7)   / 8;
			srcRegion.m_right  = (srcRegion.m_right+7)  / 8;
			elemWidth          = (elemWidth+7)          / 8;
			break;
		case 4:
		case 8:
			srcRegion.m_left   = (srcRegion.m_left+3)   / 4;
			srcRegion.m_top    = (srcRegion.m_top+3)    / 4;
			srcRegion.m_right  = (srcRegion.m_right+3)  / 4;
			srcRegion.m_bottom = (srcRegion.m_bottom+3) / 4;
			elemWidth          = (elemWidth+3)          / 4;
			elemHeight         = (elemHeight+3)         / 4;
			// srcRegion.m_back doesn't need to be rescaled; BCn blocks don't have thickness.
			break;
		case 16:
			// TODO
			break;
		default:
			SCE_GNM_ASSERT_MSG_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", tp->m_bitsPerFragment);
			break;
		}
	}
	return detileSurfaceRegion(outUntiledPixels, tiledPixels, tp, &srcRegion, elemWidth, elemWidth*elemHeight);
}

int32_t sce::GpuAddress::computeLinearElementByteOffset(uint64_t *outUntiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex,
														uint32_t pitch, uint32_t SlicePitchElems, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outUntiledByteOffset != 0, kStatusInvalidArgument, "outUntiledByteOffset must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(numFragmentsPerPixel) && numFragmentsPerPixel <= 8, kStatusInvalidArgument, "numFragmentsPerPixel (%u) must be 1, 2, 4, or 8.", numFragmentsPerPixel);
	uint64_t absolute_element_index = z * SlicePitchElems + y*pitch + x;
	*outUntiledByteOffset = (absolute_element_index * bitsPerElement * numFragmentsPerPixel) + (bitsPerElement * fragmentIndex);
	*outUntiledByteOffset /= 8;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeTiledElementByteOffset(uint64_t *outTiledByteOffset, const TilingParameters *tp, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex)
{
	SCE_GNM_ASSERT_MSG_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(outTiledByteOffset != 0, kStatusInvalidArgument, "outTiledByteOffset must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp->m_linearDepth == 1 || tp->m_surfaceFlags.m_volume, kStatusInvalidArgument, "for non-volume textures, tp->m_linearDepth must be 1.");
	// Use gpu_addr to come up with actual legal/padded surface parameters
	SurfaceInfo surfInfoOut = {0};
	int32_t status = computeSurfaceInfo(&surfInfoOut, tp);
	if (status != kStatusSuccess)
		return status;

	TilingParameters correctedTP;
	memcpy(&correctedTP, tp, sizeof(correctedTP));
	status = adjustTileMode(tp->m_minGpuMode, &correctedTP.m_tileMode, tp->m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;
	Gnm::ArrayMode arrayMode;
	status = getArrayMode(&arrayMode, correctedTP.m_tileMode);
	if (status != kStatusSuccess)
		return status;
	switch(arrayMode)
	{
	case Gnm::kArrayModeLinearAligned:
		{
			TilerLinear tiler(&correctedTP);
			return tiler.getTiledElementByteOffset(outTiledByteOffset, x, y, z);
		}
	case Gnm::kArrayMode1dTiledThin:
	case Gnm::kArrayMode1dTiledThick:
		{
			Tiler1d tiler(&correctedTP);
			return tiler.getTiledElementByteOffset(outTiledByteOffset, x, y, z);
		}
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode2dTiledXThick:
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayMode3dTiledXThick:
	case Gnm::kArrayModeTiledThinPrt:
	case Gnm::kArrayModeTiledThickPrt:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode2dTiledThickPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThickPrt:
		{
			Tiler2d tiler(&correctedTP);
			return tiler.getTiledElementByteOffset(outTiledByteOffset, x, y, z, fragmentIndex);
		}
	default:
		// Unsupported tiling mode
		SCE_GNM_ERROR("Invalid corrected tile mode (0x%02X).", correctedTP.m_tileMode);
		return kStatusInvalidArgument;
	}
}

#include "GnmMetadata.h"

int32_t sce::GpuAddress::computeHtileBufferByteOffset(uint32_t *outOffset, const Gnm::DepthRenderTarget *depthRenderTarget, uint32_t tileX, uint32_t tileY)
{
	SCE_GNM_ASSERT_MSG_RETURN(outOffset != NULL, kStatusInvalidArgument, "outOffset must not be NULL.");
	const unsigned tilesWide = (depthRenderTarget->getWidth() + 7) / 8;
	unsigned mode = 0;
	if(!depthRenderTarget->getLinearHtile())
		mode |= 1;
	if(Gnm::kPipeConfigP16 == depthRenderTarget->getPipeConfig())
		mode |= 2;
	if(depthRenderTarget->getHtileTextureCompatible())
		mode = 4;
	*outOffset = HtileOffsetInDwords(tileX, tilesWide, tileY, mode) * sizeof(uint32_t);
	return kStatusSuccess;
}
int32_t sce::GpuAddress::computeHtileBufferByteOffset(uint32_t *outOffset, uint32_t tileX, uint32_t tileY, uint32_t tilesWide, bool isHtileLinear) // DEPRECATED
{
	SCE_GNM_ASSERT_MSG_RETURN(outOffset != NULL, kStatusInvalidArgument, "outOffset must not be NULL.");
	unsigned mode = 0;
	if(!isHtileLinear)
		mode |= 1;
	*outOffset = HtileOffsetInDwords(tileX, tilesWide, tileY, mode) * sizeof(uint32_t);
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeCmaskBufferNybbleOffset(uint32_t *outOffset, const Gnm::RenderTarget *renderTarget, uint32_t tileX, uint32_t tileY)
{
	SCE_GNM_ASSERT_MSG_RETURN(outOffset != NULL, kStatusInvalidArgument, "outOffset must not be NULL.");
	const unsigned tilesWide = (renderTarget->getWidth() + 7) / 8;
	const bool isCmaskLinear = renderTarget->getLinearCmask();
	const bool isNeo = renderTarget->getUseAltTileMode();
	*outOffset = CmaskOffsetInNybbles(tileX, tilesWide, tileY, isCmaskLinear, isNeo);
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeCmaskBufferByteOffset(uint32_t *outOffset, uint32_t tileX, uint32_t tileY, uint32_t tilesWide, bool isCmaskLinear) // DEPRECATED
{
	SCE_GNM_ASSERT_MSG_RETURN(outOffset != NULL, kStatusInvalidArgument, "outOffset must not be NULL.");
	*outOffset = CmaskOffsetInNybbles(tileX, tilesWide, tileY, isCmaskLinear, 0);
	return kStatusSuccess;
}
