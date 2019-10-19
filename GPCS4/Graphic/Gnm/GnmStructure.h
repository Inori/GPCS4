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


union DepthStencilControl
{
	struct
	{
		uint32_t stencilEnable : 1;
		uint32_t depthEnable : 1;
		uint32_t zWrite : 1;
		uint32_t depthBoundsEnable : 1;

		uint32_t zFunc : 3;
		uint32_t separateStencilEnable : 1;

		uint32_t stencilFunc : 3;
		uint32_t reserved0 : 1;

		uint32_t reserved1 : 8;

		uint32_t stencilFuncBack : 3;
		uint32_t reserved2 : 1;

		uint32_t reserved3 : 8;
	};

	uint32_t reg;
};


union PrimitiveSetup
{
	struct
	{
		uint32_t cullMode : 2;
		uint32_t frontFace : 1;
		uint32_t pointOrWairframe : 1;  // Not sure, means one of front or back is not rendered as solid

		uint32_t reserved0 : 1;
		uint32_t frontMode : 3;

		uint32_t backMode : 3;
		uint32_t frontOffsetMode : 1;

		uint32_t backOffsetMode : 1;
		uint32_t reserved1 : 3;

		uint32_t vertexWindowOffsetEnable : 1;
		uint32_t reserved2 : 1;
		uint32_t reserved3 : 1;
		uint32_t provokeVertexMode : 1;

		uint32_t perspectiveCorrectionEnable : 1;
		uint32_t reserved4 : 3;

		uint32_t reserved5 : 8;
	};

	uint32_t reg;
};


union BlendControl
{
	struct
	{
		uint32_t colorSourceMul : 5;
		uint32_t colorBlendFunc : 3;

		uint32_t colorDestMul : 5;
		uint32_t reserved0 : 3;
		

		uint32_t alphaSourceMul : 5;
		uint32_t alphaBlendFunc : 3;

		uint32_t alphaDestMul : 5;
		uint32_t separateAlphaEnable : 1;
		uint32_t blendEnable : 1;
		uint32_t reserved1 : 1;
	};

	uint32_t reg;
};

//////////////////////////////////////////////////////////////////////////
typedef uint32_t AlignmentType;

struct SizeAlign
{
	uint32_t m_size;
	AlignmentType m_align;
};