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
	// Program Flow instructions have many encodings.
	// We need to determine the encoding first
	// then dispatch them.
	auto insEnc = ins.instruction->GetInstructionFormat();
	switch (insEnc)
	{
	case Instruction::InstructionSet_SOP2:
	{
	
	}
		break;
	case Instruction::InstructionSet_SOPK:
	{

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
	case SISOP1Instruction::S_GETPC_B64:
		break;
	case SISOP1Instruction::S_SETPC_B64:
		break;
	default:
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
