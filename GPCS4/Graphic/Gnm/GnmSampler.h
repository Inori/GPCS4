#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmRegInfo.h"
#include "GnmDataFormat.h"
#include "GnmSharpBuffer.h"

class GnmSampler
{
public:
	enum
	{
		kSqImgSampWord0 = 0,
		kSqImgSampWord1 = 1,
		kSqImgSampWord2 = 2,
		kSqImgSampWord3 = 3,
		kNumSqImgSampRegisters
	};

	const SSharpBuffer& getSsharp() const
	{
		return m_ssharp;
	}

	WrapMode getWrapModeX() const
	{
		return (WrapMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, CLAMP_X);
	}

	WrapMode getWrapModeY() const
	{
		return (WrapMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, CLAMP_Y);
	}

	WrapMode getWrapModeZ() const
	{
		return (WrapMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, CLAMP_Z);
	}

	FilterMode getMagFilterMode() const
	{
		return (FilterMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord2], SQ_IMG_SAMP_WORD2, XY_MAG_FILTER);
	}

	FilterMode getMinFilterMode() const
	{
		return (FilterMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord2], SQ_IMG_SAMP_WORD2, XY_MIN_FILTER);
	}

	ZFilterMode getZFilterMode() const
	{
		return (ZFilterMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord2], SQ_IMG_SAMP_WORD2, Z_FILTER);
	}

	MipFilterMode getMipFilterMode() const
	{
		return (MipFilterMode)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord2], SQ_IMG_SAMP_WORD2, MIP_FILTER);
	}

	FilterReductionMode getFilterReductionMode() const
	{
		return (FilterReductionMode)SCE_GNM_GET_FIELD(m_regs[0], SQ_IMG_SAMP_WORD0, FILTER_MODE);
	}

	uint32_t getMinLod() const
	{
		return (uint32_t)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord1], SQ_IMG_SAMP_WORD1, MIN_LOD);
	}

	uint32_t getMaxLod() const
	{
		return (uint32_t)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord1], SQ_IMG_SAMP_WORD1, MAX_LOD);
	}

	uint16_t getLodBias() const
	{
		return (uint16_t)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord2], SQ_IMG_SAMP_WORD2, LOD_BIAS);
	}

	uint16_t getLodBiasSecondary() const
	{
		return (uint16_t)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord2], SQ_IMG_SAMP_WORD2, LOD_BIAS_SEC);
	}

	AnisotropyRatio getAnisotropyRatio() const
	{
		return (AnisotropyRatio)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, MAX_ANISO_RATIO);
	}

	DepthCompare getDepthCompareFunction() const
	{
		return (DepthCompare)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, DEPTH_COMPARE_FUNC);
	}

	bool getForceDegamma() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, FORCE_DEGAMMA) ? true : false;
	}

	bool getForceUnnormalized(void) const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, FORCE_UNNORMALIZED) ? true : false;
	}

	uint32_t getAnisotropyThreshold() const
	{
		return (uint32_t)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord0], SQ_IMG_SAMP_WORD0, ANISO_THRESHOLD);
	}

	BorderColor getBorderColor() const
	{
		return (BorderColor)SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord3], SQ_IMG_SAMP_WORD3, BORDER_COLOR_TYPE);
	}

	uint32_t getBorderColorTableIndex() const
	{
		return SCE_GNM_GET_FIELD(m_regs[kSqImgSampWord3], SQ_IMG_SAMP_WORD3, BORDER_COLOR_PTR);
	}

	union
	{
		uint32_t m_regs[4];
		SSharpBuffer m_ssharp;
	};
	

};
