//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSIVINTRP.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for VINTRP (Interpolate data for the pixel shader) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSIVINTRP.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSIVINTRP.h"

VINTRPInstruction::VSRC ParserSIVINTRP::GetVSRC(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, VINTRP, vsrc, VSRC, 0);
    RETURN_EXTRACT_INSTRUCTION(vsrc);
}

VINTRPInstruction::ATTRCHAN ParserSIVINTRP::GetATTRCHAN(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, VINTRP, attrchan, ATTRCHAN, 8);
    RETURN_EXTRACT_INSTRUCTION(attrchan);
}

VINTRPInstruction::ATTR ParserSIVINTRP::GetATTR(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, VINTRP, attr, ATTR, 10);
    RETURN_EXTRACT_INSTRUCTION(attr);
}

SIVINTRPInstruction::OP ParserSIVINTRP::GetSIVINTRPOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, VINTRP, op, OP, 16);
    RETURN_EXTRACT_INSTRUCTION(op);
}

VIVINTRPInstruction::OP ParserSIVINTRP::GetVIVINTRPOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, VINTRP, op, OP, 16);
    RETURN_EXTRACT_INSTRUCTION(op);
}

VINTRPInstruction::VDST ParserSIVINTRP::GetVDST(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, VINTRP, vdst, VDST, 18);
    RETURN_EXTRACT_INSTRUCTION(vdst);
}

ParserSI::kaStatus ParserSIVINTRP::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, Instruction*& instruction, bool& hasLiteral)
{
    VINTRPInstruction::VSRC vsrc = GetVSRC(hexInstruction);
    VINTRPInstruction::ATTRCHAN attrchan = GetATTRCHAN(hexInstruction);
    VINTRPInstruction::ATTR attr = GetATTR(hexInstruction);
    VINTRPInstruction::VDST vdst = GetVDST(hexInstruction);

	hasLiteral = false;

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SIVINTRPInstruction::OP op = GetSIVINTRPOp(hexInstruction);
        instruction = new SIVINTRPInstruction(vsrc, attrchan, attr, op, vdst);
    }
    else
    {
        VIVINTRPInstruction::OP op = GetVIVINTRPOp(hexInstruction);
        instruction = new VIVINTRPInstruction(vsrc, attrchan, attr, op, vdst);
    }

    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSIVINTRP::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, Instruction*&)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}


