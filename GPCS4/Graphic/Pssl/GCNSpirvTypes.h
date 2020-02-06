#pragma once

#include "PsslCommon.h"
#include "GCNEnums.h"

namespace pssl
{;

/**
 * \brief Vector type
 *
 * Convenience struct that stores a scalar
 * type and a component count. The compiler
 * can use this to generate SPIR-V types.
 */
struct SpirvVectorType
{
	SpirvVectorType() :
		ctype(SpirvScalarType::Unknown), ccount(0)
	{
	}
	SpirvVectorType(SpirvScalarType type, uint32_t count) :
		ctype(type), ccount(count)
	{
	}

	SpirvScalarType ctype;
	uint32_t ccount;
};

/**
 * \brief Array type
 *
 * Convenience struct that stores a scalar type, a
 * component count and an array size. An array of
 * length 0 will be evaluated to a vector type. The
 * compiler can use this to generate SPIR-V types.
 */
struct SpirvArrayType
{
	SpirvArrayType() :
		alength(0)
	{
	}

	SpirvArrayType(const SpirvVectorType& type, uint32_t alen) :
		vtype(type), alength(alen)
	{
	}

	SpirvArrayType(SpirvScalarType type, uint32_t count, uint32_t alen) :
		vtype(type, count), alength(alen)
	{
	}

	SpirvVectorType vtype;
	uint32_t alength;
};

/**
 * \brief Register info
 *
 * Stores the array type of a register and
 * its storage class. The compiler can use
 * this to generate SPIR-V pointer types.
 */
struct SpirvRegisterInfo
{
	SpirvRegisterInfo() :
		sclass(spv::StorageClassMax)
	{
	}

	SpirvRegisterInfo(const SpirvVectorType& type, spv::StorageClass cls) :
		atype(type, 0), sclass(cls)
	{
	}

	SpirvRegisterInfo(const SpirvArrayType& type, spv::StorageClass cls) :
		atype(type), sclass(cls)
	{
	}

	SpirvRegisterInfo(SpirvScalarType type,
					  uint32_t count, uint32_t alen,
					  spv::StorageClass cls) :
		atype(type, count, alen),
		sclass(cls)
	{
	}

	SpirvArrayType atype;
	spv::StorageClass sclass;
};

/**
 * \brief Register value
 *
 * Stores a vector type and a SPIR-V ID that
 * represents an intermediate value. This is
 * used to track the type of such values.
 */
struct SpirvRegisterValue
{
	SpirvRegisterValue() :
		id(InvalidSpvId)
	{
	}
	SpirvRegisterValue(SpirvVectorType vType, uint32_t spvId) :
		type(vType), id(spvId)
	{
	}
	SpirvRegisterValue(SpirvScalarType sType, uint32_t count, uint32_t spvId) :
		type(sType, count), id(spvId)
	{
	}

	SpirvVectorType type;
	uint32_t id;
};

/**
 * \brief Register pointer
 *
 * Stores a vector type and a SPIR-V ID that
 * represents a pointer to such a vector. This
 * can be used to load registers conveniently.
 */
struct SpirvRegisterPointer
{
	SpirvRegisterPointer() :
		id(InvalidSpvId)
	{
	}
	SpirvRegisterPointer(SpirvVectorType vType, uint32_t spvId) :
		type(vType), id(spvId)
	{
	}
	SpirvRegisterPointer(SpirvScalarType sType, uint32_t count, uint32_t spvId) :
		type(sType, count), id(spvId)
	{
	}

	SpirvVectorType type;
	uint32_t id;
};

/**
 * \brief Literal Constant
 *
 * A single literal constant
 * 
 */
struct SpirvLiteralConstant
{
	SpirvLiteralConstant() :
		literalConst(0)
	{
	}
	SpirvLiteralConstant(SpirvScalarType sType, uint32_t value) :
		type(sType), literalConst(value)
	{
	}

	SpirvScalarType type;
	// Note: this may need to cast to proper type
	// depending on the type member
	uint32_t literalConst;
};

/**
 * \brief GPR array
 *
 * SGPR or VGPR array/group
 * e.g. s[8:11], v[4:6] etc.
 * Useful when access #V #T #S buffers
 */
struct SpirvGprArray
{
	SpirvGprType type;
	uint32_t startIndex;
	uint32_t count;
};

/**
 * \brief Sampler binding
 *
 * Stores a sampler variable that can be
 * used together with a texture resource.
 */
struct SpirvSampler
{
	uint32_t varId  = 0;
	uint32_t typeId = 0;
};

/**
 * \brief Image type information
 */
struct SpirvImageInfo
{
	spv::Dim dim            = spv::Dim1D;
	uint32_t array          = 0;
	uint32_t ms             = 0;
	uint32_t sampled        = 0;
	spv::ImageFormat format = spv::ImageFormatUnknown;
};

/**
 * \brief Shader resource binding
 *
 * Stores a resource variable
 * and associated type IDs.
 */
struct SpirvTexture
{
	SpirvImageInfo imageInfo;
	uint32_t varId       = 0;
	uint32_t imageTypeId = 0;
};

}  // namespace pssl