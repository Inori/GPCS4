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
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_TTRACEDATA:
				// Nothing to do.
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}
}  // namespace sce::gcn