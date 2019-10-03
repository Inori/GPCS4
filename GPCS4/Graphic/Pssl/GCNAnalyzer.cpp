#include "GCNAnalyzer.h"
#include "GCNDecoder.h"
#include "GCNParser/ParserSI.h"

namespace pssl
{;


GCNAnalyzer::GCNAnalyzer(GcnAnalysisInfo& analysis):
	m_analysis(&analysis)
{

}

GCNAnalyzer::~GCNAnalyzer()
{
}

void GCNAnalyzer::processInstruction(GCNInstruction& ins)
{
	auto insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::InstructionClassUnknown:
		break;
	case Instruction::ScalarArith:
		break;
	case Instruction::ScalarAbs:
		break;
	case Instruction::ScalarMov:
		break;
	case Instruction::ScalarCmp:
		break;
	case Instruction::ScalarSelect:
		break;
	case Instruction::ScalarBitLogic:
		break;
	case Instruction::ScalarBitManip:
		break;
	case Instruction::ScalarBitField:
		break;
	case Instruction::ScalarConv:
		break;
	case Instruction::ScalarExecMask:
		break;
	case Instruction::ScalarQuadMask:
		break;
	case Instruction::VectorRegMov:
		break;
	case Instruction::VectorLane:
		break;
	case Instruction::VectorBitLogic:
		break;
	case Instruction::VectorBitField32:
		break;
	case Instruction::VectorThreadMask:
		break;
	case Instruction::VectorBitField64:
		break;
	case Instruction::VectorFpArith32:
		break;
	case Instruction::VectorFpRound32:
		break;
	case Instruction::VectorFpField32:
		break;
	case Instruction::VectorFpTran32:
		break;
	case Instruction::VectorFpCmp32:
		break;
	case Instruction::VectorFpArith64:
		break;
	case Instruction::VectorFpRound64:
		break;
	case Instruction::VectorFpField64:
		break;
	case Instruction::VectorFpTran64:
		break;
	case Instruction::VectorFpCmp64:
		break;
	case Instruction::VectorIntArith32:
		break;
	case Instruction::VectorIntArith64:
		break;
	case Instruction::VectorIntCmp32:
		break;
	case Instruction::VectorIntCmp64:
		break;
	case Instruction::VectorConv:
		break;
	case Instruction::VectorFpGraph32:
		break;
	case Instruction::VectorIntGraph:
		break;
	case Instruction::VectorMisc:
		break;
	case Instruction::ScalarProgFlow:
		break;
	case Instruction::ScalarSync:
		break;
	case Instruction::ScalarWait:
		break;
	case Instruction::ScalarCache:
		break;
	case Instruction::ScalarPrior:
		break;
	case Instruction::ScalarRegAccess:
		break;
	case Instruction::ScalarMsg:
		break;
	case Instruction::ScalarMemRd:
		break;
	case Instruction::ScalarMemUt:
		break;
	case Instruction::VectorMemBufNoFmt:
		break;
	case Instruction::VectorMemBufFmt:
		break;
	case Instruction::VectorMemImgNoSmp:
		break;
	case Instruction::VectorMemImgSmp:
		break;
	case Instruction::VectorMemImgUt:
		break;
	case Instruction::VectorMemL1Cache:
		break;
	case Instruction::DsIdxRd:
		break;
	case Instruction::DsIdxWr:
		break;
	case Instruction::DsIdxWrXchg:
		break;
	case Instruction::DsIdxCondXchg:
		break;
	case Instruction::DsIdxWrap:
		break;
	case Instruction::DsAtomicArith32:
		break;
	case Instruction::DsAtomicArith64:
		break;
	case Instruction::DsAtomicMinMax32:
		break;
	case Instruction::DsAtomicMinMax64:
		break;
	case Instruction::DsAtomicCmpSt32:
		break;
	case Instruction::DsAtomicCmpSt64:
		break;
	case Instruction::DsAtomicLogic32:
		break;
	case Instruction::DsAtomicLogic64:
		break;
	case Instruction::DsAppendCon:
		break;
	case Instruction::DsDataShareUt:
		break;
	case Instruction::DsDataShareMisc:
		break;
	case Instruction::GdsSync:
		break;
	case Instruction::GdsOrdCnt:
		break;
	case Instruction::VectorInterpFpCache:
		break;
	case Instruction::Exp:
		getExportInfo(ins);
		break;
	case Instruction::DbgProf:
		break;
	case Instruction::InstructionClassCount:
		break;
	default:
		break;
	}
}

void GCNAnalyzer::getExportInfo(GCNInstruction& ins)
{
	auto inst = dynamic_cast<EXPInstruction*>(ins.instruction.get());

	GcnExportInfo info;
	info.target = (uint32_t)inst->GetTGT();

	GcnRegMask mask(inst->GetEn());
	for (uint32_t i = 0; i != 4; ++i)
	{
		if (mask[i])
		{
			uint32_t regIdx = inst->GetVSRC(i);
			info.regIndices.push_back(regIdx);
		}
	}

	m_analysis->expParams.push_back(info);
}

}  // namespace pssl