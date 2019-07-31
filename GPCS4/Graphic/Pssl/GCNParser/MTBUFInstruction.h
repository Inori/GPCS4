//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __MTBUFINSTRUCTION_H
#define __MTBUFINSTRUCTION_H

#include "Instruction.h"

/// Typed memory buffer operation. Two words
/// Opcode :
///        OFFSET [11:0]
///        OFFEN  [12:12]
///        IDXEN  [13:13]
///        GLC    [14:14]
///        ADDR64 [15:15]
///        OP     [18:16]
///        DFMT   [22:19]
///        NFMT   [25:23]
///        VADDR  [39:32]
///        VDATA  [47:40]
///        SRSRC  [52:48]
///        SLC    [54:54]
///        TFE    [55:55]
///        SOFFSET [63:56]

class MTBUFInstruction : public Instruction
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
    typedef char DFMT;
    typedef char NFMT;
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

    /// Data format for typed buffer.
    DFMT m_dmft;

    /// Number format for typed buffer.
    NFMT m_nmft;

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
    static const unsigned int MTBUFInstructionWidth = 64;
public:
    //
    // Public member functions
    //

    /// ctor
    MTBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, DFMT dmft, NFMT nmft, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                     SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, 
		Instruction::InstructionCategory inst, InstructionClass insClass = InstructionClassUnknown):
		Instruction(MTBUFInstructionWidth, inst, InstructionSet_MTBUF, insClass),
        m_offset(offset), m_offen(offen), m_idxen(idxen), m_glc(glc), m_addr64(addr64), m_dmft(dmft), m_nmft(nmft),
        m_vaddr(vaddr), m_vdata(vdata), m_srsrc(srsrc), m_slc(slc), m_tfe(tfe), m_soffset(soffset), m_ridx(ridx) {}

    /// dtor
    ~MTBUFInstruction() {}

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

    /// Get the DFMT   [22:19]
    DFMT GetDFMT() const { return m_dmft; }

    /// Get the NFMT   [25:23]
    NFMT GetNFMT() const { return m_nmft; }

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

class SIMTBUFInstruction : public MTBUFInstruction
{

public:
    /// Selector for the MTBUF Instruction
    enum OP
    {
        ///  Untyped buffer load 1 Dword with format conversion.
        TBUFFER_LOAD_FORMAT_X,
        ///  Untyped buffer load 2 Dwords with format conversion.
        TBUFFER_LOAD_FORMAT_XY,
        ///  Untyped buffer load 3 Dwords with format conversion.
        TBUFFER_LOAD_FORMAT_XYZ,
        ///  Untyped buffer load 4 Dwords with format conversion.
        TBUFFER_LOAD_FORMAT_XYZW,
        ///  Untyped buffer store 1 Dword with format conversion.
        TBUFFER_STORE_FORMAT_X,
        ///  Untyped buffer store 2 Dwords with format conversion.
        TBUFFER_STORE_FORMAT_XY,
        ///  Untyped buffer store 3 Dwords with format conversion.
        TBUFFER_STORE_FORMAT_XYZ,
        ///  Untyped buffer store 4 Dwords with format conversion.
        /// All other values are reserved.
        TBUFFER_STORE_FORMAT_XYZW,
        /// Reserved
        TBUFFER_RESERVED
    };

    /// Get the OP     [18:16]
    OP GetOp() const { return m_op; }

    /// ctor
    SIMTBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, OP op, DFMT dmft, NFMT nmft, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                       SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, Instruction::InstructionCategory inst): MTBUFInstruction(offset, offen, idxen, glc, addr64, dmft, nmft, vaddr, vdata, srsrc,
                                   slc, tfe, soffset, ridx, inst), m_op(op) {}

private:
    /// MTBUF operation.
    OP m_op;

};

class VIMTBUFInstruction : public MTBUFInstruction
{

public:
    /// Selector for the MTBUF Instruction
    enum OP
    {
        tbuffer_load_format_x          = 0,
        tbuffer_load_format_xy         = 1,
        tbuffer_load_format_xyz        = 2,
        tbuffer_load_format_xyzw       = 3,
        tbuffer_store_format_x         = 4,
        tbuffer_store_format_xy        = 5,
        tbuffer_store_format_xyz       = 6,
        tbuffer_store_format_xyzw      = 7,
        tbuffer_load_format_d16_x      = 8,
        tbuffer_load_format_d16_xy     = 9,
        tbuffer_load_format_d16_xyz    = 10,
        tbuffer_load_format_d16_xyzw   = 11,
        tbuffer_store_format_d16_x     = 12,
        tbuffer_store_format_d16_xy    = 13,
        tbuffer_store_format_d16_xyz   = 14,
        tbuffer_store_format_d16_xyzw  = 15,
        tbuffer_Ilegal = 16,
    };

    /// Get the OP     [18:16]
    OP GetOp() const { return m_op; }

    /// ctor
    VIMTBUFInstruction(OFFSET offset, OFFEN offen, IDXEN idxen, GLC glc, ADDR64 addr64, OP op, DFMT dmft, NFMT nmft, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                       SLC slc, TFE tfe, SOFFSET soffset, unsigned int ridx, Instruction::InstructionCategory inst): MTBUFInstruction(offset, offen, idxen, glc, addr64, dmft, nmft, vaddr, vdata, srsrc,
                                   slc, tfe, soffset, ridx, inst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }

private:
    /// MTBUF operation.
    OP m_op;

};
#endif //__MTBUFINSTRUCTION_H

