#include "GcnAnalysis.h"
#include "GcnInstruction.h"
#include "GcnDecoder.h"

LOG_CHANNEL(Graphic.Gcn.GcnAnalysis);

namespace sce::gcn
{
	GcnAnalyzer::GcnAnalyzer(
		const GcnProgramInfo& programInfo,
		GcnAnalysisInfo&      analysis) :
		m_analysis(&analysis)
	{
	}

	GcnAnalyzer::~GcnAnalyzer()
	{
	}

	void GcnAnalyzer::processInstruction(
		const GcnShaderInstruction& ins)
	{
		analyzeInstruction(ins);
	}

	void GcnAnalyzer::analyzeInstruction(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::ScalarArith:
				break;
			case GcnInstClass::ScalarAbs:
				break;
			case GcnInstClass::ScalarMov:
				break;
			case GcnInstClass::ScalarCmp:
				break;
			case GcnInstClass::ScalarSelect:
				break;
			case GcnInstClass::ScalarBitLogic:
				break;
			case GcnInstClass::ScalarBitManip:
				break;
			case GcnInstClass::ScalarBitField:
				break;
			case GcnInstClass::ScalarConv:
				break;
			case GcnInstClass::ScalarExecMask:
				break;
			case GcnInstClass::ScalarQuadMask:
				break;
			case GcnInstClass::VectorRegMov:
				break;
			case GcnInstClass::VectorLane:
				this->analyzeLane(ins);
				break;
			case GcnInstClass::VectorBitLogic:
				break;
			case GcnInstClass::VectorBitField32:
				break;
			case GcnInstClass::VectorThreadMask:
				break;
			case GcnInstClass::VectorBitField64:
				break;
			case GcnInstClass::VectorFpArith32:
				break;
			case GcnInstClass::VectorFpRound32:
				break;
			case GcnInstClass::VectorFpField32:
				break;
			case GcnInstClass::VectorFpTran32:
				break;
			case GcnInstClass::VectorFpCmp32:
				break;
			case GcnInstClass::VectorFpArith64:
				break;
			case GcnInstClass::VectorFpRound64:
				break;
			case GcnInstClass::VectorFpField64:
				break;
			case GcnInstClass::VectorFpTran64:
				break;
			case GcnInstClass::VectorFpCmp64:
				break;
			case GcnInstClass::VectorIntArith32:
				break;
			case GcnInstClass::VectorIntArith64:
				break;
			case GcnInstClass::VectorIntCmp32:
				break;
			case GcnInstClass::VectorIntCmp64:
				break;
			case GcnInstClass::VectorConv:
				break;
			case GcnInstClass::VectorFpGraph32:
				break;
			case GcnInstClass::VectorIntGraph:
				break;
			case GcnInstClass::VectorMisc:
				break;
			case GcnInstClass::ScalarProgFlow:
				break;
			case GcnInstClass::ScalarSync:
				break;
			case GcnInstClass::ScalarWait:
				break;
			case GcnInstClass::ScalarCache:
				break;
			case GcnInstClass::ScalarPrior:
				break;
			case GcnInstClass::ScalarRegAccess:
				break;
			case GcnInstClass::ScalarMsg:
				break;
			case GcnInstClass::ScalarMemRd:
				break;
			case GcnInstClass::ScalarMemUt:
				break;
			case GcnInstClass::VectorMemBufNoFmt:
				break;
			case GcnInstClass::VectorMemBufFmt:
				break;
			case GcnInstClass::VectorMemImgNoSmp:
				break;
			case GcnInstClass::VectorMemImgSmp:
				break;
			case GcnInstClass::VectorMemImgUt:
				break;
			case GcnInstClass::VectorMemL1Cache:
				break;
			case GcnInstClass::DsIdxRd:
				break;
			case GcnInstClass::DsIdxWr:
				break;
			case GcnInstClass::DsIdxWrXchg:
				break;
			case GcnInstClass::DsIdxCondXchg:
				break;
			case GcnInstClass::DsIdxWrap:
				break;
			case GcnInstClass::DsAtomicArith32:
				break;
			case GcnInstClass::DsAtomicArith64:
				break;
			case GcnInstClass::DsAtomicMinMax32:
				break;
			case GcnInstClass::DsAtomicMinMax64:
				break;
			case GcnInstClass::DsAtomicCmpSt32:
				break;
			case GcnInstClass::DsAtomicCmpSt64:
				break;
			case GcnInstClass::DsAtomicLogic32:
				break;
			case GcnInstClass::DsAtomicLogic64:
				break;
			case GcnInstClass::DsAppendCon:
				break;
			case GcnInstClass::DsDataShareUt:
				break;
			case GcnInstClass::DsDataShareMisc:
				break;
			case GcnInstClass::GdsSync:
				break;
			case GcnInstClass::GdsOrdCnt:
				break;
			case GcnInstClass::VectorInterpFpCache:
				break;
			case GcnInstClass::Exp:
				this->analyzeExp(ins);
				break;
			case GcnInstClass::DbgProf:
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnAnalyzer::analyzeExp(const GcnShaderInstruction& ins)
	{
		auto exp = gcnInstructionAs<GcnShaderInstEXP>(ins);

		if (exp.target == GcnExportTarget::Param)
		{
			++m_analysis->exportInfo.paramCount;

			uint32_t paramIndex                       = exp.control.target - GcnExpParam0;
			m_analysis->exportInfo.params[paramIndex] = GcnRegMask(exp.control.en);
		}
		else if (exp.target == GcnExportTarget::Mrt)
		{
			++m_analysis->exportInfo.mrtCount;

			uint32_t mrtIndex                     = exp.control.target;
			m_analysis->exportInfo.mrts[mrtIndex] = GcnRegMask(exp.control.en);
		}
	}

	void GcnAnalyzer::analyzeLane(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;

		// Collect lane src vgprs.
		// We don't need to collect V_WRITELANE_B32,
		// because we can write to the lane right at we translating
		// that instruction.
		if (op == GcnOpcode::V_READFIRSTLANE_B32 || op == GcnOpcode::V_READLANE_B32)
		{
			uint32_t vgpr = ins.src[0].code;
			m_analysis->laneVgprs.insert(vgpr);
		}
	}

}  // namespace sce::gcn