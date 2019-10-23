#include "GCNCompiler.h"

namespace pssl
{;


void GCNCompiler::emitExport(GCNInstruction& ins)
{
	emitExp(ins);
}

void GCNCompiler::emitExp(GCNInstruction& ins)
{
	auto shaderType = m_programInfo.shaderType();
	switch (shaderType)
	{
	case pssl::VertexShader:
		emitExpVS(ins);
		break;
	case pssl::PixelShader:
		emitExpPS(ins);
		break;
	default:
		break;
	}
}

void GCNCompiler::emitExpVS(GCNInstruction& ins)
{
	auto inst = asInst<EXPInstruction>(ins);

	EXPInstruction::TGT expTgt = inst->GetTGT();

	auto en = inst->GetEn();
	GcnRegMask writeMask(en);

	std::vector<uint32_t> indices;
	for (uint32_t i = 0; i != 4; ++i)
	{
		if (writeMask[i])
		{
			uint32_t regIdx = inst->GetVSRC(i);
			indices.push_back(emitVgprLoad(regIdx).id);
		}
	}

	// Src vector to be exported.
	SpirvRegisterValue src;
	src.type.ctype = SpirvScalarType::Float32;
	src.type.ccount = writeMask.popCount();
	src.id = m_module.opCompositeConstruct(getVectorTypeId(src.type), 
		indices.size(), indices.data());

	// Dst vector to export to.
	SpirvRegisterPointer dst;
	// Determine the dst vector
	switch (expTgt)
	{
	case EXPInstruction::TGTExpPosMin ... EXPInstruction::TGTExpPosMax:
	{
		// TODO:
		// Only support pos0 currently
		SpirvRegisterInfo info(SpirvScalarType::Float32, 4,
			0, spv::StorageClassOutput);
		uint32_t ptrTypeId = getPointerTypeId(info);

		uint32_t memberId = m_module.constu32(0);
		dst.id = m_module.opAccessChain(ptrTypeId, m_perVertexOut, 1, &memberId);
		dst.type = info.atype.vtype;
	}
		break;
	case EXPInstruction::TGTExpParamMin ... EXPInstruction::TGTExpParamMax:
	{
		dst = m_vs.vsOutputs[expTgt];
	}
		break;
	default:
		break;
	}

	// Store
	emitValueStore(dst, src, writeMask);
}

void GCNCompiler::emitExpPS(GCNInstruction& ins)
{
	auto inst = asInst<EXPInstruction>(ins);

	EXPInstruction::TGT expTgt = inst->GetTGT();
	bool isCompressed = inst->GetCOMPR();
	auto en = inst->GetEn();

	// Src vector to be exported.
	SpirvRegisterValue src;
	// TODO:
	// Currently, I found whether exp is compressed or not, 
	// the 'en' field holds the right value, 
	// e.g, if compressed, '[1:0]' bits will be '11'
	// but I'm not sure if this will be true forever.
	GcnRegMask writeMask(en);
	if (isCompressed)
	{
		std::vector<SpirvRegisterValue> components;
		for (uint32_t i = 0; i != 2; ++i)
		{
			if (writeMask[i * 2])
			{
				uint32_t regIdx = inst->GetVSRC(i);
				auto v2fpValue = emitUnpackFloat16(emitVgprLoad(regIdx));
				//auto v2fpValue = emitUnpackFloat16({ SpirvScalarType::Uint32, 1, m_vgprs[regIdx].id });
				components.push_back(v2fpValue);
			}
		}
		
		src = components.size() == 1 ?
			components[0] :
			emitRegisterConcat(components[0], components[1]);
	}
	else
	{
		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i != 4; ++i)
		{
			if (writeMask[i])
			{
				uint32_t regIdx = inst->GetVSRC(i);
				indices.push_back(m_vgprs[regIdx].id);
			}
		}

		src.type.ctype = SpirvScalarType::Float32;
		src.type.ccount = writeMask.popCount();

		uint32_t typeId = getVectorTypeId(src.type);
		src.id = m_module.opCompositeConstruct(typeId, indices.size(), indices.data());
	}

	// Dst vector to export to.
	SpirvRegisterPointer dst;

	// Determine the dst vector
	switch (expTgt)
	{
	case EXPInstruction::TGTExpMRTMin ... EXPInstruction::TGTExpMRTMax:
	{
		dst = m_ps.psOutputs[expTgt];
	}
		break;
	default:
		break;
	}

	// Store
	emitValueStore(dst, src, writeMask);
}

}  // namespace pssl