#include "GCNCompiler.h"

LOG_CHANNEL(Graphic.Pssl.GCNCompilerDebugProfile);

namespace pssl
{;


void GCNCompiler::emitDebugProfile(GCNInstruction& ins)
{
	emitDbgProf(ins);
}

void GCNCompiler::emitDbgProf(GCNInstruction& ins)
{
	auto inst = asInst<SISOPPInstruction>(ins);
	auto op = inst->GetOp();

	switch (op)
	{
	case SISOPPInstruction::S_INCPERFLEVEL:
		break;
	case SISOPPInstruction::S_DECPERFLEVEL:
		break;
	case SISOPPInstruction::S_TTRACEDATA:
	{
		// TODO:
		// The manual tells that 
		// this instruction writes the 32-bit contents of M0 to the thread-trace stream.
		// Currently I just lot it
		// LOG_DEBUG("S_TTRACEDATA value %X", m_stateRegs.m0);
	}
		break;
	default:
		LOG_ERR("Instruction category is not DebugProfile.");
		break;
	}
}

}  // namespace pssl