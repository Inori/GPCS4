//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSISOPP_H
#define __PARSERSISOPP_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSISOPP
/// \brief Description:  Parser for the Southern Island [SI] SOPP instructions
/// -----------------------------------------------------------------------------------------------

class ParserSISOPP : public ParserSI
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
    /// Note: SOPP instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSISOPP() {}

    ///dtor
    ~ParserSISOPP() { }

private:
    /// SOPP Instruction`s fields masks
    enum SOPPMask
    {
        ///    SIMM [15:0]
        SOPPMask_SIMM16 = 0x0000FFFF,
        ///    OP     [26:22]
        SOPPMask_OP   =   0x0000007F << 16
    };

    //
    // Private member functions
    //

    /// Get SOPP instruction`s SIMM16 field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   A SIMM16.
    static SOPPInstruction::SIMM16 GetSIMM16(Instruction::instruction32bit hexInstruction);

    /// Get SOPP instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SISOPPInstruction::OP GetSISOPPOp(Instruction::instruction32bit hexInstruction);

    /// Get SOPP instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VISOPPInstruction::OP GetVISOPPOp(Instruction::instruction32bit hexInstruction);

	static Instruction::InstructionCategory GetSIOPCaterory(SISOPPInstruction::OP op);
	static Instruction::InstructionClass GetSIOPClass(SISOPPInstruction::OP op);
};

#endif //__PARSERSISOPP_H

