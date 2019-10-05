#pragma once

#include "GPCS4Common.h"


// only place shader binary file related structures

namespace pssl
{;

enum ShaderInputUsageType
{
	kShaderInputUsageImmResource = 0x00,
	kShaderInputUsageImmSampler = 0x01,
	kShaderInputUsageImmConstBuffer = 0x02,
	kShaderInputUsageImmVertexBuffer = 0x03,
	kShaderInputUsageImmRwResource = 0x04,
	kShaderInputUsageImmAluFloatConst = 0x05,
	kShaderInputUsageImmAluBool32Const = 0x06,
	kShaderInputUsageImmGdsCounterRange = 0x07,
	kShaderInputUsageImmGdsMemoryRange = 0x08,
	kShaderInputUsageImmGwsBase = 0x09,
	kShaderInputUsageImmShaderResourceTable = 0x0A,
	kShaderInputUsageImmLdsEsGsSize = 0x0D,

	kShaderInputUsageSubPtrFetchShader = 0x12,
	kShaderInputUsagePtrResourceTable = 0x13,
	kShaderInputUsagePtrInternalResourceTable = 0x14,
	kShaderInputUsagePtrSamplerTable = 0x15,
	kShaderInputUsagePtrConstBufferTable = 0x16,
	kShaderInputUsagePtrVertexBufferTable = 0x17,
	kShaderInputUsagePtrSoBufferTable = 0x18,
	kShaderInputUsagePtrRwResourceTable = 0x19,
	kShaderInputUsagePtrInternalGlobalTable = 0x1A,
	kShaderInputUsagePtrExtendedUserData = 0x1B,
	kShaderInputUsagePtrIndirectResourceTable = 0x1C,
	kShaderInputUsagePtrIndirectInternalResourceTable = 0x1D,
	kShaderInputUsagePtrIndirectRwResourceTable = 0x1E,

	kShaderInputUsageImmGdsKickRingBufferOffset = 0x22,
	kShaderInputUsageImmVertexRingBufferOffset = 0x23,
	kShaderInputUsagePtrDispatchDraw = 0x24,
	kShaderInputUsageImmDispatchDrawInstances = 0x25,
};


#pragma pack(push, 4)
struct InputUsageSlot
{
	uint8_t usageType;  // ShaderInputUsageType
	uint8_t apiSlot;
	uint8_t startRegister;

	union
	{
		struct
		{
			uint8_t registerCount	: 1;  // 0 for 4DW; 1 for 8DW; Other count depend on usageType
			uint8_t resourceType	: 1;  // If 0, resource type V#; if 1, resource type T#, in case of a Gnm::kShaderInputUsageImmResource.
			uint8_t reserved		: 2;
			uint8_t chunkMask		: 4;
		};
		uint8_t srtSizeInDWordMinusOne;
	};

};
#pragma pack(pop)


struct VertexInputSemantic
{
	uint8_t semantic;
	uint8_t vgpr;
	uint8_t sizeInElements;
	uint8_t reserved;
};

struct VertexExportSemantic
{
	uint8_t semantic;
	uint8_t outIndex	: 5;
	uint8_t reserved	: 1;
	uint8_t exportF16	: 2;
};

struct PixelInputSemantic
{

	union
	{
		struct
		{
			uint16_t              semantic		: 8;
			uint16_t              defaultValue	: 2;
			uint16_t              isFlatShaded	: 1;
			uint16_t              isLinear		: 1;
			uint16_t              isCustom		: 1;
			uint16_t              reserved		: 3;
		};
		// NEO:
		struct
		{
			uint16_t								: 12;
			uint16_t              defaultValueHi	: 2;
			uint16_t              interpF16			: 2;
		};
	};
};


enum ShaderBinaryType
{
	kShaderTypePs = 0,
	kShaderTypeVsVs = 1,
	kShaderTypeVsEs = 2,
	kShaderTypeVsLs = 3,
	kShaderTypeCs = 4,
	kShaderTypeGs = 5,
	kShaderTypeHs = 7,
	kShaderTypeDsVs = 8,
};

struct ShaderBinaryInfo
{
	uint8_t			signature[7];
	uint8_t			version;

	unsigned int	pssl_or_cg	: 1;
	unsigned int	cached		: 1;
	uint32_t		type		: 4; 
	uint32_t		source_type : 2;
	unsigned int	length		: 24;

	uint8_t			chunkUsageBaseOffsetInDW;
	uint8_t			numInputUsageSlots;
	uint8_t         isSrt					: 1;
	uint8_t         isSrtUsedInfoValid		: 1;
	uint8_t         isExtendedUsageInfo		: 1;
	uint8_t         reserved2				: 5;
	uint8_t         reserved3;

	uint32_t		shaderHash0;
	uint32_t		shaderHash1;
	uint32_t		crc32;
};


}  // pssl