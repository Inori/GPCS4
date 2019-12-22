//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSISOP2.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for SOP2 (Scalar instruction with two inputs and one output. Can be followed by a 32-bit literal constant) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISOP2.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSISOP2.h"

SOP2Instruction::SSRC ParserSISOP2::GetSSRC(Instruction::instruction32bit hexInstruction, unsigned int& ridx, unsigned int idxSSRC)
{
    SOP2Instruction::SSRC ssrc = (SOP2Instruction::SSRC)0;

    switch (idxSSRC)
    {
        case 0 :
            ssrc = static_cast<SOP2Instruction::SSRC>(hexInstruction & static_cast<Instruction::instruction32bit>(SOP2Mask_SSRC0));
            break;

        case 1:
            ssrc = static_cast<SOP2Instruction::SSRC>((hexInstruction & static_cast<Instruction::instruction32bit>(SOP2Mask_SSRC1)) >> 8);
            break;
    }

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOP2Instruction::SSRC##FIELD_MIN) && (IN <= SOP2Instruction::SSRC##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOP2Instruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP2Instruction::SSRC##FIELD) \
    { \
        return SOP2Instruction::SSRC##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(ssrc, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= SOP2Instruction::SSRC##FIELD_MIN) && (IN <= SOP2Instruction::SSRC##FIELD_MAX)) \
    { \
        return SOP2Instruction::SSRC##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP2Instruction::SSRC##FIELD) \
    { \
        return SOP2Instruction::SSRC##FIELD; \
    }
    SCALAR_INSTRUCTION_FIELDS(ssrc);
    GENERIC_INSTRUCTION_FIELDS_2(ssrc);
#undef X
#undef X_RANGE
    return SOP2Instruction::SSRCIllegal;
}

SISOP2Instruction::OP ParserSISOP2::GetSISOP2Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP2, op, OP, 23);

    if ((op > SISOP2Instruction::S_CSELECT_B64 && op < SISOP2Instruction::S_AND_B32)
        || (op >= SISOP2Instruction::S_RESERVED))
    {
        return SISOP2Instruction::S_RESERVED;
    }
    else
    {
        return op;
    }
}

VISOP2Instruction::OP ParserSISOP2::GetVISOP2Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, SOP2, op, OP, 23);

    if (op > VISOP2Instruction::S_ILLEGAL)
    {
        return VISOP2Instruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

G9SOP2Instruction::OP ParserSISOP2::GetG9SOP2Op(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, G9, SOP2, op, OP, 23);

    if (op > G9SOP2Instruction::S_ILLEGAL)
    {
        return G9SOP2Instruction::S_ILLEGAL;
    }
    else
    {
        return op;
    }
}

const GCNInstructionFormat& ParserSISOP2::GetSISOP2Meta(SISOP2Instruction::OP op)
{
	return g_instructionFormatMapSOP2[op];
}


SOP2Instruction::SDST ParserSISOP2::GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SOP2, sdst, SDST, 16);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SOP2Instruction::SDST##FIELD_MIN) && (IN <= SOP2Instruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SOP2Instruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SOP2Instruction::SDST##FIELD) \
    { \
        return SOP2Instruction::SDST##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE
    return SOP2Instruction::SDSTIllegal;
}

ParserSI::kaStatus ParserSISOP2::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
    ParserSI::kaStatus status = ParserSI::Status_SUCCESS;
    unsigned int ridx0 = 0, ridx1 = 0, sdstRidx = 0;
    SOP2Instruction::SSRC ssrc0 = GetSSRC(hexInstruction, ridx0, 0);
    SOP2Instruction::SSRC ssrc1 = GetSSRC(hexInstruction, ridx1, 1);
    SOP2Instruction::SDST sdst = GetSDST(hexInstruction, sdstRidx);

	hasLiteral = (ssrc0 == SOP2Instruction::SSRCLiteralConst) || (ssrc1 == SOP2Instruction::SSRCLiteralConst);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SISOP2Instruction::OP op = GetSISOP2Op(hexInstruction);
		auto& meta = GetSISOP2Meta(op);
		instruction = std::make_unique<SISOP2Instruction>(ssrc0, ssrc1, sdst, op, ridx0, ridx1, sdstRidx, meta.insClass, meta.operandType);
    }
    else if (hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        VISOP2Instruction::OP op = GetVISOP2Op(hexInstruction);
        instruction = std::make_unique<VISOP2Instruction>(ssrc0, ssrc1, sdst, op, ridx0, ridx1, sdstRidx);
    }
    else if (hwGen == GDT_HW_GENERATION_GFX9)
    {
        G9SOP2Instruction::OP op = GetG9SOP2Op(hexInstruction);
        instruction = std::make_unique<G9SOP2Instruction>(ssrc0, ssrc1, sdst, op, ridx0, ridx1, sdstRidx);
    }
    else
    {
        status = ParserSI::Status_UnexpectedHWGeneration;
    }

    return status;
}

ParserSI::kaStatus ParserSISOP2::Parse(GDT_HW_GENERATION, Instruction::instruction64bit, std::unique_ptr<Instruction>&)
{
    return ParserSI::Status_64BitInstructionNotSupported;
}

