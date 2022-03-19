#pragma once

#include "PsslCommon.h"
#include "PsslShaderRegField.h"

namespace pssl
{;


struct CsStageRegisters
{
	uint32_t  computePgmLo;
	uint32_t  computePgmHi;
	uint32_t  computePgmRsrc1;
	uint32_t  computePgmRsrc2;
	uint32_t  computeNumThreadX;
	uint32_t  computeNumThreadY;
	uint32_t  computeNumThreadZ;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(computePgmHi) << 40 | uintptr_t(computePgmLo) << 8);
	}
};


struct VsStageRegisters
{
	uint32_t spiShaderPgmLoVs;
	uint32_t spiShaderPgmHiVs;
	uint32_t spiShaderPgmRsrc1Vs;
	uint32_t spiShaderPgmRsrc2Vs;
	uint32_t spiVsOutConfig;
	uint32_t spiShaderPosFormat;
	uint32_t paClVsOutCntl;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(spiShaderPgmHiVs) << 40 | uintptr_t(spiShaderPgmLoVs) << 8);
	}
};


struct PsStageRegisters
{
	uint32_t spiShaderPgmLoPs;
	uint32_t spiShaderPgmHiPs;
	uint32_t spiShaderPgmRsrc1Ps;
	uint32_t spiShaderPgmRsrc2Ps;
	uint32_t spiShaderZFormat;
	uint32_t spiShaderColFormat;
	uint32_t spiPsInputEna;
	uint32_t spiPsInputAddr;
	uint32_t spiPsInControl;
	uint32_t spiBarycCntl;
	uint32_t dbShaderControl;
	uint32_t cbShaderMask;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(spiShaderPgmHiPs) << 40 | uintptr_t(spiShaderPgmLoPs) << 8);
	}
};



}  // pssl