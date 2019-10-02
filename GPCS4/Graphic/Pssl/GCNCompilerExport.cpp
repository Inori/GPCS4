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

	auto en = inst->GetEn();
	GcnRegMask writeMask(en);

	std::vector<uint32_t> indices;
	for (uint32_t i = 0; i != 4; ++i)
	{
		if (writeMask[i])
		{
			uint32_t regIdx = inst->GetVSRC(i);
			auto vgprPtr = emitVgprLoad(regIdx);
			indices.push_back(vgprPtr.id);
		}
	}

	// Src vector to be exported.
	SpirvRegisterValue src;
	src.type.ctype = SpirvScalarType::Float32;
	src.type.ccount = writeMask.popCount();

	uint32_t typeId = getVectorTypeId(src.type);
	uint32_t srcId = m_module.opCompositeConstruct(typeId, indices.size(), indices.data());
	src.id = srcId;

	// Determine the dst vector
	EXPInstruction::TGT expTgt = inst->GetTGT();
	uint32_t dstId = 0;
	SpirvVectorType dstType;
	switch (expTgt)
	{
	case EXPInstruction::TGTExpPosMin ... EXPInstruction::TGTExpPosMax:
	{
		// TODO:
		// Only support pos0 currently
		dstId = m_perVertexOut;
		dstType.ctype = SpirvScalarType::Float32;
		dstType.ccount = 4;
	}
		break;
	case EXPInstruction::TGTExpParamMin ... EXPInstruction::TGTExpParamMax:
		break;
	default:
		break;
	}

	// Dst vector to export to.
	SpirvRegisterPointer dst;
	dst.type = dstType;
	dst.id = dstId;

	// Store
	emitValueStore(dst, src, writeMask);
}

void GCNCompiler::emitExpPS(GCNInstruction& ins)
{

}

}  // namespace pssl