#pragma once

#include "GnmCommon.h"
#include <intrin.h>

namespace GpuAddress
{;

typedef void (*MicroTileFunc)(void* __restrict destTileBase, const void* __restrict srcTileBase, const uint32_t srcPitch, const uint32_t srcSlicePitch);

inline void detile8bppThinSse2(void* __restrict destTileBase, const void* __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
{
	const __m128i* src16s         = (const __m128i*)srcTileBase;
	uint8_t* destBytes            = (uint8_t*)destTileBase;
	const uint32_t destPitchBytes = destPitch * sizeof(uint8_t);

	__m128i row01 = _mm_load_si128(src16s + 0x0);
	__m128i row23 = _mm_load_si128(src16s + 0x1);
	__m128i row45 = _mm_load_si128(src16s + 0x2);
	__m128i row67 = _mm_load_si128(src16s + 0x3);

	__m128i tmp0 = _mm_unpacklo_epi64(row01, row23);
	__m128i tmp1 = _mm_unpackhi_epi64(row01, row23);
	__m128i tmp2 = _mm_unpacklo_epi64(row45, row67);
	__m128i tmp3 = _mm_unpackhi_epi64(row45, row67);

	__m128i out01 = _mm_shuffle_epi32(_mm_shufflehi_epi16(_mm_shufflelo_epi16(tmp0, _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i out23 = _mm_shuffle_epi32(_mm_shufflehi_epi16(_mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i out45 = _mm_shuffle_epi32(_mm_shufflehi_epi16(_mm_shufflelo_epi16(tmp2, _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i out67 = _mm_shuffle_epi32(_mm_shufflehi_epi16(_mm_shufflelo_epi16(tmp3, _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0));

	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes), out01);
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes), _mm_srli_si128(out01, 8));
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes), out23);
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes), _mm_srli_si128(out23, 8));
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 4 * destPitchBytes), out45);
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 5 * destPitchBytes), _mm_srli_si128(out45, 8));
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 6 * destPitchBytes), out67);
	_mm_storel_epi64(reinterpret_cast<__m128i*>(destBytes + 7 * destPitchBytes), _mm_srli_si128(out67, 8));
}

inline void detile16bppThinSse2(void* __restrict destTileBase, const void* __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
{
	const __m128i* src16s         = (const __m128i*)srcTileBase;
	uint8_t* destBytes            = (uint8_t*)destTileBase;
	const uint32_t destPitchBytes = destPitch * sizeof(uint16_t);

	__m128i row0 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x0), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row1 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x1), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row2 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x2), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row3 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x3), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row4 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x4), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row5 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x5), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row6 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x6), _MM_SHUFFLE(3, 1, 2, 0));
	__m128i row7 = _mm_shuffle_epi32(_mm_load_si128(src16s + 0x7), _MM_SHUFFLE(3, 1, 2, 0));

	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes), _mm_unpacklo_epi64(row0, row2));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes), _mm_unpackhi_epi64(row0, row2));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes), _mm_unpacklo_epi64(row1, row3));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes), _mm_unpackhi_epi64(row1, row3));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 4 * destPitchBytes), _mm_unpacklo_epi64(row4, row6));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 5 * destPitchBytes), _mm_unpackhi_epi64(row4, row6));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 6 * destPitchBytes), _mm_unpacklo_epi64(row5, row7));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 7 * destPitchBytes), _mm_unpackhi_epi64(row5, row7));
}

inline void detile32bppThinSse2(void* __restrict destTileBase, const void* __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
{
	const __m128i* src16s         = (const __m128i*)srcTileBase;
	uint8_t* destBytes            = (uint8_t*)destTileBase;
	const uint32_t destPitchBytes = destPitch * sizeof(uint32_t);

	__m128i row00 = _mm_load_si128(src16s + 0x0);
	__m128i row01 = _mm_load_si128(src16s + 0x1);
	__m128i row10 = _mm_load_si128(src16s + 0x2);
	__m128i row11 = _mm_load_si128(src16s + 0x3);
	__m128i row20 = _mm_load_si128(src16s + 0x4);
	__m128i row21 = _mm_load_si128(src16s + 0x5);
	__m128i row30 = _mm_load_si128(src16s + 0x6);
	__m128i row31 = _mm_load_si128(src16s + 0x7);
	__m128i row40 = _mm_load_si128(src16s + 0x8);
	__m128i row41 = _mm_load_si128(src16s + 0x9);
	__m128i row50 = _mm_load_si128(src16s + 0xA);
	__m128i row51 = _mm_load_si128(src16s + 0xB);
	__m128i row60 = _mm_load_si128(src16s + 0xC);
	__m128i row61 = _mm_load_si128(src16s + 0xD);
	__m128i row70 = _mm_load_si128(src16s + 0xE);
	__m128i row71 = _mm_load_si128(src16s + 0xF);

	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 0 * 16), _mm_unpacklo_epi64(row00, row01));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 1 * 16), _mm_unpacklo_epi64(row20, row21));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 0 * 16), _mm_unpackhi_epi64(row00, row01));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 1 * 16), _mm_unpackhi_epi64(row20, row21));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 0 * 16), _mm_unpacklo_epi64(row10, row11));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 1 * 16), _mm_unpacklo_epi64(row30, row31));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 0 * 16), _mm_unpackhi_epi64(row10, row11));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 1 * 16), _mm_unpackhi_epi64(row30, row31));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 4 * destPitchBytes + 0 * 16), _mm_unpacklo_epi64(row40, row41));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 4 * destPitchBytes + 1 * 16), _mm_unpacklo_epi64(row60, row61));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 5 * destPitchBytes + 0 * 16), _mm_unpackhi_epi64(row40, row41));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 5 * destPitchBytes + 1 * 16), _mm_unpackhi_epi64(row60, row61));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 6 * destPitchBytes + 0 * 16), _mm_unpacklo_epi64(row50, row51));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 6 * destPitchBytes + 1 * 16), _mm_unpacklo_epi64(row70, row71));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 7 * destPitchBytes + 0 * 16), _mm_unpackhi_epi64(row50, row51));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 7 * destPitchBytes + 1 * 16), _mm_unpackhi_epi64(row70, row71));
}

inline void detile64bppThinSse2(void* __restrict destTileBase, const void* __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
{
	const __m128i* src16s         = (const __m128i*)srcTileBase;
	uint8_t* destBytes            = (uint8_t*)destTileBase;
	const uint32_t destPitchBytes = destPitch * sizeof(uint64_t);

	int32_t loopCount = 2;
	do
	{
		__m128i row00 = _mm_load_si128(src16s + 0x0);
		__m128i row01 = _mm_load_si128(src16s + 0x1);
		__m128i row02 = _mm_load_si128(src16s + 0x2);
		__m128i row03 = _mm_load_si128(src16s + 0x3);
		__m128i row10 = _mm_load_si128(src16s + 0x4);
		__m128i row11 = _mm_load_si128(src16s + 0x5);
		__m128i row12 = _mm_load_si128(src16s + 0x6);
		__m128i row13 = _mm_load_si128(src16s + 0x7);
		__m128i row20 = _mm_load_si128(src16s + 0x8);
		__m128i row21 = _mm_load_si128(src16s + 0x9);
		__m128i row22 = _mm_load_si128(src16s + 0xA);
		__m128i row23 = _mm_load_si128(src16s + 0xB);
		__m128i row30 = _mm_load_si128(src16s + 0xC);
		__m128i row31 = _mm_load_si128(src16s + 0xD);
		__m128i row32 = _mm_load_si128(src16s + 0xE);
		__m128i row33 = _mm_load_si128(src16s + 0xF);
		src16s += 0x10;

		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 0 * 16), row00);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 1 * 16), row02);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 2 * 16), row20);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 3 * 16), row22);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 0 * 16), row01);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 1 * 16), row03);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 2 * 16), row21);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 3 * 16), row23);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 0 * 16), row10);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 1 * 16), row12);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 2 * 16), row30);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 3 * 16), row32);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 0 * 16), row11);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 1 * 16), row13);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 2 * 16), row31);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 3 * 16), row33);
		destBytes += 4 * destPitchBytes;
	} while (--loopCount);
}

inline void detile128bppThinSse2(void* __restrict destTileBase, const void* __restrict srcTileBase, const uint32_t destPitch, const uint32_t destSlicePitch)
{
	const __m128i* src16s         = (const __m128i*)srcTileBase;
	uint8_t* destBytes            = (uint8_t*)destTileBase;
	const uint32_t destPitchBytes = destPitch * sizeof(__m128i);

	int32_t loopCount = 2;
	do
	{
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 0 * 16), _mm_load_si128(src16s + 0x00));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 1 * 16), _mm_load_si128(src16s + 0x01));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 0 * 16), _mm_load_si128(src16s + 0x02));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 1 * 16), _mm_load_si128(src16s + 0x03));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 2 * 16), _mm_load_si128(src16s + 0x04));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 3 * 16), _mm_load_si128(src16s + 0x05));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 2 * 16), _mm_load_si128(src16s + 0x06));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 3 * 16), _mm_load_si128(src16s + 0x07));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 4 * 16), _mm_load_si128(src16s + 0x10));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 5 * 16), _mm_load_si128(src16s + 0x11));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 4 * 16), _mm_load_si128(src16s + 0x12));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 5 * 16), _mm_load_si128(src16s + 0x13));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 6 * 16), _mm_load_si128(src16s + 0x14));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 0 * destPitchBytes + 7 * 16), _mm_load_si128(src16s + 0x15));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 6 * 16), _mm_load_si128(src16s + 0x16));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 1 * destPitchBytes + 7 * 16), _mm_load_si128(src16s + 0x17));

		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 0 * 16), _mm_load_si128(src16s + 0x08));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 1 * 16), _mm_load_si128(src16s + 0x09));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 0 * 16), _mm_load_si128(src16s + 0x0A));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 1 * 16), _mm_load_si128(src16s + 0x0B));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 2 * 16), _mm_load_si128(src16s + 0x0C));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 3 * 16), _mm_load_si128(src16s + 0x0D));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 2 * 16), _mm_load_si128(src16s + 0x0E));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 3 * 16), _mm_load_si128(src16s + 0x0F));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 4 * 16), _mm_load_si128(src16s + 0x18));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 5 * 16), _mm_load_si128(src16s + 0x19));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 4 * 16), _mm_load_si128(src16s + 0x1A));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 5 * 16), _mm_load_si128(src16s + 0x1B));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 6 * 16), _mm_load_si128(src16s + 0x1C));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 2 * destPitchBytes + 7 * 16), _mm_load_si128(src16s + 0x1D));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 6 * 16), _mm_load_si128(src16s + 0x1E));
		_mm_storeu_si128(reinterpret_cast<__m128i*>(destBytes + 3 * destPitchBytes + 7 * 16), _mm_load_si128(src16s + 0x1F));

		src16s += 0x20;
		destBytes += 4 * destPitchBytes;
	} while (--loopCount);
}
}  // namespace GpuAddress
