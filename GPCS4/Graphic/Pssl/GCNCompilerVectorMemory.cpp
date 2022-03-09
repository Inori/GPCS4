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

	SpirvRegisterValue spvIndex = emitGprLoad<SpirvGprType::Vector>(idxReg, SpirvScalarType::Uint32);

	uint32_t bufferId = findResourceBufferId(vsharpReg);
	LOG_ASSERT(bufferId != InvalidSpvId, "buffer not found at reg %d", vsharpReg);

	uint32_t dstRegCount = 0;
	if (op <= 3)
	{
		dstRegCount = static_cast<uint32_t>(op) + 1;
	}
	else if (op >= 12 && op <= 14)
	{
		dstRegCount = 1 << (op - 12);
	}
	else if (op == 15)
	{
		dstRegCount = 3;
	}

	LOG_ASSERT(dstRegCount != 0, "error dst register count.");
	
	// Note:
	// The index value is in units of stride.

	const uint32_t stride = 4;
	uint32_t regionAddr = m_module.opIMul(typeId, spvIndex.id, m_module.constu32(stride));
	if (offen)
	{
		SpirvRegisterValue spvOffset = emitGprLoad<SpirvGprType::Vector>(idxReg + 1, SpirvScalarType::Uint32);
		regionAddr                   = m_module.opIAdd(typeId, spvOffset.id, regionAddr);
	}
	uint32_t recordId = m_module.opUDiv(typeId, regionAddr, m_module.constu32(stride));
	
	if (offen)
	{
		recordId = regionAddr;
	}

	std::vector<SpirvRegisterValue> valueArray;

	for (uint32_t i = 0; i != dstRegCount; ++i)
	{
		uint32_t offsetId = m_module.opIAdd(typeId, recordId, m_module.constu32(i));

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

void GCNCompiler::emitVectorMemBufferStore(GCNInstruction& ins)
{
	uint32_t op        = 0;
	uint32_t idxen     = 0;
	uint32_t offen     = 0;
	uint32_t offset    = 0;
	uint32_t soffset   = 0;
	uint32_t idxReg    = 0;
	uint32_t srcReg    = 0;
	uint32_t vsharpReg = 0;

	if (ins.instruction->GetInstructionFormat() == Instruction::InstructionSet_MUBUF)
	{
		auto inst = asInst<SIMUBUFInstruction>(ins);
		op        = inst->GetOp();
		idxen     = inst->GetIDXEN();
		offen     = inst->GetOFFEN();
		offset    = inst->GetOFFSET();
		soffset   = inst->GetSOFFSET();
		idxReg    = inst->GetVADDR();
		srcReg    = inst->GetVDATA();
		vsharpReg = inst->GetSRSRC() * 4;  // In unit of 4-GPR
	}

	LOG_ASSERT(idxen == 1, "only support indexed load.");
	LOG_ASSERT(offen == 0, "do not support offen.");
	LOG_ASSERT(soffset == 128, "only support 0 const SGPR offset");

	uint32_t typeId    = getScalarTypeId(SpirvScalarType::Uint32);
	uint32_t ptrTypeId = m_module.defPointerType(typeId, spv::StorageClassUniform);
	
	// Note:
	// The index value is in units of stride.
	SpirvRegisterValue spvIndex = emitGprLoad<SpirvGprType::Vector>(idxReg, SpirvScalarType::Uint32);
	uint32_t offsetId = m_module.constu32(offset);

	// TODO:
	// Currently assume stride from V# is 4.
	const uint32_t stride = 4;
	uint32_t recordId = m_module.opUDiv(
		typeId, 
		m_module.opIAdd(
			typeId, 
			offsetId, 
			m_module.opIMul(
				typeId, 
				spvIndex.id, 
				m_module.constu32(stride))),
        m_module.constu32(stride));

	uint32_t bufferId = findResourceBufferId(vsharpReg);
	LOG_ASSERT(bufferId != InvalidSpvId, "buffer not found at reg %d", vsharpReg);

	uint32_t storeCount  = 0;
	if (op != SIMUBUFInstruction::BUFFER_STORE_DWORDX3)
	{
		storeCount = 1 << (op - 28);
	}
	else
	{
		storeCount = 3;
	}

	for (uint32_t i = 0; i != storeCount; ++i)
	{
		SpirvRegisterValue value = emitLoadVectorOperand(srcReg + i, SpirvScalarType::Uint32);

		std::array<uint32_t, 2> indices = { m_module.constu32(0), recordId };
		uint32_t                element   = m_module.opAccessChain(
            ptrTypeId,
            bufferId,
            indices.size(), indices.data());

		m_module.opStore(element, value.id);
	}
}


void GCNCompiler::emitVectorMemBufNoFmt(GCNInstruction& ins)
{
	auto inst = asInst<SIMUBUFInstruction>(ins);

	auto op = inst->GetOp();

	switch (op)
	{
	case SIMUBUFInstruction::BUFFER_LOAD_UBYTE:
	case SIMUBUFInstruction::BUFFER_LOAD_SBYTE:
	case SIMUBUFInstruction::BUFFER_LOAD_USHORT:
	case SIMUBUFInstruction::BUFFER_LOAD_SSHORT:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SIMUBUFInstruction::BUFFER_LOAD_DWORD:
	case SIMUBUFInstruction::BUFFER_LOAD_DWORDX2:
	case SIMUBUFInstruction::BUFFER_LOAD_DWORDX4:
	case SIMUBUFInstruction::BUFFER_LOAD_DWORDX3:
		emitVectorMemBufferLoad(ins);
		break;
	case SIMUBUFInstruction::BUFFER_STORE_BYTE:
	case SIMUBUFInstruction::BUFFER_STORE_SHORT:
		LOG_PSSL_UNHANDLED_INST();
		break;
	case SIMUBUFInstruction::BUFFER_STORE_DWORD:
	case SIMUBUFInstruction::BUFFER_STORE_DWORDX2:
	case SIMUBUFInstruction::BUFFER_STORE_DWORDX4:
	case SIMUBUFInstruction::BUFFER_STORE_DWORDX3:
		emitVectorMemBufferStore(ins);
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}
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