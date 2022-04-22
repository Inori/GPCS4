#pragma once

#include "GcnCommon.h"

namespace sce::gcn
{
	constexpr size_t GcnMaxInterfaceRegs = 32;
	constexpr size_t GcnMaxSGPR          = 104;
	constexpr size_t GcnMaxVGPR          = 256;
	constexpr size_t GcnCodeVGPR0        = 256;

	enum class GcnZeroTest : uint32_t
	{
		TestZ  = 0,
		TestNz = 1,
	};

	/**
	 * \brief Scalar value type
	 *
	 * Enumerates possible register component
	 * types. Scalar types are represented as
	 * a one-component vector type.
	 */
	enum class GcnScalarType : uint32_t
	{
		Uint32  = 0,
		Uint64  = 1,
		Sint32  = 2,
		Sint64  = 3,
		Float32 = 4,
		Float64 = 5,
		Bool    = 6,
	};

	/**
	 * \brief Vector type
	 *
	 * Convenience struct that stores a scalar
	 * type and a component count. The compiler
	 * can use this to generate SPIR-V types.
	 */
	struct GcnVectorType
	{
		GcnScalarType ctype;
		uint32_t      ccount;
	};

	/**
	 * \brief Array type
	 *
	 * Convenience struct that stores a scalar type, a
	 * component count and an array size. An array of
	 * length 0 will be evaluated to a vector type. The
	 * compiler can use this to generate SPIR-V types.
	 */
	struct GcnArrayType
	{
		GcnScalarType ctype;
		uint32_t      ccount;
		uint32_t      alength;
	};

	/**
	 * \brief Register info
	 *
	 * Stores the array type of a register and
	 * its storage class. The compiler can use
	 * this to generate SPIR-V pointer types.
	 */
	struct GcnRegisterInfo
	{
		GcnArrayType      type;
		spv::StorageClass sclass;
	};

	/**
	 * \brief Register value
	 *
	 * Stores a vector type and a SPIR-V ID that
	 * represents an intermediate value. This is
	 * used to track the type of such values.
	 */
	struct GcnRegisterValue
	{
		GcnVectorType type;
		uint32_t      id;
	};

	/**
	 * \brief Register pointer
	 *
	 * Stores a vector type and a SPIR-V ID that
	 * represents a pointer to such a vector. This
	 * can be used to load registers conveniently.
	 */
	struct GcnRegisterPointer
	{
		GcnVectorType type;
		uint32_t      id = 0;
	};

	/**
	 * \brief Vertex shader-specific structure
	 */
	struct GcnCompilerVsPart
	{
		uint32_t functionId  = 0;
		uint32_t fetchFuncId = 0;

		uint32_t builtinVertexId     = 0;
		uint32_t builtinInstanceId   = 0;
		uint32_t builtinBaseVertex   = 0;
		uint32_t builtinBaseInstance = 0;
	};

	/**
	 * \brief Pixel shader-specific structure
	 */
	struct GcnCompilerPsPart
	{
		uint32_t functionId = 0;

		uint32_t builtinFragCoord     = 0;
		uint32_t builtinDepth         = 0;
		uint32_t builtinStencilRef    = 0;
		uint32_t builtinIsFrontFace   = 0;
		uint32_t builtinSampleId      = 0;
		uint32_t builtinSampleMaskIn  = 0;
		uint32_t builtinSampleMaskOut = 0;
		uint32_t builtinLayer         = 0;
		uint32_t builtinViewportId    = 0;

		uint32_t builtinLaneId = 0;
		uint32_t killState     = 0;

		uint32_t specRsSampleCount = 0;
	};

	/**
	 * \brief Compute shader-specific structure
	 */
	struct GcnCompilerCsPart
	{
		uint32_t functionId = 0;

		uint32_t workgroupSizeX = 0;
		uint32_t workgroupSizeY = 0;
		uint32_t workgroupSizeZ = 0;

		uint32_t builtinGlobalInvocationId   = 0;
		uint32_t builtinLocalInvocationId    = 0;
		uint32_t builtinLocalInvocationIndex = 0;
		uint32_t builtinWorkgroupId          = 0;
	};


	/**
	 * \brief Image type information
	 */
	struct GcnImageInfo
	{
		spv::Dim        dim     = spv::Dim1D;
		uint32_t        depth   = 0;
		uint32_t        array   = 0;
		uint32_t        ms      = 0;
		uint32_t        sampled = 0;

		VkImageViewType vtype = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
	};

	/**
	 * \brief Constant buffer binding
	 *
	 * Stores information for a V#
	 */
	struct GcnBuffer
	{
		uint32_t varId = 0;
		uint32_t size  = 0;
		bool     asSsbo = false;
	};

	/**
	 * \brief Sampler binding
	 *
	 * Stores information for a S#
	 */
	struct GcnSampler
	{
		uint32_t varId  = 0;
		uint32_t typeId = 0;
	};

	/**
	 * \brief Shader resource binding
	 *
	 * Stores information for a T#
	 */
	struct GcnTexture
	{
		uint32_t      varId         = 0;
		GcnImageInfo  imageInfo     = {};
		GcnScalarType sampledType   = GcnScalarType::Float32;
		uint32_t      sampledTypeId = 0;
		uint32_t      imageTypeId   = 0;
		uint32_t      colorTypeId   = 0;
		uint32_t      depthTypeId   = 0;
	};

}  // namespace sce::gcn