#include "GCNCompiler.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerVectorInterpolation);

namespace pssl
{;


void GCNCompiler::emitVectorInterpolation(GCNInstruction& ins)
{
	emitVectorInterpFpCache(ins);
}

void GCNCompiler::emitVectorInterpFpCache(GCNInstruction& ins)
{
	auto inst = asInst<SIVINTRPInstruction>(ins);
	auto op = inst->GetOp();

	uint32_t dst = inst->GetVDST();
	uint32_t attr = inst->GetATTR();
	uint32_t chan = inst->GetATTRCHAN();

	SpirvRegisterValue dstValue;

	switch (op)
	{
	case SIVINTRPInstruction::V_INTERP_P1_F32:
		// Skip the first interpolation step, let the host GPU do it.
		break;
	case SIVINTRPInstruction::V_INTERP_P2_F32:
	{
		// For the second interpolation step, 
		// we just copy the already interpolated input value
		// to the dst vgpr.
		const auto& input = m_ps.psInputs[attr];
		dstValue = emitRegisterComponentLoad(input, chan, spv::StorageClassInput);
		
	}
		break;
	case SIVINTRPInstruction::V_INTERP_MOV_F32:
		LOG_PSSL_UNHANDLED_INST();
		break;
	default:
		LOG_PSSL_UNHANDLED_INST();
		break;
	}

	if (op != SIVINTRPInstruction::V_INTERP_P1_F32)  // For V_INTERP_P1_F32, we shouldn't store.
	{
		emitVgprStore(dst, dstValue);
	}
}


}
