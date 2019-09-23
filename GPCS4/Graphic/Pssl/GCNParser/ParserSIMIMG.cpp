//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//
/// \file   ParserSIMIMG.cpp
/// \author GPU Developer Tools
/// \version $Revision: #6 $
/// \brief Description: Parser for MIMG (Image memory buffer operations) SI Instruction
//
//=============================================================
// $Id: //devtools/main/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzer/src/Emulator/Parser/ParserSIMIMG.cpp#6 $
// Last checkin:   $DateTime: 2014/07/10 11:38:24 $
// Last edited by: $Author: ekatz $
// Change list:    $Change: 499997 $
//=============================================================

/// Local:
#include "ParserSIMIMG.h"

MIMGInstruction::DMASK
ParserSIMIMG::GetDMASK(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, dmask, DMASK, 8);
    RETURN_EXTRACT_INSTRUCTION(dmask);
}

MIMGInstruction::UNORM
ParserSIMIMG::GetUNORM(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, unorm, UNORM, 12);
    RETURN_EXTRACT_INSTRUCTION(unorm);
}

MIMGInstruction::GLC
ParserSIMIMG::GetGLC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, glc, GLC, 13);
    RETURN_EXTRACT_INSTRUCTION(glc);
}

MIMGInstruction::DA
ParserSIMIMG::GetDA(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, da, DA, 14);
    RETURN_EXTRACT_INSTRUCTION(da);
}

MIMGInstruction::R128
ParserSIMIMG::GetR128(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, r128, R128, 15);
    RETURN_EXTRACT_INSTRUCTION(r128);
}

MIMGInstruction::TFE
ParserSIMIMG::GetTFE(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, tfe, TFE, 16);
    RETURN_EXTRACT_INSTRUCTION(tfe);
}

MIMGInstruction::LWE
ParserSIMIMG::GetLWE(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, lwe, LWE, 17);
    RETURN_EXTRACT_INSTRUCTION(lwe);
}

SIMIMGInstruction::OP
ParserSIMIMG::GetOpSIMIMG(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, op, OP, 18);

    if ((op >= SIMIMGInstruction::IMAGE_LOAD && op <= SIMIMGInstruction::IMAGE_LOAD_MIP_PCK_SGN) ||
        (op == SIMIMGInstruction::IMAGE_GET_RESINFO) ||
        (op >= SIMIMGInstruction::IMAGE_SAMPLE && op <= SIMIMGInstruction::IMAGE_SAMPLE_C_CD_CL_O))
    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= SIMIMGInstruction::IMAGE_STORE && op <= SIMIMGInstruction::IMAGE_STORE_MIP_PCK)
    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= SIMIMGInstruction::IMAGE_ATOMIC_SWAP && op <= SIMIMGInstruction::IMAGE_ATOMIC_FMAX)
    {
        instKind = Instruction::VectorMemory;
    }

    if ((op > SIMIMGInstruction::IMAGE_LOAD_MIP_PCK_SGN && op < SIMIMGInstruction::IMAGE_STORE) //TODO refactor this is always false!!!
        || (op > SIMIMGInstruction::IMAGE_STORE_MIP_PCK && op < SIMIMGInstruction::IMAGE_GET_RESINFO)
        || (op > SIMIMGInstruction::IMAGE_GATHER4_C_CL && op < SIMIMGInstruction::IMAGE_GATHER4_C_L)
        || (op > SIMIMGInstruction::IMAGE_GATHER4_CL_O && op < SIMIMGInstruction::IMAGE_GATHER4_L_O)
        || (op > SIMIMGInstruction::IMAGE_GATHER4_C_CL_O && op < SIMIMGInstruction::IMAGE_GATHER4_C_L_O)
        || (op > SIMIMGInstruction::IMAGE_GET_LOD && op < SIMIMGInstruction::IMAGE_SAMPLE_CD)
        || (op >= SIMIMGInstruction::IMAGE_RESERVED))
    {
        return SIMIMGInstruction::IMAGE_RESERVED;
    }
    else
    {
        return op;
    }
}

VIMIMGInstruction::OP
ParserSIMIMG::GetOpVIMIMG(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, VI, MIMG, op, OP, 18);

    if ((op >= VIMIMGInstruction::image_load && op <= VIMIMGInstruction::image_load_mip_pck_sgn) ||
        (op == VIMIMGInstruction::image_get_resinfo) ||
        (op >= VIMIMGInstruction::image_sample && op <= VIMIMGInstruction::image_sample_c_cd_cl_o))
    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= VIMIMGInstruction::image_store && op <= VIMIMGInstruction::image_store_mip_pck)
    {
        instKind = Instruction::VectorMemory;
    }
    else if (op >= VIMIMGInstruction::image_atomic_swap && op <= VIMIMGInstruction::image_atomic_dec)
    {
        instKind = Instruction::VectorMemory;
    }

    if (op > VIMIMGInstruction::image_sample_c_cd_cl_o)
    {
        return VIMIMGInstruction::image_Ilegal;
    }
    else
    {
        return op;
    }
}

MIMGInstruction::SLC
ParserSIMIMG::GetSLC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION32_FIELD(hexInstruction, SI, MIMG, slc, SLC, 25);
    RETURN_EXTRACT_INSTRUCTION(slc);
}

MIMGInstruction::VADDR
ParserSIMIMG::GetVADDR(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MIMG, vaddr, VADDR, 32);
    RETURN_EXTRACT_INSTRUCTION(vaddr);
}

MIMGInstruction::VDATA
ParserSIMIMG::GetVDATA(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MIMG, vdata, VDATA, 40);
    RETURN_EXTRACT_INSTRUCTION(vdata);
}

MIMGInstruction::SRSRC
ParserSIMIMG::GetSRSRC(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MIMG, srsrc, SRSRC, 48);
    RETURN_EXTRACT_INSTRUCTION(srsrc);
}

MIMGInstruction::SSAMP
ParserSIMIMG::GetSSAMP(Instruction::instruction64bit hexInstruction)
{
    EXTRACT_INSTRUCTION64_FIELD(hexInstruction, MIMG, ssamp, SSAMP, 53);
    RETURN_EXTRACT_INSTRUCTION(ssamp);
}

Instruction::InstructionClass ParserSIMIMG::GetSIMIMGClass(SIMIMGInstruction::OP op)
{
    return g_instructionFormatMapMIMG[op].insClass;
}

ParserSI::kaStatus
ParserSIMIMG::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction)
{
    Instruction::InstructionCategory instKind = Instruction::ScalarMemory;
    MIMGInstruction::DMASK dmask = GetDMASK(hexInstruction);
    MIMGInstruction::UNORM unorm = GetUNORM(hexInstruction);
    MIMGInstruction::GLC glc = GetGLC(hexInstruction);
    MIMGInstruction::DA da = GetDA(hexInstruction);
    MIMGInstruction::R128 r128 = GetR128(hexInstruction);
    MIMGInstruction::TFE tfe = GetTFE(hexInstruction);
    MIMGInstruction::LWE lwe = GetLWE(hexInstruction);
    MIMGInstruction::SLC slc = GetSLC(hexInstruction);
    MIMGInstruction::VADDR vaddr = GetVADDR(hexInstruction);
    MIMGInstruction::VDATA vdata = GetVDATA(hexInstruction);
    MIMGInstruction::SRSRC srsrc = GetSRSRC(hexInstruction);
    MIMGInstruction::SSAMP ssamp = GetSSAMP(hexInstruction);

    if ((hwGen == GDT_HW_GENERATION_SEAISLAND) || (hwGen == GDT_HW_GENERATION_SOUTHERNISLAND))
    {
        SIMIMGInstruction::OP op = GetOpSIMIMG(hexInstruction, instKind);
        Instruction::InstructionClass insCls = GetSIMIMGClass(op);
        instruction = std::make_unique<SIMIMGInstruction>(dmask, unorm, glc, da, r128, tfe, lwe, op, slc, vaddr, vdata, srsrc,
                                            ssamp, instKind, insCls);
    }
    else
    {
        VIMIMGInstruction::OP op = GetOpVIMIMG(hexInstruction, instKind);
        instruction = std::make_unique<VIMIMGInstruction>(dmask, unorm, glc, da, r128, tfe, lwe, op, slc, vaddr, vdata, srsrc,
                                            ssamp, instKind);
    }


    return ParserSI::Status_SUCCESS;
}

ParserSI::kaStatus ParserSIMIMG::Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral)
{
	hasLiteral = false;
    return ParserSI::Status_32BitInstructionNotSupported;
}

