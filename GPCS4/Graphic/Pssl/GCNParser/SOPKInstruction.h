//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __SOPKINSTRUCTION_H
#define __SOPKINSTRUCTION_H

#include "Instruction.h"

/// This is a scalar instruction with one inline constant input and one output.
/// Opcode :
///        SIMM16 [15:0]
///        SDST [22:16]
///        OP [27:23]

class SOPKInstruction : public Instruction
{
public:

#define X(SYM) SIMM16##SYM
#define X_INIT(SYM,VAL) SIMM16##SYM = VAL
    /// Destination for SSRC
    enum SIMM16
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
    /// 16-bit integer input for opcode. Signedness is determined by opcode..
    SIMM16 m_simm16;

    // Registers index (m_simm16).
    /// Note : Relevant only if m_simm16 == ScalarGPR or m_simm16 == ScalarTtmp
    unsigned int m_simm16Ridx;

    /// Scalar destination for instruction..
    SDST m_sdst;

    /// Registers index (m_sdst).
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int m_sdstRidx;

    /// SOP1 Instruction Width in bits
    static const unsigned int SOPKInstructionWidth = 32;
public:
    //
    // Public member functions
    //

    /// ctor
    SOPKInstruction(SIMM16 simm16, SDST sdst, unsigned int simm16Ridx, unsigned int sdstRidx,
		InstructionCategory insCat = ScalarALU, 
		InstructionClass insClass = InstructionClassUnknown, 
		OperandType operandType = TypeNone) :
		Instruction(SOPKInstructionWidth, insCat, InstructionSet_SOPK, insClass, operandType),
        m_simm16(simm16), m_simm16Ridx(simm16Ridx), m_sdst(sdst), m_sdstRidx(sdstRidx) { }

    /// dtor
    ~SOPKInstruction() {}

    /// Get the SIMM16 [15:0]
    SIMM16 GetSIMM16() const { return m_simm16; }

    /// Get the SDST [22:16]
    SDST GetSDST() const { return m_sdst; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_simm16Ridx == ScalarGPR or m_simm16Ridx == ScalarTtmp
    unsigned int GetSIMM16Ridx() const { return m_simm16Ridx; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int GetSDSTRidx() const { return m_sdstRidx; }
};

class SISOPKInstruction : public SOPKInstruction
{
public:

    /// Selector for the SOPK Instruction
    enum OP
    {
        //D.i = signext(SIMM16).
        S_MOVK_I32,
        ///  if (SCC) D.i = signext(SIMM16); else NOP.
        S_CMOVK_I32 = 2,
        ///  SCC = (D.i == signext(SIMM16).
        S_CMPK_EQ_I32,
        ///  SCC = (D.i != signext(SIMM16).
        S_CMPK_LG_I32,
        ///  SCC = (D.i != signext(SIMM16)).
        S_CMPK_GT_I32,
        ///  SCC = (D.i >= signext(SIMM16)).
        S_CMPK_GE_I32,
        ///  SCC = (D.i < signext(SIMM16)).
        S_CMPK_LT_I32,
        ///  SCC = (D.i <= signext(SIMM16)).
        S_CMPK_LE_I32,
        ///  SCC = (D.u == SIMM16).
        S_CMPK_EQ_U32,
        ///  SCC = (D.u != SIMM16).
        S_CMPK_LG_U32,
        ///  SCC = (D.u > SIMM16).
        S_CMPK_GT_U32,
        ///  SCC = (D.u >= SIMM16).
        S_CMPK_GE_U32,
        ///  SCC = (D.u < SIMM16).
        S_CMPK_LT_U32,
        ///  D.u = SCC = (D.u <= SIMM16).
        S_CMPK_LE_U32,
        ///  D.i = D.i + signext(SIMM16). SCC = overflow.
        S_ADDK_I32,
        ///  D.i = D.i * signext(SIMM16). SCC = overflow.
        S_MULK_I32,
        ///  Conditional branch using branch-stack.
        /// Arg0(sdst) = compare mask (VCC or any SGPR), SIMM16 = signed DWORD
        /// branch offset relative to next instruction.
        S_CBRANCH_I_FORK,
        ///  D.u = hardware register. Read some or all of a hardware register
        /// into the LSBs of D. SIMM16 = {size[4:0], offset[4:0], hwRegId[5:0]}; offset
        /// is 0�31, size is 1�32.
        S_GETREG_B32,
        ///  hardware register = D.u. Write some or all of the LSBs of D
        /// into a hardware register (note that D is a source SGPR).
        /// SIMM16 = {size[4:0], offset[4:0], hwRegId[5:0]}; offset is 0�31, size is 1�32.
        S_SETREG_B32,
        ///  This instruction uses a 32-bit literal constant. Write
        /// some or all of the LSBs of IMM32 into a hardware register.
        /// SIMM16 = {size[4:0], offset[4:0], hwRegId[5:0]}; offset is 0�31, size is 1�32.
        /// All other values are reserved.
        S_SETREG_IMM32_B32 = 21,
        /// Reserved
        S_RESERVED
    };

    /// Get the OP [27:23]
    OP GetOp() const { return m_op; }

    /// ctor
    SISOPKInstruction(
		SIMM16 simm16, OP op, SDST sdst, unsigned int simm16Ridx, unsigned int sdstRidx,
		InstructionClass insClass    = InstructionClassUnknown,
		OperandType operandType = TypeNone) : 
		SOPKInstruction(simm16, sdst, simm16Ridx, sdstRidx, 
			Instruction::ScalarALU, 
			insClass, 
			operandType),
		m_op(op) { }

private:
    /// SOPK operation.
    OP m_op;


};

class VISOPKInstruction : public SOPKInstruction
{
public:

    /// Selector for the SOPK Instruction
    enum OP
    {
        s_movk_i32         = 0,
        s_cmovk_i32        = 1,
        s_cmpk_eq_i32      = 2,
        s_cmpk_lg_i32      = 3,
        s_cmpk_gt_i32      = 4,
        s_cmpk_ge_i32      = 5,
        s_cmpk_lt_i32      = 6,
        s_cmpk_le_i32      = 7,
        s_cmpk_eq_u32      = 8,
        s_cmpk_lg_u32      = 9,
        s_cmpk_gt_u32      = 10,
        s_cmpk_ge_u32      = 11,
        s_cmpk_lt_u32      = 12,
        s_cmpk_le_u32      = 13,
        s_addk_i32         = 14,
        s_mulk_i32         = 15,
        s_cbranch_i_fork   = 16,
        s_getreg_b32       = 17,
        s_setreg_b32       = 18,
        s_setreg_imm32_b32 = 20,
        /// Illegal
        S_ILLEGAL          = 21,
    };

    /// Get the OP [27:23]
    OP GetOp() const { return m_op; }

    /// ctor
    VISOPKInstruction(SIMM16 simm16, OP op, SDST sdst, unsigned int simm16Ridx, unsigned int sdstRidx): SOPKInstruction(simm16, sdst, simm16Ridx, sdstRidx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }

private:
    /// SOPK operation.
    OP m_op;


};

// VEGA (GFX9)
class G9SOPKInstruction : public SOPKInstruction
{
public:
    enum OP
    {
        s_movk_i32         = 0,
        s_cmovk_i32        = 1,
        s_cmpk_eq_i32      = 2,
        s_cmpk_lg_i32      = 3,
        s_cmpk_gt_i32      = 4,
        s_cmpk_ge_i32      = 5,
        s_cmpk_lt_i32      = 6,
        s_cmpk_le_i32      = 7,
        s_cmpk_eq_u32      = 8,
        s_cmpk_lg_u32      = 9,
        s_cmpk_gt_u32      = 10,
        s_cmpk_ge_u32      = 11,
        s_cmpk_lt_u32      = 12,
        s_cmpk_le_u32      = 13,
        s_addk_i32         = 14,
        s_mulk_i32         = 15,
        s_cbranch_i_fork   = 16,
        s_getreg_b32       = 17,
        s_setreg_b32       = 18,
        s_getreg_regrd_b32 = 19,
        s_setreg_imm32_b32 = 20,
        s_call_b64         = 21,
        /// Illegal
        S_ILLEGAL          = 22
    };

    /// Get the OP [27:23]
    OP GetOp() const { return m_op; }

    /// ctor
    G9SOPKInstruction(SIMM16 simm16, OP op, SDST sdst, unsigned int simm16Ridx, unsigned int sdstRidx)
        : SOPKInstruction(simm16, sdst, simm16Ridx, sdstRidx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }

private:
    /// SOPK operation.
    OP m_op;
};

#endif //__SOPKINSTRUCTION_H

