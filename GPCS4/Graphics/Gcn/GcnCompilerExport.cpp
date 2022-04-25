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

		auto     mask           = GcnRegMask(exp.control.en);
		uint32_t componentCount = mask.popCount();

		for (uint32_t i = 0; i != componentCount; ++i)
		{
			src[i] = emitVgprLoad(ins.src[i]).id;
		}

		// Create the actual result vector
		GcnRegisterValue value;
		value.type.ctype  = GcnScalarType::Float32;
		value.type.ccount = componentCount;
		value.id          = m_module.opCompositeConstruct(getVectorTypeId(value.type),
														  componentCount, src.data());

		LOG_ASSERT(exp.control.compr == 0, "TODO: compressed output not supported yet.");

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
		auto& outputPtr = m_outputs[regIdx];

		emitValueStore(outputPtr, value, writeMask);
	}
}  // namespace sce::gcn