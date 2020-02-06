//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSISOP1.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for SOP1 (Scalar instruction with one input and one output. Can be followed by a 32-bit literal constant) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOP1.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOP1.h"

SOP1Instruction::SSRC ParserSISOP1::GetSSRC0(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP1, ssrc, SSRC, 0);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOP1Instruction::SSRC##FIELD_MIN) && (IN <= SOP1Instruction::SSRC##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOP1Instruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP1Instruction::SSRC##FIELD) \
    { \
        return SOP1Instruction::SSRC##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(ssrc, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= SOP1Instruction::SSRC##FIELD_MIN) && (IN <= SOP1Instruction::SSRC##FIELD_MAX)) \
    { \
        return SOP1Instruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP1Instruction::SSRC##FIELD) \
    { \
        return SOP1Instruction::SSRC##FIELD; \
    }
    SCALAR_INSTRUCTION_FIELDS(ssrc);
    GENERIC_INSTRUCTION_FIELDS_2(ssrc);
#undef X
#undef X_RANGE
    return SOP1Instruction::SSRCIllegal;
    //return ssrc;
}

SISOP1Instruction::OP ParserSISOP1::GetSISOP1Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP1, op, OP, 8);

    if ((op < SISOP1Instruction::S_MOV_B32)
        || (op > SISOP1Instruction::S_RFE_B64 && op < SISOP1Instruction::S_AND_SAVEEXEC_B64)
        || (op > SISOP1Instruction::S_CBRANCH_JOIN && op < SISOP1Instruction::S_ABS_I32)
        || (op >= SISOP1Instruction::S_RESERVED))
    {
        return SISOP1Instruction::S_RESERVED;
    }
    else
    {
        return op;
    }
}

VISOP1Instruction::OP ParserSISOP1::GetVISOP1Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOP1, op, OP, 8);

    if (op >= VISOP1Instruction::s_Illegal)
    {
        return VISOP1Instruction::s_Illegal;
    }
    else
    {
        return op;
    }
}

G9SOP1Instruction::OP ParserSISOP1::GetG9SOP1Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, G9, SOP1, op, OP, 8);

    return (op < G9SOP1Instruction::s_Illegal ? op : G9SOP1Instruction::s_Illegal);
}

SOP1Instruction::SDST ParserSISOP1::GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP1, sdst, SDST, 16);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOP1Instruction::SDST##FIELD_MIN) && (IN <= SOP1Instruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOP1Instruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP1Instruction::SDST##FIELD) \
    { \
        return SOP1Instruction::SDST##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE
    return SOP1Instruction::SDSTIllegal;
}

Instruction::InstructionCategory ParserSISOP1::GetSIOPCategory(SISOP1Instruction::OP op)
{
	if ((op >= SISOP1Instruction::S_GETPC_B64 && op <= SISOP1Instruction::S_SWAPPC_B64) ||
		op == SISOP1Instruction::S_CBRANCH_JOIN)
	{
		return Instruction::FlowControl;
	}
	return Instruction::ScalarALU;
}

const GCNInstructionFormat& ParserSISOP1::GetSISOP1Meta(SISOP1Instruction::OP op)
{
	return g_instructionFormatMapSOP1[op];
}


ParserSI::kaStatus ParserSISOP1::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
    ParserSI::kaStatus status = ParserSI::Status_SUCCESS;
    unsigned int ridx0 = 0, sdstRidx1 = 0;
    SOP1Instruction::SSRC ssrc0 = GetSSRC0(hexInstruction, ridx0);
    SOP1Instruction::SDST sdst = GetSDST(hexInstruction, sdstRidx1);
	
	hasLiteral = (ssrc0 == SOP1Instruction::SSRCLiteralConst);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SISOP1Instruction::OP op = GetSISOP1Op(hexInstruction);
		Instruction::InstructionCategory insCat = GetSIOPCategory(op);
		auto& meta = GetSISOP1Meta(op);
		instruction = std::make_unique<SISOP1Instruction>(ssrc0, op, sdst, ridx0, sdstRidx1, insCat, meta.insClass, meta.operandType);
    }
    else if (hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        VISOP1Instruction::OP op = GetVISOP1Op(hexInstruction);
        instruction = std::make_unique<VISOP1Instruction>(ssrc0, op, sdst, ridx0, sdstRidx1);
    }
    else if (hwGen == GDT_HW_GENERATION_GFX9)
    {
        G9SOP1Instruction::OP op = GetG9SOP1Op(hexInstruction);
        instruction = std::make_unique<G9SOP1Instruction>(ssrc0, op, sdst, ridx0, sdstRidx1);
    }
    else
    {
        status = ParserSI::Status_UnexpectedHWGeneration;
    }

    return status;
}

ParserSI::kaStatus ParserSISOP1::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, std::unique_ptr<Instruction>&)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}
