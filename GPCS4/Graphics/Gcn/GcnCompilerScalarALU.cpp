#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitScalarALU(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::ScalarArith:
			case GcnInstClass::ScalarMov:
				this->emitScalarAluCommon(ins);
				break;
			case GcnInstClass::ScalarAbs:
				this->emitScalarAbs(ins);
				break;
			case GcnInstClass::ScalarCmp:
				this->emitScalarCmp(ins);
				break;
			case GcnInstClass::ScalarSelect:
				this->emitScalarSelect(ins);
				break;
			case GcnInstClass::ScalarBitLogic:
				this->emitScalarBitLogic(ins);
				break;
			case GcnInstClass::ScalarBitManip:
				this->emitScalarBitManip(ins);
				break;
			case GcnInstClass::ScalarBitField:
				this->emitScalarBitField(ins);
				break;
			case GcnInstClass::ScalarConv:
				this->emitScalarConv(ins);
				break;
			case GcnInstClass::ScalarExecMask:
				this->emitScalarExecMask(ins);
				break;
			case GcnInstClass::ScalarQuadMask:
				this->emitScalarQuadMask(ins);
				break;
		}
	}

	void GcnCompiler::emitScalarAluCommon(const GcnShaderInstruction& ins)
	{
		std::array<GcnRegisterValuePair, GcnMaxOperandCount> src;
		for (uint32_t i = 0; i != ins.srcCount; ++i)
		{
			src[i] = emitRegisterLoad(ins.src[i]);
		}

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = ins.dst[0].type;
		dst.low.type.ccount      = 1;
		dst.high.type.ctype      = ins.dst[0].type;
		dst.high.type.ccount     = 1;

		const uint32_t typeId = getVectorTypeId(dst.low.type);

		auto op = ins.opcode;
		switch (op)
		{
			// ScalarArith
			case GcnOpcode::S_MUL_I32:
				dst.low.id = m_module.opIMul(typeId,
											 src[0].low.id,
											 src[1].low.id);
				break;
			// ScalarMov
			case GcnOpcode::S_MOV_B32:
				dst.low.id = src[0].low.id;
				break;
			case GcnOpcode::S_MOV_B64:
			{
				// Fix the type from Uint64 to Uint32 at the same time.
				dst.low  = src[0].low;
				dst.high = src[0].high;
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		emitRegisterStore(ins.dst[0], dst);
	}

	void GcnCompiler::emitScalarArith(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarAbs(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarMov(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarCmp(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarSelect(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarBitLogic(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarBitManip(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarBitField(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarConv(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarExecMask(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarQuadMask(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_WQM_B64:
				// TODO:
				// Implement this instruction.
				// Currently we just ignore it.
				LOG_ASSERT(ins.src[0].field == GcnOperandField::ExecLo &&
						   ins.dst[0].field == GcnOperandField::ExecLo,
						   "TODO: support non-exec operand quad mask.");
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}
}  // namespace sce::gcn