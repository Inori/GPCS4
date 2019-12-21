#include "GCNCompiler.h"
#include "GCNParser/SOP1Instruction.h"
#include "Platform/UtilString.h"

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
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitScalarArith(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarAbs(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarCmp(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarSelect(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarBitLogic(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarBitManip(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarBitField(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarConv(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarExecMask(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarQuadMask(GCNInstruction& ins)
{
	auto inst = asInst<SISOP1Instruction>(ins);
	auto op = inst->GetOp();
	
	auto ssrc0 = inst->GetSSRC0();
	auto sdst = inst->GetSDST();

	switch (op)
	{
	case SISOP1Instruction::S_WQM_B64:
	{
		if (static_cast<uint32_t>(ssrc0) == static_cast<uint32_t>(sdst) && 
			ssrc0 == SISOP1Instruction::SSRC::SSRCExecLo)
		{
			// ignore  s_wqm_b64     exec, exec
		}
		else
		{
			// TODO:
			// implement
			LOG_PSSL_UNHANDLED_INST();
		}
	}
		
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

}