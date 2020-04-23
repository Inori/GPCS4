//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSISOPK_H
#define __PARSERSISOPK_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSISOPK
/// \brief Description:  Parser for the Southern Island [SI] SOPK instructions
/// -----------------------------------------------------------------------------------------------

class ParserSISOPK : public ParserSI
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
    /// Note: SOPK instruction is 32 bit instruction - 64 bit instruction is not supported
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_64BitInstructionNotSupported.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSISOPK() { }

    ///dtor
    ~ParserSISOPK() { }

private:
    /// SOPK Instruction`s fields masks
    enum SOPKMask
    {
        ///    SIMM16 [15:0]
        SOPKMask_SIMM16 = 0x0000FFFF,
        ///    SDST [22:16]
        SOPKMask_SDST   =   0x0000007F << 16,
        ///    OP [27:23]
        SOPKMask_OP = 0x0000001F << 23,
    };

    //
    // Private member functions
    //

    /// Get SOPK instruction`s SIMM16 field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SIMM16 is ScalarGPR or SIMM16 ScalarTtmp.
    /// \returns                   A SSRC0.
    static SOPKInstruction::SIMM16 GetSIMM16(Instruction::instruction32bit hexInstruction, unsigned int& ridx);

    /// Get SOPK instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SISOPKInstruction::OP GetSISOPKOp(Instruction::instruction32bit hexInstruction);

    /// Get SOPK instruction`s OP field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VISOPKInstruction::OP GetVISOPKOp(Instruction::instruction32bit hexInstruction);

    /// Get SOPK instruction`s OP field (GFX9).
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An OP.
    static G9SOPKInstruction::OP GetG9SOPKOp(Instruction::instruction32bit hexInstruction);

    /// Get SOPK instruction`s SDST field.
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SDST is ScalarGPR or SSRC ScalarTtmp.
    /// \returns                   A SDST.
    static SOPKInstruction::SDST GetSDST(Instruction::instruction32bit hexInstruction, unsigned int& ridx);

	static const GCNInstructionFormat& GetSISOPKMeta(SISOPKInstruction::OP op);
};

#endif //__PARSERSISOPK_H

