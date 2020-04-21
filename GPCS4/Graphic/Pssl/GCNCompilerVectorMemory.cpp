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


void GCNCompiler::emitVectorMemBufferLoad(GCNInstruction& ins)
{
	uint32_t op      = 0;
	uint32_t idxen   = 0;
	uint32_t offen   = 0;
	uint32_t offset  = 0;
	uint32_t soffset = 0;
	uint32_t idxReg  = 0;
	uint32_t dstReg  = 0;
	uint32_t vsharpReg = 0;

	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_MUBUF)
	{
		auto inst = asInst<SIMUBUFInstruction>(ins);
		op      = inst->GetOp();
		idxen   = inst->GetIDXEN();
		offen   = inst->GetOFFEN();
		offset  = inst->GetOFFSET();
		soffset = inst->GetSOFFSET();
		idxReg  = inst->GetVADDR();
		dstReg  = inst->GetVDATA();
		vsharpReg = inst->GetSRSRC() * 4;  // In unit of 4-GPR
	}
	else
	{
		auto inst = asInst<SIMTBUFInstruction>(ins);
		op        = inst->GetOp();
		idxen     = inst->GetIDXEN();
		offen     = inst->GetOFFEN();
		offset    = inst->GetOFFSET();
		soffset   = inst->GetSOFFSET();
		idxReg    = inst->GetVADDR();
		dstReg    = inst->GetVDATA();
		vsharpReg = inst->GetSRSRC() * 4; // In unit of 4-GPR
	}

	LOG_ASSERT(idxen == 1, "only support indexed load.");
	LOG_ASSERT(offset == 0, "do not support imm offset yet.");
	LOG_ASSERT(soffset == 128, "only support 0 const SGPR offset");

	uint32_t typeId    = getScalarTypeId(SpirvScalarType::Uint32);
	uint32_t ptrTypeId = m_module.defPointerType(typeId, spv::StorageClassUniform);

	SpirvRegisterValue indexVal = emitGprLoad<SpirvGprType::Vector>(idxReg, SpirvScalarType::Uint32);

	uint32_t vgprOffsetId = 0;
	if (offen)
	{
		SpirvRegisterValue offsetVal = emitGprLoad<SpirvGprType::Vector>(idxReg + 1, SpirvScalarType::Uint32);
		vgprOffsetId                   = m_module.opUDiv(typeId, offsetVal.id, m_module.constu32(4));
	}

	uint32_t bufferId = findResourceBufferId(vsharpReg);
	LOG_ASSERT(bufferId != InvalidSpvId, "buffer not found at reg %d", vsharpReg);

	uint32_t                        dstRegCount = static_cast<uint32_t>(op) + 1;
	std::vector<SpirvRegisterValue> valueArray;

	// Note:
	// The index value is in units of stride.

	uint32_t recordId = indexVal.id; 
	for (uint32_t i = 0; i != dstRegCount; ++i)
	{
		uint32_t offsetId = m_module.opIAdd(typeId, recordId, m_module.constu32(i));

		if (offen)
		{
			offsetId = m_module.opIAdd(typeId, offsetId, vgprOffsetId);
		}

		std::array<uint32_t, 2> indices = { m_module.constu32(0), offsetId };
		uint32_t                srcId   = m_module.opAccessChain(
            ptrTypeId,
            bufferId,
            indices.size(), indices.data());

		SpirvRegisterPointer element;
		element.type.ctype  = SpirvScalarType::Uint32;
		element.type.ccount = 1;
		element.id          = srcId;
		auto value          = emitValueLoad(element);

		valueArray.emplace_back(value);
	}

	emitVgprArrayStore(dstReg, valueArray.data(), valueArray.size());
}

void GCNCompiler::emitVectorMemBufNoFmt(GCNInstruction& ins)
{
	LOG_PSSL_UNHANDLED_INST();
}

void GCNCompiler::emitVectorMemBufFmt(GCNInstruction& ins)
{
	auto encoding = ins.instruction->GetInstructionFormat();
	if (encoding == Instruction::InstructionSet_MUBUF)
	{
		emitVectorMemBufFmtUntyped(ins);
	}
	else // MTBUF
	{
		// Typed
		emitVectorMemBufFmtTyped(ins);
	}
}

void GCNCompiler::emitVectorMemBufFmtUntyped(GCNInstruction& ins)
{
	auto inst = asInst<SIMUBUFInstruction>(ins);

	auto op = inst->GetOp();

	switch (op)
	{
	case SIMUBUFInstruction::BUFFER_LOAD_FORMAT_X:
	case SIMUBUFInstruction::BUFFER_LOAD_FORMAT_XY:
	case SIMUBUFInstruction::BUFFER_LOAD_FORMAT_XYZ:
	case SIMUBUFInstruction::BUFFER_LOAD_FORMAT_XYZW:
		emitVectorMemBufferLoad(ins);
		break;
	case SIMUBUFInstruction::BUFFER_STORE_FORMAT_X:
	case SIMUBUFInstruction::BUFFER_STORE_FORMAT_XY:
	case SIMUBUFInstruction::BUFFER_STORE_FORMAT_XYZ:
	case SIMUBUFInstruction::BUFFER_STORE_FORMAT_XYZW:
		LOG_PSSL_UNHANDLED_INST();
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
}

void GCNCompiler::emitVectorMemBufFmtTyped(GCNInstruction& ins)
{
	auto inst = asInst<SIMTBUFInstruction>(ins);
	auto op   = inst->GetOp();

	switch (op)
	{
	case SIMTBUFInstruction::TBUFFER_LOAD_FORMAT_X:
	case SIMTBUFInstruction::TBUFFER_LOAD_FORMAT_XY:
	case SIMTBUFInstruction::TBUFFER_LOAD_FORMAT_XYZ:
	case SIMTBUFInstruction::TBUFFER_LOAD_FORMAT_XYZW:
		emitVectorMemBufferLoad(ins);
		break;
	case SIMTBUFInstruction::TBUFFER_STORE_FORMAT_X:
	case SIMTBUFInstruction::TBUFFER_STORE_FORMAT_XY:
	case SIMTBUFInstruction::TBUFFER_STORE_FORMAT_XYZ:
	case SIMTBUFInstruction::TBUFFER_STORE_FORMAT_XYZW:
		LOG_PSSL_UNHANDLED_INST();
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
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
	
	auto& sampler = m_samplers.at(sampReg);
	auto& texture = m_textures.at(texReg);

	switch (op)
	{
	case SIMIMGInstruction::IMAGE_SAMPLE_L:
	{
		auto u = emitGprLoad<SpirvGprType::Vector>(uvReg, SpirvScalarType::Float32);
		auto v = emitGprLoad<SpirvGprType::Vector>(uvReg + 1, SpirvScalarType::Float32);

		std::array<uint32_t, 2> uvComponents = { u.id, v.id };
		uint32_t typeId = getVectorTypeId({ SpirvScalarType::Float32, 2 });
		uint32_t uvId = m_module.opCompositeConstruct(typeId, uvComponents.size(), uvComponents.data());

		uint32_t sampledImageId = emitLoadSampledImage(texture, sampler);
		SpirvRegisterValue colorValue;
		colorValue.type.ctype = SpirvScalarType::Float32;
		colorValue.type.ccount = 4; 
		uint32_t resultTypeId = getVectorTypeId(colorValue.type);
		colorValue.id = m_module.opImageSampleImplicitLod(resultTypeId, sampledImageId, uvId, { 0 });
		emitVgprVectorStore(dstReg, colorValue, GcnRegMask::firstN(4));
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