#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"

union DataFormat
{
	class
	{
	public:
		uint32_t   m_surfaceFormat : 8;
		uint32_t   m_channelType : 4;
		uint32_t   m_channelX : 3;
		uint32_t   m_channelY : 3;
		uint32_t   m_channelZ : 3;
		uint32_t   m_channelW : 3;
		uint32_t   m_unused : 8;
	} m_bits;
	uint32_t m_asInt;

	static DataFormat build(SurfaceFormat surfFmt, TextureChannelType channelType,
		TextureChannel chanX = kTextureChannelX,
		TextureChannel chanY = kTextureChannelY,
		TextureChannel chanZ = kTextureChannelZ,
		TextureChannel chanW = kTextureChannelW)
	{
		DataFormat result = { 0 };
		result.m_bits.m_surfaceFormat = surfFmt;
		result.m_bits.m_channelType = channelType;
		result.m_bits.m_channelX = chanX;
		result.m_bits.m_channelY = chanY;
		result.m_bits.m_channelZ = chanZ;
		result.m_bits.m_channelW = chanW;
		result.m_bits.m_unused = 0;

		return result;
	}

	static DataFormat build(RenderTargetFormat rtFmt, RenderTargetChannelType rtChannelType, RenderTargetChannelOrder channelOrder)
	{
		const uint32_t channelCountTab[kRenderTargetFormatX24_8_32 + 1] = 
		{
			0,1,1,2,1,2,3,3,4,4,4,2,4,0,4,0,3,4,4,4,2,2,3
		};

		uint32_t channelCount = channelCountTab[rtFmt];

		DataFormat result = { {0} };
		result.m_bits.m_surfaceFormat = rtFmt;
		result.m_bits.m_channelType = rtChannelType;

		switch (channelOrder)
		{
		case kRenderTargetChannelOrderStandard:
		{
			switch (channelCount)
			{
			case 4: result.m_bits.m_channelW = kTextureChannelW;
			case 3: result.m_bits.m_channelZ = kTextureChannelZ;
			case 2: result.m_bits.m_channelY = kTextureChannelY;
			case 1: result.m_bits.m_channelX = kTextureChannelX;
			}
		}
			break;
		case kRenderTargetChannelOrderAlt:
		{
			switch (channelCount)
			{
			case 1: { result.m_bits.m_channelX = kTextureChannelY; } break;
			case 2: { result.m_bits.m_channelX = kTextureChannelX; result.m_bits.m_channelY = kTextureChannelW; } break;
			case 3: { result.m_bits.m_channelX = kTextureChannelX; result.m_bits.m_channelY = kTextureChannelY; result.m_bits.m_channelZ = kTextureChannelW; } break;
			case 4: { result.m_bits.m_channelX = kTextureChannelZ; result.m_bits.m_channelY = kTextureChannelY; result.m_bits.m_channelZ = kTextureChannelX; result.m_bits.m_channelW = kTextureChannelW; } break;
			}
		}
			break;
		case kRenderTargetChannelOrderReversed:
		{
			switch (channelCount)
			{
			case 1: { result.m_bits.m_channelX = kTextureChannelZ; } break;
			case 2: { result.m_bits.m_channelX = kTextureChannelY; result.m_bits.m_channelY = kTextureChannelX; } break;
			case 3: { result.m_bits.m_channelX = kTextureChannelZ; result.m_bits.m_channelY = kTextureChannelY; result.m_bits.m_channelZ = kTextureChannelX; } break;
			case 4: { result.m_bits.m_channelX = kTextureChannelW; result.m_bits.m_channelY = kTextureChannelZ; result.m_bits.m_channelZ = kTextureChannelY; result.m_bits.m_channelW = kTextureChannelX; } break;
			}
		}
			break;
		case kRenderTargetChannelOrderAltReversed:
		{
			switch (channelCount)
			{
			case 1: { result.m_bits.m_channelX = kTextureChannelW; } break;
			case 2: { result.m_bits.m_channelX = kTextureChannelW; result.m_bits.m_channelY = kTextureChannelX; } break;
			case 3: { result.m_bits.m_channelX = kTextureChannelW; result.m_bits.m_channelY = kTextureChannelY; result.m_bits.m_channelZ = kTextureChannelX; } break;
			case 4: { result.m_bits.m_channelX = kTextureChannelW; result.m_bits.m_channelY = kTextureChannelX; result.m_bits.m_channelZ = kTextureChannelY; result.m_bits.m_channelW = kTextureChannelZ; } break;
			}
		}
			break;
		}

		return result;
	}

	// From IDA
	uint32_t getTotalBitsPerElement() const
	{
		// TODO:
		// Make it more beautiful, not just copy from ida.
		uint32_t surfFmt = m_bits.m_surfaceFormat;
		uint32_t result = 0LL;
		if ((unsigned int)surfFmt <= 0x3C)
		{
			uint32_t v3 = 16;
			if ((unsigned __int8)(surfFmt - 35) >= 7u)
			{
				if ((unsigned __int8)(surfFmt - 59) > 1u)
				{
					v3 = 1;
				}
				else
				{
					v3 = 8;
				}
			}
			result = (unsigned int)(s_bitsPerElement[surfFmt] * v3);
		}
		return result;
	}

private:
	static constexpr int s_bitsPerElement[] = 
	{
		0, 8, 16, 16, 32, 32, 32, 32, 32, 32, 32, 64, 64, 96, 128, -1, 
		16, 16, 16, 16, 32, 32, 64, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
		16, 16, 32, 4, 8, 8, 4, 8, 8, 8, -1, -1, 8, 8, 8, 8, 8, 8, 16,
		16, 32, 32, 32, 64, 64, 8, 16, 1, 1
	};
};



const DataFormat		kDataFormatInvalid = { {kSurfaceFormatInvalid, kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR32G32B32A32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB32G32R32A32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR32G32B32X32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB32G32R32X32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR32G32B32A32Uint = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR32G32B32A32Sint = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR32G32B32Float = { {kSurfaceFormat32_32_32,    kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR32G32B32Uint = { {kSurfaceFormat32_32_32,    kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR32G32B32Sint = { {kSurfaceFormat32_32_32,    kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR16G16B16A16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR16G16B16X16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB16G16R16X16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR16G16B16A16Uint = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR16G16B16A16Sint = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR16G16B16A16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB16G16R16A16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR16G16B16X16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB16G16R16X16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR16G16B16A16Snorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatL32A32Float = { {kSurfaceFormat32_32,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX,  kTextureChannelX,  kTextureChannelY, 0 } };
const DataFormat		kDataFormatR32G32Float = { {kSurfaceFormat32_32,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR32G32Uint = { {kSurfaceFormat32_32,       kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR32G32Sint = { {kSurfaceFormat32_32,       kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR11G11B10Float = { {kSurfaceFormat10_11_11,    kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8G8B8A8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR8G8B8X8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8G8B8A8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR8G8B8X8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8G8B8A8Uint = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR8G8B8A8Snorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR8G8B8A8Sint = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatL16A16Float = { {kSurfaceFormat16_16,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX,  kTextureChannelX,  kTextureChannelY, 0 } };
const DataFormat		kDataFormatR16G16Float = { {kSurfaceFormat16_16,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL16A16Unorm = { {kSurfaceFormat16_16,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelX,  kTextureChannelX,  kTextureChannelY, 0 } };
const DataFormat		kDataFormatR16G16Unorm = { {kSurfaceFormat16_16,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR16G16Uint = { {kSurfaceFormat16_16,       kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR16G16Snorm = { {kSurfaceFormat16_16,       kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR16G16Sint = { {kSurfaceFormat16_16,       kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR32Float = { {kSurfaceFormat32,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL32Float = { {kSurfaceFormat32,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA32Float = { {kSurfaceFormat32,          kTextureChannelTypeFloat, kTextureChannelConstant0,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
const DataFormat		kDataFormatR32Uint = { {kSurfaceFormat32,          kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR32Sint = { {kSurfaceFormat32,          kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR8G8Unorm = { {kSurfaceFormat8_8,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8G8Uint = { {kSurfaceFormat8_8,         kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR8G8Snorm = { {kSurfaceFormat8_8,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8G8Sint = { {kSurfaceFormat8_8,         kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatL8A8Unorm = { {kSurfaceFormat8_8,         kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelX,  kTextureChannelX, kTextureChannelY, 0 } };
const DataFormat		kDataFormatL8A8UnormSrgb = { {kSurfaceFormat8_8,         kTextureChannelTypeSrgb,  kTextureChannelX, kTextureChannelX,  kTextureChannelX, kTextureChannelY, 0 } };
const DataFormat		kDataFormatR16Float = { {kSurfaceFormat16,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL16Float = { {kSurfaceFormat16,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA16Float = { {kSurfaceFormat16,          kTextureChannelTypeFloat, kTextureChannelConstant0,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
const DataFormat		kDataFormatR16Unorm = { {kSurfaceFormat16,          kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL16Unorm = { {kSurfaceFormat16,          kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA16Unorm = { {kSurfaceFormat16,          kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
const DataFormat		kDataFormatR16Uint = { {kSurfaceFormat16,          kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR16Snorm = { {kSurfaceFormat16,          kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR16Sint = { {kSurfaceFormat16,          kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR8Unorm = { {kSurfaceFormat8,           kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL8Unorm = { {kSurfaceFormat8,           kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL8UnormSrgb = { {kSurfaceFormat8,           kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8Uint = { {kSurfaceFormat8,           kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatR8Snorm = { {kSurfaceFormat8,           kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR8Sint = { {kSurfaceFormat8,           kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
const DataFormat		kDataFormatA8Unorm = { {kSurfaceFormat8,           kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };

const DataFormat		kDataFormatR1Unorm = { { kSurfaceFormat1, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL1Unorm = { { kSurfaceFormat1, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA1Unorm = { { kSurfaceFormat1, kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
const DataFormat		kDataFormatR1Uint = { { kSurfaceFormat1, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL1Uint = { { kSurfaceFormat1, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA1Uint = { { kSurfaceFormat1, kTextureChannelTypeUInt, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };

const DataFormat		kDataFormatR1ReversedUnorm = { { kSurfaceFormat1Reversed, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL1ReversedUnorm = { { kSurfaceFormat1Reversed, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA1ReversedUnorm = { { kSurfaceFormat1Reversed, kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
const DataFormat		kDataFormatR1ReversedUint = { { kSurfaceFormat1Reversed, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatL1ReversedUint = { { kSurfaceFormat1Reversed, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatA1ReversedUint = { { kSurfaceFormat1Reversed, kTextureChannelTypeUInt, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };

const DataFormat		kDataFormatBc1Unorm = { {kSurfaceFormatBc1,         kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelY, kTextureChannelZ, kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc1UBNorm = { {kSurfaceFormatBc1,         kTextureChannelTypeUBNorm, kTextureChannelX, kTextureChannelY, kTextureChannelZ, kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc1UnormSrgb = { {kSurfaceFormatBc1,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc2Unorm = { {kSurfaceFormatBc2,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc2UBNorm = { {kSurfaceFormatBc2,         kTextureChannelTypeUBNorm, kTextureChannelX, kTextureChannelY, kTextureChannelZ, kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc2UnormSrgb = { {kSurfaceFormatBc2,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc3Unorm = { {kSurfaceFormatBc3,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc3UBNorm = { {kSurfaceFormatBc3,         kTextureChannelTypeUBNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc3UnormSrgb = { {kSurfaceFormatBc3,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc4Unorm = { {kSurfaceFormatBc4,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelConstant0,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc4Snorm = { {kSurfaceFormatBc4,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelConstant0,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc5Unorm = { {kSurfaceFormatBc5,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc5Snorm = { {kSurfaceFormatBc5,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc6Unorm = { {kSurfaceFormatBc6,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc6Snorm = { {kSurfaceFormatBc6,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc6Uf16 = { {kSurfaceFormatBc6,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc6Sf16 = { {kSurfaceFormatBc6,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc7Unorm = { {kSurfaceFormatBc7,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc7UBNorm = { {kSurfaceFormatBc7,         kTextureChannelTypeUBNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatBc7UnormSrgb = { {kSurfaceFormatBc7,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB5G6R5Unorm = { {kSurfaceFormat5_6_5,       kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR5G5B5A1Unorm = { {kSurfaceFormat1_5_5_5,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB5G5R5A1Unorm = { {kSurfaceFormat1_5_5_5,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB8G8R8A8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB8G8R8X8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB8G8R8A8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB8G8R8X8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };

const DataFormat		kDataFormatR4G4B4A4Unorm = { {kSurfaceFormat4_4_4_4,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB4G4R4A4Unorm = { {kSurfaceFormat4_4_4_4,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB4G4R4X4Unorm = { {kSurfaceFormat4_4_4_4,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB5G5R5X1Unorm = { {kSurfaceFormat1_5_5_5,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR5G6B5Unorm = { {kSurfaceFormat5_6_5,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

const DataFormat		kDataFormatB10G10R10A2Unorm = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR10G10B10A2Unorm = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR10G10B10A2Uint = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB10G10R10A2Uint = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUInt,  kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatB16G16R16A16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
const DataFormat		kDataFormatR11G11B10Unorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatR11G11B10Snorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB10G11R11Unorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB10G11R11Snorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeSNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };

const DataFormat		kDataFormatR9G9B9E5Float = { {kSurfaceFormat5_9_9_9,     kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatB8G8R8G8Unorm = { {kSurfaceFormatBG_RG,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatG8B8G8R8Unorm = { {kSurfaceFormatGB_GR,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

const DataFormat		kDataFormatBc1UnormNoAlpha = { {kSurfaceFormatBc1,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc1UnormSrgbNoAlpha = { {kSurfaceFormatBc1,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

const DataFormat		kDataFormatBc7UnormNoAlpha = { {kSurfaceFormatBc7,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
const DataFormat		kDataFormatBc7UnormSrgbNoAlpha = { {kSurfaceFormatBc7,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

const DataFormat		kDataFormatBc3UnormRABG = { {kSurfaceFormatBc3,         kTextureChannelTypeUNorm,  kTextureChannelX,  kTextureChannelW,  kTextureChannelZ,  kTextureChannelY, 0 } };