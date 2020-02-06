//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSISOP1_H
#define __PARSERSISOP1_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSISOP1
/// \brief Description:  Parser for the Southern Island [SI] SOP1 instructions
/// -----------------------------------------------------------------------------------------------

class ParserSISOP1 : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                   A status.
    ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral);

    /// Parse the instruction
    /// Note: SOP1 instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSISOP1() { }

    ///dtor
    ~ParserSISOP1() { }

private:
    /// SOP1 Instruction`s fields masks
    enum SOP1Mask
    {
        ///    SSRC0 [7:0]
        SOP1Mask_SSRC = 0x000000FF,
        ///    OP     [15:8]
        SOP1Mask_OP   =   0x000000FF << 8,
        ///    SDST [22:16]
        SOP1Mask_SDST = 0x0000007F << 16,
    };

    //
    // Private member functions
    //

    /// Get SOP1 instruction`s SSRC0 field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SSRC is ScalarGPR or SSRC ScalarTtmp.
    /// \returns                   A SSRC0.
    static SOP1Instruction::SSRC GetSSRC0(Instruction::instruction32bit hexInstruction, unsigned int& ridx);

    /// Get SOPC instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SISOP1Instruction::OP GetSISOP1Op(Instruction::instruction32bit hexInstruction);

    /// Get SOPC instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VISOP1Instruction::OP GetVISOP1Op(Instruction::instruction32bit hexInstruction);

    /// Get SOPC instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static G9SOP1Instruction::OP GetG9SOP1Op(Instruction::instruction32bit hexInstruction);

    /// Get SOP1 instruction`s SDST field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SSRC is ScalarGPR or SSRC ScalarTtmp.
    /// \returns                   A SDST.
    static SOP1Instruction::SDST GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx);

	static Instruction::InstructionCategory GetSIOPCategory(SISOP1Instruction::OP op);

	const GCNInstructionFormat& GetSISOP1Meta(SISOP1Instruction::OP op);
};

#endif //__PARSERSISOP1_H

