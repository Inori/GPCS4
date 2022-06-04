#include "GcnInstructionUtil.h"
#include "GcnDecoder.h"

LOG_CHANNEL(Graphic.Gcn.GcnInstructionUtil);

namespace sce::gcn
{
	bool isUnconditionalBranch(const GcnShaderInstruction& ins)
	{
		return ins.opcode == GcnOpcode::S_BRANCH;
	}

	bool isForkInstruction(const GcnShaderInstruction& ins)
	{
		return ins.opcode == GcnOpcode::S_CBRANCH_I_FORK ||
			   ins.opcode == GcnOpcode::S_CBRANCH_G_FORK ||
			   ins.opcode == GcnOpcode::S_CBRANCH_JOIN;
	}

	bool isConditionalBranch(const GcnShaderInstruction& ins)
	{
		bool result = false;
		auto op     = ins.opcode;
		switch (op)
		{
			case GcnOpcode::S_CBRANCH_SCC0:
			case GcnOpcode::S_CBRANCH_SCC1:
			case GcnOpcode::S_CBRANCH_VCCZ:
			case GcnOpcode::S_CBRANCH_VCCNZ:
			case GcnOpcode::S_CBRANCH_EXECZ:
			case GcnOpcode::S_CBRANCH_EXECNZ:
				result = true;
				break;
			case GcnOpcode::S_CBRANCH_CDBGSYS:
			case GcnOpcode::S_CBRANCH_CDBGUSER:
			case GcnOpcode::S_CBRANCH_CDBGSYS_OR_USER:
			case GcnOpcode::S_CBRANCH_CDBGSYS_AND_USER:
				LOG_GCN_UNHANDLED_INST();
				break;
			default:
				break;
		}
		return result;
	}

	bool isBranchInstruction(const GcnShaderInstruction& ins)
	{
		return isUnconditionalBranch(ins) || isConditionalBranch(ins);
	}

	bool isTerminateInstruction(const GcnShaderInstruction& ins)
	{
		return isBranchInstruction(ins) ||
			   isForkInstruction(ins) ||
			   ins.opcode == GcnOpcode::S_ENDPGM;
	}

	uint32_t calculateBranchTarget(uint32_t pc, const GcnShaderInstruction& ins)
	{
		auto     sopp   = gcnInstructionAs<GcnShaderInstSOPP>(ins);
		uint32_t target = static_cast<uint32_t>(static_cast<int32_t>(pc) +
												static_cast<int16_t>(sopp.control.simm << 2) + 4);
		return target;
	}


}  // namespace sce::gcn