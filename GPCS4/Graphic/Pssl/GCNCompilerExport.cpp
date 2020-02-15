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

SpirvRegisterValue GCNCompiler::emitExpSrcLoadCompr(GCNInstruction& ins)
{
	auto inst = asInst<EXPInstruction>(ins);
	auto en   = inst->GetEn();

	GcnRegMask writeMask(en);

	std::vector<SpirvRegisterValue> components;
	for (uint32_t i = 0; i != 2; ++i)
	{
		if (!writeMask[i * 2])
		{
			continue;
		}

		uint32_t regIdx = inst->GetVSRC(i);
		auto v2fpValue  = emitUnpackFloat16(emitVgprLoad(regIdx));
		components.push_back(v2fpValue);
	}

	// Src vector to be exported.
	SpirvRegisterValue result;
	if (components.size())
	{
		result = components.size() == 1 ? 
			components[0] : 
			emitRegisterConcat(components[0], components[1]);
	}
	else
	{
		result.type.ctype  = SpirvScalarType::Unknown;
		result.type.ccount = 0;
		result.id          = InvalidSpvId;
	}

	return result;
}

SpirvRegisterValue GCNCompiler::emitExpSrcLoadNoCompr(GCNInstruction& ins)
{
	auto inst = asInst<EXPInstruction>(ins);
	auto en   = inst->GetEn();

	GcnRegMask writeMask(en);

	std::vector<uint32_t> indices;
	for (uint32_t i = 0; i != 4; ++i)
	{
		if (!writeMask[i])
		{
			continue;
		}

		uint32_t regIdx = inst->GetVSRC(i);
		indices.push_back(emitLoadVectorOperand(regIdx, SpirvScalarType::Float32).id);
	}

	// Src vector to be exported.
	SpirvRegisterValue result;
	if (indices.size())
	{
		result.type.ctype  = SpirvScalarType::Float32;
		result.type.ccount = writeMask.popCount();
		result.id          = m_module.opCompositeConstruct(getVectorTypeId(result.type),
                                                  indices.size(), indices.data());
	}
	else
	{
		result.type.ctype  = SpirvScalarType::Unknown;
		result.type.ccount = 0;
		result.id          = InvalidSpvId;
	}

	return result;
}

void GCNCompiler::emitExpVS(GCNInstruction& ins)
{
	auto inst = asInst<EXPInstruction>(ins);

	EXPInstruction::TGT expTgt = inst->GetTGT();
	bool isCompressed          = inst->GetCOMPR();
	auto en                    = inst->GetEn();

	// Src vector to be exported.
	auto src = !isCompressed ? 
		emitExpSrcLoadNoCompr(ins) : 
		emitExpSrcLoadCompr(ins);

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
		dst = m_vs.vsOutputs[expTgt];
		break;
	default:
		break;
	}

	// Store
	emitValueStore(dst, src, en);
}

void GCNCompiler::emitExpPS(GCNInstruction& ins)
{
	auto inst = asInst<EXPInstruction>(ins);

	EXPInstruction::TGT expTgt = inst->GetTGT();
	bool isCompressed = inst->GetCOMPR();
	auto en = inst->GetEn();

	auto src = isCompressed ? 
		emitExpSrcLoadCompr(ins) : 
		emitExpSrcLoadNoCompr(ins);

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

	if (src.id != InvalidSpvId)
	{
		// Store
		emitValueStore(dst, src, en);
	}
	else
	{
		// same as "discard"
		m_module.opKill();
	}
}

}  // namespace pssl