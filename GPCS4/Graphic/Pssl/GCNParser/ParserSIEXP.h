//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSIEXP_H
#define __PARSERSIEXP_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSIEXP
/// \brief Description:  Parser for the Southern Island [SI] EXP instructions
/// -----------------------------------------------------------------------------------------------

class ParserSIEXP : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                    A status.
    ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// Parse the instruction
    /// Note: EXP instruction is 64 bit instruction - 32 bit instruction is not supported
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_32BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral);

    /// ctor
    ParserSIEXP() { }

    ///dtor
    ~ParserSIEXP() { }

private:
    /// EXP Instruction`s fields masks
    enum EXTMask
    {
        /// Low 32 bits
        ///        EN [3:0]
        EXPMask_EN = 0x0000000F,
        ///        TGT [9:4]
        EXPMask_TGT = 0x0000003F << 4,
        ///        COMPR [10:10]
        EXPMask_COMPR = 0x00000001 << 10,
        ///        DONE [11:11]
        EXPMask_DONE = 0x00000001 << 11,
        ///        VM [12:12]
        EXPMask_VM = 0x00000001 << 12,

        /// High 32 bits
        ///        VSRC0 [39:32]
        EXPMask_VSRC0 = 0x000000FF ,
        ///        VSRC1 [47:40]
        EXPMask_VSRC1 = 0x000000FF << 8,
        ///        VSRC2 [55:48]
        EXPMask_VSRC2 = 0x000000FF << 16,
        ///        VSRC3 [63:56]
        EXPMask_VSRC3 = 0x000000FF << 24
    };

    //
    // Private member functions
    //

    /// Get the EN [3:0]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An EN.
    static EXPInstruction::EN GetEn(Instruction::instruction64bit);

    /// Get the TGT [9:4]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An TGT.
    static EXPInstruction::TGT GetTGT(Instruction::instruction64bit);

    /// Get the COMPR [10:10]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An COMPR.
    static EXPInstruction::COMPR GetCOMPR(Instruction::instruction64bit);

    /// Get the DONE [11:11]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An DONE.
    static EXPInstruction::DONE GetDone(Instruction::instruction64bit);

    /// Get the VM [12:12]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VM.
    static EXPInstruction::VM GetVM(Instruction::instruction64bit);

    /// Get the VSRC
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \param[in]  the index of the VSRC data to export.
    /// \returns                   An VSRC.
    static EXPInstruction::VSRC GetVSRC(Instruction::instruction64bit, const unsigned int vsrcIdx);
};

#endif //__PARSERSIEXP_H
