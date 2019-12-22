//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __SOPPINSTRUCTION_H
#define __SOPPINSTRUCTION_H

#include "Instruction.h"
/// Scalar instruction taking one inline constant input and performing a special operation
/// (for example: jump).
/// Opcode :
///        SIMM16 [15:0]
///        OP [22:16]

class SOPPInstruction : public Instruction
{
public:

    typedef short SIMM16;

private:
    /// 16-bit integer input for opcode. Signedness is determined by opcode.
    SIMM16 m_simm16;

    /// SOPP Instruction Width in bits
    static const unsigned int SOPPInstructionWidth = 32;
public:
    //
    // Public member functions
    //

    /// ctor
    SOPPInstruction(SIMM16 simm16,
		InstructionCategory insCat = FlowControl, 
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone) :
		Instruction(SOPPInstructionWidth, insCat, InstructionSet_SOPP, insClass, operandType), 
		m_simm16(simm16) {}

    /// dtor
    ~SOPPInstruction() {}

    /// Get the SIMM16 [15:0]
    SIMM16 GetSIMM16() const { return m_simm16; }

};

class SISOPPInstruction : public SOPPInstruction
{
public:
    /// Selector for the SOPP Instruction
    enum OP
    {
        ///  do nothing. Repeat NOP 1..8 times based on SIMM16[2:0]. 0 = 1 time,= 8 times.,
        S_NOP,
        ///  end of program; terminate wavefront.
        S_ENDPGM,
        ///  PC = PC + signext(SIMM16 * 4) + 4.
        S_BRANCH,
        ///  if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else
        /// nop.
        S_CBRANCH_SCC0 = 4,
        ///  if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else
        /// nop.
        S_CBRANCH_SCC1,
        ///  if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else
        /// nop.
        S_CBRANCH_VCCZ,
        ///  if(VCC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else
        /// nop.
        S_CBRANCH_VCCNZ,
        ///  if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4;
        /// else nop.
        S_CBRANCH_EXECZ,
        ///  if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4;
        /// else nop.
        S_CBRANCH_EXECNZ,
        ///  Sync waves within a thread group.
        S_BARRIER,
        ///  Wait for count of outstanding lds, vector-memory and
        /// export/vmem-write-data to be at or below the specified levels. simm16[3:0] =
        /// vmcount, simm16[6:4] = export/mem-write-data count, simm16[12:8] =
        /// LGKM_cnt (scalar-mem/GDS/LDS count).
        S_WAITCNT = 12,
        ///  set HALT bit to value of SIMM16[0]. 1=halt, 0=resume. Halt is
        /// ignored while priv=1.
        S_SETHALT,
        ///  Cause a wave to sleep for approximately 64*SIMM16[2:0] clocks.
        S_SLEEP,
        ///  User settable wave priority. 0 = lowest, 3 = highest.
        S_SETPRIO,
        ///  Send a message.
        S_SENDMSG,
        ///  Send a message and then HALT.
        S_SENDMSGHALT,
        ///  Enter the trap handler. TrapID = SIMM16[7:0]. Wait for all instructions
        /// to complete, save {pc_rewind,trapID,pc} into ttmp0,1; load TBA into PC,
        /// set PRIV=1 and continue.
        S_TRAP,
        ///  Invalidate entire L1 I cache.
        S_ICACHE_INV,
        ///  Increment performance counter specified in SIMM16[3:0]
        /// by 1.
        S_INCPERFLEVEL,
        ///  Decrement performance counter specified in SIMM16[3:0]
        /// by 1.
        S_DECPERFLEVEL,
        ///  Send M0 as user data to thread-trace.
        S_TTRACEDATA,

        S_CBRANCH_CDBGSYS = 23, // CI Specific

        S_CBRANCH_CDBGUSER = 24, // CI Specific

        S_CBRANCH_CDBGSYS_OR_USER = 25, // CI Specific

        S_CBRANCH_CDBGSYS_AND_USER = 26, // CI Specific

        /// Reserved
        S_RESERVED,
        /// ILLEGAL
        S_ILLEGAL
    };

    /// Get the OP     [22:16]
    OP GetOp() const { return m_op; }

    /// ctor
    SISOPPInstruction(SIMM16 simm16, OP op,
					  InstructionCategory insCat = FlowControl, 
		InstructionClass insClass = InstructionClassUnknown, 
		OperandType operandType = TypeNone) : 
		SOPPInstruction(simm16, insCat, insClass, operandType),
		m_op(op)
	{
	}


private:
    /// SOPP operation.
    OP m_op;

};

class VISOPPInstruction : public SOPPInstruction
{
public:
    /// Selector for the SOPP Instruction
    enum OP
    {
        s_nop                        = 0,
        s_endpgm                     = 1,
        s_branch                     = 2,
        s_wakeup                     = 3,
        s_cbranch_scc0               = 4,
        s_cbranch_scc1               = 5,
        s_cbranch_vccz               = 6,
        s_cbranch_vccnz              = 7,
        s_cbranch_execz              = 8,
        s_cbranch_execnz             = 9,
        s_barrier                    = 10,
        s_setkill                    = 11,
        s_waitcnt                    = 12,
        s_sethalt                    = 13,
        s_sleep                      = 14,
        s_setprio                    = 15,
        s_sendmsg                    = 16,
        s_sendmsghalt                = 17,
        s_trap                       = 18,
        s_icache_inv                 = 19,
        s_incperflevel               = 20,
        s_decperflevel               = 21,
        s_ttracedata                 = 22,
        s_cbranch_cdbgsys            = 23,
        s_cbranch_cdbguser           = 24,
        s_cbranch_cdbgsys_or_user    = 25,
        s_cbranch_cdbgsys_and_user   = 26,
        s_endpgm_saved               = 27,
        s_set_gpr_idx_off            = 28,
        s_set_gpr_idx_mode           = 29,
        /// ILLEGAL
        S_ILLEGAL                    = 30,
    };

    /// Get the OP     [22:16]
    OP GetOp() const { return m_op; }

    /// ctor
    VISOPPInstruction(SIMM16 simm16, OP op): SOPPInstruction(simm16), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }


private:
    /// SOPP operation.
    OP m_op;

};


#endif //__SOPPINSTRUCTION_H

