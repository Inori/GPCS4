#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <set>

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4309)
#endif
#include "ParserSI.h"

#ifdef _WIN32
#pragma warning(pop)
#endif

class GCNParser
{
public:
	GCNParser(ParserSI::LoggingCallBackFuncP logFunc = NULL);
	~GCNParser();

	/// Parse the 32 instruction.
	/// \returns true if the instruction`s parsing succeeded.
	bool Parse(Instruction*& pOutInstruction, GDT_HW_GENERATION asicGen, Instruction::instruction32bit hexInstruction, bool& hasLiteral);

	/// Parse the 64 instruction.
	/// \returns true if the instruction`s parsing succeeded.
	bool Parse(Instruction*& pOutInstruction, GDT_HW_GENERATION asicGen, Instruction::instruction64bit hexInstruction);

	unsigned int GetVgprs() const { return m_vgprs; }
	unsigned int GetSgprs() const { return m_sgprs; }
	unsigned int GetCodeLen() const { return m_CodeLen; }

private:

	unsigned int m_sgprs;
	unsigned int m_vgprs;
	unsigned int m_CodeLen;

	/// The map between Parser`s instruction kind identifier and the parser
	std::map<Instruction::InstructionSet, ParserSI*> m_parsersSI;
};

