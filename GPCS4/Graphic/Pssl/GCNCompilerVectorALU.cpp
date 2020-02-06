#include "GCNCompiler.h"

#include "Platform/UtilString.h"

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

std::vector<SpirvRegisterValue> GCNCompiler::emitVop3InputModifier(
	const GCNInstruction& ins,
	const std::vector<SpirvRegisterValue>& values)
{
	std::vector<SpirvRegisterValue> result(values);

	auto inst = asInst<SIVOP3Instruction>(ins);

	uint32_t neg = inst->GetNEG();
	uint32_t abs = inst->GetABS();

	if (abs)
	{
		for (uint32_t i = 0; i != 3; ++i)
		{
			if (abs & (1 << i))
			{
				result[i] = emitRegisterAbsolute(result[i]);
			}
		}
	}

	if (neg)
	{
		for (uint32_t i = 0; i != 3; ++i)
		{
			if (neg & (1 << i))
			{
				result[i] = emitRegisterNegate(result[i]);
			}
		}
	}

	return result;
}

SpirvRegisterValue GCNCompiler::emitVop3OutputModifier(const GCNInstruction& ins, SpirvRegisterValue value)
{
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
	const GCNInstruction& ins,
	uint32_t srcOperand,
	uint32_t regIndex,
	SpirvScalarType dstType /*= SpirvScalarType::Float32*/)
{
	// For VOP encodings, there will be 2 types of SRC1
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
		operand = emitLoadScalarOperand(srcOperand, regIndex, 0, dstType);  // 64 bit VOP3 won't have literal const
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

	switch (op)
	{
	case SIVOP1Instruction::V_MOV_B32:
	{
		auto value = emitLoadScalarOperand(src, sidx, ins.literalConst, SpirvScalarType::Uint32);
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

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, ins.literalConst, SpirvScalarType::Uint32);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx, SpirvScalarType::Uint32);
	SpirvRegisterValue spvSrc2;
	
	if (isVop3Encoding(ins))
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, ins.literalConst, SpirvScalarType::Uint32);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = SpirvScalarType::Uint32;
	dstVal.type.ccount = 1;

	switch (op)
	{
	case SIVOP3Instruction::V3_CNDMASK_B32:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SIVOP2Instruction::V_CNDMASK_B32:
	{
		auto condVal = emitValueLoad(m_statusRegs.vcc.low());
		auto condition = emitRegisterZeroTest(condVal, SpirvZeroTest::TestNz);
		dstVal.id      = m_module.opSelect(b32TypeId, condition.id, spvSrc1.id, spvSrc0.id);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
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

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0RIdx, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadVopSrc1(ins, src1, src1RIdx);
	SpirvRegisterValue spvSrc2;

	if (isVop3Encoding(ins))
	{
		// Only VOP3 has SRC2
		spvSrc2 = emitLoadScalarOperand(src2, src2RIdx, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = SpirvScalarType::Float32;
	dstVal.type.ccount = 1;

	switch (op)
	{
	case SIVOP3Instruction::V3_MAC_F32:
	case SIVOP2Instruction::V_MAC_F32:
	{
		dstVal    = emitLoadVectorOperand(dstRIdx);
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
	case SIVOP3Instruction::V3_ADD_F32:
	case SIVOP2Instruction::V_ADD_F32:
	{
		dstVal.id = m_module.opFAdd(fpTypeId, spvSrc0.id, spvSrc1.id);
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

	uint32_t src0     = 0;
	uint32_t vdst     = 0;
	uint32_t src0Ridx = 0;
	uint32_t vdstRidx = 0;
	getVopOperands(ins, &vdst, &vdstRidx, &src0, &src0Ridx);

	auto spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, ins.literalConst);

	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = SpirvScalarType::Float32;
	dstValue.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstValue.type);

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		auto srcGroup = emitVop3InputModifier(ins, { spvSrc0 });
		spvSrc0       = srcGroup[0];
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

	auto spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, ins.literalConst);
	auto spvSrc1 = emitLoadVopSrc1(ins, src1, src1Ridx);

	bool isVop3 = isVop3Encoding(ins);
	if (isVop3)
	{
		auto srcGroup = emitVop3InputModifier(ins, { spvSrc0, spvSrc1 });
		spvSrc0       = srcGroup[0];
		spvSrc1       = srcGroup[1];
	}

	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = SpirvScalarType::Bool;
	dstValue.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstValue.type);

	// For VectorFpCmp32 class, VOPC and VOP3 instructions use same opcode values,
	// ie. SIVOPCInstruction::V_CMP_LT_F32 == SIVOP3Instruction::V3_CMP_LT_F32 ,
	// so we don't need to switch both cases.
	switch (op)
	{
	case SIVOPCInstruction::V_CMP_LT_F32:
		dstValue.id = m_module.opFOrdLessThan(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SIVOPCInstruction::V_CMP_GE_F32:
		dstValue.id = m_module.opFOrdGreaterThanEqual(typeId, spvSrc0.id, spvSrc1.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (isVop3)
	{
		dstValue = emitVop3OutputModifier(ins, dstValue);
	}

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
	uint32_t op       = getVopOpcode(ins);
	uint32_t src0     = 0;
	uint32_t src1     = 0;
	uint32_t dst      = 0;
	uint32_t src0RIdx = 0;
	uint32_t src1RIdx = 0;
	uint32_t dstRIdx  = 0;
	getVopOperands(ins, &dst, &dstRIdx, &src0, &src0RIdx, &src1, &src1RIdx);

	auto value0 = emitLoadScalarOperand(src0, src0RIdx, ins.literalConst);

	SpirvRegisterValue dstValue;

	switch (op)
	{
	case SIVOP2Instruction::V_CVT_PKRTZ_F16_F32:
	{
		auto value1 = emitLoadVectorOperand(src1RIdx);
		dstValue    = emitPackFloat16(
            emitRegisterConcat(value0, value1));
	}
	break;
	case SIVOP3Instruction::V3_CVT_PKRTZ_F16_F32:
	{
		auto value1 = emitLoadScalarOperand(src1, src1RIdx, ins.literalConst);
		dstValue    = emitPackFloat16(
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
		uint32_t varId      = m_module.newVar(u32PtrType, spv::StorageClassPrivate);
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