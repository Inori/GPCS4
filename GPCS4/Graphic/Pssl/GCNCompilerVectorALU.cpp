#include "GCNCompiler.h"


namespace pssl
{;

void GCNCompiler::emitVectorALU(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::VectorRegMov:
		emitVectorRegMov(ins);
		break;
	case Instruction::VectorLane:
		emitVectorLane(ins);
		break;
	case Instruction::VectorBitLogic:
		emitVectorBitLogic(ins);
		break;
	case Instruction::VectorBitField32:
		emitVectorBitField32(ins);
		break;
	case Instruction::VectorThreadMask:
		emitVectorThreadMask(ins);
		break;
	case Instruction::VectorBitField64:
		emitVectorBitField64(ins);
		break;
	case Instruction::VectorFpArith32:
		emitVectorFpArith32(ins);
		break;
	case Instruction::VectorFpRound32:
		emitVectorFpRound32(ins);
		break;
	case Instruction::VectorFpField32:
		emitVectorFpField32(ins);
		break;
	case Instruction::VectorFpTran32:
		emitVectorFpTran32(ins);
		break;
	case Instruction::VectorFpCmp32:
		emitVectorFpCmp32(ins);
		break;
	case Instruction::VectorFpArith64:
		emitVectorFpArith64(ins);
		break;
	case Instruction::VectorFpRound64:
		emitVectorFpRound64(ins);
		break;
	case Instruction::VectorFpField64:
		emitVectorFpField64(ins);
		break;
	case Instruction::VectorFpTran64:
		emitVectorFpTran64(ins);
		break;
	case Instruction::VectorFpCmp64:
		emitVectorFpCmp64(ins);
		break;
	case Instruction::VectorIntArith32:
		emitVectorIntArith32(ins);
		break;
	case Instruction::VectorIntArith64:
		emitVectorIntArith64(ins);
		break;
	case Instruction::VectorIntCmp32:
		emitVectorIntCmp32(ins);
		break;
	case Instruction::VectorIntCmp64:
		emitVectorIntCmp64(ins);
		break;
	case Instruction::VectorConv:
		emitVectorConv(ins);
		break;
	case Instruction::VectorFpGraph32:
		emitVectorFpGraph32(ins);
		break;
	case Instruction::VectorIntGraph:
		emitVectorIntGraph(ins);
		break;
	case Instruction::VectorMisc:
		emitVectorMisc(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not VectorALU.");
		break;
	}
}


void GCNCompiler::emitVectorRegMov(GCNInstruction& ins)
{
	LOG_ASSERT(ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_VOP1, "vector mov is not VOP1");

	auto inst = asInst<SIVOP1Instruction>(ins);
	auto op = inst->GetOp();

	auto src = inst->GetSRC0();
	auto dst = inst->GetVDST();
	auto sidx = inst->GetSRidx0();
	auto didx = inst->GetVDSTRidx();

	switch (op)
	{
	case SIVOP1Instruction::V_MOV_B32:
	{
		auto value = emitLoadScalarOperand(src, sidx, ins.literalConst);
		emitStoreVectorOperand(didx, value);
	}
		break;
	default:
		break;
	}
}

void GCNCompiler::emitVectorLane(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorBitLogic(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorBitField32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorThreadMask(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorBitField64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpArith32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpRound32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpField32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpTran32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpCmp32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpArith64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpRound64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpField64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpTran64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpCmp64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorIntArith32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorIntArith64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorIntCmp32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorIntCmp64(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorConv(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorFpGraph32(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorIntGraph(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorMisc(GCNInstruction& ins)
{

}

}  // namespace pssl