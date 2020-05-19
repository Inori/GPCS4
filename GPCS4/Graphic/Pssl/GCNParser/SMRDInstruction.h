//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __SMRDINSTRUCTION_H
#define __SMRDINSTRUCTION_H

#include "Instruction.h"
/// Scalar instruction performing a memory read from L1 (constant) memory.
/// Opcode :
///        OFFSET [7:0]
///        IMM    [8:8]
///        SBASE  [14:9]
///        SDST   [21:15]
///        OP     [26:22]
class SMRDInstruction : public Instruction
{
public:

#define X(SYM) SDST##SYM
#define X_INIT(SYM,VAL) SDST##SYM = VAL
    /// Destination for scalar memory read instruction
    enum SDST
    {
#include "GenericInstructionFields1.h"
        X(Illegal)
    };
#undef X_INIT
#undef X

    typedef unsigned char  OFFSET;
    typedef bool IMM;
    typedef unsigned char SBASE;
private:
    //
    // Data.
    //

    ///Unsigned eight-bit Dword offset to the address specified in SBASE.
    OFFSET m_offset;

    /// IMM = 0 (false): Specifies an SGPR address that supplies a Dword offset for the
    /// memory operation
    /// IMM = 1 (true): Specifies an 8-bit unsigned Dword offset.
    IMM m_imm;

    /// Bits [6:1] of an aligned pair of SGPRs specifying {size[16], base[48]}, where base
    /// and size are in Dword units. The low-order bits are in the first SGPR.
    SBASE m_sbase;

    /// Destination for instruction.
    SDST m_sdst;

    /// Registers index (m_sdst).
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int m_sridx;

    /// SMRD Instruction Width in bits
    static const unsigned int SMRDInstructionWidth = 32;

public:
    //
    // Public member functions
    //

    /// ctor
    SMRDInstruction(OFFSET offset, IMM imm, SBASE sbase, SDST sdst, unsigned int sridx,
		InstructionClass insClass = InstructionClassUnknown) :
        Instruction(SMRDInstructionWidth, ScalarMemory, InstructionSet_SMRD, insClass),
		m_offset(offset), m_imm(imm), m_sbase(sbase), m_sdst(sdst), m_sridx(sridx) {}

    /// dtor
    ~SMRDInstruction() {}

    /// Get the OFFSET [7:0]
    OFFSET GetOffset() const { return m_offset; }

    /// Get the IMM    [8:8]
    IMM GetImm() const { return m_imm; }

    /// Get the SBASE  [14:9]
    SBASE GetSbase() const { return m_sbase;}

    /// Get the SDST   [21:15]
    SDST GetSDST() const { return m_sdst; }

    /// Get the (scalar) register`s index.
    /// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
    unsigned int GetSRidx() const { return m_sridx; }
};

class SISMRDInstruction : public SMRDInstruction
{
public:
    /// Selector for the scalar memory read instruction.
    enum OP
    {
        ///  Read from read-only constant memory.
        S_LOAD_DWORD = 0,
        ///  Read from read-only constant memory.
        S_LOAD_DWORDX2,
        ///  Read from read-only constant memory.
        S_LOAD_DWORDX4,
        ///  Read from read-only constant memory.
        S_LOAD_DWORDX8,
        ///  Read from read-only constant memory.
        S_LOAD_DWORDX16 = 4,
        ///  Read from read-only constant memory.
        S_BUFFER_LOAD_DWORD = 8,
        ///  Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX2,
        ///  Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX4,
        ///  Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX8,
        ///  Read from read-only constant memory.
        S_BUFFER_LOAD_DWORDX16 = 12,

        S_DCACHE_INV_VOL = 29, // CI Specific

        ///  Return current 64-bit timestamp.
        S_MEMTIME = 30,
        ///  Invalidate entire L1 K cache.
        /// All other values are reserved.
        S_DCACHE_INV = 31,
        /// Reserved
        S_RESERVED
    };

    /// Get the OP     [26:22]
    OP GetOp() const { return m_op; }

    /// ctor
    SISMRDInstruction(OFFSET offset, IMM imm, SBASE sbase, SDST sdst, unsigned int sridx, OP op, 
		InstructionClass insClass = InstructionClassUnknown) : 
		SMRDInstruction(offset, imm, sbase, sdst, sridx, insClass), 
		m_op(op) {}

private:
    /// SMRD operation.
    OP m_op;

};

class VISMEMInstruction : public SMRDInstruction
{
public:
    /// Selector for the scalar memory read instruction.
    enum OP
    {
        s_load_dword            = 0,
        s_load_dwordx2          = 1,
        s_load_dwordx4          = 2,
        s_load_dwordx8          = 3,
        s_load_dwordx16         = 4,
        s_buffer_load_dword     = 8,
        s_buffer_load_dwordx2   = 9,
        s_buffer_load_dwordx4   = 10,
        s_buffer_load_dwordx8   = 11,
        s_buffer_load_dwordx16  = 12,
        s_store_dword           = 16,
        s_store_dwordx2         = 17,
        s_store_dwordx4         = 18,
        s_buffer_store_dword    = 24,
        s_buffer_store_dwordx2  = 25,
        s_buffer_store_dwordx4  = 26,
        s_dcache_inv            = 32,
        s_dcache_wb             = 33,
        s_dcache_inv_vol        = 34,
        s_dcache_wb_vol         = 35,
        s_memtime               = 36,
        s_memrealtime           = 37,
        s_atc_probe             = 38,
        s_atc_probe_buffer      = 39,
        /// Illegal
        S_ILLEGAL               = 40,
    };

    /// Get the OP     [26:22]
    OP GetOp() const { return m_op; }

    /// ctor
    VISMEMInstruction(OFFSET offset, IMM imm, SBASE sbase, SDST sdst, unsigned int sridx, OP op) : SMRDInstruction(offset, imm, sbase, sdst, sridx), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }

private:
    /// SMRD operation.
    OP m_op;

};

#endif //__SMRDINSTRUCTION_H

