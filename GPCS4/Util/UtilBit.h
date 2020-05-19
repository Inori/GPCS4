#pragma once

#include "GPCS4Common.h"
#include "UtilLikely.h"

#ifndef _MSC_VER
#include <x86intrin.h>
#else
#include <intrin.h>
#endif


namespace bit {
  
  template<typename T>
  T extract(T value, uint32_t lst, uint32_t fst) {
    return (value >> fst) & ~(~T(0) << (lst - fst + 1));
  }

  inline uint32_t popcntStep(uint32_t n, uint32_t mask, uint32_t shift) {
    return (n & mask) + ((n & ~mask) >> shift);
  }
  
  inline uint32_t popcnt(uint32_t n) {
    n = popcntStep(n, 0x55555555, 1);
    n = popcntStep(n, 0x33333333, 2);
    n = popcntStep(n, 0x0F0F0F0F, 4);
    n = popcntStep(n, 0x00FF00FF, 8);
    n = popcntStep(n, 0x0000FFFF, 16);
    return n;
  }

#ifndef _tzcnt_u32
  extern unsigned int _tzcnt_u32(unsigned int);
#endif  // !_tzcnt_u32

  inline uint32_t tzcnt(uint32_t n) {
    #if defined(_MSC_VER)
    return _tzcnt_u32(n);
    #elif defined(__BMI__)
    return __tzcnt_u32(n);
    #elif defined(__GNUC__)
    uint32_t res;
    uint32_t tmp;
    asm (
      "mov  $32, %1;"
      "bsf   %2, %0;"
      "cmovz %1, %0;"
      : "=&r" (res), "=&r" (tmp)
      : "r" (n));
    return res;
    #else
    uint32_t r = 31;
    n &= -n;
    r -= (n & 0x0000FFFF) ? 16 : 0;
    r -= (n & 0x00FF00FF) ?  8 : 0;
    r -= (n & 0x0F0F0F0F) ?  4 : 0;
    r -= (n & 0x33333333) ?  2 : 0;
    r -= (n & 0x55555555) ?  1 : 0;
    return n != 0 ? r : 32;
    #endif
  }


  template<typename T>
  uint32_t pack(T& dst, uint32_t& shift, T src, uint32_t count) {
    constexpr uint32_t Bits = 8 * sizeof(T);
    if (likely(shift < Bits))
      dst |= src << shift;
    shift += count;
    return shift > Bits ? shift - Bits : 0;
  }

  template<typename T>
  uint32_t unpack(T& dst, T src, uint32_t& shift, uint32_t count) {
    constexpr uint32_t Bits = 8 * sizeof(T);
    if (likely(shift < Bits))
      dst = (src >> shift) & ((T(1) << count) - 1);
    shift += count;
    return shift > Bits ? shift - Bits : 0;
  }

  /**
   * \brief Compares two aligned structs bit by bit
   *
   * \param [in] a First struct
   * \param [in] b Second struct
   * \returns \c true if the structs are equal
   */
  template <typename T>
  bool bcmpeq(const T* a, const T* b)
  {
	  static_assert(alignof(T) >= 16);
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
	  auto ai = reinterpret_cast<const __m128i*>(a);
	  auto bi = reinterpret_cast<const __m128i*>(b);

	  size_t i = 0;

#if defined(__clang__)
#pragma nounroll
#elif defined(__GNUC__)
#pragma GCC unroll 0
#endif

	  for (; i < 2 * (sizeof(T) / 32); i += 2)
	  {
		  __m128i eq0 = _mm_cmpeq_epi8(
			  _mm_load_si128(ai + i),
			  _mm_load_si128(bi + i));
		  __m128i eq1 = _mm_cmpeq_epi8(
			  _mm_load_si128(ai + i + 1),
			  _mm_load_si128(bi + i + 1));
		  __m128i eq = _mm_and_si128(eq0, eq1);

		  int mask = _mm_movemask_epi8(eq);
		  if (mask != 0xFFFF)
			  return false;
	  }

	  for (; i < sizeof(T) / 16; i++)
	  {
		  __m128i eq = _mm_cmpeq_epi8(
			  _mm_load_si128(ai + i),
			  _mm_load_si128(bi + i));

		  int mask = _mm_movemask_epi8(eq);
		  if (mask != 0xFFFF)
			  return false;
	  }

	  return true;
#else
	  return !std::memcmp(a, b, sizeof(T));
#endif
  }
  
}