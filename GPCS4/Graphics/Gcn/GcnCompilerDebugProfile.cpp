#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitDebugProfile(const GcnShaderInstruction& ins)
	{
		this->emitDbgPro(ins);
	}

	void GcnCompiler::emitDbgPro(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn