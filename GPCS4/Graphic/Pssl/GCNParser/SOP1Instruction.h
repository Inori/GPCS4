//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __SOP1INSTRUCTION_H
#define __SOP1INSTRUCTION_H

#include "Instruction.h"

/// This is a scalar instruction with one input and one output.
// Can be followed by a 32-bit literal constant.
/// Opcode :
///        SSRC0 [7:0]
///        OP [15:8]
///        SDST [22:16]

class SOP1Instruction : public Instruction
{
public:

#define X(SYM) SSRC##SYM
#define X_INIT(SYM,VAL) SSRC##SYM = VAL
    /// Destination for SSRC
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

    // Registers index (m_ssrc0).
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int m_sridx0;

    /// Scalar destination for instruction..
    SDST m_sdst;

    /// Registers index (m_sdst).
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int m_sdstRidx;

    /// SOP1 Instruction Width in bits
    static const unsigned int SOP1InstructionWidth = 32;
public:
    //
    // Public member functions
    //

    /// ctor
    SOP1Instruction(SSRC ssrc0, SDST sdst, unsigned int sridx0, unsigned int sdstRidx, 
		InstructionCategory insCat = ScalarALU, 
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone) :
		Instruction(SOP1InstructionWidth, insCat, InstructionSet_SOP1, insClass, operandType),
        m_ssrc0(ssrc0), m_sridx0(sridx0), m_sdst(sdst), m_sdstRidx(sdstRidx) { }

    /// dtor
    ~SOP1Instruction() {}

    /// Get the SSRC0 [7:0]
    SSRC GetSSRC0() const { return m_ssrc0; }

    /// Get the SDST [22:16]
    SDST GetSDST() const { return m_sdst; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_ssrc == ScalarGPR or m_ssrc == ScalarTtmp
    unsigned int GetSRidx() const { return m_sridx0; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int GetSDSTRidx() const { return m_sdstRidx; }
};

class SISOP1Instruction : public SOP1Instruction
{
public:
    /// Selector for the SOP1 Instruction
    enum OP
    {
        ///  D.u = S0.u.
        S_MOV_B32 = 3,
        ///  D/u = S0.u.
        S_MOV_B64,
        ///  if(SCC) D.u = S0.u; else NOP.
        S_CMOV_B32,
        ///  if(SCC) D.u = S0.u; else NOP.
        S_CMOV_B64,
        ///  D.u = ~S0.u SCC = 1 if result non-zero.
        S_NOT_B32,
        ///  D.u = ~S0.u SCC = 1 if result non-zero.
        S_NOT_B64,
        ///  D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        S_WQM_B32,
        ///  D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
        S_WQM_B64 ,
        ///  D.u = S0.u[0:31] (reverse bits).
        S_BREV_B32,
        ///  D.u = S0.u[0:63] (reverse bits).
        S_BREV_B64,
        ///  D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT0_I32_B32,
        ///  D.i = CountZeroBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT0_I32_B64,
        ///  D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT1_I32_B32,
        ///  D.i = CountOneBits(S0.u). SCC = 1 if result is non-zero.
        S_BCNT1_I32_B64,
        ///  D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        S_FF0_I32_B32,
        ///  D.i = FindFirstZero(S0.u) from LSB; if no zeros, return -1.
        S_FF0_I32_B64,
        ///  D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        S_FF1_I32_B32,
        ///  D.i = FindFirstOne(S0.u) from LSB; if no ones, return -1.
        S_FF1_I32_B64,
        ///  D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        S_FLBIT_I32_B32,
        ///  D.i = FindFirstOne(S0.u) from MSB; if no ones, return -1.
        S_FLBIT_I32_B64,
        ///  D.i = Find first bit opposite of sign bit from MSB. If S0 == -1,
        /// return -1.
        S_FLBIT_I32,
        ///  D.i = Find first bit opposite of sign bit from MSB. If
        /// S0 == -1, return -1.
        S_FLBIT_I32_I64,
        ///  D.i = signext(S0.i[7:0]).
        S_SEXT_I32_I8,
        ///  D.i = signext(S0.i[15:0]).
        S_SEXT_I32_I16,
        ///  D.u[S0.u[4:0]] = 0.
        S_BITSET0_B32,
        ///  D.u[S0.u[5:0]] = 0.
        S_BITSET0_B64,
        ///  D.u[S0.u[4:0]] = 1.
        S_BITSET1_B32,
        ///  D.u[S0.u[5:0]] = 1.
        S_BITSET1_B64,
        ///  D.u = PC + 4; destination receives the byte address of the next
        /// instruction.
        S_GETPC_B64,
        ///  PC = S0.u; S0.u is a byte address of the instruction to jump to.
        S_SETPC_B64,
        ///  D.u = PC + 4; PC = S0.u.
        S_SWAPPC_B64,
        ///  Return from Exception; PC = TTMP1,0.
        S_RFE_B64,
        ///  D.u = EXEC, EXEC = S0.u & EXEC. SCC = 1 if the
        /// new value of EXEC is non-zero.
        S_AND_SAVEEXEC_B64 = 36,
        ///  D.u = EXEC, EXEC = S0.u | EXEC. SCC = 1 if the new
        /// value of EXEC is non-zero.
        S_OR_SAVEEXEC_B64,
        ///  D.u = EXEC, EXEC = S0.u ^ EXEC. SCC = 1 if the
        /// new value of EXEC is non-zero.
        S_XOR_SAVEEXEC_B64,
        ///  D.u = EXEC, EXEC = S0.u & ~EXEC. SCC = 1
        //if the new value of EXEC is non-zero.
        S_ANDN2_SAVEEXEC_B64,
        ///  D.u = EXEC, EXEC = S0.u | ~EXEC. SCC = 1 if
        /// the new value of EXEC is non-zero.
        S_ORN2_SAVEEXEC_B64,
        ///  D.u = EXEC, EXEC = ~(S0.u & EXEC). SCC = 1
        //if the new value of EXEC is non-zero.
        S_NAND_SAVEEXEC_B64,
        ///  D.u = EXEC, EXEC = ~(S0.u | EXEC). SCC = 1 if the
        /// new value of EXEC is non-zero.
        S_NOR_SAVEEXEC_B64,
        ///  D.u = EXEC, EXEC = ~(S0.u ^ EXEC). SCC = 1 if the
        /// new value of EXEC is non-zero.
        S_XNOR_SAVEEXEC_B64,
        ///  D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]),
        /// D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero.
        S_QUADMASK_B32,
        ///  D.u = QuadMask(S0.u). D[0] = OR(S0[3:0]),
        /// D[1] = OR(S0[7:4]) .... SCC = 1 if result is non-zero
        S_QUADMASK_B64,
        ///  SGPR[D.u] = SGPR[S0.u + M0.u].
        S_MOVRELS_B32,
        ///  SGPR[D.u] = SGPR[S0.u + M0.u].
        S_MOVRELS_B64,
        ///  SGPR[D.u + M0.u] = SGPR[S0.u].
        S_MOVRELD_B32,
        ///  SGPR[D.u + M0.u] = SGPR[S0.u].
        S_MOVRELD_B64,
        ///  Conditional branch join point. Arg0 = saved CSP value. No
        /// dest.
        S_CBRANCH_JOIN,
        ///  D.i = abs(S0.i). SCC=1 if result is non-zero.
        S_ABS_I32 = 52,
        ///  D.u = S0.u, introduce edc double error upon write to dest
        /// sgpr.
        /// All other values are reserved.
        S_MOV_FED_B32,
        /// Reserved
        S_RESERVED
    };

    /// Get the OP [15:8]
    OP GetOp() const { return m_op; }

    /// ctor
    SISOP1Instruction(SSRC ssrc0, OP op, SDST sdst, unsigned int sridx0, unsigned int sdstRidx,
		InstructionCategory insCat = ScalarALU, 
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone) :
		SOP1Instruction(ssrc0, sdst, sridx0, sdstRidx, insCat, insClass, operandType),
		m_op(op)
	{
	}

private:
    /// SOP1 operation.
    OP m_op;

};

class VISOP1Instruction : public SOP1Instruction
{
public:
    /// Selector for the SOP1 Instruction
    enum OP
    {
        s_mov_b32 = 0,
        s_mov_b64  = 1,
        s_cmov_b32  = 2,
        s_cmov_b64  = 3 ,
        s_not_b32  = 4,
        s_not_b64  = 5,
        s_wqm_b32  = 6,
        s_wqm_b64  = 7,
        s_brev_b32  = 8,
        s_brev_b64  = 9,
        s_bcnt0_i32_b32  = 10,
        s_bcnt0_i32_b64  = 11,
        s_bcnt1_i32_b32  = 12,
        s_bcnt1_i32_b64  = 13,
        s_ff0_i32_b32  = 14,
        s_ff0_i32_b64  = 15,
        s_ff1_i32_b32  = 16,
        s_ff1_i32_b64  = 17,
        s_flbit_i32_b32  = 18,
        s_flbit_i32_b64  = 19,
        s_flbit_i32  = 20,
        s_flbit_i32_i64  = 21,
        s_sext_i32_i8  = 22,
        s_sext_i32_i16  = 23,
        s_bitset0_b32  = 24,
        s_bitset0_b64  = 25,
        s_bitset1_b32  = 26,
        s_bitset1_b64  = 27,
        s_getpc_b64  = 28,
        s_setpc_b64  = 29,
        s_swappc_b64  = 30,
        s_rfe_b64  = 31,
        s_and_saveexec_b64 = 32,
        s_or_saveexec_b64 = 33,
        s_xor_saveexec_b64 = 34,
        s_andn2_saveexec_b64 = 35,
        s_orn2_saveexec_b64 = 36,
        s_nand_saveexec_b64 = 37,
        s_nor_saveexec_b64 = 38,
        s_xnor_saveexec_b64 = 39,
        s_quadmask_b32 = 40,
        s_quadmask_b64 = 41,
        s_movrels_b32 = 42,
        s_movrels_b64 = 43,
        s_movreld_b32 = 44,
        s_movreld_b64 = 45,
        s_cbranch_join = 46,
        s_abs_i32 = 48,
        s_mov_fed_b32 = 49,
        s_set_gpr_idx_idx = 50,
        s_Illegal = 51,
    };

    /// Get the OP [15:8]
    OP GetOp() const { return m_op; }

    /// ctor
    VISOP1Instruction(SSRC ssrc0, OP op, SDST sdst, unsigned int sridx0, unsigned int sdstRidx): SOP1Instruction(ssrc0, sdst, sridx0, sdstRidx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }
private:
    /// SOP1 operation.
    OP m_op;

};


// SOP1 instructions for VEGA (GFX9)
class G9SOP1Instruction : public SOP1Instruction
{
public:
    /// Selector for the SOP1 Instruction
    enum OP
    {
        s_mov_b32            = 0,
        s_mov_b64            = 1,
        s_cmov_b32           = 2,
        s_cmov_b64           = 3,
        s_not_b32            = 4,
        s_not_b64            = 5,
        s_wqm_b32            = 6,
        s_wqm_b64            = 7,
        s_brev_b32           = 8,
        s_brev_b64           = 9,
        s_bcnt0_i32_b32      = 10,
        s_bcnt0_i32_b64      = 11,
        s_bcnt1_i32_b32      = 12,
        s_bcnt1_i32_b64      = 13,
        s_ff0_i32_b32        = 14,
        s_ff0_i32_b64        = 15,
        s_ff1_i32_b32        = 16,
        s_ff1_i32_b64        = 17,
        s_flbit_i32_b32      = 18,
        s_flbit_i32_b64      = 19,
        s_flbit_i32          = 20,
        s_flbit_i32_i64      = 21,
        s_sext_i32_i8        = 22,
        s_sext_i32_i16       = 23,
        s_bitset0_b32        = 24,
        s_bitset0_b64        = 25,
        s_bitset1_b32        = 26,
        s_bitset1_b64        = 27,
        s_getpc_b64          = 28,
        s_setpc_b64          = 29,
        s_swappc_b64         = 30,
        s_rfe_b64            = 31,
        s_and_saveexec_b64   = 32,
        s_or_saveexec_b64    = 33,
        s_xor_saveexec_b64   = 34,
        s_andn2_saveexec_b64 = 35,
        s_orn2_saveexec_b64  = 36,
        s_nand_saveexec_b64  = 37,
        s_nor_saveexec_b64   = 38,
        s_xnor_saveexec_b64  = 39,
        s_quadmask_b32       = 40,
        s_quadmask_b64       = 41,
        s_movrels_b32        = 42,
        s_movrels_b64        = 43,
        s_movreld_b32        = 44,
        s_movreld_b64        = 45,
        s_cbranch_join       = 46,
        s_abs_i32            = 48,
        s_mov_fed_b32        = 49,
        s_set_gpr_idx_idx    = 50,
        s_andn1_saveexec_b64 = 51,
        s_orn1_saveexec_b64  = 52,
        s_andn1_wrexec_b64   = 53,
        s_andn2_wrexec_b64   = 54,
        s_bitreplicate_b64_b32 = 55,
        s_Illegal            = 56,
    };

    /// Get the OP [15:8]
    OP GetOp() const { return m_op; }

    /// ctor
    G9SOP1Instruction(SSRC ssrc0, OP op, SDST sdst, unsigned int sridx0, unsigned int sdstRidx): SOP1Instruction(ssrc0, sdst, sridx0, sdstRidx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }
private:
    /// SOP1 operation.
    OP m_op;
};

#endif //__SOP1INSTRUCTION_H

