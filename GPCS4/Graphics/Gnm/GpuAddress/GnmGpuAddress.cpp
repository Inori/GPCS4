#include "GnmGpuAddress.h"
#include "GnmGpuAddressInternal.h"

using namespace sce;
using namespace sce::GpuAddress;

#include "Gnm/GnmTexture.h"
#include "Gnm/GnmRenderTarget.h"
#include "Gnm/GnmDepthRenderTarget.h"


#include <algorithm>

LOG_CHANNEL("GpuAddress");

int32_t sce::GpuAddress::getFlagsForSurfaceType(Gnm::GpuMode minGpuMode, SurfaceFlags *outFlags, SurfaceType surfaceType, SurfaceMipmapMode mipmapMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outFlags != NULL, kStatusInvalidArgument, "outFlags must not be NULL.");
	outFlags->m_value = 0;
	switch(surfaceType)
	{
	case kSurfaceTypeColorTargetDisplayable:
		//outFlags->m_display = 1;
		break;
	case kSurfaceTypeColorTarget:
		break;
	case kSurfaceTypeDepthTarget:
		outFlags->m_depthTarget = 1;
		outFlags->m_stencilTarget = 1;
        break;
	case kSurfaceTypeDepthOnlyTarget:
		outFlags->m_depthTarget = 1;
		break;
	case kSurfaceTypeStencilOnlyTarget:
		outFlags->m_stencilTarget = 1;
		break;
	case kSurfaceTypeFmaskBuffer:
		outFlags->m_fmask = 1;
		break;
	case kSurfaceTypeTextureFlat:
	case kSurfaceTypeRwTextureFlat:
		outFlags->m_pow2Pad = (mipmapMode == kSurfaceMipmapEnable) ? 1 : 0; // Must be set for textures w/mipmaps.
		outFlags->m_texCompatible = (minGpuMode == Gnm::kGpuModeNeo) ? 1 : 0;
		break;
	case kSurfaceTypeTextureVolume:
	case kSurfaceTypeRwTextureVolume:
		outFlags->m_volume = 1;
		outFlags->m_pow2Pad = (mipmapMode == kSurfaceMipmapEnable) ? 1 : 0; // Must be set for textures w/mipmaps.
		outFlags->m_texCompatible = (minGpuMode == Gnm::kGpuModeNeo) ? 1 : 0;
		break;
	case kSurfaceTypeTextureCubemap:
	case kSurfaceTypeRwTextureCubemap:
		outFlags->m_cube = 1;
		outFlags->m_pow2Pad = (mipmapMode == kSurfaceMipmapEnable) ? 1 : 0; // Must be set for textures w/mipmaps.
		outFlags->m_texCompatible = (minGpuMode == Gnm::kGpuModeNeo) ? 1 : 0;
		break;
	default:
		SCE_GNM_ERROR("Invalid surfaceType (%d).", surfaceType);
		return kStatusInvalidArgument;
	}
	return kStatusSuccess;
}


int32_t sce::GpuAddress::getArrayModeForSurfaceType(Gnm::GpuMode minGpuMode, Gnm::ArrayMode *outArrayMode, SurfaceType surfaceType, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_UNUSED(minGpuMode);
	SCE_GNM_ASSERT_MSG_RETURN(outArrayMode != NULL, kStatusInvalidArgument, "outArrayMode must not be NULL.");
	switch(surfaceType)
	{
	case kSurfaceTypeColorTargetDisplayable:
	case kSurfaceTypeColorTarget:
    case kSurfaceTypeDepthTarget:
	case kSurfaceTypeDepthOnlyTarget:
	case kSurfaceTypeStencilOnlyTarget:
	case kSurfaceTypeFmaskBuffer:
		// Macro-tiling is optimized for writing to a surface through the CB/DBs.
		*outArrayMode = Gnm::kArrayMode2dTiledThin;
		break;
	case kSurfaceTypeRwTextureFlat:
	case kSurfaceTypeRwTextureCubemap:
	case kSurfaceTypeTextureFlat:
	case kSurfaceTypeTextureCubemap:
		// multi-sampled textures must be macro-tiled.
		// Otherwise, there should be no benefit to macro-tiling for textures.
		*outArrayMode = (numFragmentsPerPixel > 1) ? Gnm::kArrayMode2dTiledThin : Gnm::kArrayMode1dTiledThin;
		break;
	case kSurfaceTypeTextureVolume:
	case kSurfaceTypeRwTextureVolume:
		*outArrayMode = Gnm::kArrayMode1dTiledThick;
		break;
	default:
		SCE_GNM_ERROR("Invalid surfaceType (%d).", surfaceType);
		return kStatusInvalidArgument;
	}
	return kStatusSuccess;
}

 int32_t sce::GpuAddress::getMicroTileModeForSurfaceType(Gnm::GpuMode minGpuMode, Gnm::MicroTileMode *outMicroTileMode, SurfaceType surfaceType)
{
	SCE_GNM_UNUSED(minGpuMode);
	SCE_GNM_ASSERT_MSG_RETURN(outMicroTileMode != NULL, kStatusInvalidArgument, "outMicroTileMode must not be NULL.");
	switch(surfaceType)
	{
	case kSurfaceTypeColorTargetDisplayable:
		*outMicroTileMode = Gnm::kMicroTileModeDisplay;
		break;
    case kSurfaceTypeDepthTarget:
	case kSurfaceTypeDepthOnlyTarget:
	case kSurfaceTypeStencilOnlyTarget:
		*outMicroTileMode = Gnm::kMicroTileModeDepth;
		break;
	case kSurfaceTypeColorTarget:
	case kSurfaceTypeFmaskBuffer:
	case kSurfaceTypeRwTextureFlat:
	case kSurfaceTypeRwTextureCubemap:
	case kSurfaceTypeTextureFlat:
	case kSurfaceTypeTextureCubemap:
		*outMicroTileMode = Gnm::kMicroTileModeThin;
		break;
	case kSurfaceTypeTextureVolume:
	case kSurfaceTypeRwTextureVolume:
		*outMicroTileMode = Gnm::kMicroTileModeThick;
		break;
	default:
		SCE_GNM_ERROR("Invalid surfaceType (%d).", surfaceType);
		return kStatusInvalidArgument;
	}
	return kStatusSuccess;
}
int32_t sce::GpuAddress::getMicroTileModeForSurfaceType(Gnm::MicroTileMode *outMicroTileMode, SurfaceType surfaceType) // DEPRECATED
{
	return getMicroTileModeForSurfaceType(Gnm::kGpuModeBase, outMicroTileMode, surfaceType);
}


int32_t sce::GpuAddress::computeSurfaceMacroTileMode(Gnm::MacroTileMode *outMacroTileMode, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outMacroTileMode != NULL, kStatusInvalidArgument, "outMacroTileMode must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(numFragmentsPerPixel) && numFragmentsPerPixel <= 8, kStatusInvalidArgument, "numFragmentsPerPixel (%u) must be 1, 2, 4, or 8.", numFragmentsPerPixel);
	Gnm::ArrayMode arrayMode;
	int32_t status = getArrayMode(&arrayMode, tileMode);
	if (status != kStatusSuccess)
		return status;
	SCE_GNM_ASSERT_MSG_RETURN(isMacroTiled(arrayMode), kStatusInvalidArgument, "tileMode (%d) must be a macro-tiled (2D or 3D) mode.", tileMode);
	SCE_GNM_ASSERT_MSG_RETURN(bitsPerElement >= 1 && bitsPerElement <= 128, kStatusInvalidArgument, "bitsPerElement must be in the range [1..128].");
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(numFragmentsPerPixel) && numFragmentsPerPixel >= 1 && numFragmentsPerPixel <= 16, kStatusInvalidArgument, "numFragmentsPerPixel (%u) must be a power of two in the range [1..16].", numFragmentsPerPixel);
	Gnm::MicroTileMode microTileMode;
	Gnm::SampleSplit sampleSplitHw;
	Gnm::TileSplit tileSplitHw;
	status = getMicroTileMode(&microTileMode, tileMode);
	SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getMicroTileMode() failed for tileMode = 0x%08X", tileMode);
	status = getSampleSplit(&sampleSplitHw, tileMode);
	SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getSampleSplit() failed for tileMode = 0x%08X", tileMode);
	status = getTileSplit(&tileSplitHw, tileMode);
	SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getTileSplit() failed for tileMode = 0x%08X", tileMode);

	uint32_t tileThickness = getMicroTileThickness(arrayMode);
	uint32_t tileBytes1x = bitsPerElement*kMicroTileWidth*kMicroTileHeight*tileThickness / 8;
	uint32_t sampleSplit = 1<<sampleSplitHw;
	uint32_t colorTileSplit = std::max(256U, sampleSplit*tileBytes1x);
	uint32_t tileSplit = (microTileMode == Gnm::kMicroTileModeDepth)
		? (64UL << tileSplitHw) : colorTileSplit;
	uint32_t tileSplitC = std::min(kDramRowSize, tileSplit);
	uint32_t tileBytes = std::min(tileSplitC, numFragmentsPerPixel*tileBytes1x);
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(tileBytes) && tileBytes >= 64 && tileBytes <= 4096, kStatusInvalidArgument, "Invalid tile split bytes (bad bitsPerElement/numElements?)"); // sanity check
	uint32_t mtmIndex = fastIntLog2(tileBytes/64);
	*outMacroTileMode = (Gnm::MacroTileMode)(isPartiallyResidentTexture(arrayMode) ? (mtmIndex+8) : mtmIndex);
	return status;
}

int32_t sce::GpuAddress::computeSurfaceTileMode(Gnm::GpuMode minGpuMode, Gnm::TileMode *outTileMode, Gnm::ArrayMode arrayMode, SurfaceFlags flags, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel, Gnm::MicroTileMode microTileMode)
{
    SCE_GNM_ASSERT_MSG_RETURN((arrayMode >= Gnm::kArrayModeLinearGeneral) && (arrayMode <= Gnm::kArrayMode3dTiledThickPrt), kStatusInvalidArgument, "invalid ArrayMode was passed.");
	SCE_GNM_ASSERT_MSG_RETURN(outTileMode != NULL, kStatusInvalidArgument, "outTileMode must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(numFragmentsPerPixel) && numFragmentsPerPixel <= 8, kStatusInvalidArgument, "numFragmentsPerPixel (%u) must be 1, 2, 4, or 8.", numFragmentsPerPixel);
	SCE_GNM_ASSERT_MSG_RETURN(numFragmentsPerPixel == 1 || isMacroTiled(arrayMode), kStatusInvalidArgument, "if numFragmentsPerPixel > 1, arrayMode (%d) must be a 2D/3D (macro-tiled) mode.", arrayMode);
	uint32_t bitsPerElement = surfaceFormat.getBitsPerElement();
	if (bitsPerElement == 96)
		bitsPerElement = 32; // 96bpp surfaces are tiled as if they're 32bpp with 3x the width
	uint32_t thickness = getMicroTileThickness(arrayMode);
	Gnm::TileMode tileMode = (Gnm::TileMode)-1; // invalid

	// Compressed formats can not use Linear modes, so fix that up front
	if (surfaceFormat.isBlockCompressedFormat() && isLinear(arrayMode))
	{
		arrayMode = (thickness>1) ? Gnm::kArrayMode1dTiledThick : Gnm::kArrayMode1dTiledThin;
	}
	// 1bpp formats must use kMicroTileModeDisplay
	if (surfaceFormat.getBitsPerElement() == 1)
	{
		*outTileMode = Gnm::kTileModeDisplay_LinearAligned;
		return kStatusSuccess;
	}

	arrayMode = GpuAddress::degradeLargeThickArrayMode(arrayMode, bitsPerElement);
	thickness = getMicroTileThickness(arrayMode); // recalculate thickness, in case arrayMode changed

	// Adjust microTileMode
	if (!isLinear(arrayMode))
	{
		if (thickness > 1)
		{
			microTileMode = Gnm::kMicroTileModeThick;
		}
		// 128 bpp
		// Fmask reuse color buffer's entry but bank-height field can be from another entry
		// To simplify the logic, fmask entry should be picked from thin / non-displayable ones
		if (bitsPerElement == 128 || flags.m_fmask)
		{
			microTileMode = Gnm::kMicroTileModeThin;
		}
		// 3D thin array modes must use thin micro-tile-mode
		if (arrayMode == Gnm::kArrayMode3dTiledThin || arrayMode == Gnm::kArrayMode3dTiledThinPrt)
		{
			microTileMode = Gnm::kMicroTileModeThin;
		}
		// Depth/stencil buffers must use depth micro-tile mode
		if (flags.m_depthTarget || flags.m_stencilTarget)
		{
			microTileMode = Gnm::kMicroTileModeDepth;
		}
	}

	if (microTileMode == Gnm::kMicroTileModeDepth)
	{
		uint32_t tileSize = thickness * bitsPerElement * numFragmentsPerPixel * kMicroTileWidth * kMicroTileHeight / 8;
		// Turn off tc compatible if row_size is smaller than tile size (tile split occurs).
		if (minGpuMode == Gnm::kGpuModeNeo && kDramRowSize < tileSize)
		{
			flags.m_texCompatible = 0;
		}
		if (flags.m_depthTarget && flags.m_texCompatible)
		{
			 // Texure readable depth surface should not be split
            switch (tileSize)
            {
                case 128:
                    tileMode = Gnm::kTileModeDepth_2dThin_128; // 1
                    break;
                case 256:
                    tileMode = Gnm::kTileModeDepth_2dThin_256; // 2
                    break;
                case 512:
                    tileMode = Gnm::kTileModeDepth_2dThin_512; // 3
                    break;
                default:
                    tileMode = Gnm::kTileModeDepth_2dThin_1K;  // 4
                    break;
            }
		}
		else
		{
			 // Depth and stencil need to use the same index, thus the pre-defined tile_split
            // can meet the requirement to choose the same macro mode index
            // uncompressed depth/stencil are not supported for now
            switch(numFragmentsPerPixel)
            {
                case 1:
                    tileMode = Gnm::kTileModeDepth_2dThin_64; // 0
                    break;
                case 2:
                case 4:
                    tileMode = Gnm::kTileModeDepth_2dThin_128; // 1
                    break;
                case 8:
                    tileMode = Gnm::kTileModeDepth_2dThin_256; // 2
                    break;
                default:
                    break;
            }
		}

		switch (arrayMode)
        {
		case Gnm::kArrayMode1dTiledThin:
                tileMode = Gnm::kTileModeDepth_1dThin; // 5
                break;
		case Gnm::kArrayModeTiledThinPrt:
                tileMode = Gnm::kTileModeDepth_2dThinPrt_256; // 6
                break;
        default:
                break; // already set above
        }

        if(flags.m_depthTarget && !flags.m_stencilTarget && Gnm::kGpuModeNeo == minGpuMode && tileMode < Gnm::kTileModeDepth_2dThin_256)
            tileMode = Gnm::kTileModeDepth_2dThin_256;
	}
	else if (microTileMode == Gnm::kMicroTileModeDisplay)
	{
		// TODO: only 2dThin is supported?
		if      (arrayMode == Gnm::kArrayMode1dTiledThin)
			tileMode = Gnm::kTileModeDisplay_1dThin;
		else if (arrayMode == Gnm::kArrayMode2dTiledThin)
			tileMode = Gnm::kTileModeDisplay_2dThin;
		else if (arrayMode == Gnm::kArrayModeTiledThinPrt)
			tileMode = Gnm::kTileModeDisplay_ThinPrt;
		else if (arrayMode == Gnm::kArrayMode2dTiledThinPrt)
			tileMode = Gnm::kTileModeDisplay_2dThinPrt;
		else if (surfaceFormat.getBitsPerElement() == 1)
			tileMode = Gnm::kTileModeDisplay_1dThin;
	}
	else if (microTileMode == Gnm::kMicroTileModeThin)
	{
		if      (arrayMode == Gnm::kArrayMode1dTiledThin)
			tileMode = Gnm::kTileModeThin_1dThin;
		else if (arrayMode == Gnm::kArrayMode2dTiledThin)
			tileMode = Gnm::kTileModeThin_2dThin;
		else if (arrayMode == Gnm::kArrayMode3dTiledThin)
			tileMode = Gnm::kTileModeThin_3dThin;
		else if (arrayMode == Gnm::kArrayModeTiledThinPrt)
			tileMode = Gnm::kTileModeThin_ThinPrt;
		else if (arrayMode == Gnm::kArrayMode2dTiledThinPrt)
			tileMode = Gnm::kTileModeThin_2dThinPrt;
		else if (arrayMode == Gnm::kArrayMode3dTiledThinPrt)
			tileMode = Gnm::kTileModeThin_3dThinPrt;
		else
			tileMode = Gnm::kTileModeThin_1dThin; // catch-all
	}
	else if (microTileMode == Gnm::kMicroTileModeThick)
	{
		if      (arrayMode == Gnm::kArrayMode1dTiledThick)
			tileMode = Gnm::kTileModeThick_1dThick;
		else if (arrayMode == Gnm::kArrayMode2dTiledThick)
			tileMode = Gnm::kTileModeThick_2dThick;
		else if (arrayMode == Gnm::kArrayMode3dTiledThick)
			tileMode = Gnm::kTileModeThick_3dThick;
		else if (arrayMode == Gnm::kArrayModeTiledThickPrt)
			tileMode = Gnm::kTileModeThick_ThickPrt;
		else if (arrayMode == Gnm::kArrayMode2dTiledThickPrt)
			tileMode = Gnm::kTileModeThick_2dThickPrt;
		else if (arrayMode == Gnm::kArrayMode3dTiledThickPrt)
			tileMode = Gnm::kTileModeThick_3dThickPrt;
		else if (arrayMode == Gnm::kArrayMode2dTiledXThick)
			tileMode = Gnm::kTileModeThick_2dXThick;
		else if (arrayMode == Gnm::kArrayMode3dTiledXThick)
			tileMode = Gnm::kTileModeThick_3dXThick;
		else
			tileMode = Gnm::kTileModeThick_1dThick; // catch-all
	}
	else if (microTileMode == Gnm::kMicroTileModeRotated)
	{
		SCE_GNM_ERROR("microTileMode=kMicroTileModeRotated is not currently supported.");
		return kStatusInvalidArgument;
		/*
		if      (arrayMode == Gnm::kArrayMode1dTiledThin)
			tileMode = Gnm::kTileModeRotated_1dThin;
		else if (arrayMode == Gnm::kArrayMode2dTiledThin)
			tileMode = Gnm::kTileModeRotated_2dThin;
		else if (arrayMode == Gnm::kArrayModeTiledThinPrt)
			tileMode = Gnm::kTileModeRotated_ThinPrt;
		else if (arrayMode == Gnm::kArrayMode2dTiledThinPrt)
			tileMode = Gnm::kTileModeRotated_2dThinPrt;
		*/
	}

	// Override above table if a linear mode was requested
	if (arrayMode == Gnm::kArrayModeLinearGeneral)
	{
		SCE_GNM_ERROR("arrayMode=kArrayModeLinearGeneral is not currently supported.");
		return kStatusInvalidArgument;
		/*
		tileMode = Gnm::kTileModeDisplay_LinearGeneral;
		*/
	}
	else if (arrayMode == Gnm::kArrayModeLinearAligned)
	{
		tileMode = Gnm::kTileModeDisplay_LinearAligned;
	}

	if (tileMode == (Gnm::TileMode)-1)
	{
		SCE_GNM_ERROR("Could not determine a valid tile mode for provided inputs.");
		return kStatusInvalidArgument;
	}
	*outTileMode = tileMode;
	return kStatusSuccess;
}
int32_t sce::GpuAddress::computeSurfaceTileMode(Gnm::TileMode *outTileMode, Gnm::ArrayMode arrayMode, SurfaceFlags flags, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel, Gnm::MicroTileMode microTileMode) // DEPRECATED
{
	return computeSurfaceTileMode(Gnm::kGpuModeBase, outTileMode, arrayMode, flags, surfaceFormat, numFragmentsPerPixel, microTileMode);
}

int32_t sce::GpuAddress::computeSurfaceTileMode(Gnm::GpuMode minGpuMode, Gnm::TileMode *outTileMode, SurfaceType surfaceType, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel)
{
	Gnm::ArrayMode arrayMode;
	int32_t status = getArrayModeForSurfaceType(minGpuMode, &arrayMode, surfaceType, numFragmentsPerPixel);
	if (status != kStatusSuccess)
		return status;
	SurfaceFlags flags;
	status = getFlagsForSurfaceType(minGpuMode, &flags, surfaceType, kSurfaceMipmapDisable); //		computeSurfaceTileMode() doesn't use m_pow2pad, so it's safe to pass kSurfaceMipmapDisable.
	if (status != kStatusSuccess)
		return status;
	Gnm::MicroTileMode microTileMode;
	status = getMicroTileModeForSurfaceType(minGpuMode, &microTileMode, surfaceType);
	if (status != kStatusSuccess)
		return status;
	return computeSurfaceTileMode(minGpuMode, outTileMode, arrayMode, flags, surfaceFormat, numFragmentsPerPixel, microTileMode);
}
int32_t sce::GpuAddress::computeSurfaceTileMode(Gnm::TileMode *outTileMode, SurfaceType surfaceType, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel) // DEPRECATED
{
	return computeSurfaceTileMode(Gnm::kGpuModeBase, outTileMode, surfaceType, surfaceFormat, numFragmentsPerPixel);
}

int32_t sce::GpuAddress::adjustTileMode(Gnm::GpuMode minGpuMode, Gnm::TileMode *outTileMode, Gnm::TileMode oldTileMode, Gnm::ArrayMode newArrayMode)
{
	SCE_GNM_UNUSED(minGpuMode);
	SCE_GNM_ASSERT_MSG_RETURN(outTileMode, kStatusInvalidArgument, "outTileMode must not be NULL.");
	// If the array mode didn't change, neither should the tile mode.
	Gnm::ArrayMode oldArrayMode;
	int32_t status = GpuAddress::getArrayMode(&oldArrayMode, oldTileMode);
	if (status != kStatusSuccess)
		return status;
	if (newArrayMode == oldArrayMode)
	{
		*outTileMode = oldTileMode;
		return kStatusSuccess;
	}

	Gnm::MicroTileMode microTileMode;
	status = GpuAddress::getMicroTileMode(&microTileMode, oldTileMode);
	if (status != kStatusSuccess)
		return status;
	switch(microTileMode)
	{
	case Gnm::kMicroTileModeDepth:
		SCE_GNM_ASSERT_MSG(newArrayMode == Gnm::kArrayMode1dTiledThin, "Expected degraded tile mode (%d) to be 1dThin.", newArrayMode);
		*outTileMode = Gnm::kTileModeDepth_1dThin;
		return kStatusSuccess;
	case Gnm::kMicroTileModeDisplay:
		if      (newArrayMode == Gnm::kArrayMode1dTiledThin)
			*outTileMode = Gnm::kTileModeDisplay_1dThin;
		else
			break;
		return kStatusSuccess;
	case Gnm::kMicroTileModeThick:
		if      (newArrayMode == Gnm::kArrayMode3dTiledThick)
			*outTileMode = Gnm::kTileModeThick_3dThick;
		else if      (newArrayMode == Gnm::kArrayMode2dTiledThick)
			*outTileMode = Gnm::kTileModeThick_2dThick;
		else if (newArrayMode == Gnm::kArrayMode1dTiledThick)
			*outTileMode = Gnm::kTileModeThick_1dThick;
		else if (newArrayMode == Gnm::kArrayMode3dTiledThin)
			*outTileMode = Gnm::kTileModeThin_3dThin; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode3dTiledThinPrt)
			*outTileMode = Gnm::kTileModeThin_3dThinPrt; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode2dTiledThin)
			*outTileMode = Gnm::kTileModeThin_2dThin; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode2dTiledThinPrt)
			*outTileMode = Gnm::kTileModeThin_2dThinPrt; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayModeTiledThinPrt)
			*outTileMode = Gnm::kTileModeThin_ThinPrt; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode1dTiledThin)
			*outTileMode = Gnm::kTileModeThin_1dThin; // thick modes can degrade to thin
		else
			break;
		return kStatusSuccess;
	case Gnm::kMicroTileModeThin:
		if      (newArrayMode == Gnm::kArrayMode3dTiledThick)
			*outTileMode = Gnm::kTileModeThick_3dThick;
		else if      (newArrayMode == Gnm::kArrayMode2dTiledThick)
			*outTileMode = Gnm::kTileModeThick_2dThick;
		else if (newArrayMode == Gnm::kArrayMode1dTiledThick)
			*outTileMode = Gnm::kTileModeThick_1dThick;
		else if (newArrayMode == Gnm::kArrayMode3dTiledThin)
			*outTileMode = Gnm::kTileModeThin_3dThin; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode3dTiledThinPrt)
			*outTileMode = Gnm::kTileModeThin_3dThinPrt; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode2dTiledThin)
			*outTileMode = Gnm::kTileModeThin_2dThin; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode2dTiledThinPrt)
			*outTileMode = Gnm::kTileModeThin_2dThinPrt; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayModeTiledThinPrt)
			*outTileMode = Gnm::kTileModeThin_ThinPrt; // thick modes can degrade to thin
		else if (newArrayMode == Gnm::kArrayMode2dTiledThin)
			*outTileMode = Gnm::kTileModeThin_2dThin;
		else if (newArrayMode == Gnm::kArrayMode1dTiledThin)
			*outTileMode = Gnm::kTileModeThin_1dThin;
		else
			break;
		return kStatusSuccess;
	case Gnm::kMicroTileModeRotated:
	default:
		SCE_GNM_ERROR("Rotated tile modes should not be used.");
		//if      (newArrayMode == Gnm::kArrayMode1dTiledThin)
		//	return Gnm::kTileModeRotated_1dThin;
		return kStatusInvalidArgument;
	}
	SCE_GNM_ERROR("Unhandled case: oldTileMode=%d, newArrayMode=%d\n", oldTileMode, newArrayMode);
	return kStatusInvalidArgument;
}
int32_t sce::GpuAddress::adjustTileMode(Gnm::TileMode *outTileMode, Gnm::TileMode oldTileMode, Gnm::ArrayMode newArrayMode) // DEPRECATED
{
	return adjustTileMode(Gnm::kGpuModeBase, outTileMode, oldTileMode, newArrayMode);
}

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4996) // deprecated function use
#else
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
bool sce::GpuAddress::isMacroTiled(Gnm::TileMode tileMode)
{
	const uint64_t macroTiledMask = 0
		| (1 << Gnm::kTileModeDepth_2dThin_64)       // 0x00000000
		| (1 << Gnm::kTileModeDepth_2dThin_128)      // 0x00000001
		| (1 << Gnm::kTileModeDepth_2dThin_256)      // 0x00000002
		| (1 << Gnm::kTileModeDepth_2dThin_512)      // 0x00000003
		| (1 << Gnm::kTileModeDepth_2dThin_1K)       // 0x00000004
		| (0 << Gnm::kTileModeDepth_1dThin)          // 0x00000005
		| (1 << Gnm::kTileModeDepth_2dThinPrt_256)   // 0x00000006
		| (1 << Gnm::kTileModeDepth_2dThinPrt_1K)    // 0x00000007
		| (0 << Gnm::kTileModeDisplay_LinearAligned) // 0x00000008
		| (0 << Gnm::kTileModeDisplay_1dThin)        // 0x00000009
		| (1 << Gnm::kTileModeDisplay_2dThin)        // 0x0000000A
		| (1 << Gnm::kTileModeDisplay_ThinPrt)       // 0x0000000B
		| (1 << Gnm::kTileModeDisplay_2dThinPrt)     // 0x0000000C
		| (0 << Gnm::kTileModeThin_1dThin)           // 0x0000000D
		| (1 << Gnm::kTileModeThin_2dThin)           // 0x0000000E
		| (1 << Gnm::kTileModeThin_3dThin)           // 0x0000000F
		| (1 << Gnm::kTileModeThin_ThinPrt)          // 0x00000010
		| (1 << Gnm::kTileModeThin_2dThinPrt)        // 0x00000011
		| (1 << Gnm::kTileModeThin_3dThinPrt)        // 0x00000012
		| (0 << Gnm::kTileModeThick_1dThick)         // 0x00000013
		| (1 << Gnm::kTileModeThick_2dThick)         // 0x00000014
		| (1 << Gnm::kTileModeThick_3dThick)         // 0x00000015
		| (1 << Gnm::kTileModeThick_ThickPrt)        // 0x00000016
		| (1 << Gnm::kTileModeThick_2dThickPrt)      // 0x00000017
		| (1 << Gnm::kTileModeThick_3dThickPrt)      // 0x00000018
		| (1 << Gnm::kTileModeThick_2dXThick)        // 0x00000019
		| (1 << Gnm::kTileModeThick_3dXThick)        // 0x0000001A
		| (0 << Gnm::kTileModeDisplay_LinearGeneral) // 0x0000001F
		;
	return (macroTiledMask & (1ULL<<tileMode)) != 0;
}

bool sce::GpuAddress::isMicroTiled(Gnm::TileMode tileMode)
{
	const uint64_t microTiledMask = 0
		| (0 << Gnm::kTileModeDepth_2dThin_64)       // 0x00000000
		| (0 << Gnm::kTileModeDepth_2dThin_128)      // 0x00000001
		| (0 << Gnm::kTileModeDepth_2dThin_256)      // 0x00000002
		| (0 << Gnm::kTileModeDepth_2dThin_512)      // 0x00000003
		| (0 << Gnm::kTileModeDepth_2dThin_1K)       // 0x00000004
		| (1 << Gnm::kTileModeDepth_1dThin)          // 0x00000005
		| (0 << Gnm::kTileModeDepth_2dThinPrt_256)   // 0x00000006
		| (0 << Gnm::kTileModeDepth_2dThinPrt_1K)    // 0x00000007
		| (0 << Gnm::kTileModeDisplay_LinearAligned) // 0x00000008
		| (1 << Gnm::kTileModeDisplay_1dThin)        // 0x00000009
		| (0 << Gnm::kTileModeDisplay_2dThin)        // 0x0000000A
		| (0 << Gnm::kTileModeDisplay_ThinPrt)       // 0x0000000B
		| (0 << Gnm::kTileModeDisplay_2dThinPrt)     // 0x0000000C
		| (1 << Gnm::kTileModeThin_1dThin)           // 0x0000000D
		| (0 << Gnm::kTileModeThin_2dThin)           // 0x0000000E
		| (0 << Gnm::kTileModeThin_3dThin)           // 0x0000000F
		| (0 << Gnm::kTileModeThin_ThinPrt)          // 0x00000010
		| (0 << Gnm::kTileModeThin_2dThinPrt)        // 0x00000011
		| (0 << Gnm::kTileModeThin_3dThinPrt)        // 0x00000012
		| (1 << Gnm::kTileModeThick_1dThick)         // 0x00000013
		| (0 << Gnm::kTileModeThick_2dThick)         // 0x00000014
		| (0 << Gnm::kTileModeThick_3dThick)         // 0x00000015
		| (0 << Gnm::kTileModeThick_ThickPrt)        // 0x00000016
		| (0 << Gnm::kTileModeThick_2dThickPrt)      // 0x00000017
		| (0 << Gnm::kTileModeThick_3dThickPrt)      // 0x00000018
		| (0 << Gnm::kTileModeThick_2dXThick)        // 0x00000019
		| (0 << Gnm::kTileModeThick_3dXThick)        // 0x0000001A
		| (0 << Gnm::kTileModeDisplay_LinearGeneral) // 0x0000001F
		;
	return (microTiledMask & (1ULL<<tileMode)) != 0;
}
#ifdef _MSC_VER
#	pragma warning(pop)
#else
#	pragma clang diagnostic pop
#endif

int32_t sce::GpuAddress::computeSurfaceInfo(SurfaceInfo *infoOut, const TilingParameters *tp)
{
	SCE_GNM_ASSERT_MSG_RETURN(infoOut != NULL, kStatusInvalidArgument, "infoOut must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp != NULL, kStatusInvalidArgument, "tp must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(tp->m_surfaceFlags.m_volume == 1 || tp->m_linearDepth == 1, kStatusInvalidArgument, "If tp->m_surfaceFlags.m_volume is not set, tp->m_linearDepth must be 1.");
	if (tp->m_mipLevel == 0)
	{
		SCE_GNM_ASSERT_MSG_RETURN(tp->m_baseTiledPitch == 0 || tp->m_baseTiledPitch >= tp->m_linearWidth, kStatusInvalidArgument,
			"if infoIn->m_mipLevel is 0, tp->m_baseTiledPitch must be 0 or >= tp->m_linearWidth.");
	}
	else
	{
		SCE_GNM_ASSERT_MSG_RETURN(tp->m_baseTiledPitch >= tp->m_linearWidth, kStatusInvalidArgument, "tp->m_baseTiledPitch (%u) must be >= tp->m_linearWidth (%u).", tp->m_baseTiledPitch, tp->m_linearWidth);
	}

	uint32_t bitsPerElement      = tp->m_bitsPerFragment;
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(bitsPerElement) && bitsPerElement > 0 && bitsPerElement <= 128, kStatusInvalidArgument, "tp->m_bitsPerFragment (%u) must be a power of 2 in the range [1..128].", bitsPerElement);
	TilingParameters tpTemp = {}; // Starts as a copy of surfInfoIn, but may change along the way
	memcpy(&tpTemp, tp, sizeof(tpTemp));
	Gnm::MicroTileMode microTileMode;
	int32_t status = getMicroTileMode(&microTileMode, tpTemp.m_tileMode);
	if (status != kStatusSuccess)
		return status;
	SCE_GNM_ASSERT_MSG_RETURN( !(tpTemp.m_numFragmentsPerPixel > 1 && tpTemp.m_mipLevel > 0), kStatusInvalidArgument, "multi-sampled surfaces cannot have mipmaps.");
	SCE_GNM_ASSERT_MSG_RETURN(tpTemp.m_surfaceFlags.m_depthTarget == 0 || microTileMode == Gnm::kMicroTileModeDepth,
		kStatusInvalidArgument, "if tp->m_surfaceFlags.m_depthTarget is 1, tp->m_tileMode must be kTileModeDepth_*dThin.");

	Gnm::PipeConfig pipeConfig;
	if (tpTemp.m_minGpuMode == Gnm::kGpuModeNeo)
	{
		status = getAltPipeConfig(&pipeConfig, tpTemp.m_tileMode);
	}
	else
	{
		status = getPipeConfig(&pipeConfig, tpTemp.m_tileMode);
	}
	if (status != kStatusSuccess)
		return status;
	uint32_t numPipes = getPipeCount(pipeConfig);
	
	uint32_t requestedPitch = tpTemp.m_baseTiledPitch;

	// Preserve some output values before we change them
	infoOut->m_bitsPerElement = static_cast<uint16_t>(tp->m_bitsPerFragment);

	// Mipmap check.
	uint32_t numTexelsPerElement = 1;
	uint32_t expandX = 1;
	uint32_t expandY = 1;
	if (tp->m_isBlockCompressed)
	{
		switch(bitsPerElement)
		{
		case 1: // 1bpp
			expandX = 8;
			bitsPerElement = 8;
			numTexelsPerElement = 8;
			if (tpTemp.m_mipLevel == 0)
			{
				// 1-bit mip level 0 must have width divisible by 8.
				tpTemp.m_linearWidth  = (tpTemp.m_linearWidth +7) & ~7;
				requestedPitch        =        (requestedPitch+7) & ~7;
			}
			break;
		case 4: // BC1, BC4
		case 8: // other BCn formats
			expandX = 4;
			expandY = 4;
			bitsPerElement *= 16;
			numTexelsPerElement = 16;
			if (tpTemp.m_mipLevel == 0)
			{
				// BCn mip level 0 must have width/height divisible by 4.
				tpTemp.m_linearWidth  = (tpTemp.m_linearWidth +3) & ~3;
				tpTemp.m_linearHeight = (tpTemp.m_linearHeight+3) & ~3;
				requestedPitch        =        (requestedPitch+3) & ~3;
			}
			break;
		case 16: // GB_GR, BG_RG
			// TODO
			break;
		default:
			SCE_GNM_ASSERT_MSG_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", bitsPerElement);
			break;
		}
	}
	if (tpTemp.m_mipLevel > 0 )
	{
		// mip level pitches should be generated from base level's pitch instead of its width.
		tpTemp.m_linearWidth = std::max(tpTemp.m_baseTiledPitch >> tpTemp.m_mipLevel, 1U);
	}
	tpTemp.m_linearWidth  = std::max( (tpTemp.m_linearWidth  + expandX - 1U) / expandX, 1U);
	tpTemp.m_linearHeight = std::max( (tpTemp.m_linearHeight + expandY - 1U) / expandY, 1U);
	requestedPitch        = std::max( (       requestedPitch + expandX - 1U) / expandX, 1U);
	if (tp->m_surfaceFlags.m_pow2Pad)
	{
		tpTemp.m_linearWidth  = nextPowerOfTwo(tpTemp.m_linearWidth);
		tpTemp.m_linearHeight = nextPowerOfTwo(tpTemp.m_linearHeight);
		tpTemp.m_linearDepth  = nextPowerOfTwo(tpTemp.m_linearDepth);
		requestedPitch = nextPowerOfTwo(requestedPitch);
	}

	// R800AddrLib::HwlComputeSurfaceInfo()
	// Compute surface thickness
	Gnm::ArrayMode arrayMode;
	status = getArrayMode(&arrayMode, tpTemp.m_tileMode);
	if (status != kStatusSuccess)
		return status;
	uint32_t thickness = getMicroTileThickness(arrayMode);

	SCE_GNM_ASSERT_MSG_RETURN(numTexelsPerElement != 16 || !isLinear(arrayMode), kStatusInvalidArgument,
		"if numTexelsPerElement is 16, tp->m_tileMode must not be LinearAligned or LinearGeneral!\n");

	// Degrade array mode from thick to thin for compressed/packed surfaces
	if (thickness > 1)
	{
		Gnm::ArrayMode oldArrayMode = arrayMode;
		if (numTexelsPerElement > 1) // BCn or 1bpp
		{
			switch(arrayMode)
			{
			case Gnm::kArrayMode1dTiledThick:
				arrayMode = Gnm::kArrayMode1dTiledThin;
				break;
			case Gnm::kArrayMode2dTiledXThick:
			case Gnm::kArrayMode2dTiledThick:
				arrayMode = Gnm::kArrayMode2dTiledThin;
				break;
			case Gnm::kArrayMode3dTiledXThick:
			case Gnm::kArrayMode3dTiledThick:
				arrayMode = Gnm::kArrayMode3dTiledThin;
				break;
			case Gnm::kArrayModeTiledThickPrt:
				arrayMode = Gnm::kArrayModeTiledThinPrt;
				break;
			case Gnm::kArrayMode2dTiledThickPrt:
				arrayMode = Gnm::kArrayMode2dTiledThinPrt;
				break;
			case Gnm::kArrayMode3dTiledThickPrt:
				arrayMode = Gnm::kArrayMode3dTiledThinPrt;
				break;
			default:
				break;
			}
			if (oldArrayMode != arrayMode)
			{
				thickness = 1;
			}
		}
	}

	// More sanity-checking)
	SCE_GNM_ASSERT_MSG_RETURN( !(bitsPerElement > 64 && thickness == 8), kStatusInvalidArgument, ">64bpp surfaces can not use XThick tile modes.");
	SCE_GNM_ASSERT_MSG_RETURN( !(tpTemp.m_numFragmentsPerPixel < 1 || tpTemp.m_numFragmentsPerPixel > 16), kStatusInvalidArgument, "tp->m_numFragmentsPerPixel must be in the range [1..16].");
	//if (surfInfoTemp.numSamples < surfInfoTemp.numFragments)
	//	return; // EQAA means numSamples > numFragments

	// R800AddrLib::DispatchComputeSurfaceInfo()
	arrayMode = degradeLargeThickArrayMode(arrayMode, bitsPerElement);
	status = adjustTileMode(tpTemp.m_minGpuMode, &tpTemp.m_tileMode, tpTemp.m_tileMode, arrayMode);
	thickness = getMicroTileThickness(arrayMode); // recalculate thickness, in case arrayMode changed

	// Disable texCompatible flag under certain conditions
	if (tpTemp.m_minGpuMode == Gnm::kGpuModeNeo)
	{
		Gnm::MicroTileMode microTileMode;
		getMicroTileMode(&microTileMode, tpTemp.m_tileMode);
		infoOut->m_isTexCompatible = tpTemp.m_surfaceFlags.m_texCompatible;
		if (microTileMode == Gnm::kMicroTileModeDepth)
		{
			uint32_t tileSizeBytes = thickness * bitsPerElement * tpTemp.m_numFragmentsPerPixel * kMicroTileWidth * kMicroTileHeight / 8;
			if (kDramRowSize < tileSizeBytes)
			{
				infoOut->m_isTexCompatible = false;
			}
		}
		else
		{
			if (isMacroTiled(arrayMode))
			{
				Gnm::SampleSplit sampleSplitHw = Gnm::kSampleSplit1;
				getSampleSplit(&sampleSplitHw, tpTemp.m_tileMode);
				uint32_t sampleSplit = 1<<sampleSplitHw;
				uint32_t tileBytes1x = bitsPerElement * kMicroTileWidth * kMicroTileHeight * thickness / 8;
				uint32_t colorTileSplit = std::max(256U, sampleSplit*tileBytes1x);
				if (kDramRowSize < colorTileSplit)
				{
					infoOut->m_isTexCompatible = false;
				}
			}
			else
			{
				infoOut->m_isTexCompatible = false;
			}
		}
	}

	//if (surfInfoTemp.numFragments != surfInfoTemp.numSamples) // EQAA
	//{
	//	surfInfoTemp.numSamples = (NumSamples)surfInfoTemp.numFragments;
	//}
	uint32_t numDimensionsToPad = 0; // ???
	if (tpTemp.m_surfaceFlags.m_cube)
	{
		if (tpTemp.m_mipLevel == 0)
			numDimensionsToPad = 2;
	}

	// And now we branch out into ComputeSurfaceInfoLinear(), ComputeSurfaceInfoMicroTiled() or
	// ComputeSurfaceInfoMacroTiled()
	switch(arrayMode)
	{
	case Gnm::kArrayModeLinearGeneral:
	case Gnm::kArrayModeLinearAligned:
		{
			// Compute alignment
			if (arrayMode == Gnm::kArrayModeLinearGeneral)
			{
				infoOut->m_baseAlign = (bitsPerElement>8) ? (bitsPerElement+7)/8 : 1;
				infoOut->m_pitchAlign = 1;
				infoOut->m_heightAlign = 1;
			}
			else if (arrayMode == Gnm::kArrayModeLinearAligned)
			{
				infoOut->m_baseAlign = kPipeInterleaveBytes;
				infoOut->m_pitchAlign = std::max(8UL, 64UL/((bitsPerElement+7)/8UL));
				infoOut->m_heightAlign = 1;
			}
			
			// Display engine hardwires lower 5 bit of GRPH_PITCH to ZERO which means 32 pixel alignment
			// Maybe it will be fixed in future but let's make it general for now.
			//if (tpTemp.m_surfaceFlags.m_display && tpTemp.m_mipLevel == 0 /* || infoTemp.m_flags.m_overlay */)
			//{
			//	infoOut->m_pitchAlign = (infoOut->m_pitchAlign + 31) & ~31;
			//}
			// When linear_general surface is accessed in multiple lines, it requires 8 pixels in pitch
			// alignment since PITCH_TILE_MAX is in unit of 8 pixels.
			// It is OK if it is accessed per line.
			//if ((arrayMode == Gnm::kArrayModeLinearGeneral) && tpTemp.m_surfaceFlags.m_display && (tpTemp.m_linearHeight > 1))
			//{
			//	SCE_GNM_ASSERT_MSG((tpTemp.m_linearWidth % 8) == 0, "If arrayMode is kArrayModeLinearGeneral and displayable, surface width (%d) must be a multiple of 8 pixels.", tpTemp.m_linearWidth);
			//}
			infoOut->m_depthAlign = thickness;

			uint32_t outPitch  = (tpTemp.m_mipLevel == 0 && tpTemp.m_baseTiledPitch > 0) ? requestedPitch : tpTemp.m_linearWidth;
			uint32_t outHeight = tpTemp.m_linearHeight;
			uint32_t outDepth  = tpTemp.m_linearDepth;
			// Pad dimensions -- AddrLib::PadDimensions()
			// TODO: re-expand X and Y before pitch/height alignment? Otherwise, these can get pretty huge.
			if (tpTemp.m_mipLevel > 0 && tpTemp.m_surfaceFlags.m_cube)
			{
				numDimensionsToPad = (tpTemp.m_linearDepth > 1) ? 3 : 2; // pad cubemap sub-levels when we treat it as a 3d texture
			}
			if (numDimensionsToPad == 0)
				numDimensionsToPad = 3;
			SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_pitchAlign), kStatusInternalTilingError, "pitchAlign (%u) must be a power of two.", infoOut->m_pitchAlign);
			outPitch = (outPitch + infoOut->m_pitchAlign-1) & ~(infoOut->m_pitchAlign-1);
			if (numDimensionsToPad > 1)
			{
				SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_heightAlign), kStatusInternalTilingError, "heightAlign (%u) must be a power of two.", infoOut->m_heightAlign);
				outHeight = (outHeight + infoOut->m_heightAlign-1) & ~(infoOut->m_heightAlign-1);
			}
			if (numDimensionsToPad > 2 || thickness > 1)
			{
				// For cubemap single-face, we do not pad slices.
				// If we pad it, the slice number should be set to 6 and current mip level > 1
				if (tpTemp.m_surfaceFlags.m_cube)// && (1 || surfInfoTemp.flags.cubeAsArray))
					outDepth = nextPowerOfTwo(outDepth);
				// Normal 3D textures or 2D arrays or cubemaps have a thick mode?
				if (thickness > 1)
				{
					SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_depthAlign), kStatusInternalTilingError, "depthAlign (%u) must be a power of two.", infoOut->m_depthAlign);
					outDepth = (outDepth + infoOut->m_depthAlign-1) & ~(infoOut->m_depthAlign-1);
				}
			}

			// Hardware-specific size adjustment
			uint64_t sliceSizeBytes = 0;
			if (arrayMode == Gnm::kArrayModeLinearGeneral)
			{
				sliceSizeBytes = (static_cast<uint64_t>(outPitch) * (outHeight) * bitsPerElement * tpTemp.m_numFragmentsPerPixel + 7) / 8;
			}
			else if (arrayMode == Gnm::kArrayModeLinearAligned)
			{
				uint32_t pitch = outPitch;
				uint32_t height = outHeight;
				uint32_t pixelsPerPipeInterleave = kPipeInterleaveBytes / ((bitsPerElement+7)/8);
				uint32_t sliceAlignInPixel = pixelsPerPipeInterleave < 64 ? 64 : pixelsPerPipeInterleave;
				SCE_GNM_ASSERT_MSG_RETURN(tpTemp.m_numFragmentsPerPixel==1, kStatusInvalidArgument, "numFragmentsPerPixel (%u) must be 1 for linear surfaces.", tpTemp.m_numFragmentsPerPixel);
				uint64_t pixelsPerSlice = static_cast<uint64_t>(pitch) * height * tpTemp.m_numFragmentsPerPixel;
				while(pixelsPerSlice % sliceAlignInPixel)
				{
					pitch += infoOut->m_pitchAlign;
					pixelsPerSlice = static_cast<uint64_t>(pitch) * height * tpTemp.m_numFragmentsPerPixel;
				}
				outPitch = pitch;
				uint32_t heightAlign = 1;
				while ((pitch * heightAlign) % sliceAlignInPixel)
				{
					heightAlign++;
				}
				infoOut->m_heightAlign = heightAlign;
				sliceSizeBytes = (pixelsPerSlice*bitsPerElement+7)/8;
			}
			infoOut->m_pitch  = outPitch;
			infoOut->m_height = outHeight;
			infoOut->m_depth  = outDepth;
			infoOut->m_surfaceSize = sliceSizeBytes * outDepth;
			infoOut->m_arrayMode = arrayMode;
		}
		break;
	case Gnm::kArrayMode1dTiledThin:
	case Gnm::kArrayMode1dTiledThick:
		{
			// skip this check when jumping here from other code
			SCE_GNM_ASSERT_MSG_RETURN(tpTemp.m_numFragmentsPerPixel==1, kStatusInvalidArgument, "numFragmentsPerPixel (%u) must be 1 for 1D-tiled surfaces surfaces.", tpTemp.m_numFragmentsPerPixel);
MICRO_TILE:
			thickness = getMicroTileThickness(arrayMode);
			// MSAA surfaces must be 2D tiled

			// Compute alignments
			infoOut->m_baseAlign = kPipeInterleaveBytes;
			infoOut->m_pitchAlign = 8;
			//if (tpTemp.m_surfaceFlags.m_display && tpTemp.m_mipLevel == 0 /*|| infoTemp.m_flags.m_overlay*/)
			//{
			//	// Display engine hardwires low 5 bits of GRPH_PITCH to zero, which forces
			//	// 32-pixel alignment for displayable surfaces.
			//	infoOut->m_pitchAlign = (infoOut->m_pitchAlign + 31) & ~31;
			//}
			infoOut->m_heightAlign = 8; // MicroTileHeight
			infoOut->m_depthAlign = thickness;

			uint32_t outPitch  = (tpTemp.m_mipLevel == 0 && tpTemp.m_baseTiledPitch > 0) ? requestedPitch : tpTemp.m_linearWidth;
			uint32_t outHeight = tpTemp.m_linearHeight;
			uint32_t outDepth  = tpTemp.m_linearDepth;
			// Pad dimensions -- AddrLib::PadDimensions()
			if (tpTemp.m_mipLevel > 0 && tpTemp.m_surfaceFlags.m_cube)
			{
				numDimensionsToPad = (tpTemp.m_linearDepth > 1) ? 3 : 2; // pad cubemap sub-levels when we treat it as a 3d texture
			}
			if (numDimensionsToPad == 0)
				numDimensionsToPad = 3;
			SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_pitchAlign), kStatusInternalTilingError, "pitchAlign (%u) must be a power of two.", infoOut->m_pitchAlign);
			outPitch = (outPitch + infoOut->m_pitchAlign-1) & ~(infoOut->m_pitchAlign-1);
			if (numDimensionsToPad > 1)
			{
				SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_heightAlign), kStatusInternalTilingError, "heightAlign (%u) must be a power of two.", infoOut->m_heightAlign);
				outHeight = (outHeight + infoOut->m_heightAlign-1) & ~(infoOut->m_heightAlign-1);
			}
			if (numDimensionsToPad > 2 || thickness > 1)
			{
				// For cubemap single-face, we do not pad slices.
				// If we pad it, the slice number should be set to 6 and current mip level > 1
				if (tpTemp.m_surfaceFlags.m_cube)// && (1 || surfInfoTemp.flags.cubeAsArray))
					outDepth = nextPowerOfTwo(outDepth);
				// Normal 3D textures or 2D arrays or cubemaps have a thick mode?
				if (thickness > 1)
				{
					SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_depthAlign), kStatusInternalTilingError, "depthAlign (%u) must be a power of two.", infoOut->m_depthAlign);
					outDepth = (outDepth + infoOut->m_depthAlign-1) & ~(infoOut->m_depthAlign-1);
				}
			}
			// DCC-compatible MSAA surfaces require additional pitch padding.
			const bool dccCompatible = (microTileMode != Gnm::kMicroTileModeDepth) && (tpTemp.m_minGpuMode >= Gnm::kGpuModeNeo);
			if (dccCompatible && tpTemp.m_numFragmentsPerPixel > 1 && tpTemp.m_mipLevel == 0 && isMacroTiled(arrayMode))
			{
				Gnm::SampleSplit sampleSplitHW;
				Gnm::TileSplit tileSplitHW;
				GpuAddress::getSampleSplit(&sampleSplitHW, tpTemp.m_tileMode);
				GpuAddress::getTileSplit(&tileSplitHW, tpTemp.m_tileMode);
				uint32_t tileBytes1x = (thickness*bitsPerElement*kMicroTileWidth*kMicroTileHeight + 7)/8;
				uint32_t sampleSplit = 1 << sampleSplitHW;
				uint32_t tileSplit = (microTileMode == Gnm::kMicroTileModeDepth)
					? (64<<tileSplitHW) // depth modes store the tile split bytes directly
					: std::max(256U, sampleSplit*tileBytes1x); // other modes store a sample split multiplier
				uint32_t tileSplitBytes = std::min(kDramRowSize, tileSplit);

				uint32_t tileSizePerFragment = (bitsPerElement * kMicroTileWidth * kMicroTileHeight + 7) / 8;
				uint32_t fragmentsPerSplit = tileSplitBytes / tileSizePerFragment;
				if (fragmentsPerSplit < tpTemp.m_numFragmentsPerPixel)
				{
					uint32_t dccFastClearByteAlign = numPipes * kPipeInterleaveBytes * 256;
					uint32_t bytesPerSplit = outPitch * outHeight * bitsPerElement * fragmentsPerSplit;
					SCE_GNM_ASSERT_MSG(isPowerOfTwo(dccFastClearByteAlign), "dccFastClearByteAlign is supposed to be a power of two...");
					if (0 != (bytesPerSplit & (dccFastClearByteAlign-1)))
					{
						uint32_t dccFastClearPixelAlign = dccFastClearByteAlign / ((bitsPerElement+7)/8) / fragmentsPerSplit;
						uint32_t macroTilePixelAlign = infoOut->m_pitchAlign * infoOut->m_heightAlign;
						if (dccFastClearPixelAlign >= macroTilePixelAlign &&
							(dccFastClearPixelAlign  % macroTilePixelAlign) == 0)
						{
							uint32_t dccFastClearPitchAlignInMacroTile = dccFastClearPixelAlign / macroTilePixelAlign;
							uint32_t heightInMacroTile = outHeight / infoOut->m_heightAlign;
							while(	heightInMacroTile > 1 &&
									(heightInMacroTile % 2) == 0 &&
									dccFastClearPitchAlignInMacroTile > 1 &&
(									 dccFastClearPitchAlignInMacroTile % 2) == 0)
							{
								heightInMacroTile >>= 1;
								dccFastClearPitchAlignInMacroTile >>= 1;
							}
							uint32_t dccFastClearPitchAlignInPixels = infoOut->m_pitchAlign * dccFastClearPitchAlignInMacroTile;
							if (isPowerOfTwo(dccFastClearPitchAlignInPixels))
							{
								outPitch = (outPitch + dccFastClearPitchAlignInPixels-1) & ~(dccFastClearPitchAlignInPixels-1);
							}
							else
							{
								outPitch += (dccFastClearPitchAlignInPixels-1);
								outPitch /= dccFastClearPitchAlignInPixels;
								outPitch *= dccFastClearPitchAlignInPixels;
							}
						}
					}
				}
			}

			// hardware-specific pitch adjustment.
			// Pitch alignment is always 8, so if slice size is not padded to base alignment (pipe_interleave_size)
			// we need to increase pitch
			uint32_t tempPitch = outPitch;
			uint64_t logicalSliceSizeBytes = (uint64_t(tempPitch) * outHeight * bitsPerElement * tpTemp.m_numFragmentsPerPixel + 7) / 8;
			uint64_t physicalSliceSizeBytes = logicalSliceSizeBytes * thickness;
			while((physicalSliceSizeBytes % infoOut->m_baseAlign) != 0)
			{
				tempPitch += infoOut->m_pitchAlign;
				logicalSliceSizeBytes = (tempPitch * outHeight * bitsPerElement * tpTemp.m_numFragmentsPerPixel + 7) / 8;
				physicalSliceSizeBytes = logicalSliceSizeBytes * thickness;
			}
			// Workaround for depth/stencil: use 8bpp to align depth buffer again, since the stencil plane
			// may have a larger pitch if the slice size is smaller than the base alignment
			if (tpTemp.m_surfaceFlags.m_depthTarget /*&& !infoTemp.m_flags.m_noStencil*/) // assume m_noStencil is set; a little extra padding doesn't hurt.
			{
				uint64_t logicalSliceSizeStencilBytes = tempPitch * outHeight;
				while((logicalSliceSizeStencilBytes % infoOut->m_baseAlign) != 0)
				{
					tempPitch += infoOut->m_pitchAlign;
					logicalSliceSizeStencilBytes = tempPitch * outHeight;
				}
				if (tempPitch != outPitch)
				{
					logicalSliceSizeBytes = (logicalSliceSizeStencilBytes * bitsPerElement + 7) / 8;
				}
			}
			outPitch = tempPitch;

			// Output!
			infoOut->m_pitch = outPitch;
			infoOut->m_height = outHeight;
			infoOut->m_depth = outDepth;
			infoOut->m_surfaceSize = logicalSliceSizeBytes * outDepth;
			infoOut->m_arrayMode = arrayMode;
		}
		break;
	case Gnm::kArrayMode2dTiledThin:
	case Gnm::kArrayMode2dTiledThick:
	case Gnm::kArrayMode2dTiledXThick:
	case Gnm::kArrayMode3dTiledThin:
	case Gnm::kArrayMode3dTiledThick:
	case Gnm::kArrayMode3dTiledXThick:
	case Gnm::kArrayModeTiledThinPrt:
	case Gnm::kArrayModeTiledThickPrt:
	case Gnm::kArrayMode2dTiledThinPrt:
	case Gnm::kArrayMode2dTiledThickPrt:
	case Gnm::kArrayMode3dTiledThinPrt:
	case Gnm::kArrayMode3dTiledThickPrt:
		{
MACRO_TILE:
			Gnm::ArrayMode originalArrayMode = arrayMode; // this may change!
			thickness = getMicroTileThickness(arrayMode);
			uint32_t sliceAlignBytes = 1;
			uint32_t outPitch  = (tpTemp.m_mipLevel == 0 && tpTemp.m_baseTiledPitch > 0) ? requestedPitch : tpTemp.m_linearWidth;
			uint32_t outHeight = tpTemp.m_linearHeight;
			uint32_t outDepth = tpTemp.m_linearDepth;

			// Extract macro-tile parameters.
			Gnm::TileSplit tileSplitHW;
			status = getTileSplit(&tileSplitHW, tpTemp.m_tileMode);
			SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getTileSplit() failed for tileMode=0x%08X", tpTemp.m_tileMode);
			Gnm::SampleSplit sampleSplitHW;
			status = getSampleSplit(&sampleSplitHW, tpTemp.m_tileMode);
			SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getSampleSplit() failed for tileMode=0x%08X", tpTemp.m_tileMode);
			Gnm::BankWidth bankWidthHW;
			Gnm::BankHeight bankHeightHW;
			Gnm::MacroTileAspect macroAspectHW;
			Gnm::NumBanks numBanksHW;
			if (tpTemp.m_minGpuMode == Gnm::kGpuModeNeo)
			{
				status = getAllAltMacroTileData(tpTemp.m_tileMode, bitsPerElement, tpTemp.m_numFragmentsPerPixel,
					&bankWidthHW, &bankHeightHW, &macroAspectHW, &numBanksHW);
				SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getAllAltMacroTileData() failed for tileMode=0x%08X", tpTemp.m_tileMode);
			}
			else
			{
				status = getAllMacroTileData(tpTemp.m_tileMode, bitsPerElement, tpTemp.m_numFragmentsPerPixel,
					&bankWidthHW, &bankHeightHW, &macroAspectHW, &numBanksHW);
				SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getAllMacroTileData() failed for tileMode=0x%08X", tpTemp.m_tileMode);
			}
			status = getMicroTileMode(&microTileMode, tpTemp.m_tileMode);
			SCE_GNM_ASSERT_MSG_RETURN(status == kStatusSuccess, status, "getMicroTileMode() failed for tileMode=0x%08X", tpTemp.m_tileMode);
			// Convert to human-readable values
			uint32_t numBanks = 2<<numBanksHW;
			uint32_t macroAspect = 1<<macroAspectHW;
			uint32_t tileBytes1x = (thickness*bitsPerElement*kMicroTileWidth*kMicroTileHeight + 7)/8;
			uint32_t sampleSplit = 1 << sampleSplitHW;
			uint32_t tileSplit = (microTileMode == Gnm::kMicroTileModeDepth)
				? (64<<tileSplitHW) // depth modes store the tile split bytes directly
				: std::max(256U, sampleSplit*tileBytes1x); // other modes store a sample split multiplier
			uint32_t tileSplitC = std::min(kDramRowSize, tileSplit);
			uint32_t bankWidth = 1<<bankWidthHW;
			uint32_t bankHeight = 1<<bankHeightHW;
			// sanity-check/adjust macro-tile parameters
			// We could probably skip this step, since we assume there's nothing invalid in the TileMode table
			SCE_GNM_ASSERT_MSG_RETURN(numBanks >= macroAspect, kStatusInternalTilingError, "expected numBanks (%u) >= macroAspect (%u).", numBanks, macroAspect);
//			SCE_GNM_ASSERT_MSG_RETURN(tileSplitC <= kDramRowSize, kStatusInternalTilingError, "expected tileSplitC (%d) <= DRAM row size (%d).", tileSplitC, kDramRowSize);
			SCE_GNM_ASSERT_MSG_RETURN(numPipes * numBanks >= 4, kStatusInternalTilingError, "expected %u*numBanks (%u) >= 4.", numPipes, numBanks);
			// align bank height
			uint32_t tileSize = std::min(tileSplitC, (thickness*bitsPerElement*tpTemp.m_numFragmentsPerPixel*kMicroTileWidth*kMicroTileHeight+7)/8);
			uint32_t bankHeightAlign = std::max(1U, kPipeInterleaveBytes*kBankInterleave / (tileSize*bankWidth));
			SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(bankHeightAlign), kStatusInternalTilingError, "Expected bankHeightAlign (%u) to be a power of two.", bankHeightAlign);
			bankHeight = (bankHeight + bankHeightAlign-1) & ~(bankHeightAlign-1);
			// align macro tile aspect ratio. This restriction is only for mipmaps (where numValuesPerElement must be 1)
			if (tpTemp.m_numFragmentsPerPixel == 1)
			{
				uint32_t macroAspectAlign = std::max(1U, kPipeInterleaveBytes*kBankInterleave / (tileSize*numPipes*bankWidth));
				SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroAspectAlign), kStatusInternalTilingError, "Expected macroAspectAlign (%u) to be a power of two.", macroAspectAlign);
				macroAspect = (macroAspect+macroAspectAlign-1) & ~(macroAspectAlign-1);
			}
			// Reduce bankHeight/bankWidth if needed & possible
			if (tileSize*bankWidth*bankHeight > kDramRowSize)
			{
				bool stillGreater = true;
				// Try reducing bankWidth first
				if (stillGreater && bankWidth > 1)
				{
					while(stillGreater && bankWidth>0)
					{
						bankWidth >>= 1;
						if (bankWidth == 0)
						{
							bankWidth = 1;
							break;
						}
						stillGreater = tileSize*bankWidth*bankHeight > kDramRowSize;
					}

					// Recalculate bankHeight and ratio
					bankHeightAlign = std::max(1U, kPipeInterleaveBytes*kBankInterleave / (tileSize*bankWidth));
					SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(bankHeightAlign), kStatusInternalTilingError, "Expected bankHeightAlign (%u) to be a power of two.", bankHeightAlign);
					SCE_GNM_ASSERT_MSG_RETURN((bankHeight % bankHeightAlign) == 0, kStatusInternalTilingError, "bankHeight (%u) must be a multiple of bankHeightAlign (%u).", bankHeight, bankHeightAlign); // can't increase bankHeight, so just assert
					//bankHeight = (bankHeight + bankHeightAlign-1) & ~(bankHeightAlign-1);
					if (tpTemp.m_numFragmentsPerPixel == 1)
					{
						uint32_t macroAspectAlign = std::max(1U, kPipeInterleaveBytes*kBankInterleave / (tileSize*numPipes*bankWidth));
						SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroAspectAlign), kStatusInternalTilingError, "macroAspectAlign (%u) must be a power of two.", macroAspectAlign);
						macroAspect = (macroAspect+macroAspectAlign-1) & ~(macroAspectAlign-1);
					}
				}
				// Early quit bankHeight degradation for 64-bit z buffer
				if (tpTemp.m_surfaceFlags.m_depthTarget && bitsPerElement >= 64)
					stillGreater = false;
				// Try reducing bankHeight
				if (stillGreater && bankHeight > bankHeightAlign)
				{
					while(stillGreater && bankHeight > bankHeightAlign)
					{
						bankHeight >>= 1;
						if (bankHeight < bankHeightAlign)
						{
							bankHeight = bankHeightAlign;
							break;
						}
						stillGreater = tileSize*bankWidth*bankHeight > kDramRowSize;
					}
				}
			}
			// The required granularity for pitch is the macro tile width
			uint32_t macroTileWidth = kMicroTileWidth * bankWidth * numPipes * macroAspect;
			infoOut->m_pitchAlign = macroTileWidth;
			//if (tpTemp.m_surfaceFlags.m_display && tpTemp.m_mipLevel == 0 /*|| infoTemp.m_flags.m_overlay*/)
			//	infoOut->m_pitchAlign = (infoOut->m_pitchAlign + 31) & ~31;
			// The required granularity for height is the macro tile height
			uint32_t macroTileHeight = kMicroTileHeight * bankHeight * numBanks / macroAspect;
			infoOut->m_heightAlign = macroTileHeight;
			// The required granularity for numSlices is the micro tile thickness
			infoOut->m_depthAlign = thickness;
			// Compute base alignment
			infoOut->m_baseAlign = numPipes * bankWidth * numBanks * bankHeight * tileSize;

			bool isPrt = arrayMode == Gnm::kArrayModeTiledThinPrt ||
						 arrayMode == Gnm::kArrayMode2dTiledThinPrt ||
						 arrayMode == Gnm::kArrayModeTiledThickPrt ||
						 arrayMode == Gnm::kArrayMode2dTiledThickPrt ||
						 arrayMode == Gnm::kArrayMode3dTiledThinPrt ||
						 arrayMode == Gnm::kArrayMode3dTiledThickPrt;
			if (tpTemp.m_mipLevel == 0 && isPrt /* && family==SI */)
			{
				const uint32_t prtTileSize = 0x10000;
				uint32_t macroTileSize = macroTileWidth * macroTileHeight * tpTemp.m_numFragmentsPerPixel * bitsPerElement / 8;
				if (macroTileSize < prtTileSize)
				{
					uint32_t numMacroTiles = prtTileSize / macroTileSize;
					SCE_GNM_ASSERT_MSG_RETURN( (prtTileSize % macroTileSize) == 0, kStatusInternalTilingError, "Internal consistency check failed: PRT tile size %u is not evenly divisible by macro tile size %u", prtTileSize, macroTileSize);
					infoOut->m_pitchAlign *= numMacroTiles;
					infoOut->m_baseAlign  *= numMacroTiles;
				}
			}

			SCE_GNM_ASSERT_MSG_RETURN(kBankInterleave*kPipeInterleaveBytes <= std::min(tileSplitC, tileSize) * bankWidth * bankHeight, kStatusInternalTilingError, "Internal consistency check failed: macro tile size is too large.");
			SCE_GNM_ASSERT_MSG_RETURN(kBankInterleave*kPipeInterleaveBytes <= std::min(tileSplitC, tileSize) * bankWidth * macroAspect * numPipes, kStatusInternalTilingError, "Internal consistency check failed: macro tile width is too large.");

			// Find correct array mode for mip levels
			if (tpTemp.m_mipLevel > 0)
			{
				// Adjust the array mode
				// Compute the size of a slice
				uint64_t bytesPerSlice = static_cast<uint64_t>(outPitch) * outHeight * bitsPerElement * tpTemp.m_numFragmentsPerPixel + 7;
				bytesPerSlice /= 8; // bits to bytes
				uint32_t interleaveSize = kPipeInterleaveBytes * kBankInterleave;
				uint32_t bytesPerTile = kNumMicroTilePixels * thickness * nextPowerOfTwo(bitsPerElement) * tpTemp.m_numFragmentsPerPixel + 7;
				bytesPerTile /= 8; // bits to bytes
				// CI doesn't do this; left commented as a landmark when comparing to reference impl.
				/*
				if (infoTemp.m_numSlices < thickness)
				{
					// Degrade thick mode to thin mode, to avoid wasting too much space.
				}
				*/
				if (bytesPerTile > tileSplitC)
					bytesPerTile = tileSplitC;
				uint32_t threshold1 = bytesPerTile * numPipes * bankWidth * macroAspect;
				uint32_t threshold2 = bytesPerTile * bankWidth * bankHeight;
				// Reduce the array mode from 2D/3D to 1D in the following conditions
				bool downgradeToMicroTile = false;
				switch(arrayMode)
				{
				case Gnm::kArrayMode2dTiledThin:
				case Gnm::kArrayMode3dTiledThin:
				case Gnm::kArrayModeTiledThinPrt:
				case Gnm::kArrayMode2dTiledThinPrt:
				case Gnm::kArrayMode3dTiledThinPrt:
					if (outPitch < infoOut->m_pitchAlign ||
						outHeight < infoOut->m_heightAlign ||
						interleaveSize > threshold1 ||
						interleaveSize > threshold2)
					{
						arrayMode = Gnm::kArrayMode1dTiledThin;
						downgradeToMicroTile = true;
					}
					break;
				case Gnm::kArrayMode2dTiledThick:
				case Gnm::kArrayMode3dTiledThick:
				case Gnm::kArrayMode2dTiledXThick:
				case Gnm::kArrayMode3dTiledXThick:
				case Gnm::kArrayModeTiledThickPrt:
				case Gnm::kArrayMode2dTiledThickPrt:
				case Gnm::kArrayMode3dTiledThickPrt:
					if (outPitch < infoOut->m_pitchAlign || outHeight < infoOut->m_heightAlign)
					{
						arrayMode = Gnm::kArrayMode1dTiledThick;
						downgradeToMicroTile = true;
					}
					break;
				default:
					break; // 1d/linear modes pass through unchanged
				}

				// Depending on how the array mode changed, jump to a different path of this function
				if (downgradeToMicroTile)
					goto MICRO_TILE;
				else if (thickness != getMicroTileThickness(arrayMode))
					goto MACRO_TILE;

			}
			if (arrayMode != originalArrayMode)
			{
				goto MACRO_TILE;
			}

			// Pad dimensions -- AddrLib::PadDimensions()
			if (tpTemp.m_mipLevel > 0 && tpTemp.m_surfaceFlags.m_cube)
			{
				numDimensionsToPad = (tpTemp.m_linearDepth > 1) ? 3 : 2; // pad cubemap sub-levels when we treat it as a 3d texture
			}
			if (numDimensionsToPad == 0)
				numDimensionsToPad = 3;
			SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_pitchAlign), kStatusInternalTilingError, "pitchAlign (%u) must be a power of two.", infoOut->m_pitchAlign);
			outPitch = (outPitch + infoOut->m_pitchAlign-1) & ~(infoOut->m_pitchAlign-1);
			if (numDimensionsToPad > 1)
			{
				SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_heightAlign), kStatusInternalTilingError, "heightAlign (%u) must be a power of two.", infoOut->m_heightAlign);
				outHeight = (outHeight + infoOut->m_heightAlign-1) & ~(infoOut->m_heightAlign-1);
			}
			if (numDimensionsToPad > 2 || thickness > 1)
			{
				// For cubemap single-face, we do not pad slices.
				// If we pad it, the slice number should be set to 6 and current mip level > 1
				if (tpTemp.m_surfaceFlags.m_cube)// && (1 || surfInfoTemp.flags.cubeAsArray))
					outDepth = nextPowerOfTwo(outDepth);
				// Normal 3D textures or 2D arrays or cubemaps have a thick mode?
				if (thickness > 1)
				{
					SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(infoOut->m_depthAlign), kStatusInternalTilingError, "depthAlign (%u) must be a power of two.", infoOut->m_depthAlign);
					outDepth = (outDepth + infoOut->m_depthAlign-1) & ~(infoOut->m_depthAlign-1);
				}
			}
			// DCC-compatible MSAA surfaces require additional pitch padding.
			const bool dccCompatible = (microTileMode != Gnm::kMicroTileModeDepth) && (tpTemp.m_minGpuMode >= Gnm::kGpuModeNeo);
			if (dccCompatible && tpTemp.m_numFragmentsPerPixel > 1 && tpTemp.m_mipLevel == 0 && isMacroTiled(arrayMode))
			{
				Gnm::SampleSplit sampleSplitHW;
				Gnm::TileSplit tileSplitHW;
				GpuAddress::getSampleSplit(&sampleSplitHW, tpTemp.m_tileMode);
				GpuAddress::getTileSplit(&tileSplitHW, tpTemp.m_tileMode);
				uint32_t tileBytes1x = (thickness*bitsPerElement*kMicroTileWidth*kMicroTileHeight + 7)/8;
				uint32_t sampleSplit = 1 << sampleSplitHW;
				uint32_t tileSplit = (microTileMode == Gnm::kMicroTileModeDepth)
					? (64<<tileSplitHW) // depth modes store the tile split bytes directly
					: std::max(256U, sampleSplit*tileBytes1x); // other modes store a sample split multiplier
				uint32_t tileSplitBytes = std::min(kDramRowSize, tileSplit);


				uint32_t tileSizePerFragment = (bitsPerElement * kMicroTileWidth * kMicroTileHeight + 7) / 8;
				uint32_t fragmentsPerSplit = tileSplitBytes / tileSizePerFragment;
				if (fragmentsPerSplit < tpTemp.m_numFragmentsPerPixel)
				{
					uint32_t dccFastClearByteAlign = numPipes * kPipeInterleaveBytes * 256;
					uint32_t bytesPerSplit = (outPitch * outHeight * bitsPerElement * fragmentsPerSplit+7) / 8;
					SCE_GNM_ASSERT_MSG(isPowerOfTwo(dccFastClearByteAlign), "dccFastClearByteAlign is supposed to be a power of two...");
					if (0 != (bytesPerSplit & (dccFastClearByteAlign-1)))
					{
						uint32_t dccFastClearPixelAlign = dccFastClearByteAlign / ((bitsPerElement+7)/8) / fragmentsPerSplit;
						uint32_t macroTilePixelAlign = infoOut->m_pitchAlign * infoOut->m_heightAlign;
						if (dccFastClearPixelAlign >= macroTilePixelAlign &&
							(dccFastClearPixelAlign  % macroTilePixelAlign) == 0)
						{
							uint32_t dccFastClearPitchAlignInMacroTile = dccFastClearPixelAlign / macroTilePixelAlign;
							uint32_t heightInMacroTile = outHeight / infoOut->m_heightAlign;
							while(	heightInMacroTile > 1 &&
									(heightInMacroTile % 2) == 0 &&
									dccFastClearPitchAlignInMacroTile > 1 &&
(									 dccFastClearPitchAlignInMacroTile % 2) == 0)
							{
								heightInMacroTile >>= 1;
								dccFastClearPitchAlignInMacroTile >>= 1;
							}
							uint32_t dccFastClearPitchAlignInPixels = infoOut->m_pitchAlign * dccFastClearPitchAlignInMacroTile;
							if (isPowerOfTwo(dccFastClearPitchAlignInPixels))
							{
								outPitch = (outPitch + dccFastClearPitchAlignInPixels-1) & ~(dccFastClearPitchAlignInPixels-1);
							}
							else
							{
								outPitch += (dccFastClearPitchAlignInPixels-1);
								outPitch /= dccFastClearPitchAlignInPixels;
								outPitch *= dccFastClearPitchAlignInPixels;
							}
						}
					}
				}
			}


			// Compute size of a slice
			uint64_t bytesPerSlice = static_cast<uint64_t>(outPitch) * outHeight * nextPowerOfTwo(bitsPerElement) * tpTemp.m_numFragmentsPerPixel + 7;
			bytesPerSlice /= 8; // bits to bytes;
			// Pad height until the slice slice meets the alignment requirements
			while(bytesPerSlice & (sliceAlignBytes-1))
			{
				outHeight += infoOut->m_heightAlign;
				bytesPerSlice = static_cast<uint64_t>(outPitch) * outHeight * nextPowerOfTwo(bitsPerElement) * tpTemp.m_numFragmentsPerPixel + 7;
				bytesPerSlice /= 8; // bits to bytes;
			}
			// Output!
			infoOut->m_pitch = outPitch;
			infoOut->m_height = outHeight;
			infoOut->m_depth = outDepth;
			infoOut->m_surfaceSize = bytesPerSlice * outDepth;
			infoOut->m_arrayMode = arrayMode;
		}
		break;
	default:
		SCE_GNM_ERROR("Invalid arrayMode (0x%02X).", arrayMode);
		return kStatusInvalidArgument;
	}

	// Restore dimensions (basically, take block-compressed textures back to per-pixel instead of per-block calculations)
	infoOut->m_pitch  *= expandX;
	infoOut->m_height *= expandY;
	infoOut->m_pitchAlign  *= expandX;
	infoOut->m_heightAlign *= expandY;
	if (tp->m_surfaceFlags.m_pow2Pad)
	{
		infoOut->m_pitch = nextPowerOfTwo(infoOut->m_pitch);
		infoOut->m_height = nextPowerOfTwo(infoOut->m_height);
		infoOut->m_depth = nextPowerOfTwo(infoOut->m_depth);
	}
	status = adjustTileMode(tpTemp.m_minGpuMode, &infoOut->m_tileMode, tp->m_tileMode, infoOut->m_arrayMode);
	if (status != kStatusSuccess)
		return status;

	// If a specific pitch was patched in for mip level 0, it must exactly match what we calculated above based on
	// hardware restrictions. Otherwise, error!
	if (tp->m_mipLevel == 0 && tp->m_baseTiledPitch > 0 && tp->m_baseTiledPitch != infoOut->m_pitch)
		return kStatusInvalidArgument;

	// Fill in s'more output
	//pSurfInfoOut->pixelPitch = pSurfInfoOut->pitch;
	//pSurfInfoOut->pixelHeight = pSurfInfoOut->height;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeCmaskInfo(uint64_t *outCmaskSizeBytes, uint32_t *outCmaskAlign, uint32_t *outCmaskPitch, uint32_t *outCmaskHeight,
	const Gnm::RenderTarget *target)
{
	SCE_GNM_ASSERT_MSG_RETURN(target, kStatusInvalidArgument, "target must not be NULL");
	uint32_t bitsPerElement = 4;
	uint32_t cacheBits = kCmaskCacheBits;
	uint32_t macroWidth = 0;
	uint32_t macroHeight = 0;
	Gnm::PipeConfig pipeConfig;
	Gnm::TileMode tileMode = target->getTileMode();
	int32_t status = 0;
	if (target->getUseAltTileMode())
	{
		status = getAltPipeConfig(&pipeConfig, tileMode);
	}
	else
	{
		status = getPipeConfig(&pipeConfig, tileMode);
	}
	if (status != kStatusSuccess)
		return status;
	uint32_t numPipes = getPipeCount(pipeConfig);

	uint32_t rtPitch = target->getPitch();
	uint32_t rtHeight = target->getHeight();
	uint32_t rtNumSlices = 1 + target->getLastArraySliceIndex();
	bool isCmaskLinear = target->getLinearCmask();

	if (isCmaskLinear)
	{
		macroWidth  = 8 * kMicroTileWidth;
		macroHeight = 8 * kMicroTileHeight;
	}
	else
	{
		uint32_t h = 1;
		uint32_t w = cacheBits / bitsPerElement;
		while((w>h*2*numPipes) && !(w&1))
		{
			w /= 2;
			h *= 2;
		}
		macroWidth  = 8*w;
		macroHeight = 8*h*numPipes;
	}
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroWidth), kStatusInternalTilingError, "macroWidth (%u) must be a power of two.", macroWidth);
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroHeight), kStatusInternalTilingError, "macroHeight (%u) must be a power of two.", macroHeight);
	uint32_t outPitch = (rtPitch + macroWidth - 1) & ~(macroWidth-1);
	uint32_t outHeight = (rtHeight + macroHeight - 1) & ~(macroHeight-1);
	uint64_t sliceBytes = (static_cast<uint64_t>(outPitch) * outHeight * bitsPerElement) /
		(8ULL*kNumMicroTilePixels);
	// Compute size and alignment
	uint32_t cmaskAlign = kPipeInterleaveBytes * numPipes;
	while(sliceBytes % cmaskAlign)
	{
		outHeight += macroHeight;
		sliceBytes = (static_cast<uint64_t>(outPitch) * outHeight * bitsPerElement) /
			(8ULL*kNumMicroTilePixels);
	}
	if (outCmaskSizeBytes)
		*outCmaskSizeBytes = sliceBytes * rtNumSlices;
	if (outCmaskAlign)
		*outCmaskAlign = cmaskAlign;
	if (outCmaskPitch)
		*outCmaskPitch = outPitch;
	if (outCmaskHeight)
		*outCmaskHeight = outHeight;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeCmaskInfo(uint64_t *outCmaskSizeBytes, uint32_t *outCmaskAlign, uint32_t *outCmaskPitch, uint32_t *outCmaskHeight,
	uint32_t rtPitch, uint32_t rtHeight, uint32_t rtNumSlices, bool isCmaskLinear) // DEPRECATED
{
	uint32_t bitsPerElement = 4;
	uint32_t cacheBits = kCmaskCacheBits;
	uint32_t macroWidth = 0;
	uint32_t macroHeight = 0;
	uint32_t numPipes = 8; // In this variant, we have to guess the pipe count.
	if (isCmaskLinear)
	{
		macroWidth  = 8 * kMicroTileWidth;
		macroHeight = 8 * kMicroTileHeight;
	}
	else
	{
		uint32_t h = 1;
		uint32_t w = cacheBits / bitsPerElement;
		while((w>h*2*numPipes) && !(w&1))
		{
			w /= 2;
			h *= 2;
		}
		macroWidth  = 8*w;
		macroHeight = 8*h*numPipes;
	}
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroWidth), kStatusInternalTilingError, "macroWidth (%u) must be a power of two.", macroWidth);
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroHeight), kStatusInternalTilingError, "macroHeight (%u) must be a power of two.", macroHeight);
	uint32_t outPitch = (rtPitch + macroWidth - 1) & ~(macroWidth-1);
	uint32_t outHeight = (rtHeight + macroHeight - 1) & ~(macroHeight-1);
	uint64_t sliceBytes = (static_cast<uint64_t>(outPitch) * outHeight * bitsPerElement) /
		(8ULL*kNumMicroTilePixels);
	// Compute size and alignment
	uint32_t cmaskAlign = kPipeInterleaveBytes * numPipes;
	while(sliceBytes % cmaskAlign)
	{
		outHeight += macroHeight;
		sliceBytes = (static_cast<uint64_t>(outPitch) * outHeight * bitsPerElement) /
			(8ULL*kNumMicroTilePixels);
	}
	if (outCmaskSizeBytes)
		*outCmaskSizeBytes = sliceBytes * rtNumSlices;
	if (outCmaskAlign)
		*outCmaskAlign = cmaskAlign;
	if (outCmaskPitch)
		*outCmaskPitch = outPitch;
	if (outCmaskHeight)
		*outCmaskHeight = outHeight;
	return kStatusSuccess;
}

int32_t sce::GpuAddress::computeFmaskInfo(uint64_t *outFmaskSizeBytes, uint32_t *outFmaskAlign, uint32_t *outFmaskPitch, uint32_t *outFmaskHeight,
			const Gnm::RenderTarget *target)
{
	SCE_GNM_ASSERT_MSG_RETURN(target, kStatusInvalidArgument, "target must not be NULL");

	uint32_t rtPitch = target->getPitch();
	uint32_t rtHeight = target->getHeight();
	uint32_t rtNumSlices = 1 + target->getLastArraySliceIndex();
	Gnm::DataFormat fmaskFormat = Gnm::DataFormat::build(target->getNumSamples(), target->getNumFragments());
	Gnm::TileMode fmaskTileMode = target->getFmaskTileMode();

	// Forward to computeSurfaceInfo
	TilingParameters tp = {};
	int32_t status;
	tp.m_minGpuMode = target->getMinimumGpuMode();
	tp.m_bitsPerFragment = fmaskFormat.getTotalBitsPerElement() / fmaskFormat.getTexelsPerElement();
	tp.m_isBlockCompressed = (fmaskFormat.getTexelsPerElement() > 1);
	tp.m_tileSwizzleMask = target->getFmaskTileSwizzleMask();
	tp.m_linearHeight = rtHeight;
	tp.m_mipLevel = 0;
	tp.m_numFragmentsPerPixel = 1;
	tp.m_linearDepth = 1;
	tp.m_tileMode = fmaskTileMode;
	tp.m_linearWidth = rtPitch;
	tp.m_baseTiledPitch = 0; // FMASK surfaces can't have arbitrary pitches, so overriding the pitch isn't really an option.
	tp.m_arraySlice = 0; // not used by computeSurfaceInfo
	status = getFlagsForSurfaceType(tp.m_minGpuMode, &tp.m_surfaceFlags, kSurfaceTypeFmaskBuffer, kSurfaceMipmapDisable);
	if (status != kStatusSuccess)
		return status;
	tp.m_surfaceFlags.m_texCompatible = (target->getMinimumGpuMode() == Gnm::kGpuModeNeo) ? 1 : 0;
	SurfaceInfo infoOut;
	memset(&infoOut, 0, sizeof(infoOut));
	status = computeSurfaceInfo(&infoOut, &tp);
	if (outFmaskSizeBytes)
		*outFmaskSizeBytes = infoOut.m_surfaceSize * rtNumSlices;
	if (outFmaskAlign)
		*outFmaskAlign = infoOut.m_baseAlign;
	if (outFmaskPitch)
		*outFmaskPitch = infoOut.m_pitch;
	if (outFmaskHeight)
		*outFmaskHeight = infoOut.m_height;
	return status;
}
int32_t sce::GpuAddress::computeFmaskInfo(uint64_t *outFmaskSizeBytes, uint32_t *outFmaskAlign, uint32_t *outFmaskPitch, uint32_t *outFmaskHeight,
	uint32_t rtPitch, uint32_t rtHeight, uint32_t rtNumSlices, Gnm::TileMode fmaskTileMode, Gnm::DataFormat fmaskFormat) // DEPRECATED
{
	// Forward to computeSurfaceInfo
	TilingParameters tp = {};
	int32_t status;
	tp.m_minGpuMode = Gnm::kGpuModeBase;
	tp.m_bitsPerFragment = fmaskFormat.getTotalBitsPerElement() / fmaskFormat.getTexelsPerElement();
	tp.m_isBlockCompressed = (fmaskFormat.getTexelsPerElement() > 1);
	tp.m_tileSwizzleMask = 0; // Can safely be zero here, since all we care about is surface size, not individual element locations.
	tp.m_linearHeight = rtHeight;
	tp.m_mipLevel = 0;
	tp.m_numFragmentsPerPixel = 1;
	tp.m_linearDepth = 1;
	tp.m_tileMode = fmaskTileMode;
	tp.m_linearWidth = rtPitch;
	tp.m_baseTiledPitch = 0; // FMASK surfaces can't have arbitrary pitches, so overriding the pitch isn't really an option.
	tp.m_arraySlice = 0; // not used by computeSurfaceInfo
	status = getFlagsForSurfaceType(tp.m_minGpuMode, &tp.m_surfaceFlags, kSurfaceTypeFmaskBuffer, kSurfaceMipmapDisable);
	if (status != kStatusSuccess)
		return status;
	tp.m_surfaceFlags.m_texCompatible = 0;
	SurfaceInfo infoOut;
	memset(&infoOut, 0, sizeof(infoOut));
	status = computeSurfaceInfo(&infoOut, &tp);
	if (outFmaskSizeBytes)
		*outFmaskSizeBytes = infoOut.m_surfaceSize * rtNumSlices;
	if (outFmaskAlign)
		*outFmaskAlign = infoOut.m_baseAlign;
	if (outFmaskPitch)
		*outFmaskPitch = infoOut.m_pitch;
	if (outFmaskHeight)
		*outFmaskHeight = infoOut.m_height;
	return status;
}


int32_t sce::GpuAddress::computeHtileInfo(uint64_t *outHtileSizeBytes, uint32_t *outHtileAlign, uint32_t *outHtilePitch, uint32_t *outHtileHeight,
	const Gnm::DepthRenderTarget *depthTarget)
{
	SCE_GNM_ASSERT_MSG_RETURN(depthTarget, kStatusInvalidArgument, "depthTarget must not be NULL");

	const uint32_t bitsPerElement = 32;
	const uint32_t cacheBits = kHtileCacheBits;
	uint32_t macroWidth = 0;
	uint32_t macroHeight = 0;
	Gnm::PipeConfig pipeConfig = depthTarget->getPipeConfig();
	uint32_t numPipes = getPipeCount(pipeConfig);

	uint32_t rtPitch = depthTarget->getPitch();
	uint32_t rtHeight = depthTarget->getHeight();
	uint32_t rtNumSlices = 1 + depthTarget->getLastArraySliceIndex();
	bool isHtileLinear = depthTarget->getLinearHtile();
	if (isHtileLinear)
	{
		uint32_t numTiles = 8;
		macroWidth  = numTiles * kMicroTileWidth;
		macroHeight = numTiles * kMicroTileHeight;
	}
	else
	{
		uint32_t h = 1;
		uint32_t w = cacheBits / bitsPerElement;
		// Double height until the macro-tile is close to square
		while((w>h*2*numPipes) && !(w&1))
		{
			w /= 2;
			h *= 2;
		}
		macroWidth  = 8 * w;
		macroHeight = 8 * h * numPipes;
	}
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroWidth), kStatusInternalTilingError, "macroWidth (%u) must be a power of two.", macroWidth);
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroHeight), kStatusInternalTilingError, "macroHeight (%u) must be a power of two.", macroHeight);
	uint32_t htilePitch  = (rtPitch  + (macroWidth -1)) & ~(macroWidth -1);
	uint32_t htileHeight = (rtHeight + (macroHeight-1)) & ~(macroHeight-1);
	// Compute size and alignment
	bool isTcCompatible = (depthTarget->getMinimumGpuMode() == Gnm::kGpuModeNeo && depthTarget->getHtileTextureCompatible()) ? 1 : 0;
	uint32_t htileAlign = kPipeInterleaveBytes * numPipes;
	if (isTcCompatible)
	{
		Gnm::NumBanks numBanksHw = Gnm::kNumBanks8;
		htileAlign *= 2<<numBanksHw;
	}
	uint64_t surfaceBytes = static_cast<uint64_t>(htilePitch) * htileHeight * bitsPerElement * rtNumSlices / 64;
	surfaceBytes /= 8; // bits to bytes
	const uint64_t htileCacheLineSize = kHtileCacheBits/8;
	const uint64_t cacheAlign = htileCacheLineSize * numPipes;
	uint64_t htileBytes = (surfaceBytes + (cacheAlign-1)) & ~(cacheAlign-1);
	if (outHtileSizeBytes)
		*outHtileSizeBytes = htileBytes;
	if (outHtileAlign)
		*outHtileAlign = htileAlign;
	if (outHtilePitch)
		*outHtilePitch = htilePitch;
	if (outHtileHeight)
		*outHtileHeight = htileHeight;
	return kStatusSuccess;
}
int32_t sce::GpuAddress::computeHtileInfo(uint64_t *outHtileSizeBytes, uint32_t *outHtileAlign, uint32_t *outHtilePitch, uint32_t *outHtileHeight,
	uint32_t rtPitch, uint32_t rtHeight, uint32_t rtNumSlices, bool isHtileLinear) // DEPRECATED
{
	const uint32_t bitsPerElement = 32;
	const uint32_t cacheBits = kHtileCacheBits;
	uint32_t macroWidth = 0;
	uint32_t macroHeight = 0;
	uint32_t numPipes = 8; // In this variant, we have to guess the pipe count.
	if (isHtileLinear)
	{
		uint32_t numTiles = 8;
		macroWidth  = numTiles * kMicroTileWidth;
		macroHeight = numTiles * kMicroTileHeight;
	}
	else
	{
		uint32_t h = 1;
		uint32_t w = cacheBits / bitsPerElement;
		// Double height until the macro-tile is close to square
		while((w>h*2*numPipes) && !(w&1))
		{
			w /= 2;
			h *= 2;
		}
		macroWidth  = 8 * w;
		macroHeight = 8 * h * numPipes;
	}
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroWidth), kStatusInternalTilingError, "macroWidth (%u) must be a power of two.", macroWidth);
	SCE_GNM_ASSERT_MSG_RETURN(isPowerOfTwo(macroHeight), kStatusInternalTilingError, "macroHeight (%u) must be a power of two.", macroHeight);
	uint32_t htilePitch  = (rtPitch  + (macroWidth -1)) & ~(macroWidth -1);
	uint32_t htileHeight = (rtHeight + (macroHeight-1)) & ~(macroHeight-1);
	// Compute size and alignment
	uint32_t htileAlign = kPipeInterleaveBytes * numPipes;
	uint64_t surfaceBytes = static_cast<uint64_t>(htilePitch) * htileHeight * bitsPerElement * rtNumSlices / 64;
	surfaceBytes /= 8; // bits to bytes
	const uint64_t htileCacheLineSize = kHtileCacheBits/8;
	const uint64_t cacheAlign = htileCacheLineSize * numPipes;
	uint64_t htileBytes = (surfaceBytes + (cacheAlign-1)) & ~(cacheAlign-1);
	if (outHtileSizeBytes)
		*outHtileSizeBytes = htileBytes;
	if (outHtileAlign)
		*outHtileAlign = htileAlign;
	if (outHtilePitch)
		*outHtilePitch = htilePitch;
	if (outHtileHeight)
		*outHtileHeight = htileHeight;
	return kStatusSuccess;
}

static inline uintptr_t addrMask(uint32_t width)
{
	uintptr_t ret;
	if (width >= sizeof(uintptr_t)*8)
	{
		ret = ~((uintptr_t)0);
	}
	else
	{
		return (((uintptr_t)1) << width) - 1;
	}
	return ret;
}
static inline uintptr_t addrGetBits(uintptr_t addr, uint32_t msb, uint32_t lsb)
{
	uintptr_t ret = 0;
	if (msb >= lsb)
	{
		ret = (addr >> lsb) & addrMask(1+msb-lsb);
	}
	return ret;
}
static inline uintptr_t addrRemoveBits(uintptr_t addr, uint32_t msb, uint32_t lsb)
{
	uintptr_t ret = addr;
	if (msb >= lsb)
	{
		ret = (addrGetBits(addr, lsb-1, 0) << 0)
			| (addrGetBits(addr, 8*sizeof(addr)-1, msb+1)<<lsb);
	}
	return ret;
}
static inline uintptr_t addrInsertBits(uintptr_t addr, uintptr_t newBits, uint32_t msb, uint32_t lsb)
{
	uintptr_t ret = addr;
	if (msb >= lsb)
	{
		ret = (addrGetBits(addr, lsb-1, 0) << 0)
			| (addrGetBits(newBits, msb-lsb, 0) << lsb)
			| (addrGetBits(addr, 8*sizeof(addr)-1, lsb) << (msb+1));
	}
	return ret;
}

// TODO: not verified
uintptr_t computeMetadataNibbleAddress(void *cbElem, void *cbBase, void *metaBase,
	uint32_t metaBitSize, uint32_t bitsPerElement, uint32_t blockByteSize, Gnm::NumBanks numBanksHw,
	uint32_t numSamplesPerSplit)
{
	uint32_t pipeInterleaveBits = fastIntLog2(kPipeInterleaveBytes);
	const uint32_t numPipes = 16; // DCC requires useAltTileMode=1, which implies 16 pipes
	uint32_t pipeBits = fastIntLog2(numPipes);
	uint32_t bankBits = (uint32_t)numBanksHw + 1;
	
	uint32_t macrotileBits = pipeInterleaveBits + pipeBits + bankBits;
	uintptr_t macrotileClearMask = ~((1L << macrotileBits) -1);
	uintptr_t cbBaseAligned = (uintptr_t)cbBase & macrotileClearMask;

	SCE_GNM_ASSERT_MSG_RETURN(metaBitSize != 0, 0, "metaBitSize must not be zero");
	uintptr_t metaBaseShifted = uintptr_t(metaBase) * blockByteSize*8 / metaBitSize;
	uintptr_t offset = uintptr_t(cbElem) - cbBaseAligned + metaBaseShifted;

	uint32_t lsb = pipeBits + pipeInterleaveBits;
	uint32_t msb = bankBits - 1 + lsb;
	uintptr_t bankDataBits = addrGetBits(offset, msb, lsb);

	lsb = pipeInterleaveBits;
	msb = pipeBits - 1 + lsb;
	uintptr_t pipeDataBits = addrGetBits(offset, msb, lsb);

	lsb = pipeInterleaveBits;
	msb = macrotileBits - 1;
	uintptr_t offsetWithoutPipeBankBits = addrRemoveBits(offset, msb, lsb);

	SCE_GNM_ASSERT_MSG_RETURN(blockByteSize != 0, 0, "blockByteSize must not be zero");
	uintptr_t blockInBankPipe = offsetWithoutPipeBankBits / blockByteSize;

	uint32_t tileSize = kMicroTileWidth * kMicroTileHeight * bitsPerElement/8 * numSamplesPerSplit;
	uint32_t blocksInTile = tileSize / blockByteSize;

	lsb = (blocksInTile == 0) ? 0 : fastIntLog2(blocksInTile);
	msb = bankBits - 1 + lsb;

	uintptr_t blockInBankPipeWithBankBits = addrInsertBits(blockInBankPipe, bankDataBits, msb, lsb);
	uintptr_t metaAddressInPipe = blockInBankPipeWithBankBits * 2 * metaBitSize / 8;

	lsb = pipeInterleaveBits + 1;
	msb = pipeBits - 1 + lsb;
	uintptr_t metaAddress = addrInsertBits(metaAddressInPipe, pipeDataBits, msb, lsb);
	return metaAddress;
}

int32_t sce::GpuAddress::computeDccInfo(uint64_t *outDccSizeBytes, uint32_t *outDccAlign, const Gnm::RenderTarget *target)
{
	SCE_GNM_ASSERT_MSG_RETURN(target, kStatusInvalidArgument, "target must not be NULL");

	Gnm::TileMode cbTileMode = target->getTileMode();
	uint32_t cbBitsPerElement = target->getDataFormat().getBitsPerElement();
	uint32_t cbNumFragmentsPerPixel = 1 << target->getNumFragments();
	uint32_t cbSliceSizeBytes = target->getSliceSizeInBytes();
	uint32_t cbNumSlices = 1 + target->getLastArraySliceIndex();

	int32_t status;
	Gnm::ArrayMode arrayMode;
	status = GpuAddress::getArrayMode(&arrayMode, cbTileMode);
	SCE_GNM_ASSERT_MSG_RETURN( status == kStatusSuccess, status, "Could not extract ArrayMode from cbTileMode (%d)", cbTileMode);
	SCE_GNM_ASSERT_MSG_RETURN( isMacroTiled(arrayMode), kStatusInvalidArgument, "cbTileMode (%d) must be macro-tiled in order to use DCC surfaces.", cbTileMode);

	Gnm::TileSplit tileSplitHw;
	status = GpuAddress::getTileSplit(&tileSplitHw, cbTileMode);
	SCE_GNM_ASSERT_MSG_RETURN( status == kStatusSuccess, status, "Could not extract TileSplit from cbTileMode (%d)", cbTileMode);

	Gnm::SampleSplit sampleSplitHw;
	status = GpuAddress::getSampleSplit(&sampleSplitHw, cbTileMode);
	SCE_GNM_ASSERT_MSG_RETURN( status == kStatusSuccess, status, "Could not extract SampleSplit from cbTileMode (%d)", cbTileMode);

	Gnm::NumBanks numBanksHw;
	// DCC requires useAltTileMode, so we can safely call getAltNumBanks() here
	status = GpuAddress::getAltNumBanks(&numBanksHw, cbTileMode, cbBitsPerElement, cbNumFragmentsPerPixel);
	SCE_GNM_ASSERT_MSG_RETURN( status == kStatusSuccess, status, "Could not extract NumBanks from cbTileMode (%d)", cbTileMode);
	uint32_t numBanks = 2 << numBanksHw;
	
	Gnm::MicroTileMode microTileMode;
	status = GpuAddress::getMicroTileMode(&microTileMode, cbTileMode);
	SCE_GNM_ASSERT_MSG_RETURN( status == kStatusSuccess, status, "Could not extract MicroTileMode from cbTileMode (%d)", cbTileMode);

	const uint32_t numPipes = 16; // DCC requires useAltTileMode=1, which implies 16 pipes

	uint32_t tileThickness = getMicroTileThickness(arrayMode);
	uint32_t tileBytes1x = cbBitsPerElement*kMicroTileWidth*kMicroTileHeight*tileThickness / 8;
	uint32_t sampleSplit = 1<<sampleSplitHw;
	uint32_t colorTileSplit = std::max(256U, sampleSplit*tileBytes1x);
	uint32_t tileSplit = (microTileMode == Gnm::kMicroTileModeDepth)
		? (64UL << tileSplitHw) : colorTileSplit;
	uint32_t tileSplitC = std::min(kDramRowSize, tileSplit);
	uint32_t tileSplitBytes = std::min(tileSplitC, cbNumFragmentsPerPixel*tileBytes1x);

	uint64_t dccFastClearSize = cbSliceSizeBytes / 256;
	if (cbNumFragmentsPerPixel > 1)
	{
		uint32_t tileSizePerFragment = cbBitsPerElement * kMicroTileWidth * kMicroTileHeight / 8;
		uint32_t samplesPerSplit = tileSplitBytes / tileSizePerFragment;
		// Prevent divide by zero below. This can also be avoided if we forbid depth
		// microtiling for DCC surfaces (which seems reasonable, but hasn't been officially confirmed yet).
		samplesPerSplit = std::max(1U, samplesPerSplit);
		if (samplesPerSplit < cbNumFragmentsPerPixel)
		{
			uint32_t numSplits = cbNumFragmentsPerPixel / samplesPerSplit;
			uint32_t fastClearBaseAlign = numPipes * kPipeInterleaveBytes;
			SCE_GNM_ASSERT( isPowerOfTwo(fastClearBaseAlign) );
			dccFastClearSize /= numSplits;
			if ( 0 != (dccFastClearSize & (fastClearBaseAlign-1)) )
			{
				// disable DCC fast clear if key size of first sample split isn't pipeinterleave aligned
				dccFastClearSize = 0;
			}
		}
	}
	uint64_t dccSize = (cbSliceSizeBytes+255) / 256;
	uint32_t dccAlign = numBanks * numPipes * kPipeInterleaveBytes;
	SCE_GNM_ASSERT( isPowerOfTwo(dccAlign) );
	bool subLevelCompressible;
	if ( 0 == (dccSize & (dccAlign-1)) )
	{
		subLevelCompressible = true;
	}
	else
	{
		uint64_t dccSizeAlign = numPipes * kPipeInterleaveBytes;
		if (dccSize == dccFastClearSize)
		{
			dccFastClearSize = (dccSize + dccSizeAlign-1) & ~(dccSizeAlign-1);
		}
		dccSize = (dccSize + dccSizeAlign-1) & ~(dccSizeAlign-1);
		subLevelCompressible = false;
	}
	if (outDccSizeBytes)
		*outDccSizeBytes = dccSize * cbNumSlices;
	if (outDccAlign)
		*outDccAlign = dccAlign;
	return kStatusSuccess;
}
