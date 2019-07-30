//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSIMUBUF_H
#define __PARSERSIMUBUF_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSIMUBUF
/// \brief Description:  Parser for the Southern Island [SI] MUBUF instructions
/// -----------------------------------------------------------------------------------------------

class ParserSIMUBUF : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// Note: MUBUF instruction is 64 bit instruction - 32 bit instruction is not supported
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
    ParserSIMUBUF() { }

    ///dtor
    ~ParserSIMUBUF() { }

private:
    /// MUBUF Instruction`s fields masks
    enum MUBUFMask
    {
        /// Low 32 bits
        ///    OFFSET [11:0]
        MUBUFMask_OFFSET = 0x00000FFF,
        ///    OFFEN  [12:12]
        MUBUFMask_OFFEN = 0x00000001 << 12,
        ///    IDXEN  [13:13]
        MUBUFMask_IDXEN = 0x00000001 << 13,
        ///    GLC    [14:14]
        MUBUFMask_GLC = 0x00000001 << 14,
        ///    ADDR64 [15:15]
        MUBUFMask_ADDR64 = 0x00000001 << 15,
        ///    LDS [16:16]
        MUBUFMask_LDS = 0x00000001 << 16,
        ///    OP     [24:18]
        MUBUFMask_OP = 0x0000003F << 18,

        /// High 32 bits
        ///    VADDR  [39:32]
        MUBUFMask_VADDR = 0x000000FF,
        ///    VDATA  [47:40]
        MUBUFMask_VDATA = 0x000000FF << 8,
        ///    SRSRC  [52:48]
        MUBUFMask_SRSRC = 0x0000001F << 16,
        ///    SLC    [54:54]
        MUBUFMask_SLC = 0x00000001 << 22,
        ///    TFE    [55:55]
        MUBUFMask_TFE = 0x00000001 << 23,
        ///    SOFFSET [63:56]
        MUBUFMask_SOFFSET = 0x000000FF << 24
    };

    //
    // Private member functions
    //

    /// Get the OFFSET [11:0]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OFFSET.
    static MUBUFInstruction::OFFSET GetOFFSET(Instruction::instruction64bit hexInstruction);

    /// Get the OFFEN  [12:12]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OFFEN.
    static MUBUFInstruction::OFFEN GetOFFEN(Instruction::instruction64bit hexInstruction);

    /// Get the IDXEN  [13:13]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An IDXEN.
    static MUBUFInstruction::IDXEN GetIDXEN(Instruction::instruction64bit hexInstruction);

    /// Get the GLC    [14:14]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An GLC.
    static MUBUFInstruction::GLC GetGLC(Instruction::instruction64bit hexInstruction);

    /// Get the  ADDR64 [15:15]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An ADDR64.
    static MUBUFInstruction::ADDR64 GetADDR64(Instruction::instruction64bit hexInstruction);

    /// Get the LDS   [16:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An LDS.
    static MUBUFInstruction::LDS GetLDS(Instruction::instruction64bit hexInstruction);

    /// Get the OP     [18:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite or Atomics).
    /// \returns           s        An OP.
    static SIMUBUFInstruction::OP GetSIOpMUBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the OP     [18:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite or Atomics).
    /// \returns           s        An OP.
    static VIMUBUFInstruction::OP GetVIOpMUBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the OP     [18:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite or Atomics).
    /// \returns           s        An OP.
    static G9MUBUFInstruction::OP GetG9OpMUBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the VADDR  [39:32]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VADDR.
    static MUBUFInstruction::VADDR GetVADDR(Instruction::instruction64bit hexInstruction);

    /// Get the VDATA  [47:40]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VDATA.
    static MUBUFInstruction::VDATA GetVDATA(Instruction::instruction64bit hexInstruction);

    /// Get the SRSRC  [52:48]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SRSRC.
    static MUBUFInstruction::SRSRC GetSRSRC(Instruction::instruction64bit hexInstruction);

    /// Get the SLC    [54:54]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SLC.
    static MUBUFInstruction::SLC GetSLC(Instruction::instruction64bit hexInstruction);

    /// Get the TFE    [55:55]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An TFE.
    static MUBUFInstruction::TFE GetTFE(Instruction::instruction64bit hexInstruction);

    /// Get the SOFFSET [63:56]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SOFFSET is ScalarGPR or SOFFSET ScalarTtmp.
    /// \returns                   An SOFFSET.
    static MUBUFInstruction::SOFFSET GetSOFFSET(Instruction::instruction64bit hexInstruction, unsigned int& ridx);
};

#endif //__PARSERSIMUBUF_H

