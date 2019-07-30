//=============================================================
// Copyright (c) 2017 Advanced Micro Devices, Inc.
//=============================================================

#pragma once

#ifndef __FLATINSTRUCTION_H
#define __FLATINSTRUCTION_H

#include "Instruction.h"
#include "ParserSI.h"


// Representation of FLAT, SCRATCH and GLOBAL instructions.
class FLATInstruction : public Instruction
{
public:
    FLATInstruction(uint16_t OFFSET, bool LDS, uint8_t SEG, bool GLC, bool SLC, uint8_t OP, uint8_t ADDR, uint8_t DATA, uint8_t SADDR, bool NV, uint8_t VDST)
        : Instruction(64, GetInstKind(SEG, OP), InstructionSet::InstructionSet_FLAT),
          m_OFFSET(OFFSET), m_LDS(LDS), m_SEG(SEG), m_GLC(GLC), m_SLC(SLC), m_OP(OP), m_ADDR(ADDR), m_DATA(DATA), m_SADDR(SADDR), m_NV(NV), m_VDST(VDST) {}

    ~FLATInstruction() = default;

    uint16_t  GetOOFSET() const { return m_OFFSET; }
    bool      GetLDS()    const { return m_LDS;    }
    uint8_t   GetSEG()    const { return m_SEG;    }
    bool      GetGLC()    const { return m_GLC;    }
    bool      GetSLC()    const { return m_SLC;    }
    uint8_t   GetOP()     const { return m_OP;     }
    uint8_t   GetADDR()   const { return m_ADDR;   }
    uint8_t   GetDATA()   const { return m_DATA;   }
    uint8_t   GetSADDR()  const { return m_SADDR;  }
    bool      GetNV()     const { return m_NV;     }
    uint8_t   GetVDST()   const { return m_VDST;   }

private:
    // FLAT/GLOBAL opcodes
    enum FLAT_GLBL_OP
    {
        load_ubyte         = 16,
        load_sbyte         = 17,
        load_ushort        = 18,
        load_sshort        = 19,
        load_dword         = 20,
        load_dwordx2       = 21,
        load_dwordx3       = 22,
        load_dwordx4       = 23,
        store_byte         = 24,
        store_byte_d16_hi  = 25,
        store_short        = 26,
        store_short_d16_hi = 27,
        store_dword        = 28,
        store_dwordx2      = 29,
        store_dwordx3      = 30,
        store_dwordx4      = 31,
        load_ubyte_d16     = 32,
        load_ubyte_d16_hi  = 33,
        load_sbyte_d16     = 34,
        load_sbyte_d16_hi  = 35,
        load_short_d16     = 36,
        load_short_d16_hi  = 37,
        atomic_swap        = 64,
        atomic_cmpswap     = 65,
        atomic_add         = 66,
        atomic_sub         = 67,
        atomic_smin        = 68,
        atomic_umin        = 69,
        atomic_smax        = 70,
        atomic_umax        = 71,
        atomic_and         = 72,
        atomic_or          = 73,
        atomic_xor         = 74,
        atomic_inc         = 75,
        atomic_dec         = 76,
        atomic_swap_x2     = 96,
        atomic_cmpswap_x2  = 97,
        atomic_add_x2      = 98,
        atomic_sub_x2      = 99,
        atomic_smin_x2     = 100,
        atomic_umin_x2     = 101,
        atomic_smax_x2     = 102,
        atomic_umax_x2     = 103,
        atomic_and_x2      = 104,
        atomic_or_x2       = 105,
        atomic_xor_x2      = 106,
        atomic_inc_x2      = 107,
        atomic_dec_x2      = 108,

        FG_ILLEGAL            = 109
    };

    // SCRATCH opcodes
    enum SCRATCH_OP
    {
        scratch_load_ubyte         = 16,
        scratch_load_sbyte         = 17,
        scratch_load_ushort        = 18,
        scratch_load_sshort        = 19,
        scratch_load_dword         = 20,
        scratch_load_dwordx2       = 21,
        scratch_load_dwordx3       = 22,
        scratch_load_dwordx4       = 23,
        scratch_store_byte         = 24,
        scratch_store_byte_d16_hi  = 25,
        scratch_store_short        = 26,
        scratch_store_short_d16_hi = 27,
        scratch_store_dword        = 28,
        scratch_store_dwordx2      = 29,
        scratch_store_dwordx3      = 30,
        scratch_store_dwordx4      = 31,
        scratch_load_ubyte_d16     = 32,
        scratch_load_ubyte_d16_hi  = 33,
        scratch_load_sbyte_d16     = 34,
        scratch_load_sbyte_d16_hi  = 35,
        scratch_load_short_d16     = 36,
        scratch_load_short_d16_hi  = 37,

        S_ILLEGAL                    = 38
    };

    // Returns the category of instruction (Load, Store or Atomic).
    static InstructionCategory  GetInstKind(uint8_t SEG, uint8_t OP)
    {
        if (SEG == 0b01)
        {
            return ( ((SCRATCH_OP)OP >= SCRATCH_OP::scratch_store_byte && (SCRATCH_OP)OP <= SCRATCH_OP::scratch_store_dwordx4) ?
                     InstructionCategory::VectorMemoryWrite : InstructionCategory::VectorMemoryRead );
        }
        else
        {
            return ( (FLAT_GLBL_OP)OP >= FLAT_GLBL_OP::atomic_swap  ? InstructionCategory::Atomics :
                     (((FLAT_GLBL_OP)OP >= FLAT_GLBL_OP::store_byte && (FLAT_GLBL_OP)OP <= FLAT_GLBL_OP::store_dwordx4) ?
                     InstructionCategory::VectorMemoryWrite : InstructionCategory::VectorMemoryRead) );
        }
    }

    uint16_t  m_OFFSET;
    bool      m_LDS;
    uint8_t   m_SEG;     // "0 0" for FLAT, "0 1" for SCRATCH and "1 0" for GLOBAL
    bool      m_GLC;
    bool      m_SLC;
    uint8_t   m_OP;
    uint8_t   m_ADDR;
    uint8_t   m_DATA;
    uint8_t   m_SADDR;
    bool      m_NV;
    uint8_t   m_VDST;
};

// Parser for FLAT/SCRATCH/GLOBAL instructions
class ParserFLAT : public ParserSI
{
public:
    ParserFLAT() = default;
    ~ParserFLAT() = default;

    // See description in "PerserSI" class.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION hwGen, Instruction::instruction32bit hexInstruction, std::unique_ptr<Instruction>& instruction, bool& hasLiteral) override
    {
		hasLiteral = false;
        return ParserSI::Status_32BitInstructionNotSupported;
    }

    // See description in "PerserSI" class.
    virtual ParserSI::kaStatus Parse(GDT_HW_GENERATION, Instruction::instruction64bit hexInstruction, std::unique_ptr<Instruction>& instruction) override
    {
        instruction = std::make_unique<FLATInstruction>((uint16_t)INSTRUCTION_FIELD(hexInstruction, FLAT, OFFSET, 0),
                                               0 != INSTRUCTION_FIELD(hexInstruction, FLAT, LDS,   13),
                                           (uint8_t)INSTRUCTION_FIELD(hexInstruction, FLAT, SEG,   14),
                                               0 != INSTRUCTION_FIELD(hexInstruction, FLAT, GLC,   16),
                                               0 != INSTRUCTION_FIELD(hexInstruction, FLAT, SLC,   17),
                                           (uint8_t)INSTRUCTION_FIELD(hexInstruction, FLAT, OP,    18),
                                           (uint8_t)INSTRUCTION_FIELD(hexInstruction, FLAT, ADDR,  32),
                                           (uint8_t)INSTRUCTION_FIELD(hexInstruction, FLAT, DATA,  40),
                                           (uint8_t)INSTRUCTION_FIELD(hexInstruction, FLAT, SADDR, 48),
                                               0 != INSTRUCTION_FIELD(hexInstruction, FLAT, NV,    55),
                                           (uint8_t)INSTRUCTION_FIELD(hexInstruction, FLAT, VDST,  56));
        return ParserSI::Status_SUCCESS;
    }

private:
    enum FLATMask : uint64_t
    {
        FLATMask_OFFSET  = 0x0000000000000FFFULL <<  0, // [12:0]
        FLATMask_LDS     = 0x0000000000000001ULL << 13, // [13:13]
        FLATMask_SEG     = 0x0000000000000003ULL << 14, // [15:14]
        FLATMask_GLC     = 0x0000000000000001ULL << 16, // [16:16]
        FLATMask_SLC     = 0x0000000000000001ULL << 17, // [17:17]
        FLATMask_OP      = 0x000000000000007FULL << 18, // [24:18]
        FLATMask_ADDR    = 0x00000000000000FFULL << 32, // [39:32]
        FLATMask_DATA    = 0x00000000000000FFULL << 40, // [47:40]
        FLATMask_SADDR   = 0x000000000000007FULL << 48, // [54:48]
        FLATMask_NV      = 0x0000000000000001ULL << 55, // [55:55]
        FLATMask_VDST    = 0x00000000000000FFULL << 56, // [63:56]
    };
};

#endif // __FLATINSTRUCTION_H
