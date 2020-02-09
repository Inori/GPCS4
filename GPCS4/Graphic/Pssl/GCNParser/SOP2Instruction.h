//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __SOP2INSTRUCTION_H
#define __SOP2INSTRUCTION_H

#include "Instruction.h"

/// This is a scalar instruction with two inputs and one output.
/// Can be followed by a 32-bit literalconstant.
/// Opcode :
///        SSRC0 [7:0]
///        SSRC1 [15:8]
///        SDST [22:16]
///        OP [29:23]

class SOP2Instruction : public Instruction
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

#define X(SYM) SDST##SYM
#define X_INIT(SYM,VAL) SDST##SYM = VAL
    /// Destination for SDST
    enum SDST
    {
#include "GenericInstructionFields1.h"
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

    /// Scalar destination for instruction..
    SDST m_sdst;

    /// Registers index (m_sdst).
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int m_sdstRidx;

    /// SOPP Instruction Width in bits
    static const unsigned int SOPCInstructionWidth = 32;
public:
    //
    // Public member functions
    //

    /// ctor
    SOP2Instruction(SSRC ssrc0, SSRC ssrc1, SDST sdst, unsigned int sridx0, unsigned int sridx1, unsigned int sdstRidx, 
		InstructionCategory insCat = ScalarALU, 
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone):
		Instruction(SOPCInstructionWidth, insCat, InstructionSet_SOP2, insClass, operandType),
        m_ssrc0(ssrc0), m_ssrc1(ssrc1), m_sridx0(sridx0), m_sridx1(sridx1), m_sdst(sdst), m_sdstRidx(sdstRidx) {}

    /// dtor
    ~SOP2Instruction() {}

    /// Get the SSRC0 [7:0]
    SSRC GetSSRC0() const { return m_ssrc0; }

    /// Get the SSRC1 [15:8]
    SSRC GetSSRC1() const { return m_ssrc0; }

    /// Get the SDST [22:16]
    SDST GetSDST() const { return m_sdst; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_ssrc0 == ScalarGPR or m_ssrc0 == ScalarTtmp
    unsigned int GetSRidx0() const { return m_sridx0; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_ssrc1 == ScalarGPR or m_ssrc1 == ScalarTtmp
    unsigned int GetSRidx1() const { return m_sridx1; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int GetSDSTRidx() const { return m_sdstRidx; }
};

class SISOP2Instruction : public SOP2Instruction
{
public:
    /// Selector for the SOP2 Instruction
    enum OP
    {
        ///  D.u = S0.u + S1.u. SCC = carry out.
        S_ADD_U32,
        ///  D.u = S0.u - S1.u. SCC = carry out.
        S_SUB_U32,
        ///  D.u = S0.i + S1.i. SCC = overflow.
        S_ADD_I32,
        ///  D.u = S0.i - S1.i. SCC = overflow.
        S_SUB_I32,
        ///  D.u = S0.u + S1.u + SCC. SCC = carry-out.
        S_ADDC_U32,
        ///  D.u = S0.u - S1.u - SCC. SCC = carry-out.
        S_SUBB_U32,
        ///  D.i = (S0.i < S1.i) ? S0.i : S1.i. SCC = 1 if S0 is min.
        S_MIN_I32,
        ///  D.u = (S0.u < S1.u) ? S0.u : S1.u. SCC = 1 if S0 is min.
        S_MIN_U32,
        ///  D.i = (S0.i > S1.i) ? S0.i : S1.i. SCC = 1 if S0 is max.
        S_MAX_I32,
        ///  D.u = (S0.u > S1.u) ? S0.u : S1.u. SCC = 1 if S0 is max.
        S_MAX_U32,
        ///  D.u = SCC ? S0.u : S1.u.
        S_CSELECT_B32,
        ///  D.u = SCC ? S0.u : S1.u.
        S_CSELECT_B64 = 11,
        ///  D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        S_AND_B32 = 14,
        ///  D.u = S0.u & S1.u. SCC = 1 if result is non-zero.
        S_AND_B64,
        ///  D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        S_OR_B32,
        ///  D.u = S0.u | S1.u. SCC = 1 if result is non-zero.
        S_OR_B64,
        ///  D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        S_XOR_B32,
        ///  D.u = S0.u ^ S1.u. SCC = 1 if result is non-zero.
        S_XOR_B64,
        ///  D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        S_ANDN2_B32,
        ///  D.u = S0.u & ~S1.u. SCC = 1 if result is non-zero.
        S_ANDN2_B64,
        ///  D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        S_ORN2_B32,
        ///  D.u = S0.u | ~S1.u. SCC = 1 if result is non-zero.
        S_ORN2_B64,
        ///  D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        S_NAND_B32,
        ///  D.u = ~(S0.u & S1.u). SCC = 1 if result is non-zero.
        S_NAND_B64,
        ///  D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        S_NOR_B32,
        ///  D.u = ~(S0.u | S1.u). SCC = 1 if result is non-zero.
        S_NOR_B64,
        ///  D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        S_XNOR_B32,
        ///  D.u = ~(S0.u ^ S1.u). SCC = 1 if result is non-zero.
        S_XNOR_B64,
        ///  D.u = S0.u << S1.u[4:0]. SCC = 1 if result is non-zero.
        S_LSHL_B32,
        ///  D.u = S0.u << S1.u[5:0]. SCC = 1 if result is non-zero.
        S_LSHL_B64,
        ///  D.u = S0.u >> S1.u[4:0]. SCC = 1 if result is non-zero.
        S_LSHR_B32,
        ///  D.u = S0.u >> S1.u[5:0]. SCC = 1 if result is non-zero.
        S_LSHR_B64,
        ///  D.i = signtext(S0.i) >> S1.i[4:0]. SCC = 1 if result is non-zero.
        S_ASHR_I32,
        ///  D.i = signtext(S0.i) >> S1.i[5:0]. SCC = 1 if result is non-zero.
        S_ASHR_I64,
        ///  D.u = ((1 << S0.u[4:0]) - 1) << S1.u[4:0]; bitfield mask.
        S_BFM_B32,
        ///  D.u = ((1 << S0.u[5:0]) - 1) << S1.u[5:0]; bitfield mask.
        S_BFM_B64,
        ///  D.i = S0.i * S1.i.
        S_MUL_I32,
        ///  Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field
        /// width. D.u = (S0.u >> S1.u[4:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if result
        /// is non-zero.
        S_BFE_U32,
        ///  Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field
        /// width. D.i = (S0.u >> S1.u[4:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if result is
        /// non-zero. Test sign-extended result.
        S_BFE_I32,
        ///  Bit field extract. S0 is data, S1[4:0] is field offset, S1[22:16] is field
        /// width. D.u = (S0.u >> S1.u[5:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if result
        /// is non-zero.
        S_BFE_U64,
        ///  Bit field extract. S0 is data, S1[5:0] is field offset, S1[22:16] is field
        /// width. D.i = (S0.u >> S1.u[5:0]) & ((1 << S1.u[22:16]) - 1). SCC = 1 if result is
        /// non-zero. Test sign-extended result.
        S_BFE_I64,
        ///  Conditional branch using branch stack. Arg0 = compare
        /// mask (VCC or any SGPR), Arg1 = 64-bit byte address of target instruction.
        S_CBRANCH_G_FORK,
        ///  D.i = abs(S0.i >> S1.i). SCC = 1 if result is non-zero.
        /// All other values are reserved.
        S_ABSDIFF_I32,
        /// Reserved
        S_RESERVED
    };

    /// Get the OP [29:23]
    OP GetOp() const { return m_op; }

    /// ctor
    SISOP2Instruction(SSRC ssrc0, SSRC ssrc1, SDST sdst, 
		OP op, 
		unsigned int sridx0, unsigned int sridx1, unsigned int sdstRidx, 
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone) :
		SOP2Instruction(ssrc0, ssrc1, sdst, sridx0, sridx1, sdstRidx, ScalarALU, insClass, operandType),
		m_op(op) {}


private:
    /// SOPC operation.
    OP m_op;

};

class VISOP2Instruction : public SOP2Instruction
{
public:
    /// Selector for the SOP2 Instruction
    enum OP
    {
        s_add_u32         = 0 ,
        s_sub_u32         = 1 ,
        s_add_i32         = 2 ,
        s_sub_i32         = 3 ,
        s_addc_u32        = 4 ,
        s_subb_u32        = 5 ,
        s_min_i32         = 6 ,
        s_min_u32         = 7 ,
        s_max_i32         = 8 ,
        s_max_u32         = 9 ,
        s_cselect_b32     = 10,
        s_cselect_b64     = 11,
        s_and_b32         = 12,
        s_and_b64         = 13,
        s_or_b32          = 14,
        s_or_b64          = 15,
        s_xor_b32         = 16,
        s_xor_b64         = 17,
        s_andn2_b32       = 18,
        s_andn2_b64       = 19,
        s_orn2_b32        = 20,
        s_orn2_b64        = 21,
        s_nand_b32        = 22,
        s_nand_b64        = 23,
        s_nor_b32         = 24,
        s_nor_b64         = 25,
        s_xnor_b32        = 26,
        s_xnor_b64        = 27,
        s_lshl_b32        = 28,
        s_lshl_b64        = 29,
        s_lshr_b32        = 30,
        s_lshr_b64        = 31,
        s_ashr_i32        = 32,
        s_ashr_i64        = 33,
        s_bfm_b32         = 34,
        s_bfm_b64         = 35,
        s_mul_i32         = 36,
        s_bfe_u32         = 37,
        s_bfe_i32         = 38,
        s_bfe_u64         = 39,
        s_bfe_i64         = 40,
        s_cbranch_g_fork  = 41,
        s_absdiff_i32     = 42,
        s_rfe_restore_b64 = 43,
        /// Illegal
        S_ILLEGAL         = 44,
    };

    /// Get the OP [29:23]
    OP GetOp() const { return m_op; }

    /// ctor
    VISOP2Instruction(SSRC ssrc0, SSRC ssrc1, SDST sdst, OP op, unsigned int sridx0, unsigned int sridx1, unsigned int sdstRidx): SOP2Instruction(ssrc0, ssrc1, sdst, sridx0, sridx1, sdstRidx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }


private:
    /// SOPC operation.
    OP m_op;

};


// SOP2 instructions for VEGA (GFX9)
class G9SOP2Instruction : public SOP2Instruction
{
public:
    /// Selector for the SOP2 Instruction
    enum OP
    {
        s_add_u32         = 0 ,
        s_sub_u32         = 1 ,
        s_add_i32         = 2 ,
        s_sub_i32         = 3 ,
        s_addc_u32        = 4 ,
        s_subb_u32        = 5 ,
        s_min_i32         = 6 ,
        s_min_u32         = 7 ,
        s_max_i32         = 8 ,
        s_max_u32         = 9 ,
        s_cselect_b32     = 10,
        s_cselect_b64     = 11,
        s_and_b32         = 12,
        s_and_b64         = 13,
        s_or_b32          = 14,
        s_or_b64          = 15,
        s_xor_b32         = 16,
        s_xor_b64         = 17,
        s_andn2_b32       = 18,
        s_andn2_b64       = 19,
        s_orn2_b32        = 20,
        s_orn2_b64        = 21,
        s_nand_b32        = 22,
        s_nand_b64        = 23,
        s_nor_b32         = 24,
        s_nor_b64         = 25,
        s_xnor_b32        = 26,
        s_xnor_b64        = 27,
        s_lshl_b32        = 28,
        s_lshl_b64        = 29,
        s_lshr_b32        = 30,
        s_lshr_b64        = 31,
        s_ashr_i32        = 32,
        s_ashr_i64        = 33,
        s_bfm_b32         = 34,
        s_bfm_b64         = 35,
        s_mul_i32         = 36,
        s_bfe_u32         = 37,
        s_bfe_i32         = 38,
        s_bfe_u64         = 39,
        s_bfe_i64         = 40,
        s_cbranch_g_fork  = 41,
        s_absdiff_i32     = 42,
        s_rfe_restore_b64 = 43,
        s_mul_hi_u32      = 44,
        s_mul_hi_i32      = 45,
        s_lshl1_add_u32   = 46,
        s_lshl2_add_u32   = 47,
        s_lshl3_add_u32   = 48,
        s_lshl4_add_u32   = 49,
        s_pack_ll_b32_b16 = 50,
        s_pack_lh_b32_b16 = 51,
        s_pack_hh_b32_b16 = 52,
        /// Illegal
        S_ILLEGAL         = 53,
    };

    /// Get the OP [29:23]
    OP GetOp() const { return m_op; }

    /// ctor
    G9SOP2Instruction(SSRC ssrc0, SSRC ssrc1, SDST sdst, OP op, unsigned int sridx0, unsigned int sridx1, unsigned int sdstRidx): SOP2Instruction(ssrc0, ssrc1, sdst, sridx0, sridx1, sdstRidx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }

private:
    /// SOPC operation.
    OP m_op;
};

#endif //__SOP2INSTRUCTION_H

