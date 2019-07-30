//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSIMTBUF.cpp
/// \author GPU Developer Tools
/// \version $Revision: #8 $
/// \brief Description: Parser for MTBUF (Typed memory buffer operation) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSIMTBUF.cpp#8 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSIMTBUF.h"

MTBUFInstruction::OFFSET
ParserSIMTBUF::GetOFFSET(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, offset, OFFSET, 0);
    RETURN_EXTRACT_INSTRUCTION(offset);
}

MTBUFInstruction::OFFEN
ParserSIMTBUF::GetOFFEN(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, offen, OFFEN, 12);
    RETURN_EXTRACT_INSTRUCTION(offen);
}

MTBUFInstruction::IDXEN
ParserSIMTBUF::GetIDXEN(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, idxen, IDXEN, 13);
    RETURN_EXTRACT_INSTRUCTION(idxen);
}


MTBUFInstruction::GLC
ParserSIMTBUF::GetGLC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, glc, GLC, 14);
    RETURN_EXTRACT_INSTRUCTION(glc);
}


MTBUFInstruction::ADDR64
ParserSIMTBUF::GetADDR64(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, addr64, ADDR64, 15);
    RETURN_EXTRACT_INSTRUCTION(addr64);
}


SIMTBUFInstruction::OP
ParserSIMTBUF::GetSIOpMTBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, op, OP, 16);

    if (op <= SIMTBUFInstruction::TBUFFER_LOAD_FORMAT_XYZW)
    {
        instKind = Instruction::VectorMemoryRead;
    }
    else
    {
        instKind = Instruction::VectorMemoryWrite;
    }

    if ((op >= SIMTBUFInstruction::TBUFFER_RESERVED))
    {
        return SIMTBUFInstruction::TBUFFER_RESERVED;
    }
    else
    {
        return op;
    }
}

VIMTBUFInstruction::OP
ParserSIMTBUF::GetVIOpMTBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, MTBUF, op, OP, 16);

    if (((op <= VIMTBUFInstruction::tbuffer_load_format_xyzw) || (op >= VIMTBUFInstruction::tbuffer_load_format_d16_xyzw)) && (op <= VIMTBUFInstruction::tbuffer_load_format_d16_x))
    {
        instKind = Instruction::VectorMemoryRead;
    }
    else
    {
        instKind = Instruction::VectorMemoryWrite;
    }

    if ((op >= VIMTBUFInstruction::tbuffer_Ilegal))
    {
        return VIMTBUFInstruction::tbuffer_Ilegal;
    }
    else
    {
        return op;
    }
}


MTBUFInstruction::DFMT
ParserSIMTBUF::GetDFMT(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, dmft, ADDR64, 19);
    RETURN_EXTRACT_INSTRUCTION(dmft);
}

MTBUFInstruction::NFMT
ParserSIMTBUF::GetNFMT(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MTBUF, nmft, ADDR64, 15);
    RETURN_EXTRACT_INSTRUCTION(nmft);
}


MTBUFInstruction::VADDR
ParserSIMTBUF::GetVADDR(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MTBUF, vaddr, VADDR, 32);
    RETURN_EXTRACT_INSTRUCTION(vaddr);
}

MTBUFInstruction::VDATA
ParserSIMTBUF::GetVDATA(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MTBUF, vdata, VDATA, 40);
    RETURN_EXTRACT_INSTRUCTION(vdata);
}

MTBUFInstruction::SRSRC
ParserSIMTBUF::GetSRSRC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MTBUF, srsrc, SRSRC, 48);
    RETURN_EXTRACT_INSTRUCTION(srsrc);
}

MTBUFInstruction::SLC
ParserSIMTBUF::GetSLC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MTBUF, slc, SLC, 54);
    RETURN_EXTRACT_INSTRUCTION(slc);
}

MTBUFInstruction::TFE
ParserSIMTBUF::GetTFE(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MTBUF, tfe, TFE, 55);
    RETURN_EXTRACT_INSTRUCTION(tfe);
}


MTBUFInstruction::SOFFSET
ParserSIMTBUF::GetSOFFSET(Instruction::instruction64bit hexInstruction, unsigned int&)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MTBUF, soffset, SOFFSET, 56);
    RETURN_EXTRACT_INSTRUCTION(soffset);

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= MTBUFInstruction::SOFFSET##FIELD_MIN) && (IN <= MTBUFInstruction::SOFFSET##FIELD_MAX)) \
    { \
        VAL = IN; \
        return MTBUFInstruction::SOFFSET##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == MTBUFInstruction::SOFFSET##FIELD) \
    { \
        return MTBUFInstruction::SOFFSET##FIELD; \
    }
    //GENERIC_INSTRUCTION_FIELDS_1(soffset, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= MTBUFInstruction::SOFFSET##FIELD_MIN) && (IN <= MTBUFInstruction::SOFFSET##FIELD_MAX)) \
    { \
        return MTBUFInstruction::SOFFSET##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == MTBUFInstruction::SOFFSET##FIELD) \
    { \
        return MTBUFInstruction::SOFFSET##FIELD; \
    }
    //SCALAR_INSTRUCTION_FIELDS(soffset);
#undef X
#undef X_RANGE
    //return MTBUFInstruction::SOFFSETIllegal;
}

ParserSI::kaStatus
ParserSIMTBUF::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, Instruction*& instruction)
{
    Instruction::InstructionCategory instKind = Instruction::VectorMemoryRead;
    unsigned int ridx = 0;
    MTBUFInstruction::OFFSET offset = GetOFFSET(hexInstruction);
    MTBUFInstruction::OFFEN offen = GetOFFEN(hexInstruction);
    MTBUFInstruction::IDXEN idxen = GetIDXEN(hexInstruction);
    MTBUFInstruction::GLC glc = GetGLC(hexInstruction);
    MTBUFInstruction::ADDR64 addr64 = GetADDR64(hexInstruction);
    MTBUFInstruction::DFMT dfmt = GetDFMT(hexInstruction);
    MTBUFInstruction::NFMT nmft = GetNFMT(hexInstruction);
    MTBUFInstruction::VADDR vaddr = GetVADDR(hexInstruction);
    MTBUFInstruction::VDATA vdata = GetVDATA(hexInstruction);
    MTBUFInstruction::SRSRC srsrc = GetSRSRC(hexInstruction);
    MTBUFInstruction::SLC slc = GetSLC(hexInstruction);
    MTBUFInstruction::TFE tfe = GetTFE(hexInstruction);
    MTBUFInstruction::SOFFSET soffset = GetSOFFSET(hexInstruction, ridx);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SIMTBUFInstruction::OP op = GetSIOpMTBUF(hexInstruction, instKind);
        instruction = new SIMTBUFInstruction(offset, offen, idxen, glc, addr64, op, dfmt, nmft, vaddr, vdata, srsrc, slc,
                                             tfe, soffset, ridx, instKind);
    }
    else
    {
        VIMTBUFInstruction::OP op = GetVIOpMTBUF(hexInstruction, instKind);
        instruction = new VIMTBUFInstruction(offset, offen, idxen, glc, addr64, op, dfmt, nmft, vaddr, vdata, srsrc, slc,
                                             tfe, soffset, ridx, instKind);
    }

    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSIMTBUF::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, Instruction*& instruction, bool& hasLiteral)
{
	hasLiteral = false;
    return ParserSI::Status_32BitInstructionNotSupported;
}
