//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __PARSERSI_H
#define __PARSERSI_H

#include <string>
#include <memory>
#include "Instruction.h"
#include "SMRDInstruction.h"
#include "SOPPInstruction.h"
#include "SOPCInstruction.h"
#include "SOP1Instruction.h"
#include "SOPKInstruction.h"
#include "SOP2Instruction.h"
#include "VINTRPInstruction.h"
#include "DSInstruction.h"
#include "MUBUFInstruction.h"
#include "MTBUFInstruction.h"
#include "MIMGInstruction.h"
#include "EXPInstruction.h"
#include "VOPInstruction.h"
#include "GCNInstructionDefs.h"

/// -----------------------------------------------------------------------------------------------
/// \class Name: ParserSI
/// \brief Description:  [Abstract Factory] declares an interface a parser for the Southern Island [SI] instructions
/// -----------------------------------------------------------------------------------------------

class ParserSI
{
public:
    /// Return Codes
    /// Anything other than SUCCESS is a problem.
    enum kaStatus
    {
        Status_SUCCESS,
        Status_32BitInstructionNotSupported,
        Status_64BitInstructionNotSupported,
        Status_UnexpectedHWGeneration
    };

    /// SI instruction`s encoding mask
    enum InstructionEncodingMask
    {
        InstructionEncodingMask_9bit = 0x000001FF << 23,
        InstructionEncodingMask_7bit = 0x0000007F << 25,
        InstructionEncodingMask_6bit = 0x0000003F << 26,
        InstructionEncodingMask_5bit = 0x0000001F << 27,
        InstructionEncodingMask_4bit = 0x0000000F << 28,
        InstructionEncodingMask_2bit = 0x00000003 << 30,
        InstructionEncodingMask_1bit = 0x00000001 << 31
    };

    /// SI instruction`s encoding
    /// InstructionEncoding has uniform representation : [encoding] << [encoding start bit]
    enum InstructionEncoding : uint32_t
    {
        /// InstructionEncodingMask_9bit
        /// bits [31:23] - (1 0 1 1 1 1 1 0 1)
        InstructionEncoding_SOP1 = 0x0000017Du << 23,
        /// bits [31:23] - (1 0 1 1 1 1 1 1 1)
        InstructionEncoding_SOPP = 0x0000017Fu << 23,
        /// bits [31:23] - (1 0 1 1 1 1 1 1 0)
        InstructionEncoding_SOPC = 0x0000017Eu << 23,
        /// InstructionEncodingMask_7bit
        /// bits [31:25] - (0 1 1 1 1 1 1)
        InstructionEncoding_VOP1 = 0x0000003Fu << 25,
        /// bits [31:25] - (0 1 1 1 1 1 0)
        InstructionEncoding_VOPC = 0x0000003Eu << 25,
        /// InstructionEncodingMask_6bit
        /// bits [31:26] - (1 1 0 1 0 0)
        InstructionEncoding_VOP3 = 0x00000034u << 26,
        /// bits [31:26] - (1 1 1 1 1 0)
        InstructionEncoding_EXP = 0x0000003Eu << 26,
        /// bits [31:26] - (1 1 0 0 1 0)
        InstructionEncoding_VINTRP = 0x00000032u << 26,
        /// bits [31:26] - (1 1 0 1 1 0)
        InstructionEncoding_DS =  0x00000036u << 26,
        /// bits [31:26] - (1 1 1 0 0 0)
        InstructionEncoding_MUBUF =  0x00000038u << 26,
        /// bits [31:26] - (1 1 1 0 1 0)
        InstructionEncoding_MTBUF =  0x0000003Au << 26,
        /// bits [31:26] - (1 1 1 1 0 0)
        InstructionEncoding_MIMG =  0x0000003Cu << 26,
        /// InstructionEncodingMask_5bit
        /// bits [31:27] - (1 1 0 0 0)
        InstructionEncoding_SMRD = 0x00000018u << 27,
        /// InstructionEncodingMask_4bit
        /// bits [31:28] - (1 0 1 1)
        InstructionEncoding_SOPK = 0x0000000Bu << 28,
        /// InstructionEncodingMask_2bit
        /// bits [31:30] - (1 0)
        InstructionEncoding_SOP2 = 0x00000002u << 30,
        /// InstructionEncodingMask_1bit
        /// bits [31:31] - (0)
        InstructionEncoding_VOP2 = 0x00000000u << 31,

        /// bits [31:26] - (1 1 0 0 0 0)
        VIInstructionEncoding_SMEM = 0x00000030u << 26,
        /// bits [31:26] - (1 1 0 1 0 1)
        VIInstructionEncoding_VINTRP = 0x00000035u << 26,
        /// bits [31:26] - (1 1 0 1 1 1)
        VIInstructionEncoding_FLAT = 0x00000037u << 26,

        InstructionEncoding_ILLEGAL
    };

    //
    // Public member functions
    //

    /// ctor
    ParserSI() {};

    /// dtor
    virtual ~ParserSI() {};
	
    /// Parse the instruction
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                   A status.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral) = 0;

    /// Parse the instruction
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \instruction[out]  instruction  The parsed instruction.
    /// \returns                   A status.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction) = 0;

    /// Get instruction`s encoding
    /// \param[in]  hexInstruction  The 32 bit hexadecimal instruction.
    /// \returns                   An instruction`s encoding.
    static InstructionEncoding GetInstructionEncoding(Instruction::instruction32bit hexInstruction);

	/// Get instruction`s length in dwords
	/// \param[in]  encoding		An instruction`s encoding.
	/// \returns					An instruction`s length in dwords. Should be either 1 or 2,
	///								0 for illegal encoding.
	///								Note: the returned length doesn't include literal constant followed by the instruction.
	static uint32_t GetInstructionLengthDwords(InstructionEncoding encoding);

    /// Get instruction`s encoding
    /// \param[in]  hexInstruction  The 64 bit hexadecimal instruction.
    /// \returns                   An instruction`s encoding.
    //static InstructionEncoding GetInstructionEncoding(Instruction::instruction64bit hexInstruction) = 0;

    // Logging callback type.
    typedef void (*LoggingCallBackFuncP)(const std::string&);

    /// Set callback function for diagnostic output.
    /// If NULL, no output is generated.
    /// Used to report :
    /// 1)Unrecognized Instruction (encoding)
    /// 2)Unrecognized Instruction fields
    /// 3)Memory allocation failures
    /// 4)Report of start/end of parsing
    /// \param[in] callback a pointer to callback function.
    static void SetLog(LoggingCallBackFuncP callback);

private:
    /// Stream for diagnostic output.
    static LoggingCallBackFuncP m_LogCallback;
};

#endif //__PARSERSI_H

