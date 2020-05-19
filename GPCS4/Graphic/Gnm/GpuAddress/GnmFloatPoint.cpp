#include "GnmFloatPoint.h"

#include <algorithm>

namespace GpuAddress
{;

Half::Half()
{
}

Half::Half(uint16_t h) :
	m_data(h)
{
}

Half::Half(float f)
{
	if (f == 0)
	{
		m_data = 0;
	}
	else
	{

		uif x;
		x.f = f;

		int32_t e = (x.i >> 23) & 0x000001ff;
		e         = s_arrExpLUT[e];

		if (e)
		{
			m_data = (uint16_t)(e + (((x.i & 0x007fffff) + 0x00001000) >> 13));
		}
		else
		{
			m_data = Convert(int32_t(x.i));
		}
	}
}

Half::operator uint16_t() const
{
	return m_data;
}


const uint16_t Half::s_arrExpLUT[1 << 9] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1024,
	2048,
	3072,
	4096,
	5120,
	6144,
	7168,
	8192,
	9216,
	10240,
	11264,
	12288,
	13312,
	14336,
	15360,
	16384,
	17408,
	18432,
	19456,
	20480,
	21504,
	22528,
	23552,
	24576,
	25600,
	26624,
	27648,
	28672,
	29696,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	33792,
	34816,
	35840,
	36864,
	37888,
	38912,
	39936,
	40960,
	41984,
	43008,
	44032,
	45056,
	46080,
	47104,
	48128,
	49152,
	50176,
	51200,
	52224,
	53248,
	54272,
	55296,
	56320,
	57344,
	58368,
	59392,
	60416,
	61440,
	62464,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

float Half::Overflow()
{
	volatile float f = 1e10;

	for (int32_t i = 0; i < 10; i++)
	{
		f *= f;
	}

	return f;
}

int32_t Half::Convert(int16_t i)
{

	int s = (i >> 15) & 0x00000001;

	int e = (i >> 10) & 0x0000001f;

	int m = i & 0x000003ff;

	if (e == 0)
	{
		if (m == 0)
		{
			return s << 31;
		}
		else
		{

			while (!(m & 0x00000400))
			{
				m <<= 1;
				e -= 1;
			}

			e += 1;
			m &= ~0x00000400;
		}
	}
	else if (e == 31)
	{
		if (m == 0)
		{

			return (s << 31) | 0x7f800000;
		}
		else
		{

			return (s << 31) | 0x7f800000 | (m << 13);
		}
	}

	e = e + (127 - 15);
	m = m << 13;

	return (s << 31) | (e << 23) | m;
}

int16_t Half::Convert(int32_t i)
{
	_F32 d;
	d.i = i;

	H16 o;
	o.i   = 0;
	o.h.s = d.f.s;

	int32_t e = d.f.e - (127 - 15);
	int32_t m = d.f.m;

	if (e <= 0)
	{
		if (e < -10)
		{

			return 0;
		}

		m = (m | 0x00800000) >> (1 - e);

		if (m & 0x00001000)
			m += 0x00002000;

		o.h.m = m >> 13;
		return o.i;
	}
	else if (e == 0xff - (127 - 15))
	{
		if (m == 0)
		{

			o.i |= 0x7c00;
			return o.i;
		}
		else
		{

			m >>= 13;
			o.i |= 0x7c00 | m | (m == 0);
			return o.i;
		}
	}
	else
	{

		if (m & 0x00001000)
		{
			m += 0x00002000;
			if (m & 0x00800000)
			{
				m = 0;
				e += 1;
			}
		}

		if (e > 30)
		{
#ifdef GPCS4_DEBUG
			Overflow();
#endif

			o.i |= 0x7c00;
			return o.i;
		}

		o.h.e = e;
		o.h.m = m >> 13;
		return o.i;
	}
}

//////////////////////////////////////////////////////////////////////////

union f32 
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
	uint32_t u;
	float    f;
};

uint32_t packBits(uint32_t value, uint32_t offset, uint32_t count, uint32_t field)
{
	const uint32_t mask = ((1 << count) - 1) << offset;
	return (value & ~mask) | ((field << offset) & mask);
}

uint32_t unpackBits(uint32_t value, uint32_t offset, uint32_t count)
{
	return (value >> offset) & ((1 << count) - 1);
}

uint32_t packFloat(float value, uint32_t signBits, uint32_t exponentBits, uint32_t mantissaBits)
{
	if (signBits == 0)
		value = std::max(0.f, value);
	if (exponentBits == 5)
	{
		if (signBits == 1 && mantissaBits == 10)
			return floatToFloat16(value);
		if (signBits == 0 && mantissaBits == 6)
			return floatToFloat11(value);
		if (signBits == 0 && mantissaBits == 5)
			return floatToFloat10(value);
	}
	f32 in;
	in.f                       = value;
	const int32_t  maxExponent = (1 << exponentBits) - 1;
	const uint32_t bias        = maxExponent >> 1;
	const uint32_t sign        = in.bits.m_sign;
	uint32_t       mantissa    = in.bits.m_mantissa >> (23 - mantissaBits);
	int32_t        exponent;
	switch (in.bits.m_exponent)
	{
	case 0x00:
		exponent = 0;
		break;
	case 0xFF:
		exponent = maxExponent;
		break;
	default:
		exponent = in.bits.m_exponent - 127 + bias;
		if (exponent < 1)
		{
			exponent = 1;
			mantissa = 0;
		}
		if (exponent > maxExponent - 1)
		{
			exponent = maxExponent - 1;
			mantissa = (1 << 23) - 1;
		}
	}
	uint32_t result = 0;
	result          = packBits(result, 0, mantissaBits, mantissa);
	result          = packBits(result, mantissaBits, exponentBits, exponent);
	result          = packBits(result, mantissaBits + exponentBits, signBits, sign);
	return result;
}

float unpackFloat(uint32_t value, uint32_t signBits, uint32_t exponentBits, uint32_t mantissaBits)
{
	if (exponentBits == 5)
	{
		if (signBits == 1 && mantissaBits == 10)
			return float16ToFloat(value);
		if (signBits == 0 && mantissaBits == 6)
			return float11ToFloat(value);
		if (signBits == 0 && mantissaBits == 5)
			return float10ToFloat(value);
	}
	f32            out;
	const uint32_t maxExponent = (1 << exponentBits) - 1;
	const uint32_t bias        = maxExponent >> 1;
	const uint32_t mantissa    = unpackBits(value, 0, mantissaBits);
	const uint32_t exponent    = unpackBits(value, mantissaBits, exponentBits);
	const uint32_t sign        = unpackBits(value, mantissaBits + exponentBits, signBits);
	out.bits.m_mantissa        = mantissa << (23 - mantissaBits);
	out.bits.m_exponent        = (exponent == 0) ? 0 : (exponent == maxExponent) ? 0xFF : exponent - bias + 127;
	out.bits.m_sign            = sign;
	return out.f;
}

int32_t convertFloatToInt(float value)
{
	return static_cast<int32_t>(floorf(value + 0.5f));
}

uint32_t convertFloatToUint(float value)
{
	return static_cast<uint32_t>(floorf(value + 0.5f));
}

uint32_t floatToFloat10(float value)
{
	if (value < 0.f)
		value = 0.f;
	uint32_t half = floatToFloat16(value);
	return half >> 5;
}

uint32_t floatToFloat11(float value)
{
	if (value < 0.f)
		value = 0.f;
	uint32_t half = floatToFloat16(value);
	return half >> 4;
}

uint32_t floatToFloat16(float value)
{
	Half half(value);
	return (uint16_t)half;
}

uint32_t floatToFloat32(float value)
{
	f32 f;
	f.f = value;
	return f.u;
}

float float10ToFloat(uint32_t value)
{
	Half half(static_cast<uint16_t>(value << 5));
	f32  fromHalf;
	fromHalf.f = (float)half;
	if (value >= 993)
		fromHalf.u = 4290772992;
	return fromHalf.f;
}

float float11ToFloat(uint32_t value)
{
	Half half(static_cast<uint16_t>(value << 4));
	f32  fromHalf;
	fromHalf.f = (float)half;
	if (value >= 1985)
		fromHalf.u = 4290772992;
	return fromHalf.f;
}

float float16ToFloat(uint32_t value)
{
	Half half(static_cast<uint16_t>(value));
	return (float)half;
}

float float32ToFloat(uint32_t value)
{
	f32 f;
	f.u = value;
	return f.f;
}

}  // namespace GpuAddress