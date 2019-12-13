#pragma once

#include "PsslCommon.h"
#include "GCNInstruction.h"

namespace pssl
{;

class GCNInstructionIterator
{
public:
	GCNInstructionIterator();
	virtual ~GCNInstructionIterator();

	virtual void processInstruction(GCNInstruction& ins) = 0;

protected:

	// Convenience function to dynamic cast instruction types
	template<typename InsType>
	inline
		typename std::enable_if<std::is_base_of<Instruction, InsType>::value, const InsType*>::type
		asInst(const GCNInstruction& ins)
	{
		return dynamic_cast<const InsType*>(ins.instruction.get());
	}

	void updateProgramCounter(const GCNInstruction& ins);

protected:
	// PC pointer, will be updated after processing each instruction.
	uint32_t m_programCounter = 0;

private:

};


	
}  // namespace pssl