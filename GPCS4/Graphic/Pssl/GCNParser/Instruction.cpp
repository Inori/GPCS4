#include "Instruction.h"
#include "GPCS4Common.h"

// C++.
#include <algorithm>
#include <sstream>

Instruction::Instruction(
	unsigned int instructionWidth, 
	InstructionCategory instructionFormatKind, 
	InstructionSet instructionFormat, 
	InstructionClass instructionClass, /*= InstructionClassUnknown*/
	unsigned int instructionOperandWidth /*= 0*/) :
    m_instructionWidth(instructionWidth), 
	m_instructionCategory(instructionFormatKind), 
	m_instructionFormat(instructionFormat), 
	m_HwGen(GDT_HW_GENERATION_SOUTHERNISLAND),
	m_instructionClass(instructionClass),
	m_instructionOperandWidth(instructionOperandWidth)
{
}

unsigned int Instruction::GetInstructionOperandWidth() const
{
	LOG_ASSERT(m_instructionOperandWidth != 0, "instruction operand width not initialized");
	return m_instructionOperandWidth;
}

Instruction::InstructionClass Instruction::GetInstructionClass() const
{
	LOG_ASSERT(m_instructionClass != Instruction::InstructionClassUnknown, "instruction class not initialized");
	return m_instructionClass;
}
