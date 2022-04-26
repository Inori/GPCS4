#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitExport(const GcnShaderInstruction& ins)
	{
		this->emitExp(ins);
	}

	void GcnCompiler::emitExp(const GcnShaderInstruction& ins)
	{
		auto exp = gcnInstructionAs<GcnShaderInstEXP>(ins);

		std::array<uint32_t, 4> src;

		// EN:
		// COMPR==1: export half-dword enable. Valid values are: 0x0,3,c,f
		// [0] enables VSRC0 : R,G from one VGPR (R in low bits, G high)
		// [2] enables VSRC1 : B,A from one VGPR (B in low bits, A high)
		// COMPR==0: [0-3] = enables for VSRC0..3.
		// EN may be zero only for "NULL Pixel Shader" exports (used when exporting
		// only valid mask to NULL target).
		auto     mask           = GcnRegMask(exp.control.en);
		uint32_t componentCount = mask.popCount();
		
		if (exp.control.compr)
		{
			for (uint32_t i = 0; i != componentCount / 2; ++i)
			{
				auto packedVgpr = emitVgprLoad(ins.src[i]);
				// Cast to uint type before unpack
				packedVgpr      = emitRegisterBitcast(packedVgpr, GcnScalarType::Uint32);
				auto unpackPair = emitUnpackHalf2x16(packedVgpr);
				src[i * 2]      = unpackPair.low.id;
				src[i * 2 + 1]  = unpackPair.high.id;
			}
		}
		else
		{
			for (uint32_t i = 0; i != componentCount; ++i)
			{
				src[i] = emitVgprLoad(ins.src[i]).id;
			}
		}

		// Create the actual result vector
		GcnRegisterValue value;
		value.type.ctype  = GcnScalarType::Float32;
		value.type.ccount = componentCount;
		value.id          = m_module.opCompositeConstruct(getVectorTypeId(value.type),
														  componentCount, src.data());

		switch (exp.target)
		{
			case GcnExportTarget::Pos:
			{
				uint32_t regIdx = exp.control.target - GcnExpPos0;
				this->emitExpPosStore(regIdx, value, mask);
			}
				break;
			case GcnExportTarget::Param:
			{
				uint32_t regIdx = exp.control.target - GcnExpParam0;
				this->emitExpParamStore(regIdx, value, mask);
			}
				break;
			case GcnExportTarget::Mrt:
				this->emitExpMrtStore(exp.control.target, value, mask);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitExpPosStore(uint32_t                regIdx,
									  const GcnRegisterValue& value,
									  const GcnRegMask&       writeMask)
	{
		LOG_ASSERT(regIdx == 0, "only support pos0.");
		LOG_ASSERT(writeMask.popCount() == 4, "vertex output component count should always be 4.");

		emitVsSystemValueStore(GcnSystemValue::Position,
							   writeMask,
							   value);
	}

	void GcnCompiler::emitExpParamStore(uint32_t                regIdx,
										const GcnRegisterValue& value,
										const GcnRegMask&       writeMask)
	{
		auto& outputPtr = m_params[regIdx];

		emitValueStore(outputPtr, value, writeMask);
	}
	
	void GcnCompiler::emitExpMrtStore(
		uint32_t                regIdx,
		const GcnRegisterValue& value,
		const GcnRegMask&       writeMask)
	{
		auto& mrtPtr = m_mrts[regIdx];

		emitValueStore(mrtPtr, value, writeMask);
	}

}  // namespace sce::gcn