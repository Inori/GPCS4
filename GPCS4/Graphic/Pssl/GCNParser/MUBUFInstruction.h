//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __MUBUFINSTRUCTION_H
#define __MUBUFINSTRUCTION_H

#include "Instruction.h"

/// Untyped memory buffer operation. First word with LDS, second word non-LDS.
/// Opcode :
///        OFFSET [11:0]
///        OFFEN  [12:12]
///        IDXEN  [13:13]
///        GLC    [14:14]
///        ADDR64 [15:15]
///        LDS    [16:16]
///        OP     [24:18]
///        VADDR  [39:32]
///        VDATA  [47:40]
///        SRSRC  [52:48]
///        SLC    [54:54]
///        TFE    [55:55]
///        SOFFSET [63:56]

class MUBUFInstruction : public Instruction
{
public:

#define X(SYM) SOFFSET##SYM
#define X_INIT(SYM,VAL) SOFFSET##SYM = VAL
    /// Byte offset added to the memory address. Scalar or constant GPR containing the
    /// base offset. This is always sent.
    enum SOFFSET
    {
#include "GenericInstructionFields1.h"
#include "ScalarInstructionFields.h"
        X(Illegal)
    };
#undef X_INIT
#undef X

    typedef short OFFSET;
    typedef char OFFEN;
    typedef char IDXEN;
    typedef char GLC;
    typedef char ADDR64;
    typedef char LDS;
    typedef char VADDR;
    typedef char VDATA;
    typedef char SRSRC;
    typedef char SLC;
    typedef char TFE;


private:
    /// Unsigned byte offset. Only used when OFFEN = 0.
    OFFSET m_offset;

    /// If set, send VADDR as an offset. If clear, send the instruction offset stored in
    ///OFFSET. Only one of these offsets can be sent.
    OFFEN m_offen;

    /// If set, send VADDR as an index. If clear, treat the index as zero.
    IDXEN m_idxen;

    /// If set, operation is globally coherent.
    GLC m_glc;

    /// If set, buffer address is 64-bits (base and size in resource is ignored).
    ADDR64 m_addr64;

    /// If set, data is read from/written to LDS memory. If unset, data is read from/written
    /// to a VGPR.
    LDS m_lds;

    /// VGPR address source. Can carry an offset or an index.
    VADDR m_vaddr;

    /// Vector GPR to read/write result to.
    VDATA m_vdata;

    /// Scalar GPR that specifies the resource constant, in units of four SGPRs.
    SRSRC m_srsrc;

    /// System Level Coherent.
    SLC m_slc;

    /// Texture Fail Enable (for partially resident textures).
    TFE m_tfe;

    /// Byte offset added to the memory address. Scalar or constant GPR containing the
    ///base offset. This is always sent.
    SOFFSET m_soffset;

    /// Registers index (m_soffset).
    /// Note : Relevant only if m_soffset == ScalarGPR or m_soffset == ScalarTtmp
    unsigned int m_ridx;

    /// MTBUF Instruction Width in bits
    static const unsigned int MUBUFInstructionWidth = 64;
public:
    //
    // Public member functions
    //

    /// ctor
    MUBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, LDS lds, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                     SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, Instruction::InstructionCategory inst): Instruction(MUBUFInstructionWidth, inst, InstructionSet_MUBUF),
        m_offset(offset), m_offen(offen), m_idxen(idxen), m_glc(glc), m_addr64(addr64), m_lds(lds),
        m_vaddr(vaddr), m_vdata(vdata), m_srsrc(srsrc), m_slc(slc), m_tfe(tfe), m_soffset(soffset), m_ridx(ridx) {}

    /// dtor
    ~MUBUFInstruction() {}

    /// Get the OFFSET [11:0]
    OFFSET GetOFFSET() const { return m_offset; }

    /// Get the OFFEN  [12:12]
    OFFEN GetOFFEN() const { return m_offen; }

    /// Get the IDXEN  [13:13]
    IDXEN GetIDXEN() const { return m_idxen; }

    /// Get the GLC    [14:14]
    GLC GetGLC() const { return m_glc; }

    /// Get the  ADDR64 [15:15]
    ADDR64 GetADDR64() const { return m_addr64; }

    /// Get the LDS   [16:16]
    LDS GetLDS() const { return m_lds; }

    /// Get the VADDR  [39:32]
    VADDR GetVADDR() const { return m_vaddr; }

    /// Get the VDATA  [47:40]
    VDATA GetVDATA() const { return m_vdata; }

    /// Get the SRSRC  [52:48]
    SRSRC GetSRSRC() const { return m_srsrc; }

    /// Get the SLC    [54:54]
    SLC GetSLC() const { return m_slc; }

    /// Get the TFE    [55:55]
    TFE GetTFE() const { return m_tfe; }

    /// Get the SOFFSET [63:56]
    SOFFSET GetSOFFSET() const { return m_soffset; }
};

class SIMUBUFInstruction : public MUBUFInstruction
{

public:
    /// Selector for the MTBUF Instruction
    enum OP
    {
        ///  Untyped buffer load 1 Dword with format conversion.
        BUFFER_LOAD_FORMAT_X,
        ///  Untyped buffer load 2 Dwords with format conversion.
        BUFFER_LOAD_FORMAT_XY,
        ///  Untyped buffer load 3 Dwords with format conversion.
        BUFFER_LOAD_FORMAT_XYZ,
        ///  Untyped buffer load 4 Dwords with format conversion.
        BUFFER_LOAD_FORMAT_XYZW,
        ///  Untyped buffer store 1 Dword with format conversion.
        BUFFER_STORE_FORMAT_X,
        ///  Untyped buffer store 2 Dwords with format conversion.
        BUFFER_STORE_FORMAT_XY,
        ///  Untyped buffer store 3 Dwords with format conversion.
        BUFFER_STORE_FORMAT_XYZ,
        ///  Untyped buffer store 4 Dwords with format conversion.
        BUFFER_STORE_FORMAT_XYZW,
        ///  Untyped buffer load unsigned byte.
        BUFFER_LOAD_UBYTE,
        ///  Untyped buffer load signed byte.
        BUFFER_LOAD_SBYTE,
        ///  Untyped buffer load unsigned short.
        BUFFER_LOAD_USHORT,
        ///  Untyped buffer load signed short.
        BUFFER_LOAD_SSHORT,
        ///  Untyped buffer load Dword.
        BUFFER_LOAD_DWORD,
        ///  Untyped buffer load 2 Dwords.
        BUFFER_LOAD_DWORDX2,
        ///  Untyped buffer load 4 Dwords.
        BUFFER_LOAD_DWORDX4 = 14,

        BUFFER_LOAD_DWORDX3 = 15, // CI Specific

        ///  Untyped buffer store byte.
        BUFFER_STORE_BYTE = 24,
        ///  Untyped buffer store short.
        BUFFER_STORE_SHORT = 26,
        ///  Untyped buffer store Dword.
        BUFFER_STORE_DWORD = 28,
        ///  Untyped buffer store 2 Dwords.
        BUFFER_STORE_DWORDX2,
        ///  Untyped buffer store 4 Dwords.
        BUFFER_STORE_DWORDX4 = 30,

        BUFFER_STORE_DWORDX3 = 31, // CI Specific

        ///  32b. dst=src, returns previous value if glc==1.
        BUFFER_ATOMIC_SWAP = 48,
        ///  32b, dst = (dst==cmp) ? src : dst. Returns previous
        /// value if glc==1. src comes from the first data-vgpr, cmp from the second.
        BUFFER_ATOMIC_CMPSWAP,
        ///  32b, dst += src. Returns previous value if glc==1.
        BUFFER_ATOMIC_ADD,
        ///  32b, dst -= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_SUB,
        ///  32b, dst = src-dst. Returns previous value if glc==1.
        BUFFER_ATOMIC_RSUB,
        ///  32b, dst = (src < dst) ? src : dst (signed). Returns previous
        /// value if glc==1.
        BUFFER_ATOMIC_SMIN,
        ///  32b, dst = (src < dst) ? src : dst (unsigned). Returns
        /// previous value if glc==1.
        BUFFER_ATOMIC_UMIN,
        ///  32b, dst = (src > dst) ? src : dst (signed). Returns previous
        /// value if glc==1.
        BUFFER_ATOMIC_SMAX,
        ///  32b, dst = (src > dst) ? src : dst (unsigned). Returns
        /// previous value if glc==1.
        BUFFER_ATOMIC_UMAX,
        ///  32b, dst &= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_AND,
        ///  32b, dst |= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_OR,
        ///  32b, dst ^= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_XOR,
        ///  32b, dst = (dst >= src) ? 0 : dst+1. Returns previous
        /// value if glc==1.
        BUFFER_ATOMIC_INC,
        ///  32b, dst = ((dst==0 || (dst > src)) ? src : dst-1. Returns
        /// previous value if glc==1.
        BUFFER_ATOMIC_DEC,
        ///  32b , dst = (dst == cmp) ? src : dst, returns previous
        /// value if glc==1. Float compare swap (handles NaN/INF/denorm). src
        /// comes from the first data-vgpr; cmp from the second.
        BUFFER_ATOMIC_FCMPSWAP,
        ///  32b , dst = (src < dst) ? src : dst,. Returns previous
        /// value if glc==1. float, handles NaN/INF/denorm.
        BUFFER_ATOMIC_FMIN,
        ///  32b , dst = (src > dst) ? src : dst, returns previous value
        /// if glc==1. float, handles NaN/INF/denorm.
        BUFFER_ATOMIC_FMAX,
        ///  64b. dst=src, returns previous value if glc==1.
        BUFFER_ATOMIC_SWAP_X2 = 80,
        ///  64b, dst = (dst==cmp) ? src : dst. Returns previous
        /// value if glc==1. src comes from the first two data-vgprs, cmp from the
        /// second two.
        BUFFER_ATOMIC_CMPSWAP_X2,
        ///  64b, dst += src. Returns previous value if glc==1.
        BUFFER_ATOMIC_ADD_X2,
        ///  64b, dst -= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_SUB_X2,
        ///  64b, dst = src-dst. Returns previous value if
        /// glc==1.
        BUFFER_ATOMIC_RSUB_X2,
        ///  64b, dst = (src < dst) ? src : dst (signed). Returns
        /// previous value if glc==1.
        BUFFER_ATOMIC_SMIN_X2,
        ///  64b, dst = (src < dst) ? src : dst (unsigned).
        /// Returns previous value if glc==1.
        BUFFER_ATOMIC_UMIN_X2,
        ///  64b, dst = (src > dst) ? src : dst (signed). Returns
        /// previous value if glc==1.
        BUFFER_ATOMIC_SMAX_X2,
        ///  64b, dst = (src > dst) ? src : dst (unsigned).
        /// Returns previous value if glc==1.
        BUFFER_ATOMIC_UMAX_X2,
        ///  64b, dst &= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_AND_X2,
        ///  64b, dst |= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_OR_X2,
        ///  64b, dst ^= src. Returns previous value if glc==1.
        BUFFER_ATOMIC_XOR_X2,
        ///  64b, dst = (dst >= src) ? 0 : dst+1. Returns previous
        /// value if glc==1.
        BUFFER_ATOMIC_INC_X2,
        ///  64b, dst = ((dst==0 || (dst > src)) ? src : dst-1.
        /// Returns previous value if glc==1.
        BUFFER_ATOMIC_DEC_X2,
        ///  64b , dst = (dst == cmp) ? src : dst, returns
        /// previous value if glc==1. Double compare swap (handles NaN/INF/denorm).
        /// src comes from the first two data-vgprs, cmp from the second two.
        BUFFER_ATOMIC_FCMPSWAP_X2,
        ///  64b , dst = (src < dst) ? src : dst, returns previous
        /// value if glc==1. Double, handles NaN/INF/denorm.
        BUFFER_ATOMIC_FMIN_X2,
        ///  64b , dst = (src > dst) ? src : dst, returns previous
        /// value if glc==1. Double, handles NaN/INF/denorm.
        BUFFER_ATOMIC_FMAX_X2,
        ///  write back and invalidate the shader L1 only for lines of
        /// MTYPE SC and GC. Always returns ACK to shader.
        BUFFER_WBINVL1_SC = 112, // SI Only

        BUFFER_WBINVL1_VOL = 112, // CI Only

        ///  write back and invalidate the shader L1. Always returns
        /// ACK to shader.
        /// All other values are reserved.
        BUFFER_WBINVL1,
        /// Reserved
        BUFFER_RESERVED
    };

    /// Get the OP     OP [24:18]
    OP GetOp() const { return m_op; }

    /// ctor
    SIMUBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, LDS lds, OP op, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                       SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, Instruction::InstructionCategory inst): MUBUFInstruction(offset, offen, idxen, glc, addr64, lds, vaddr, vdata, srsrc,
                                   slc, tfe, soffset, ridx, inst), m_op(op) {}

private:

    /// MTBUF operation.
    OP m_op;

};


class VIMUBUFInstruction : public MUBUFInstruction
{

public:
    /// Selector for the MTBUF Instruction
    enum OP
    {
        buffer_load_format_x         = 0,
        buffer_load_format_xy        = 1,
        buffer_load_format_xyz       = 2,
        buffer_load_format_xyzw      = 3,
        buffer_store_format_x        = 4,
        buffer_store_format_xy       = 5,
        buffer_store_format_xyz      = 6,
        buffer_store_format_xyzw     = 7,
        buffer_load_format_d16_x     = 8,
        buffer_load_format_d16_xy    = 9,
        buffer_load_format_d16_xyz   = 10,
        buffer_load_format_d16_xyzw  = 11,
        buffer_store_format_d16_x    = 12,
        buffer_store_format_d16_xy   = 13,
        buffer_store_format_d16_xyz  = 14,
        buffer_store_format_d16_xyzw = 15,
        buffer_load_ubyte            = 16,
        buffer_load_sbyte            = 17,
        buffer_load_ushort           = 18,
        buffer_load_sshort           = 19,
        buffer_load_dword            = 20,
        buffer_load_dwordx2          = 21,
        buffer_load_dwordx3          = 22,
        buffer_load_dwordx4          = 23,
        buffer_store_byte            = 24,
        buffer_store_short           = 26,
        buffer_store_dword           = 28,
        buffer_store_dwordx2         = 29,
        buffer_store_dwordx3         = 30,
        buffer_store_dwordx4         = 31,
        buffer_store_lds_dword       = 61,
        buffer_wbinvl1               = 62,
        buffer_wbinvl1_vol           = 63,
        buffer_atomic_swap           = 64,
        buffer_atomic_cmpswap        = 65,
        buffer_atomic_add            = 66,
        buffer_atomic_sub            = 67,
        buffer_atomic_smin           = 68,
        buffer_atomic_umin           = 69,
        buffer_atomic_smax           = 70,
        buffer_atomic_umax           = 71,
        buffer_atomic_and            = 72,
        buffer_atomic_or             = 73,
        buffer_atomic_xor            = 74,
        buffer_atomic_inc            = 75,
        buffer_atomic_dec            = 76,
        buffer_atomic_swap_x2        = 96,
        buffer_atomic_cmpswap_x2     = 97,
        buffer_atomic_add_x2         = 98,
        buffer_atomic_sub_x2         = 99,
        buffer_atomic_smin_x2        = 100,
        buffer_atomic_umin_x2        = 101,
        buffer_atomic_smax_x2        = 102,
        buffer_atomic_umax_x2        = 103,
        buffer_atomic_and_x2         = 104,
        buffer_atomic_or_x2          = 105,
        buffer_atomic_xor_x2         = 106,
        buffer_atomic_inc_x2         = 107,
        buffer_atomic_dec_x2         = 108  ,
        buffer_Ilegal = 109,
    };

    /// Get the OP     OP [24:18]
    OP GetOp() const { return m_op; }

    /// ctor
    VIMUBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, LDS lds, OP op, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                       SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, Instruction::InstructionCategory inst): MUBUFInstruction(offset, offen, idxen, glc, addr64, lds, vaddr, vdata, srsrc,
                                   slc, tfe, soffset, ridx, inst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }

private:

    /// MTBUF operation.
    OP m_op;

};


// MUBUF instructions for VEGA (GFX9)
class G9MUBUFInstruction : public MUBUFInstruction
{

public:
    /// Selector for the MTBUF Instruction
    enum OP
    {
        buffer_load_format_x         = 0,
        buffer_load_format_xy        = 1,
        buffer_load_format_xyz       = 2,
        buffer_load_format_xyzw      = 3,
        buffer_store_format_x        = 4,
        buffer_store_format_xy       = 5,
        buffer_store_format_xyz      = 6,
        buffer_store_format_xyzw     = 7,
        buffer_load_format_d16_x     = 8,
        buffer_load_format_d16_xy    = 9,
        buffer_load_format_d16_xyz   = 10,
        buffer_load_format_d16_xyzw  = 11,
        buffer_store_format_d16_x    = 12,
        buffer_store_format_d16_xy   = 13,
        buffer_store_format_d16_xyz  = 14,
        buffer_store_format_d16_xyzw = 15,
        buffer_load_ubyte            = 16,
        buffer_load_sbyte            = 17,
        buffer_load_ushort           = 18,
        buffer_load_sshort           = 19,
        buffer_load_dword            = 20,
        buffer_load_dwordx2          = 21,
        buffer_load_dwordx3          = 22,
        buffer_load_dwordx4          = 23,
        buffer_store_byte            = 24,
        buffer_store_short           = 26,
        buffer_store_dword           = 28,
        buffer_store_dwordx2         = 29,
        buffer_store_dwordx3         = 30,
        buffer_store_dwordx4         = 31,
        buffer_load_ubyte_d16        = 32,
        buffer_load_ubyte_d16_hi     = 33,
        buffer_load_sbyte_d16        = 34,
        buffer_load_sbyte_d16_hi     = 35,
        buffer_load_short_d16        = 36,
        buffer_load_short_d16_hi     = 37,
        buffer_load_format_d16_hi_x  = 38,
        buffer_store_format_d16_hi_x = 39,
        buffer_store_lds_dword       = 61,
        buffer_wbinvl1               = 62,
        buffer_wbinvl1_vol           = 63,
        buffer_atomic_swap           = 64,
        buffer_atomic_cmpswap        = 65,
        buffer_atomic_add            = 66,
        buffer_atomic_sub            = 67,
        buffer_atomic_smin           = 68,
        buffer_atomic_umin           = 69,
        buffer_atomic_smax           = 70,
        buffer_atomic_umax           = 71,
        buffer_atomic_and            = 72,
        buffer_atomic_or             = 73,
        buffer_atomic_xor            = 74,
        buffer_atomic_inc            = 75,
        buffer_atomic_dec            = 76,
        buffer_atomic_swap_x2        = 96,
        buffer_atomic_cmpswap_x2     = 97,
        buffer_atomic_add_x2         = 98,
        buffer_atomic_sub_x2         = 99,
        buffer_atomic_smin_x2        = 100,
        buffer_atomic_umin_x2        = 101,
        buffer_atomic_smax_x2        = 102,
        buffer_atomic_umax_x2        = 103,
        buffer_atomic_and_x2         = 104,
        buffer_atomic_or_x2          = 105,
        buffer_atomic_xor_x2         = 106,
        buffer_atomic_inc_x2         = 107,
        buffer_atomic_dec_x2         = 108,
        buffer_Illegal               = 109
    };

    /// Get the OP     OP [24:18]
    OP GetOp() const { return m_op; }

    /// ctor
    G9MUBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, LDS lds, OP op, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                       SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, Instruction::InstructionCategory inst): MUBUFInstruction(offset, offen, idxen, glc, addr64, lds, vaddr, vdata, srsrc,
                                   slc, tfe, soffset, ridx, inst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }

private:

    /// MTBUF operation.
    OP m_op;
};


#endif //__MUBUFINSTRUCTION_H

