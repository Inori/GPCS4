#include "GCNCompiler.h"

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


void GCNCompiler::emitScalarProgFlow(GCNInstruction& ins)
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
	default:
		LOG_FIXME("Not implementd: op %X", op);
		break;
	}
}

void GCNCompiler::emitScalarSync(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarWait(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarCache(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarPrior(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarRegAccess(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarMsg(GCNInstruction& ins)
{

}

}  // namespace pssl
