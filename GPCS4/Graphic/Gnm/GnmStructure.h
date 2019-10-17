#pragma once

#include "GPCS4Common.h"
#include "GnmRegInfo.h"
#include "../Pssl/PsslShaderRegister.h"

struct DrawModifier
{
	uint32_t renderTargetSliceOffset : 3;
	uint32_t reserved : 29;
};

//////////////////////////////////////////////////////////////////////////

// Self defined structures.
// Used to record gnm driver calls' parameters.

// Note:
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
	pssl::VsStageRegisters vsRegs;
	EmbeddedVsShader shaderId;
	uint32_t modifier;
	uint32_t reserved[19];
};

struct GnmCmdPSShader
{
	uint32_t opcode;
	pssl::PsStageRegisters psRegs;
	uint32_t reserved[27];
};

struct GnmCmdCSShader
{
	uint32_t opcode;
	pssl::CsStageRegisters csRegs;
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


enum GnmEnumDrawIndexInlineMode : uint32_t
{
	INLINE_MODE_NOINLINE = 1,
	INLINE_MODE_INLINE = 2,
};

struct GnmCmdDrawIndex
{
	uint32_t opcode;
	uint32_t indexCount;
	uintptr_t indexAddr;
	uint32_t predAndMod;
	GnmEnumDrawIndexInlineMode inlineMode;
	uint32_t reserved[4];
};

struct GnmCmdDrawIndexAuto
{
	uint32_t opcode;
	uint32_t indexCount;
	uint32_t predAndMod;
	uint32_t reserved[4];
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

//////////////////////////////////////////////////////////////////////////

// Encoded regs' definition


union ViewportTransformControl
{
	struct
	{
		uint16_t scaleX		: 1;
		uint16_t offsetX	: 1;
		uint16_t scaleY		: 1;
		uint16_t offsetY	: 1;
		uint16_t scaleZ		: 1;
		uint16_t offsetZ	: 1;

		uint16_t passThrough : 2;

		uint16_t perspectiveDivideXY	: 1;  // 0 for enable
		uint16_t perspectiveDivideZ		: 1;  // 0 for enable
		uint16_t invertW	: 1;

		uint16_t reserved0	: 1;
		uint16_t reserved1	: 1;
		uint16_t reserved2	: 1;
		uint16_t reserved3	: 1;
		uint16_t reserved4	: 1;

		uint16_t reserved5;
	};

	uint32_t reg;
};


//////////////////////////////////////////////////////////////////////////

#define SCE_GNM_GET_FIELD(src, registername, field) \
	(((src) & (GNM_ ## registername ## __ ## field ## __MASK)) >> (GNM_ ## registername ## __ ## field ## __SHIFT))

// RenderTarget
struct RenderTarget
{
	enum
	{
		kCbColorBase = mmCB_COLOR0_BASE - mmCB_COLOR0_BASE,
		kCbColorPitch = mmCB_COLOR0_PITCH - mmCB_COLOR0_BASE,
		kCbColorSlice = mmCB_COLOR0_SLICE - mmCB_COLOR0_BASE,
		kCbColorView = mmCB_COLOR0_VIEW - mmCB_COLOR0_BASE,
		kCbColorInfo = mmCB_COLOR0_INFO - mmCB_COLOR0_BASE,
		kCbColorAttrib = mmCB_COLOR0_ATTRIB - mmCB_COLOR0_BASE,
		kCbColorDccControl = mmCB_COLOR0_DCC_CONTROL - mmCB_COLOR0_BASE,
		kCbColorCmask = mmCB_COLOR0_CMASK - mmCB_COLOR0_BASE,
		kCbColorCmaskSlice = mmCB_COLOR0_CMASK_SLICE - mmCB_COLOR0_BASE,
		kCbColorFmask = mmCB_COLOR0_FMASK - mmCB_COLOR0_BASE,
		kCbColorFmaskSlice = mmCB_COLOR0_FMASK_SLICE - mmCB_COLOR0_BASE,
		kCbColorClearWord0 = mmCB_COLOR0_CLEAR_WORD0 - mmCB_COLOR0_BASE,
		kCbColorClearWord1 = mmCB_COLOR0_CLEAR_WORD1 - mmCB_COLOR0_BASE,
		kCbColorDccBase = mmCB_COLOR0_DCC_BASE - mmCB_COLOR0_BASE,
		// 14: unused
		kCbWidthHeight = 15, // not a GPU register. width in [15:0], height in [31:16].
		kNumCbRegisters
	};

	uint32_t regColorBase;
	uint32_t regColorPitch;
	uint32_t regColorSlice;
	uint32_t regColorView;
	uint32_t regColorInfo;
	uint32_t regColorAttrib;
	uint32_t regColorDccControl;
	uint32_t regColorCmask;
	uint32_t regColorCmaskSlice;
	uint32_t regColorFmask;
	uint32_t regColorFmaskSlice;
	uint32_t regColorClearWord0;
	uint32_t regColorClearWord1;
	uint32_t regColorDccBase;
	uint32_t unused;
	uint16_t regWidth;
	uint16_t regHeight;
};


// DepthRenderTarget
struct DepthRenderTarget
{
	enum
	{
		kDbZInfo = 0,
		kDbStencilInfo = 1,
		kDbZReadBase = 2,
		kDbStencilReadBase = 3,
		kDbZWriteBase = 4,
		kDbStencilWriteBase = 5,
		kDbDepthSize = 6,
		kDbDepthSlice = 7,
		kDbDepthView = 8,
		kDbHtileDataBase = 9,
		kDbHtileSurface = 10,
		kDbDepthInfo = 11,
		kNumDbRegisters
	};

	uint32_t regDbZInfo;
	uint32_t regDbStencilInfo;
	uint32_t regDbZReadBase;
	uint32_t regDbStencilReadBase;
	uint32_t regDbZWriteBase;
	uint32_t regDbStencilWriteBase;
	uint32_t regDbDepthSize;
	uint32_t regDbDepthSlice;
	uint32_t regDbDepthView;
	uint32_t regDbHtileDataBase;
	uint32_t regDbHtileSurface;
	uint32_t regDbDepthInfo;
	uint16_t regWidth;
	uint16_t regHeight;
};