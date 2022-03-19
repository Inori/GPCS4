#pragma once

#include "GnmGpuAddressInternal.h"

#include <cstdint>
#include <x86intrin.h>


namespace sce
{
	namespace GpuAddress
	{
		typedef void (*MicroTileFunc)(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch);

		/** @brief Tiles an 8x8 microtile of an 8bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile8bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch * sizeof(uint8_t);

			const __m128i row0 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 0 * srcPitchBytes));
			const __m128i row1 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 1 * srcPitchBytes));
			const __m128i row2 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 2 * srcPitchBytes));
			const __m128i row3 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 3 * srcPitchBytes));
			const __m128i row4 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 4 * srcPitchBytes));
			const __m128i row5 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 5 * srcPitchBytes));
			const __m128i row6 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 6 * srcPitchBytes));
			const __m128i row7 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcBytes + 7 * srcPitchBytes));

			const __m128i row02 = _mm_unpacklo_epi64(row0, row2);
			const __m128i row13 = _mm_unpacklo_epi64(row1, row3);
			const __m128i row46 = _mm_unpacklo_epi64(row4, row6);
			const __m128i row57 = _mm_unpacklo_epi64(row5, row7);

			_mm_store_si128( dest16s + 0x0, row02 );
			_mm_store_si128( dest16s + 0x1, row13 );
			_mm_store_si128( dest16s + 0x2, row46 );
			_mm_store_si128( dest16s + 0x3, row57 );
		}

		/** @brief Detiles an 8x8 microtile of an 8bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile8bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i *src16s    = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint8_t);

			const __m128i row02 = _mm_load_si128( src16s + 0x0 );
			const __m128i row13 = _mm_load_si128( src16s + 0x1 );
			const __m128i row46 = _mm_load_si128( src16s + 0x2 );
			const __m128i row57 = _mm_load_si128( src16s + 0x3 );

			const __m128i out01 = _mm_unpacklo_epi64(row02, row13);
			const __m128i out23 = _mm_unpackhi_epi64(row02, row13);
			const __m128i out45 = _mm_unpacklo_epi64(row46, row57);
			const __m128i out67 = _mm_unpackhi_epi64(row46, row57);

			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes),                out01     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes), _mm_srli_si128(out01, 8) );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes),                out23     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes), _mm_srli_si128(out23, 8) );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 4 * destPitchBytes),                out45     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 5 * destPitchBytes), _mm_srli_si128(out45, 8) );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 6 * destPitchBytes),                out67     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 7 * destPitchBytes), _mm_srli_si128(out67, 8) );
		}

		/** @brief Tiles an 8x8 microtile of an 16bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile16bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint16_t);

			const __m128i row0 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0 * srcPitchBytes) );
			const __m128i row1 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1 * srcPitchBytes) );
			const __m128i row2 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2 * srcPitchBytes) );
			const __m128i row3 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3 * srcPitchBytes) );
			const __m128i row4 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 4 * srcPitchBytes) );
			const __m128i row5 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 5 * srcPitchBytes) );
			const __m128i row6 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 6 * srcPitchBytes) );
			const __m128i row7 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 7 * srcPitchBytes) );

			_mm_store_si128( dest16s + 0x0, row0 );
			_mm_store_si128( dest16s + 0x1, row1 );
			_mm_store_si128( dest16s + 0x2, row2 );
			_mm_store_si128( dest16s + 0x3, row3 );
			_mm_store_si128( dest16s + 0x4, row4 );
			_mm_store_si128( dest16s + 0x5, row5 );
			_mm_store_si128( dest16s + 0x6, row6 );
			_mm_store_si128( dest16s + 0x7, row7 );
		}

		/** @brief Detiles an 8x8 microtile of an 16bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile16bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i * src16s   = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint16_t);

			const __m128i row0 = _mm_load_si128( src16s + 0x0 );
			const __m128i row1 = _mm_load_si128( src16s + 0x1 );
			const __m128i row2 = _mm_load_si128( src16s + 0x2 );
			const __m128i row3 = _mm_load_si128( src16s + 0x3 );
			const __m128i row4 = _mm_load_si128( src16s + 0x4 );
			const __m128i row5 = _mm_load_si128( src16s + 0x5 );
			const __m128i row6 = _mm_load_si128( src16s + 0x6 );
			const __m128i row7 = _mm_load_si128( src16s + 0x7 );

			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes), row0 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes), row1 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes), row2 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes), row3 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 4 * destPitchBytes), row4 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 5 * destPitchBytes), row5 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 6 * destPitchBytes), row6 );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 7 * destPitchBytes), row7 );
		}

		/** @brief Tiles an 8x8 microtile of an 32bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile32bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint32_t);

			int32_t loopCount = 2;
			do
			{
			    const __m128i row00 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 0*16) );
			    const __m128i row01 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 1*16) );
			    const __m128i row10 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 0*16) );
			    const __m128i row11 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 1*16) );
			    const __m128i row20 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 0*16) );
			    const __m128i row21 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 1*16) );
			    const __m128i row30 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 0*16) );
			    const __m128i row31 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 1*16) );
				srcBytes += 4 * srcPitchBytes;

			    _mm_store_si128( dest16s + 0, row00 );
			    _mm_store_si128( dest16s + 1, row10 );
			    _mm_store_si128( dest16s + 2, row01 );
			    _mm_store_si128( dest16s + 3, row11 );
			    _mm_store_si128( dest16s + 4, row20 );
			    _mm_store_si128( dest16s + 5, row30 );
			    _mm_store_si128( dest16s + 6, row21 );
			    _mm_store_si128( dest16s + 7, row31 );
                dest16s += 8;
			}
			while (--loopCount);
		}

		/** @brief Detiles an 8x8 microtile of an 32bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile32bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i *src16s    = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint32_t);

			int32_t loopCount = 2;
			do
			{
			    const __m128i row00 = _mm_load_si128( src16s + 0 );
			    const __m128i row10 = _mm_load_si128( src16s + 1 );
			    const __m128i row01 = _mm_load_si128( src16s + 2 );
			    const __m128i row11 = _mm_load_si128( src16s + 3 );
			    const __m128i row20 = _mm_load_si128( src16s + 4 );
			    const __m128i row30 = _mm_load_si128( src16s + 5 );
			    const __m128i row21 = _mm_load_si128( src16s + 6 );
			    const __m128i row31 = _mm_load_si128( src16s + 7 );
                src16s += 8;

			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 0 * 16), row00 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 1 * 16), row01 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 0 * 16), row10 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 1 * 16), row11 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 0 * 16), row20 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 1 * 16), row21 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 0 * 16), row30 );
			    _mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 1 * 16), row31 );
				destBytes += 4 * destPitchBytes;
			}
			while (--loopCount);
		}

		/** @brief Tiles an 8x8 microtile of an 64bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile64bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint64_t);

			int32_t loopCount = 4;
			do
			{
				const __m128i row00 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0 * srcPitchBytes + 0 * 16) );
				const __m128i row01 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0 * srcPitchBytes + 1 * 16) );
				const __m128i row02 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0 * srcPitchBytes + 2 * 16) );
				const __m128i row03 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0 * srcPitchBytes + 3 * 16) );
				const __m128i row10 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1 * srcPitchBytes + 0 * 16) );
				const __m128i row11 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1 * srcPitchBytes + 1 * 16) );
				const __m128i row12 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1 * srcPitchBytes + 2 * 16) );
				const __m128i row13 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1 * srcPitchBytes + 3 * 16) );
				srcBytes += 2 * srcPitchBytes;

				_mm_store_si128( dest16s + 0, row00 );
				_mm_store_si128( dest16s + 1, row10 );
				_mm_store_si128( dest16s + 2, row01 );
				_mm_store_si128( dest16s + 3, row11 );
				_mm_store_si128( dest16s + 4, row02 );
				_mm_store_si128( dest16s + 5, row12 );
				_mm_store_si128( dest16s + 6, row03 );
				_mm_store_si128( dest16s + 7, row13 );
				dest16s += 8;
			}
			while (--loopCount);
		}

		/** @brief Detiles an 8x8 microtile of an 64bpp surface, using the Display microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile64bppDisplaySse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i *src16s    = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint64_t);

			int32_t loopCount = 4;
			do
			{
				const __m128i row00 = _mm_load_si128( src16s + 0 );
				const __m128i row10 = _mm_load_si128( src16s + 1 );
				const __m128i row01 = _mm_load_si128( src16s + 2 );
				const __m128i row11 = _mm_load_si128( src16s + 3 );
				const __m128i row02 = _mm_load_si128( src16s + 4 );
				const __m128i row12 = _mm_load_si128( src16s + 5 );
				const __m128i row03 = _mm_load_si128( src16s + 6 );
				const __m128i row13 = _mm_load_si128( src16s + 7 );
				src16s += 8;

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 0 * 16), row00 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 1 * 16), row01 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 2 * 16), row02 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 3 * 16), row03 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 0 * 16), row10 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 1 * 16), row11 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 2 * 16), row12 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 3 * 16), row13 );
				destBytes += 2 * destPitchBytes;
			}
			while (--loopCount);
		}

		/** @brief Tiles an 8x8 microtile of an 8bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile8bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint8_t);

			__m128i row0 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  0*srcPitchBytes) );
			__m128i row1 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  1*srcPitchBytes) );
			__m128i row2 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  2*srcPitchBytes) );
			__m128i row3 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  3*srcPitchBytes) );
			__m128i row4 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  4*srcPitchBytes) );
			__m128i row5 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  5*srcPitchBytes) );
			__m128i row6 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  6*srcPitchBytes) );
			__m128i row7 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes +  7*srcPitchBytes) );

			__m128i row01= _mm_unpacklo_epi64(row0, row1);
			__m128i row23= _mm_unpacklo_epi64(row2, row3);
			__m128i row45= _mm_unpacklo_epi64(row4, row5);
			__m128i row67= _mm_unpacklo_epi64(row6, row7);

			__m128i tmp0 = _mm_unpacklo_epi32(row01, row23);
			__m128i tmp1 = _mm_unpackhi_epi32(row01, row23);
			__m128i tmp2 = _mm_unpacklo_epi32(row45, row67);
			__m128i tmp3 = _mm_unpackhi_epi32(row45, row67);

			_mm_store_si128( dest16s + 0x0, _mm_unpacklo_epi16(tmp0, tmp1) );
			_mm_store_si128( dest16s + 0x1, _mm_unpackhi_epi16(tmp0, tmp1) );
			_mm_store_si128( dest16s + 0x2, _mm_unpacklo_epi16(tmp2, tmp3) );
			_mm_store_si128( dest16s + 0x3, _mm_unpackhi_epi16(tmp2, tmp3) );
		}

		/** @brief Detiles an 8x8 microtile of an 8bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile8bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i *src16s    = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint8_t);

			__m128i row01 = _mm_load_si128( src16s + 0x0 );
			__m128i row23 = _mm_load_si128( src16s + 0x1 );
			__m128i row45 = _mm_load_si128( src16s + 0x2 );
			__m128i row67 = _mm_load_si128( src16s + 0x3 );

			__m128i tmp0 = _mm_unpacklo_epi64(row01, row23);
			__m128i tmp1 = _mm_unpackhi_epi64(row01, row23);
			__m128i tmp2 = _mm_unpacklo_epi64(row45, row67);
			__m128i tmp3 = _mm_unpackhi_epi64(row45, row67);

			__m128i out01 = _mm_shuffle_epi32( _mm_shufflehi_epi16( _mm_shufflelo_epi16(tmp0, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0));
			__m128i out23 = _mm_shuffle_epi32( _mm_shufflehi_epi16( _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0));
			__m128i out45 = _mm_shuffle_epi32( _mm_shufflehi_epi16( _mm_shufflelo_epi16(tmp2, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0));
			__m128i out67 = _mm_shuffle_epi32( _mm_shufflehi_epi16( _mm_shufflelo_epi16(tmp3, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0));

			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes),                out01     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes), _mm_srli_si128(out01, 8) );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes),                out23     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes), _mm_srli_si128(out23, 8) );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 4*destPitchBytes),                out45     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 5*destPitchBytes), _mm_srli_si128(out45, 8) );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 6*destPitchBytes),                out67     );
			_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 7*destPitchBytes), _mm_srli_si128(out67, 8) );
		}

		/** @brief Tiles an 8x8 microtile of an 16bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile16bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint16_t);

			__m128i row0 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes) );
			__m128i row1 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes) );
			__m128i row2 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes) );
			__m128i row3 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes) );
			__m128i row4 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 4*srcPitchBytes) );
			__m128i row5 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 5*srcPitchBytes) );
			__m128i row6 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 6*srcPitchBytes) );
			__m128i row7 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 7*srcPitchBytes) );

			_mm_store_si128( dest16s + 0x0, _mm_unpacklo_epi32(row0, row1) );
			_mm_store_si128( dest16s + 0x1, _mm_unpacklo_epi32(row2, row3) );
			_mm_store_si128( dest16s + 0x2, _mm_unpackhi_epi32(row0, row1) );
			_mm_store_si128( dest16s + 0x3, _mm_unpackhi_epi32(row2, row3) );
			_mm_store_si128( dest16s + 0x4, _mm_unpacklo_epi32(row4, row5) );
			_mm_store_si128( dest16s + 0x5, _mm_unpacklo_epi32(row6, row7) );
			_mm_store_si128( dest16s + 0x6, _mm_unpackhi_epi32(row4, row5) );
			_mm_store_si128( dest16s + 0x7, _mm_unpackhi_epi32(row6, row7) );
		}

		/** @brief Detiles an 8x8 microtile of an 16bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile16bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i * src16s   = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint16_t);

			__m128i row0 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x0 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row1 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x1 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row2 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x2 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row3 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x3 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row4 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x4 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row5 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x5 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row6 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x6 ), _MM_SHUFFLE(3,1,2,0) );
			__m128i row7 = _mm_shuffle_epi32( _mm_load_si128( src16s + 0x7 ), _MM_SHUFFLE(3,1,2,0) );

			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes), _mm_unpacklo_epi64(row0, row2) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes), _mm_unpackhi_epi64(row0, row2) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes), _mm_unpacklo_epi64(row1, row3) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes), _mm_unpackhi_epi64(row1, row3) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 4*destPitchBytes), _mm_unpacklo_epi64(row4, row6) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 5*destPitchBytes), _mm_unpackhi_epi64(row4, row6) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 6*destPitchBytes), _mm_unpacklo_epi64(row5, row7) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 7*destPitchBytes), _mm_unpackhi_epi64(row5, row7) );
		}

		/** @brief Tiles an 8x8 microtile of an 32bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile32bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint32_t);

			__m128i row00 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 0*16) );
			__m128i row01 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 1*16) );
			__m128i row10 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 0*16) );
			__m128i row11 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 1*16) );
			__m128i row20 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 0*16) );
			__m128i row21 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 1*16) );
			__m128i row30 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 0*16) );
			__m128i row31 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 1*16) );
			__m128i row40 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 4*srcPitchBytes + 0*16) );
			__m128i row41 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 4*srcPitchBytes + 1*16) );
			__m128i row50 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 5*srcPitchBytes + 0*16) );
			__m128i row51 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 5*srcPitchBytes + 1*16) );
			__m128i row60 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 6*srcPitchBytes + 0*16) );
			__m128i row61 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 6*srcPitchBytes + 1*16) );
			__m128i row70 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 7*srcPitchBytes + 0*16) );
			__m128i row71 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 7*srcPitchBytes + 1*16) );

			_mm_store_si128( dest16s + 0x0, _mm_unpacklo_epi64(row00, row10) );
			_mm_store_si128( dest16s + 0x1, _mm_unpackhi_epi64(row00, row10) );
			_mm_store_si128( dest16s + 0x2, _mm_unpacklo_epi64(row20, row30) );
			_mm_store_si128( dest16s + 0x3, _mm_unpackhi_epi64(row20, row30) );
			_mm_store_si128( dest16s + 0x4, _mm_unpacklo_epi64(row01, row11) );
			_mm_store_si128( dest16s + 0x5, _mm_unpackhi_epi64(row01, row11) );
			_mm_store_si128( dest16s + 0x6, _mm_unpacklo_epi64(row21, row31) );
			_mm_store_si128( dest16s + 0x7, _mm_unpackhi_epi64(row21, row31) );
			_mm_store_si128( dest16s + 0x8, _mm_unpacklo_epi64(row40, row50) );
			_mm_store_si128( dest16s + 0x9, _mm_unpackhi_epi64(row40, row50) );
			_mm_store_si128( dest16s + 0xA, _mm_unpacklo_epi64(row60, row70) );
			_mm_store_si128( dest16s + 0xB, _mm_unpackhi_epi64(row60, row70) );
			_mm_store_si128( dest16s + 0xC, _mm_unpacklo_epi64(row41, row51) );
			_mm_store_si128( dest16s + 0xD, _mm_unpackhi_epi64(row41, row51) );
			_mm_store_si128( dest16s + 0xE, _mm_unpacklo_epi64(row61, row71) );
			_mm_store_si128( dest16s + 0xF, _mm_unpackhi_epi64(row61, row71) );
		}

		/** @brief Detiles an 8x8 microtile of an 32bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile32bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i *src16s    = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint32_t);

			__m128i row00 = _mm_load_si128( src16s + 0x0 );
			__m128i row01 = _mm_load_si128( src16s + 0x1 );
			__m128i row10 = _mm_load_si128( src16s + 0x2 );
			__m128i row11 = _mm_load_si128( src16s + 0x3 );
			__m128i row20 = _mm_load_si128( src16s + 0x4 );
			__m128i row21 = _mm_load_si128( src16s + 0x5 );
			__m128i row30 = _mm_load_si128( src16s + 0x6 );
			__m128i row31 = _mm_load_si128( src16s + 0x7 );
			__m128i row40 = _mm_load_si128( src16s + 0x8 );
			__m128i row41 = _mm_load_si128( src16s + 0x9 );
			__m128i row50 = _mm_load_si128( src16s + 0xA );
			__m128i row51 = _mm_load_si128( src16s + 0xB );
			__m128i row60 = _mm_load_si128( src16s + 0xC );
			__m128i row61 = _mm_load_si128( src16s + 0xD );
			__m128i row70 = _mm_load_si128( src16s + 0xE );
			__m128i row71 = _mm_load_si128( src16s + 0xF );

			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 0*16), _mm_unpacklo_epi64(row00, row01) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 1*16), _mm_unpacklo_epi64(row20, row21) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 0*16), _mm_unpackhi_epi64(row00, row01) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 1*16), _mm_unpackhi_epi64(row20, row21) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 0*16), _mm_unpacklo_epi64(row10, row11) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 1*16), _mm_unpacklo_epi64(row30, row31) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 0*16), _mm_unpackhi_epi64(row10, row11) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 1*16), _mm_unpackhi_epi64(row30, row31) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 4*destPitchBytes + 0*16), _mm_unpacklo_epi64(row40, row41) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 4*destPitchBytes + 1*16), _mm_unpacklo_epi64(row60, row61) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 5*destPitchBytes + 0*16), _mm_unpackhi_epi64(row40, row41) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 5*destPitchBytes + 1*16), _mm_unpackhi_epi64(row60, row61) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 6*destPitchBytes + 0*16), _mm_unpacklo_epi64(row50, row51) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 6*destPitchBytes + 1*16), _mm_unpacklo_epi64(row70, row71) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 7*destPitchBytes + 0*16), _mm_unpackhi_epi64(row50, row51) );
			_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 7*destPitchBytes + 1*16), _mm_unpackhi_epi64(row70, row71) );
		}

		/** @brief Tiles an 8x8 microtile of an 64bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile64bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t *srcBytes  = (const uint8_t*)srcTileBase;
			__m128i *dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(uint64_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row00 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 0*16) );
				__m128i row01 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 1*16) );
				__m128i row02 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 2*16) );
				__m128i row03 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 3*16) );
				__m128i row10 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 0*16) );
				__m128i row11 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 1*16) );
				__m128i row12 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 2*16) );
				__m128i row13 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 3*16) );
				__m128i row20 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 0*16) );
				__m128i row21 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 1*16) );
				__m128i row22 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 2*16) );
				__m128i row23 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 3*16) );
				__m128i row30 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 0*16) );
				__m128i row31 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 1*16) );
				__m128i row32 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 2*16) );
				__m128i row33 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 3*16) );
				srcBytes += 4*srcPitchBytes;

				_mm_store_si128( dest16s + 0x0, row00 );
				_mm_store_si128( dest16s + 0x1, row10 );
				_mm_store_si128( dest16s + 0x2, row01 );
				_mm_store_si128( dest16s + 0x3, row11 );
				_mm_store_si128( dest16s + 0x4, row20 );
				_mm_store_si128( dest16s + 0x5, row30 );
				_mm_store_si128( dest16s + 0x6, row21 );
				_mm_store_si128( dest16s + 0x7, row31 );
				_mm_store_si128( dest16s + 0x8, row02 );
				_mm_store_si128( dest16s + 0x9, row12 );
				_mm_store_si128( dest16s + 0xA, row03 );
				_mm_store_si128( dest16s + 0xB, row13 );
				_mm_store_si128( dest16s + 0xC, row22 );
				_mm_store_si128( dest16s + 0xD, row32 );
				_mm_store_si128( dest16s + 0xE, row23 );
				_mm_store_si128( dest16s + 0xF, row33 );
				dest16s += 0x10;
			}
			while (--loopCount);
		}

		/** @brief Detiles an 8x8 microtile of an 64bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile64bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i *src16s    = (const __m128i*)srcTileBase;
			uint8_t       *destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(uint64_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row00 = _mm_load_si128( src16s + 0x0 );
				__m128i row01 = _mm_load_si128( src16s + 0x1 );
				__m128i row02 = _mm_load_si128( src16s + 0x2 );
				__m128i row03 = _mm_load_si128( src16s + 0x3 );
				__m128i row10 = _mm_load_si128( src16s + 0x4 );
				__m128i row11 = _mm_load_si128( src16s + 0x5 );
				__m128i row12 = _mm_load_si128( src16s + 0x6 );
				__m128i row13 = _mm_load_si128( src16s + 0x7 );
				__m128i row20 = _mm_load_si128( src16s + 0x8 );
				__m128i row21 = _mm_load_si128( src16s + 0x9 );
				__m128i row22 = _mm_load_si128( src16s + 0xA );
				__m128i row23 = _mm_load_si128( src16s + 0xB );
				__m128i row30 = _mm_load_si128( src16s + 0xC );
				__m128i row31 = _mm_load_si128( src16s + 0xD );
				__m128i row32 = _mm_load_si128( src16s + 0xE );
				__m128i row33 = _mm_load_si128( src16s + 0xF );
				src16s += 0x10;

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 0*16), row00 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 1*16), row02 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 2*16), row20 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 3*16), row22 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 0*16), row01 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 1*16), row03 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 2*16), row21 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 3*16), row23 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 0*16), row10 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 1*16), row12 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 2*16), row30 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 3*16), row32 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 0*16), row11 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 1*16), row13 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 2*16), row31 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 3*16), row33 );
				destBytes += 4*destPitchBytes;
			}
			while (--loopCount);
		}

		/** @brief Tiles an 8x8 microtile of an 128bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch This parameter is ignored.
		*/
		inline void tile128bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch)
		{
			SCE_GNM_UNUSED(srcSlicePitch);
			const uint8_t * srcBytes  = (const uint8_t*)srcTileBase;
			__m128i * dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcPitchBytes = srcPitch*sizeof(__m128i);

			int32_t loopCount = 2;
			do
			{
				_mm_store_si128( dest16s + 0x00, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x01, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x02, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x03, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x04, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x05, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x06, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x07, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x10, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x11, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x12, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x13, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x14, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x15, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x16, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x17, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcPitchBytes + 7*16)) );

				_mm_store_si128( dest16s + 0x08, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x09, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x0A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x0B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x0C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x0D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x0E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x0F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x18, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x19, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x1A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x1B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x1C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x1D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 2*srcPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x1E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x1F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 3*srcPitchBytes + 7*16)) );

				srcBytes += 4*srcPitchBytes;
				dest16s += 0x20;
			}
			while (--loopCount);
		}

		/** @brief Detiles an 8x8 microtile of an 128bpp surface, using the Thin microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destPitch Number of elements in one row of destination data.
			@param[in] destSlicePitch This parameter is ignored.
		*/
		inline void detile128bppThinSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
		{
			SCE_GNM_UNUSED(destSlicePitch);
			const __m128i * src16s    = (const __m128i*)srcTileBase;
			uint8_t       * destBytes = (      uint8_t*)destTileBase;
			const uint32_t destPitchBytes = destPitch*sizeof(__m128i);

			int32_t loopCount = 2;
			do
			{
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 0*16), _mm_load_si128(src16s + 0x00) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 1*16), _mm_load_si128(src16s + 0x01) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 0*16), _mm_load_si128(src16s + 0x02) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 1*16), _mm_load_si128(src16s + 0x03) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 2*16), _mm_load_si128(src16s + 0x04) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 3*16), _mm_load_si128(src16s + 0x05) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 2*16), _mm_load_si128(src16s + 0x06) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 3*16), _mm_load_si128(src16s + 0x07) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 4*16), _mm_load_si128(src16s + 0x10) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 5*16), _mm_load_si128(src16s + 0x11) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 4*16), _mm_load_si128(src16s + 0x12) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 5*16), _mm_load_si128(src16s + 0x13) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 6*16), _mm_load_si128(src16s + 0x14) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destPitchBytes + 7*16), _mm_load_si128(src16s + 0x15) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 6*16), _mm_load_si128(src16s + 0x16) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destPitchBytes + 7*16), _mm_load_si128(src16s + 0x17) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 0*16), _mm_load_si128(src16s + 0x08) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 1*16), _mm_load_si128(src16s + 0x09) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 0*16), _mm_load_si128(src16s + 0x0A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 1*16), _mm_load_si128(src16s + 0x0B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 2*16), _mm_load_si128(src16s + 0x0C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 3*16), _mm_load_si128(src16s + 0x0D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 2*16), _mm_load_si128(src16s + 0x0E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 3*16), _mm_load_si128(src16s + 0x0F) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 4*16), _mm_load_si128(src16s + 0x18) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 5*16), _mm_load_si128(src16s + 0x19) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 4*16), _mm_load_si128(src16s + 0x1A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 5*16), _mm_load_si128(src16s + 0x1B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 6*16), _mm_load_si128(src16s + 0x1C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destPitchBytes + 7*16), _mm_load_si128(src16s + 0x1D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 6*16), _mm_load_si128(src16s + 0x1E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destPitchBytes + 7*16), _mm_load_si128(src16s + 0x1F) );

				src16s += 0x20;
				destBytes += 4*destPitchBytes;
			}
			while (--loopCount);
		}

		/** @brief Tiles an 8x8x4 microtile of an 8bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcRowPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch Number of elements in one row of source data.
		*/
		inline void tile8bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcRowPitch, const uint32_t srcSlicePitch)
		{
			const uint8_t * srcBytes  = (const uint8_t*)srcTileBase;
			__m128i * dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcRowPitchBytes   = srcRowPitch  *sizeof(uint8_t);
			const uint32_t srcSlicePitchBytes = srcSlicePitch*sizeof(uint8_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row0 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row1 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i row2 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i row3 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				__m128i row4 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row5 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i row6 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i row7 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				__m128i row8 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row9 = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i rowA = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i rowB = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				__m128i rowC = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i rowD = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i rowE = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i rowF = _mm_loadl_epi64( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				srcBytes += 4*srcRowPitchBytes;

				__m128i row01= _mm_unpacklo_epi16(row0, row1);
				__m128i row23= _mm_unpacklo_epi16(row2, row3);
				__m128i row45= _mm_unpacklo_epi16(row4, row5);
				__m128i row67= _mm_unpacklo_epi16(row6, row7);
				__m128i row89= _mm_unpacklo_epi16(row8, row9);
				__m128i rowAB= _mm_unpacklo_epi16(rowA, rowB);
				__m128i rowCD= _mm_unpacklo_epi16(rowC, rowD);
				__m128i rowEF= _mm_unpacklo_epi16(rowE, rowF);

				_mm_store_si128( dest16s + 0x0, _mm_unpacklo_epi64(row01, row23) );
				_mm_store_si128( dest16s + 0x1, _mm_unpacklo_epi64(row45, row67) );
				_mm_store_si128( dest16s + 0x2, _mm_unpacklo_epi64(row89, rowAB) );
				_mm_store_si128( dest16s + 0x3, _mm_unpacklo_epi64(rowCD, rowEF) );
				_mm_store_si128( dest16s + 0x4, _mm_unpackhi_epi64(row01, row23) );
				_mm_store_si128( dest16s + 0x5, _mm_unpackhi_epi64(row45, row67) );
				_mm_store_si128( dest16s + 0x6, _mm_unpackhi_epi64(row89, rowAB) );
				_mm_store_si128( dest16s + 0x7, _mm_unpackhi_epi64(rowCD, rowEF) );
				dest16s += 0x8;
			}
			while(--loopCount);
		}

		/** @brief Detiles an 8x8x4 microtile of an 8bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destRowPitch Number of elements in one row of source data.
			@param[in] destSlicePitch Number of elements in one row of source data.
			*/
		inline void detile8bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destRowPitch, const uint32_t destSlicePitch)
		{
			const __m128i * src16s  = (const __m128i*)srcTileBase;
			uint8_t       * destBytes = (      uint8_t*)destTileBase;
			const uint32_t destRowPitchBytes   = destRowPitch  *sizeof(uint8_t);
			const uint32_t destSlicePitchBytes = destSlicePitch*sizeof(uint8_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row0 = _mm_load_si128( src16s + 0x0 );
				__m128i row1 = _mm_load_si128( src16s + 0x1 );
				__m128i row2 = _mm_load_si128( src16s + 0x2 );
				__m128i row3 = _mm_load_si128( src16s + 0x3 );
				__m128i row4 = _mm_load_si128( src16s + 0x4 );
				__m128i row5 = _mm_load_si128( src16s + 0x5 );
				__m128i row6 = _mm_load_si128( src16s + 0x6 );
				__m128i row7 = _mm_load_si128( src16s + 0x7 );
				src16s += 0x8;

				row0 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row0, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row1 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row1, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row2 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row2, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row3 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row3, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row4 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row4, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row5 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row5, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row6 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row6, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );
				row7 = _mm_shufflehi_epi16( _mm_shufflelo_epi16(row7, _MM_SHUFFLE(3,1,2,0)), _MM_SHUFFLE(3,1,2,0) );

				__m128i out01 = _mm_unpacklo_epi32(row0, row4);
				__m128i out23 = _mm_unpackhi_epi32(row0, row4);
				__m128i out45 = _mm_unpacklo_epi32(row1, row5);
				__m128i out67 = _mm_unpackhi_epi32(row1, row5);
				__m128i out89 = _mm_unpacklo_epi32(row2, row6);
				__m128i outAB = _mm_unpackhi_epi32(row2, row6);
				__m128i outCD = _mm_unpacklo_epi32(row3, row7);
				__m128i outEF = _mm_unpackhi_epi32(row3, row7);

				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes),                out01     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes), _mm_srli_si128(out01, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes),                out23     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes), _mm_srli_si128(out23, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes),                out45     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes), _mm_srli_si128(out45, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes),                out67     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes), _mm_srli_si128(out67, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 0*destRowPitchBytes),                out89     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 1*destRowPitchBytes), _mm_srli_si128(out89, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 2*destRowPitchBytes),                outAB     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 3*destRowPitchBytes), _mm_srli_si128(outAB, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 0*destRowPitchBytes),                outCD     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 1*destRowPitchBytes), _mm_srli_si128(outCD, 8) );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 2*destRowPitchBytes),                outEF     );
				_mm_storel_epi64( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 3*destRowPitchBytes), _mm_srli_si128(outEF, 8) );
				destBytes += 4*destRowPitchBytes;
			}
			while(--loopCount);
		}

		/** @brief Tiles an 8x8x4 microtile of an 16bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcRowPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch Number of elements in one row of source data.
		*/
		inline void tile16bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcRowPitch, const uint32_t srcSlicePitch)
		{
			const uint8_t * srcBytes  = (const uint8_t*)srcTileBase;
			__m128i * dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcRowPitchBytes   = srcRowPitch  *sizeof(uint16_t);
			const uint32_t srcSlicePitchBytes = srcSlicePitch*sizeof(uint16_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row00 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row01 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i row02 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i row03 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				__m128i row10 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row11 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i row12 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i row13 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				_mm_store_si128( dest16s + 0x0, _mm_unpacklo_epi32(row00, row01) );
				_mm_store_si128( dest16s + 0x1, _mm_unpacklo_epi32(row02, row03) );
				_mm_store_si128( dest16s + 0x2, _mm_unpacklo_epi32(row10, row11) );
				_mm_store_si128( dest16s + 0x3, _mm_unpacklo_epi32(row12, row13) );
				_mm_store_si128( dest16s + 0x8, _mm_unpackhi_epi32(row00, row01) );
				_mm_store_si128( dest16s + 0x9, _mm_unpackhi_epi32(row02, row03) );
				_mm_store_si128( dest16s + 0xA, _mm_unpackhi_epi32(row10, row11) );
				_mm_store_si128( dest16s + 0xB, _mm_unpackhi_epi32(row12, row13) );

				__m128i row20 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row21 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i row22 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i row23 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				__m128i row30 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 0*srcRowPitchBytes) );
				__m128i row31 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 1*srcRowPitchBytes) );
				__m128i row32 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 2*srcRowPitchBytes) );
				__m128i row33 = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 3*srcRowPitchBytes) );
				_mm_store_si128( dest16s + 0x4, _mm_unpacklo_epi32(row20, row21) );
				_mm_store_si128( dest16s + 0x5, _mm_unpacklo_epi32(row22, row23) );
				_mm_store_si128( dest16s + 0x6, _mm_unpacklo_epi32(row30, row31) );
				_mm_store_si128( dest16s + 0x7, _mm_unpacklo_epi32(row32, row33) );
				_mm_store_si128( dest16s + 0xC, _mm_unpackhi_epi32(row20, row21) );
				_mm_store_si128( dest16s + 0xD, _mm_unpackhi_epi32(row22, row23) );
				_mm_store_si128( dest16s + 0xE, _mm_unpackhi_epi32(row30, row31) );
				_mm_store_si128( dest16s + 0xF, _mm_unpackhi_epi32(row32, row33) );
				srcBytes += 4*srcRowPitchBytes;
				dest16s += 0x10;
			}
			while(--loopCount);
		}

		/** @brief Detiles an 8x8x4 microtile of an 16bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destRowPitch Number of elements in one row of source data.
			@param[in] destSlicePitch Number of elements in one row of source data.
			*/
		inline void detile16bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destRowPitch, const uint32_t destSlicePitch)
		{
			const __m128i * src16s    = (const __m128i*)srcTileBase;
			uint8_t       * destBytes = (      uint8_t*)destTileBase;
			const uint32_t destRowPitchBytes   = destRowPitch  *sizeof(uint16_t);
			const uint32_t destSlicePitchBytes = destSlicePitch*sizeof(uint16_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row0 = _mm_load_si128( src16s + 0x0 );
				__m128i row1 = _mm_load_si128( src16s + 0x1 );
				__m128i row2 = _mm_load_si128( src16s + 0x2 );
				__m128i row3 = _mm_load_si128( src16s + 0x3 );
				__m128i row8 = _mm_load_si128( src16s + 0x8 );
				__m128i row9 = _mm_load_si128( src16s + 0x9 );
				__m128i rowA = _mm_load_si128( src16s + 0xA );
				__m128i rowB = _mm_load_si128( src16s + 0xB );
				row0 = _mm_shuffle_epi32( row0, _MM_SHUFFLE(3,1,2,0) );
				row1 = _mm_shuffle_epi32( row1, _MM_SHUFFLE(3,1,2,0) );
				row2 = _mm_shuffle_epi32( row2, _MM_SHUFFLE(3,1,2,0) );
				row3 = _mm_shuffle_epi32( row3, _MM_SHUFFLE(3,1,2,0) );
				row8 = _mm_shuffle_epi32( row8, _MM_SHUFFLE(3,1,2,0) );
				row9 = _mm_shuffle_epi32( row9, _MM_SHUFFLE(3,1,2,0) );
				rowA = _mm_shuffle_epi32( rowA, _MM_SHUFFLE(3,1,2,0) );
				rowB = _mm_shuffle_epi32( rowB, _MM_SHUFFLE(3,1,2,0) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes), _mm_unpacklo_epi64(row0, row8) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes), _mm_unpackhi_epi64(row0, row8) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes), _mm_unpacklo_epi64(row1, row9) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes), _mm_unpackhi_epi64(row1, row9) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes), _mm_unpacklo_epi64(row2, rowA) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes), _mm_unpackhi_epi64(row2, rowA) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes), _mm_unpacklo_epi64(row3, rowB) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes), _mm_unpackhi_epi64(row3, rowB) );

				__m128i row4 = _mm_load_si128( src16s + 0x4 );
				__m128i row5 = _mm_load_si128( src16s + 0x5 );
				__m128i row6 = _mm_load_si128( src16s + 0x6 );
				__m128i row7 = _mm_load_si128( src16s + 0x7 );
				__m128i rowC = _mm_load_si128( src16s + 0xC );
				__m128i rowD = _mm_load_si128( src16s + 0xD );
				__m128i rowE = _mm_load_si128( src16s + 0xE );
				__m128i rowF = _mm_load_si128( src16s + 0xF );
				row4 = _mm_shuffle_epi32( row4, _MM_SHUFFLE(3,1,2,0) );
				row5 = _mm_shuffle_epi32( row5, _MM_SHUFFLE(3,1,2,0) );
				row6 = _mm_shuffle_epi32( row6, _MM_SHUFFLE(3,1,2,0) );
				row7 = _mm_shuffle_epi32( row7, _MM_SHUFFLE(3,1,2,0) );
				rowC = _mm_shuffle_epi32( rowC, _MM_SHUFFLE(3,1,2,0) );
				rowD = _mm_shuffle_epi32( rowD, _MM_SHUFFLE(3,1,2,0) );
				rowE = _mm_shuffle_epi32( rowE, _MM_SHUFFLE(3,1,2,0) );
				rowF = _mm_shuffle_epi32( rowF, _MM_SHUFFLE(3,1,2,0) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 0*destRowPitchBytes), _mm_unpacklo_epi64(row4, rowC) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 1*destRowPitchBytes), _mm_unpackhi_epi64(row4, rowC) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 2*destRowPitchBytes), _mm_unpacklo_epi64(row5, rowD) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 3*destRowPitchBytes), _mm_unpackhi_epi64(row5, rowD) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 0*destRowPitchBytes), _mm_unpacklo_epi64(row6, rowE) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 1*destRowPitchBytes), _mm_unpackhi_epi64(row6, rowE) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 2*destRowPitchBytes), _mm_unpacklo_epi64(row7, rowF) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 3*destRowPitchBytes), _mm_unpackhi_epi64(row7, rowF) );
				src16s += 0x10;
				destBytes += 4*destRowPitchBytes;
			}
			while(--loopCount);
		}

		/** @brief Tiles an 8x8x4 microtile of an 32bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcRowPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch Number of elements in one row of source data.
		*/
		inline void tile32bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcRowPitch, const uint32_t srcSlicePitch)
		{
			const uint8_t * srcBytes  = (const uint8_t*)srcTileBase;
			__m128i * dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcRowPitchBytes   = srcRowPitch  *sizeof(uint32_t);
			const uint32_t srcSlicePitchBytes = srcSlicePitch*sizeof(uint32_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row00a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16) );
				__m128i row00b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16) );
				__m128i row01a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16) );
				__m128i row01b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16) );
				__m128i row10a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16) );
				__m128i row10b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16) );
				__m128i row11a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16) );
				__m128i row11b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16) );
				_mm_store_si128( dest16s + 0x00, _mm_unpacklo_epi64(row00a, row01a) );
				_mm_store_si128( dest16s + 0x01, _mm_unpackhi_epi64(row00a, row01a) );
				_mm_store_si128( dest16s + 0x02, _mm_unpacklo_epi64(row10a, row11a) );
				_mm_store_si128( dest16s + 0x03, _mm_unpackhi_epi64(row10a, row11a) );
				_mm_store_si128( dest16s + 0x10, _mm_unpacklo_epi64(row00b, row01b) );
				_mm_store_si128( dest16s + 0x11, _mm_unpackhi_epi64(row00b, row01b) );
				_mm_store_si128( dest16s + 0x12, _mm_unpacklo_epi64(row10b, row11b) );
				_mm_store_si128( dest16s + 0x13, _mm_unpackhi_epi64(row10b, row11b) );

				__m128i row02a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16) );
				__m128i row02b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16) );
				__m128i row03a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16) );
				__m128i row03b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16) );
				__m128i row12a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16) );
				__m128i row12b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16) );
				__m128i row13a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16) );
				__m128i row13b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16) );
				_mm_store_si128( dest16s + 0x04, _mm_unpacklo_epi64(row02a, row03a) );
				_mm_store_si128( dest16s + 0x05, _mm_unpackhi_epi64(row02a, row03a) );
				_mm_store_si128( dest16s + 0x06, _mm_unpacklo_epi64(row12a, row13a) );
				_mm_store_si128( dest16s + 0x07, _mm_unpackhi_epi64(row12a, row13a) );
				_mm_store_si128( dest16s + 0x14, _mm_unpacklo_epi64(row02b, row03b) );
				_mm_store_si128( dest16s + 0x15, _mm_unpackhi_epi64(row02b, row03b) );
				_mm_store_si128( dest16s + 0x16, _mm_unpacklo_epi64(row12b, row13b) );
				_mm_store_si128( dest16s + 0x17, _mm_unpackhi_epi64(row12b, row13b) );

				__m128i row20a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16) );
				__m128i row20b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16) );
				__m128i row21a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16) );
				__m128i row21b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16) );
				__m128i row30a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16) );
				__m128i row30b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16) );
				__m128i row31a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16) );
				__m128i row31b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16) );
				_mm_store_si128( dest16s + 0x08, _mm_unpacklo_epi64(row20a, row21a) );
				_mm_store_si128( dest16s + 0x09, _mm_unpackhi_epi64(row20a, row21a) );
				_mm_store_si128( dest16s + 0x0A, _mm_unpacklo_epi64(row30a, row31a) );
				_mm_store_si128( dest16s + 0x0B, _mm_unpackhi_epi64(row30a, row31a) );
				_mm_store_si128( dest16s + 0x18, _mm_unpacklo_epi64(row20b, row21b) );
				_mm_store_si128( dest16s + 0x19, _mm_unpackhi_epi64(row20b, row21b) );
				_mm_store_si128( dest16s + 0x1A, _mm_unpacklo_epi64(row30b, row31b) );
				_mm_store_si128( dest16s + 0x1B, _mm_unpackhi_epi64(row30b, row31b) );

				__m128i row22a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16) );
				__m128i row22b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16) );
				__m128i row23a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16) );
				__m128i row23b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 2*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16) );
				__m128i row32a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16) );
				__m128i row32b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16) );
				__m128i row33a = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16) );
				__m128i row33b = _mm_loadu_si128( reinterpret_cast<const __m128i*>(srcBytes + 3*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16) );
				_mm_store_si128( dest16s + 0x0C, _mm_unpacklo_epi64(row22a, row23a) );
				_mm_store_si128( dest16s + 0x0D, _mm_unpackhi_epi64(row22a, row23a) );
				_mm_store_si128( dest16s + 0x0E, _mm_unpacklo_epi64(row32a, row33a) );
				_mm_store_si128( dest16s + 0x0F, _mm_unpackhi_epi64(row32a, row33a) );
				_mm_store_si128( dest16s + 0x1C, _mm_unpacklo_epi64(row22b, row23b) );
				_mm_store_si128( dest16s + 0x1D, _mm_unpackhi_epi64(row22b, row23b) );
				_mm_store_si128( dest16s + 0x1E, _mm_unpacklo_epi64(row32b, row33b) );
				_mm_store_si128( dest16s + 0x1F, _mm_unpackhi_epi64(row32b, row33b) );

				srcBytes += 4*srcRowPitchBytes;
				dest16s += 0x20;
			}
			while(--loopCount);
		}

		/** @brief Detiles an 8x8x4 microtile of an 32bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destRowPitch Number of elements in one row of source data.
			@param[in] destSlicePitch Number of elements in one row of source data.
			*/
		inline void detile32bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destRowPitch, const uint32_t destSlicePitch)
		{
			const __m128i * src16s    = (const __m128i*)srcTileBase;
			uint8_t       * destBytes = (      uint8_t*)destTileBase;
			const uint32_t destRowPitchBytes   = destRowPitch  *sizeof(uint32_t);
			const uint32_t destSlicePitchBytes = destSlicePitch*sizeof(uint32_t);

			int32_t loopCount = 2;
			do
			{
				__m128i row00 = _mm_load_si128( src16s + 0x00 );
				__m128i row01 = _mm_load_si128( src16s + 0x01 );
				__m128i row02 = _mm_load_si128( src16s + 0x02 );
				__m128i row03 = _mm_load_si128( src16s + 0x03 );
				__m128i row10 = _mm_load_si128( src16s + 0x10 );
				__m128i row11 = _mm_load_si128( src16s + 0x11 );
				__m128i row12 = _mm_load_si128( src16s + 0x12 );
				__m128i row13 = _mm_load_si128( src16s + 0x13 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row00, row01) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row10, row11) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row00, row01) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row10, row11) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row02, row03) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row12, row13) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row02, row03) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row12, row13) );

				__m128i row04 = _mm_load_si128( src16s + 0x04 );
				__m128i row05 = _mm_load_si128( src16s + 0x05 );
				__m128i row06 = _mm_load_si128( src16s + 0x06 );
				__m128i row07 = _mm_load_si128( src16s + 0x07 );
				__m128i row14 = _mm_load_si128( src16s + 0x14 );
				__m128i row15 = _mm_load_si128( src16s + 0x15 );
				__m128i row16 = _mm_load_si128( src16s + 0x16 );
				__m128i row17 = _mm_load_si128( src16s + 0x17 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row04, row05) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row14, row15) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row04, row05) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row14, row15) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row06, row07) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row16, row17) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row06, row07) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row16, row17) );

				__m128i row08 = _mm_load_si128( src16s + 0x08 );
				__m128i row09 = _mm_load_si128( src16s + 0x09 );
				__m128i row0A = _mm_load_si128( src16s + 0x0A );
				__m128i row0B = _mm_load_si128( src16s + 0x0B );
				__m128i row18 = _mm_load_si128( src16s + 0x18 );
				__m128i row19 = _mm_load_si128( src16s + 0x19 );
				__m128i row1A = _mm_load_si128( src16s + 0x1A );
				__m128i row1B = _mm_load_si128( src16s + 0x1B );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row08, row09) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row18, row19) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row08, row09) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row18, row19) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row0A, row0B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row1A, row1B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row0A, row0B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row1A, row1B) );

				__m128i row0C = _mm_load_si128( src16s + 0x0C );
				__m128i row0D = _mm_load_si128( src16s + 0x0D );
				__m128i row0E = _mm_load_si128( src16s + 0x0E );
				__m128i row0F = _mm_load_si128( src16s + 0x0F );
				__m128i row1C = _mm_load_si128( src16s + 0x1C );
				__m128i row1D = _mm_load_si128( src16s + 0x1D );
				__m128i row1E = _mm_load_si128( src16s + 0x1E );
				__m128i row1F = _mm_load_si128( src16s + 0x1F );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row0C, row0D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row1C, row1D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row0C, row0D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 2*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row1C, row1D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_unpacklo_epi64(row0E, row0F) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_unpacklo_epi64(row1E, row1F) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_unpackhi_epi64(row0E, row0F) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 3*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_unpackhi_epi64(row1E, row1F) );

				src16s += 0x20;
				destBytes += 4*destRowPitchBytes;
			}
			while(--loopCount);
		}

		/** @brief Tiles an 8x8x4 microtile of an 64bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcRowPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch Number of elements in one row of source data.
		*/
		inline void tile64bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcRowPitch, const uint32_t srcSlicePitch)
		{
			const uint8_t * srcBytes  = (const uint8_t*)srcTileBase;
			__m128i * dest16s         = (      __m128i*)destTileBase;
			const uint32_t srcRowPitchBytes   = srcRowPitch  *sizeof(uint64_t);
			const uint32_t srcSlicePitchBytes = srcSlicePitch*sizeof(uint64_t);

			int32_t loopCount = 2;
			do
			{
				_mm_store_si128( dest16s + 0x00, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x01, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x02, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x03, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x04, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x05, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x06, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x07, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x08, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x09, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x0A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x0B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x0C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x0D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x0E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x0F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16)) );

				_mm_store_si128( dest16s + 0x20, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x21, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x22, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x23, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x24, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x25, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x26, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x27, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x28, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x29, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x2A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x2B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x2C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x2D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x2E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x2F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 3*16)) );

				_mm_store_si128( dest16s + 0x40, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x41, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x42, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x43, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x44, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x45, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x46, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x47, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x48, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x49, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x4A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x4B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x4C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x4D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x4E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x4F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 1*16)) );

				_mm_store_si128( dest16s + 0x60, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x61, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x62, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x63, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x64, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x65, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x66, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x67, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x68, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x69, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x6A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x6B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x6C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x6D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x6E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x6F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 3*16)) );

				srcBytes += 2*srcSlicePitchBytes;
				dest16s += 0x10;
			}
			while(--loopCount);
		}

		/** @brief Detiles an 8x8x4 microtile of an 64bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destRowPitch Number of elements in one row of source data.
			@param[in] destSlicePitch Number of elements in one row of source data.
			*/
		inline void detile64bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destRowPitch, const uint32_t destSlicePitch)
		{
			const __m128i * src16s    = (const __m128i*)srcTileBase;
			uint8_t       * destBytes = (      uint8_t*)destTileBase;
			const uint32_t destRowPitchBytes   = destRowPitch  *sizeof(uint64_t);
			const uint32_t destSlicePitchBytes = destSlicePitch*sizeof(uint64_t);

			int32_t loopCount = 2;
			do
			{
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x00) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x04) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x20) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x24) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x01) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x05) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x21) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x25) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x02) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x06) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x22) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x26) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x03) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x07) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x23) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x27) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x08) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x0C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x28) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x2C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x09) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x0D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x29) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x2D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x0A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x0E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x2A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x2E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x0B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x0F) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x2B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x2F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x40) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x44) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x60) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x64) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x41) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x45) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x61) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x65) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x42) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x46) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x62) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x66) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x43) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x47) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x63) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x67) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x48) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x4C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x68) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x6C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x49) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x4D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x69) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x6D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x4A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x4E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x6A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x6E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x4B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x4F) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x6B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x6F) );

				src16s += 0x10;
				destBytes += 2*destSlicePitchBytes;
			}
			while(--loopCount);
		}

		/** @brief Tiles an 8x8x4 microtile of an 128bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the untiled data.
			@param[in] srcRowPitch Number of elements in one row of source data.
			@param[in] srcSlicePitch Number of elements in one row of source data.
		*/
		inline void tile128bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t srcRowPitch, const uint32_t srcSlicePitch)
		{
			const uint8_t * srcBytes  = (const uint8_t*)srcTileBase;
			__m128i * dest16s       = (      __m128i*)destTileBase;
			const uint32_t srcRowPitchBytes   = srcRowPitch  *sizeof(__m128i);
			const uint32_t srcSlicePitchBytes = srcSlicePitch*sizeof(__m128i);

			int32_t loopCount = 2;
			do
			{
				_mm_store_si128( dest16s + 0x00, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x01, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x02, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x03, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x08, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x09, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x0A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x0B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x04, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x05, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x06, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x07, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x0C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x0D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x0E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x0F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 3*16)) );

				_mm_store_si128( dest16s + 0x10, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x11, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x12, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x13, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x18, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x19, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x1A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x1B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x14, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x15, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x16, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x17, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x1C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x1D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x1E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x1F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 3*16)) );

				_mm_store_si128( dest16s + 0x40, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x41, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x42, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x43, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x48, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x49, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 0*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x4A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x4B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 1*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x44, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x45, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x46, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x47, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x4C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x4D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 0*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x4E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x4F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 1*srcRowPitchBytes + 7*16)) );

				_mm_store_si128( dest16s + 0x50, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x51, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x52, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x53, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x58, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x59, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 2*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x5A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x5B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 3*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x54, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x55, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x56, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0x57, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0x5C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x5D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 2*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0x5E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0x5F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 3*srcRowPitchBytes + 7*16)) );

				_mm_store_si128( dest16s + 0x80, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x81, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x82, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x83, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x88, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x89, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x8A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x8B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x84, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x85, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x86, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x87, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x8C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x8D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x8E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x8F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 3*16)) );

				_mm_store_si128( dest16s + 0x90, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x91, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x92, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x93, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x98, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x99, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x9A, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x9B, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x94, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x95, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x96, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 0*16)) );
				_mm_store_si128( dest16s + 0x97, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 1*16)) );
				_mm_store_si128( dest16s + 0x9C, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x9D, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 3*16)) );
				_mm_store_si128( dest16s + 0x9E, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 2*16)) );
				_mm_store_si128( dest16s + 0x9F, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 3*16)) );

				_mm_store_si128( dest16s + 0xC0, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xC1, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xC2, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xC3, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xC8, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xC9, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 4*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0xCA, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xCB, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 5*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0xC4, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xC5, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xC6, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xC7, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xCC, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xCD, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 4*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0xCE, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xCF, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 5*srcRowPitchBytes + 7*16)) );

				_mm_store_si128( dest16s + 0xD0, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xD1, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xD2, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xD3, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xD8, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xD9, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 6*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0xDA, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xDB, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 0*srcSlicePitchBytes + 7*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0xD4, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xD5, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xD6, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 4*16)) );
				_mm_store_si128( dest16s + 0xD7, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 5*16)) );
				_mm_store_si128( dest16s + 0xDC, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xDD, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 6*srcRowPitchBytes + 7*16)) );
				_mm_store_si128( dest16s + 0xDE, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 6*16)) );
				_mm_store_si128( dest16s + 0xDF, _mm_loadu_si128(reinterpret_cast<const __m128i*>(srcBytes + 1*srcSlicePitchBytes + 7*srcRowPitchBytes + 7*16)) );

				srcBytes += 2*srcSlicePitchBytes;
				dest16s += 0x20;
			}
			while(--loopCount);
		}

		/** @brief Detiles an 8x8x4 microtile of an 128bpp surface, using the Thick microtile mode.
			@param[out] destTileBase Pointer to the beginning of the destination microtile in the untiled data.
			@param[in] srcTileBase Pointer to the beginning of the source microtile in the tiled data. This pointer must be 16-byte aligned.
			@param[in] destRowPitch Number of elements in one row of source data.
			@param[in] destSlicePitch Number of elements in one row of source data.
			*/
		inline void detile128bppThickSse2(void * __restrict destTileBase, const void * __restrict srcTileBase, const uint32_t destRowPitch, const uint32_t destSlicePitch)
		{
			const __m128i * src16s  = (const __m128i*)srcTileBase;
			uint8_t       * destBytes = (      uint8_t*)destTileBase;
			const uint32_t destRowPitchBytes   = destRowPitch  *sizeof(__m128i);
			const uint32_t destSlicePitchBytes = destSlicePitch*sizeof(__m128i);

			int32_t loopCount = 2;
			do
			{
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x00) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x01) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x02) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x03) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x08) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x09) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x0A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x0B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x04) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x05) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x06) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x07) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x0C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x0D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x0E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x0F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x10) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x11) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x12) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x13) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x18) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x19) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x1A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x1B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x14) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x15) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x16) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x17) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x1C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x1D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x1E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x1F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x40) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x41) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x42) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x43) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x48) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 0*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x49) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x4A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 1*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x4B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x44) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x45) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x46) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x47) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x4C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 0*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x4D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x4E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 1*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x4F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x50) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x51) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x52) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x53) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x58) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 2*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x59) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x5A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 3*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x5B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x54) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x55) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0x56) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0x57) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x5C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 2*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x5D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0x5E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 3*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0x5F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x80) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x81) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x82) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x83) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x88) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x89) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x8A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x8B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x84) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x85) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x86) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x87) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x8C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x8D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x8E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x8F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x90) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x91) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x92) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x93) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x98) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x99) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x9A) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x9B) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x94) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x95) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 0*16), _mm_load_si128(src16s + 0x96) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 1*16), _mm_load_si128(src16s + 0x97) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x9C) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x9D) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 2*16), _mm_load_si128(src16s + 0x9E) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 3*16), _mm_load_si128(src16s + 0x9F) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xC0) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xC1) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xC2) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xC3) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xC8) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 4*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xC9) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xCA) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 5*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xCB) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xC4) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xC5) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xC6) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xC7) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xCC) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 4*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xCD) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xCE) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 5*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xCF) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xD0) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xD1) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xD2) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xD3) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xD8) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 6*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xD9) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xDA) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 0*destSlicePitchBytes + 7*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xDB) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xD4) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xD5) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 4*16), _mm_load_si128(src16s + 0xE6) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 5*16), _mm_load_si128(src16s + 0xD7) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xDC) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 6*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xDD) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 6*16), _mm_load_si128(src16s + 0xDE) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>(destBytes + 1*destSlicePitchBytes + 7*destRowPitchBytes + 7*16), _mm_load_si128(src16s + 0xDF) );

				src16s += 0x20;
				destBytes += 2*destSlicePitchBytes;
			}
			while(--loopCount);
		}
	}
}
