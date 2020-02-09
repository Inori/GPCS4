#include "GCNCompiler.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerScalarMemory);

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

void GCNCompiler::emitScalarMemBufferLoad(
	uint32_t bufferId, 
	uint32_t dstRegStart, 
	uint32_t dstRegCount,
	uint32_t offsetDw)
{
	// Load dwords into sgprs from buffer object.

	uint32_t uniformFloatPtrId = m_module.defFloatPointerType(32, spv::StorageClassUniform);

	std::vector<SpirvRegisterValue> valueArray;

	for (uint32_t i = 0; i != dstRegCount; ++i)
	{
		uint32_t dwordOffset            = offsetDw + i;
		uint32_t offsetId               = m_module.constu32(dwordOffset);
		std::array<uint32_t, 2> indices = { m_module.constu32(0), offsetId };
		uint32_t srcId                  = m_module.opAccessChain(
            uniformFloatPtrId,
            m_vs.m_uboId,
            indices.size(), indices.data());
		auto value = emitValueLoad({ SpirvScalarType::Float32, 1, srcId });
		valueArray.emplace_back(SpirvScalarType::Float32, 1, value.id);
	}

	emitSgprArrayStore(dstRegStart, valueArray.data(), valueArray.size());
}

void GCNCompiler::emitScalarMemRd(GCNInstruction& ins)
{
	auto inst = asInst<SISMRDInstruction>(ins);

	auto op = inst->GetOp();

	uint32_t dstStartReg = inst->GetSRidx();
	uint32_t srcStartReg = inst->GetSbase();

	uint32_t imm    = inst->GetImm();
	LOG_ASSERT(imm == true, "not support SGPR offset yet.");

	uint32_t offsetDw = inst->GetOffset();

	switch (op)
	{
	case SISMRDInstruction::S_LOAD_DWORD:
	case SISMRDInstruction::S_LOAD_DWORDX2:
	case SISMRDInstruction::S_LOAD_DWORDX4:
	case SISMRDInstruction::S_LOAD_DWORDX8:
	case SISMRDInstruction::S_LOAD_DWORDX16:
	{
		// TODO:
		// Currently, I only found S_LOAD_DWORD[XN] used to load resource in EUD.
		// If there's other usage, we need to supported it.
		LOG_ASSERT(m_shaderInput.shaderResources.eud.has_value() &&
				   srcStartReg == m_shaderInput.shaderResources.eud->startRegister,
				   "only support load EUD currently.");
		emitDclShaderResourceEUD(dstStartReg, offsetDw);
	}
		break;
	case SISMRDInstruction::S_BUFFER_LOAD_DWORD:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX2:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX4:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX8:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX16:
	{
		uint32_t regCount = 2 << ((uint32_t)op - 10 + 1);
		emitScalarMemBufferLoad(m_vs.m_uboId, dstStartReg, regCount, offsetDw);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitScalarMemUt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

}  // namespace pssl