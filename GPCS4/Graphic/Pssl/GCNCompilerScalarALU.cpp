#include "GCNCompiler.h"
#include "GCNParser/SOP1Instruction.h"

namespace pssl
{;

void GCNCompiler::emitScalarALU(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::ScalarArith:
		emitScalarArith(ins);
		break;
	case Instruction::ScalarAbs:
		emitScalarAbs(ins);
		break;
	case Instruction::ScalarMov:
		emitScalarMov(ins);
		break;
	case Instruction::ScalarCmp:
		emitScalarCmp(ins);
		break;
	case Instruction::ScalarSelect:
		emitScalarSelect(ins);
		break;
	case Instruction::ScalarBitLogic:
		emitScalarBitLogic(ins);
		break;
	case Instruction::ScalarBitManip:
		emitScalarBitManip(ins);
		break;
	case Instruction::ScalarBitField:
		emitScalarBitField(ins);
		break;
	case Instruction::ScalarConv:
		emitScalarConv(ins);
		break;
	case Instruction::ScalarExecMask:
		emitScalarExecMask(ins);
		break;
	case Instruction::ScalarQuadMask:
		emitScalarQuadMask(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not ScalarALU.");
		break;
	}
}

void GCNCompiler::emitScalarMov(GCNInstruction& ins)
{
	auto inst = asInst<SISOP1Instruction>(ins);
	auto op = inst->GetOp();

	auto sdst = inst->GetSDST();
	auto sidx = inst->GetSRidx();
	auto ssrc = inst->GetSSRC0();
	auto didx = inst->GetSDSTRidx();

	switch (op)
	{
	case SISOP1Instruction::S_MOV_B32:
	{
		auto value = emitLoadScalarOperand(ssrc, sidx, ins.literalConst);
		emitStoreScalarOperand(sdst, didx, value);
	}
		break;
	case SISOP1Instruction::S_MOV_B64:
	{
		// TODO:
		// I only find 's_mov_b64 vcc, exec' currently, just pass
	}
		break;
	default:
		LOG_FIXME("Not implementd: op %X", op);
		break;
	}
}

void GCNCompiler::emitScalarArith(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarAbs(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarCmp(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarSelect(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarBitLogic(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarBitManip(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarBitField(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarConv(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarExecMask(GCNInstruction& ins)
{

}

void GCNCompiler::emitScalarQuadMask(GCNInstruction& ins)
{
	auto inst = asInst<SISOP1Instruction>(ins);
	auto op = inst->GetOp();
	
	switch (op)
	{
	case SISOP1Instruction::S_WQM_B64:
		break;
	default:
		break;
	}
}

}