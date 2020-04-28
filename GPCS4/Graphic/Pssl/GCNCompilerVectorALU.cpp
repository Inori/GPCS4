#include "GCNCompiler.h"

#include "UtilString.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerVectorALU);

namespace pssl
{
;

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

void GCNCompiler::emitVop3InputModifier(
	const GCNInstruction& ins,
	const std::vector<std::reference_wrapper<SpirvRegisterValue>>& values)
{
	// Input modifiers are only supported for floating point inputs


	auto inst = asInst<SIVOP3Instruction>(ins);

	uint32_t neg = inst->GetNEG();
	uint32_t abs = inst->GetABS();

	if (abs)
	{
		for (uint32_t i = 0; i != 3; ++i)
		{
			if (abs & (1 << i))
			{
				values[i].get() = emitRegisterAbsolute(values[i]);
			}
		}
	}

	if (neg)
	{
		for (uint32_t i = 0; i != 3; ++i)
		{
			if (neg & (1 << i))
			{
				values[i].get() = emitRegisterNegate(values[i]);
			}
		}
	}
}

SpirvRegisterValue GCNCompiler::emitVop3OutputModifier(const GCNInstruction& ins, const SpirvRegisterValue& value)
{
	// Integer and non-specific instructions (such as moves) ignore output modifiers.

	SpirvRegisterValue result = value;

	auto inst = asInst<SIVOP3Instruction>(ins);

	uint32_t omod         = inst->GetOMOD();
	uint32_t clmp         = inst->GetCLMP();
	const uint32_t typeId = getVectorTypeId(result.type);

	if (omod != 0)
	{
		float mul = 0.0;
		switch (omod)
		{
		case 1:
			mul = 2.0;
			break;
		case 2:
			mul = 4.0;
			break;
		case 3:
			mul = 0.5;
			break;
		}

		uint32_t mulId = m_module.constf32(mul);
		result.id      = m_module.opFMul(typeId, result.id, mulId);
	}

	if (clmp)
	{
		result.id = m_module.opFClamp(
			typeId,
			result.id,
			m_module.constf32(0.0f),
			m_module.constf32(1.0f));
	}

	return result;
}

SpirvRegisterValue GCNCompiler::emitLoadVopSrc1(
	const GCNInstruction&	ins,
	uint32_t				srcOperand,
	uint32_t				regIndex,
	SpirvScalarType			dstType)
{
	// For VOP encodings, there will be 2 types of SRC1:
	// VOP2 and VOPC use 8bits VSRC1
	// VOP3 use 9 bits SRC1
	// other VOP encodings don't have SRC1 field.

	auto encoding = ins.instruction->GetInstructionFormat();

	SpirvRegisterValue operand;
	switch (encoding)
	{
	case Instruction::InstructionSet_VOP2:
	case Instruction::InstructionSet_VOPC:
		operand = emitLoadVectorOperand(regIndex, dstType);
		break;
	case Instruction::InstructionSet_VOP3:
		operand = emitLoadScalarOperand(srcOperand, regIndex, dstType, 0);  // 64 bit VOP3 won't have literal const
		break;
	}
	return operand;
}

bool GCNCompiler::isVop3Encoding(const GCNInstruction& ins)
{
	return ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_VOP3;
}

uint32_t GCNCompiler::getVopOpcode(GCNInstruction& ins)
{
	uint32_t op   = 0;
	auto encoding = ins.instruction->GetInstructionFormat();
	switch (encoding)
	{
	case Instruction::InstructionSet_VOP1:
	{
		auto vop1Ins = asInst<SIVOP1Instruction>(ins);
		op           = vop1Ins->GetOp();
	}
	break;
	case Instruction::InstructionSet_VOP2:
	{
		auto vop2Ins = asInst<SIVOP2Instruction>(ins);
		op           = vop2Ins->GetOp();
	}
	break;
	case Instruction::InstructionSet_VOP3:
	{
		auto vop3Ins = asInst<SIVOP3Instruction>(ins);
		op           = vop3Ins->GetOp();
	}
	break;
	case Instruction::InstructionSet_VOPC:
	{
		auto vopcIns = asInst<SIVOPCInstruction>(ins);
		op           = vopcIns->GetOp();
	}
	break;
	default:
		break;
	}
	return op;
}

void GCNCompiler::getVopOperands(
	GCNInstruction& ins,
	uint32_t* vdst, uint32_t* vdstRidx,
	uint32_t* src0, uint32_t* src0Ridx,
	uint32_t* src1 /*= nullptr*/, uint32_t* src1Ridx /*= nullptr*/,
	uint32_t* src2 /*= nullptr*/, uint32_t* src2Ridx /*= nullptr*/,
	uint32_t* sdst /*= nullptr*/, uint32_t* sdstRidx /*= nullptr*/)
{
	auto encoding = ins.instruction->GetInstructionFormat();
	switch (encoding)
	{
	case Instruction::InstructionSet_VOP1:
	{
		auto vop1Ins = asInst<SIVOP1Instruction>(ins);
		*vdst        = vop1Ins->GetVDST();
		*vdstRidx    = vop1Ins->GetVDSTRidx();
		*src0        = vop1Ins->GetSRC0();
		*src0Ridx    = vop1Ins->GetSRidx0();
	}
		break;
	case Instruction::InstructionSet_VOP2:
	{
		auto vop2Ins = asInst<SIVOP2Instruction>(ins);
		*vdst        = vop2Ins->GetVDST();
		*vdstRidx    = vop2Ins->GetVDSTRidx();
		*src0        = vop2Ins->GetSRC0();
		*src0Ridx    = vop2Ins->GetSRidx0();
		if (src1) *src1			= vop2Ins->GetVSRC1();
		if (src1Ridx) *src1Ridx = vop2Ins->GetVRidx1();
	}
		break;
	case Instruction::InstructionSet_VOP3:
	{
		auto vop3Ins = asInst<SIVOP3Instruction>(ins);
		*vdst        = vop3Ins->GetVDST();
		*vdstRidx    = vop3Ins->GetVDSTRidx();
		*src0        = vop3Ins->GetSRC0();
		*src0Ridx				= vop3Ins->GetSRidx0();
		if (src1) *src1			= vop3Ins->GetSRC1();
		if (src1Ridx) *src1Ridx = vop3Ins->GetRidx1();
		if (src2) *src2			= vop3Ins->GetSRC2();
		if (src2Ridx) *src2Ridx = vop3Ins->GetRidx2();
		if (sdst) *sdst			= vop3Ins->GetSDST();
		if (sdstRidx) *sdstRidx = vop3Ins->GetSDSTRidx();
	}
		break;
	case Instruction::InstructionSet_VOPC:
	{
		auto vopcIns = asInst<SIVOPCInstruction>(ins);
		// VOPC doesn't have vdst
		*src0					= vopcIns->GetSRC0();
		*src0Ridx				= vopcIns->GetSRidx0();
		if (src1) *src1			= vopcIns->GetVSRC1();
		if (src1Ridx) *src1Ridx = vopcIns->GetVRidx1();
	}
		break;
	default:
		break;
	}
}

void GCNCompiler::emitVectorRegMov(GCNInstruction& ins)
{
	uint32_t op = getVopOpcode(ins);

	uint32_t src  = 0;
	uint32_t dst  = 0;
	uint32_t sidx = 0;
	uint32_t didx = 0;
	getVopOperands(ins, &dst, &didx, &src, &sidx);

	SpirvScalarType type   = getScalarType(ins.instruction->GetInstructionOperandType());
	auto            spvSrc = emitLoadScalarOperand(src, sidx, type, ins.literalConst);

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3 && spvSrc.type.ctype == SpirvScalarType::Float32)
	{
		// As special cases, the untyped operations v_mov_b32, v_movreld_b32, v_movrels_b32,
		// v_movrelsd_b32 and v_cndmask_b32 also support input modifiers when applied to input floating point values
		emitVop3InputModifier(ins, { spvSrc });
	}

	SpirvRegisterValue dstVal;

	switch (op)
	{
	case SIVOP1Instruction::V_MOV_B32:
	case SIVOP3Instruction::V3_MOV_B32:
	{
		dstVal = spvSrc;
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// Integer and non-specific instructions (such as moves) ignore output modifiers.

	emitStoreVectorOperand(didx, dstVal);
}

void GCNCompiler::emitVectorLane(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorBitLogic(GCNInstruction& ins)
{
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t src2RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx, &src2, &src2RIdx);

	uint32_t b32TypeId = getScalarTypeId(SpirvScalarType::Uint32);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Uint32, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, SpirvScalarType::Uint32);
	SpirvRegisterValue spvSrc2;

	if (isVop3Encoding(ins))
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, SpirvScalarType::Uint32, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = SpirvScalarType::Uint32;
	dstVal.type.ccount = 1;

	switch (op)
	{
	case SIVOP2Instruction::V_AND_B32:
	case SIVOP3Instruction::V3_AND_B32:
	{
		dstVal.id = m_module.opBitwiseAnd(b32TypeId, spvSrc0.id, spvSrc1.id);
	}
	break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
}


void GCNCompiler::emitVectorBitField32(GCNInstruction& ins)
{
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t src2RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx, &src2, &src2RIdx);

	uint32_t b32TypeId = getScalarTypeId(SpirvScalarType::Uint32);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Uint32, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, SpirvScalarType::Uint32);
	SpirvRegisterValue spvSrc2;

	if (isVop3Encoding(ins))
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, SpirvScalarType::Uint32, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = SpirvScalarType::Uint32;
	dstVal.type.ccount = 1;

	switch (op)
	{
	case SIVOP3Instruction::V3_BFE_U32:
	{
		uint32_t count  = m_module.opBitwiseAnd(b32TypeId, spvSrc2.id, m_module.constu32(0x1F));
		uint32_t offset = m_module.opBitwiseAnd(b32TypeId, spvSrc1.id, m_module.constu32(0x1F));
		dstVal.id       = m_module.opBitFieldUExtract(b32TypeId, spvSrc0.id, offset, count);
	}
		break;
	case SIVOP2Instruction::V_LSHR_B32:
	case SIVOP3Instruction::V3_LSHR_B32:
		dstVal.id = m_module.opShiftRightLogical(b32TypeId, spvSrc0.id,
												 m_module.opBitwiseAnd(b32TypeId, spvSrc1.id, m_module.constu32(0b11111)));
		break;
	case SIVOP2Instruction::V_LSHLREV_B32:
	case SIVOP3Instruction::V3_LSHLREV_B32:
		dstVal.id = m_module.opShiftLeftLogical(b32TypeId, spvSrc1.id,
												m_module.opBitwiseAnd(b32TypeId, spvSrc0.id, m_module.constu32(0b11111)));
		break;
	case SIVOP2Instruction::V_LSHRREV_B32:
	case SIVOP3Instruction::V3_LSHRREV_B32:
		dstVal.id = m_module.opShiftRightLogical(b32TypeId, spvSrc1.id,
												m_module.opBitwiseAnd(b32TypeId, spvSrc0.id, m_module.constu32(0b11111)));
		break;
	case SIVOP2Instruction::V_ASHRREV_I32:
	case SIVOP3Instruction::V3_ASHRREV_I32:
		dstVal.id = m_module.opShiftRightArithmetic(b32TypeId, spvSrc1.id,
												 m_module.opBitwiseAnd(b32TypeId, spvSrc0.id, m_module.constu32(0b11111)));
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
}

void GCNCompiler::emitVectorThreadMask(GCNInstruction& ins)
{
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t src2RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx, &src2, &src2RIdx);

	SpirvScalarType    type    = getScalarType(ins.instruction->GetInstructionOperandType());
	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, type, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, type);
	SpirvRegisterValue spvSrc2;
	
	uint32_t typeId = getScalarTypeId(spvSrc0.type.ctype);

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, type, ins.literalConst);
		emitVop3InputModifier(ins, { spvSrc0, spvSrc1, spvSrc2 });
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = spvSrc0.type.ctype;
	dstVal.type.ccount = 1;

	switch (op)
	{
	case SIVOP3Instruction::V3_CNDMASK_B32:
		LOG_ASSERT(static_cast<Instruction::OperandSRC>(src2) == Instruction::OperandSRC::SRCVccLo, "V3_CNDMASK_B32 smask not vcc.");
	case SIVOP2Instruction::V_CNDMASK_B32:
	{
		auto condVal   = emitValueLoad(m_stateRegs.vcc.lo);
		auto condition = emitRegisterZeroTest(condVal, SpirvZeroTest::TestNz);
		dstVal.id      = m_module.opSelect(typeId, condition.id, spvSrc1.id, spvSrc0.id);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (isVop3)
	{
		dstVal = emitVop3OutputModifier(ins, dstVal);
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
}

void GCNCompiler::emitVectorBitField64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpArith32(GCNInstruction& ins)
{
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t src2RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx, &src2, &src2RIdx);

	uint32_t fpTypeId = getScalarTypeId(SpirvScalarType::Float32);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Float32, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, SpirvScalarType::Float32);
	SpirvRegisterValue spvSrc2;

	if (isVop3Encoding(ins))
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, SpirvScalarType::Float32, ins.literalConst);
	}

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		emitVop3InputModifier(ins, { spvSrc0, spvSrc1, spvSrc2 });
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = SpirvScalarType::Float32;
	dstVal.type.ccount = 1;

	switch (op)
	{
	case SIVOP3Instruction::V3_MAC_F32:
	case SIVOP2Instruction::V_MAC_F32:
	{
		dstVal    = emitLoadVectorOperand(dstRIdx, SpirvScalarType::Float32);
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
	case SIVOP2Instruction::V_MADAK_F32:
	{
		float constVal = *reinterpret_cast<float*>(&ins.literalConst);
		uint32_t constId = m_module.constf32(constVal);
		dstVal.id = m_module.opFAdd(fpTypeId,
                                    constId,
									m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id));
	}
		break;
	case SIVOP2Instruction::V_MADMK_F32:
	{
		float constVal   = *reinterpret_cast<float*>(&ins.literalConst);
		uint32_t constId = m_module.constf32(constVal);
		dstVal.id        = m_module.opFAdd(fpTypeId,
                                    spvSrc1.id,
                                    m_module.opFMul(fpTypeId, spvSrc0.id, constId));
	}
		break;
	case SIVOP3Instruction::V3_ADD_F32:
	case SIVOP2Instruction::V_ADD_F32:
	{
		dstVal.id = m_module.opFAdd(fpTypeId, spvSrc0.id, spvSrc1.id);
	}
		break;
	case SIVOP3Instruction::V3_FMA_F32:
	{
		dstVal.id = m_module.opFAdd(fpTypeId,
									spvSrc2.id,
									m_module.opFMul(fpTypeId, spvSrc0.id, spvSrc1.id));
	}
		break;
	case SIVOP2Instruction::V_SUBREV_F32:
	case SIVOP3Instruction::V3_SUBREV_F32:
	{
		dstVal.id = m_module.opFSub(fpTypeId, spvSrc1.id, spvSrc0.id);
	}
		break;
	case SIVOP2Instruction::V_SUB_F32:
	case SIVOP3Instruction::V3_SUB_F32:
	{
		dstVal.id = m_module.opFSub(fpTypeId, spvSrc0.id, spvSrc1.id);
	}
		break;
	case SIVOP2Instruction::V_MAX_F32:
	case SIVOP3Instruction::V3_MAX_F32:
	{
		dstVal.id = m_module.opNMax(fpTypeId, spvSrc0.id, spvSrc1.id);
	}
		break;
	case SIVOP2Instruction::V_MIN_F32:
	case SIVOP3Instruction::V3_MIN_F32:
	{
		dstVal.id = m_module.opFMin(fpTypeId, spvSrc0.id, spvSrc1.id);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (isVop3)
	{
		dstVal = emitVop3OutputModifier(ins, dstVal);
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
}

void GCNCompiler::emitVectorFpRound32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorFpField32(GCNInstruction& ins)
{
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t src2RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx, &src2, &src2RIdx);

	uint32_t fpTypeId = getScalarTypeId(SpirvScalarType::Float32);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Float32, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, SpirvScalarType::Float32);
	SpirvRegisterValue spvSrc2;

	if (isVop3Encoding(ins))
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, SpirvScalarType::Float32, ins.literalConst);
	}

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		emitVop3InputModifier(ins, { spvSrc0, spvSrc1, spvSrc2 });
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = SpirvScalarType::Float32;
	dstVal.type.ccount = 1;

	switch (op)
	{
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (isVop3)
	{
		dstVal = emitVop3OutputModifier(ins, dstVal);
	}

	emitStoreVectorOperand(dstRIdx, dstVal);
}

void GCNCompiler::emitVectorFpTran32(GCNInstruction& ins)
{
	auto op = getVopOpcode(ins);

	uint32_t src0     = 0;
	uint32_t vdst     = 0;
	uint32_t src0Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx);

	auto spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, SpirvScalarType::Float32, ins.literalConst);

	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = SpirvScalarType::Float32;
	dstValue.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstValue.type);

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		emitVop3InputModifier(ins, { spvSrc0 });
	}

	switch (op)
	{
	case SIVOP1Instruction::V_LOG_F32:
	case SIVOP3Instruction::V3_LOG_F32:
		dstValue.id = m_module.opLog2(typeId, spvSrc0.id);
		break;
	case SIVOP1Instruction::V_EXP_F32:
	case SIVOP3Instruction::V3_EXP_F32:
		dstValue.id = m_module.opExp2(typeId, spvSrc0.id);
		break;
	case SIVOP1Instruction::V_RCP_F32:
	case SIVOP3Instruction::V3_RCP_F32:
		dstValue.id = m_module.opFDiv(typeId, m_module.constf32(1.0f), spvSrc0.id);
		break;
	case SIVOP1Instruction::V_RSQ_F32:
	case SIVOP3Instruction::V3_RSQ_F32:
		dstValue.id = m_module.opFDiv(typeId,
									m_module.constf32(1.0),
									m_module.opSqrt(typeId, spvSrc0.id));
		break;
	case SIVOP1Instruction::V_SQRT_F32:
	case SIVOP3Instruction::V3_SQRT_F32:
		dstValue.id = m_module.opSqrt(typeId, spvSrc0.id);
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
	auto op = getVopOpcode(ins);

	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t vdst     = 0;
	uint32_t src0Ridx = 0;
	uint32_t src1Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx, &src1, &src1Ridx);

	auto spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, SpirvScalarType::Float32, ins.literalConst);
	auto spvSrc1 = emitLoadVopSrc1(ins, src1, src1Ridx, SpirvScalarType::Float32);

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		emitVop3InputModifier(ins, { spvSrc0, spvSrc1 });
	}

	const uint32_t typeId = getScalarTypeId(SpirvScalarType::Bool);
	uint32_t conditionId  = InvalidSpvId;
	// For VectorFpCmp32 class, VOPC and VOP3 instructions use same opcode values,
	// ie. SIVOPCInstruction::V_CMP_LT_F32 == SIVOP3Instruction::V3_CMP_LT_F32 ,
	// so we don't need to switch both cases.
	switch (op)
	{
	case SIVOPCInstruction::V_CMP_LT_F32:
		conditionId = m_module.opFOrdLessThan(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_GE_F32:
		conditionId = m_module.opFOrdGreaterThanEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_LE_F32:
		conditionId = m_module.opFOrdLessThanEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_EQ_F32:
		conditionId = m_module.opFOrdEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_NEQ_F32:
		conditionId = m_module.opFOrdNotEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	SpirvRegisterValue dstValue;
	dstValue.type.ctype       = SpirvScalarType::Uint32;
	dstValue.type.ccount      = 1;

	const uint32_t uintTypeId = getVectorTypeId(dstValue.type);
	dstValue.id               = m_module.opSelect(uintTypeId, conditionId,
												  m_module.constu32(1), m_module.constu32(0));

	// V_CMP_X output are all uint types, no output modifier

	if (!isVop3)
	{
		// 32-bit VOPC encoding requires sdst to be VCC, vsrc1 to be a VGPR, and no input modifiers.
		vdst = static_cast<uint32_t>(Instruction::OperandSDST::SDSTVccLo);
	}
	emitStoreScalarOperand(vdst, vdstRidx, dstValue);
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
	auto op = getVopOpcode(ins);

	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t vdst     = 0;
	uint32_t src0Ridx = 0;
	uint32_t src1Ridx = 0;
	uint32_t src2Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx, &src1, &src1Ridx, &src2, &src2Ridx);

	SpirvScalarType opType    = getScalarType(ins.instruction->GetInstructionOperandType());
	// Uint32 or Sint32
	const uint32_t  i32TypeId = getScalarTypeId(opType);  

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, opType, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1Ridx, opType);
	SpirvRegisterValue spvSrc2;

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		spvSrc2 = emitLoadScalarOperand(src2, src2Ridx, opType, ins.literalConst);
	}

	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = opType;
	dstValue.type.ccount = 1;


	switch (op)
	{
	case SIVOP2Instruction::V_ADD_I32:
	case SIVOP3Instruction::V3_ADD_I32:
		dstValue.id = m_module.opIAdd(i32TypeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOP2Instruction::V_SUB_I32:
	case SIVOP3Instruction::V3_SUB_I32:
		dstValue.id = m_module.opISub(i32TypeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOP2Instruction::V_SUBREV_I32:
	case SIVOP3Instruction::V3_SUBREV_I32:
		dstValue.id = m_module.opISub(i32TypeId, spvSrc1.id, spvSrc0.id);
		break;
	case SIVOP3Instruction::V3_MUL_LO_I32:
		dstValue.id = m_module.opIMul(i32TypeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOP3Instruction::V3_MAD_I32_I24:
	case SIVOP3Instruction::V3_MAD_U32_U24:
	{
		uint32_t arg0Id = m_module.opBitwiseAnd(i32TypeId, spvSrc0.id, m_module.constu32(0x00FFFFFF));
		uint32_t arg1Id = m_module.opBitwiseAnd(i32TypeId, spvSrc1.id, m_module.constu32(0x00FFFFFF));
		uint32_t mulId  = m_module.opIMul(i32TypeId, arg0Id, arg1Id);
		dstValue.id     = m_module.opIAdd(i32TypeId, mulId, spvSrc2.id);
	}
		break;
	case SIVOP2Instruction::V_MUL_I32_I24:
	case SIVOP3Instruction::V3_MUL_I32_I24:
	{
		uint32_t arg0Id = m_module.opBitwiseAnd(i32TypeId, spvSrc0.id, m_module.constu32(0x00FFFFFF));
		uint32_t arg1Id = m_module.opBitwiseAnd(i32TypeId, spvSrc1.id, m_module.constu32(0x00FFFFFF));
		dstValue.id = m_module.opIMul(i32TypeId, arg0Id, arg1Id);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreVectorOperand(vdstRidx, dstValue);
	// TODO:
	// Here we need to update VCC or SDST of VOP3

}

void GCNCompiler::emitVectorIntArith64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntCmp32(GCNInstruction& ins)
{
	auto op = getVopOpcode(ins);

	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t vdst     = 0;
	uint32_t src0Ridx = 0;
	uint32_t src1Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx, &src1, &src1Ridx);

	SpirvScalarType opType  = getScalarType(ins.instruction->GetInstructionOperandType());
	auto            spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, opType, ins.literalConst);
	auto            spvSrc1 = emitLoadVopSrc1(ins, src1, src1Ridx, opType);

	const uint32_t typeId      = getScalarTypeId(SpirvScalarType::Bool);
	uint32_t       conditionId = InvalidSpvId;
	bool           writeExec   = false;
	// For VectorIntCmp32 class, VOPC and VOP3 instructions use same opcode values,
	// ie. SIVOPCInstruction::V_CMP_LE_U32 == SIVOP3Instruction::V3_CMP_LE_U32 ,
	// so we don't need to switch both cases.
	switch (op)
	{
	case SIVOPCInstruction::V_CMP_LE_I32:
	case SIVOPCInstruction::V_CMP_LE_U32:
		conditionId = m_module.opULessThanEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_LT_I32:
	case SIVOPCInstruction::V_CMP_LT_U32:
		conditionId = m_module.opULessThan(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMPX_NE_U32:
		writeExec = true;
	case SIVOPCInstruction::V_CMP_NE_U32:
		conditionId = m_module.opINotEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_EQ_I32:
		conditionId = m_module.opIEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMPX_GT_I32:
	case SIVOPCInstruction::V_CMPX_GT_U32:
		writeExec = true;
	case SIVOPCInstruction::V_CMP_GT_I32:
	case SIVOPCInstruction::V_CMP_GT_U32:
		conditionId = m_module.opSGreaterThan(typeId, spvSrc0.id, spvSrc1.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = SpirvScalarType::Uint64;
	dstValue.type.ccount = 1;

	const uint32_t uintTypeId = getVectorTypeId(dstValue.type);
	dstValue.id               = m_module.opSelect(uintTypeId, conditionId,
                                    m_module.constu64(1), m_module.constu64(0));

	// V_CMP_X output are all uint types, no output modifier
	bool isVop3 = isVop3Encoding(ins);
	if (!isVop3)
	{
		// 32-bit VOPC encoding requires sdst to be VCC, vsrc1 to be a VGPR, and no input modifiers.
		vdst = static_cast<uint32_t>(Instruction::OperandSDST::SDSTVccLo);
	}

	if (writeExec)
	{
		uint32_t execOprand = static_cast<uint32_t>(Instruction::OperandSDST::SDSTExecLo);
		emitStoreScalarOperand(execOprand, 0, dstValue);
	}

	emitStoreScalarOperand(vdst, vdstRidx, dstValue);
}

void GCNCompiler::emitVectorIntCmp64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorConv(GCNInstruction& ins)
{
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx);

	SpirvRegisterValue dstValue;
	switch (op)
	{
	case SIVOP2Instruction::V_CVT_PKRTZ_F16_F32:
	case SIVOP3Instruction::V3_CVT_PKRTZ_F16_F32:
	{
		auto spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Float32, ins.literalConst);
		auto spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, SpirvScalarType::Float32);
		dstValue     = emitPackFloat16(
            emitRegisterConcat(spvSrc0, spvSrc1));
	}
	break;
	case SIVOP1Instruction::V_CVT_F32_U32:
	case SIVOP3Instruction::V3_CVT_F32_U32:
	{
		auto spvSrc0         = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Uint32, ins.literalConst);
		dstValue.type.ctype  = SpirvScalarType::Float32;
		dstValue.type.ccount = 1;
		dstValue.id          = m_module.opConvertUtoF(getScalarTypeId(SpirvScalarType::Float32), spvSrc0.id);
	}
	break;
	case SIVOP1Instruction::V_CVT_F32_I32:
	case SIVOP3Instruction::V3_CVT_F32_I32:
	{
		auto spvSrc0         = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Sint32, ins.literalConst);
		dstValue.type.ctype  = SpirvScalarType::Float32;
		dstValue.type.ccount = 1;
		dstValue.id          = m_module.opConvertStoF(getScalarTypeId(SpirvScalarType::Float32), spvSrc0.id);
	}
	break;
	case SIVOP1Instruction::V_CVT_U32_F32:
	case SIVOP3Instruction::V3_CVT_U32_F32:
	{
		auto spvSrc0         = emitLoadScalarOperand(src0, src0RIdx, SpirvScalarType::Float32, ins.literalConst);
		dstValue.type.ctype  = SpirvScalarType::Uint32;
		dstValue.type.ccount = 1;
		dstValue.id          = m_module.opConvertFtoU(getScalarTypeId(SpirvScalarType::Uint32), spvSrc0.id);
	}
	break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreVectorOperand(dstRIdx, dstValue);
}

void GCNCompiler::emitVectorFpGraph32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorIntGraph(GCNInstruction& ins)
{
	auto op = getVopOpcode(ins);

	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t src2     = 0;
	uint32_t vdst     = 0;
	uint32_t src0Ridx = 0;
	uint32_t src1Ridx = 0;
	uint32_t src2Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx, &src1, &src1Ridx, &src2, &src2Ridx);

	SpirvScalarType opType  = getScalarType(ins.instruction->GetInstructionOperandType());
	const uint32_t  typeId  = getScalarTypeId(opType);

	auto            spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, opType, ins.literalConst);
	auto            spvSrc1 = emitLoadVopSrc1(ins, src1, src1Ridx, opType);
	auto            spvSrc2 = emitLoadScalarOperand(src2, src2Ridx, opType, ins.literalConst);

	
	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = opType;
	dstValue.type.ccount = 1;

	switch (op)
	{
	case SIVOP3Instruction::V3_SAD_U32:
	{
		// TODO:
		// Do we have a better way?
		uint32_t conditionId = m_module.opUGreaterThan(
			getScalarTypeId(SpirvScalarType::Bool),
			spvSrc0.id, spvSrc1.id);
		uint32_t absDiffId = m_module.opSelect(typeId, conditionId,
											   m_module.opISub(typeId, spvSrc0.id, spvSrc1.id),
											   m_module.opISub(typeId, spvSrc1.id, spvSrc0.id));
		dstValue.id        = m_module.opIAdd(typeId, absDiffId, spvSrc2.id);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreVectorOperand(vdstRidx, dstValue);
}

void GCNCompiler::emitVectorMisc(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

}  // namespace pssl
