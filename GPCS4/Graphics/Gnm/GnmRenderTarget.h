#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"
#include "GnmRegInfo.h"
#include "GnmStructure.h"
#include "GpuAddress/GnmGpuAddress.h"
#include "Emulator/Emulator.h"
#include "VirtualGPU.h"

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

	class RenderTarget
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

		int32_t init(const Gnm::RenderTargetSpec* spec)
		{
			// TODO:
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
			// TODO:
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
			return SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, ALT_TILE_MODE) != 0;  // [vi]
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
			// TODO:
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
				uint8_t  alt            = SCE_GNM_GET_FIELD(m_regs[kCbColorInfo], CB_COLOR0_INFO, ALT_TILE_MODE);
				uint32_t bitsPerElement = dataFormat.getTotalBitsPerElement();
				uint32_t numFragments   = 1 << SCE_GNM_GET_FIELD(m_regs[kCbColorAttrib], CB_COLOR0_ATTRIB, NUM_FRAGMENTS);
				NumBanks numBanks       = {};
				uint32_t shift          = 0;
				if (alt)
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
	};

}  // namespace sce::Gnm
