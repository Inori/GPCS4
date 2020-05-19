//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSISMRD.cpp
/// \author GPU Developer Tools
/// \version $Revision: #6 $
/// \brief Description: Parser for SMRD (Scalar instruction performing a memory read from L1 (constant) memory) SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSISMRD.cpp#6 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================



#include "ParserSISMRD.h"


unsigned char ParserSISMRD::GetOffset(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SMRD, offset, OFFSET, 0);
    RETURN_EXTRACT_INSTRUCTION(offset);
}


SMRDInstruction::IMM ParserSISMRD::GetImm(Instruction::instruction32bit hexInstruction)
{
    Instruction::instruction32bit imm = (hexInstruction & SMRDMask_IIM) >> 8;
    return static_cast<SMRDInstruction::IMM>(imm != 0);
}

SMRDInstruction::SBASE ParserSISMRD::GetSBase(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SMRD, sbase, SBASE, 8);
    RETURN_EXTRACT_INSTRUCTION(sbase);
}

SMRDInstruction::SDST ParserSISMRD::GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx)
{
    SMRDInstruction::SDST sdst = static_cast<SMRDInstruction::SDST>((hexInstruction & SMRDMask_SDST) >> 15);
#define X_RANGE(FIELD_MIN,FIELD_MAX,FIELD,IN,VAL)\
    if ((IN >= SMRDInstruction::SDST##FIELD_MIN) && (IN <= SMRDInstruction::SDST##FIELD_MAX)) \
    { \
        VAL = IN; \
        return SMRDInstruction::SDST##FIELD; \
    }
#define X(FIELD,IN) \
    if (IN == SMRDInstruction::SDST##FIELD) \
    { \
        return SMRDInstruction::SDST##FIELD; \
    }
    GENERIC_INSTRUCTION_FIELDS_1(sdst, ridx);
#undef X
#undef X_RANGE
    return SMRDInstruction::SDSTIllegal;
}

SISMRDInstruction::OP ParserSISMRD::GetSISMRDOp(Instruction::instruction32bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, SMRD, op, OP, 22);

    if ((op > SISMRDInstruction::S_LOAD_DWORDX16 && op < SISMRDInstruction::S_BUFFER_LOAD_DWORD) ||
        (op > SISMRDInstruction::S_BUFFER_LOAD_DWORDX16 && op < SISMRDInstruction::S_MEMTIME) ||
        (op >= SISMRDInstruction::S_RESERVED))
    {
        return SISMRDInstruction::S_RESERVED;
    }
    else
    {
        return op;
    }
}

VISMEMInstruction::OP ParserSISMRD::GetVISMRDOp(Instruction::instruction64bit hexInstruction)
{
    VISMEMInstruction::OP theOp;

    // we want bits[26:18] out of the 64bit.
    Instruction::instruction64bit the32 = static_cast<Instruction::instruction64bit>(hexInstruction << 38);
    Instruction::instruction64bit the321 = static_cast<Instruction::instruction64bit>(the32 >> 56);
    theOp    = static_cast<VISMEMInstruction::OP>(the321);

    if ((theOp > VISMEMInstruction::s_store_dwordx4 && theOp < VISMEMInstruction::s_buffer_store_dword) ||
        (theOp > VISMEMInstruction::s_buffer_store_dwordx4 && theOp < VISMEMInstruction::s_dcache_inv) ||
        (theOp >= VISMEMInstruction::S_ILLEGAL))
    {
        return VISMEMInstruction::S_ILLEGAL;
    }
    else
    {
        return theOp;
    }
}

Instruction::InstructionClass ParserSISMRD::GetSISMRDOPClass(SISMRDInstruction::OP op)
{
	return g_instructionFormatMapSMRD[op].insClass;
}

ParserSI::kaStatus ParserSISMRD::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
    if (hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        return ParserSI::Status_32BitInstructionNotSupported;
    }

    unsigned int ridx = 0;

	hasLiteral = false;

    // todo offset: [32:52]- it is not a char
    SMRDInstruction::OFFSET offset = GetOffset(hexInstruction);
    SMRDInstruction::IMM imm    = GetImm(hexInstruction);
    SMRDInstruction::SBASE sbase  = GetSBase(hexInstruction);
    SMRDInstruction::SDST sdst = GetSDST(hexInstruction, ridx);
    SISMRDInstruction::OP op = GetSISMRDOp(hexInstruction);
	Instruction::InstructionClass insCls = GetSISMRDOPClass(op);
    instruction = std::make_unique<SISMRDInstruction>(offset, imm, sbase, sdst, ridx, op, insCls);

    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSISMRD::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction)
{
    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        return ParserSI::Status_64BitInstructionNotSupported;
    }

    unsigned int ridx = 0;
    SMRDInstruction::OFFSET offset = GetOffset(Instruction::instruction32bit(hexInstruction & 0xffff));
    SMRDInstruction::IMM imm = GetImm(Instruction::instruction32bit(hexInstruction & 0xffff));
    SMRDInstruction::SBASE sbase = GetSBase(Instruction::instruction32bit(hexInstruction & 0xffff));
    SMRDInstruction::SDST sdst = GetSDST(Instruction::instruction32bit(hexInstruction & 0xffff), ridx);
    VISMEMInstruction::OP op = GetVISMRDOp(hexInstruction);
    instruction = std::make_unique<VISMEMInstruction>(offset, imm, sbase, sdst, ridx, op);

    return ParserSI::Status_SUCCESS;
}
