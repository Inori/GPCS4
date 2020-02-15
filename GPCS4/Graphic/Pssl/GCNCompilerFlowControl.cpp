#include "GCNCompiler.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerFlowControl);

namespace pssl
{;

void GCNCompiler::emitFlowControl(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::ScalarProgFlow:
		emitScalarProgFlow(ins);
		break;
	case Instruction::ScalarSync:
		emitScalarSync(ins);
		break;
	case Instruction::ScalarWait:
		emitScalarWait(ins);
		break;
	case Instruction::ScalarCache:
		emitScalarCache(ins);
		break;
	case Instruction::ScalarPrior:
		emitScalarPrior(ins);
		break;
	case Instruction::ScalarRegAccess:
		emitScalarRegAccess(ins);
		break;
	case Instruction::ScalarMsg:
		emitScalarMsg(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not FlowControl.");
		break;
	}
}

void GCNCompiler::emitBranchLabelTry()
{
	do
	{
		auto iter = m_branchLabels.find(m_programCounter);
		if (iter == m_branchLabels.end())
		{
			break;
		}

		uint32_t& labelId = iter->second;

		// A label can occur before or after s_branch_xxx instruction.
		// If before, labelId should be InvalidSpvId, then we allocate a new id for it.
		// If after, labelId should be already set by s_branch_xxx instruction handler.
		if (labelId == InvalidSpvId)
		{
			labelId = m_module.allocateId();
		}

		m_module.opLabel(labelId);
	} while (false);
}

void GCNCompiler::emitScalarProgFlow(GCNInstruction& ins)
{
	// Program Flow instructions have many encodings.
	// We need to determine the encoding first
	// then dispatch them.
	auto insEnc = ins.instruction->GetInstructionFormat();
	switch (insEnc)
	{
	case Instruction::InstructionSet_SOP2:
	{
		LOG_PSSL_UNHANDLED_INST();
	}
		break;
	case Instruction::InstructionSet_SOPK:
	{
		LOG_PSSL_UNHANDLED_INST();
	}
		break;
	case Instruction::InstructionSet_SOP1:
	{
		auto inst = asInst<SISOP1Instruction>(ins);
		auto op = inst->GetOp();
		switch (op)
		{
		case SISOP1Instruction::S_GETPC_B64:
		case SISOP1Instruction::S_SETPC_B64:
		case SISOP1Instruction::S_SWAPPC_B64:
			emitScalarProgFlowPC(ins);
			break;
		}
	}
		break;
	case Instruction::InstructionSet_SOPC:
	{
		LOG_PSSL_UNHANDLED_INST();
	}
		break;
	case Instruction::InstructionSet_SOPP:
	{
		auto inst = asInst<SISOPPInstruction>(ins);
		auto op = inst->GetOp();
		switch (op)
		{
		case SISOPPInstruction::S_ENDPGM:
			emitFunctionEnd();
			break;
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
			emitScalarProgFlowBranch(ins);
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}

void GCNCompiler::emitScalarProgFlowPC(GCNInstruction& ins)
{
	auto inst = asInst<SISOP1Instruction>(ins);
	auto op = inst->GetOp();
	auto sidx = inst->GetSRidx();
	auto didx = inst->GetSDSTRidx();

	switch (op)
	{
	case SISOP1Instruction::S_SWAPPC_B64:
	{
		if (sidx == 0 && sidx == didx)
		{
			// s_swappc_b64  s[0:1], s[0:1]
			// call fetch shader, since we've emulated fetch shader with a fetch_vs function
			// we ignore this
			LOG_DEBUG("call fetch shader.");
		}
	}
		break;
	case SISOP1Instruction::S_GETPC_B64:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SISOP1Instruction::S_SETPC_B64:
		LOG_PSSL_UNHANDLED_INST();
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitScalarProgFlowBranch(GCNInstruction& ins)
{
	// This function is paired with emitBranchLabelTry

	auto inst = asInst<SISOPPInstruction>(ins);
	auto op = inst->GetOp();

	int16_t imm = inst->GetSIMM16();
	uint32_t target = m_programCounter + imm * 4 + 4;

	auto iter = m_branchLabels.find(target);
	LOG_ASSERT(iter != m_branchLabels.end(), "branch target can not be found in m_branchLabels");
	uint32_t& trueLabelId = iter->second;

	// If trueLabelId is InvalidSpvId, this indicates the target label is under current branch instruction.
	// Or, it's above current branch instruction, 
	// in this case, trueLabelId should be already initialized to a non-zero value.
	if (trueLabelId == InvalidSpvId)
	{
		trueLabelId = m_module.allocateId();
	}

	const uint32_t boolTypeId = getScalarTypeId(SpirvScalarType::Bool);

	bool isUnconditional = false;
	uint32_t conditionId = 0;

	switch (op)
	{
	case SISOPPInstruction::S_BRANCH:
	{
		m_module.opBranch(trueLabelId);
		isUnconditional = true;
	}
		break;
	case SISOPPInstruction::S_CBRANCH_SCC0:
		conditionId = m_module.opLogicalNot(boolTypeId, m_statusRegs.sccz.id);
		break;
	case SISOPPInstruction::S_CBRANCH_SCC1:
		conditionId = m_statusRegs.sccz.id;
		break;
	case SISOPPInstruction::S_CBRANCH_VCCZ:
		conditionId = emitRegisterZeroTest(
						  emitValueLoad(m_statusRegs.vcc),
						  SpirvZeroTest::TestZ).id;
		break;
	case SISOPPInstruction::S_CBRANCH_VCCNZ:
		conditionId = emitRegisterZeroTest(
						  emitValueLoad(m_statusRegs.vcc),
						  SpirvZeroTest::TestNz).id;
		break;
	case SISOPPInstruction::S_CBRANCH_EXECZ:
		conditionId = emitRegisterZeroTest(
						  emitValueLoad(m_statusRegs.exec),
						  SpirvZeroTest::TestZ).id;
		break;
	case SISOPPInstruction::S_CBRANCH_EXECNZ:
		conditionId = emitRegisterZeroTest(
						  emitValueLoad(m_statusRegs.exec),
						  SpirvZeroTest::TestNz).id;
		break;
	case SISOPPInstruction::S_CBRANCH_CDBGSYS:
	case SISOPPInstruction::S_CBRANCH_CDBGUSER:
	case SISOPPInstruction::S_CBRANCH_CDBGSYS_OR_USER:
	case SISOPPInstruction::S_CBRANCH_CDBGSYS_AND_USER:
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (!isUnconditional)
	{
		uint32_t falseLabelId = m_module.allocateId();
		m_module.opBranchConditional(conditionId, trueLabelId, falseLabelId);
		m_module.opLabel(falseLabelId);
	}
}

void GCNCompiler::emitScalarSync(GCNInstruction& ins)
{
	auto inst = asInst<SISOPPInstruction>(ins);
	auto op = inst->GetOp();
	switch (op)
	{
	case SISOPPInstruction::S_BARRIER:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SISOPPInstruction::S_WAITCNT:
		// pass
		break;
	}
}

void GCNCompiler::emitScalarWait(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarCache(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarPrior(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarRegAccess(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarMsg(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

}  // namespace pssl
