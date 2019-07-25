#pragma once

#include "GPCS4Common.h"
#include "../Pssl/ShaderRegister.h"

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
