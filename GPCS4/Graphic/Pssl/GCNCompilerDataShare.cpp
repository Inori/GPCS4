#include "GCNCompiler.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerDataShare);

namespace pssl
{;

void GCNCompiler::emitDataShare(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::DsIdxRd:
		emitDsIdxRd(ins);
		break;
	case Instruction::DsIdxWr:
		emitDsIdxWr(ins);
		break;
	case Instruction::DsIdxWrXchg:
		emitDsIdxWrXchg(ins);
		break;
	case Instruction::DsIdxCondXchg:
		emitDsIdxCondXchg(ins);
		break;
	case Instruction::DsIdxWrap:
		emitDsIdxWrap(ins);
		break;
	case Instruction::DsAtomicArith32:
		emitDsAtomicArith32(ins);
		break;
	case Instruction::DsAtomicArith64:
		emitDsAtomicArith64(ins);
		break;
	case Instruction::DsAtomicMinMax32:
		emitDsAtomicMinMax32(ins);
		break;
	case Instruction::DsAtomicMinMax64:
		emitDsAtomicMinMax64(ins);
		break;
	case Instruction::DsAtomicCmpSt32:
		emitDsAtomicCmpSt32(ins);
		break;
	case Instruction::DsAtomicCmpSt64:
		emitDsAtomicCmpSt64(ins);
		break;
	case Instruction::DsAtomicLogic32:
		emitDsAtomicLogic32(ins);
		break;
	case Instruction::DsAtomicLogic64:
		emitDsAtomicLogic64(ins);
		break;
	case Instruction::DsAppendCon:
		emitDsAppendCon(ins);
		break;
	case Instruction::DsDataShareUt:
		emitDsDataShareUt(ins);
		break;
	case Instruction::DsDataShareMisc:
		emitDsDataShareMisc(ins);
		break;
	case Instruction::GdsSync:
		emitGdsSync(ins);
		break;
	case Instruction::GdsOrdCnt:
		emitGdsOrdCnt(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not DataShare.");
		break;
	}
}


void GCNCompiler::emitDsIdxRd(GCNInstruction& ins)
{
	auto inst = asInst<SIDSInstruction>(ins);
	auto op   = inst->GetOp();

	uint32_t gds = inst->GetGDS();
	LOG_ASSERT(gds == 0, "do not support gds yet.");

	uint32_t vbindex = inst->GetADDR();
	uint32_t offset0 = inst->GetOFFSET(0);
	uint32_t offset1 = inst->GetOFFSET(1);
	uint32_t src0    = inst->GetDATA(0);
	uint32_t src1    = inst->GetDATA(1);

	SpirvRegisterValue spvIndex   = emitLoadVectorOperand(vbindex, SpirvScalarType::Uint32);
	uint32_t           offset0Id = m_module.constu32(offset0);

	const uint32_t u32TypeId = getScalarTypeId(SpirvScalarType::Uint32);

	uint32_t           indexId0, indexId1;
	SpirvRegisterValue spvSrc00, spvSrc01;

	switch (op)
	{
	case SIDSInstruction::DS_READ_B32:
	case SIDSInstruction::DS_READ_B64:
		// We treat LDS memory as uint array, so divide by 4.
		indexId0 = m_module.opUDiv(u32TypeId,
								   m_module.opIAdd(u32TypeId, spvIndex.id, offset0Id),
								   m_module.constu32(4));
		break;
	case SIDSInstruction::DS_READ2_B32:
	{
		uint32_t offset1Id = m_module.constu32(offset1);
		// For dual ds read, offset is unit of OpDataSize
		uint32_t regionAddr0 = m_module.opIAdd(
			u32TypeId,
			spvIndex.id,
			m_module.opIMul(u32TypeId, offset0Id, m_module.constu32(4)));

		indexId0 = m_module.opUDiv(u32TypeId,
								   regionAddr0,
								   m_module.constu32(4));

		uint32_t regionAddr1 = m_module.opIAdd(
			u32TypeId,
			spvIndex.id,
			m_module.opIMul(u32TypeId, offset1Id, m_module.constu32(4)));

		indexId1 = m_module.opUDiv(u32TypeId,
								   regionAddr1,
								   m_module.constu32(4));
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	switch (op)
	{
	case SIDSInstruction::DS_READ_B32:
	{
		auto     element = emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId0);
		uint32_t value   = m_module.opLoad(u32TypeId, element.id);
		emitStoreVectorOperand(src0, { SpirvScalarType::Uint32, 1, value });
	}
		break;
	case SIDSInstruction::DS_READ_B64:
	{
		indexId1          = m_module.opIAdd(u32TypeId, indexId0, m_module.constu32(1));
		auto     element0 = emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId0);
		auto     element1 = emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId1);

		uint32_t value0 = m_module.opLoad(u32TypeId, element0.id);
		uint32_t value1 = m_module.opLoad(u32TypeId, element1.id);

		emitStoreVectorOperand(src0, { SpirvScalarType::Uint32, 1, value0 });
		emitStoreVectorOperand(src0 + 1, { SpirvScalarType::Uint32, 1, value1 });
	}
		break;
	case SIDSInstruction::DS_READ2_B32:
	{
		auto element0 = emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId0);
		auto element1 = emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId1);

		uint32_t value0 = m_module.opLoad(u32TypeId, element0.id);
		uint32_t value1 = m_module.opLoad(u32TypeId, element1.id);

		emitStoreVectorOperand(src0, { SpirvScalarType::Uint32, 1, value0 });
		emitStoreVectorOperand(src1, { SpirvScalarType::Uint32, 1, value1 });
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

}

void GCNCompiler::emitDsIdxWr(GCNInstruction& ins)
{
	auto inst = asInst<SIDSInstruction>(ins);
	auto op   = inst->GetOp();

	uint32_t gds     = inst->GetGDS();
	LOG_ASSERT(gds == 0, "do not support gds yet.");

	uint32_t vbindex = inst->GetADDR();
	uint32_t offset0 = inst->GetOFFSET(0);
	uint32_t src0    = inst->GetDATA(0);

	SpirvRegisterValue spvIndex = emitLoadVectorOperand(vbindex, SpirvScalarType::Uint32);
	SpirvRegisterValue spvOffset0 = emitLoadVectorOperand(offset0, SpirvScalarType::Uint32);

	const uint32_t     u32TypeId = getScalarTypeId(SpirvScalarType::Uint32);
	// We treat LDS memory as uint array, so divide by 4.
	uint32_t           indexId = m_module.opUDiv(u32TypeId,
                                             m_module.opIAdd(u32TypeId, spvIndex.id, spvOffset0.id),
                                             m_module.constu32(sizeof(uint32_t)));
	SpirvRegisterValue spvSrc00, spvSrc01;

	switch (op)
	{
	case SIDSInstruction::DS_WRITE_B8:
	case SIDSInstruction::DS_WRITE_B16:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SIDSInstruction::DS_WRITE_B32:
		spvSrc00 = emitLoadVectorOperand(src0, SpirvScalarType::Uint32);
		break;
	case SIDSInstruction::DS_WRITE_B64:
		spvSrc00 = emitLoadVectorOperand(src0, SpirvScalarType::Uint32);
		spvSrc01 = emitLoadVectorOperand(src0 + 1, SpirvScalarType::Uint32);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	switch (op)
	{
	case SIDSInstruction::DS_WRITE_B8:
	case SIDSInstruction::DS_WRITE_B16:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SIDSInstruction::DS_WRITE_B32:
	{
		SpirvRegisterPointer element = 
			emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId);
		m_module.opStore(element.id, spvSrc00.id);
	}
		break;
	case SIDSInstruction::DS_WRITE_B64:
	{
		SpirvRegisterPointer element0 =
			emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, indexId);
		SpirvRegisterPointer element1 =
			emitArrayAccess(m_cs.lds, spv::StorageClassWorkgroup, 
				m_module.opIAdd(u32TypeId, indexId, m_module.constu32(1)) );

		m_module.opStore(element0.id, spvSrc00.id);
		m_module.opStore(element1.id, spvSrc01.id);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitDsIdxWrXchg(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsIdxCondXchg(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsIdxWrap(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicArith32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicArith64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicMinMax32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicMinMax64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicCmpSt32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicCmpSt64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicLogic32(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAtomicLogic64(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsAppendCon(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsDataShareUt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitDsDataShareMisc(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitGdsSync(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitGdsOrdCnt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

}  //namespace pssl