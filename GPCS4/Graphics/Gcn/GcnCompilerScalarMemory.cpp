#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitScalarMemory(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::ScalarMemRd:
				this->emitScalarMemRd(ins);
				break;
			case GcnInstClass::ScalarMemUt:
				this->emitScalarMemUt(ins);
				break;
		}
	}

	void GcnCompiler::emitScalarMemRd(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitScalarMemUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn