#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmRegInfo.h"
#include "GnmDataFormat.h"
#include "GnmSharpBuffer.h"

class GnmBuffer
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

	ResourceMemoryType getResourceMemoryType() const
	{
		// TODO
	}

	void *getBaseAddress() const
	{
		uintptr_t baseAddr = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, BASE_ADDRESS_HI);
		baseAddr <<= 32;
		baseAddr |= SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord0], SQ_BUF_RSRC_WORD0, BASE_ADDRESS);
		return (void *)baseAddr;
	}

	uint32_t getStride() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, STRIDE);
	}

	uint32_t getSize() const
	{
		uint32_t stride = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord1], SQ_BUF_RSRC_WORD1, STRIDE);
		uint32_t numElements = SCE_GNM_GET_FIELD(m_regs[kSqBufRsrcWord2], SQ_BUF_RSRC_WORD2, NUM_RECORDS);
		return stride ? numElements * stride : numElements; // If stride is 0, NUM_RECORDS contains the buffer size
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
		uint32_t m_regs[4];
		VSharpBuffer m_vsharp;
	};
	
};