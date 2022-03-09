//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSISOPK.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for SOPK (Scalar instruction with one inline constant input and one output) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOPK.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOPK.h"

SOPKInstruction::SIMM16 ParserSISOPK::GetSIMM16(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPK, simm16, SIMM16, 0);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOPKInstruction::SIMM16##FIELD_MIN) && (IN <= SOPKInstruction::SIMM16##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOPKInstruction::SIMM16##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPKInstruction::SIMM16##FIELD) \
    { \
        return SOPKInstruction::SIMM16##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(simm16, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN, FIELD_MAX, FIELD, IN, VAL)\
    if ((IN >= SOPKInstruction::SIMM16##FIELD_MIN) && (IN <= SOPKInstruction::SIMM16##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOPKInstruction::SIMM16##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPKInstruction::SIMM16##FIELD) \
    { \
        return SOPKInstruction::SIMM16##FIELD; \
    }
    SCALAR_INSTRUCTION_FIELDS(simm16, ridx);
	GENERIC_INSTRUCTION_FIELDS_2(simm16, ridx);
#undef X
#undef X_RANGE
    return SOPKInstruction::SIMM16Illegal;
}

SISOPKInstruction::OP ParserSISOPK::GetSISOPKOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPK, op, OP, 23);

    if ((op > SISOPKInstruction::S_MOVK_I32 && op < SISOPKInstruction::S_CMOVK_I32)
        || (op >= SISOPKInstruction::S_RESERVED))
    {
        return SISOPKInstruction::S_RESERVED;
    }
    else
    {
        return op;
    }
}

VISOPKInstruction::OP ParserSISOPK::GetVISOPKOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOPK, op, OP, 23);

    if (op > VISOPKInstruction::S_ILLEGAL)
    {
        return VISOPKInstruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

G9SOPKInstruction::OP ParserSISOPK::GetG9SOPKOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, G9, SOPK, op, OP, 23);

    return (op < G9SOPKInstruction::S_ILLEGAL ? op : G9SOPKInstruction::S_ILLEGAL);
}

SOPKInstruction::SDST ParserSISOPK::GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOPK, sdst, SDST, 16);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOPKInstruction::SDST##FIELD_MIN) && (IN <= SOPKInstruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOPKInstruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOPKInstruction::SDST##FIELD) \
    { \
        return SOPKInstruction::SDST##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE
    return SOPKInstruction::SDSTIllegal;
}

const GCNInstructionFormat& ParserSISOPK::GetSISOPKMeta(SISOPKInstruction::OP op)
{
	return g_instructionFormatMapSOPK[op];
}

ParserSI::kaStatus ParserSISOPK::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
    kaStatus status = Status_SUCCESS;
    unsigned int simm16Ridx = 0, sdstRidx = 0;
    SOPKInstruction::SIMM16 simm16 = GetSIMM16(hexInstruction, simm16Ridx);
    SOPKInstruction::SDST sdst = GetSDST(hexInstruction, sdstRidx);

    switch (hwGen)
    {
        case GDT_HW_GENERATION_SEAISLAND:
        case GDT_HW_GENERATION_SOUTHERNISLAND:
        {
            SISOPKInstruction::OP op = GetSISOPKOp(hexInstruction);
			auto meta = GetSISOPKMeta(op);
			instruction = std::make_unique<SISOPKInstruction>(simm16, op, sdst, simm16Ridx, sdstRidx, meta.insClass, meta.operandType);
			hasLiteral = (op == SISOPKInstruction::S_SETREG_IMM32_B32);
            break;
        }
        case GDT_HW_GENERATION_VOLCANICISLAND:
        {
            VISOPKInstruction::OP op = GetVISOPKOp(hexInstruction);
            instruction = std::make_unique<VISOPKInstruction>(simm16, op, sdst, simm16Ridx, sdstRidx);
			hasLiteral = (op == VISOPKInstruction::s_setreg_imm32_b32);
            break;
        }
        case GDT_HW_GENERATION_GFX9:
        {
            G9SOPKInstruction::OP op = GetG9SOPKOp(hexInstruction);
            instruction = std::make_unique<G9SOPKInstruction>(simm16, op, sdst, simm16Ridx, sdstRidx);
			hasLiteral = (op == G9SOPKInstruction::s_setreg_imm32_b32);
            break;
        }
        default:
            status = Status_UnexpectedHWGeneration;
    }

    return status;
}

ParserSI::kaStatus ParserSISOPK::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, std::unique_ptr<Instruction>&)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}

