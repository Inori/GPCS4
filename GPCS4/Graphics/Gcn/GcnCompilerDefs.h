#pragma once

#include "GcnCommon.h"
#include "GcnEnum.h"

namespace sce::gcn
{
	struct GcnInstOperand;

	constexpr size_t   GcnMaxInterfaceRegs = 32;
	constexpr size_t   GcnMaxSGPR          = 104;
	constexpr size_t   GcnMaxVGPR          = 256;
	constexpr size_t   GcnMaxOperandCount  = 5;
	constexpr size_t   GcnMaxExportParam   = 32;

	constexpr size_t   GcnExpPos0          = 12;
	constexpr size_t   GcnExpParam0        = 32;

	enum class GcnZeroTest : uint32_t
	{
		TestZ  = 0,
		TestNz = 1,
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
	 * \brief Value pair
	 * 
	 * Stores value for a pair of sgpr/vgpr
	 * e.g. s[0:1], v[4:5].
	 * 
	 * The high part may be empty/undefined if
	 * it's not needed.
	 * 
	 * If type is float16 or float64,
	 * we use low to save the value.
	 * 
	 */
	struct GcnRegisterValuePair
	{
		GcnRegisterValue low;
		GcnRegisterValue high;
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
	 * \brief Used to index a buffer(V#)
	 */
	struct GcnRegIndex
	{
		uint32_t        regIdx = 0;
		GcnInstOperand* relReg = nullptr;
		int32_t         offset = 0;
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
	 * \brief Image type information
	 */
	struct GcnImageInfo
	{
		spv::Dim dim     = spv::Dim1D;
		uint32_t depth   = 0;
		uint32_t array   = 0;
		uint32_t ms      = 0;
		uint32_t sampled = 0;

		VkImageViewType vtype = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
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


	struct GcnCfgBlock
	{
		uint32_t lableId  = 0;
		size_t   labelPtr = 0;
	};
	

	/**
	 * \brief Component swizzle
	 *
	 * Maps vector components to
	 * other vector components.
	 */
	class GcnRegSwizzle
	{

	public:
		GcnRegSwizzle()
		{
		}
		GcnRegSwizzle(uint32_t x, uint32_t y, uint32_t z, uint32_t w) :
			m_mask((x << 0) | (y << 2) | (z << 4) | (w << 6))
		{
		}

		uint32_t operator[](uint32_t id) const
		{
			return (m_mask >> (id + id)) & 0x3;
		}

		bool operator==(const GcnRegSwizzle& other) const
		{
			return m_mask == other.m_mask;
		}
		bool operator!=(const GcnRegSwizzle& other) const
		{
			return m_mask != other.m_mask;
		}

	private:
		uint8_t m_mask = 0;
	};

	/**
	 * \brief Component mask
	 *
	 * Enables access to certain
	 * subset of vector components.
	 */
	class GcnRegMask
	{

	public:
		GcnRegMask()
		{
		}
		GcnRegMask(uint32_t mask) :
			m_mask(mask)
		{
		}
		GcnRegMask(bool x, bool y, bool z, bool w) :
			m_mask((x ? 0x1 : 0) | (y ? 0x2 : 0) | (z ? 0x4 : 0) | (w ? 0x8 : 0))
		{
		}

		bool operator[](uint32_t id) const
		{
			return (m_mask >> id) & 1;
		}

		uint32_t popCount() const
		{
			const uint8_t n[16] = { 0, 1, 1, 2, 1, 2, 2, 3,
									1, 2, 2, 3, 2, 3, 3, 4 };
			return n[m_mask & 0xF];
		}

		uint32_t firstSet() const
		{
			const uint8_t n[16] = { 4, 0, 1, 0, 2, 0, 1, 0,
									3, 0, 1, 0, 2, 0, 1, 0 };
			return n[m_mask & 0xF];
		}

		uint32_t minComponents() const
		{
			const uint8_t n[16] = { 0, 1, 2, 2, 3, 3, 3, 3,
									4, 4, 4, 4, 4, 4, 4, 4 };
			return n[m_mask & 0xF];
		}

		bool operator==(const GcnRegMask& other) const
		{
			return m_mask == other.m_mask;
		}
		bool operator!=(const GcnRegMask& other) const
		{
			return m_mask != other.m_mask;
		}

		GcnRegMask& operator|=(const GcnRegMask& other)
		{
			m_mask |= other.m_mask;
			return *this;
		}

		static GcnRegMask firstN(uint32_t n)
		{
			return GcnRegMask(n >= 1, n >= 2, n >= 3, n >= 4);
		}

		static GcnRegMask select(uint32_t n)
		{
			return GcnRegMask(n == 0, n == 1, n == 2, n == 3);
		}

		std::string maskString() const
		{
			std::string out = "";
			out += (m_mask & 0x1) ? "x" : "";
			out += (m_mask & 0x2) ? "y" : "";
			out += (m_mask & 0x4) ? "z" : "";
			out += (m_mask & 0x8) ? "w" : "";
			return out;
		}

	private:
		uint8_t m_mask = 0;
	};

}  // namespace sce::gcn