#include "GCNCompiler.h"
#include "Platform/UtilString.h"

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
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitVectorLane(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorBitLogic(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorBitField32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorThreadMask(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorBitField64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpArith32(GCNInstruction& ins)
{
	uint32_t op = getVopOpcode(ins);
	uint32_t src0 = 0;
	uint32_t src1 = 0;
	uint32_t src2 = 0;
	uint32_t dst = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t src2RIdx = 0;
	uint32_t dstRIdx = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx, &src2, &src2RIdx);
	
	uint32_t fpTypeId = getScalarTypeId(SpirvScalarType::Float32);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, ins.literalConst);
	SpirvRegisterValue spvSrc1;
	SpirvRegisterValue spvSrc2;

	// For SRC1, there are two types, VOP2/VOPC use 8 bits VSRC,
	// VOP3 use 9 bits SRC. 
	// We need to select proper src1 for different encodings.
	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_VOP3)
	{
		spvSrc1 = emitLoadScalarOperand(src1, src1RIdx, ins.literalConst);
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, ins.literalConst);
	}
	else
	{
		spvSrc1 = emitLoadVectorOperand(src1RIdx);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype = SpirvScalarType::Float32;
	dstVal.type.ccount = 1;
	
	switch (op)
	{
	case SIVOP3Instruction::V3_MAC_F32:
	case SIVOP2Instruction::V_MAC_F32:
	{
		dstVal = emitLoadVectorOperand(dstRIdx);
		dstVal.id = m_module.opFAdd(fpTypeId,
			dstVal.id,
			m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id));
	}
		break;
	case SIVOP3Instruction::V3_MUL_F32:
	case SIVOP2Instruction::V_MUL_F32:
	{
		dstVal.id = m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id);
	}
		break;
	case SIVOP3Instruction::V3_MAD_F32:
	{
		dstVal.id = m_module.opFAdd(fpTypeId,
			spvSrc2.id,
			m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id));
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
}

void GCNCompiler::emitVectorFpRound32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpField32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpTran32(GCNInstruction& ins)
{
	auto op = getVopOpcode(ins);
	
	uint32_t src0 = 0;
	uint32_t vdst = 0;
	uint32_t src0Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx);

	auto spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, ins.literalConst);

	SpirvRegisterValue dstValue;
	dstValue.type.ctype = SpirvScalarType::Float32;
	dstValue.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstValue.type);

	bool isVop3 = ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_VOP3;
	if (isVop3)
	{
		spvSrc0 = emitVop3InputModifier(ins, spvSrc0);
	}

	switch (op)
	{
	case SIVOP3Instruction::V3_LOG_F32:
	case SIVOP1Instruction::V_LOG_F32:
		dstValue.id = m_module.opLog2(typeId, spvSrc0.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (isVop3)
	{
		dstValue = emitVop3OutputModifier(ins, dstValue);
	}

	emitStoreVectorOperand(vdstRidx, dstValue);
}

void GCNCompiler::emitVectorFpCmp32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpArith64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpRound64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpField64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpTran64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpCmp64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntArith32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntArith64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntCmp32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntCmp64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
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

	auto value0 = emitLoadScalarOperand(src0, src0RIdx, ins.literalConst);

	SpirvRegisterValue dstValue;

	switch (op)
	{
	case SIVOP2Instruction::V_CVT_PKRTZ_F16_F32:
	{
		auto value1 = emitLoadVectorOperand(src1RIdx);
		dstValue = emitPackFloat16(
			emitRegisterConcat(value0, value1));
	}
		break;
	case SIVOP3Instruction::V3_CVT_PKRTZ_F16_F32:
	{
		auto value1 = emitLoadScalarOperand(src1, src1RIdx, ins.literalConst);
		dstValue = emitPackFloat16(
			emitRegisterConcat(value0, value1));
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// v/sgprs store as float32 type by default,
	// here we need to new a proper type variable.
	if (m_vgprs[dstRIdx].type.ctype != dstValue.type.ctype)
	{
		uint32_t u32PtrType = m_module.defPointerType(getVectorTypeId(dstValue.type), spv::StorageClassPrivate);
		uint32_t varId = m_module.newVar(u32PtrType, spv::StorageClassPrivate);
		m_module.setDebugName(varId, UtilString::Format("v_%d", dstRIdx).c_str());
		m_vgprs[dstRIdx] = SpirvRegisterPointer(dstValue.type, varId);
	}

	emitStoreVectorOperand(dstRIdx, dstValue);
}

void GCNCompiler::emitVectorFpGraph32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntGraph(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorMisc(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

}  // namespace pssl