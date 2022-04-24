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
		switch (exp.target)
		{
			case GcnExportTarget::Pos:
				this->emitExpPos(ins);
				break;
			case GcnExportTarget::Param:
				this->emitExpParam(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitExpPos(const GcnShaderInstruction& ins)
	{
		auto exp = gcnInstructionAs<GcnShaderInstEXP>(ins);

		std::array<uint32_t, 4> src;

		auto     posMask        = GcnRegMask(exp.control.en);
		uint32_t componentCount = posMask.popCount();

		LOG_ASSERT(componentCount == 4, "vertex output component count should always be 4.");

		for (uint32_t i = 0; i != componentCount; ++i)
		{
			src[i] = emitVgprLoad(ins.src[i]).id;
		}

		// Create the actual position vector
		GcnRegisterValue position;
		position.type.ctype  = GcnScalarType::Float32;
		position.type.ccount = componentCount;
		position.id          = m_module.opCompositeConstruct(
					 getVectorTypeId(position.type),
					 componentCount, src.data());

		emitVsSystemValueStore(GcnSystemValue::Position,
							   posMask,
							   position);
	}

	void GcnCompiler::emitExpParam(const GcnShaderInstruction& ins)
	{
		auto exp = gcnInstructionAs<GcnShaderInstEXP>(ins);

		std::array<uint32_t, 4> src;

		auto     paramMask      = GcnRegMask(exp.control.en);
		uint32_t componentCount = paramMask.popCount();

		LOG_ASSERT(exp.control.compr == 0, "TODO: compressed output not supported yet.");

		for (uint32_t i = 0; i != componentCount; ++i)
		{
			src[i] = emitVgprLoad(ins.src[i]).id;
		}

		// Create the actual param vector
		GcnRegisterValue param;
		param.type.ctype  = GcnScalarType::Float32;
		param.type.ccount = componentCount;
		param.id          = m_module.opCompositeConstruct(
					 getVectorTypeId(param.type),
					 componentCount, src.data());

		uint32_t regIndex  = exp.control.target - GcnMinExpParam;
		auto&    outputPtr = m_outputs[regIndex];

		emitValueStore(outputPtr, param, paramMask);
	}
}  // namespace sce::gcn