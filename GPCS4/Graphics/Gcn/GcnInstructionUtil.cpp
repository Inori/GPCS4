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

	bool isVop3BEncoding(GcnOpcode opcode)
	{
		return opcode == GcnOpcode::V_ADD_I32 ||
			   opcode == GcnOpcode::V_ADDC_U32 ||
			   opcode == GcnOpcode::V_SUB_I32 ||
			   opcode == GcnOpcode::V_SUBB_U32 ||
			   opcode == GcnOpcode::V_SUBREV_I32 ||
			   opcode == GcnOpcode::V_SUBBREV_U32 ||
			   opcode == GcnOpcode::V_DIV_SCALE_F32 ||
			   opcode == GcnOpcode::V_DIV_SCALE_F64 ||
			   opcode == GcnOpcode::V_MAD_U64_U32 ||
			   opcode == GcnOpcode::V_MAD_I64_I32;
	}

	bool isImageAccessSampling(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		return op >= GcnOpcode::IMAGE_SAMPLE &&
			   op <= GcnOpcode::IMAGE_SAMPLE_C_CD_CL_O &&
			   op != GcnOpcode::IMAGE_GET_LOD;
	}

	bool isImageAccessNoSampling(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		return op >= GcnOpcode::IMAGE_LOAD &&
			   op <= GcnOpcode::IMAGE_ATOMIC_FMAX &&
			   op != GcnOpcode::IMAGE_GET_RESINFO;
	}

	bool isUavReadAccess(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		return op >= GcnOpcode::IMAGE_LOAD &&
			   op <= GcnOpcode::IMAGE_LOAD_MIP_PCK_SGN;
	}

}  // namespace sce::gcn