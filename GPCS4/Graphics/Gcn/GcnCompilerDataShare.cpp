#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitDataShare(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::DsIdxRd:
				this->emitDsIdxRd(ins);
				break;
			case GcnInstClass::DsIdxWr:
				this->emitDsIdxWr(ins);
				break;
			case GcnInstClass::DsIdxWrXchg:
				this->emitDsIdxWrXchg(ins);
				break;
			case GcnInstClass::DsIdxCondXchg:
				this->emitDsIdxCondXchg(ins);
				break;
			case GcnInstClass::DsIdxWrap:
				this->emitDsIdxWrap(ins);
				break;
			case GcnInstClass::DsAtomicArith32:
				this->emitDsAtomicArith32(ins);
				break;
			case GcnInstClass::DsAtomicArith64:
				this->emitDsAtomicArith64(ins);
				break;
			case GcnInstClass::DsAtomicMinMax32:
				this->emitDsAtomicMinMax32(ins);
				break;
			case GcnInstClass::DsAtomicMinMax64:
				this->emitDsAtomicMinMax64(ins);
				break;
			case GcnInstClass::DsAtomicCmpSt32:
				this->emitDsAtomicCmpSt32(ins);
				break;
			case GcnInstClass::DsAtomicCmpSt64:
				this->emitDsAtomicCmpSt64(ins);
				break;
			case GcnInstClass::DsAtomicLogic32:
				this->emitDsAtomicLogic32(ins);
				break;
			case GcnInstClass::DsAtomicLogic64:
				this->emitDsAtomicLogic64(ins);
				break;
			case GcnInstClass::DsAppendCon:
				this->emitDsAppendCon(ins);
				break;
			case GcnInstClass::DsDataShareUt:
				this->emitDsDataShareUt(ins);
				break;
			case GcnInstClass::DsDataShareMisc:
				this->emitDsDataShareMisc(ins);
				break;
			case GcnInstClass::GdsSync:
				this->emitGdsSync(ins);
				break;
			case GcnInstClass::GdsOrdCnt:
				this->emitGdsOrdCnt(ins);
				break;
		}
	}

	void GcnCompiler::emitDsIdxRd(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsIdxWr(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsIdxWrXchg(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsIdxCondXchg(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsIdxWrap(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicArith32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicArith64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicMinMax32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicMinMax64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicCmpSt32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicCmpSt64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicLogic32(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAtomicLogic64(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsAppendCon(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsDataShareUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitDsDataShareMisc(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitGdsSync(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitGdsOrdCnt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn
