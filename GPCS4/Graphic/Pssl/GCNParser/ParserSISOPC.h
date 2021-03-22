//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSISOPC_H
#define __PARSERSISOPC_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSISOPC
/// \brief Description:  Parser for the Southern Island [SI] SOPC instructions
/// -----------------------------------------------------------------------------------------------

class ParserSISOPC : public ParserSI
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
    /// Note: SOPC instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSISOPC() { }

    ///dtor
    ~ParserSISOPC() { }

private:
    /// SOPC Instruction`s fields masks
    enum SOPCMask
    {
        ///    SSRC0 [7:0]
        SOPCMask_SSRC0 = 0x000000FF,
        ///    SSRC1 [15:8]
        SOPCMask_SSRC1 = 0x000000FF << 8,
        ///    OP     [22:16]
        SOPCMask_OP   =   0x0000007F << 16
    };

    //
    // Private member functions
    //

    /// Get SOPC instruction`s SSRC field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SSRC is ScalarGPR or SSRC ScalarTtmp.
    /// \param[in]  idxSSRC        The SSRC index [indicates SSRC0 or SSRC1].
    /// \returns                   A SSRC.
    static SOPCInstruction::SSRC GetSSRC(Instruction::instruction32bit hexInstruction, unsigned int& ridx, unsigned int idxSSRC);


    /// Get SOPC instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SISOPCInstruction::OP GetSISOPCOp(Instruction::instruction32bit hexInstruction);

    /// Get SOPC instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VISOPCInstruction::OP GetVISOPCOp(Instruction::instruction32bit hexInstruction);

	const GCNInstructionFormat& GetSISOPCMeta(SISOPCInstruction::OP op);
};

#endif //__PARSERSISOPC_H

