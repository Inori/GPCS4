#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitVectorInterpolation(const GcnShaderInstruction& ins)
	{
		this->emitVectorInterpFpCache(ins);
	}

	void GcnCompiler::emitVectorInterpFpCache(const GcnShaderInstruction& ins)
	{
		auto vinterp = gcnInstructionAs<GcnShaderInstVINTRP>(ins);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::V_INTERP_P1_F32:
				// Skip the first interpolation step, let the GPU do it automatically.
				break;
			case GcnOpcode::V_INTERP_P2_F32:
			{
				// Stores the auto-interpolated value to dst vgpr.
				uint32_t regIdx = vinterp.control.attr;
				auto     mask   = GcnRegMask::select(vinterp.control.chan);

				auto& input   = m_inputs[regIdx];
				auto  compPtr = emitVectorAccess(input,
												 spv::StorageClass::StorageClassInput,
												 mask);
				auto  value   = emitValueLoad(compPtr);
				emitVgprStore(vinterp.vdst, value);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}
}  // namespace sce::gcn