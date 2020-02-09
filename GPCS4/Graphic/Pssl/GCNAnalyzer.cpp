#include "GCNAnalyzer.h"
#include "GCNDecoder.h"
#include "UtilBit.h"
#include "GCNEnums.h"
#include "GCNParser/ParserSI.h"


LOG_CHANNEL(Graphic.Pssl.GCNAnalyzer);

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
	analyzeInstruction(ins);

	updateProgramCounter(ins);
}

void GCNAnalyzer::analyzeInstruction(GCNInstruction& ins)
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
		collectBranchLabel(ins);
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
		getVinterpInfo(ins);
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
	auto inst = asInst<EXPInstruction>(ins);

	GcnExportInfo info;
	info.target = (uint32_t)inst->GetTGT();
	info.isCompressed = inst->GetCOMPR();

	GcnRegMask mask;
	if (!info.isCompressed)
	{
		mask = GcnRegMask(inst->GetEn());
	}
	else
	{
		uint8_t en = inst->GetEn();
		mask = GcnRegMask(
			bit::extract<uint8_t>(en, 0, 1),
			bit::extract<uint8_t>(en, 2, 3),
			false,
			false
		);
	}

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

void GCNAnalyzer::getVinterpInfo(GCNInstruction& ins)
{
	auto inst = asInst<SIVINTRPInstruction>(ins);

	uint32_t attrIdx = inst->GetATTR();
	m_vinterpAttrSet.insert(attrIdx);

	m_analysis->vinterpAttrCount = m_vinterpAttrSet.size();
}

void GCNAnalyzer::collectBranchLabel(GCNInstruction& ins)
{
	// TODO:
	// Support s_cbranch_i_fork, s_cbranch_g_fork s_cbranch_join and etc..

	do 
	{
		auto inst = asInst<SISOPPInstruction>(ins);
		if (!inst)
		{
			LOG_WARN("control flow instruction not SOPP encoding");
			break;
		}

		auto op = inst->GetOp();
		switch (op)
		{
		case SISOPPInstruction::S_BRANCH:
		case SISOPPInstruction::S_CBRANCH_SCC0:
		case SISOPPInstruction::S_CBRANCH_SCC1:
		case SISOPPInstruction::S_CBRANCH_VCCZ:
		case SISOPPInstruction::S_CBRANCH_VCCNZ:
		case SISOPPInstruction::S_CBRANCH_EXECZ:
		case SISOPPInstruction::S_CBRANCH_EXECNZ:
		case SISOPPInstruction::S_CBRANCH_CDBGSYS:
		case SISOPPInstruction::S_CBRANCH_CDBGUSER:
		case SISOPPInstruction::S_CBRANCH_CDBGSYS_OR_USER:
		case SISOPPInstruction::S_CBRANCH_CDBGSYS_AND_USER:
		{
			int16_t imm = inst->GetSIMM16();
			uint32_t target = m_programCounter + imm * 4 + 4;
			m_analysis->branchLabels.insert(std::make_pair(target, InvalidSpvId));
		}
			break;
		}
	} while (false);
}

}  // namespace pssl