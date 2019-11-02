#include "GCNCompiler.h"


namespace pssl
{;

void GCNCompiler::emitScalarMemory(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::ScalarMemRd:
		emitScalarMemRd(ins);
		break;
	case Instruction::ScalarMemUt:
		emitScalarMemUt(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not ScalarMemory.");
		break;
	}
}

void GCNCompiler::emitScalarMemRd(GCNInstruction& ins)
{
	auto inst = asInst<SISMRDInstruction>(ins);

	auto op = inst->GetOp();
	uint32_t startRegIdx = inst->GetSRidx();
	uint32_t offset = inst->GetOffset() << 2;

	uint32_t uniformFloatPtrId = m_module.defFloatPointerType(32, spv::StorageClassUniform);

	switch (op)
	{
	case SISMRDInstruction::S_LOAD_DWORD:
	case SISMRDInstruction::S_LOAD_DWORDX2:
	case SISMRDInstruction::S_LOAD_DWORDX4:
	case SISMRDInstruction::S_LOAD_DWORDX8:
	case SISMRDInstruction::S_LOAD_DWORDX16:
		break;
	case SISMRDInstruction::S_BUFFER_LOAD_DWORD:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX2:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX4:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX8:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX16:
	{
		uint32_t regCount = 2 << ((uint32_t)op - 10 + 1);

		std::vector<SpirvRegisterValue> valueArray;
		for (uint32_t i = 0; i != regCount; ++i)
		{
			uint32_t dwordOffset = offset / sizeof(uint32_t) + i;
			uint32_t offsetId = m_module.constu32(dwordOffset);
			std::array<uint32_t, 2> indices = { m_module.constu32(0), offsetId };
			uint32_t srcId = m_module.opAccessChain(
				uniformFloatPtrId,
				m_vs.m_uboId,
				indices.size(), indices.data());
			auto value = emitValueLoad({ SpirvScalarType::Float32, 1, srcId });
			valueArray.emplace_back(SpirvScalarType::Float32, 1, value.id);
		}

		emitSgprArrayStore(startRegIdx, valueArray.data(), valueArray.size());
	}
		break;
	default:
		break;
	}
}

void GCNCompiler::emitScalarMemUt(GCNInstruction& ins)
{

}

}  // namespace pssl