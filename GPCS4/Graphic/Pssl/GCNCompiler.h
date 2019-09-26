#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "PsslFetchShader.h"
#include "GCNInstruction.h"

#include "GCNParser/SMRDInstruction.h"
#include "GCNParser/SOPPInstruction.h"
#include "GCNParser/SOPCInstruction.h"
#include "GCNParser/SOP1Instruction.h"
#include "GCNParser/SOPKInstruction.h"
#include "GCNParser/SOP2Instruction.h"
#include "GCNParser/VINTRPInstruction.h"
#include "GCNParser/DSInstruction.h"
#include "GCNParser/MUBUFInstruction.h"
#include "GCNParser/MTBUFInstruction.h"
#include "GCNParser/MIMGInstruction.h"
#include "GCNParser/EXPInstruction.h"
#include "GCNParser/VOPInstruction.h"

#include "../Gve/GveShader.h"
#include "../SpirV/SpirvModule.h"

#include <optional>

namespace pssl
{;

/**
 * \brief Scalar value type
 *
 * Enumerates possible register component
 * types. Scalar types are represented as
 * a one-component vector type.
 */
enum PsslScalarType : uint32_t
{
	Uint32 = 0,
	Uint64 = 1,
	Sint32 = 2,
	Sint64 = 3,
	Float32 = 4,
	Float64 = 5,
	Bool = 6,
};

/**
 * \brief Vector type
 *
 * Convenience struct that stores a scalar
 * type and a component count. The compiler
 * can use this to generate SPIR-V types.
 */
struct PsslVectorType 
{
	PsslScalarType    ctype;
	uint32_t          ccount;
};


/**
 * \brief Array type
 *
 * Convenience struct that stores a scalar type, a
 * component count and an array size. An array of
 * length 0 will be evaluated to a vector type. The
 * compiler can use this to generate SPIR-V types.
 */
struct PsslArrayType 
{
	PsslScalarType    ctype;
	uint32_t          ccount;
	uint32_t          alength;
};


struct GcnStateRegister
{
	// local data share
	uint64_t lds;
	// exec mask
	uint64_t exec;
	// vector condition code
	uint64_t vcc;
	// mode register
	uint32_t mode;
	// memory descriptor register
	uint32_t m0;
};

/**
 * \brief Vertex shader-specific structure
 */
struct GcnCompilerVsPart {
	uint32_t functionId = 0;

	uint32_t builtinVertexId = 0;
	uint32_t builtinInstanceId = 0;
	uint32_t builtinBaseVertex = 0;
	uint32_t builtinBaseInstance = 0;
};


/**
 * \brief Pixel shader-specific structure
 */
struct GcnCompilerPsPart {
	uint32_t functionId = 0;

	uint32_t builtinFragCoord = 0;
	uint32_t builtinDepth = 0;
	uint32_t builtinIsFrontFace = 0;
	uint32_t builtinSampleId = 0;
	uint32_t builtinSampleMaskIn = 0;
	uint32_t builtinSampleMaskOut = 0;
	uint32_t builtinLayer = 0;
	uint32_t builtinViewportId = 0;

	uint32_t builtinLaneId = 0;
	uint32_t killState = 0;

	uint32_t specRsSampleCount = 0;
};


/**
 * \brief Compute shader-specific structure
 */
struct GcnCompilerCsPart {
	uint32_t functionId = 0;

	uint32_t workgroupSizeX = 0;
	uint32_t workgroupSizeY = 0;
	uint32_t workgroupSizeZ = 0;

	uint32_t builtinGlobalInvocationId = 0;
	uint32_t builtinLocalInvocationId = 0;
	uint32_t builtinLocalInvocationIndex = 0;
	uint32_t builtinWorkgroupId = 0;
};


class GCNCompiler
{
public:
	GCNCompiler(const PsslProgramInfo& progInfo);
	GCNCompiler(const PsslProgramInfo& progInfo, const std::vector<VertexInputSemantic>& inputSemantic);
	~GCNCompiler();

	void processInstruction(GCNInstruction& ins);

	RcPtr<gve::GveShader> finalize();


private:

	PsslProgramInfo m_programInfo;

	std::vector<VertexInputSemantic> m_vsInputSemantic;

	SpirvModule m_module;

	///////////////////////////////////////////////////
	// Entry point description - we'll need to declare
	// the function ID and all input/output variables.
	std::vector<uint32_t> m_entryPointInterfaces;
	uint32_t              m_entryPointId = 0;

	////////////////////////////////////////////////////
	// Per-vertex input and output blocks. Depending on
	// the shader stage, these may be declared as arrays.
	uint32_t m_perVertexIn = 0;
	uint32_t m_perVertexOut = 0;


	//////////////////////////////////////////////
	// Function state tracking. Required in order
	// to properly end functions in some cases.
	bool m_insideFunction = false;


	///////////////////////////////////
	// Shader-specific data structures
	GcnCompilerVsPart m_vs;
	GcnCompilerPsPart m_ps;
	GcnCompilerCsPart m_cs;

private:

	void emitInit();

	void emitVsInit();
	void emitHsInit();
	void emitDsInit();
	void emitGsInit();
	void emitPsInit();
	void emitCsInit();

	void emitFunctionBegin(
		uint32_t                entryPoint,
		uint32_t                returnType,
		uint32_t                funcType);

	void emitFunctionEnd();

	void emitFunctionLabel();


	// Category handlers
	void emitScalarALU(GCNInstruction& ins);
	void emitScalarMemory(GCNInstruction& ins);
	void emitVectorALU(GCNInstruction& ins);
	void emitVectorMemory(GCNInstruction& ins);
	void emitFlowControl(GCNInstruction& ins);
	void emitDataShare(GCNInstruction& ins);
	void emitVectorInterpolation(GCNInstruction& ins);
	void emitExport(GCNInstruction& ins);
	void emitDebugProfile(GCNInstruction& ins);

	// ScalarALU
	void emitScalarArith(GCNInstruction& ins);
	void emitScalarAbs(GCNInstruction& ins);
	void emitScalarMov(GCNInstruction& ins);
	void emitScalarCmp(GCNInstruction& ins);
	void emitScalarSelect(GCNInstruction& ins);
	void emitScalarBitLogic(GCNInstruction& ins);
	void emitScalarBitManip(GCNInstruction& ins);
	void emitScalarBitField(GCNInstruction& ins);
	void emitScalarConv(GCNInstruction& ins);
	void emitScalarExecMask(GCNInstruction& ins);
	void emitScalarQuadMask(GCNInstruction& ins);

	// VectorALU
	void emitVectorRegMov(GCNInstruction& ins);
	void emitVectorLane(GCNInstruction& ins);
	void emitVectorBitLogic(GCNInstruction& ins);
	void emitVectorBitField32(GCNInstruction& ins);
	void emitVectorThreadMask(GCNInstruction& ins);
	void emitVectorBitField64(GCNInstruction& ins);
	void emitVectorFpArith32(GCNInstruction& ins);
	void emitVectorFpRound32(GCNInstruction& ins);
	void emitVectorFpField32(GCNInstruction& ins);
	void emitVectorFpTran32(GCNInstruction& ins);
	void emitVectorFpCmp32(GCNInstruction& ins);
	void emitVectorFpArith64(GCNInstruction& ins);
	void emitVectorFpRound64(GCNInstruction& ins);
	void emitVectorFpField64(GCNInstruction& ins);
	void emitVectorFpTran64(GCNInstruction& ins);
	void emitVectorFpCmp64(GCNInstruction& ins);
	void emitVectorIntArith32(GCNInstruction& ins);
	void emitVectorIntArith64(GCNInstruction& ins);
	void emitVectorIntCmp32(GCNInstruction& ins);
	void emitVectorIntCmp64(GCNInstruction& ins);
	void emitVectorConv(GCNInstruction& ins);
	void emitVectorFpGraph32(GCNInstruction& ins);
	void emitVectorIntGraph(GCNInstruction& ins);
	void emitVectorMisc(GCNInstruction& ins);

	// FlowControl
	void emitScalarProgFlow(GCNInstruction& ins);
	void emitScalarSync(GCNInstruction& ins);
	void emitScalarWait(GCNInstruction& ins);
	void emitScalarCache(GCNInstruction& ins);
	void emitScalarPrior(GCNInstruction& ins);
	void emitScalarRegAccess(GCNInstruction& ins);
	void emitScalarMsg(GCNInstruction& ins);

	// ScalarMemory
	void emitScalarMemRd(GCNInstruction& ins);
	void emitScalarMemUt(GCNInstruction& ins);

	// VectorMemory
	void emitVectorMemBufNoFmt(GCNInstruction& ins);
	void emitVectorMemBufFmt(GCNInstruction& ins);
	void emitVectorMemImgNoSmp(GCNInstruction& ins);
	void emitVectorMemImgSmp(GCNInstruction& ins);
	void emitVectorMemImgUt(GCNInstruction& ins);
	void emitVectorMemL1Cache(GCNInstruction& ins);

	// DataShare
	void emitDsIdxRd(GCNInstruction& ins);
	void emitDsIdxWr(GCNInstruction& ins);
	void emitDsIdxWrXchg(GCNInstruction& ins);
	void emitDsIdxCondXchg(GCNInstruction& ins);
	void emitDsIdxWrap(GCNInstruction& ins);
	void emitDsAtomicArith32(GCNInstruction& ins);
	void emitDsAtomicArith64(GCNInstruction& ins);
	void emitDsAtomicMinMax32(GCNInstruction& ins);
	void emitDsAtomicMinMax64(GCNInstruction& ins);
	void emitDsAtomicCmpSt32(GCNInstruction& ins);
	void emitDsAtomicCmpSt64(GCNInstruction& ins);
	void emitDsAtomicLogic32(GCNInstruction& ins);
	void emitDsAtomicLogic64(GCNInstruction& ins);
	void emitDsAppendCon(GCNInstruction& ins);
	void emitDsDataShareUt(GCNInstruction& ins);
	void emitDsDataShareMisc(GCNInstruction& ins);
	void emitGdsSync(GCNInstruction& ins);
	void emitGdsOrdCnt(GCNInstruction& ins);

	// VectorInterpolation
	void emitVectorInterpFpCache(GCNInstruction& ins);

	// Export
	void emitExp(GCNInstruction& ins);

	// DebugProfile
	void emitDbgProf(GCNInstruction& ins);

	/////////////////////////////////////////////////////////


	// Convenient functions to dynamic cast instruction types
	template <typename InsType>
	inline InsType* castTo(GCNInstruction& ins)
	{
		return dynamic_cast<InsType*>(ins.instruction.get());
	}

	///////////////////////////
	// Type definition methods
	uint32_t getPerVertexBlockId();

};



} // namespace pssl
