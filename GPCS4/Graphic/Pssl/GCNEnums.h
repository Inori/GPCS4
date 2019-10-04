#pragma once

#include "GPCS4Common.h"

namespace pssl
{;

enum class SpirvGprType : uint32_t
{
	Scalar = 0,
	Vector = 1,
};

enum class SpirvResourceType : uint32_t
{
	VSharp = 0,  // V#
	TSharp = 1,  // T#
	SSharp = 2   // S#
};

/**
 * \brief Scalar value type
 *
 * Enumerates possible register component
 * types. Scalar types are represented as
 * a one-component vector type.
 */
enum class SpirvScalarType : uint32_t
{
	Uint32 = 0,
	Uint64 = 1,
	Sint32 = 2,
	Sint64 = 3,
	Float32 = 4,
	Float64 = 5,
	Bool = 6,
	Unknown = 7
};

enum class SpirvZeroTest : uint32_t
{
	TestZ = 0,
	TestNz = 1,
};


}  // namespace pssl

