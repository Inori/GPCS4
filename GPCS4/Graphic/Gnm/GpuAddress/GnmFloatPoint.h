#pragma once

#include "../GnmCommon.h"

namespace GpuAddress
{;

#define HALF_MIN     (5.96046448e-08)
#define HALF_NRM_MIN (6.10351562e-05)
#define HALF_MAX     (65504.0)
#define HALF_EPSILON (0.00097656)

#define HALF_MANT_DIG   (11)
#define HALF_DIG        (2)
#define HALF_RADIX      (2)
#define HALF_MIN_EXP    (-13)
#define HALF_MAX_EXP    (16)
#define HALF_MIN_10_EXP (-4)
#define HALF_MAX_10_EXP (4)

class Half
{
public:
	union uif 
	{
		uint32_t i;
		float    f;
	};

	union H16 
	{
		class
		{
		public:
			int16_t m : 10;
			int16_t e : 5;
			int16_t s : 1;
		} h;
		int16_t i;
	};

	union _F32 
	{
		class
		{
		public:
			int32_t m : 23;
			int32_t e : 8;
			int32_t s : 1;
		} f;
		int32_t i;
	};

	Half();
	Half(uint16_t h);
	Half(float f);

	operator float() const;
	operator uint16_t() const;

	Half operator-() const;

	Half& operator=(const Half& h);
	Half& operator=(float f);
	Half& operator+=(const Half& h);
	Half& operator+=(float f);
	Half& operator-=(const Half& h);
	Half& operator-=(float f);
	Half& operator*=(const Half& h);
	Half& operator*=(float f);
	Half& operator/=(const Half& h);
	Half& operator/=(float f);

	bool IsFinite() const;
	bool IsNormalized() const;
	bool IsDenormalized() const;
	bool IsZero() const;
	bool IsNAN() const;
	bool IsInf() const;
	bool IsNegative() const;

	static Half PosInf();
	static Half NegInf();
	static Half QNAN();
	static Half SNAN();

private:
	uint16_t m_data;

	static const uint16_t s_arrExpLUT[1 << 9];

	static int16_t Convert(int32_t i);
	static int32_t Convert(int16_t i);
	static float   Overflow();
};

inline Half::operator float() const
{
	uif u;
	u.i = Convert(static_cast<int16_t>(m_data));
	return u.f;
}

inline Half Half::operator-() const
{
	Half h;
	h.m_data = m_data ^ 0x8000;
	return h;
}

inline Half& Half::operator=(const Half& h)
{
	m_data = h.m_data;
	return *this;
}

inline Half& Half::operator=(float f)
{
	*this = Half(f);
	return *this;
}

inline Half& Half::operator+=(const Half& h)
{
	*this = Half(float(*this) + float(h));
	return *this;
}

inline Half& Half::operator+=(float f)
{
	*this = Half(float(*this) + f);
	return *this;
}

inline Half& Half::operator-=(const Half& h)
{
	*this = Half(float(*this) - float(h));
	return *this;
}

inline Half& Half::operator-=(float f)
{
	*this = Half(float(*this) - f);
	return *this;
}

inline Half& Half::operator*=(const Half& h)
{
	*this = Half(float(*this) * float(h));
	return *this;
}

inline Half& Half::operator*=(float f)
{
	*this = Half(float(*this) * f);
	return *this;
}

inline Half& Half::operator/=(const Half& h)
{
	*this = Half(float(*this) / float(h));
	return *this;
}

inline Half& Half::operator/=(float f)
{
	*this = Half(float(*this) / f);
	return *this;
}

inline bool Half::IsFinite() const
{
	uint16_t e = (m_data >> 10) & 0x001f;
	return e < 31;
}

inline bool Half::IsNormalized() const
{
	uint16_t e = (m_data >> 10) & 0x001f;
	return e > 0 && e < 31;
}

inline bool Half::IsDenormalized() const
{
	uint16_t e = (m_data >> 10) & 0x001f;
	uint16_t m = m_data & 0x3ff;
	return e == 0 && m != 0;
}

inline bool Half::IsZero() const
{
	return (m_data & 0x7fff) == 0;
}

inline bool Half::IsNAN() const
{
	uint16_t e = (m_data >> 10) & 0x001f;
	uint16_t m = m_data & 0x3ff;
	return e == 31 && m != 0;
}

inline bool Half::IsInf() const
{
	uint16_t e = (m_data >> 10) & 0x001f;
	uint16_t m = m_data & 0x3ff;
	return e == 31 && m == 0;
}

inline bool Half::IsNegative() const
{
	return (m_data & 0x8000) != 0;
}

inline Half Half::PosInf()
{
	Half h;
	h.m_data = 0x7c00;
	return h;
}

inline Half Half::NegInf()
{
	Half h;
	h.m_data = 0xfc00;
	return h;
}

inline Half Half::QNAN()
{
	Half h;
	h.m_data = 0x7fff;
	return h;
}

inline Half Half::SNAN()
{
	Half h;
	h.m_data = 0x7dff;
	return h;
}

//////////////////////////////////////////////////////////////////////////
	
uint32_t packFloat(float value, uint32_t signBits, uint32_t exponentBits, uint32_t mantissaBits);

float unpackFloat(uint32_t value, uint32_t signBits, uint32_t exponentBits, uint32_t mantissaBits);

int32_t convertFloatToInt(float value);

uint32_t convertFloatToUint(float value);

uint32_t floatToFloat10(float value);

uint32_t floatToFloat11(float value);

uint32_t floatToFloat16(float value);

uint32_t floatToFloat32(float value);

float float10ToFloat(uint32_t value);

float float11ToFloat(uint32_t value);

float float16ToFloat(uint32_t value);

float float32ToFloat(uint32_t value);

}  // namespace GpuAddress