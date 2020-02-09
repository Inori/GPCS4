#include "GnmDataFormat.h"

LOG_CHANNEL(Graphic.Gnm.GnmDataFormat);

const char* dataFormatName(DataFormat dataFmt)
{
	const static std::unordered_map<DataFormat, const char*, DataFormatHash> formatMap =
	{
		{ kDataFormatInvalid, "kDataFormatInvalid" },
		{ kDataFormatR32G32B32A32Float, "kDataFormatR32G32B32A32Float" },
		{ kDataFormatB32G32R32A32Float, "kDataFormatB32G32R32A32Float" },
		{ kDataFormatR32G32B32X32Float, "kDataFormatR32G32B32X32Float" },
		{ kDataFormatB32G32R32X32Float, "kDataFormatB32G32R32X32Float" },
		{ kDataFormatR32G32B32A32Uint, "kDataFormatR32G32B32A32Uint" },
		{ kDataFormatR32G32B32A32Sint, "kDataFormatR32G32B32A32Sint" },
		{ kDataFormatR32G32B32Float, "kDataFormatR32G32B32Float" },
		{ kDataFormatR32G32B32Uint, "kDataFormatR32G32B32Uint" },
		{ kDataFormatR32G32B32Sint, "kDataFormatR32G32B32Sint" },
		{ kDataFormatR16G16B16A16Float, "kDataFormatR16G16B16A16Float" },
		{ kDataFormatR16G16B16X16Float, "kDataFormatR16G16B16X16Float" },
		{ kDataFormatB16G16R16X16Float, "kDataFormatB16G16R16X16Float" },
		{ kDataFormatR16G16B16A16Uint, "kDataFormatR16G16B16A16Uint" },
		{ kDataFormatR16G16B16A16Sint, "kDataFormatR16G16B16A16Sint" },
		{ kDataFormatR16G16B16A16Unorm, "kDataFormatR16G16B16A16Unorm" },
		{ kDataFormatB16G16R16A16Unorm, "kDataFormatB16G16R16A16Unorm" },
		{ kDataFormatR16G16B16X16Unorm, "kDataFormatR16G16B16X16Unorm" },
		{ kDataFormatB16G16R16X16Unorm, "kDataFormatB16G16R16X16Unorm" },
		{ kDataFormatR16G16B16A16Snorm, "kDataFormatR16G16B16A16Snorm" },
		{ kDataFormatL32A32Float, "kDataFormatL32A32Float" },
		{ kDataFormatR32G32Float, "kDataFormatR32G32Float" },
		{ kDataFormatR32G32Uint, "kDataFormatR32G32Uint" },
		{ kDataFormatR32G32Sint, "kDataFormatR32G32Sint" },
		{ kDataFormatR11G11B10Float, "kDataFormatR11G11B10Float" },
		{ kDataFormatR8G8B8A8Unorm, "kDataFormatR8G8B8A8Unorm" },
		{ kDataFormatR8G8B8X8Unorm, "kDataFormatR8G8B8X8Unorm" },
		{ kDataFormatR8G8B8A8UnormSrgb, "kDataFormatR8G8B8A8UnormSrgb" },
		{ kDataFormatR8G8B8X8UnormSrgb, "kDataFormatR8G8B8X8UnormSrgb" },
		{ kDataFormatR8G8B8A8Uint, "kDataFormatR8G8B8A8Uint" },
		{ kDataFormatR8G8B8A8Snorm, "kDataFormatR8G8B8A8Snorm" },
		{ kDataFormatR8G8B8A8Sint, "kDataFormatR8G8B8A8Sint" },
		{ kDataFormatL16A16Float, "kDataFormatL16A16Float" },
		{ kDataFormatR16G16Float, "kDataFormatR16G16Float" },
		{ kDataFormatL16A16Unorm, "kDataFormatL16A16Unorm" },
		{ kDataFormatR16G16Unorm, "kDataFormatR16G16Unorm" },
		{ kDataFormatR16G16Uint, "kDataFormatR16G16Uint" },
		{ kDataFormatR16G16Snorm, "kDataFormatR16G16Snorm" },
		{ kDataFormatR16G16Sint, "kDataFormatR16G16Sint" },
		{ kDataFormatR32Float, "kDataFormatR32Float" },
		{ kDataFormatL32Float, "kDataFormatL32Float" },
		{ kDataFormatA32Float, "kDataFormatA32Float" },
		{ kDataFormatR32Uint, "kDataFormatR32Uint" },
		{ kDataFormatR32Sint, "kDataFormatR32Sint" },
		{ kDataFormatR8G8Unorm, "kDataFormatR8G8Unorm" },
		{ kDataFormatR8G8Uint, "kDataFormatR8G8Uint" },
		{ kDataFormatR8G8Snorm, "kDataFormatR8G8Snorm" },
		{ kDataFormatR8G8Sint, "kDataFormatR8G8Sint" },
		{ kDataFormatL8A8Unorm, "kDataFormatL8A8Unorm" },
		{ kDataFormatL8A8UnormSrgb, "kDataFormatL8A8UnormSrgb" },
		{ kDataFormatR16Float, "kDataFormatR16Float" },
		{ kDataFormatL16Float, "kDataFormatL16Float" },
		{ kDataFormatA16Float, "kDataFormatA16Float" },
		{ kDataFormatR16Unorm, "kDataFormatR16Unorm" },
		{ kDataFormatL16Unorm, "kDataFormatL16Unorm" },
		{ kDataFormatA16Unorm, "kDataFormatA16Unorm" },
		{ kDataFormatR16Uint, "kDataFormatR16Uint" },
		{ kDataFormatR16Snorm, "kDataFormatR16Snorm" },
		{ kDataFormatR16Sint, "kDataFormatR16Sint" },
		{ kDataFormatR8Unorm, "kDataFormatR8Unorm" },
		{ kDataFormatL8Unorm, "kDataFormatL8Unorm" },
		{ kDataFormatL8UnormSrgb, "kDataFormatL8UnormSrgb" },
		{ kDataFormatR8Uint, "kDataFormatR8Uint" },
		{ kDataFormatR8Snorm, "kDataFormatR8Snorm" },
		{ kDataFormatR8Sint, "kDataFormatR8Sint" },
		{ kDataFormatA8Unorm, "kDataFormatA8Unorm" },
		{ kDataFormatR1Unorm, "kDataFormatR1Unorm" },
		{ kDataFormatL1Unorm, "kDataFormatL1Unorm" },
		{ kDataFormatA1Unorm, "kDataFormatA1Unorm" },
		{ kDataFormatR1Uint, "kDataFormatR1Uint" },
		{ kDataFormatL1Uint, "kDataFormatL1Uint" },
		{ kDataFormatA1Uint, "kDataFormatA1Uint" },
		{ kDataFormatR1ReversedUnorm, "kDataFormatR1ReversedUnorm" },
		{ kDataFormatL1ReversedUnorm, "kDataFormatL1ReversedUnorm" },
		{ kDataFormatA1ReversedUnorm, "kDataFormatA1ReversedUnorm" },
		{ kDataFormatR1ReversedUint, "kDataFormatR1ReversedUint" },
		{ kDataFormatL1ReversedUint, "kDataFormatL1ReversedUint" },
		{ kDataFormatA1ReversedUint, "kDataFormatA1ReversedUint" },
		{ kDataFormatBc1Unorm, "kDataFormatBc1Unorm" },
		{ kDataFormatBc1UBNorm, "kDataFormatBc1UBNorm" },
		{ kDataFormatBc1UnormSrgb, "kDataFormatBc1UnormSrgb" },
		{ kDataFormatBc2Unorm, "kDataFormatBc2Unorm" },
		{ kDataFormatBc2UBNorm, "kDataFormatBc2UBNorm" },
		{ kDataFormatBc2UnormSrgb, "kDataFormatBc2UnormSrgb" },
		{ kDataFormatBc3Unorm, "kDataFormatBc3Unorm" },
		{ kDataFormatBc3UBNorm, "kDataFormatBc3UBNorm" },
		{ kDataFormatBc3UnormSrgb, "kDataFormatBc3UnormSrgb" },
		{ kDataFormatBc4Unorm, "kDataFormatBc4Unorm" },
		{ kDataFormatBc4Snorm, "kDataFormatBc4Snorm" },
		{ kDataFormatBc5Unorm, "kDataFormatBc5Unorm" },
		{ kDataFormatBc5Snorm, "kDataFormatBc5Snorm" },
		{ kDataFormatBc6Unorm, "kDataFormatBc6Unorm" },
		{ kDataFormatBc6Snorm, "kDataFormatBc6Snorm" },
		{ kDataFormatBc6Uf16, "kDataFormatBc6Uf16" },
		{ kDataFormatBc6Sf16, "kDataFormatBc6Sf16" },
		{ kDataFormatBc7Unorm, "kDataFormatBc7Unorm" },
		{ kDataFormatBc7UBNorm, "kDataFormatBc7UBNorm" },
		{ kDataFormatBc7UnormSrgb, "kDataFormatBc7UnormSrgb" },
		{ kDataFormatB5G6R5Unorm, "kDataFormatB5G6R5Unorm" },
		{ kDataFormatR5G5B5A1Unorm, "kDataFormatR5G5B5A1Unorm" },
		{ kDataFormatB5G5R5A1Unorm, "kDataFormatB5G5R5A1Unorm" },
		{ kDataFormatB8G8R8A8Unorm, "kDataFormatB8G8R8A8Unorm" },
		{ kDataFormatB8G8R8X8Unorm, "kDataFormatB8G8R8X8Unorm" },
		{ kDataFormatB8G8R8A8UnormSrgb, "kDataFormatB8G8R8A8UnormSrgb" },
		{ kDataFormatB8G8R8X8UnormSrgb, "kDataFormatB8G8R8X8UnormSrgb" },
		{ kDataFormatR4G4B4A4Unorm, "kDataFormatR4G4B4A4Unorm" },
		{ kDataFormatB4G4R4A4Unorm, "kDataFormatB4G4R4A4Unorm" },
		{ kDataFormatB4G4R4X4Unorm, "kDataFormatB4G4R4X4Unorm" },
		{ kDataFormatB5G5R5X1Unorm, "kDataFormatB5G5R5X1Unorm" },
		{ kDataFormatR5G6B5Unorm, "kDataFormatR5G6B5Unorm" },
		{ kDataFormatB10G10R10A2Unorm, "kDataFormatB10G10R10A2Unorm" },
		{ kDataFormatR10G10B10A2Unorm, "kDataFormatR10G10B10A2Unorm" },
		{ kDataFormatR10G10B10A2Uint, "kDataFormatR10G10B10A2Uint" },
		{ kDataFormatB10G10R10A2Uint, "kDataFormatB10G10R10A2Uint" },
		{ kDataFormatB16G16R16A16Float, "kDataFormatB16G16R16A16Float" },
		{ kDataFormatR11G11B10Unorm, "kDataFormatR11G11B10Unorm" },
		{ kDataFormatR11G11B10Snorm, "kDataFormatR11G11B10Snorm" },
		{ kDataFormatB10G11R11Unorm, "kDataFormatB10G11R11Unorm" },
		{ kDataFormatB10G11R11Snorm, "kDataFormatB10G11R11Snorm" },
		{ kDataFormatR9G9B9E5Float, "kDataFormatR9G9B9E5Float" },
		{ kDataFormatB8G8R8G8Unorm, "kDataFormatB8G8R8G8Unorm" },
		{ kDataFormatG8B8G8R8Unorm, "kDataFormatG8B8G8R8Unorm" },
		{ kDataFormatBc1UnormNoAlpha, "kDataFormatBc1UnormNoAlpha" },
		{ kDataFormatBc1UnormSrgbNoAlpha, "kDataFormatBc1UnormSrgbNoAlpha" },
		{ kDataFormatBc7UnormNoAlpha, "kDataFormatBc7UnormNoAlpha" },
		{ kDataFormatBc7UnormSrgbNoAlpha, "kDataFormatBc7UnormSrgbNoAlpha" },
		{ kDataFormatBc3UnormRABG, "kDataFormatBc3UnormRABG" }
	};

	const char* fmtName = nullptr;
	auto iter = formatMap.find(dataFmt);
	if (iter != formatMap.end())
	{
		fmtName = iter->second;
	}
	else
	{
		LOG_WARN("data format not found %X", dataFmt.m_asInt);
		fmtName = nullptr;
	}
	return fmtName;
}

DataFormat DataFormat::build(SurfaceFormat surfFmt, TextureChannelType channelType, TextureChannel chanX /*= kTextureChannelX*/, TextureChannel chanY /*= kTextureChannelY*/, TextureChannel chanZ /*= kTextureChannelZ*/, TextureChannel chanW /*= kTextureChannelW*/)
{
	DataFormat result{};
	result.m_bits.m_surfaceFormat = surfFmt;
	result.m_bits.m_channelType   = channelType;
	result.m_bits.m_channelX      = chanX;
	result.m_bits.m_channelY      = chanY;
	result.m_bits.m_channelZ      = chanZ;
	result.m_bits.m_channelW      = chanW;
	result.m_bits.m_unused        = 0;

	return result;
}

DataFormat DataFormat::build(RenderTargetFormat rtFmt, RenderTargetChannelType rtChannelType, RenderTargetChannelOrder channelOrder)
{
	const uint32_t channelCountTab[kRenderTargetFormatX24_8_32 + 1] = {
		0, 1, 1, 2, 1, 2, 3, 3, 4, 4, 4, 2, 4, 0, 4, 0, 3, 4, 4, 4, 2, 2, 3
	};

	uint32_t channelCount = channelCountTab[rtFmt];

	DataFormat result             = { { 0 } };
	result.m_bits.m_surfaceFormat = rtFmt;
	result.m_bits.m_channelType   = rtChannelType;

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
		case 1:
		{
			result.m_bits.m_channelX = kTextureChannelY;
		}
		break;
		case 2:
		{
			result.m_bits.m_channelX = kTextureChannelX;
			result.m_bits.m_channelY = kTextureChannelW;
		}
		break;
		case 3:
		{
			result.m_bits.m_channelX = kTextureChannelX;
			result.m_bits.m_channelY = kTextureChannelY;
			result.m_bits.m_channelZ = kTextureChannelW;
		}
		break;
		case 4:
		{
			result.m_bits.m_channelX = kTextureChannelZ;
			result.m_bits.m_channelY = kTextureChannelY;
			result.m_bits.m_channelZ = kTextureChannelX;
			result.m_bits.m_channelW = kTextureChannelW;
		}
		break;
		}
	}
	break;
	case kRenderTargetChannelOrderReversed:
	{
		switch (channelCount)
		{
		case 1:
		{
			result.m_bits.m_channelX = kTextureChannelZ;
		}
		break;
		case 2:
		{
			result.m_bits.m_channelX = kTextureChannelY;
			result.m_bits.m_channelY = kTextureChannelX;
		}
		break;
		case 3:
		{
			result.m_bits.m_channelX = kTextureChannelZ;
			result.m_bits.m_channelY = kTextureChannelY;
			result.m_bits.m_channelZ = kTextureChannelX;
		}
		break;
		case 4:
		{
			result.m_bits.m_channelX = kTextureChannelW;
			result.m_bits.m_channelY = kTextureChannelZ;
			result.m_bits.m_channelZ = kTextureChannelY;
			result.m_bits.m_channelW = kTextureChannelX;
		}
		break;
		}
	}
	break;
	case kRenderTargetChannelOrderAltReversed:
	{
		switch (channelCount)
		{
		case 1:
		{
			result.m_bits.m_channelX = kTextureChannelW;
		}
		break;
		case 2:
		{
			result.m_bits.m_channelX = kTextureChannelW;
			result.m_bits.m_channelY = kTextureChannelX;
		}
		break;
		case 3:
		{
			result.m_bits.m_channelX = kTextureChannelW;
			result.m_bits.m_channelY = kTextureChannelY;
			result.m_bits.m_channelZ = kTextureChannelX;
		}
		break;
		case 4:
		{
			result.m_bits.m_channelX = kTextureChannelW;
			result.m_bits.m_channelY = kTextureChannelX;
			result.m_bits.m_channelZ = kTextureChannelY;
			result.m_bits.m_channelW = kTextureChannelZ;
		}
		break;
		}
	}
	break;
	}

	return result;
}

uint32_t DataFormat::getTotalBitsPerElement() const
{
	uint32_t surfFmt = m_bits.m_surfaceFormat;
	uint32_t result  = 0LL;
	if ((unsigned int)surfFmt <= kSurfaceFormat1Reversed)
	{
		uint32_t texelsPerElement = getTexelsPerElement();
		result = (unsigned int)(s_bitsPerElement[surfFmt] * texelsPerElement);
	}
	return result;
}

uint32_t DataFormat::getTexelsPerElement(void) const
{
	uint32_t result = 0;

	if (m_bits.m_surfaceFormat >= kSurfaceFormatBc1 && m_bits.m_surfaceFormat <= kSurfaceFormatBc7)
	{
		result = 16;
	}
	else if (m_bits.m_surfaceFormat >= kSurfaceFormat1)
	{
		result = 8;
	}
	else
	{
		result = 1;
	}

	return result;
}

ZFormat DataFormat::getZFormat(void) const
{
	ZFormat fmt = kZFormat32Float;
	if (m_bits.m_surfaceFormat != kSurfaceFormat32)
	{
		fmt = kZFormat16;
	}
	return fmt;
}

StencilFormat DataFormat::getStencilFormat(void) const
{
	return static_cast<StencilFormat>(m_bits.m_surfaceFormat == kSurfaceFormat8);
}

uint32_t DataFormat::getTotalBytesPerElement(void) const
{
	return getTotalBitsPerElement() / 8;
}

bool DataFormat::isBlockCompressedFormat(void) const
{
	return (m_bits.m_surfaceFormat >= kSurfaceFormatBc1) && (m_bits.m_surfaceFormat <= kSurfaceFormatBc7);
}

bool DataFormat::operator==(const DataFormat& other) const
{
	return m_asInt == other.m_asInt;
}
