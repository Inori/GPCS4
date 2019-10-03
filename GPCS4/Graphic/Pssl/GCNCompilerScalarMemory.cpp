#include "GCNCompiler.h"

namespace pssl
{;

void GCNCompiler::emitScalarMemory(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::ScalarMemRd:
		emitScalarMemRd(ins);
		break;
	case Instruction::ScalarMemUt:
		emitScalarMemUt(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not ScalarMemory.");
		break;
	}
}

void GCNCompiler::emitScalarMemRd(GCNInstruction& ins)
{
	auto inst = asInst<SISMRDInstruction>(ins);
	auto op = inst->GetOp();

	switch (op)
	{
	case SISMRDInstruction::S_LOAD_DWORD:
	case SISMRDInstruction::S_LOAD_DWORDX2:
	case SISMRDInstruction::S_LOAD_DWORDX4:
	case SISMRDInstruction::S_LOAD_DWORDX8:
	case SISMRDInstruction::S_LOAD_DWORDX16:
		break;
	case SISMRDInstruction::S_BUFFER_LOAD_DWORD:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX2:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX4:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX8:
	case SISMRDInstruction::S_BUFFER_LOAD_DWORDX16:
		break;
	default:
		break;
	}
}

void GCNCompiler::emitScalarMemUt(GCNInstruction& ins)
{

}

}  // namespace pssl