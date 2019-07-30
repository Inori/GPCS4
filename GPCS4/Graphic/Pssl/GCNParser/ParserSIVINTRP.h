//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSIVINTRP_H
#define __PARSERSIVINTRP_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSIVINTRP
/// \brief Description:  Parser for the Southern Island [SI] VINTRP instructions
/// -----------------------------------------------------------------------------------------------

class ParserSIVINTRP : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                   A status.
    ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, Instruction*& instruction, bool& hasLiteral);

    /// Parse the instruction
    /// Note: VINTRP instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, Instruction*& instruction);

    /// ctor
    ParserSIVINTRP() { }

    ///dtor
    ~ParserSIVINTRP() { }

private:
    /// VINTRP Instruction`s fields masks
    enum VINTRPMask
    {
        ///    VSRC [7:0]
        VINTRPMask_VSRC = 0x00000FFF,
        ///    ATTRCHAN [9:8]
        VINTRPMask_ATTRCHAN = 0x00000003 << 8,
        ///    ATTR [15:10]
        VINTRPMask_ATTR = 0x0000003F << 10,
        ///    OP [27:23]
        VINTRPMask_OP = 0x00000003 << 16,
        ///    VDST [25:18]
        VINTRPMask_VDST = 0x000000FF << 18,
    };

    //
    // Private member functions
    //

    /// Get VINTRP instruction`s VSRC field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   A VSRC.
    static VINTRPInstruction::VSRC GetVSRC(Instruction::instruction32bit hexInstruction);

    /// Get VINTRP instruction`s ATTRCHAN field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   A ATTRCHAN.
    static VINTRPInstruction::ATTRCHAN GetATTRCHAN(Instruction::instruction32bit hexInstruction);

    /// Get VINTRP instruction`s SIMM16 field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   A ATTR.
    static VINTRPInstruction::ATTR GetATTR(Instruction::instruction32bit hexInstruction);

    /// Get VINTRP instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SIVINTRPInstruction::OP GetSIVINTRPOp(Instruction::instruction32bit hexInstruction);

    /// Get VINTRP instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VIVINTRPInstruction::OP GetVIVINTRPOp(Instruction::instruction32bit hexInstruction);

    /// Get VINTRP instruction`s VDST field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   A VDST.
    static VINTRPInstruction::VDST GetVDST(Instruction::instruction32bit hexInstruction);
};

#endif //__PARSERSIVINTRP_H

