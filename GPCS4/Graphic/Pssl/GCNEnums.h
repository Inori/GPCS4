#pragma once

#include "PsslCommon.h"

namespace pssl
{;

constexpr uint32_t InvalidSpvId = 0;

enum class SpirvGprType : uint32_t
{
	Scalar = 0,
	Vector = 1,
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


enum class SpirvSystemValue : uint32_t
{
	None                        = 0,
	Position                    = 1,
	ClipDistance                = 2,
	CullDistance                = 3,
	RenderTargetId              = 4,
	ViewportId                  = 5,
	VertexId                    = 6,
	PrimitiveId                 = 7,
	InstanceId                  = 8,
	IsFrontFace                 = 9,
	SampleIndex                 = 10,
	FinalQuadUeq0EdgeTessFactor = 11,
	FinalQuadVeq0EdgeTessFactor = 12,
	FinalQuadUeq1EdgeTessFactor = 13,
	FinalQuadVeq1EdgeTessFactor = 14,
	FinalQuadUInsideTessFactor  = 15,
	FinalQuadVInsideTessFactor  = 16,
	FinalTriUeq0EdgeTessFactor  = 17,
	FinalTriVeq0EdgeTessFactor  = 18,
	FinalTriWeq0EdgeTessFactor  = 19,
	FinalTriInsideTessFactor    = 20,
	FinalLineDetailTessFactor   = 21,
	FinalLineDensityTessFactor  = 22,
	Target                      = 64,
	Depth                       = 65,
	Coverage                    = 66,
	DepthGe                     = 67,
	DepthLe                     = 68
};


}  // namespace pssl

