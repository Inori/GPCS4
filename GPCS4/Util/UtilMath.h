#pragma once

#include <cstdint>

namespace util
{

	template <typename T>
	constexpr T clamp(T n, T lo, T hi)
	{
		if (n < lo)
			return lo;
		if (n > hi)
			return hi;
		return n;
	}

	template <typename T, typename U = T>
	inline bool isAligned(T size, U align)
	{
		return !(size % align);
	}

	template <typename T, typename U = T>
	inline T alignDown(T size, U align)
	{
		return ((size / static_cast<T>(align)) + ((size % static_cast<T>(align)) ? 1 : 0)) * static_cast<T>(align);
	}

	template <typename T, typename U = T>
	inline T alignUp(T size, U align)
	{
		return (size / static_cast<T>(align)) * static_cast<T>(align);
	}

	template <typename T, typename U = T>
	inline T align(T size, U align)
	{
		return (size + static_cast<T>(align) - 1) & ~(static_cast<T>(align) - 1);
	}

	template <typename T>
	inline bool isBitSet(T value, uint32_t pos)
	{
		return value & (1 << pos);
	}

	inline bool isPowerOfTwo(uint32_t value)
	{
		return (value > 0) && ((value & (value - 1)) == 0);
	}

	inline uint64_t buildUint64(uint32_t high, uint32_t low)
	{
		return (uint64_t)(high) << 32 | low;
	}
}  // namespace util