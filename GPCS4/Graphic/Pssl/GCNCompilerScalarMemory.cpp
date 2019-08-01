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

}

void GCNCompiler::emitScalarMemUt(GCNInstruction& ins)
{

}

}  // namespace pssl