#include "GCNCompiler.h"

namespace pssl
{;

GCNCompiler::GCNCompiler(const PsslProgramInfo& progInfo,
	std::optional<PsslFetchShader> fsShader)
{

}

GCNCompiler::~GCNCompiler()
{
}

void GCNCompiler::processInstruction(GCNInstruction& ins)
{
	Instruction::InstructionCategory insCategory = ins.instruction->GetInstructionCategory();
	switch (insCategory)
	{
	case Instruction::ScalarALU:
		emitScalarALU(ins);
		break;
	case Instruction::ScalarMemory:
		emitScalarMemory(ins);
		break;
	case Instruction::VectorALU:
		emitVectorALU(ins);
		break;
	case Instruction::VectorMemory:
		emitVectorMemory(ins);
		break;
	case Instruction::FlowControl:
		emitFlowControl(ins);
		break;
	case Instruction::DataShare:
		emitDataShare(ins);
		break;
	case Instruction::VectorInterpolation:
		emitVectorInterpolation(ins);
		break;
	case Instruction::Export:
		emitExport(ins);
		break;
	case Instruction::DebugProfile:
		emitDebugProfile(ins);
		break;
	case Instruction::CategoryUnknown:
	case Instruction::InstructionsCategoriesCount:
		LOG_FIXME("Instruction category not initialized. Encoding %d", ins.instruction->GetInstructionFormat());
		break;
	default:
		break;
	}
}

RcPtr<gve::GveShader> GCNCompiler::finalize()
{

}







//////////////////////////////////////////////////////////////////////////





} // namespace pssl