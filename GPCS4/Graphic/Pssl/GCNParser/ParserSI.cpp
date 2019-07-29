//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSI.cpp
/// \author GPU Developer Tools
/// \version $Revision: #5 $
/// \brief Description: Parser for SI ISA
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSI.cpp#5 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSI.h"

ParserSI::InstructionEncoding ParserSI::GetInstructionEncoding(Instruction::instruction32bit hexInstruction)
{
    Instruction::instruction32bit instructionEncoding;
    /// Try all instructions encoding masks from longest to shortest until
    /// legal InstructionEncoding is found.

    /// InstructionEncodingMask_9bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_9bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_SOP1:
        case InstructionEncoding_SOPP:
        case InstructionEncoding_SOPC:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// InstructionEncodingMask_7bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_7bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_VOP1:
        case InstructionEncoding_VOPC:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// InstructionEncodingMask_6bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_6bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_VOP3:
        case InstructionEncoding_EXP:
        case InstructionEncoding_VINTRP:
        case InstructionEncoding_DS:
        case InstructionEncoding_MUBUF:
        case InstructionEncoding_MTBUF:
        case InstructionEncoding_MIMG:
        case VIInstructionEncoding_SMEM:
        case VIInstructionEncoding_VINTRP:
        case VIInstructionEncoding_FLAT:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// InstructionEncodingMask_5bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_5bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_SMRD:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// InstructionEncodingMask_4bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_4bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_SOPK:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// InstructionEncodingMask_2bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_2bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_SOP2:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// InstructionEncodingMask_1bit
    instructionEncoding = (hexInstruction & InstructionEncodingMask_1bit);

    switch (instructionEncoding)
    {
        case InstructionEncoding_VOP2:
            return static_cast<InstructionEncoding>(instructionEncoding);

        default:
            break;
    }

    /// If no legal InstructionEncoding found return InstructionEncoding_ILLEGAL
    return InstructionEncoding_ILLEGAL;
}

void
ParserSI::SetLog(
    ParserSI::LoggingCallBackFuncP callback)
{
    m_LogCallback = callback;
}

ParserSI::LoggingCallBackFuncP ParserSI::m_LogCallback;
