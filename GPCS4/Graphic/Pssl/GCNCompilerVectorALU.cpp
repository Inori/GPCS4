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
	uint32_t op = getVopOpcode(ins);

	uint32_t src = 0;
	uint32_t dst = 0;
	uint32_t sidx = 0;
	uint32_t didx = 0;
	getVopOperands(ins, &dst, &didx, &src, &sidx);

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
	uint32_t op = getVopOpcode(ins);
	uint32_t src0 = 0;
	uint32_t src1 = 0;
	uint32_t dst = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t dstRIdx = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx);
	
	uint32_t fpTypeId = getScalarTypeId(SpirvScalarType::Float32);

	auto spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, ins.literalConst);
	auto spvSrc1 = emitLoadVectorOperand(src1RIdx);

	SpirvRegisterValue dstVal;
	dstVal.type.ctype = SpirvScalarType::Float32;
	dstVal.type.ccount = 1;
	
	switch (op)
	{
	case SIVOP2Instruction::V_MAC_F32:
	case SIVOP3Instruction::V3_MAC_F32:
	{
		dstVal = emitLoadVectorOperand(dstRIdx);
		dstVal.id = m_module.opFAdd(fpTypeId,
			dstVal.id,
			m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id));
	}
	case SIVOP2Instruction::V_MUL_F32:
	case SIVOP3Instruction::V3_MUL_F32:
	{
		dstVal.id = m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id);
	}
	default:
		break;
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
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
	uint32_t op = getVopOpcode(ins);
	uint32_t src0 = 0;
	uint32_t src1 = 0;
	uint32_t dst = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t dstRIdx = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx);

	switch (op)
	{
	case SIVOP2Instruction::V_CVT_PKRTZ_F16_F32:
	case SIVOP3Instruction::V3_CVT_PKRTZ_F16_F32:
	{

	}
	default:
		break;
	}
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