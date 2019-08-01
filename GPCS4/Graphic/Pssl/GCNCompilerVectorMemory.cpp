#include "GCNCompiler.h"

namespace pssl
{;

void GCNCompiler::emitVectorMemory(GCNInstruction& ins)
{
	Instruction::InstructionClass insClass = ins.instruction->GetInstructionClass();

	switch (insClass)
	{
	case Instruction::VectorMemBufNoFmt:
		emitVectorMemBufNoFmt(ins);
		break;
	case Instruction::VectorMemBufFmt:
		emitVectorMemBufFmt(ins);
		break;
	case Instruction::VectorMemImgNoSmp:
		emitVectorMemImgNoSmp(ins);
		break;
	case Instruction::VectorMemImgSmp:
		emitVectorMemImgSmp(ins);
		break;
	case Instruction::VectorMemImgUt:
		emitVectorMemImgUt(ins);
		break;
	case Instruction::VectorMemL1Cache:
		emitVectorMemL1Cache(ins);
		break;
	case Instruction::InstructionClassUnknown:
	case Instruction::InstructionClassCount:
		LOG_FIXME("Instruction class not initialized.");
		break;
	default:
		LOG_ERR("Instruction category is not VectorMemory.");
		break;
	}
}


void GCNCompiler::emitVectorMemBufNoFmt(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorMemBufFmt(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorMemImgNoSmp(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorMemImgSmp(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorMemImgUt(GCNInstruction& ins)
{

}

void GCNCompiler::emitVectorMemL1Cache(GCNInstruction& ins)
{

}

}  // namespace pssl