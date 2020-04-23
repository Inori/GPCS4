//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSIDS_H
#define __PARSERSIDS_H

#include "ParserSI.h"

/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSIDS
/// \brief Description:  Parser for the Southern Island [SI] DS instructions
/// -----------------------------------------------------------------------------------------------

class ParserSIDS : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// Note: DS instruction is 64 bit instruction - 32 bit instruction is not supported
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \instruction[out]  instruction  Not relevant.
    /// \returns                   Status_32BitInstructionNotSupported.
    ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral);

    /// Parse the instruction
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                   A status.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction);

    /// ctor
    ParserSIDS() { }

    ///dtor
    ~ParserSIDS() { }

private:
    /// DS Instruction`s fields masks
    enum DSMask
    {
        /// Low 32 bits
        ///    OFFSET0 [7:0]
        DSMask_OFFSET0 = 0x000000FF,
        ///    OFFSET1 [15:8]
        DSMask_OFFSET1 = 0x000000FF << 8,
        ///    GDS [17:17]
        DSMask_GDS = 0x00000001 << 17,
        ///    OP [25:18]
        DSMask_OP = 0x000000FF << 18,

        /// High 32 bits
        ///    ADDR [39:32]
        DSMask_ADDR   =   0x000000FF,
        ///    DATA0 [47:40]
        DSMask_DATA0   =   0x000000FF << 8,
        ///    DATA1 [55:48]
        DSMask_DATA1   =   0x000000FF << 16,
        ///    VDST [63:56]
        DSMask_VDST   =   0x000000FF << 24,
    };

    //
    // Private member functions
    //

    /// Get the OFFSET0 [7:0] / OFFSET1
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \param[in]  offsetIdx the index of the OFFSET.
    /// \returns                   An OFFSET.
    static DSInstruction::OFFSET GetOffset(Instruction::instruction64bit, const unsigned char offsetIdx);

    /// Get the GDS [17:17]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An GDS.
    static DSInstruction::GDS GetGDS(Instruction::instruction64bit);

    /// Get the OP [25:18]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OP.
    static SIDSInstruction::OP GetSIDSOp(Instruction::instruction64bit);

    /// Get the OP [25:18]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OP.
    static VIDSInstruction::OP GetVIDSOp(Instruction::instruction64bit);

    /// Get the OP [25:18]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OP.
    static G9DSInstruction::OP GetG9DSOp(Instruction::instruction64bit);

    /// Get the ADDR [39:32]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An ADDR.
    static DSInstruction::ADDR GetADDR(Instruction::instruction64bit);

    /// Get the DATA
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \param[in]  dataIdx the index of the DATA.
    /// \returns                   An DATA.
    static DSInstruction::DATA GetDATA(Instruction::instruction64bit, const unsigned int dataIdx);

    /// Get the VDST [63:56]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VDST.
    static DSInstruction::VDST GetVDST(Instruction::instruction64bit);

	static const GCNInstructionFormat& GetSIDSMeta(SIDSInstruction::OP op);
};

#endif //__PARSERSIDS_H

