#include "GnmGpuAddressTool.h"
#include "GnmGpuAddress.h"

LOG_CHANNEL(Graphic.Gnm.GpuAddress);

namespace GpuAddress
{;

uint32_t nextPowerOfTwo(uint32_t x)
{
	x = x - 1;
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return x + 1;
}

bool isPowerOfTwo(uint32_t x)
{
	return (x > 0) && ((x & (x - 1)) == 0);
}

uint32_t getPipeCount(PipeConfig pipeConfig)
{
	switch (pipeConfig)
	{
	case kPipeConfigP8_32x32_8x16:
	case kPipeConfigP8_32x32_16x16:
		return 8;
	case kPipeConfigP16:
		return 16;
	default:
		LOG_ERR("Unknown PipeConfig %d\n", pipeConfig);
		return 0;
	}
}

bool isLinear(ArrayMode arrayMode)
{
	const uint64_t linearTiledMask = 0 | (1 << kArrayModeLinearGeneral)
									 | (1 << kArrayModeLinearAligned)
									 | (0 << kArrayMode1dTiledThin)
									 | (0 << kArrayMode1dTiledThick)
									 | (0 << kArrayMode2dTiledThin)
									 | (0 << kArrayModeTiledThinPrt)
									 | (0 << kArrayMode2dTiledThinPrt)
									 | (0 << kArrayMode2dTiledThick)
									 | (0 << kArrayMode2dTiledXThick)
									 | (0 << kArrayModeTiledThickPrt)
									 | (0 << kArrayMode2dTiledThickPrt)
									 | (0 << kArrayMode3dTiledThinPrt)
									 | (0 << kArrayMode3dTiledThin)
									 | (0 << kArrayMode3dTiledThick)
									 | (0 << kArrayMode3dTiledXThick)
									 | (0 << kArrayMode3dTiledThickPrt);
	return (linearTiledMask & (1ULL << arrayMode)) != 0;
}

bool isMacroTiled(ArrayMode arrayMode)
{
	const uint64_t macroTiledMask = 0 | (0 << kArrayModeLinearGeneral) 
									| (0 << kArrayModeLinearAligned)
									| (0 << kArrayMode1dTiledThin) 
									| (0 << kArrayMode1dTiledThick)    
									| (1 << kArrayMode2dTiledThin)     
									| (1 << kArrayModeTiledThinPrt)    
									| (1 << kArrayMode2dTiledThinPrt)  
									| (1 << kArrayMode2dTiledThick)    
									| (1 << kArrayMode2dTiledXThick)   
									| (1 << kArrayModeTiledThickPrt)   
									| (1 << kArrayMode2dTiledThickPrt) 
									| (1 << kArrayMode3dTiledThinPrt)  
									| (1 << kArrayMode3dTiledThin)     
									| (1 << kArrayMode3dTiledThick)    
									| (1 << kArrayMode3dTiledXThick)   
									| (1 << kArrayMode3dTiledThickPrt) 
		;
	return (macroTiledMask & (1ULL << arrayMode)) != 0;
}

uint32_t getMicroTileThickness(ArrayMode arrayMode)
{
	uint32_t thickness = 1;
	switch (arrayMode)
	{
	case kArrayMode1dTiledThick:
	case kArrayMode2dTiledThick:
	case kArrayMode3dTiledThick:
	case kArrayModeTiledThickPrt:
	case kArrayMode2dTiledThickPrt:
	case kArrayMode3dTiledThickPrt:
		thickness = 4;
		break;
	case kArrayMode2dTiledXThick:
	case kArrayMode3dTiledXThick:
		thickness = 8;
		break;
	case kArrayModeLinearGeneral:
	case kArrayModeLinearAligned:
	case kArrayMode1dTiledThin:
	case kArrayMode2dTiledThin:
	case kArrayModeTiledThinPrt:
	case kArrayMode2dTiledThinPrt:
	case kArrayMode3dTiledThinPrt:
	case kArrayMode3dTiledThin:
		break;  // thickness = 1, above
	default:
		LOG_ERR("Invalid arrayMode (0x%02X).", arrayMode);
		break;
	}
	return thickness;
}


ArrayMode degradeLargeThickArrayMode(ArrayMode oldArrayMode, uint32_t bitsPerElement)
{
	uint32_t thickness = getMicroTileThickness(oldArrayMode);
	if (thickness > 1)
	{
		uint32_t tileSize = kMicroTileWidth * kMicroTileHeight * thickness * bitsPerElement / 8;
		if (tileSize > kDramRowSize)
		{
			switch (oldArrayMode)
			{
			case kArrayMode2dTiledXThick:
				if (tileSize / 2 <= kDramRowSize)
				{
					return kArrayMode2dTiledThick;
				}
				// fall through
			case kArrayMode2dTiledThick:
				return kArrayMode2dTiledThin;
			case kArrayMode3dTiledXThick:
				if (tileSize / 2 <= kDramRowSize)
				{
					return kArrayMode3dTiledThick;
				}
				// fall through
			case kArrayMode3dTiledThick:
				return kArrayMode3dTiledThin;
				break;
			case kArrayModeTiledThickPrt:
				return kArrayModeTiledThinPrt;
				break;
			case kArrayMode2dTiledThickPrt:
				return kArrayMode2dTiledThinPrt;
				break;
			case kArrayMode3dTiledThickPrt:
				return kArrayMode3dTiledThinPrt;
				break;
			case kArrayMode1dTiledThick:
				return oldArrayMode;  // No conversion for 1D thick modes
			default:
				LOG_ERR("Unexpected arrayMode (0x%02X).", oldArrayMode);  // all other modes have thickness == 1, and shouldn't make it into this block
				break;
			}
		}
	}
	return oldArrayMode;
}

uint32_t fastIntLog2(uint32_t i)
{
	return 31 - __builtin_clz(i | 1);
}

}  // namespace GpuAddress