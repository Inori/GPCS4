//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSIMTBUF_H
#define __PARSERSIMTBUF_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSIMTBUF
/// \brief Description:  Parser for the Southern Island [SI] MTBUF instructions
/// -----------------------------------------------------------------------------------------------

class ParserSIMTBUF : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// Note: MTBUF instruction is 64 bit instruction - 32 bit instruction is not supported
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
    ParserSIMTBUF() { }

    ///dtor
    ~ParserSIMTBUF() { }

private:
    /// MTBUF Instruction`s fields masks
    enum MTBUFMask
    {
        /// Low 32 bits
        ///    OFFSET [11:0]
        MTBUFMask_OFFSET = 0x00000FFF,
        ///    OFFEN  [12:12]
        MTBUFMask_OFFEN = 0x00000001 << 12,
        ///    IDXEN  [13:13]
        MTBUFMask_IDXEN = 0x00000001 << 13,
        ///    GLC    [14:14]
        MTBUFMask_GLC = 0x00000001 << 14,
        ///    ADDR64 [15:15]
        MTBUFMask_ADDR64 = 0x00000001 << 15,
        ///    OP     [18:16]
        MTBUFMask_OP = 0x00000007 << 16,
        ///    DFMT   [22:19]
        MTBUFMask_DFMT = 0x0000000F << 19,
        ///    NFMT   [25:23]
        MTBUFMask_NFMT = 0x00000007 << 23,

        /// High 32 bits
        ///    VADDR  [39:32]
        MTBUFMask_VADDR = 0x000000FF,
        ///    VDATA  [47:40]
        MTBUFMask_VDATA = 0x000000FF << 8,
        ///    SRSRC  [52:48]
        MTBUFMask_SRSRC = 0x0000001F << 16,
        ///    SLC    [54:54]
        MTBUFMask_SLC = 0x00000001 << 22,
        ///    TFE    [55:55]
        MTBUFMask_TFE = 0x00000001 << 23,
        ///    SOFFSET [63:56]
        MTBUFMask_SOFFSET = 0x000000FF << 24
    };

    //
    // Private member functions
    //

    /// Get the OFFSET [11:0]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OFFSET.
    static MTBUFInstruction::OFFSET GetOFFSET(Instruction::instruction64bit hexInstruction);

    /// Get the OFFEN  [12:12]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An OFFEN.
    static MTBUFInstruction::OFFEN GetOFFEN(Instruction::instruction64bit hexInstruction);

    /// Get the IDXEN  [13:13]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An IDXEN.
    static MTBUFInstruction::IDXEN GetIDXEN(Instruction::instruction64bit hexInstruction);

    /// Get the GLC    [14:14]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An GLC.
    static MTBUFInstruction::GLC GetGLC(Instruction::instruction64bit hexInstruction);

    /// Get the  ADDR64 [15:15]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An ADDR64.
    static MTBUFInstruction::ADDR64 GetADDR64(Instruction::instruction64bit hexInstruction);

    /// Get the OP     [18:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite).
    /// \returns                   An OP.
    static SIMTBUFInstruction::OP GetSIOpMTBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the OP     [18:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite).
    /// \returns                   An OP.
    static VIMTBUFInstruction::OP GetVIOpMTBUF(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the DFMT   [22:19]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An DFMT.
    static MTBUFInstruction::DFMT GetDFMT(Instruction::instruction64bit hexInstruction);

    /// Get the NFMT   [25:23]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An NFMT.
    static MTBUFInstruction::NFMT GetNFMT(Instruction::instruction64bit hexInstruction);

    /// Get the VADDR  [39:32]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VADDR.
    static MTBUFInstruction::VADDR GetVADDR(Instruction::instruction64bit hexInstruction);

    /// Get the VDATA  [47:40]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VDATA.
    static MTBUFInstruction::VDATA GetVDATA(Instruction::instruction64bit hexInstruction);

    /// Get the SRSRC  [52:48]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SRSRC.
    static MTBUFInstruction::SRSRC GetSRSRC(Instruction::instruction64bit hexInstruction);

    /// Get the SLC    [54:54]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SLC.
    static MTBUFInstruction::SLC GetSLC(Instruction::instruction64bit hexInstruction);

    /// Get the TFE    [55:55]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An TFE.
    static MTBUFInstruction::TFE GetTFE(Instruction::instruction64bit hexInstruction);

    /// Get the SOFFSET [63:56]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \param[in]  ridx           register value ,relevant only if SOFFSET is ScalarGPR or SOFFSET ScalarTtmp.
    /// \returns                   An SOFFSET.
    static MTBUFInstruction::SOFFSET GetSOFFSET(Instruction::instruction64bit hexInstruction, unsigned int& ridx);

	static Instruction::InstructionClass GetSIMTBUFClass(SIMTBUFInstruction::OP op);
};

#endif //__PARSERSIMTBUF_H

