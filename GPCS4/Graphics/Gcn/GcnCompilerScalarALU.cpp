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
				this->emitScalarArith(ins);
				break;
			case GcnInstClass::ScalarAbs:
				this->emitScalarAbs(ins);
				break;
			case GcnInstClass::ScalarMov:
				this->emitScalarMov(ins);
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
		std::array<GcnRegisterValuePair, GcnMaxOperandCount> src;
		for (uint32_t i = 0; i != ins.srcCount ; ++i)
		{
			src[i] = emitRegisterLoad(ins.src[i]);
		}

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_MOV_B32:
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}


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
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn