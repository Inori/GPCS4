#pragma once

#include "GcnCommon.h"

#include <vector>

namespace sce::gcn
{
	struct ShaderBinaryInfo
	{
		uint8_t m_signature[7];  // 'OrbShdr'
		uint8_t m_version;       // ShaderBinaryInfoVersion

		unsigned int m_pssl_or_cg : 1;   // 1 = PSSL / Cg, 0 = IL / shtb
		unsigned int m_cached : 1;       // 1 = when compile, debugging source was cached.  May only make sense for PSSL=1
		uint32_t     m_type : 4;         // See enum ShaderBinaryType
		uint32_t     m_source_type : 2;  // See enum ShaderSourceType
		unsigned int m_length : 24;      // Binary code length (does not include this structure or any of its preceding associated tables)

		uint8_t m_chunkUsageBaseOffsetInDW;  // in DW, which starts at ((uint32_t*)&ShaderBinaryInfo) - m_chunkUsageBaseOffsetInDW; max is currently 7 dwords (128 T# + 32 V# + 20 CB V# + 16 UAV T#/V#)
		uint8_t m_numInputUsageSlots;        // Up to 16 user data reg slots + 128 extended user data dwords supported by CUE; up to 16 user data reg slots + 240 extended user data dwords supported by Gnm::InputUsageSlot
		uint8_t m_isSrt : 1;                 // 1 if this shader uses shader resource tables and has an SrtDef table embedded below the input usage table and any extended usage info
		uint8_t m_isSrtUsedInfoValid : 1;    // 1 if SrtDef::m_isUsed=0 indicates an element is definitely unused; 0 if SrtDef::m_isUsed=0 indicates only that the element is not known to be used (m_isUsed=1 always indicates a resource is known to be used)
		uint8_t m_isExtendedUsageInfo : 1;   // 1 if this shader has extended usage info for the InputUsage table embedded below the input usage table
		uint8_t m_reserved2 : 5;             // For future use
		uint8_t m_reserved3;                 // For future use

		uint32_t m_shaderHash0;  // Association hash first 4 bytes
		uint32_t m_shaderHash1;  // Association hash second 4 bytes
		uint32_t m_crc32;        // crc32 of shader + this struct, just up till this field
	};


	struct alignas(4) InputUsageSlot
	{
		uint8_t m_usageType;      ///< From Gnm::ShaderInputUsageType.
		uint8_t m_apiSlot;        ///< API slot or chunk ID.
		uint8_t m_startRegister;  ///< User data slot.

		union
		{
			struct
			{
				uint8_t m_registerCount : 1;  ///< If 0, count is 4DW; if 1, count is 8DW. Other sizes are defined by the usage type.
				uint8_t m_resourceType : 1;   ///< If 0, resource type <c>V#</c>; if 1, resource type <c>T#</c>, in case of a Gnm::kShaderInputUsageImmResource.
				uint8_t m_reserved : 2;       ///< Unused; must be set to zero.
				uint8_t m_chunkMask : 4;      ///< Internal usage data.
			};
			uint8_t m_srtSizeInDWordMinusOne;  ///< Size of the SRT data; used for Gnm::kShaderInputUsageImmShaderResourceTable.
		};
	};

	using InputUsageSlotTable = std::vector<InputUsageSlot>;


	struct VertexInputSemantic
	{
		uint8_t m_semantic;
		uint8_t m_vgpr;
		uint8_t m_sizeInElements;
		uint8_t m_reserved;
	};

	using VertexInputSemanticTable = std::vector<VertexInputSemantic>;



	struct VertexExportSemantic
	{                      // __cplusplus
		uint8_t m_semantic;      ///< Description to be specified.
		uint8_t m_outIndex : 5;  ///< Description to be specified.
		uint8_t m_reserved : 1;
		uint8_t m_exportF16 : 2;  ///< if (m_exportF16 == 0) this shader exports a 32-bit value to this parameter; if (m_exportF16 & 1) this shader exports a 16-bit float value to the low 16-bits of each channel; if (m_exportF16 & 2) this shader exports a 16-bit float value to the high 16-bits of each channel
	};


	struct PixelInputSemantic
	{
		union
		{
			struct
			{
				uint16_t m_semantic : 8;      ///< The semantic, matched against the semantic value in the VertexExportSemantic table in the VS shader.
				uint16_t m_defaultValue : 2;  ///< The default value supplied to the shader, if m_semantic is not matched in the VS shader. 0={0,0,0,0}, 1={0,0,0,1.0}, 2={1.0,1.0,1.0,0}, 3={1.0,1.0,1.0,1.0}
				uint16_t m_isFlatShaded : 1;  ///< if (m_interpF16 == 0) A bitflag that specifies whether the value interpolation is constant in the shader. It is ignored if <c><i>m_isCustom</i></c> is set; otherwise, it  indicates that a shader reads only { P0 } and that some handling of infinite values in the calculation of P1-P0 and P2-P0 can be disabled.
				uint16_t m_isLinear : 1;      ///< A bitflag that specifies whether the value interpolation is linear in the shader. It is unused by the Gnm runtime.
				uint16_t m_isCustom : 1;      ///< if (m_interpF16 == 0) A bitflag that specifies whether the value interpolation is custom in the shader. It determines whether hardware subtraction should be disabled, supplying { P0, P1, P2 } to the shader instead of { P0, P1-P0, P2-P0 }.
				uint16_t m_reserved : 3;      ///< Unused; set to zero.
			};
			// NEO mode only:
			struct
			{
				uint16_t : 12;                  ///< Description to be specified.
				uint16_t m_defaultValueHi : 2;  ///< if (m_interpF16 != 0) indicates the default value supplied to the shader for the upper 16-bits if m_semantic is not matched in the VS shader, and m_defaultValue indicates the default value for the lower 16-bits.
				uint16_t m_interpF16 : 2;       ///< if (m_interpF16 == 0) this is a 32-bit float or custom value; if (m_interpF16 & 1) the low 16-bits of this parameter expect 16-bit float interpolation and/or default value; if (m_interpF16 & 2) the high 16-bits of this parameter expect 16-bit float interpolation and/or default value
			};
		};
	};

}  // namespace sce::gcn
