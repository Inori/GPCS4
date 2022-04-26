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
		auto smrd = gcnInstructionAs<GcnShaderInstSMRD>(ins);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_BUFFER_LOAD_DWORD:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX2:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX4:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX8:
			case GcnOpcode::S_BUFFER_LOAD_DWORDX16:
			{
				GcnRegIndex index = {};
				// This is the sgpr pair index, so we need to multiply by 2
				index.regIdx      = smrd.sbase.code * 2;
				if (smrd.control.imm)
				{
					index.offset = smrd.control.offset << 2;
					index.relReg = nullptr;
				}
				else
				{
					index.offset = 0;
					index.relReg = &smrd.offset;
				}
				emitBufferLoadNoFmt(index, smrd.sdst, smrd.control.count);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitScalarMemUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}
}  // namespace sce::gcn