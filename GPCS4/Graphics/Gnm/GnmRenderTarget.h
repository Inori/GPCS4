#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"
#include "GnmRegInfo.h"
#include "GnmStructure.h"
#include "GpuAddress/GnmGpuAddress.h"
#include "Emulator.h"
#include "VirtualGPU.h"
#include "UtilBit.h"

namespace sce::Gnm
{

	union RenderTargetInitFlags
	{
		struct
		{
			uint32_t enableCmaskFastClear : 1;
			uint32_t enableFmaskCompression : 1;
			uint32_t enableColorTextureWithoutDecompress : 1;
			uint32_t enableFmaskTextureWithoutDecompress : 1;
			uint32_t enableDccCompression : 1;
			uint32_t reserved : 27;
		};
		int32_t asInt;
	};

	class RenderTargetSpec
	{
	public:
		void init(void)
		{
			// From IDA
			m_width             = 0;
			m_height            = 0;
			m_pitch             = 0;
			m_numSlices         = 1;
			m_colorFormat       = DataFormat::build(kSurfaceFormatInvalid,
                                              kTextureChannelTypeFloat,
                                              kTextureChannelConstant0,
                                              kTextureChannelConstant0,
                                              kTextureChannelConstant0,
                                              kTextureChannelConstant0);
			m_colorTileModeHint = kTileModeDisplay_LinearAligned;
			m_minGpuMode        = GPU().mode();
			m_numSlices         = kNumSamples1;
			m_numFragments      = kNumFragments1;
			m_flags.asInt       = 0;
		}

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_pitch;

		uint32_t                   m_numSlices;
		Gnm::DataFormat            m_colorFormat;
		Gnm::TileMode              m_colorTileModeHint;
		Gnm::GpuMode               m_minGpuMode;
		Gnm::NumSamples            m_numSamples;
		Gnm::NumFragments          m_numFragments;
		Gnm::RenderTargetInitFlags m_flags;    
	};

	class alignas(16) RenderTarget
	{
	public:
		enum
		{
			kCbColorBase       = mmCB_COLOR0_BASE - mmCB_COLOR0_BASE,
			kCbColorPitch      = mmCB_COLOR0_PITCH - mmCB_COLOR0_BASE,
			kCbColorSlice      = mmCB_COLOR0_SLICE - mmCB_COLOR0_BASE,
			kCbColorView       = mmCB_COLOR0_VIEW - mmCB_COLOR0_BASE,
			kCbColorInfo       = mmCB_COLOR0_INFO - mmCB_COLOR0_BASE,
			kCbColorAttrib     = mmCB_COLOR0_ATTRIB - mmCB_COLOR0_BASE,
			kCbColorDccControl = mmCB_COLOR0_DCC_CONTROL - mmCB_COLOR0_BASE,
			kCbColorCmask      = mmCB_COLOR0_CMASK - mmCB_COLOR0_BASE,
			kCbColorCmaskSlice = mmCB_COLOR0_CMASK_SLICE - mmCB_COLOR0_BASE,
			kCbColorFmask      = mmCB_COLOR0_FMASK - mmCB_COLOR0_BASE,
			kCbColorFmaskSlice = mmCB_COLOR0_FMASK_SLICE - mmCB_COLOR0_BASE,
			kCbColorClearWord0 = mmCB_COLOR0_CLEAR_WORD0 - mmCB_COLOR0_BASE,
			kCbColorClearWord1 = mmCB_COLOR0_CLEAR_WORD1 - mmCB_COLOR0_BASE,
			kCbColorDccBase    = mmCB_COLOR0_DCC_BASE - mmCB_COLOR0_BASE,
			// 14: unused
			kCbWidthHeight = 15,  // not a GPU register. width in [15:0], height in [31:16].
			kNumCbRegisters
		};

		RenderTarget& operator=(const RenderTarget& other)
		{
			std::memcpy(m_regs, other.m_regs, sizeof(m_regs));
			return *this;
		}

		bool operator==(const RenderTarget& other) const
		{
			return util::bit::bcmpeq(this, &other);
		}

		bool operator!=(const RenderTarget& other) const
		{
			return !util::bit::bcmpeq(this, &other);
		}


		// NOTE:
		// This method is only used to track display buffer.
		// Use this to initialize a render target representing the display buffer.
		// Do not call it for other use case.
		//
		// TODO:
		// As we are not supposed to call it for other use case.
		// it is not implement completely and most part of it is just a coppy 
		// of messy IDA decompiler output, it need to be refactor for human friendly.
		// 
		// In most cases, we don't write to a Gnm::RenderTarget, 
		// we just read it from the game.
		// 

		int32_t init(const Gnm::RenderTargetSpec* spec)
		{
			int32_t error = 0x8EEE00FF;
			do
			{
				if (!spec)
				{
					break;
				}

				memset(this, 0, sizeof(RenderTarget));

				if (GPU().mode() != kGpuModeNeo || spec->m_minGpuMode == kGpuModeNeo)
				{
					if (spec->m_minGpuMode == kGpuModeNeo)
					{
						m_regs[kCbColorDccControl] = 8;
					}
				}
				else
				{
					if (spec->m_flags.enableCmaskFastClear ||
						spec->m_flags.enableFmaskCompression ||
						spec->m_flags.enableDccCompression)
					{
						break;
					}
				}

				uint32_t width  = spec->m_width;
				uint32_t height = spec->m_height;
				if (spec->m_flags.enableCmaskFastClear)
				{
					width  = (width + 7) & 0xFFFFFFF8;
					height = (height + 7) & 0xFFFFFFF8;
				}

				GpuAddress::TilingParameters params   = {};
				params.m_tileMode                     = spec->m_colorTileModeHint;
				params.m_minGpuMode                   = spec->m_minGpuMode;
				params.m_linearWidth                  = width;
				params.m_linearHeight                 = height;
				params.m_linearDepth                  = 1;
				params.m_numFragmentsPerPixel         = 1 << (uint8_t)spec->m_numFragments;
				params.m_baseTiledPitch               = spec->m_pitch;
				params.m_mipLevel                     = 0;
				params.m_arraySlice                   = 0;
				params.m_surfaceFlags.m_texCompatible = (spec->m_minGpuMode == kGpuModeNeo);

				uint32_t bitsPerElement    = spec->m_colorFormat.getTotalBitsPerElement();
				uint32_t texelsPerElement  = spec->m_colorFormat.getTexelsPerElement();
				params.m_bitsPerFragment   = bitsPerElement / texelsPerElement;
				params.m_isBlockCompressed = texelsPerElement > 1;
				params.m_tileSwizzleMask   = 0;

				GpuAddress::SurfaceInfo surfaceInfo = {};
				if (GpuAddress::computeSurfaceInfo(&surfaceInfo, &params) != GpuAddress::kStatusSuccess)
				{
					break;
				}

				m_regs[3] = (m_regs[3] & 0xFF001800) | (((spec->m_numSlices << 13) + 0xFFE000) & 0xFFE000);

				setDataFormat(spec->m_colorFormat);

				m_regs[5] = (m_regs[5] & 0xFFFE0FFF) | ((spec->m_numSamples & 7) << 12) | ((spec->m_numFragments & 3) << 15);

				m_regs[1] = (unsigned int)(m_regs[1] & 0x800FF800) |
							(((unsigned __int16)(surfaceInfo.m_pitch >> 3) + 0x7FF) & 0x7FF) |
							(((surfaceInfo.m_pitch >> 3 << 20) + 0x7FF00000) & 0x7FF00000);

				uint32_t v21 = (surfaceInfo.m_pitch * surfaceInfo.m_height >> 6) + 0x3FFFFF;
				m_regs[2]    = (m_regs[2] & 0xFFC00000) | (v21 & 0x3FFFFF);
				m_regs[15]   = spec->m_width | (spec->m_height << 16);
				m_regs[4]    = (m_regs[4] & 0x7FFFFFFF) | ((spec->m_minGpuMode == 1) << 31);
				m_regs[5]    = (m_regs[5] & 0xFFFFFC00) | (surfaceInfo.m_tileMode & 0x1F) | 32 * (surfaceInfo.m_tileMode & 0x1F);

				if (spec->m_flags.enableCmaskFastClear)
				{
					initCmaskForTarget(spec->m_numSlices);
				}
				if (spec->m_flags.enableFmaskCompression && spec->m_numSamples > 0)
				{
					initFmaskForTarget();
				}

				if (GPU().mode() == kGpuModeNeo &&
					(*(uint8_t*)&spec->m_flags & 0x10))
				{
					*((uint8_t*)m_regs + 0x13) |= 0x10u;
					uint32_t bitsPerElement = spec->m_colorFormat.getBitsPerElement();

					uint32_t v25 = (bitsPerElement != 16) + 1;
					if (bitsPerElement == 8)
						v25 = 0;

					uint32_t v26 = 4 * v25;
					uint32_t v27 = 32 * v25;
					uint32_t v28 = (m_regs[6] & 0xFFFFFF93) | v26;

					if (spec->m_flags.enableColorTextureWithoutDecompress)
						v27 = 0;
					uint32_t v29 = v28 | v27;
					m_regs[6]    = (v29 & 0xFFFFFDFF) | (spec->m_flags.enableColorTextureWithoutDecompress << 7);
				}

			} while (false);
			return error;
		}

		void setDataFormat(DataFormat format)
		{
			// From IDA.
			// 
			// TODO:
			// refator these shit.

			int                      v2;  // er13
			int                      v3;  // er12
			int                      v4;  // er15
			RenderTargetChannelOrder order;  // [rsp+0h] [rbp-40h]
			RenderTargetChannelType  type;  // [rsp+4h] [rbp-3Ch]
			__int64                  v9;  // [rsp+10h] [rbp-30h]

			bool typeConvertable = format.getRenderTargetChannelType(&type);
			
			v2                    = type | kRenderTargetChannelTypeSNorm;
			v3                    = (uint8_t) - (type < 7) & (uint8_t)(0x43u >> type) & 1;
			bool orderConvertable = format.getRenderTargetChannelOrder(&order);
			v4 = 0;
			if (format.m_bits.m_surfaceFormat <= kSurfaceFormatX24_8_32)
			{
				if (format.m_bits.m_surfaceFormat < kSurfaceFormat8_8_8_8)
				{
					if (format.m_bits.m_channelType == kTextureChannelTypeSrgb)
						goto LABEL_10;
				}
				else if (format.m_asInt == 13 || format.m_asInt == 15)
				{
					goto LABEL_10;
				}
				v4 = 0;
				if (typeConvertable)
				{
					v4 = 0;
					if (orderConvertable)
						v4 = 4 * format.m_bits.m_surfaceFormat & 0x7C;
				}
			}
		LABEL_10:
			m_regs[4] =
				(
				 ((type & 7) << 8)		|
				 v4							|
				 (m_regs[4] & 0xFFFA6083)	|
				 ((order & 3) << 11)		|
				 (((uint8_t)v3 & (v2 != 5)) << 15) |
				 ((v2 == 5) << 16) |
				 (v3 << 18)
				) ^ 0x40000;
		}

		SizeAlign getColorSizeAlign(void) const
		{
			// From IDA
			SizeAlign result = {};

			GpuAddress::TilingParameters params = {};
			params.initFromRenderTarget(this, 0);

			GpuAddress::SurfaceInfo info;
			int32_t                 status = GpuAddress::computeSurfaceInfo(&info, &params);
			if (status == GpuAddress::kStatusSuccess)
			{
				result.m_size           = 0;
				result.m_align          = info.m_baseAlign;
				uint32_t lastSliceIndex = getLastArraySliceIndex();
				// check if the result exceed uint32 max
				if (!((info.m_surfaceSize * (lastSliceIndex + 1)) >> 32))
				{
					result.m_size = info.m_surfaceSize * (lastSliceIndex + 1);
				}
			}

			return result;
		}

		void setBaseAddress256ByteBlocks(uint32_t baseAddr256)
		{
			// From IDA
			auto dataFormat = getDataFormat();
			auto tileMode   = getTileMode();

			if (dataFormat.getSurfaceFormat() != kSurfaceFormatInvalid &&
				GpuAddress::isMacroTiled(tileMode))
			{
				auto     mode           = getMinimumGpuMode();
				uint32_t numFragments   = 1 << getNumFragments();
				uint32_t bitsPerElement = dataFormat.getTotalBitsPerElement();
				NumBanks numBanks       = kNumBanks2;
				uint8_t  shift          = 0;
				if (mode != kGpuModeBase)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
					shift = 3;
				}

				baseAddr256 = (baseAddr256 & ~(((1 << (numBanks + 1)) - 1) << shift)) | 
					          (m_regs[0] & (((1 << (numBanks + 1)) - 1) << shift));
			}

			m_regs[kCbColorBase] = baseAddr256;
		}

		void setBaseAddress(void* baseAddr)
		{
			// baseAddr must be 256-byte-aligned!
			return setBaseAddress256ByteBlocks((uint32_t)(uintptr_t(baseAddr) >> 8));
		}

		void setCmaskAddress256ByteBlocks(uint32_t cmaskAddr256)
		{
			SCE_GNM_SET_FIELD(m_regs[kCbColorCmask], CB_COLOR0_CMASK, BASE_256B, cmaskAddr256);
		}

		void setCmaskAddress(void* cmaskAddr)
		{
			// cmaskAddr must be 256-byte-aligned!
			return setCmaskAddress256ByteBlocks((uint32_t)(uintptr_t(cmaskAddr) >> 8));
		}

		void setFmaskAddress256ByteBlocks(uint32_t fmaskAddr256)
		{
			// From IDA
			auto numSamples = getNumSamples();
			auto tileMode   = getFmaskTileMode();

			if (numSamples != kNumSamples1 &&
				GpuAddress::isMacroTiled(tileMode))
			{
				auto     dataFormat     = getDataFormat();
				auto     mode           = getMinimumGpuMode();
				uint32_t numFragments   = 1 << getNumFragments();
				uint32_t bitsPerElement = dataFormat.getTotalBitsPerElement();
				NumBanks numBanks       = kNumBanks2;
				uint8_t  shift          = 0;
				if (mode != kGpuModeBase)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
					shift = 3;
				}

				fmaskAddr256 = (fmaskAddr256 & ~(((1 << (numBanks + 1)) - 1) << shift)) | 
					           (m_regs[9] & (((1 << (numBanks + 1)) - 1) << shift));
			}

			m_regs[kCbColorFmask] = fmaskAddr256;
		}

		void setFmaskAddress(void* fmaskAddr)
		{
			// fmaskAddr must be 256-byte-aligned!
			return setFmaskAddress256ByteBlocks((uint32_t)(uintptr_t(fmaskAddr) >> 8));
		}

		void setAddresses(void* baseAddress, void* cmaskAddress, void* fmaskAddress)
		{
			setBaseAddress(baseAddress);
			setCmaskAddress(cmaskAddress);
			setFmaskAddress(fmaskAddress);
		}

		uint32_t getWidth() const
		{
			return m_regs[kCbWidthHeight] & 0x0000FFFF;
		}

		uint32_t getHeight() const
		{
			return (m_regs[kCbWidthHeight] >> 16) & 0x0000FFFF;
		}

		uint32_t getPitchDiv8Minus1() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kCbColorPitch], CB_COLOR0_PITCH, TILE_MAX);
		}

		uint32_t getPitch() const
		{
			return (getPitchDiv8Minus1() + 1) * 8;
		}

		DataFormat getDataFormat() const
		{
			// From IDA
			return DataFormat::build(
				(RenderTargetFormat)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, FORMAT),
				(RenderTargetChannelType)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, NUMBER_TYPE),
				(RenderTargetChannelOrder)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, COMP_SWAP));
		}

		TileMode getTileMode(void) const
		{
			return (TileMode)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, TILE_MODE_INDEX);
		}

		NumSamples getNumSamples() const
		{
			return (NumSamples)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, NUM_SAMPLES);  // cannot be greater than 4
		}

		NumFragments getNumFragments() const
		{
			return (NumFragments)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, NUM_FRAGMENTS);  // cannot be greater than NUM_SAMPLES
		}

		GpuMode getMinimumGpuMode(void) const
		{
			return (GpuMode)SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, ALT_TILE_MODE);
		}

		uint32_t getBaseArraySliceIndex() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kCbColorView], CB_COLOR0_VIEW, SLICE_START);
		}

		uint32_t getLastArraySliceIndex() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kCbColorView], CB_COLOR0_VIEW, SLICE_MAX);
		}

		bool getUseAltTileMode(void) const
		{
			return getMinimumGpuMode() != kGpuModeBase;  // [vi]
		}

		TileMode getFmaskTileMode(void) const
		{
			return (TileMode)SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, FMASK_TILE_MODE_INDEX);
		}

		uint8_t getFmaskTileSwizzleMask(void) const
		{
			// TODO:
		}

		uint8_t getTileSwizzleMask(void) const
		{
			// From IDA.

			auto mode       = getMinimumGpuMode();
			auto dataFormat = getDataFormat();
			auto tileMode   = getTileMode();

			if (mode == kGpuModeBase ||
				dataFormat.getSurfaceFormat() == kSurfaceFormatInvalid ||
				!GpuAddress::isMacroTiled(tileMode))
			{
				return 0;
			}

			uint32_t numFragments   = 1 << getNumFragments();
			uint32_t bitsPerElement = dataFormat.getTotalBitsPerElement();
			NumBanks numBanks       = kNumBanks2;
			uint8_t  shift          = 0;
			if (mode != kGpuModeBase)
			{
				GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
				shift = 4;
			}
			else
			{
				GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
				shift = 3;
			}
			return (m_regs[0] & (((1 << (numBanks + 1)) - 1) << shift)) >> 4;
		}

		bool getLinearCmask(void) const
		{
			if (GPU().mode() == kGpuModeNeo)
				return SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, CMASK_ADDR_TYPE) == 1;  // [vi]
			else
				return SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, CMASK_IS_LINEAR) == 1;
		}

		bool getDccCompressionEnable() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, DCC_ENABLE) != 0;  // [vi]
		}

		uint32_t getSliceSizeDiv64Minus1() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kCbColorSlice], CB_COLOR0_SLICE, TILE_MAX);
		}

		uint32_t getSliceSizeInBytes() const
		{
			const uint32_t bytesPerElement = getDataFormat().getBytesPerElement();
			return (getSliceSizeDiv64Minus1() + 1) * 64 * 1 * bytesPerElement * (1 << getNumFragments());
		}

		uint32_t getBaseAddress256ByteBlocks() const
		{
			// From IDA.
			uint32_t   base       = SCE_GNM_GET_FIELD(m_regs[kCbColorBase], CB_COLOR0_BASE, BASE_256B);
			DataFormat dataFormat = getDataFormat();
			TileMode   tileMode   = getTileMode();

			if (dataFormat.m_asInt && GpuAddress::isMacroTiled(tileMode))
			{
				auto     mode           = getMinimumGpuMode();
				uint32_t bitsPerElement = dataFormat.getTotalBitsPerElement();
				uint32_t numFragments   = 1 << getNumFragments();
				NumBanks numBanks       = {};
				uint32_t shift          = 0;
				if (mode != kGpuModeBase)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragments);
					shift = 3;
				}
				base &= ~(((1 << (numBanks + 1)) - 1) << shift);
			}

			return base;
		}

		void* getBaseAddress() const
		{
			return (void*)(uintptr_t(getBaseAddress256ByteBlocks()) << 8);
		}

		uint32_t m_regs[16];

	private:
		void initCmaskForTarget(uint32_t numSlice)
		{
			// TODO:
		}

		void initFmaskForTarget()
		{
			// TODO:
		}
	};

}  // namespace sce::Gnm
