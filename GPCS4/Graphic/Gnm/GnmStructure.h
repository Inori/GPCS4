#pragma once

#include "GPCS4Common.h"

// we must keep the size of these structs
// thus if add new field, decrease the reserved count to keep size unchanged.

#pragma pack(push, 4)

struct GnmInitDefaultHardwareState
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
	uint32_t reserved[29];
};

struct GnmPSShader
{
	uint32_t reserved[40];
};

struct GnmCSShader
{
	uint32_t reserved[25];
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
	uint32_t reserved[3];
};

// not fixed size
struct GnmPushMarker
{
	uint32_t* reserved;
};

// not fixed size
struct GnmPushColorMarker
{
	uint32_t* reserved;
};

struct GnmPopMarker
{
	uint32_t reserved[6];
};

struct GnmDrawIndex
{
	uint32_t reserved[10];
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
	uint32_t reserved[9];
};

struct GnmDispatchIndirect
{
	uint32_t reserved[7];
};

struct GnmWaitFlipDone
{
	uint32_t opcode;
	uint32_t reserved[6];
};

#pragma pack(pop)


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

