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
	OperandType instructionOperandType /*= TypeNone*/) :
    m_instructionWidth(instructionWidth), 
	m_instructionCategory(instructionFormatKind), 
	m_instructionFormat(instructionFormat), 
	m_HwGen(GDT_HW_GENERATION_SOUTHERNISLAND),
	m_instructionClass(instructionClass),
	m_instructionOperandType(instructionOperandType)
{
}

unsigned int Instruction::GetInstructionOperandType() const
{
	LOG_ASSERT(m_instructionOperandType != 0, "instruction operand width not initialized");
	return m_instructionOperandType;
}

Instruction::InstructionClass Instruction::GetInstructionClass() const
{
	LOG_ASSERT(m_instructionClass != Instruction::InstructionClassUnknown, "instruction class not initialized");
	return m_instructionClass;
}
