//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSIMUBUF.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for MUBUF (Untyped memory buffer operation. First word with LDS, second word non-LDS) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSIMUBUF.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSIMUBUF.h"

MUBUFInstruction::OFFSET
ParserSIMUBUF::GetOFFSET(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, offset, OFFSET, 0);
    RETURN_EXTRACT_INSTRUCTION(offset);
}

MUBUFInstruction::OFFEN
ParserSIMUBUF::GetOFFEN(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, offen, OFFEN, 12);
    RETURN_EXTRACT_INSTRUCTION(offen);
}

MUBUFInstruction::IDXEN
ParserSIMUBUF::GetIDXEN(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, idxen, IDXEN, 13);
    RETURN_EXTRACT_INSTRUCTION(idxen);
}


MUBUFInstruction::GLC
ParserSIMUBUF::GetGLC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, glc, GLC, 14);
    RETURN_EXTRACT_INSTRUCTION(glc);
}


MUBUFInstruction::ADDR64
ParserSIMUBUF::GetLDS(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, lds, LDS, 16);
    RETURN_EXTRACT_INSTRUCTION(lds);
}

MUBUFInstruction::LDS
ParserSIMUBUF::GetADDR64(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, addr64, ADDR64, 15);
    RETURN_EXTRACT_INSTRUCTION(addr64);
}

SIMUBUFInstruction::OP
ParserSIMUBUF::GetSIOpMUBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MUBUF, op, OP, 18);

    if ((op >= SIMUBUFInstruction::BUFFER_LOAD_FORMAT_X && op <= SIMUBUFInstruction::BUFFER_LOAD_FORMAT_XYZW) ||
        (op >= SIMUBUFInstruction::BUFFER_LOAD_UBYTE && op <= SIMUBUFInstruction::BUFFER_LOAD_DWORDX4))
    {
        instKind = Instruction::VectorMemory;
    }
    else if ((op >= SIMUBUFInstruction::BUFFER_STORE_FORMAT_X && op <= SIMUBUFInstruction::BUFFER_STORE_FORMAT_XYZW) ||
             (op >= SIMUBUFInstruction::BUFFER_STORE_BYTE && op <= SIMUBUFInstruction::BUFFER_STORE_DWORDX4) ||
             (op >= SIMUBUFInstruction::BUFFER_WBINVL1_SC && op <= SIMUBUFInstruction::BUFFER_WBINVL1))
    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= SIMUBUFInstruction::BUFFER_ATOMIC_SWAP && op <= SIMUBUFInstruction::BUFFER_ATOMIC_FMAX_X2)
    {
        instKind = Instruction::VectorMemory;
    }

    if ((op > SIMUBUFInstruction::BUFFER_LOAD_DWORDX3 && op < SIMUBUFInstruction::BUFFER_STORE_BYTE)
        || (op > SIMUBUFInstruction::BUFFER_STORE_BYTE && op < SIMUBUFInstruction::BUFFER_STORE_SHORT) 
		|| (op > SIMUBUFInstruction::BUFFER_STORE_SHORT && op < SIMUBUFInstruction::BUFFER_STORE_DWORD) 
		|| (op > SIMUBUFInstruction::BUFFER_STORE_DWORDX3 && op < SIMUBUFInstruction::BUFFER_ATOMIC_SWAP) 
		|| (op > SIMUBUFInstruction::BUFFER_ATOMIC_FMAX && op < SIMUBUFInstruction::BUFFER_ATOMIC_SWAP_X2)
        || (op > SIMUBUFInstruction::BUFFER_ATOMIC_FMAX_X2 && op < SIMUBUFInstruction::BUFFER_WBINVL1_SC)
        || (op >= SIMUBUFInstruction::BUFFER_RESERVED))
    {
        return SIMUBUFInstruction::BUFFER_RESERVED;
    }
    else
    {
        return op;
    }
}

VIMUBUFInstruction::OP
ParserSIMUBUF::GetVIOpMUBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, MUBUF, op, OP, 18);

    if ((op >= VIMUBUFInstruction::buffer_load_format_x && op <= VIMUBUFInstruction::buffer_load_format_xyzw) ||
        (op >= VIMUBUFInstruction::buffer_load_format_d16_x && op <= VIMUBUFInstruction::buffer_load_format_d16_xyzw) ||
        (op >= VIMUBUFInstruction::buffer_load_ubyte && op <= VIMUBUFInstruction::buffer_load_dwordx4))
    {
        instKind = Instruction::VectorMemory;
    }
    else if ((op >= VIMUBUFInstruction::buffer_store_format_x && op <= VIMUBUFInstruction::buffer_store_format_xyzw) ||
             (op >= VIMUBUFInstruction::buffer_store_format_d16_x && op <= VIMUBUFInstruction::buffer_store_format_d16_xyzw) ||
             (op >= VIMUBUFInstruction::buffer_store_byte && op <= VIMUBUFInstruction::buffer_wbinvl1_vol))
    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= VIMUBUFInstruction::buffer_atomic_swap && op <= VIMUBUFInstruction::buffer_atomic_dec_x2)
    {
        instKind = Instruction::VectorMemory;
    }

    if (op > VIMUBUFInstruction::buffer_Ilegal)
    {
        return VIMUBUFInstruction::buffer_Ilegal;
    }
    else
    {
        return op;
    }
}

G9MUBUFInstruction::OP
ParserSIMUBUF::GetG9OpMUBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, G9, MUBUF, op, OP, 18);

    if ((op >= G9MUBUFInstruction::buffer_load_format_x && op <= G9MUBUFInstruction::buffer_load_format_xyzw) ||
        (op >= G9MUBUFInstruction::buffer_load_format_d16_x && op <= G9MUBUFInstruction::buffer_load_format_d16_xyzw) ||
        (op >= G9MUBUFInstruction::buffer_load_ubyte && op <= G9MUBUFInstruction::buffer_load_dwordx4) ||
        (op >= G9MUBUFInstruction:: buffer_load_ubyte_d16 && op <= G9MUBUFInstruction::buffer_load_format_d16_hi_x))
    {
        instKind = Instruction::VectorMemory;
    }
    else if ((op >= G9MUBUFInstruction::buffer_store_format_x && op <= G9MUBUFInstruction::buffer_store_format_xyzw) ||
             (op >= G9MUBUFInstruction::buffer_store_format_d16_x && op <= G9MUBUFInstruction::buffer_store_format_d16_xyzw) ||
             (op >= G9MUBUFInstruction::buffer_store_byte && op <= G9MUBUFInstruction::buffer_store_dwordx4) ||
             (op >= G9MUBUFInstruction::buffer_store_format_d16_hi_x && op <= G9MUBUFInstruction::buffer_wbinvl1_vol))

    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= G9MUBUFInstruction::buffer_atomic_swap && op <= G9MUBUFInstruction::buffer_atomic_dec_x2)
    {
        instKind = Instruction::VectorMemory;
    }

    return (op < G9MUBUFInstruction::buffer_Illegal ? op : G9MUBUFInstruction::buffer_Illegal);
}


MUBUFInstruction::VADDR
ParserSIMUBUF::GetVADDR(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MUBUF, vaddr, VADDR, 32);
    RETURN_EXTRACT_INSTRUCTION(vaddr);
}

MUBUFInstruction::VDATA
ParserSIMUBUF::GetVDATA(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MUBUF, vdata, VDATA, 40);
    RETURN_EXTRACT_INSTRUCTION(vdata);
}

MUBUFInstruction::SRSRC
ParserSIMUBUF::GetSRSRC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MUBUF, srsrc, SRSRC, 48);
    RETURN_EXTRACT_INSTRUCTION(srsrc);
}

MUBUFInstruction::SLC
ParserSIMUBUF::GetSLC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MUBUF, slc, SLC, 54);
    RETURN_EXTRACT_INSTRUCTION(slc);
}

MUBUFInstruction::TFE
ParserSIMUBUF::GetTFE(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MUBUF, tfe, TFE, 55);
    RETURN_EXTRACT_INSTRUCTION(tfe);
}


MUBUFInstruction::SOFFSET
ParserSIMUBUF::GetSOFFSET(Instruction::instruction64bit hexInstruction, unsigned int&)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MUBUF, soffset, SOFFSET, 56);
    RETURN_EXTRACT_INSTRUCTION(soffset);

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= MUBUFInstruction::SOFFSET##FIELD_MIN) && (IN <= MUBUFInstruction::SOFFSET##FIELD_MAX)) \
    { \
        VAL = IN; \
        return MUBUFInstruction::SOFFSET##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == MUBUFInstruction::SOFFSET##FIELD) \
    { \
        return MUBUFInstruction::SOFFSET##FIELD; \
    }
    //GENERIC_INSTRUCTION_FIELDS_1(soffset, ridx);
#undef X
#undef X_RANGE

#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN)\
    if ((IN >= MUBUFInstruction::SOFFSET##FIELD_MIN) && (IN <= MUBUFInstruction::SOFFSET##FIELD_MAX)) \
    { \
        return MUBUFInstruction::SOFFSET##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == MUBUFInstruction::SOFFSET##FIELD) \
    { \
        return MUBUFInstruction::SOFFSET##FIELD; \
    }
    //SCALAR_INSTRUCTION_FIELDS(soffset);
#undef X
#undef X_RANGE
    //return MUBUFInstruction::SOFFSETIllegal;
}

Instruction::InstructionClass ParserSIMUBUF::GetSIMUBUFClass(SIMUBUFInstruction::OP op)
{
	return g_instructionFormatMapMUBUF[op].insClass;
}

ParserSI::kaStatus
ParserSIMUBUF::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction)
{
    ParserSI::kaStatus status = ParserSI::Status_SUCCESS;
    Instruction::InstructionCategory instKind = Instruction::VectorMemory;
    unsigned int ridx = 0;
    MUBUFInstruction::OFFSET offset = GetOFFSET(hexInstruction);
    MUBUFInstruction::OFFEN offen = GetOFFEN(hexInstruction);
    MUBUFInstruction::IDXEN idxen = GetIDXEN(hexInstruction);
    MUBUFInstruction::GLC glc = GetGLC(hexInstruction);
    MUBUFInstruction::ADDR64 addr64 = GetADDR64(hexInstruction);
    MUBUFInstruction::LDS lds = GetLDS(hexInstruction);
    MUBUFInstruction::VADDR vaddr = GetVADDR(hexInstruction);
    MUBUFInstruction::VDATA vdata = GetVDATA(hexInstruction);
    MUBUFInstruction::SRSRC srsrc = GetSRSRC(hexInstruction);
    MUBUFInstruction::SLC slc = GetSLC(hexInstruction);
    MUBUFInstruction::TFE tfe = GetTFE(hexInstruction);
    MUBUFInstruction::SOFFSET soffset = GetSOFFSET(hexInstruction, ridx);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SIMUBUFInstruction::OP op = GetSIOpMUBUF(hexInstruction, instKind);
		Instruction::InstructionClass insClass = GetSIMUBUFClass(op);
        instruction = std::make_unique<SIMUBUFInstruction>(offset, offen, idxen, glc, addr64, lds, op, vaddr, vdata, srsrc, slc,
                                             tfe, soffset, ridx, instKind, insClass);
    }
    else if (hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        VIMUBUFInstruction::OP op = GetVIOpMUBUF(hexInstruction, instKind);
        instruction = std::make_unique<VIMUBUFInstruction>(offset, offen, idxen, glc, addr64, lds, op, vaddr, vdata, srsrc, slc,
                                             tfe, soffset, ridx, instKind);
    }
    else if (hwGen == GDT_HW_GENERATION_GFX9)
    {
        G9MUBUFInstruction::OP op = GetG9OpMUBUF(hexInstruction, instKind);
        instruction = std::make_unique<G9MUBUFInstruction>(offset, offen, idxen, glc, addr64, lds, op, vaddr, vdata, srsrc, slc,
                                             tfe, soffset, ridx, instKind);
    }
    else
    {
        status = ParserSI::Status_UnexpectedHWGeneration;
    }

    return status;
}

ParserSI::kaStatus ParserSIMUBUF::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
	hasLiteral = false;
    return ParserSI::Status_32BitInstructionNotSupported;
}
