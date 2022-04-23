#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitVectorMemory(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::VectorMemBufNoFmt:
				this->emitVectorMemBufNoFmt(ins);
				break;
			case GcnInstClass::VectorMemBufFmt:
				this->emitVectorMemBufFmt(ins);
				break;
			case GcnInstClass::VectorMemImgNoSmp:
				this->emitVectorMemImgNoSmp(ins);
				break;
			case GcnInstClass::VectorMemImgSmp:
				this->emitVectorMemImgSmp(ins);
				break;
			case GcnInstClass::VectorMemImgUt:
				this->emitVectorMemImgUt(ins);
				break;
			case GcnInstClass::VectorMemL1Cache:
				this->emitVectorMemL1Cache(ins);
				break;
		}
	}

	void GcnCompiler::emitVectorMemBufNoFmt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemBufFmt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemImgNoSmp(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemImgSmp(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemImgUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemL1Cache(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

}  // namespace sce::gcn