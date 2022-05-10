#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"
#include "GnmRegInfo.h"
#include "GnmSharpBuffer.h"
#include "GnmStructure.h"
#include "GnmRenderTarget.h"
#include "GpuAddress/GnmGpuAddress.h"

namespace sce::Gnm
{

	class Texture
	{
	public:
		enum
		{
			kSqImgRsrcWord0 = 0,
			kSqImgRsrcWord1 = 1,
			kSqImgRsrcWord2 = 2,
			kSqImgRsrcWord3 = 3,
			kSqImgRsrcWord4 = 4,
			kSqImgRsrcWord5 = 5,
			kSqImgRsrcWord6 = 6,
			kSqImgRsrcWord7 = 7,
			kNumSqImgRsrcRegisters
		};

		void initFromRenderTarget(const RenderTarget* rt, bool isCubemap)
		{
			// From IDA:

			const sce::Gnm::RenderTarget* a2 = rt;

			const sce::Gnm::RenderTarget* v6;   // r14
			int                           v7;   // eax
			uint32_t                      v8;   // er15
			int                           v9;   // ebx
			int                           v10;  // er15
			int                           v11;  // ebx
			uint32_t                      v12;  // eax
			unsigned int                  v13;  // ecx
			int                           v14;  // esi
			uint32_t                      v15;  // edx
			int                           v16;  // ebx
			uint32_t                      v17;  // edi
			unsigned int                  v19;  // ecx
			unsigned int                  v20;  // edx
			bool                          v21;  // zf
			signed int                    v22;  // eax
			int                           v23;  // eax
			unsigned int                  v24;  // edx
			unsigned int                  v25;  // ebx
			int                           v26;  // eax
			uint8_t                       v27;  // al
			char                          v29;  // [rsp+4h] [rbp-3Ch]
			uint32_t                      v30;  // [rsp+8h] [rbp-38h]

			std::memset(this, 0, sizeof(Texture));
			DataFormat dataFormat = rt->getDataFormat();

			v29             = isCubemap;
			v6              = rt;
			this->m_regs[3] = 0x4000000;
			this->m_regs[1] = 0;
			v7              = (*((uint32_t*)a2 + 15) + 0x3FFF) & 0x3FFF;
			this->m_regs[2] = v7;
			this->m_regs[2] = v7 | ((((*((uint32_t*)a2 + 15) >> 2) & 0xFFFC000) + 268419072) & 0xFFFC000);
			this->m_regs[3] = 0x94000000;
			this->m_regs[4] = ((*((uint32_t*)a2 + 1) << 16) & 0x7FF0000) | 0xE000;
			this->m_regs[3] = 0x94000000;
			this->m_regs[1] = 0;
			v30             = dataFormat.m_asInt;
			this->m_regs[1] = (this->m_regs[1] & 0xC00FFFFF) | ((v30 << 18) & 0x3C000000) | ((v30 & 0x3F) << 20);
			v8              = this->m_regs[3] & 0xFFFFF000;
			v9              = v8 | (dataFormat.getChannel(0) & 7);
			v10             = v9 + 8 * (dataFormat.getChannel(1) & 7);
			v11             = v10 | ((dataFormat.getChannel(2) & 7) << 6);
			v12             = v11 | ((dataFormat.getChannel(3) & 7) << 9);
			this->m_regs[3] = v12;
			v13             = (v12 & 0xFE0FFFFF) | ((*((uint32_t*)a2 + 5) & 0x1F) << 20);
			this->m_regs[3] = v13;
			v14             = (*((uint32_t*)a2 + 3) >> 13) & 0x7FF;
			v15             = this->m_regs[4] & 0xFFFFE000;
			this->m_regs[4] = v14 | (this->m_regs[4] & 0xFFFFE000);
			v16             = *((uint32_t*)v6 + 3);
			this->m_regs[5] = (this->m_regs[5] & 0xFC000000) | (*((uint32_t*)v6 + 3) & 0xFFE7FF);
			v17             = this->m_regs[2];
			this->m_regs[2] |= 0x70000000u;
			if (v16 & 0xFFE000 && v29 == 1)
			{
				v19             = (v13 & 0xFFFFFFF) | 0xB0000000;
				this->m_regs[4] = v15 | (((unsigned __int16)((v14 + 1) / 6u) + 0x1FFF) & 0x1FFF);
				goto LABEL_15;
			}
			if (!(*((uint32_t*)v6 + 5) & 0x18000))
			{
				if (v17 & 0xFFFC000)
				{
					v20 = 0xD0000000;
					if (v16 & 0x7FF)
						goto LABEL_14;
					v21 = (v16 & 0xFFE000) == 0;
					v22 = 0x90000000;
					v20 = 0xD0000000;
				}
				else
				{
					v20 = 0xC0000000;
					if (v16 & 0x7FF)
						goto LABEL_14;
					v21 = (v16 & 0xFFE000) == 0;
					v22 = 0x80000000;
					v20 = 0xC0000000;
				}
			LABEL_12:
				if (v21)
					v20 = v22;
				goto LABEL_14;
			}
			v20 = 0xF0000000;
			if (!(v16 & 0x7FF))
			{
				v21 = (v16 & 0xFFE000) == 0;
				v22 = -536870912;
				v20 = 0xF0000000;
				goto LABEL_12;
			}
		LABEL_14:
			v19 = v20 | (v13 & 0xFFFFFFF);
		LABEL_15:
			this->m_regs[3] = v19;
			if ((*((uint32_t*)v6 + 5) >> 15) & 3)
				this->m_regs[3] = (v19 & 0xFFF0FFFF) | (((*((uint32_t*)v6 + 5) >> 15) & 3) << 16);
			if (*((uint8_t*)v6 + 19) & 0x10 && (*((uint32_t*)v6 + 6) & 0x260) == 512)
			{
				v23             = this->m_regs[6] | 0x200000;
				this->m_regs[6] = v23;
				v24             = (v23 & 0xFFBFFFFF) | (((((*((uint32_t*)v6 + 4) >> 11) & 2) | 1) != 3) << 22);
				this->m_regs[6] = v24;
				this->m_regs[6] = (v24 & 0xFF7FFFFF) | ((*((uint32_t*)v6 + 6) << 16) & 0x800000);
				this->m_regs[7] = *((uint32_t*)v6 + 13);
			}
			v25             = this->m_regs[6] & 0xFEFFFFFF;
			this->m_regs[6] = v25 | (((unsigned int)v6->getMinimumGpuMode() == 1) << 24);
			v26             = v6->getBaseAddress256ByteBlocks();
			this->setBaseAddress256ByteBlocks(v26);
			v27 = v6->getTileSwizzleMask();
			this->setTileSwizzleMask(v27);
		}

		const TSharpBuffer& getTsharp() const
		{
			return m_tsharp;
		}

		SizeAlign getSizeAlign(void) const
		{
			uint64_t      size   = 0;
			AlignmentType align  = 0;
			auto          status = GpuAddress::computeTotalTiledTextureSize(&size, &align, this);

			SizeAlign result = { 0 };
			if (status == GpuAddress::kStatusSuccess)
			{
				result.m_size  = size;
				result.m_align = align;
			}
			return result;
		}

		SizeAlign getMetadataSizeAlign(void) const
		{
			// TODO
		}

		uint8_t getMipStatsCounterIndex() const
		{
			return (uint8_t)(SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, COUNTER_BANK_ID));
		}

		bool isMipStatsEnabled() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, LOD_HDW_CNT_EN) ? true : false;
		}

		TextureType getTextureType() const
		{
			return (TextureType)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, TYPE);
		}

		// From IDA
		uint32_t getBaseAddress256ByteBlocks() const
		{
			uint32_t baseAddr256 = 0;
			TileMode tileMode;
			uint32_t numFragPerPixel = 0;
			NumBanks numBanks;
			uint32_t bitsPerElement = 0;

			DataFormat format = getDataFormat();

			bool isMacroTile = GpuAddress::isMacroTiled((TileMode)m_tsharp.tiling_idx);

			baseAddr256 = m_regs[0];

			if (isMacroTile)
			{
				bitsPerElement  = format.getTotalBitsPerElement();
				tileMode        = getTileMode();
				numFragPerPixel = 1 << SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL);

				if (m_regs[3] <= 0xDFFFFFFF)
				{
					numFragPerPixel = 1;
				}

				uint8_t shift = 0;
				if (m_regs[6] & 0x1000000)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 3;
				}
				baseAddr256 &= ~(((1 << (numBanks + 1)) - 1) << shift);
			}

			return baseAddr256;
		}

		void setBaseAddress256ByteBlocks(uint32_t baseAddr256)
		{
			TileMode tileMode;
			uint32_t numFragPerPixel = 0;
			NumBanks numBanks;
			uint32_t bitsPerElement = 0;

			DataFormat format = getDataFormat();

			bool isMacroTile = GpuAddress::isMacroTiled((TileMode)m_tsharp.tiling_idx);

			if (isMacroTile)
			{
				bitsPerElement  = format.getTotalBitsPerElement();
				tileMode        = getTileMode();
				numFragPerPixel = 1 << SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL);

				if (m_regs[3] <= 0xDFFFFFFF)
				{
					numFragPerPixel = 1;
				}

				uint8_t shift = 0;
				if (m_regs[6] & 0x1000000)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 3;
				}

				baseAddr256 = (baseAddr256 & ~(((1 << (numBanks + 1)) - 1) << shift)) | (m_regs[0] & (((1 << (numBanks + 1)) - 1) << shift));
			}

			m_regs[0] = baseAddr256;
			m_regs[1] = m_regs[1] & 0xFFFFFFC0;
		}

		void* getBaseAddress() const
		{
			return (void*)(uintptr_t(getBaseAddress256ByteBlocks()) << 8);
		}

		SamplerModulationFactor getSamplerModulationFactor(void) const
		{
			return (SamplerModulationFactor)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord2], SQ_IMG_RSRC_WORD2, PERF_MOD);
		}

		ResourceMemoryType getResourceMemoryType() const
		{
		}

		TileMode getTileMode(void) const
		{
			return (TileMode)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, TILING_INDEX);
		}

		bool getUseAltTileMode(void) const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, ALT_TILE_MODE) != 0;  // [vi]
		}

		uint32_t getMinLodClamp() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord1], SQ_IMG_RSRC_WORD1, MIN_LOD);
		}

		uint32_t getPitchMinus1() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord4], SQ_IMG_RSRC_WORD4, PITCH);
		}

		uint32_t getWidthMinus1() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord2], SQ_IMG_RSRC_WORD2, WIDTH);
		}

		uint32_t getHeightMinus1() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord2], SQ_IMG_RSRC_WORD2, HEIGHT);
		}

		uint32_t getDepthMinus1() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord4], SQ_IMG_RSRC_WORD4, DEPTH);
		}

		uint32_t getTotalArraySliceCount() const
		{
			return (getTextureType() == kTextureType3d) ? 1 : getDepth();
		}

		SurfaceFormat getSurfaceFormat() const
		{
			return (SurfaceFormat)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord1], SQ_IMG_RSRC_WORD1, DATA_FORMAT);
		}

		TextureChannelType getTextureChannelType() const
		{
			return (TextureChannelType)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord1], SQ_IMG_RSRC_WORD1, NUM_FORMAT);
		}

		DataFormat getDataFormat() const
		{
			return DataFormat::build((SurfaceFormat)m_tsharp.dfmt, (TextureChannelType)m_tsharp.nfmt,
									 (TextureChannel)m_tsharp.dst_sel_x,
									 (TextureChannel)m_tsharp.dst_sel_y,
									 (TextureChannel)m_tsharp.dst_sel_z,
									 (TextureChannel)m_tsharp.dst_sel_w);
		}

		uint32_t getBaseArraySliceIndex() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord5], SQ_IMG_RSRC_WORD5, BASE_ARRAY);
		}

		uint32_t getLastArraySliceIndex() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord5], SQ_IMG_RSRC_WORD5, LAST_ARRAY);  // For cubemaps, must be BASE_ARRAY + (numCubemaps*6)-1
		}

		uint32_t getBaseMipLevel() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, BASE_LEVEL);
		}

		uint32_t getLastMipLevel() const
		{
			return (getTextureType() == kTextureType2dMsaa ||
					getTextureType() == kTextureType2dArrayMsaa)
					   ? 0
					   : SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL);
		}

		bool isPaddedToPow2() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, POW2_PAD) != 0;
		}

		NumFragments getNumFragments() const
		{
			return (getTextureType() == kTextureType2dMsaa || getTextureType() == kTextureType2dArrayMsaa) ? (NumFragments)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL) : kNumFragments1;
		}

		uint32_t getWidth() const
		{
			return getWidthMinus1() + 1;
		}

		uint32_t getHeight() const
		{
			return getHeightMinus1() + 1;
		}

		uint32_t getDepth() const
		{
			return getDepthMinus1() + 1;
		}

		uint32_t getPitch() const
		{
			return (getPitchMinus1() + 1);
		}

		bool isTexture(void) const
		{
			uint32_t textureType = SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, TYPE);
			return ((textureType & 0x8) != 0);  // values of 8-15 represent valid texture types. Anything else is invalid!
		}

		uint8_t getTileSwizzleMask(void) const
		{
			uint8_t  mask = 0;
			
			uint32_t numFragPerPixel = 0;
			NumBanks numBanks;
			uint32_t bitsPerElement = 0;

			TileMode   tileMode = getTileMode();
			DataFormat format   = getDataFormat();

			bool isMacroTile = GpuAddress::isMacroTiled(tileMode);
			bool isAltTile   = getUseAltTileMode();

			if (isAltTile && isMacroTile && format.m_asInt)
			{
				bitsPerElement  = format.getTotalBitsPerElement();
				numFragPerPixel = 1 << SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL);

				if (m_regs[3] <= 0xDFFFFFFF)
				{
					numFragPerPixel = 1;
				}

				uint8_t shift = 0;
				if (m_regs[6] & 0x1000000)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 3;
				}
				mask = (m_regs[0] & (((1 << (numBanks + 1)) - 1) << shift)) >> 4;
			}
			else
			{
				mask = 0;
			}

			return mask;
		}

		void setTileSwizzleMask(uint8_t bankSwizzleMask)
		{
			uint32_t numFragPerPixel = 0;
			NumBanks numBanks;
			uint32_t bitsPerElement = 0;

			TileMode   tileMode = getTileMode();
			DataFormat format   = getDataFormat();

			bool isMacroTile = GpuAddress::isMacroTiled(tileMode);
			bool isAltTile   = getUseAltTileMode();

			if (isAltTile && isMacroTile && format.m_asInt)
			{
				bitsPerElement  = format.getTotalBitsPerElement();
				numFragPerPixel = 1 << SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord3], SQ_IMG_RSRC_WORD3, LAST_LEVEL);

				if (m_regs[3] <= 0xDFFFFFFF)
				{
					numFragPerPixel = 1;
				}

				uint8_t shift = 0;
				if (m_regs[6] & 0x1000000)
				{
					GpuAddress::getAltNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 4;
				}
				else
				{
					GpuAddress::getNumBanks(&numBanks, tileMode, bitsPerElement, numFragPerPixel);
					shift = 3;
				}

				m_regs[0] = (m_regs[0] & ~(((1 << (numBanks + 1)) - 1) << shift)) | ((((1 << (numBanks + 1)) - 1) << shift) & 16 * bankSwizzleMask);
			}
		}

		uint32_t getMetadataAddress256ByteBlocks() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord7], SQ_IMG_RSRC_WORD7, META_DATA_ADDRESS);  // [vi]
		}

		void* getMetadataAddress(void) const
		{
			return (void*)(uintptr_t(getMetadataAddress256ByteBlocks()) << 8);
		}

		bool getMetadataCompressionEnable(void) const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, COMPRESSION_EN) != 0;  // [vi]
		}

		TextureMetadataType getTextureMetadataType(void) const
		{
		}

		bool getDccAlphaOnMsb(void) const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, ALPHA_IS_ON_MSB) != 0;  // [vi]
		}

		DccColorTransform getDccColorTransform(void) const
		{
			return (DccColorTransform)SCE_GNM_GET_FIELD(m_regs[kSqImgRsrcWord6], SQ_IMG_RSRC_WORD6, COLOR_TRANSFORM);  // [vi]
		}

		GpuMode getMinimumGpuMode(void) const
		{
			// IDA
			return static_cast<GpuMode>(*((uint8_t*)this + 27) & 1);
		}

		union
		{
			uint32_t     m_regs[8];
			TSharpBuffer m_tsharp;
		};
	};

}  // namespace sce::Gnm
