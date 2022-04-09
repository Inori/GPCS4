#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmDataFormat.h"
#include "GnmRegInfo.h"
#include "GnmSharpBuffer.h"

namespace sce::Gnm
{

	/**
 * \brief Index Buffer Descriptor
 *
 * Since there's no dedicated Index Buffer Descriptor in Gnm,
 * we create one.
 */
	struct GnmIndexBuffer
	{
		const void* buffer = nullptr;
		VkIndexType type   = VK_INDEX_TYPE_UINT16;
		uint32_t    count  = 0;
		uint32_t    size   = 0;
	};

	class Buffer
	{
	public:
		enum
		{
			kSqBufRsrcWord0 = 0,
			kSqBufRsrcWord1 = 1,
			kSqBufRsrcWord2 = 2,
			kSqBufRsrcWord3 = 3,
			kNumSqBufRsrcRegisters
		};

		const VSharpBuffer& getVsharp() const
		{
			return m_vsharp;
		}

		void initAsDataBuffer(void* baseAddr, DataFormat format, uint32_t numElements)
		{
			// From IDA
			m_regs[3] = 0x20000000;
			m_regs[0] = static_cast<uint32_t>(reinterpret_cast<size_t>(baseAddr));
			m_regs[1] = (reinterpret_cast<uint64_t>(baseAddr) >> 32) & 0x0FFF;

			uint32_t bytesPerElement = format.getBytesPerElement();
			m_regs[1] = (m_regs[1] & 0xC000FFFF) | ((bytesPerElement << 16) & 0x3FFF0000);

			uint32_t v6 = 0;
			if ((*(uint16_t*)&format & 0x800u) <= 0x7FF && 
				(uint32_t) *(uint8_t*)&format - 1 <= 0xD)
			{
				if ((*(uint16_t*)&format & 0xF00) == 0x700)
					v6 = (*(uint8_t*)&format & 0xF) << 15;
				if ((*(uint8_t*)&format & 0xFE) != 6)
					v6 = (*(uint8_t*)&format & 0xF) << 15;
			}
			m_regs[3] = v6 | (m_regs[3] & 0xFFF80000) | (16 * *(uint16_t*)&format & 0x7000) | ((format.m_asInt >> 12) & 0xFFF);
			m_regs[2] = numElements;
		}

		ResourceMemoryType getResourceMemoryType() const
		{
			// From IDA
			return static_cast<ResourceMemoryType>(m_vsharp.mtype_L1s << 5 | m_vsharp.mtype << 2 | m_vsharp.mtype_L2);
		}

		void* getBaseAddress() const
		{
			uintptr_t baseAddr = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, BASE_ADDRESS_HI);
			baseAddr <<= 32;
			baseAddr |= SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord0], SQ_BUF_RSRC_WORD0, BASE_ADDRESS);
			return (void*)baseAddr;
		}

		uint32_t getStride() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, STRIDE);
		}

		uint32_t getSize() const
		{
			uint32_t stride      = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, STRIDE);
			uint32_t numElements = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord2], SQ_BUF_RSRC_WORD2, NUM_RECORDS);
			return stride ? numElements * stride : numElements;  // If stride is 0, NUM_RECORDS contains the buffer size
		}

		uint32_t getNumElements() const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord2], SQ_BUF_RSRC_WORD2, NUM_RECORDS);
		}

		DataFormat getDataFormat() const
		{
			return DataFormat::build(
				(SurfaceFormat)m_vsharp.dfmt,
				(TextureChannelType)m_vsharp.nfmt,
				(TextureChannel)m_vsharp.dst_sel_x,
				(TextureChannel)m_vsharp.dst_sel_y,
				(TextureChannel)m_vsharp.dst_sel_z,
				(TextureChannel)m_vsharp.dst_sel_w);
		}

		bool isBuffer(void) const
		{
			uint32_t bufferType = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord3], SQ_BUF_RSRC_WORD3, TYPE);
			return (bufferType == 0);
		}

		bool isSwizzled(void) const
		{
			return SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, SWIZZLE_ENABLE) == 1;
		}

		BufferSwizzleElementSize getSwizzleElementSize(void) const
		{
			return (BufferSwizzleElementSize)SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord3], SQ_BUF_RSRC_WORD3, ELEMENT_SIZE);
		}

		BufferSwizzleStride getSwizzleStride(void) const
		{
			return (BufferSwizzleStride)SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord3], SQ_BUF_RSRC_WORD3, INDEX_STRIDE);
		}

		union
		{
			uint32_t     m_regs[4];
			VSharpBuffer m_vsharp;
		};
	};

}  // namespace sce::Gnm
