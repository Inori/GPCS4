#include "GcnAnalysis.h"
#include "GcnInstruction.h"
#include "GcnDecoder.h"

LOG_CHANNEL(Graphic.Gcn.GcnAnalysis);

namespace sce::gcn
{
	GcnCfgPass::GcnCfgPass(
		GcnAnalysisInfo& analysis) :
		m_analysis(&analysis)
	{
	}

	GcnCfgPass::~GcnCfgPass()
	{
	}

	void GcnCfgPass::processInstruction(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		if (opClass == GcnInstClass::ScalarProgFlow)
		{
			this->analyzeBranch(ins);
		}

		updateProgramCounter(ins);
	}

	void GcnCfgPass::analyzeBranch(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_BRANCH:
			case GcnOpcode::S_CBRANCH_SCC0:
			case GcnOpcode::S_CBRANCH_SCC1:
			case GcnOpcode::S_CBRANCH_VCCZ:
			case GcnOpcode::S_CBRANCH_VCCNZ:
			case GcnOpcode::S_CBRANCH_EXECZ:
			case GcnOpcode::S_CBRANCH_EXECNZ:
			case GcnOpcode::S_CBRANCH_CDBGSYS:
			case GcnOpcode::S_CBRANCH_CDBGUSER:
			case GcnOpcode::S_CBRANCH_CDBGSYS_OR_USER:
			case GcnOpcode::S_CBRANCH_CDBGSYS_AND_USER:
			{
				uint32_t target = getBranchTarget(ins);
				GcnCfgBlock block  = {};
				m_analysis->controlFlowBlocks.insert(
					std::make_pair(target, block));
			}
				break;
			default:
				break;
		}
	}

	//////////////////////////////////////////////////////////////////////

	GcnAnalyzer::GcnAnalyzer(
		const GcnProgramInfo& programInfo,
		GcnAnalysisInfo&      analysis) :
		m_analysis(&analysis),
		m_cfgPass(analysis)
	{
	}

	GcnAnalyzer::~GcnAnalyzer()
	{
	}

	void GcnAnalyzer::processInstruction(
		const GcnShaderInstruction& ins)
	{
		analyzeInstruction(ins);

		updateProgramCounter(ins);

		m_cfgPass.processInstruction(ins);
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

}  // namespace sce::gcn