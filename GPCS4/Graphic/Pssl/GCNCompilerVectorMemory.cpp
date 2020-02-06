#include "GCNCompiler.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerVectorMemory);

namespace pssl
{;

void GCNCompiler::emitVectorMemory(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::VectorMemBufNoFmt:
		emitVectorMemBufNoFmt(ins);
		break;
	case Instruction::VectorMemBufFmt:
		emitVectorMemBufFmt(ins);
		break;
	case Instruction::VectorMemImgNoSmp:
		emitVectorMemImgNoSmp(ins);
		break;
	case Instruction::VectorMemImgSmp:
		emitVectorMemImgSmp(ins);
		break;
	case Instruction::VectorMemImgUt:
		emitVectorMemImgUt(ins);
		break;
	case Instruction::VectorMemL1Cache:
		emitVectorMemL1Cache(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not VectorMemory.");
		break;
	}
}

void GCNCompiler::emitVectorMemBufNoFmt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorMemBufFmt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorMemImgNoSmp(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorMemImgSmp(GCNInstruction& ins)
{
	auto inst = asInst<SIMIMGInstruction>(ins);
	auto op = inst->GetOp();

	uint32_t dstReg = inst->GetVDATA();
	uint32_t uvReg = inst->GetVADDR();
	
	// in units of 4 sgprs
	uint32_t sampReg = inst->GetSSAMP() * 4;
	uint32_t texReg = inst->GetSRSRC() * 4;
	
	auto& sampler = m_ps.samplers.at(sampReg);
	auto& texture = m_ps.textures.at(texReg);

	switch (op)
	{
	case SIMIMGInstruction::IMAGE_SAMPLE_L:
	{
		auto u = emitVgprLoad(uvReg);
		auto v = emitVgprLoad(uvReg + 1);
		std::array<uint32_t, 2> uvComponents = { u.id, v.id };
		uint32_t typeId = getVectorTypeId({ SpirvScalarType::Float32, 2 });
		uint32_t uvId = m_module.opCompositeConstruct(typeId, uvComponents.size(), uvComponents.data());

		uint32_t sampledImageId = emitLoadSampledImage(texture, sampler);
		SpirvRegisterValue colorValue;
		colorValue.type.ctype = SpirvScalarType::Float32;
		colorValue.type.ccount = 4; 
		uint32_t resultTypeId = getVectorTypeId(colorValue.type);
		colorValue.id = m_module.opImageSampleImplicitLod(resultTypeId, sampledImageId, uvId, { 0 });
		emitVgprVectorStore(dstReg, colorValue, 0x0F);
	}
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitVectorMemImgUt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorMemL1Cache(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

}  // namespace pssl