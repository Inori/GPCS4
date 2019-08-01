//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSISOPP.cpp
/// \author GPU Developer Tools
/// \version $Revision: #6 $
/// \brief Description: Parser for SOPP (Scalar instruction taking one inline constant input and performing a special operation (for example: jump) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOPP.cpp#6 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOPP.h"


SOPPInstruction::SIMM16 ParserSISOPP::GetSIMM16(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPP, simm16, SIMM16, 0);
    RETURN_EXTRACT_INSTRUCTION(simm16);
}


SISOPPInstruction::OP ParserSISOPP::GetSISOPPOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPP, op, OP, 16);

    if ((op > SISOPPInstruction::S_BRANCH && op < SISOPPInstruction::S_CBRANCH_SCC0))
    {
        return SISOPPInstruction::S_RESERVED;
    }
    else if ((op > SISOPPInstruction::S_BARRIER && op < SISOPPInstruction::S_WAITCNT) || (op > SISOPPInstruction::S_TTRACEDATA))
    {
        return SISOPPInstruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

VISOPPInstruction::OP ParserSISOPP::GetVISOPPOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOPP, op, OP, 16);

    if (op > VISOPPInstruction::S_ILLEGAL)
    {
        return VISOPPInstruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

Instruction::InstructionCategory ParserSISOPP::GetSIOPCaterory(SISOPPInstruction::OP op)
{
	if (op >= SISOPPInstruction::S_INCPERFLEVEL && op <= SISOPPInstruction::S_TTRACEDATA)
	{
		return Instruction::DebugProfile;
	}
	
	return Instruction::FlowControl;
}

Instruction::InstructionClass ParserSISOPP::GetSIOPClass(SISOPPInstruction::OP op)
{
	return g_instructionFormatMapSOPP[op].insClass;
}

ParserSI::kaStatus ParserSISOPP::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
    SOPPInstruction::SIMM16 simm16 = GetSIMM16(hexInstruction);

	hasLiteral = false;

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SISOPPInstruction::OP op = GetSISOPPOp(hexInstruction);
		Instruction::InstructionCategory insCat = GetSIOPCaterory(op);
		Instruction::InstructionClass insCls = GetSIOPClass(op);
        instruction = std::make_unique<SISOPPInstruction>(simm16, op, insCat, insCls);
    }
    else
    {
        VISOPPInstruction::OP op = GetVISOPPOp(hexInstruction);
        instruction = std::make_unique<VISOPPInstruction>(simm16, op);
    }



    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSISOPP::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, std::unique_ptr<Instruction>&)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}
