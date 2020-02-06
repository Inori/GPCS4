//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSISOP2_H
#define __PARSERSISOP2_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSISOP2
/// \brief Description:  Parser for the Southern Island [SI] SOP2 instructions
/// -----------------------------------------------------------------------------------------------

class ParserSISOP2 : public ParserSI
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
    /// Note: SOP2 instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSISOP2() { }

    ///dtor
    ~ParserSISOP2() { }

private:
    /// SOP2 Instruction`s fields masks
    enum SOPCMask
    {
        ///    SSRC0 [7:0]
        SOP2Mask_SSRC0 = 0x000000FF,
        ///    SSRC1 [15:8]
        SOP2Mask_SSRC1 = 0x000000FF << 8,
        ///    SDST [22:16]
        SOP2Mask_SDST = 0x0000007F << 16,
        ///    OP     [22:16]
        SOP2Mask_OP   =   0x0000007F << 23
    };

    //
    // Private member functions
    //

    /// Get SOP2 instruction`s SSRC field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SSRC is ScalarGPR or SSRC ScalarTtmp.
    /// \param[in]  idxSSRC        The SSRC index [indicates SSRC0 or SSRC1].
    /// \returns                   A SSRC.
    static SOP2Instruction::SSRC GetSSRC(Instruction::instruction32bit hexInstruction, unsigned int& ridx, unsigned int idxSSRC);

    /// Get SOP2 instruction`s SDST field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SDST is ScalarGPR or SSRC ScalarTtmp.
    /// \returns                   A SDST.
    static SOP2Instruction::SDST GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx);

    /// Get SOP2 instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SISOP2Instruction::OP GetSISOP2Op(Instruction::instruction32bit hexInstruction);

    /// Get SOP2 instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VISOP2Instruction::OP GetVISOP2Op(Instruction::instruction32bit hexInstruction);

    /// Get SOP2 instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static G9SOP2Instruction::OP GetG9SOP2Op(Instruction::instruction32bit hexInstruction);

	static const GCNInstructionFormat& GetSISOP2Meta(SISOP2Instruction::OP op);

};

#endif //__PARSERSISOP2_H

