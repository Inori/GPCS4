//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __SOPCINSTRUCTION_H
#define __SOPCINSTRUCTION_H

#include "Instruction.h"

/// Scalar instruction taking two inputs and producing a comparison result.
/// Can be followed by a 32-bit literal constant.
/// Opcode :
///        SSRC0 [7:0]
///        SSRC1 [15:8]
///        OP [22:16]

class SOPCInstruction : public Instruction
{
public:

#define X(SYM) SSRC##SYM
#define X_INIT(SYM,VAL) SSRC##SYM = VAL
    /// Destination for scalar memory read instruction
    enum SSRC
    {
#include "GenericInstructionFields1.h"
#include "ScalarInstructionFields.h"
#include "GenericInstructionFields2.h"
        X(Illegal)
    };
#undef X_INIT
#undef X

private:
    /// Source 0. First operand for the instruction.
    SSRC m_ssrc0;

    /// Source 1. Second operand for instruction.
    SSRC m_ssrc1;

    /// Registers index (m_ssrc0).
    /// Note : Relevant only if m_ssrc0 == ScalarGPR or m_ssrc0 == ScalarTtmp
    unsigned int m_sridx0;

    /// Registers index (m_ssrc1).
    /// Note : Relevant only if m_ssrc1 == ScalarGPR or m_ssrc1 == ScalarTtmp
    unsigned int m_sridx1;

    /// SOPP Instruction Width in bits
    static const unsigned int SOPCInstructionWidth = 32;
public:
    //
    // Public member functions
    //

    /// ctor
    SOPCInstruction(SSRC ssrc0, SSRC ssrc1, unsigned int sridx0, unsigned int sridx1, 
		InstructionCategory insCat = ScalarALU, 
		InstructionClass insClass = InstructionClassUnknown, 
		OperandType operandType = TypeNone) :
		Instruction(SOPCInstructionWidth, insCat, InstructionSet_SOPC, insClass, operandType),
        m_ssrc0(ssrc0), m_ssrc1(ssrc1), m_sridx0(sridx0), m_sridx1(sridx1) {}

    /// dtor
    ~SOPCInstruction() {}

    /// Get the SSRC0 [7:0]
    SSRC GetSSRC0() const { return m_ssrc0; }

    /// Get the SSRC1 [15:8]
    SSRC GetSSRC1() const { return m_ssrc0; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_ssrc == ScalarGPR or m_ssrc == ScalarTtmp
    unsigned int GetSRidx0() const { return m_sridx0; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_ssrc == ScalarGPR or m_ssrc == ScalarTtmp
    unsigned int GetSRidx1() const { return m_sridx1; }
};

class SISOPCInstruction: public SOPCInstruction
{
public:
    /// Selector for the SOPC Instruction
    enum OP
    {
        ///  SCC = (S0.i == S1.i).
        S_CMP_EQ_I32,
        ///  SCC = (S0.i != S1.i).
        S_CMP_LG_I32,
        ///  SCC = (S0.i > S1.i).
        S_CMP_GT_I32,
        ///  SCC = (S0.i >= S1.i).
        S_CMP_GE_I32,
        ///  SCC = (S0.i < S1.i).
        S_CMP_LT_I32,
        ///  SCC = (S0.i <= S1.i).
        S_CMP_LE_I32,
        ///  SCC = (S0.u == S1.u).
        S_CMP_EQ_U32,
        ///  SCC = (S0.u != S1.u).
        S_CMP_LG_U32,
        ///  SCC = (S0.u > S1.u).
        S_CMP_GT_U32,
        ///  SCC = (S0.u >= S1.u).
        S_CMP_GE_U32,
        ///  SCC = (S0.u < S1.u).
        S_CMP_LT_U32,
        ///  SCC = (S0.u <= S1.u).
        S_CMP_LE_U32,
        ///  SCC = (S0.u[S1.u[4:0]] == 0).
        S_BITCMP0_B32,
        ///  SCC = (S0.u[S1.u[4:0]] == 1).
        S_BITCMP1_B32,
        ///  SCC = (S0.u[S1.u[5:0]] == 0).
        S_BITCMP0_B64,
        ///  SCC = (S0.u[S1.u[5:0]] == 1).
        S_BITCMP1_B64,
        ///  VSKIP = S0.u[S1.u[4:0]].
        S_SETVSKIP,
        /// ILLEGAL
        S_ILLEGALD
    };

    /// Get the OP [22:16]
    OP GetOp() const { return m_op; }

    /// ctor
    SISOPCInstruction(SSRC ssrc0, SSRC ssrc1, 
		OP op, unsigned int sridx0, unsigned int sridx1, 
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone) : 
		SOPCInstruction(
			ssrc0, ssrc1, sridx0, sridx1, 
			Instruction::ScalarALU, 
			insClass, 
			operandType), 
		m_op(op) {}


private:
    /// SOPC operation.
    OP m_op;


};

class VISOPCInstruction: public SOPCInstruction
{
public:
    /// Selector for the SOPC Instruction
    enum OP
    {
        s_cmp_eq_i32      = 0,
        s_cmp_lg_i32      = 1,
        s_cmp_gt_i32      = 2,
        s_cmp_ge_i32      = 3,
        s_cmp_lt_i32      = 4,
        s_cmp_le_i32      = 5,
        s_cmp_eq_u32      = 6,
        s_cmp_lg_u32      = 7,
        s_cmp_gt_u32      = 8,
        s_cmp_ge_u32      = 9,
        s_cmp_lt_u32      = 10,
        s_cmp_le_u32      = 11,
        s_bitcmp0_b32     = 12,
        s_bitcmp1_b32     = 13,
        s_bitcmp0_b64     = 14,
        s_bitcmp1_b64     = 15,
        s_setvskip        = 16,
        s_set_gpr_idx_on  = 17,
        s_cmp_eq_u64      = 18,
        s_cmp_lg_u64      = 19,
        S_ILLEGAL         = 20,
    };

    /// Get the OP [22:16]
    OP GetOp() const { return m_op; }

    /// ctor
    VISOPCInstruction(SSRC ssrc0, SSRC ssrc1, OP op, unsigned int sridx0, unsigned int sridx1): SOPCInstruction(ssrc0, ssrc1, sridx0, sridx1), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }


private:
    /// SOPC operation.
    OP m_op;


};


#endif //__SOPCINSTRUCTION_H

