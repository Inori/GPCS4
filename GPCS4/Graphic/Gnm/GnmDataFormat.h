#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"

#include <unordered_map>

union DataFormat
{
	class
	{
	public:
		uint32_t   m_surfaceFormat : 8;  // SurfaceFormat
		uint32_t   m_channelType : 4;
		uint32_t   m_channelX : 3;
		uint32_t   m_channelY : 3;
		uint32_t   m_channelZ : 3;
		uint32_t   m_channelW : 3;
		uint32_t   m_unused : 8;
	} m_bits;
	uint32_t m_asInt;

	static DataFormat build(
		SurfaceFormat surfFmt, 
		TextureChannelType channelType,
		TextureChannel chanX = kTextureChannelX,
		TextureChannel chanY = kTextureChannelY,
		TextureChannel chanZ = kTextureChannelZ,
		TextureChannel chanW = kTextureChannelW);

	static DataFormat build(
		RenderTargetFormat rtFmt, 
		RenderTargetChannelType rtChannelType, 
		RenderTargetChannelOrder channelOrder);

	bool operator==(const DataFormat& other) const;

	uint32_t getTotalBitsPerElement() const;

	uint32_t getTexelsPerElement(void) const;

	ZFormat getZFormat(void) const;

	StencilFormat getStencilFormat(void) const;

	uint32_t getTotalBytesPerElement(void) const;

	bool isBlockCompressedFormat(void) const;

private:
	static constexpr int s_bitsPerElement[] = 
	{
		0, 8, 16, 16, 32, 32, 32, 32, 32, 32, 32, 64, 64, 96, 128, -1, 
		16, 16, 16, 16, 32, 32, 64, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
		16, 16, 32, 4, 8, 8, 4, 8, 8, 8, -1, -1, 8, 8, 8, 8, 8, 8, 16,
		16, 32, 32, 32, 64, 64, 8, 16, 1, 1
	};
};

struct DataFormatHash
{
	std::size_t operator()(DataFormat const& dataFmt) const noexcept
	{
		return dataFmt.m_asInt;
	}
};

constexpr DataFormat		kDataFormatInvalid = { {kSurfaceFormatInvalid, kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR32G32B32A32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB32G32R32A32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR32G32B32X32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB32G32R32X32Float = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR32G32B32A32Uint = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR32G32B32A32Sint = { {kSurfaceFormat32_32_32_32, kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR32G32B32Float = { {kSurfaceFormat32_32_32,    kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR32G32B32Uint = { {kSurfaceFormat32_32_32,    kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR32G32B32Sint = { {kSurfaceFormat32_32_32,    kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR16G16B16A16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR16G16B16X16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB16G16R16X16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR16G16B16A16Uint = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR16G16B16A16Sint = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR16G16B16A16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB16G16R16A16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR16G16B16X16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB16G16R16X16Unorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR16G16B16A16Snorm = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatL32A32Float = { {kSurfaceFormat32_32,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX,  kTextureChannelX,  kTextureChannelY, 0 } };
constexpr DataFormat		kDataFormatR32G32Float = { {kSurfaceFormat32_32,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR32G32Uint = { {kSurfaceFormat32_32,       kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR32G32Sint = { {kSurfaceFormat32_32,       kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR11G11B10Float = { {kSurfaceFormat10_11_11,    kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8G8B8A8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR8G8B8X8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8G8B8A8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR8G8B8X8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8G8B8A8Uint = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR8G8B8A8Snorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR8G8B8A8Sint = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatL16A16Float = { {kSurfaceFormat16_16,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX,  kTextureChannelX,  kTextureChannelY, 0 } };
constexpr DataFormat		kDataFormatR16G16Float = { {kSurfaceFormat16_16,       kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL16A16Unorm = { {kSurfaceFormat16_16,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelX,  kTextureChannelX,  kTextureChannelY, 0 } };
constexpr DataFormat		kDataFormatR16G16Unorm = { {kSurfaceFormat16_16,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR16G16Uint = { {kSurfaceFormat16_16,       kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR16G16Snorm = { {kSurfaceFormat16_16,       kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR16G16Sint = { {kSurfaceFormat16_16,       kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR32Float = { {kSurfaceFormat32,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL32Float = { {kSurfaceFormat32,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA32Float = { {kSurfaceFormat32,          kTextureChannelTypeFloat, kTextureChannelConstant0,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
constexpr DataFormat		kDataFormatR32Uint = { {kSurfaceFormat32,          kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR32Sint = { {kSurfaceFormat32,          kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR8G8Unorm = { {kSurfaceFormat8_8,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8G8Uint = { {kSurfaceFormat8_8,         kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR8G8Snorm = { {kSurfaceFormat8_8,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8G8Sint = { {kSurfaceFormat8_8,         kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatL8A8Unorm = { {kSurfaceFormat8_8,         kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelX,  kTextureChannelX, kTextureChannelY, 0 } };
constexpr DataFormat		kDataFormatL8A8UnormSrgb = { {kSurfaceFormat8_8,         kTextureChannelTypeSrgb,  kTextureChannelX, kTextureChannelX,  kTextureChannelX, kTextureChannelY, 0 } };
constexpr DataFormat		kDataFormatR16Float = { {kSurfaceFormat16,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL16Float = { {kSurfaceFormat16,          kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA16Float = { {kSurfaceFormat16,          kTextureChannelTypeFloat, kTextureChannelConstant0,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
constexpr DataFormat		kDataFormatR16Unorm = { {kSurfaceFormat16,          kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL16Unorm = { {kSurfaceFormat16,          kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA16Unorm = { {kSurfaceFormat16,          kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
constexpr DataFormat		kDataFormatR16Uint = { {kSurfaceFormat16,          kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR16Snorm = { {kSurfaceFormat16,          kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR16Sint = { {kSurfaceFormat16,          kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR8Unorm = { {kSurfaceFormat8,           kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL8Unorm = { {kSurfaceFormat8,           kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL8UnormSrgb = { {kSurfaceFormat8,           kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8Uint = { {kSurfaceFormat8,           kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatR8Snorm = { {kSurfaceFormat8,           kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR8Sint = { {kSurfaceFormat8,           kTextureChannelTypeSInt,  kTextureChannelX,  kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, 0 } };
constexpr DataFormat		kDataFormatA8Unorm = { {kSurfaceFormat8,           kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };

constexpr DataFormat		kDataFormatR1Unorm = { { kSurfaceFormat1, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL1Unorm = { { kSurfaceFormat1, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA1Unorm = { { kSurfaceFormat1, kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
constexpr DataFormat		kDataFormatR1Uint = { { kSurfaceFormat1, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL1Uint = { { kSurfaceFormat1, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA1Uint = { { kSurfaceFormat1, kTextureChannelTypeUInt, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };

constexpr DataFormat		kDataFormatR1ReversedUnorm = { { kSurfaceFormat1Reversed, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL1ReversedUnorm = { { kSurfaceFormat1Reversed, kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA1ReversedUnorm = { { kSurfaceFormat1Reversed, kTextureChannelTypeUNorm, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };
constexpr DataFormat		kDataFormatR1ReversedUint = { { kSurfaceFormat1Reversed, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatL1ReversedUint = { { kSurfaceFormat1Reversed, kTextureChannelTypeUInt, kTextureChannelX, kTextureChannelX, kTextureChannelX, kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatA1ReversedUint = { { kSurfaceFormat1Reversed, kTextureChannelTypeUInt, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelConstant0, kTextureChannelX, 0 } };

constexpr DataFormat		kDataFormatBc1Unorm = { {kSurfaceFormatBc1,         kTextureChannelTypeUNorm, kTextureChannelX, kTextureChannelY, kTextureChannelZ, kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc1UBNorm = { {kSurfaceFormatBc1,         kTextureChannelTypeUBNorm, kTextureChannelX, kTextureChannelY, kTextureChannelZ, kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc1UnormSrgb = { {kSurfaceFormatBc1,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc2Unorm = { {kSurfaceFormatBc2,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc2UBNorm = { {kSurfaceFormatBc2,         kTextureChannelTypeUBNorm, kTextureChannelX, kTextureChannelY, kTextureChannelZ, kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc2UnormSrgb = { {kSurfaceFormatBc2,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc3Unorm = { {kSurfaceFormatBc3,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc3UBNorm = { {kSurfaceFormatBc3,         kTextureChannelTypeUBNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc3UnormSrgb = { {kSurfaceFormatBc3,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc4Unorm = { {kSurfaceFormatBc4,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelConstant0,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc4Snorm = { {kSurfaceFormatBc4,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelConstant0,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc5Unorm = { {kSurfaceFormatBc5,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc5Snorm = { {kSurfaceFormatBc5,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelConstant0,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc6Unorm = { {kSurfaceFormatBc6,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc6Snorm = { {kSurfaceFormatBc6,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc6Uf16 = { {kSurfaceFormatBc6,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc6Sf16 = { {kSurfaceFormatBc6,         kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc7Unorm = { {kSurfaceFormatBc7,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc7UBNorm = { {kSurfaceFormatBc7,         kTextureChannelTypeUBNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatBc7UnormSrgb = { {kSurfaceFormatBc7,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB5G6R5Unorm = { {kSurfaceFormat5_6_5,       kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR5G5B5A1Unorm = { {kSurfaceFormat1_5_5_5,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB5G5R5A1Unorm = { {kSurfaceFormat1_5_5_5,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB8G8R8A8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB8G8R8X8Unorm = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB8G8R8A8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB8G8R8X8UnormSrgb = { {kSurfaceFormat8_8_8_8,     kTextureChannelTypeSrgb,  kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };

constexpr DataFormat		kDataFormatR4G4B4A4Unorm = { {kSurfaceFormat4_4_4_4,     kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB4G4R4A4Unorm = { {kSurfaceFormat4_4_4_4,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB4G4R4X4Unorm = { {kSurfaceFormat4_4_4_4,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB5G5R5X1Unorm = { {kSurfaceFormat1_5_5_5,     kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR5G6B5Unorm = { {kSurfaceFormat5_6_5,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

constexpr DataFormat		kDataFormatB10G10R10A2Unorm = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR10G10B10A2Unorm = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR10G10B10A2Uint = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUInt,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB10G10R10A2Uint = { {kSurfaceFormat2_10_10_10,  kTextureChannelTypeUInt,  kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatB16G16R16A16Float = { {kSurfaceFormat16_16_16_16, kTextureChannelTypeFloat, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelW, 0 } };
constexpr DataFormat		kDataFormatR11G11B10Unorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatR11G11B10Snorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeSNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB10G11R11Unorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeUNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB10G11R11Snorm = { {kSurfaceFormat10_11_11,    kTextureChannelTypeSNorm, kTextureChannelZ,  kTextureChannelY,  kTextureChannelX,  kTextureChannelConstant1, 0 } };

constexpr DataFormat		kDataFormatR9G9B9E5Float = { {kSurfaceFormat5_9_9_9,     kTextureChannelTypeFloat, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatB8G8R8G8Unorm = { {kSurfaceFormatBG_RG,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatG8B8G8R8Unorm = { {kSurfaceFormatGB_GR,       kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

constexpr DataFormat		kDataFormatBc1UnormNoAlpha = { {kSurfaceFormatBc1,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc1UnormSrgbNoAlpha = { {kSurfaceFormatBc1,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

constexpr DataFormat		kDataFormatBc7UnormNoAlpha = { {kSurfaceFormatBc7,         kTextureChannelTypeUNorm, kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };
constexpr DataFormat		kDataFormatBc7UnormSrgbNoAlpha = { {kSurfaceFormatBc7,         kTextureChannelTypeSrgb,  kTextureChannelX,  kTextureChannelY,  kTextureChannelZ,  kTextureChannelConstant1, 0 } };

constexpr DataFormat		kDataFormatBc3UnormRABG = { {kSurfaceFormatBc3,         kTextureChannelTypeUNorm,  kTextureChannelX,  kTextureChannelW,  kTextureChannelZ,  kTextureChannelY, 0 } };

// Used for debugging purpose
const char* dataFormatName(DataFormat dataFmt);