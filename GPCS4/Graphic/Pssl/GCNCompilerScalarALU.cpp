#include "GCNCompiler.h"
#include "UtilString.h"
#include "GCNParser/SOP1Instruction.h"


LOG_CHANNEL(Graphic.Pssl.GCNCompilerScalarALU);

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

uint32_t GCNCompiler::getSopOpcode(const GCNInstruction& ins)
{
	uint32_t op   = 0;
	auto encoding = ins.instruction->GetInstructionFormat();

	switch (encoding)
	{
	case Instruction::InstructionSet_SOP2:
	{
		auto sop2Ins = asInst<SISOP2Instruction>(ins);
		op           = sop2Ins->GetOp();
	}
		break;
	case Instruction::InstructionSet_SOPK:
	{
		auto sopkIns = asInst<SISOPKInstruction>(ins);
		op           = sopkIns->GetOp();
	}
		break;
	case Instruction::InstructionSet_SOP1:
	{
		auto sop1Ins = asInst<SISOP1Instruction>(ins);
		op           = sop1Ins->GetOp();
	}
		break;
	case Instruction::InstructionSet_SOPC:
	{
		auto sopcIns = asInst<SISOPCInstruction>(ins);
		op           = sopcIns->GetOp();
	}
		break;
	case Instruction::InstructionSet_SOPP:
	{
		auto soppIns = asInst<SISOPPInstruction>(ins);
		op           = soppIns->GetOp();
	}
		break;
	}
	return op;
}

void GCNCompiler::getSopOperands(
	const GCNInstruction& ins, 
	uint32_t* sdst, uint32_t* sdstRidx, 
	uint32_t* src0, uint32_t* src0Ridx, 
	uint32_t* src1 /*= nullptr*/, uint32_t* src1Ridx /*= nullptr*/,
	int16_t* imm /*= nullptr*/)
{
	auto encoding = ins.instruction->GetInstructionFormat();
	switch (encoding)
	{
	case Instruction::InstructionSet_SOP2:
	{
		auto sop2Ins = asInst<SISOP2Instruction>(ins);
		*sdst        = sop2Ins->GetSDST();
		*sdstRidx    = sop2Ins->GetSDSTRidx();
		*src0        = sop2Ins->GetSSRC0();
		*src0Ridx    = sop2Ins->GetSRidx0();
		if (src1) *src1			= sop2Ins->GetSSRC1();
		if (src1Ridx) *src1Ridx = sop2Ins->GetSRidx1();
	}
		break;
	case Instruction::InstructionSet_SOPK:
	{
		auto sopkIns = asInst<SISOPKInstruction>(ins);
		*sdst        = sopkIns->GetSDST();
		*sdstRidx    = sopkIns->GetSDSTRidx();
		if (imm) *imm = sopkIns->GetSIMM16();
	}
		break;
	case Instruction::InstructionSet_SOP1:
	{
		auto sop1Ins = asInst<SISOP1Instruction>(ins);
		*sdst        = sop1Ins->GetSDST();
		*sdstRidx    = sop1Ins->GetSDSTRidx();
		*src0        = sop1Ins->GetSSRC0();
		*src0Ridx    = sop1Ins->GetSRidx();
	}
		break;
	case Instruction::InstructionSet_SOPC:
	{
		auto sopcIns = asInst<SISOPCInstruction>(ins);
		*src0        = sopcIns->GetSSRC0();
		*src0Ridx    = sopcIns->GetSRidx0();
		if (src1) *src1			= sopcIns->GetSSRC1();
		if (src1Ridx) *src1Ridx = sopcIns->GetSRidx1();
	}
		break;
	case Instruction::InstructionSet_SOPP:
	{
		auto soppIns = asInst<SISOPPInstruction>(ins);
		if (imm) *imm = soppIns->GetSIMM16();
	}
		break;
	}
}

void GCNCompiler::emitScalarMov(GCNInstruction& ins)
{
	auto encoding = ins.instruction->GetInstructionFormat();
	switch (encoding)
	{
	case Instruction::InstructionSet_SOP1:
		emitScalarMovSOP1(ins);
		break;
	case Instruction::InstructionSet_SOPK:
		emitScalarMovSOPK(ins);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitScalarMovSOP1(GCNInstruction& ins)
{
	auto inst = asInst<SISOP1Instruction>(ins);
	auto op   = inst->GetOp();

	auto sdst   = inst->GetSDST();
	auto sidx   = inst->GetSRidx();
	auto ssrc   = inst->GetSSRC0();
	auto didx   = inst->GetSDSTRidx();
	auto opType = inst->GetInstructionOperandType();

	SpirvScalarType loadType = getScalarType(opType);
	auto            value    = emitLoadScalarOperand(ssrc, sidx, loadType, ins.literalConst);

	switch (op)
	{
	case SISOP1Instruction::S_MOV_B32:
		break;
	case SISOP1Instruction::S_MOV_B64:
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreScalarOperand(sdst, didx, value);
}

void GCNCompiler::emitScalarMovSOPK(GCNInstruction& ins)
{
	auto inst = asInst<SISOPKInstruction>(ins);
	auto op   = inst->GetOp();

	auto sdst   = inst->GetSDST();
	auto didx   = inst->GetSDSTRidx();
	auto simm   = inst->GetSIMM16Ridx();
	auto opType = inst->GetInstructionOperandType();

	SpirvScalarType loadType = getScalarType(opType);

	SpirvRegisterValue value;
	value.type.ctype = loadType;
	value.type.ccount = 1;
	
	switch (op)
	{
	case SISOPKInstruction::S_MOVK_I32:
		value.id = m_module.consti32(simm);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreScalarOperand(sdst, didx, value);
}

void GCNCompiler::emitScalarArith(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t src1;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	uint32_t src1Ridx;
	int16_t  imm = 0;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx, &src1, &src1Ridx, &imm);

	auto       opType  = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	LOG_ASSERT(dstType == SpirvScalarType::Uint32 || dstType == SpirvScalarType::Sint32, "error operand type");

	SpirvRegisterValue spvSrc0 = {};
	SpirvRegisterValue spvSrc1 = {};
	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_SOP2)
	{
		// Should be SOP2
		spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);
		spvSrc1 = emitLoadScalarOperand(src1, src1Ridx, dstType, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = dstType;
	dstVal.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstVal.type);

	switch (op)
	{
	case SISOP2Instruction::S_ADD_I32:
		dstVal.id = m_module.opIAdd(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SISOP2Instruction::S_MUL_I32:
		dstVal.id = m_module.opIMul(typeId, spvSrc0.id, spvSrc1.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreScalarOperand(sdst, sdstRidx, dstVal);

	// TODO:
	// Update scc
}

void GCNCompiler::emitScalarAbs(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarCmp(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t src1;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	uint32_t src1Ridx;
	int16_t  imm;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx, &src1, &src1Ridx, &imm);

	auto       opType  = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);
	SpirvRegisterValue spvSrc1;
	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_SOP2 ||
		ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_SOPC)
	{
		// Only SOP2 has ssrc1
		spvSrc1 = emitLoadScalarOperand(src1, src1Ridx, dstType, ins.literalConst);
	}

	SpirvRegisterValue condValue;
	condValue.type.ctype  = SpirvScalarType::Bool;
	condValue.type.ccount = 1;

	const uint32_t boolTypeId = getVectorTypeId(condValue.type);

	switch (op)
	{
	case SISOPCInstruction::S_CMP_EQ_I32:
		condValue.id = m_module.opIEqual(boolTypeId,
			spvSrc0.id, spvSrc1.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// Update scc.
	m_stateRegs.scc = condValue;
}

void GCNCompiler::emitScalarSelect(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t src1;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	uint32_t src1Ridx;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx, &src1, &src1Ridx);

	auto       opType  = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);
	SpirvRegisterValue spvSrc1 = emitLoadScalarOperand(src1, src1Ridx, dstType, ins.literalConst);

	SpirvRegisterValue dstValue;
	dstValue.type.ctype  = dstType;
	dstValue.type.ccount = 1;

	const uint32_t dstTypeId = getVectorTypeId(dstValue.type);

	switch (op)
	{
	case SISOP2Instruction::S_CSELECT_B32:
	case SISOP2Instruction::S_CSELECT_B64:
		dstValue.id = m_module.opSelect(dstTypeId,
			m_stateRegs.scc.id, spvSrc0.id, spvSrc1.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	emitStoreScalarOperand(sdst, sdstRidx, dstValue);
}

void GCNCompiler::emitScalarBitLogic(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t src1;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	uint32_t src1Ridx;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx, &src1, &src1Ridx);

	auto opType        = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	LOG_ASSERT(dstType == SpirvScalarType::Uint32 || dstType == SpirvScalarType::Uint64, "error operand type");

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);
	SpirvRegisterValue spvSrc1;
	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_SOP2)
	{
		// Only SOP2 has ssrc1
		spvSrc1 = emitLoadScalarOperand(src1, src1Ridx, dstType, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype = dstType;
	dstVal.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstVal.type);

	switch (op)
	{
	case SISOP2Instruction::S_AND_B32:
	case SISOP2Instruction::S_AND_B64:
		dstVal.id = m_module.opBitwiseAnd(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SISOP2Instruction::S_ANDN2_B64:
		dstVal.id = m_module.opBitwiseAnd(typeId,
										  spvSrc0.id,
										  m_module.opNot(typeId, spvSrc1.id));
		break;
	case SISOP2Instruction::S_OR_B64:
		dstVal.id = m_module.opBitwiseOr(typeId, spvSrc0.id, spvSrc1.id);
		break;
	case SISOP2Instruction::S_NOR_B64:
		dstVal.id = m_module.opNot(typeId, 
			m_module.opBitwiseOr(typeId, spvSrc0.id, spvSrc1.id));
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// Update scc.
	// TODO:
	// Many scalar ALU instructions will update scc on real GPU hardware, not only ScalarBitLogic ones.
	// And implementation of flag register update strategy is a world wide challenge in binary translation domain.
	// Here I only update scc for ScalarBitLogic instructions, and see if this will work.
	m_stateRegs.scc = emitRegisterZeroTest(dstVal, SpirvZeroTest::TestNz);

	emitStoreScalarOperand(sdst, sdstRidx, dstVal);
}

void GCNCompiler::emitScalarBitManip(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t src1;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	uint32_t src1Ridx;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx, &src1, &src1Ridx);

	auto       opType  = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);
	SpirvRegisterValue spvSrc1;
	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_SOP2)
	{
		// Only SOP2 has ssrc1
		spvSrc1 = emitLoadScalarOperand(src1, src1Ridx, dstType, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = dstType;
	dstVal.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstVal.type);

	switch (op)
	{
	case SISOP2Instruction::S_LSHR_B32:
		dstVal.id = m_module.opShiftRightLogical(typeId,
												 spvSrc0.id,
												 m_module.opBitwiseAnd(typeId, spvSrc1.id, m_module.constu32(0b11111)));
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// Update scc.
	// TODO:
	// Many scalar ALU instructions will update scc on real GPU hardware, not only ScalarBitLogic ones.
	// And implementation of flag register update strategy is a world wide challenge in binary translation domain.
	// Here I only update scc for ScalarBitLogic instructions, and see if this will work.
	m_stateRegs.scc = emitRegisterZeroTest(dstVal, SpirvZeroTest::TestNz);

	emitStoreScalarOperand(sdst, sdstRidx, dstVal);
}

void GCNCompiler::emitScalarBitField(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t src1;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	uint32_t src1Ridx;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx, &src1, &src1Ridx);

	auto       opType  = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);
	SpirvRegisterValue spvSrc1;
	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_SOP2)
	{
		// Only SOP2 has ssrc1
		spvSrc1 = emitLoadScalarOperand(src1, src1Ridx, dstType, ins.literalConst);
	}

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = dstType;
	dstVal.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstVal.type);

	switch (op)
	{
	case SISOP2Instruction::S_BFE_U32:
	{
		uint32_t offsetId = m_module.opBitwiseAnd(typeId, spvSrc1.id, 
			m_module.constu32(0b11111));
		uint32_t widthId  = m_module.opBitFieldUExtract(typeId, spvSrc1.id, 
			m_module.constu32(16), m_module.constu32(7));
		dstVal.id         = m_module.opBitFieldUExtract(typeId,
                                                spvSrc0.id, offsetId, widthId);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// Update scc.
	// TODO:
	// Many scalar ALU instructions will update scc on real GPU hardware, not only ScalarBitLogic ones.
	// And implementation of flag register update strategy is a world wide challenge in binary translation domain.
	// Here I only update scc for ScalarBitLogic instructions, and see if this will work.
	m_stateRegs.scc = emitRegisterZeroTest(dstVal, SpirvZeroTest::TestNz);

	emitStoreScalarOperand(sdst, sdstRidx, dstVal);
}

void GCNCompiler::emitScalarConv(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitScalarExecMask(GCNInstruction& ins)
{
	uint32_t op = getSopOpcode(ins);

	uint32_t sdst;
	uint32_t src0;
	uint32_t sdstRidx;
	uint32_t src0Ridx;
	getSopOperands(ins, &sdst, &sdstRidx, &src0, &src0Ridx);

	auto       opType  = ins.instruction->GetInstructionOperandType();
	const auto dstType = getScalarType(opType);

	SpirvRegisterValue spvSrc0 = emitLoadScalarOperand(src0, src0Ridx, dstType, ins.literalConst);

	SpirvRegisterValue dstVal;
	dstVal.type.ctype  = dstType;  // Should be Uint64
	dstVal.type.ccount = 1;

	const uint32_t typeId = getVectorTypeId(dstVal.type);

	// Save exec
	auto exec = m_stateRegs.exec.load(m_module);
	emitStoreScalarOperand(sdst, sdstRidx, exec);

	// Perform mask operations
	switch (op)
	{
	case SISOP1Instruction::S_AND_SAVEEXEC_B64:
		dstVal.id = m_module.opBitwiseAnd(typeId, spvSrc0.id, exec.id);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	// Update exec
	m_stateRegs.exec.store(m_module, dstVal);
	// Update sccz.
	m_stateRegs.scc = emitRegisterZeroTest(dstVal, SpirvZeroTest::TestNz);
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
		if (static_cast<uint32_t>(sdst) == SISOP1Instruction::SSRC::SSRCExecLo)
		{
			// ignore  s_wqm_b64     exec, xxxx
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