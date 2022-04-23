#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitVectorALU(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::VectorRegMov:
				this->emitVectorRegMov(ins);
				break;
			case GcnInstClass::VectorLane:
				this->emitVectorLane(ins);
				break;
			case GcnInstClass::VectorBitLogic:
				this->emitVectorBitLogic(ins);
				break;
			case GcnInstClass::VectorBitField32:
				this->emitVectorBitField32(ins);
				break;
			case GcnInstClass::VectorThreadMask:
				this->emitVectorThreadMask(ins);
				break;
			case GcnInstClass::VectorBitField64:
				this->emitVectorBitField64(ins);
				break;
			case GcnInstClass::VectorFpArith32:
				this->emitVectorFpArith32(ins);
				break;
			case GcnInstClass::VectorFpRound32:
				this->emitVectorFpRound32(ins);
				break;
			case GcnInstClass::VectorFpField32:
				this->emitVectorFpField32(ins);
				break;
			case GcnInstClass::VectorFpTran32:
				this->emitVectorFpTran32(ins);
				break;
			case GcnInstClass::VectorFpCmp32:
				this->emitVectorFpCmp32(ins);
				break;
			case GcnInstClass::VectorFpArith64:
				this->emitVectorFpArith64(ins);
				break;
			case GcnInstClass::VectorFpRound64:
				this->emitVectorFpRound64(ins);
				break;
			case GcnInstClass::VectorFpField64:
				this->emitVectorFpField64(ins);
				break;
			case GcnInstClass::VectorFpTran64:
				this->emitVectorFpTran64(ins);
				break;
			case GcnInstClass::VectorFpCmp64:
				this->emitVectorFpCmp64(ins);
				break;
			case GcnInstClass::VectorIntArith32:
				this->emitVectorIntArith32(ins);
				break;
			case GcnInstClass::VectorIntArith64:
				this->emitVectorIntArith64(ins);
				break;
			case GcnInstClass::VectorIntCmp32:
				this->emitVectorIntCmp32(ins);
				break;
			case GcnInstClass::VectorIntCmp64:
				this->emitVectorIntCmp64(ins);
				break;
			case GcnInstClass::VectorConv:
				this->emitVectorConv(ins);
				break;
			case GcnInstClass::VectorFpGraph32:
				this->emitVectorFpGraph32(ins);
				break;
			case GcnInstClass::VectorIntGraph:
				this->emitVectorIntGraph(ins);
				break;
			case GcnInstClass::VectorMisc:
				this->emitVectorMisc(ins);
				break;
		}
	}


	void GcnCompiler::emitVectorRegMov(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorLane(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorBitLogic(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorBitField32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorThreadMask(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorBitField64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpArith32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpRound32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpField32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpTran32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpCmp32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpArith64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpRound64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpField64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpTran64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpCmp64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorIntArith32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorIntArith64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorIntCmp32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorIntCmp64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorConv(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorFpGraph32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorIntGraph(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMisc(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn