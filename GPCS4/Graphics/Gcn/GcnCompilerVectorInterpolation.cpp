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
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn