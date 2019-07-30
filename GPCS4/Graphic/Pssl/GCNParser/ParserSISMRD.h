//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSISMRD_H
#define __PARSERSISMRD_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSISMRD
/// \brief Description:  Parser for the Southern Island [SI] SMRD instructions
/// -----------------------------------------------------------------------------------------------

class ParserSISMRD : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                   A status.
    ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral);

    /// Parse the instruction
    /// Note: SMRD instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   in SI+CI: Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSISMRD() { }

    ///dtor
    ~ParserSISMRD() { }

private:
    /// SMRD Instruction`s fields masks
    enum SMRDMask
    {
        ///    OFFSET [7:0]
        SMRDMask_OFFSET = 0x000000FF,
        ///    IMM    [8:8]
        SMRDMask_IIM    = 0x00000001 << 8,
        ///    SBASE  [14:9]
        SMRDMask_SBASE =  0x0000003F << 9,
        ///    SDST   [21:15]
        SMRDMask_SDST =   0x0000007F << 15,
        ///    OP     [26:22]
        SMRDMask_OP   =   0x0000001F << 22,
        ///    OP     [25:18]
        SMEMMask_OP   =   0x000000FF << 18,
    };

    //
    // Private member functions
    //

    /// Get SMRD instruction`s OFFSET field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OFFSET.
    static SMRDInstruction::OFFSET GetOffset(Instruction::instruction32bit hexInstruction);

    /// Get SMRD instruction`s IMM field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An IMM.
    static SMRDInstruction::IMM GetImm(Instruction::instruction32bit hexInstruction);

    /// Get SMRD instruction`s SBASE field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   A SBASE.
    static SMRDInstruction::SBASE GetSBase(Instruction::instruction32bit hexInstruction);

    /// Get SMRD instruction`s SDST field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           A register`s index.Relevant only if SDST == ScalarGPR or SDST == ScalarTtmp
    /// \returns                   A SBASE.
    static SMRDInstruction::SDST GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx);

    /// Get SMRD instruction`s OP field
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SISMRDInstruction::OP GetSISMRDOp(Instruction::instruction32bit hexInstruction);

    /// Get SMRD instruction`s OP field
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VISMEMInstruction::OP GetVISMRDOp(Instruction::instruction64bit hexInstruction);
};

#endif //__PARSERSISMRD_H

