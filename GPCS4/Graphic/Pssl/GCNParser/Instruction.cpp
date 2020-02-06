#include "Instruction.h"
#include "GPCS4Common.h"

// C++.
#include <algorithm>
#include <sstream>

LOG_CHANNEL(Graphic.Pssl.GCNParser);

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

Instruction::OperandType Instruction::GetInstructionOperandType() const
{
	LOG_ASSERT(m_instructionOperandType != TypeNone, "instruction operand type not initialized");
	return m_instructionOperandType;
}

Instruction::InstructionClass Instruction::GetInstructionClass() const
{
	LOG_ASSERT(m_instructionClass != Instruction::InstructionClassUnknown, "instruction class not initialized");
	return m_instructionClass;
}
