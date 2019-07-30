//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSIEXP.cpp
/// \author GPU Developer Tools
/// \version $Revision: #4 $
/// \brief Description: Parser for EXP (Export (output) pixel color, pixel depth, vertex position, or vertex parameter data) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSIEXP.cpp#4 $
// Last checkin:   $DateTime: 2014/07/10 11:38:24 $
// Last edited by: $Author: ekatz $
// Change list:    $Change: 499997 $
//=============================================================

#include "ParserSIEXP.h"

EXPInstruction::EN ParserSIEXP::GetEn(Instruction::instruction64bit hexInstruction)
{
    EXPInstruction::EN en = static_cast<EXPInstruction::EN>(hexInstruction & EXPMask_EN);
    return en;
}

EXPInstruction::TGT ParserSIEXP::GetTGT(Instruction::instruction64bit hexInstruction)
{
    EXPInstruction::TGT tgt = static_cast<EXPInstruction::TGT>((hexInstruction & EXPMask_TGT) >> 4);

    /// tgt 0 - 63 has not reserved values
    if (tgt >= EXPInstruction::TGTExpReserved)
    {
        return EXPInstruction::TGTExpReserved;
    }

    return tgt;
}

EXPInstruction::COMPR ParserSIEXP::GetCOMPR(Instruction::instruction64bit hexInstruction)
{
    EXPInstruction::COMPR compr = static_cast<EXPInstruction::COMPR>((hexInstruction & EXPMask_COMPR) >> 10);
    return compr;
}

EXPInstruction::DONE ParserSIEXP::GetDone(Instruction::instruction64bit hexInstruction)
{
    EXPInstruction::DONE done = static_cast<EXPInstruction::DONE>((hexInstruction & EXPMask_DONE) >> 11);
    return done;
}


EXPInstruction::VM ParserSIEXP::GetVM(Instruction::instruction64bit hexInstruction)
{
    EXPInstruction::VM vm = static_cast<EXPInstruction::VM>((hexInstruction & EXPMask_VM) >> 12);
    return vm;
}

EXPInstruction::VSRC ParserSIEXP::GetVSRC(Instruction::instruction64bit hexInstruction, const unsigned int vsrcIdx)
{
    EXPInstruction::VSRC vsrc = 0;
    Instruction::instruction64bit EXPMaskVSRC;

    switch (vsrcIdx)
    {
        case 0:
            EXPMaskVSRC = (static_cast<Instruction::instruction64bit>(EXPMask_VSRC0)) << 32;
            vsrc = static_cast<EXPInstruction::VM>((hexInstruction & EXPMaskVSRC) >> 32);
            break;

        case 1:
            EXPMaskVSRC = (static_cast<Instruction::instruction64bit>(EXPMask_VSRC1)) << 32;
            vsrc = static_cast<EXPInstruction::VM>((hexInstruction & EXPMaskVSRC) >> 40);
            break;

        case 2:
            EXPMaskVSRC = (static_cast<Instruction::instruction64bit>(EXPMask_VSRC2)) << 32;
            vsrc = static_cast<EXPInstruction::VM>((hexInstruction & EXPMaskVSRC) >> 48);
            break;

        case 3:
            EXPMaskVSRC = (static_cast<Instruction::instruction64bit>(EXPMask_VSRC3)) << 32;
            vsrc = static_cast<EXPInstruction::VM>((hexInstruction & EXPMaskVSRC) >> 56);
            break;
    }

    return vsrc;
}

ParserSI::kaStatus ParserSIEXP::Parse(GDT_HW_GENERATION, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction)
{
    EXPInstruction::VSRC vsrc[4];
    EXPInstruction::EN en = GetEn(hexInstruction);
    EXPInstruction::TGT tgt = GetTGT(hexInstruction);
    EXPInstruction::COMPR compr = GetCOMPR(hexInstruction);
    EXPInstruction::DONE done = GetDone(hexInstruction);
    EXPInstruction::VM vm = GetVM(hexInstruction);
    vsrc[0] = GetVSRC(hexInstruction, 0);
    vsrc[1] = GetVSRC(hexInstruction, 1);
    vsrc[2] = GetVSRC(hexInstruction, 2);
    vsrc[3] = GetVSRC(hexInstruction, 3);

    instruction = std::make_unique<EXPInstruction>(en, tgt, compr, done, vm, vsrc[0], vsrc[1], vsrc[2], vsrc[3]);
    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSIEXP::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
	hasLiteral = false;
    return ParserSI::Status_32BitInstructionNotSupported;
}

