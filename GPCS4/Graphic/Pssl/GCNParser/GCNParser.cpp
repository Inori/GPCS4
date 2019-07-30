#include "GCNParser.h"


#include "ParserSISOP2.h"
#include "ParserSISOPK.h"
#include "ParserSISOP1.h"
#include "ParserSISOPP.h"
#include "ParserSISOPC.h"
#include "ParserSISMRD.h"
#include "ParserSIVINTRP.h"
#include "ParserSIDS.h"
#include "ParserSIMUBUF.h"
#include "ParserSIMTBUF.h"
#include "ParserSIMIMG.h"
#include "ParserSIEXP.h"
#include "ParserSIVOP.h"
#include "ParserFLAT.h"


GCNParser::GCNParser(ParserSI::LoggingCallBackFuncP logFunc) : m_sgprs(0), m_vgprs(0), m_CodeLen(0)
{
	ParserSI::SetLog(logFunc);
	m_parsersSI[Instruction::InstructionSet_SOP2] = new ParserSISOP2();
	m_parsersSI[Instruction::InstructionSet_SOPK] = new ParserSISOPK();
	m_parsersSI[Instruction::InstructionSet_SOP1] = new ParserSISOP1();
	m_parsersSI[Instruction::InstructionSet_SOPC] = new ParserSISOPC();
	m_parsersSI[Instruction::InstructionSet_SOPP] = new ParserSISOPP();
	m_parsersSI[Instruction::InstructionSet_SMRD] = new ParserSISMRD();
	m_parsersSI[Instruction::InstructionSet_VINTRP] = new ParserSIVINTRP();
	m_parsersSI[Instruction::InstructionSet_DS] = new ParserSIDS();
	m_parsersSI[Instruction::InstructionSet_MUBUF] = new ParserSIMUBUF();
	m_parsersSI[Instruction::InstructionSet_MTBUF] = new ParserSIMTBUF();
	m_parsersSI[Instruction::InstructionSet_MIMG] = new ParserSIMIMG();
	m_parsersSI[Instruction::InstructionSet_EXP] = new ParserSIEXP();
	m_parsersSI[Instruction::InstructionSet_VOP] = new ParserSIVOP();
	m_parsersSI[Instruction::InstructionSet_FLAT] = new ParserFLAT();
}


GCNParser::~GCNParser()
{
	ParserSI::SetLog(NULL);
	delete m_parsersSI[Instruction::InstructionSet_SOP2];
	delete m_parsersSI[Instruction::InstructionSet_SOPK];
	delete m_parsersSI[Instruction::InstructionSet_SOP1];
	delete m_parsersSI[Instruction::InstructionSet_SOPC];
	delete m_parsersSI[Instruction::InstructionSet_SOPP];
	delete m_parsersSI[Instruction::InstructionSet_SMRD];
	delete m_parsersSI[Instruction::InstructionSet_VINTRP];
	delete m_parsersSI[Instruction::InstructionSet_DS];
	delete m_parsersSI[Instruction::InstructionSet_MUBUF];
	delete m_parsersSI[Instruction::InstructionSet_MTBUF];
	delete m_parsersSI[Instruction::InstructionSet_MIMG];
	delete m_parsersSI[Instruction::InstructionSet_EXP];
	delete m_parsersSI[Instruction::InstructionSet_VOP];
	delete m_parsersSI[Instruction::InstructionSet_FLAT];
}

bool GCNParser::Parse(std::unique_ptr<Instruction>& outInstruction, GDT_HW_GENERATION asicGen, Instruction::instruction32bit hexInstruction, bool& hasLiteral)
{
	ParserSI::InstructionEncoding instructionEncoding = ParserSI::GetInstructionEncoding(hexInstruction);

	if (instructionEncoding == ParserSI::InstructionEncoding_SOP2)
	{
		m_parsersSI[Instruction::InstructionSet_SOP2]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if (instructionEncoding == ParserSI::InstructionEncoding_SOPK)
	{
		m_parsersSI[Instruction::InstructionSet_SOPK]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if (instructionEncoding == ParserSI::InstructionEncoding_SOP1)
	{
		m_parsersSI[Instruction::InstructionSet_SOP1]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if (instructionEncoding == ParserSI::InstructionEncoding_SOPC)
	{
		m_parsersSI[Instruction::InstructionSet_SOPC]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if (instructionEncoding == ParserSI::InstructionEncoding_SOPP)
	{
		m_parsersSI[Instruction::InstructionSet_SOPP]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if ((instructionEncoding == ParserSI::InstructionEncoding_SMRD) || (instructionEncoding == ParserSI::VIInstructionEncoding_SMEM))
	{
		m_parsersSI[Instruction::InstructionSet_SMRD]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if ((instructionEncoding == ParserSI::InstructionEncoding_VOP2) ||
		(instructionEncoding == ParserSI::InstructionEncoding_VOP1) ||
		(instructionEncoding == ParserSI::InstructionEncoding_VOPC))
	{
		m_parsersSI[Instruction::InstructionSet_VOP]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}
	else if ((instructionEncoding == ParserSI::InstructionEncoding_VINTRP) ||
		(instructionEncoding == ParserSI::VIInstructionEncoding_VINTRP))
	{
		m_parsersSI[Instruction::InstructionSet_VINTRP]->Parse(asicGen, hexInstruction, outInstruction, hasLiteral);
	}

	return (outInstruction != NULL);
}

bool GCNParser::Parse(std::unique_ptr<Instruction>& outInstruction, GDT_HW_GENERATION asicGen, Instruction::instruction64bit hexInstruction)
{
	/// Instruction encoding appears only in low 32 bits of instruction
	ParserSI::InstructionEncoding instructionEncoding = ParserSI::GetInstructionEncoding(static_cast<Instruction::instruction32bit>(hexInstruction));

	switch (instructionEncoding)
	{
	case ParserSI::InstructionEncoding_DS:
		m_parsersSI[Instruction::InstructionSet_DS]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::InstructionEncoding_MUBUF:
		m_parsersSI[Instruction::InstructionSet_MUBUF]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::InstructionEncoding_MIMG:
		m_parsersSI[Instruction::InstructionSet_MIMG]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::InstructionEncoding_MTBUF:
		m_parsersSI[Instruction::InstructionSet_MTBUF]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::InstructionEncoding_EXP:
		m_parsersSI[Instruction::InstructionSet_EXP]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::InstructionEncoding_VOP3:
		m_parsersSI[Instruction::InstructionSet_VOP]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::VIInstructionEncoding_SMEM:
		m_parsersSI[Instruction::InstructionSet_SMRD]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	case ParserSI::VIInstructionEncoding_FLAT:
		m_parsersSI[Instruction::InstructionSet_FLAT]->Parse(asicGen, hexInstruction, outInstruction);
		break;

	default:
		break;
	}

	return (outInstruction != NULL);
}
