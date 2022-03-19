#pragma once

#include "GnmGpuAddress.h"
#include "Gnm/GnmConstant.h"


namespace sce
{
	namespace GpuAddress
	{
		/** @brief Returns the first power of two greater than or equal to x.
			*/
		uint32_t nextPowerOfTwo(uint32_t x);

		/** @brief Returns true if x is a power of two, and false if not.
			*/
		bool isPowerOfTwo(uint32_t x);

		/** @brief Returns a fast approximation of the base-2 logarithm.
			@param[in] f Input value. Must be positive, non-infinite, and non-denormalized.
			*/
		uint32_t fastLog2(float f);

		/** @brief Returns floor(log2(i)).
			@param[in] i Input value.
			*/
        inline uint32_t fastIntLog2(uint32_t i)
        {
            return 31 - __builtin_clz(i | 1);

	        //unsigned long temp;
	        //_BitScanReverse(&temp, i | 1);
            //return temp;
        }

		uint32_t getPipeCount(Gnm::PipeConfig pipeConfig);

		/**
		 * @brief Degrades thick tile modes if the tiles will be too large.
		 * @param oldArrayMode Current array mode.
		 * @param bitsPerElement Bits per surface element.
		 * @return The new array mode (which may be the same as the old one).
		 */
		Gnm::ArrayMode degradeLargeThickArrayMode(Gnm::ArrayMode oldArrayMode, uint32_t bitsPerElement);

		/** @brief Returns the thickness of one micro-tile in the specified array mode.
			@param arrayMode The array mode to query.
			@return The thickness of one micro-tile: 1 for Thin modes, 4 for Thick modes, and 8 for XThick modes.
		*/
		uint32_t getMicroTileThickness(Gnm::ArrayMode arrayMode);

		/** @brief Indicates whether an array mode uses linear tiling.
			@param arrayMode The array mode to query.
			@return True if arrayMode uses linear tiling, or false otherwise.
		*/
		bool isLinear(Gnm::ArrayMode arrayMode);

		/** @brief Indicates whether an array mode uses 1D (micro) tiling.
			@note This function will return false for 2D/3D-tiled surfaces, even though they technically do use micro-tiles.
			@param arrayMode The array mode to query.
			@return True if arrayMode uses 1D/micro tiling, or false otherwise.
		*/
		bool isMicroTiled(Gnm::ArrayMode arrayMode);

		/** @brief Indicates whether an array mode uses 2D or 3D (macro) tiling.
			@param arrayMode The array mode to query.
			@return True if arrayMode uses 2D/3D/macro tiling, or false otherwise.
		*/
		bool isMacroTiled(Gnm::ArrayMode arrayMode);

		/** @brief Indicates whether an array mode supports partially-resident textures.
			@param arrayMode The array mode to query.
			@return True if arrayMode supports partially-resident textures, or false otherwise.
		*/
		bool isPartiallyResidentTexture(Gnm::ArrayMode arrayMode);

		// Various hardware-specific constants
		const uint32_t kDramRowSize = 0x400;
		const uint32_t kNumLogicalBanks = 16;
		const uint32_t kPipeInterleaveBytes = 256;
		const uint32_t kBankInterleave = 1;
		const uint32_t kMicroTileWidth = 8;
		const uint32_t kMicroTileHeight = 8;
		const uint32_t kNumMicroTilePixels = kMicroTileWidth*kMicroTileHeight;
		const uint32_t kCmaskCacheBits = 0x400;
		const uint32_t kHtileCacheBits = 0x4000;
	}
}

