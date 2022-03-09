//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSIDS.cpp
/// \author GPU Developer Tools
/// \version $Revision: #7 $
/// \brief Description: Parser for DS (Local and global data share instructions)SI Instruction.
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSIDS.cpp#7 $
// Last checkin:   $DateTime: 2015/05/10 04:38:55 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 526865 $
//=============================================================

/// Local:
#include "ParserSIDS.h"

DSInstruction::OFFSET
ParserSIDS::GetOffset(Instruction::instruction64bit hexInstruction, const unsigned char offsetIdx)
{
    DSInstruction::OFFSET offset;

    /// offsetIdx = 1
    if (offsetIdx)
    {
        offset = static_cast<DSInstruction::OFFSET>((hexInstruction & DSMask_OFFSET1) >> 8);
    }
    /// offsetIdx = 0
    else
    {
        offset = static_cast<DSInstruction::OFFSET>(hexInstruction & DSMask_OFFSET0);
    }

    return offset;
}

DSInstruction::GDS
ParserSIDS::GetGDS(Instruction::instruction64bit hexInstruction)
{
    DSInstruction::GDS gds;
    gds  = static_cast<DSInstruction::GDS>(((hexInstruction & DSMask_GDS) >> 17) != 0);
    return gds;
}

SIDSInstruction::OP
ParserSIDS::GetSIDSOp(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, DS, op, OP, 18);

    if ((op >= SIDSInstruction::DS_RESERVED))
    {
        return SIDSInstruction::DS_RESERVED;
    }
    else
    {
        return op;
    }
}

VIDSInstruction::OP
ParserSIDS::GetVIDSOp(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, DS, op, OP, 18);

    if ((op >= VIDSInstruction::ds_Ilegal))
    {
        return VIDSInstruction::ds_Ilegal;
    }
    else
    {
        return op;
    }
}

G9DSInstruction::OP
ParserSIDS::GetG9DSOp(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, G9, DS, op, OP, 18);

    return (op < G9DSInstruction::ds_Ilegal ? op : G9DSInstruction::ds_Ilegal);
}

DSInstruction::ADDR
ParserSIDS::GetADDR(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, DS, addr, ADDR, 32);
    RETURN_EXTRACT_INSTRUCTION(addr);
}

DSInstruction::DATA
ParserSIDS::GetDATA(Instruction::instruction64bit hexInstruction, const unsigned int dataIdx)
{
    DSInstruction::DATA data;
    Instruction::instruction64bit DSMaskDATA;

    /// dataIdx = 1
    if (dataIdx)
    {
        DSMaskDATA = (static_cast<Instruction::instruction64bit>(DSMask_DATA1)) << 32;
        data = static_cast<DSInstruction::DATA>((hexInstruction & DSMaskDATA) >> 48);
    }
    /// dataIdx = 0
    else
    {
        DSMaskDATA = (static_cast<Instruction::instruction64bit>(DSMask_DATA0)) << 32;
        data = static_cast<DSInstruction::DATA>((hexInstruction & DSMaskDATA) >> 40);
    }

    return data;
}

DSInstruction::VDST
ParserSIDS::GetVDST(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, DS, vdst, VDST, 56);
    RETURN_EXTRACT_INSTRUCTION(vdst);
}

const GCNInstructionFormat& ParserSIDS::GetSIDSMeta(SIDSInstruction::OP op)
{
	return g_instructionFormatMapDS[op];
}

ParserSI::kaStatus
ParserSIDS::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction)
{
    ParserSI::kaStatus status = ParserSI::Status_SUCCESS;
    DSInstruction::OFFSET offset0 = GetOffset(hexInstruction, 0);
    DSInstruction::OFFSET offset1 = GetOffset(hexInstruction, 1);
    DSInstruction::GDS gds = GetGDS(hexInstruction);
    DSInstruction::ADDR addr = GetADDR(hexInstruction);
    DSInstruction::DATA data0 = GetDATA(hexInstruction, 0);
    DSInstruction::DATA data1 = GetDATA(hexInstruction, 1);
    DSInstruction::VDST vdst = GetVDST(hexInstruction);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SIDSInstruction::OP op = GetSIDSOp(hexInstruction);
		auto meta = GetSIDSMeta(op);
		instruction  = std::make_unique<SIDSInstruction>(offset0, offset1, gds, op, addr, data0, data1, vdst, meta.insClass, meta.operandType);
    }
    else if (hwGen == GDT_HW_GENERATION_VOLCANICISLAND)
    {
        VIDSInstruction::OP op = GetVIDSOp(hexInstruction);
        instruction = std::make_unique<VIDSInstruction>(offset0, offset1, gds, op, addr, data0, data1, vdst);
    }
    else if (hwGen == GDT_HW_GENERATION_GFX9)
    {
        G9DSInstruction::OP op = GetG9DSOp(hexInstruction);
        instruction = std::make_unique<G9DSInstruction>(offset0, offset1, gds, op, addr, data0, data1, vdst);
    }
    else
    {
        status = ParserSI::Status_UnexpectedHWGeneration;
    }

    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSIDS::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
	hasLiteral = false;
    return ParserSI::Status_32BitInstructionNotSupported;
}


