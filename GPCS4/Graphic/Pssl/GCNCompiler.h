#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "PsslFetchShader.h"
#include "../Gve/GveShader.h"
#include "GCNInstruction.h"
#include <optional>

namespace pssl
{;


class GCNCompiler
{
public:
	GCNCompiler(const PsslProgramInfo& progInfo,
		std::optional<PsslFetchShader> fsShader = {});
	~GCNCompiler();

	void processInstruction(GCNInstruction& ins);

	RcPtr<gve::GveShader> finalize();
private:

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

	// Convenient functions to dynamic cast instruction types
	template <typename InsType>
	inline InsType* castTo(GCNInstruction& ins)
	{
		return dynamic_cast<InsType*>(ins.instruction.get());
	}

};



} // namespace pssl
