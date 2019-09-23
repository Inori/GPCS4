//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSIMIMG_H
#define __PARSERSIMIMG_H

#include "ParserSI.h"


/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSIMIMG
/// \brief Description:  Parser for the Southern Island [SI] MIMG instructions
/// -----------------------------------------------------------------------------------------------

class ParserSIMIMG : public ParserSI
{
public:
    //
    // Public member functions
    //

    /// Parse the instruction
    /// Note: MIMG instruction is 64 bit instruction - 32 bit instruction is not supported
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
    ParserSIMIMG() { }

    ///dtor
    ~ParserSIMIMG() { }

private:
    /// MUBUF Instruction`s fields masks
    enum MIMGMask
    {
        /// Low 32 bits
        ///    DMASK  [11:8]
        MIMGMask_DMASK = 0x0000000F << 8,
        ///    UNORM  [12:12]
        MIMGMask_UNORM = 0x00000001 << 12,
        ///    GLC    [13:13]
        MIMGMask_GLC = 0x00000001 << 13,
        ///    DA     [14:14]
        MIMGMask_DA = 0x00000001 << 14,
        ///    R128   [15:15]
        MIMGMask_R128 = 0x00000001 << 15,
        ///    TFE    [16:16]
        MIMGMask_TFE = 0x00000001 << 16,
        ///    LWE    [17:17]
        MIMGMask_LWE = 0x00000001 << 17,
        ///    OP     [24:18]
        MIMGMask_OP = 0x000000FF << 18,
        ///    SLC    [25:25]
        MIMGMask_SLC = 0x00000001 << 25,

        /// High 32 bits
        ///    VADDR  [39:32]
        MIMGMask_VADDR = 0x000000FF,
        ///    VDATA  [47:40]
        MIMGMask_VDATA = 0x000000FF << 8,
        ///    SRSRC  [52:48]
        MIMGMask_SRSRC = 0x0000001F << 16,
        ///    SSAMP  [57:53]
        MIMGMask_SSAMP = 0x0000001F << 21
    };

    //
    // Private member functions
    //

    /// Get the DMASK  [11:8]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An DMASK.
    static MIMGInstruction::DMASK GetDMASK(Instruction::instruction64bit hexInstruction);

    /// Get the UNORM  [12:12]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An UNORM.
    static MIMGInstruction::UNORM GetUNORM(Instruction::instruction64bit hexInstruction);

    /// Get the GLC    [14:14]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An GLC.
    static MIMGInstruction::GLC GetGLC(Instruction::instruction64bit hexInstruction);

    /// Get the  DA     [14:14]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An DA.
    static MIMGInstruction::DA GetDA(Instruction::instruction64bit hexInstruction);

    /// Get the R128   [15:15]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An R128.
    static MIMGInstruction::R128 GetR128(Instruction::instruction64bit hexInstruction);

    /// Get the TFE    [16:16]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An TFE.
    static MIMGInstruction::TFE GetTFE(Instruction::instruction64bit hexInstruction);

    /// Get the LWE    [17:17]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An LWE.
    static MIMGInstruction::LWE GetLWE(Instruction::instruction64bit hexInstruction);

    /// Get the     OP [24:18]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite or Atomics).
    /// \returns                 An OP.
    static SIMIMGInstruction::OP GetOpSIMIMG(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the     OP [24:18]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instKind  The instruction`s kind (could be VectorMemoryRead or VectorMemoryWrite or Atomics).
    /// \returns                 An OP.
    static VIMIMGInstruction::OP GetOpVIMIMG(Instruction::instruction64bit hexInstruction, Instruction::InstructionCategory& instKind);

    /// Get the SLC    [25:25]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SLC.
    static MIMGInstruction::SLC GetSLC(Instruction::instruction64bit hexInstruction);

    /// Get the VADDR  [39:32]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VADDR.
    static MIMGInstruction::VADDR GetVADDR(Instruction::instruction64bit hexInstruction);

    /// Get the VDATA  [47:40]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An VDATA.
    static MIMGInstruction::VDATA GetVDATA(Instruction::instruction64bit hexInstruction);

    /// Get the SRSRC  [52:48]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SRSRC.
    static MIMGInstruction::SRSRC GetSRSRC(Instruction::instruction64bit hexInstruction);

    /// Get the SSAMP  [57:53]
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An SSAMP.
    static MIMGInstruction::SSAMP GetSSAMP(Instruction::instruction64bit hexInstruction);

    static Instruction::InstructionClass GetSIMIMGClass(SIMIMGInstruction::OP op);
};

#endif //__PARSERSIMIMG_H

