#pragma once

#include <xhash>

namespace util
{
	template <typename T>
	inline size_t hash(const T& value)
	{
		return std::hash<T>{}(value);
	}

	inline size_t hash_combine(std::size_t digestA, std::size_t digestB)
	{
		// see: boost/container_hash/hash.hpp
		// The constant is the N-bits reciprocal of the golden ratio:
		//  phi = (1 + sqrt(5)) / 2
#if SIZE_MAX == UINT64_MAX
		//  trunc(2^64 / phi) = 0x9e3779b97f4a7c15
		digestA ^= digestB + 0x9e3779b97f4a7c15 + (digestA << 12) + (digestA >> 4);
#else
		//  trunc(2^32 / phi) = 0x9e3779b9
		digestA ^= digestB + 0x9e3779b9 + (digestA << 6) + (digestA >> 2);
#endif
		return digestA;
	}

	// Hashes `value` and combines the resulting digest into the existing digest.
	// Use instead of `hash_combine` if `value` is not another digest.
	template <typename T>
	inline size_t rehash(std::size_t digest, const T& value)
	{
		return hash_combine(digest, hash(value));
	}
}  // namespace util

namespace std
{
	// Hashing pairs is often useful
	template <typename T1, typename T2>
	struct hash<pair<T1, T2>>
	{
		size_t operator()(const pair<T1, T2>& p) const
		{
			auto digest = util::hash(p.first);
			return util::rehash(digest, p.second);
		}
	};

}  // namespace std