#pragma once

#include "../GnmCommon.h"
#include "../GnmDataFormat.h"

namespace GpuAddress
{;

union Reg32
{
	enum
	{
		kBias = 127
	};
	struct
	{
		uint32_t m_mantissa : 23;
		uint32_t m_exponent : 8;
		uint32_t m_sign : 1;
	} bits;
	float    f;
	uint32_t u;
	int32_t  i;
};

void dataFormatDecoder(
	Reg32* __restrict			dest,
	const uint32_t* __restrict	src,
	const DataFormat			dataFormat);

}  // namespace DataFormat