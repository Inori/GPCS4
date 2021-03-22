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
	bool     imm,
	uint32_t offset,
	uint32_t literal)
{
	// Load dwords into sgprs from buffer object.

	// TODO:
	// We need to detect the value type specified by the V# from which
	// we declare the constant buffer.
	// Here I just use float.
	
	uint32_t typeId    = getScalarTypeId(SpirvScalarType::Uint32);
	uint32_t ptrTypeId = m_module.defPointerType(typeId, spv::StorageClassUniform);

	uint32_t offsetInDwordsId = 0;
	if (!imm)
	{
		auto value = emitLoadScalarOperand(
			static_cast<uint32_t>(Instruction::OperandSRC::SRCScalarGPR), 
			offset, 
			SpirvScalarType::Uint32,
			literal);
		
		uint32_t offsetInBytesId = m_module.opBitwiseAnd(typeId, 
			value.id, m_module.constu32(~0x3));
		offsetInDwordsId         = m_module.opShiftRightLogical(typeId, 
			offsetInBytesId, m_module.constu32(2));
	}

	std::vector<SpirvRegisterValue> valueArray;

	for (uint32_t i = 0; i != dstRegCount; ++i)
	{
		uint32_t offsetId = 0;
		if (imm)
		{
			uint32_t dwordOffset = offset + i;
			offsetId             = m_module.constu32(dwordOffset);
		}
		else
		{
			offsetId = m_module.opIAdd(
				typeId, 
				offsetInDwordsId, 
				m_module.constu32(i));
		}

		std::array<uint32_t, 2> indices = { m_module.constu32(0), offsetId };
		uint32_t                srcId   = m_module.opAccessChain(
            ptrTypeId,
            bufferId,
            indices.size(), indices.data());

		SpirvRegisterPointer elementPtr;
		elementPtr.type.ctype  = SpirvScalarType::Uint32;
		elementPtr.type.ccount = 1;
		elementPtr.id          = srcId;
		auto value             = emitValueLoad(elementPtr);

		valueArray.emplace_back(value);
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
	// When imm is true, offset is the dword offset of the buffer,
	// when imm is false, offset is SGPR index or SRCLiteralConst
	uint32_t offset = inst->GetOffset();

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
		emitDclShaderResourceEUD(dstStartReg, offset);
	}
		break;
	case SISMRDInstruction::S_BUFFER_LOAD_DWORD:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX2:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX4:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX8:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX16:
	{
		uint32_t regCount = 1 << ((uint32_t)op - 8);
		uint32_t bufferId = m_constantBuffers.at(srcStartReg).varId;
		emitScalarMemBufferLoad(bufferId,
								dstStartReg, regCount,
								imm, offset, ins.literalConst);
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