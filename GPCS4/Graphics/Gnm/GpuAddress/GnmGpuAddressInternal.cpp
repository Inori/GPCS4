#include "GnmGpuAddressInternal.h"
using namespace sce;
using namespace sce::GpuAddress;

LOG_CHANNEL("GpuAddress");

uint32_t sce::GpuAddress::nextPowerOfTwo(uint32_t x)
{
	x = x-1;
	x |= (x>>1);
	x |= (x>>2);
	x |= (x>>4);
	x |= (x>>8);
	x |= (x>>16);
	return x+1;
}

bool sce::GpuAddress::isPowerOfTwo(uint32_t x)
{
	return (x > 0) && ( (x & (x-1)) == 0 );
}

uint32_t sce::GpuAddress::fastLog2(float f)
{
	union IntToFloat
	{
		unsigned int asInt;
		float asFloat;
	} convert;
	convert.asFloat = f;
	return (convert.asInt >> 23U) - 127U;
}

uint32_t sce::GpuAddress::getPipeCount(Gnm::PipeConfig pipeConfig)
{
	switch(pipeConfig)
	{
	case Gnm::kPipeConfigP8_32x32_8x16:
	case Gnm::kPipeConfigP8_32x32_16x16:
		return 8;
	case Gnm::kPipeConfigP16:
		return 16;
	default:
		SCE_GNM_ERROR("Unknown PipeConfig %d\n", pipeConfig);
		return 0;
	}
}

Gnm::ArrayMode sce::GpuAddress::degradeLargeThickArrayMode(Gnm::ArrayMode oldArrayMode, uint32_t bitsPerElement)
{
	// For macro tile modes, when tile width (8) * tile height (8) * thickness * element_bytes > row_size,
	// it's better (and required) to use a thinner array mode
	uint32_t thickness = getMicroTileThickness(oldArrayMode);
	if (thickness > 1)
	{
		uint32_t tileSize = kMicroTileWidth*kMicroTileHeight*thickness*bitsPerElement/8;
		if (tileSize > kDramRowSize)
		{
			switch(oldArrayMode)
			{
			case Gnm::kArrayMode2dTiledXThick:
				if (tileSize/2 <= kDramRowSize)
				{
					return Gnm::kArrayMode2dTiledThick;
				}
				// fall through
			case Gnm::kArrayMode2dTiledThick:
				return Gnm::kArrayMode2dTiledThin;
			case Gnm::kArrayMode3dTiledXThick:
				if (tileSize/2 <= kDramRowSize)
				{
					return Gnm::kArrayMode3dTiledThick;
				}
				// fall through
			case Gnm::kArrayMode3dTiledThick:
				return Gnm::kArrayMode3dTiledThin;
				break;
			case Gnm::kArrayModeTiledThickPrt:
				return Gnm::kArrayModeTiledThinPrt;
				break;
			case Gnm::kArrayMode2dTiledThickPrt:
				return Gnm::kArrayMode2dTiledThinPrt;
				break;
			case Gnm::kArrayMode3dTiledThickPrt:
				return Gnm::kArrayMode3dTiledThinPrt;
				break;
			case Gnm::kArrayMode1dTiledThick:
				return oldArrayMode; // No conversion for 1D thick modes
			default:
				SCE_GNM_ERROR("Unexpected arrayMode (0x%02X).", oldArrayMode); // all other modes have thickness == 1, and shouldn't make it into this block
				break;
			}
		}
	}
	return oldArrayMode;
}

uint32_t sce::GpuAddress::getMicroTileThickness(Gnm::ArrayMode arrayMode)
{
	uint32_t thickness = 1;
	switch(arrayMode)
	{
	case Gnm::kArrayMode1dTiledThick:
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayModeTiledThickPrt:
	case Gnm::kArrayMode2dTiledThickPrt:
	case Gnm::kArrayMode3dTiledThickPrt:
		thickness = 4;
		break;
	case Gnm::kArrayMode2dTiledXThick:
	case Gnm::kArrayMode3dTiledXThick:
		thickness = 8;
		break;
	case Gnm::kArrayModeLinearGeneral:
	case Gnm::kArrayModeLinearAligned:
	case Gnm::kArrayMode1dTiledThin:
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayModeTiledThinPrt:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThin:
		break; // thickness = 1, above
	default:
		SCE_GNM_ERROR("Invalid arrayMode (0x%02X).", arrayMode);
		break;
	}
	return thickness;
}

bool sce::GpuAddress::isLinear(Gnm::ArrayMode arrayMode)
{
	const uint64_t linearTiledMask = 0
		| (1 << Gnm::kArrayModeLinearGeneral)   // 0x00000000
		| (1 << Gnm::kArrayModeLinearAligned)   // 0x00000001
		| (0 << Gnm::kArrayMode1dTiledThin)     // 0x00000002
		| (0 << Gnm::kArrayMode1dTiledThick)    // 0x00000003
		| (0 << Gnm::kArrayMode2dTiledThin)     // 0x00000004
		| (0 << Gnm::kArrayModeTiledThinPrt)    // 0x00000005
		| (0 << Gnm::kArrayMode2dTiledThinPrt)  // 0x00000006
		| (0 << Gnm::kArrayMode2dTiledThick)    // 0x00000007
		| (0 << Gnm::kArrayMode2dTiledXThick)   // 0x00000008
		| (0 << Gnm::kArrayModeTiledThickPrt)   // 0x00000009
		| (0 << Gnm::kArrayMode2dTiledThickPrt) // 0x0000000a
		| (0 << Gnm::kArrayMode3dTiledThinPrt)  // 0x0000000b
		| (0 << Gnm::kArrayMode3dTiledThin)     // 0x0000000c
		| (0 << Gnm::kArrayMode3dTiledThick)    // 0x0000000d
		| (0 << Gnm::kArrayMode3dTiledXThick)   // 0x0000000e
		| (0 << Gnm::kArrayMode3dTiledThickPrt) // 0x0000000f
		;
	return (linearTiledMask & (1ULL<<arrayMode)) != 0;
}
bool sce::GpuAddress::isMicroTiled(Gnm::ArrayMode arrayMode)
{
	const uint64_t microTiledMask = 0
		| (0 << Gnm::kArrayModeLinearGeneral)   // 0x00000000
		| (0 << Gnm::kArrayModeLinearAligned)   // 0x00000001
		| (1 << Gnm::kArrayMode1dTiledThin)     // 0x00000002
		| (1 << Gnm::kArrayMode1dTiledThick)    // 0x00000003
		| (0 << Gnm::kArrayMode2dTiledThin)     // 0x00000004
		| (0 << Gnm::kArrayModeTiledThinPrt)    // 0x00000005
		| (0 << Gnm::kArrayMode2dTiledThinPrt)  // 0x00000006
		| (0 << Gnm::kArrayMode2dTiledThick)    // 0x00000007
		| (0 << Gnm::kArrayMode2dTiledXThick)   // 0x00000008
		| (0 << Gnm::kArrayModeTiledThickPrt)   // 0x00000009
		| (0 << Gnm::kArrayMode2dTiledThickPrt) // 0x0000000a
		| (0 << Gnm::kArrayMode3dTiledThinPrt)  // 0x0000000b
		| (0 << Gnm::kArrayMode3dTiledThin)     // 0x0000000c
		| (0 << Gnm::kArrayMode3dTiledThick)    // 0x0000000d
		| (0 << Gnm::kArrayMode3dTiledXThick)   // 0x0000000e
		| (0 << Gnm::kArrayMode3dTiledThickPrt) // 0x0000000f
		;
	return (microTiledMask & (1ULL<<arrayMode)) != 0;
}
bool sce::GpuAddress::isMacroTiled(Gnm::ArrayMode arrayMode)
{
	const uint64_t macroTiledMask = 0
		| (0 << Gnm::kArrayModeLinearGeneral)   // 0x00000000
		| (0 << Gnm::kArrayModeLinearAligned)   // 0x00000001
		| (0 << Gnm::kArrayMode1dTiledThin)     // 0x00000002
		| (0 << Gnm::kArrayMode1dTiledThick)    // 0x00000003
		| (1 << Gnm::kArrayMode2dTiledThin)     // 0x00000004
		| (1 << Gnm::kArrayModeTiledThinPrt)    // 0x00000005
		| (1 << Gnm::kArrayMode2dTiledThinPrt)  // 0x00000006
		| (1 << Gnm::kArrayMode2dTiledThick)    // 0x00000007
		| (1 << Gnm::kArrayMode2dTiledXThick)   // 0x00000008
		| (1 << Gnm::kArrayModeTiledThickPrt)   // 0x00000009
		| (1 << Gnm::kArrayMode2dTiledThickPrt) // 0x0000000a
		| (1 << Gnm::kArrayMode3dTiledThinPrt)  // 0x0000000b
		| (1 << Gnm::kArrayMode3dTiledThin)     // 0x0000000c
		| (1 << Gnm::kArrayMode3dTiledThick)    // 0x0000000d
		| (1 << Gnm::kArrayMode3dTiledXThick)   // 0x0000000e
		| (1 << Gnm::kArrayMode3dTiledThickPrt) // 0x0000000f
		;
	return (macroTiledMask & (1ULL<<arrayMode)) != 0;
}
bool sce::GpuAddress::isPartiallyResidentTexture(Gnm::ArrayMode arrayMode)
{
	const uint64_t prtTiledMask = 0
		| (0 << Gnm::kArrayModeLinearGeneral)   // 0x00000000
		| (0 << Gnm::kArrayModeLinearAligned)   // 0x00000001
		| (0 << Gnm::kArrayMode1dTiledThin)     // 0x00000002
		| (0 << Gnm::kArrayMode1dTiledThick)    // 0x00000003
		| (0 << Gnm::kArrayMode2dTiledThin)     // 0x00000004
		| (1 << Gnm::kArrayModeTiledThinPrt)    // 0x00000005
		| (1 << Gnm::kArrayMode2dTiledThinPrt)  // 0x00000006
		| (0 << Gnm::kArrayMode2dTiledThick)    // 0x00000007
		| (0 << Gnm::kArrayMode2dTiledXThick)   // 0x00000008
		| (1 << Gnm::kArrayModeTiledThickPrt)   // 0x00000009
		| (1 << Gnm::kArrayMode2dTiledThickPrt) // 0x0000000a
		| (1 << Gnm::kArrayMode3dTiledThinPrt)  // 0x0000000b
		| (0 << Gnm::kArrayMode3dTiledThin)     // 0x0000000c
		| (0 << Gnm::kArrayMode3dTiledThick)    // 0x0000000d
		| (0 << Gnm::kArrayMode3dTiledXThick)   // 0x0000000e
		| (1 << Gnm::kArrayMode3dTiledThickPrt) // 0x0000000f
		;
	return (prtTiledMask & (1ULL<<arrayMode)) != 0;
}
