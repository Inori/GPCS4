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
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn