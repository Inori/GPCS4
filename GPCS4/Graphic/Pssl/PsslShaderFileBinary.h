#pragma once

#include "GPCS4Common.h"


// only place shader binary file related structures

namespace pssl
{;


#pragma pack(push, 4)
struct InputUsageSlot
{
	uint8_t usageType;
	uint8_t apiSlot;
	uint8_t startRegister;

	union
	{
		struct
		{
			uint8_t registerCount	: 1;
			uint8_t resourceType	: 1;
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


struct ShaderBinaryInfo
{
	uint8_t			signature[7];
	uint8_t			version;

	unsigned int	pssl_or_cg : 1;
	unsigned int	cached : 1;
	uint32_t		type : 4;
	uint32_t		source_type : 2;
	unsigned int	length : 24;

	uint8_t			chunkUsageBaseOffsetInDW;
	uint8_t			numInputUsageSlots;
	uint8_t         isSrt : 1;
	uint8_t         isSrtUsedInfoValid : 1;
	uint8_t         isExtendedUsageInfo : 1;
	uint8_t         reserved2 : 5;
	uint8_t         reserved3;

	uint32_t		shaderHash0;
	uint32_t		shaderHash1;
	uint32_t		crc32;
};


}  // pssl