#include "Instruction.h"

// C++.
#include <algorithm>
#include <sstream>

Instruction::Instruction(unsigned int instructionWidth, InstructionCategory instructionFormatKind, InstructionSet instructionFormat, InstructionClass instructionClass /*= InstructionClassUnknown*/) :
    m_instructionWidth(instructionWidth), 
	m_instructionCategory(instructionFormatKind), 
	m_instructionFormat(instructionFormat), 
	m_HwGen(GDT_HW_GENERATION_SOUTHERNISLAND),
	m_instructionClass(instructionClass)
{
}

