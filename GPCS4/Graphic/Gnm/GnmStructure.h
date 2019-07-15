#pragma once

#include "GPCS4Common.h"


struct CsStageRegisters
{
	uint32_t  computePgmLo;
	uint32_t  computePgmHi;
	uint32_t  computePgmRsrc1;
	uint32_t  computePgmRsrc2;
	uint32_t  computeNumThreadX;
	uint32_t  computeNumThreadY;
	uint32_t  computeNumThreadZ;
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
};

struct DrawModifier
{
	uint32_t m_renderTargetSliceOffset : 3;
	uint32_t m_reserved : 29;
};


// we must keep the size of these structs
// thus if add new field, decrease the reserved count to keep size unchanged.

#pragma pack(push, 1)

struct GnmDrawInitDefaultHardwareState
{
	uint32_t opcode;
	uint32_t reserved[255];
};

struct GnmDispatchInitDefaultHardwareState
{
	uint32_t opcode;
	uint32_t reserved[255];
};

struct GnmInitToDefaultContextState
{
	uint32_t reserved[256];
};

struct GnmVSShader
{
	uint32_t opcode;
	VsStageRegisters vsRegs;
	uint32_t modifier;
	uint32_t reserved[20];
};

struct GnmPSShader
{
	uint32_t opcode;
	PsStageRegisters psRegs;
	uint32_t reserved[27];
};

struct GnmCSShader
{
	uint32_t opcode;
	CsStageRegisters csRegs;
	uint32_t modifier;
	uint32_t reserved[16];
};

struct GnmESShader
{
	uint32_t reserved[20];
};

struct GnmGSShader
{
	uint32_t reserved[29];
};

struct GnmHSShader
{
	uint32_t reserved[30];
};

struct GnmLSShader
{
	uint32_t reserved[23];
};

struct GnmVgtControl
{
	uint32_t opcode;
	uint32_t primGroupSizeMinusOne;
	uint16_t partialVsWaveMode;
	uint16_t wdSwitchOnlyOnEopMode;
};

// not fixed size
struct GnmPushMarker
{
	uint32_t opcode;
	char debugString[1];
};

// not fixed size
struct GnmPushColorMarker
{
	uint32_t* reserved;
};

struct GnmPopMarker
{
	uint32_t opcode;
	uint32_t reserved[5];
};

struct GnmDrawIndex
{
	uint32_t opcode;
	uint32_t indexCount;
	ulong_ptr indexAddr;
	uint32_t pred;
	uint32_t inlineMode;
	uint32_t reserved[4];
};

struct GnmDrawIndexAuto
{
	uint32_t reserved[7];
};

struct GnmDrawIndexOffset
{
	uint32_t reserved[9];
};

struct GnmDrawIndexIndirect
{
	uint32_t reserved[9];
};

struct GnmDrawIndexIndirectCountMulti
{
	uint32_t reserved[16];
};

struct GnmDrawIndexMultiInstanced
{
	uint32_t reserved[13];
};

struct GnmDrawOpaqueAuto
{
	uint32_t reserved[7];
};

struct GnmDispatchDirect
{
	uint32_t opcode;
	uint32_t threadGroupX;
	uint32_t threadGroupY;
	uint32_t threadGroupZ;
	uint32_t pred;
	uint32_t reserved[4];
};

struct GnmDispatchIndirect
{
	uint32_t reserved[7];
};

struct GnmWaitFlipDone
{
	uint32_t opcode;
	uint32_t videoOutHandle;
	uint32_t displayBufferIndex;
	uint32_t reserved[4];
};



#pragma pack(pop)
