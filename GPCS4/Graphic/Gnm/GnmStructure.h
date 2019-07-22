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

struct GnmCmdDrawInitDefaultHardwareState
{
	uint32_t opcode;
	uint32_t reserved[255];
};

struct GnmCmdDispatchInitDefaultHardwareState
{
	uint32_t opcode;
	uint32_t reserved[255];
};

struct GnmCmdInitToDefaultContextState
{
	uint32_t reserved[256];
};

struct GnmCmdVSShader
{
	uint32_t opcode;
	VsStageRegisters vsRegs;
	uint32_t modifier;
	uint32_t reserved[20];
};

struct GnmCmdPSShader
{
	uint32_t opcode;
	PsStageRegisters psRegs;
	uint32_t reserved[27];
};

struct GnmCmdCSShader
{
	uint32_t opcode;
	CsStageRegisters csRegs;
	uint32_t modifier;
	uint32_t reserved[16];
};

struct GnmCmdESShader
{
	uint32_t reserved[20];
};

struct GnmCmdGSShader
{
	uint32_t reserved[29];
};

struct GnmCmdHSShader
{
	uint32_t reserved[30];
};

struct GnmCmdLSShader
{
	uint32_t reserved[23];
};

struct GnmCmdVgtControl
{
	uint32_t opcode;
	uint32_t primGroupSizeMinusOne;
	uint16_t partialVsWaveMode;
	uint16_t wdSwitchOnlyOnEopMode;
};

// not fixed size
struct GnmCmdPushMarker
{
	uint32_t opcode;
	char debugString[1];
};

// not fixed size
struct GnmCmdPushColorMarker
{
	uint32_t* reserved;
};

struct GnmCmdPopMarker
{
	uint32_t opcode;
	uint32_t reserved[5];
};

struct GnmCmdDrawIndex
{
	uint32_t opcode;
	uint32_t indexCount;
	ulong_ptr indexAddr;
	uint32_t pred;
	uint32_t inlineMode;
	uint32_t reserved[4];
};

struct GnmCmdDrawIndexAuto
{
	uint32_t reserved[7];
};

struct GnmCmdDrawIndexOffset
{
	uint32_t reserved[9];
};

struct GnmCmdDrawIndexIndirect
{
	uint32_t reserved[9];
};

struct GnmCmdDrawIndexIndirectCountMulti
{
	uint32_t reserved[16];
};

struct GnmCmdDrawIndexMultiInstanced
{
	uint32_t reserved[13];
};

struct GnmCmdDrawOpaqueAuto
{
	uint32_t reserved[7];
};

struct GnmCmdDispatchDirect
{
	uint32_t opcode;
	uint32_t threadGroupX;
	uint32_t threadGroupY;
	uint32_t threadGroupZ;
	uint32_t pred;
	uint32_t reserved[4];
};

struct GnmCmdDispatchIndirect
{
	uint32_t reserved[7];
};

struct GnmCmdWaitFlipDone
{
	uint32_t opcode;
	uint32_t videoOutHandle;
	uint32_t displayBufferIndex;
	uint32_t reserved[4];
};



#pragma pack(pop)
