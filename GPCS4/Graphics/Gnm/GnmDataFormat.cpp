#include "GnmDataFormat.h"
#include "UtilBit.h"

LOG_CHANNEL(Graphic.Gnm.GnmDataFormat);

namespace sce::Gnm
{

	const char* dataFormatName(DataFormat dataFmt)
	{
		const static std::unordered_map<DataFormat, const char*, DataFormatHash> formatMap = {
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
		auto        iter    = formatMap.find(dataFmt);
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

		DataFormat result             = { { { 0 } } };
		result.m_bits.m_surfaceFormat = rtFmt;

		// From IDA
		TextureChannelType channelType = kTextureChannelTypeSrgb;
		if (rtChannelType != kRenderTargetChannelTypeSrgb)
		{
			channelType = static_cast<TextureChannelType>(rtChannelType & 0xF);
		}
		result.m_bits.m_channelType = channelType;

		switch (channelOrder)
		{
			case kRenderTargetChannelOrderStandard:
			{
				switch (channelCount)
				{
					case 4:
						result.m_bits.m_channelW = kTextureChannelW;
					case 3:
						result.m_bits.m_channelZ = kTextureChannelZ;
					case 2:
						result.m_bits.m_channelY = kTextureChannelY;
					case 1:
						result.m_bits.m_channelX = kTextureChannelX;
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

	DataFormat DataFormat::build(ZFormat zFmt)
	{
		DataFormat result;
		uint8_t    depthFormats[] = { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0 };
		uint32_t   channelTypes[] = { 0, 0, 0, 7 };

		result.m_bits.m_surfaceFormat = depthFormats[zFmt * 4];
		result.m_bits.m_channelType   = channelTypes[zFmt];
		result.m_bits.m_channelX      = kTextureChannelX;
		result.m_bits.m_channelY      = kTextureChannelX;
		result.m_bits.m_channelZ      = kTextureChannelX;
		result.m_bits.m_channelW      = kTextureChannelX;
		return result;
	}

	DataFormat DataFormat::build(NumSamples numSamples, NumFragments numFragments)
	{
		DataFormat result = {};

		uint32_t fmtType = 0;
		uint32_t channel = 0;

		fmtType = 0x442C;
		switch (numFragments | 16 * numSamples)
		{
			case 0x10:
				break;
			case 0x11:
				fmtType = 0x442F;
				break;
			case 0x20:
				fmtType = 0x442D;
				break;
			case 0x21:
				fmtType = 0x4430;
				break;
			case 0x22:
				fmtType = 0x4431;
				break;
			case 0x30:
				fmtType = 0x442E;
				break;
			case 0x31:
				fmtType = 0x4433;
				break;
			case 0x32:
				fmtType = 0x4435;
				break;
			case 0x33:
				fmtType = 0x4436;
				break;
			case 0x40:
				fmtType = 0x4432;
				break;
			case 0x41:
				fmtType = 0x4434;
				break;
			case 0x42:
				fmtType = 0x4437;
				break;
			case 0x43:
				fmtType = 0x4438;
				break;
			default:
				fmtType = 0x4400;
				break;
		}

		channel = 0x220000;
		if (numSamples == kNumSamples16)
		{
			channel = 0x228000;
		}

		result.m_asInt = ((numSamples == kNumSamples16) << 18) | fmtType | (unsigned int)channel;
		return result;
	}

	DataFormat DataFormat::build(StencilFormat sFmt, TextureChannelType channelType)
	{
		DataFormat result             = {};
		result.m_bits.m_surfaceFormat = (sFmt == kStencil8 ? sFmt : kStencilInvalid);
		result.m_bits.m_channelType   = channelType;
		result.m_bits.m_channelX      = kTextureChannelX;
		result.m_bits.m_channelY      = kTextureChannelX;
		result.m_bits.m_channelZ      = kTextureChannelX;
		result.m_bits.m_channelW      = kTextureChannelX;
		return result;
	}

	uint32_t DataFormat::getBytesPerElement(void) const
	{
		return getBitsPerElement() / 8;
	}

	uint32_t DataFormat::getBitsPerElement(void) const
	{
		uint32_t surfFmt = m_bits.m_surfaceFormat;
		uint32_t result  = 0LL;
		if ((unsigned int)surfFmt <= kSurfaceFormat1Reversed)
		{
			result = (unsigned int)(s_bitsPerElement[surfFmt]);
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
			result                    = (unsigned int)(s_bitsPerElement[surfFmt] * texelsPerElement);
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

	SurfaceFormat DataFormat::getSurfaceFormat(void) const
	{
		return (SurfaceFormat)m_bits.m_surfaceFormat;
	}

	TextureChannel DataFormat::getChannel(uint32_t chanIdx) const
	{
		// From IDA
		TextureChannel result = (TextureChannel)2;
		if (chanIdx <= 3)
		{
			TextureChannel channels[4] = {
				(TextureChannel)m_bits.m_channelX,
				(TextureChannel)m_bits.m_channelY,
				(TextureChannel)m_bits.m_channelZ,
				(TextureChannel)m_bits.m_channelW,
			};
			result = channels[chanIdx];
		}
		return result;
	}

	TextureChannelType DataFormat::getTextureChannelType() const
	{
		return (TextureChannelType)m_bits.m_channelType;
	}

	uint32_t DataFormat::getNumComponents(void) const
	{
		// From IDA
		constexpr const int numComponentsPerElement[] = {
			0, 1, 1, 2, 1, 2, 3, 3,
			4, 4, 4, 2, 4, 3, 4, -1,
			3, 4, 4, 4, 2, 2, 2, -1,
			-1, -1, -1, -1, -1, -1, -1, -1,
			3, 3, 3, 4, 4, 4, 1, 2,
			3, 4, -1, -1, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 3, 1, 1
		};

		uint32_t numComponents = 0;
		if (m_bits.m_surfaceFormat <= kSurfaceFormat1Reversed)
		{
			numComponents = static_cast<uint32_t>(numComponentsPerElement[m_bits.m_surfaceFormat]);
		}
		return numComponents;
	}

	bool DataFormat::operator==(const DataFormat& other) const
	{
		return m_asInt == other.m_asInt;
	}

	bool DataFormat::getRenderTargetChannelType(RenderTargetChannelType* outType) const
	{
		// From IDA.
		bool result = false;
		switch (m_bits.m_channelType)
		{
			case 0:
				result = true;
				if (outType)
					*outType = kRenderTargetChannelTypeUNorm;
				break;
			case 1:
				result = true;
				if (outType)
					*outType = kRenderTargetChannelTypeSNorm;
				break;
			case 4:
				result = true;
				if (outType)
					*outType = kRenderTargetChannelTypeUInt;
				break;
			case 5:
				result = true;
				if (outType)
					*outType = kRenderTargetChannelTypeSInt;
				break;
			case 7:
				result = true;
				if (outType)
					*outType = kRenderTargetChannelTypeFloat;
				break;
			case 9:
				result = true;
				if (outType)
					*outType = kRenderTargetChannelTypeSrgb;
				break;
			default:
				return result;
		}
		return result;
	}

	bool DataFormat::getRenderTargetChannelOrder(RenderTargetChannelOrder* outOrder) const
	{
		// From IDA.
		//
		// TODO:
		// refactor these shit.

		unsigned int*    a1 = (unsigned int*)this;
		int*             a2 = (int*)outOrder;
		unsigned int     v2;      // ecx
		uint64_t		 v3;      // rax
		unsigned int     v4;      // edx
		int              v5;      // er11
		int              v6;      // er10
		int              v7;      // er8
		int              v8;      // er9
		signed int       v9;      // edi
		char             result;  // al
		int64_t			 v11;     // rdi
		bool             v12;     // dl
		bool             v13;     // cl

		v2 = *a1;
		v3 = (uint8_t)*a1;
		if ((uint8_t)*a1 > 0x3Cu)
			return 0;
		v4 = s_numComponentsPerElement[(uint8_t)v2];
		if (v4 - 1 > 3)
			return 0;
		v5 = (v2 >> 12) & 7;
		v6 = (v2 >> 15) & 7;
		v7 = (v2 >> 21) & 7;
		if (v4 == 2)
		{
			if (v5 == 4)
			{
				v9 = 0;
				if (v6 == 5)
					goto LABEL_42;
			}
			if (v5 == 4)
			{
				v9 = 1;
				if (v7 == 5)
					goto LABEL_42;
			}
			if (v6 == 4)
			{
				v9 = 2;
				if (v5 == 5)
					goto LABEL_42;
			}
			result = 0;
			goto LABEL_31;
		}
		v8 = (v2 >> 18) & 7;
		if (v4 == 1)
		{
			v9 = 0;
			if (v5 != 4)
			{
				v9 = 1;
				if (v6 != 4)
				{
					v9 = 2;
					if (v8 != 4)
					{
						v9 = 3;
						if (v7 != 4)
							return 0;
					}
				}
			}
			goto LABEL_42;
		}
		v11 = 0x4000107000120C0LL;
		v12 = v5 == 4 && v6 == 5;
		v13 = v12 && v8 == 6;
		if (util::bit::bittest64(&v11, v3))
		{
			v9 = 0;
			if (v13)
				goto LABEL_42;
			v9 = 1;
			if (v12 && v7 == 6)
				goto LABEL_42;
			if (v5 == 6 && v8 == 4)
			{
				v9 = 2;
				if (v6 == 5)
					goto LABEL_42;
			}
			result = 0;
			if (v5 == 6 && v7 == 4)
			{
				v9 = 3;
				if (v6 == 5)
					goto LABEL_42;
			}
		}
		else
		{
			v9 = 0;
			if (v13 && v7 == 7)
				goto LABEL_42;
			if (v7 == 7 && v5 == 6 && v8 == 4)
			{
				v9 = 1;
				if (v6 == 5)
					goto LABEL_42;
			}
			if (v5 == 7 && v6 == 6 && v7 == 4)
			{
				v9 = 2;
				if (v8 == 5)
					goto LABEL_42;
			}
			result = 0;
			if (v8 == 7 && v6 == 6)
			{
			LABEL_31:
				if (v7 != 4)
					return result;
				v9 = 3;
				if (v5 != 5)
					return result;
			LABEL_42:
				result = 1;
				if (a2)
					*a2 = v9;
				return result;
			}
		}
		return result;
	}

}  // namespace sce::Gnm
