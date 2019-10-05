#pragma once

#ifndef _MSC_VER
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

#include "GPCS4Common.h"
#include "LikelyHelper.h"

namespace bit {
  
  template<typename T>
  T extract(T value, uint32_t fst, uint32_t lst) {
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

// TODO:
// This is an ugly hack, I don't know why I can't compile when using clang 9
// if declare this function.
#if (__clang_major__ < 9)
  extern unsigned int _tzcnt_u32(unsigned int);
#endif // __clang_major__ < 9

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
  
}