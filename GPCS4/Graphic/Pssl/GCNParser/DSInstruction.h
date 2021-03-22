//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __DSINSTRUCTION_H
#define __DSINSTRUCTION_H

#include "Instruction.h"

/// Local and global data share instructions.
/// Opcode :
///        OFFSET0 [7:0]
///        OFFSET1 [15:8]
///        GDS     [17:17] - 0 = LDS; 1 = GDS
///        OP      [25:18]
///        ADDR    [39:32]
///        DATA0 [47:40]
///        DATA1 [55:48]
///        VDST [63:56]

class DSInstruction : public Instruction
{
public:
    typedef char OFFSET;
    typedef bool GDS;
    typedef char ADDR;
    typedef char DATA;
    typedef char VDST;

protected:
    /// Unsigned byte offset added to the address supplied by the ADDR VGPR.
    OFFSET m_offset0;

    // Unsigned byte offset added to the address supplied by the ADDR VGPR.
    OFFSET m_offset1;

    /// 0(false) = LDS; 1(true) = GDS.
    GDS m_isGDS;

    /// Source LDS address VGPR 0 - 255.
    ADDR m_addr;

    /// Source data0 VGPR 0 - 255.
    DATA m_data0;

    /// Source data1 VGPR 0 - 255.
    DATA m_data1;

    /// Destination VGPR 0 - 255.
    VDST m_vdst;

    /// DS Instruction Width in bits
    static const unsigned int DSInstructionWidth = 64;
public:
    //
    // Public member functions
    //

    /// ctor
	DSInstruction(OFFSET offset0, OFFSET offset1, GDS isGDS, ADDR addr, DATA data0, DATA data1, VDST vdst, 
		InstructionClass insClass = InstructionClassUnknown, OperandType instructionOperandType = Instruction::TypeNone) :
		Instruction(DSInstructionWidth, Instruction::DataShare, InstructionSet_DS, insClass, instructionOperandType),
        m_offset0(offset0), m_offset1(offset1), m_isGDS(isGDS), m_addr(addr), m_data0(data0), m_data1(data1), m_vdst(vdst) { }

    /// dtor
    virtual ~DSInstruction() {}

    /// Get the OFFSET0 [7:0] / OFFSET1 [15:8]
    /// \param[in] offsetIdx The offset`s index (0 or 1)
    OFFSET GetOFFSET(const unsigned char offsetIdx) const
    {
        if (offsetIdx)
        {
            return m_offset1;
        }

        return m_offset0;
    }


    /// Get the GDS [17:17]
    GDS GetGDS() const { return m_isGDS; }

    /// Get the ADDR [39:32]
    ADDR GetADDR() const { return m_addr; }

    /// Get the DATA0 [47:40] / DATA1 [55:48]
    /// \param[in] dataIdx The data`s index (0 or 1)
    DATA GetDATA(const unsigned char dataIdx) const
    {
        if (dataIdx)
        {
            return m_data1;
        }

        return m_data0;
    }

    /// Get the VDST [63:56]
    VDST GetVDST() const { return m_vdst; }


};

class SIDSInstruction : public DSInstruction
{

public:
    /// Selector for the DS Instruction
    enum OP
    {
        /// DS[A] = DS[A] + D0; uint add.
        DS_ADD_U32,
        /// DS[A] = DS[A] - D0; uint subtract.
        DS_SUB_U32,
        /// DS[A] = D0 - DS[A]; uint reverse subtract.
        DS_RSUB_U32,
        /// DS[A] = (DS[A] >= D0 ? 0 : DS[A] + 1); uint increment.
        DS_INC_U32,
        /// DS[A] = (DS[A] == 0 || DS[A] > D0 ? D0 : DS[A] -
        /// 1); uint decrement.
        DS_DEC_U32,
        /// DS[A] = min(DS[A], D0); int min.
        DS_MIN_I32,
        /// DS[A] = max(DS[A], D0); int max.
        DS_MAX_I32,
        /// DS[A] = min(DS[A], D0); uint min.
        DS_MIN_U32,
        /// DS[A] = max(DS[A], D0); uint max.
        DS_MAX_U32,
        /// DS[A] = DS[A] & D0; Dword AND.
        DS_AND_B32,
        /// DS[A] = DS[A] | D0; Dword OR.
        DS_OR_B32,
        /// DS[A] = DS[A] ^ D0; Dword XOR.
        DS_XOR_B32,
        /// DS[A] = (DS[A] ^ ~D0) | D1; masked Dword OR.
        DS_MSKOR_B32,
        /// DS[A] = D0; write a Dword.
        DS_WRITE_B32,
        /// DS[ADDR+offset0*4] = D0;
        /// DS[ADDR+offset1*4] = D1; write 2 Dwords.
        DS_WRITE2_B32,
        /// DS[ADDR+offset0*4*64] = D0;
        /// DS[ADDR+offset1*4*64] = D1; write 2 Dwords.
        DS_WRITE2ST64_B32,
        /// DS[A] = (DS[A] == D0 ? D1 : DS[A]); compare
        /// store.
        DS_CMPST_B32,
        /// DS[A] = (DS[A] == D0 ? D1 : DS[A]); compare
        /// store with float rules.
        DS_CMPST_F32,
        /// DS[A] = (DS[A] < D1) ? D0 : DS[A]; float compare
        /// swap (handles NaN/INF/denorm).
        DS_MIN_F32,
        /// DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MAX_F32 = 19,

        DS_NOP = 20, // CI Specific
        DS_GWS_SEMA_RELEASE_ALL = 24, //CI Specific

        /// GDS only.
        DS_GWS_INIT = 25,
        /// GDS only.
        DS_GWS_SEMA_V,
        /// GDS only.
        DS_GWS_SEMA_BR,
        /// GDS only.
        DS_GWS_SEMA_P,
        /// GDS only.
        DS_GWS_BARRIER,
        /// DS[A] = D0[7:0]; byte write.
        DS_WRITE_B8,
        /// DS[A] = D0[15:0]; short write.
        DS_WRITE_B16,
        /// Uint add.
        DS_ADD_RTN_U32,
        /// Uint subtract.
        DS_SUB_RTN_U32,
        /// Uint reverse subtract.
        DS_RSUB_RTN_U32,
        /// Uint increment.
        DS_INC_RTN_U32,
        /// Uint decrement.
        DS_DEC_RTN_U32,
        /// Int min.
        DS_MIN_RTN_I32,
        /// Int max.
        DS_MAX_RTN_I32,
        /// Uint min.
        DS_MIN_RTN_U32,
        /// Uint max.
        DS_MAX_RTN_U32,
        /// Dword AND.
        DS_AND_RTN_B32,
        /// Dword OR.
        DS_OR_RTN_B32,
        /// Dword XOR.
        DS_XOR_RTN_B32,
        /// Masked Dword OR.
        DS_MSKOR_RTN_B32,
        /// Write exchange. Offset = {offset1,offset0}.
        /// A = ADDR+offset. D=DS[Addr].
        /// DS[Addr]=D0.
        DS_WRXCHG_RTN_B32,
        /// Write exchange 2 separate Dwords.
        DS_WRXCHG2_RTN_B32,
        /// Write echange 2 Dwords, stride 64.
        DS_WRXCHG2ST64_RTN_B32,
        /// Compare store.
        DS_CMPST_RTN_B32,
        /// Compare store with float rules.
        DS_CMPST_RTN_F32,
        /// DS[A] = (DS[A] < D1) ? D0 : DS[A]; float compare
        /// swap (handles NaN/INF/denorm).
        DS_MIN_RTN_F32,
        /// DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm .
        DS_MAX_RTN_F32 = 51,

        DS_WRAP_RTN_B32 = 52, //CI Specific

        /// R = swizzle(Data(vgpr), offset1:offset0).
        /// Dword swizzle. no data is written to
        /// LDS. see ds_opcodes.docx for details.
        DS_SWIZZLE_B32 = 53,
        /// R = DS[A]; Dword read.
        DS_READ_B32,
        /// R = DS[ADDR+offset0*4], R+1 =
        /// DS[ADDR+offset1*4]. Read 2 Dwords.
        DS_READ2_B32,
        /// R = DS[ADDR+offset0*4*64], R+1 =
        /// DS[ADDR+offset1*4*64]. Read 2
        /// Dwords.
        DS_READ2ST64_B32,
        /// R = signext(DS[A][7:0]}; signed byte read.
        DS_READ_I8,
        /// R = {24�h0,DS[A][7:0]}; unsigned byte read.
        DS_READ_U8,
        /// R = signext(DS[A][15:0]}; signed short read.
        DS_READ_I16,
        /// R = {16�h0,DS[A][15:0]}; unsigned short read.
        DS_READ_U16,
        /// Consume entries from a buffer.
        DS_CONSUME,
        /// Append one or more entries to a buffer.
        DS_APPEND,
        /// Increment an append counter. Operation is
        /// done in order of wavefront creation.
        DS_ORDERED_COUNT,
        /// Uint add.
        DS_ADD_U64,
        /// Uint subtract.
        DS_SUB_U64,
        /// Uint reverse subtract.
        DS_RSUB_U64,
        /// Uint increment.
        DS_INC_U64,
        /// Uint decrement.
        DS_DEC_U64,
        /// Int min.
        DS_MIN_I64,
        /// Int max.
        DS_MAX_I64,
        /// Uint min.
        DS_MIN_U64,
        /// Uint max.
        DS_MAX_U64,
        /// Dword AND.
        DS_AND_B64,
        /// Dword OR.
        DS_OR_B64,
        /// Dword XOR.
        DS_XOR_B64,
        /// Masked Dword XOR.
        DS_MSKOR_B64,
        /// Write.
        DS_WRITE_B64,
        /// DS[ADDR+offset0*8] = D0;
        /// DS[ADDR+offset1*8] = D1; write 2 Dwords.
        DS_WRITE2_B64,
        /// DS[ADDR+offset0*8*64] = D0;
        /// DS[ADDR+offset1*8*64] = D1; write 2 Dwords.
        DS_WRITE2ST64_B64,
        /// Compare store.
        DS_CMPST_B64,
        /// Compare store with float rules.
        DS_CMPST_F64,
        /// DS[A] = (D0 < DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MIN_F64,
        /// DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MAX_F64 = 83,
        /// Uint add.
        DS_ADD_RTN_U64 = 96,
        /// Uint subtract.
        DS_SUB_RTN_U64,
        /// Uint reverse subtract.
        DS_RSUB_RTN_U64,
        /// Uint increment.
        DS_INC_RTN_U64,
        /// Uint decrement.
        DS_DEC_RTN_U64,
        /// Int min.
        DS_MIN_RTN_I64,
        /// Int max.
        DS_MAX_RTN_I64,
        /// Uint min.
        DS_MIN_RTN_U64,
        /// Uint max.
        DS_MAX_RTN_U64,
        /// Dword AND.
        DS_AND_RTN_B64,
        /// Dword OR.
        DS_OR_RTN_B64,
        /// Dword XOR.
        DS_XOR_RTN_B64,
        /// Masked Dword XOR.
        DS_MSKOR_RTN_B64,
        /// Write exchange.
        DS_WRXCHG_RTN_B64,
        /// Write exchange relative.
        DS_WRXCHG2_RTN_B64,
        /// Write echange 2 Dwords.
        DS_WRXCHG2ST64_RTN_B64,
        /// Compare store.
        DS_CMPST_RTN_B64,
        /// Compare store with float rules.
        DS_CMPST_RTN_F64,
        /// DS[A] = (D0 < DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MIN_RTN_F64,
        /// DS[A] = (D0 > DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MAX_RTN_F64 = 115,
        /// Dword read.
        DS_READ_B64 = 118,
        /// R = DS[ADDR+offset0*8], R+1 =
        /// DS[ADDR+offset1*8]. Read 2 Dwords
        DS_READ2_B64 = 119,
        /// R = DS[ADDR+offset0*8*64], R+1 =
        /// DS[ADDR+offset1*8*64]. Read 2 Dwords.
        DS_READ2ST64_B64 = 120,

        DS_CONDXCHG32_RTN_B64 = 126, //CI Specific

        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A]
        /// + DS[B]; uint add.
        DS_ADD_SRC2_U32 = 128,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A]
        /// - DS[B]; uint subtract.
        DS_SUB_SRC2_U32 = 129,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = DS[B]
        /// - DS[A]; uint reverse subtract
        DS_RSUB_SRC2_U32 = 130,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = (DS[A]
        /// >= DS[B] ? 0 : DS[A] + 1); uint increment.
        DS_INC_SRC2_U32 = 131,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = (DS[A]
        /// == 0 || DS[A] > DS[B] ? DS[B] : DS[A] - 1); uint
        /// decrement.
        DS_DEC_SRC2_U32 = 132,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] =
        /// min(DS[A], DS[B]); int min.
        DS_MIN_SRC2_I32 = 133,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] =
        /// max(DS[A], DS[B]); int max.
        DS_MAX_SRC2_I32 = 134,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] =
        /// min(DS[A], DS[B]); uint min.
        DS_MIN_SRC2_U32 = 135,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = max(DS[A], DS[B]); uint maxw
        DS_MAX_SRC2_U32 = 136,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] =
        /// max(DS[A], DS[B]); uint maxw137
        /// DS_AND_SRC2_B32B = A + 4*(offset1[7] ?
        DS_AND_SRC2_B32 = 137,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] |
        /// DS[B]; Dword OR.
        DS_OR_SRC2_B32 = 138,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = DS[A] ^
        /// DS[B]; Dword XOR.
        DS_XOR_SRC2_B32 = 139,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = DS[B];
        /// write Dword.
        DS_WRITE_SRC2_B32 = 140,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = (DS[B]
        /// < DS[A]) ? DS[B] : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MIN_SRC2_F32 = 146,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] = (DS[B]
        /// > DS[A]) ? DS[B] : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MAX_SRC2_F32 = 147,
        /// Uint add.
        DS_ADD_SRC2_U64 = 192,
        /// Uint subtract.
        DS_SUB_SRC2_U64,
        /// Uint reverse subtract.
        DS_RSUB_SRC2_U64,
        /// Uint increment.
        DS_INC_SRC2_U64,
        /// Uint decrement.
        DS_DEC_SRC2_U64,
        /// Int min.
        DS_MIN_SRC2_I64,
        /// Int max.
        DS_MAX_SRC2_I64,
        /// Uint min.
        DS_MIN_SRC2_U64,
        /// Uint max.
        DS_MAX_SRC2_U64 = 200,
        /// Dword AND.
        DS_AND_SRC2_B64,
        /// Dword OR.
        DS_OR_SRC2_B64,
        /// Dword XOR.
        DS_XOR_SRC2_B64,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). DS[A] =
        /// DS[B]; write Qword.
        DS_WRITE_SRC2_B64 = 204,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). [A] = (D0 <
        /// DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MIN_SRC2_F64 = 210,
        /// B = A + 4*(offset1[7] ? {A[31],A[31:17]} :
        /// {offset1[6],offset1[6:0],offset0}). [A] = (D0 >
        /// DS[A]) ? D0 : DS[A]; float, handles
        /// NaN/INF/denorm.
        DS_MAX_SRC2_F64 = 211,

        DS_WRITE_B96 = 222, // CI Specific
        DS_WRITE_B128 = 223, // CI Specific
        DS_CONDXCHG32_RTN_B128 = 253, // CI Specific
        DS_READ_B96 = 254, // CI Specific
        DS_READ_B128 = 255, // CI Specific
        /// Reserved
        DS_RESERVED
    };

    int GetInstructionClockCount()
    {
        int iRet = 1;

        switch (m_op)
        {
            case    DS_MIN_I32:
            case    DS_MAX_I32:
            case    DS_MIN_U32:
            case    DS_MAX_U32:
            case    DS_AND_B32:
            case    DS_OR_B32:
            case    DS_XOR_B32:
            case    DS_MSKOR_B32:
            case    DS_WRITE_B32:
            case    DS_WRITE2_B32:
            case    DS_WRITE2ST64_B32:
            case    DS_CMPST_B32:
            case    DS_CMPST_F32:
            case    DS_MIN_F32:
            case    DS_MAX_F32:
            case    DS_WRITE_B8:
            case    DS_WRITE_B16:
            case    DS_ADD_RTN_U32:
            case    DS_SUB_RTN_U32:
            case    DS_RSUB_RTN_U32:
            case    DS_INC_RTN_U32:
            case    DS_DEC_RTN_U32:
            case    DS_MIN_RTN_I32:
            case    DS_MAX_RTN_I32:
            case    DS_MIN_RTN_U32:
            case    DS_MAX_RTN_U32:
            case    DS_AND_RTN_B32:
            case    DS_OR_RTN_B32:
            case    DS_XOR_RTN_B32:
            case    DS_MSKOR_RTN_B32:
            case    DS_WRXCHG_RTN_B32:
            case    DS_WRXCHG2_RTN_B32:
            case    DS_WRXCHG2ST64_RTN_B32:
            case    DS_CMPST_RTN_B32:
            case    DS_CMPST_RTN_F32:
            case    DS_MIN_RTN_F32:
            case    DS_MAX_RTN_F32:
            case    DS_NOP: // hunch
                iRet = 4;
                break;

            case    DS_SWIZZLE_B32:
            case    DS_READ_B32:
            case    DS_READ2_B32:
            case    DS_READ2ST64_B32:
            case    DS_READ_I8:
            case    DS_READ_U8:
            case    DS_READ_I16:
            case    DS_READ_U16:
            case    DS_CONSUME:
            case    DS_APPEND:
            case    DS_ADD_U64:
            case    DS_SUB_U64:
            case    DS_RSUB_U64:
            case    DS_INC_U64:
            case    DS_DEC_U64:
            case    DS_MIN_I64:
            case    DS_MAX_I64:
            case    DS_MIN_U64:
            case    DS_MAX_U64:
            case    DS_AND_B64:
            case    DS_OR_B64:
            case    DS_XOR_B64:
            case    DS_MSKOR_B64:
            case    DS_WRITE_B64:
            case    DS_WRITE2_B64:
            case    DS_WRITE2ST64_B64:
            case    DS_CMPST_B64:
            case    DS_CMPST_F64:
            case    DS_MIN_F64:
            case    DS_MAX_F64:
                iRet = 8;
                break;

            case    DS_ADD_RTN_U64:
            case    DS_SUB_RTN_U64:
            case    DS_RSUB_RTN_U64:
            case    DS_INC_RTN_U64:
            case    DS_DEC_RTN_U64:
            case    DS_MIN_RTN_I64:
            case    DS_MAX_RTN_I64:
            case    DS_MIN_RTN_U64:
            case    DS_MAX_RTN_U64:
            case    DS_AND_RTN_B64:
            case    DS_OR_RTN_B64:
            case    DS_XOR_RTN_B64:
            case    DS_MSKOR_RTN_B64:
            case    DS_WRXCHG_RTN_B64:
            case    DS_WRXCHG2_RTN_B64:
            case    DS_WRXCHG2ST64_RTN_B64:
            case    DS_CMPST_RTN_B64:
            case    DS_CMPST_RTN_F64:
            case    DS_MIN_RTN_F64:
            case    DS_MAX_RTN_F64:
            case    DS_READ_B64:
            case    DS_READ2_B64:
            case    DS_READ2ST64_B64:
            case    DS_ADD_SRC2_U32:
            case    DS_SUB_SRC2_U32:
            case    DS_RSUB_SRC2_U32:
            case    DS_INC_SRC2_U32:
            case    DS_DEC_SRC2_U32:
            case    DS_MIN_SRC2_I32:
            case    DS_MAX_SRC2_I32:
            case    DS_MIN_SRC2_U32:
            case    DS_MAX_SRC2_U32:
            case    DS_AND_SRC2_B32:
            case    DS_OR_SRC2_B32:
            case    DS_XOR_SRC2_B32:
            case    DS_WRITE_SRC2_B32:
            case    DS_MIN_SRC2_F32:
            case    DS_MAX_SRC2_F32:
                iRet = 12;
                break;

            case    DS_ADD_SRC2_U64:
            case    DS_SUB_SRC2_U64:
            case    DS_RSUB_SRC2_U64:
            case    DS_INC_SRC2_U64:
            case    DS_DEC_SRC2_U64:
            case    DS_MIN_SRC2_I64:
            case    DS_MAX_SRC2_I64:
            case    DS_MIN_SRC2_U64:
            case    DS_MAX_SRC2_U64:
            case    DS_AND_SRC2_B64:
            case    DS_OR_SRC2_B64:
            case    DS_XOR_SRC2_B64:
            case    DS_WRITE_SRC2_B64:
            case    DS_MIN_SRC2_F64:
            case    DS_MAX_SRC2_F64:
                iRet = 20;
                break;

            default:
                break;
        }

        return iRet;
    }

    SIDSInstruction(OFFSET offset0, OFFSET offset1, GDS isGDS, OP op, ADDR addr, DATA data0, DATA data1, VDST vdst,
		InstructionClass insClass, OperandType instructionOperandType) :
		DSInstruction(offset0, offset1, isGDS, addr, data0, data1, vdst, insClass, instructionOperandType), 
		m_op(op)
    { }

    /// Get the OP [27:23]
    OP GetOp() const { return m_op; }

private:
    /// DS operation.
    OP m_op;




};

class VIDSInstruction : public DSInstruction
{

public:
    /// Selector for the DS Instruction
    enum OP
    {
        ds_add_u32              = 0,
        ds_sub_u32              = 1,
        ds_rsub_u32             = 2,
        ds_inc_u32              = 3,
        ds_dec_u32              = 4,
        ds_min_i32              = 5,
        ds_max_i32              = 6,
        ds_min_u32              = 7,
        ds_max_u32              = 8,
        ds_and_b32              = 9,
        ds_or_b32               = 10,
        ds_xor_b32              = 11,
        ds_mskor_b32            = 12,
        ds_write_b32            = 13,
        ds_write2_b32           = 14,
        ds_write2st64_b32       = 15,
        ds_cmpst_b32            = 16,
        ds_cmpst_f32            = 17,
        ds_min_f32              = 18,
        ds_max_f32              = 19,
        ds_nop                  = 20,
        ds_add_f32              = 21,
        ds_write_b8             = 30,
        ds_write_b16            = 31,
        ds_add_rtn_u32          = 32,
        ds_sub_rtn_u32          = 33,
        ds_rsub_rtn_u32         = 34,
        ds_inc_rtn_u32          = 35,
        ds_dec_rtn_u32          = 36,
        ds_min_rtn_i32          = 37,
        ds_max_rtn_i32          = 38,
        ds_min_rtn_u32          = 39,
        ds_max_rtn_u32          = 40,
        ds_and_rtn_b32          = 41,
        ds_or_rtn_b32           = 42,
        ds_xor_rtn_b32          = 43,
        ds_mskor_rtn_b32        = 44,
        ds_wrxchg_rtn_b32       = 45,
        ds_wrxchg2_rtn_b32      = 46,
        ds_wrxchg2st64_rtn_b32  = 47,
        ds_cmpst_rtn_b32        = 48,
        ds_cmpst_rtn_f32        = 49,
        ds_min_rtn_f32          = 50,
        ds_max_rtn_f32          = 51,
        ds_wrap_rtn_b32         = 52,
        ds_add_rtn_f32          = 53,
        ds_read_b32             = 54,
        ds_read2_b32            = 55,
        ds_read2st64_b32        = 56,
        ds_read_i8              = 57,
        ds_read_u8              = 58,
        ds_read_i16             = 59,
        ds_read_u16             = 60,
        ds_swizzle_b32          = 61,
        ds_permute_b32          = 62,
        ds_bpermute_b32         = 63,
        ds_add_u64              = 64,
        ds_sub_u64              = 65,
        ds_rsub_u64             = 66,
        ds_inc_u64              = 67,
        ds_dec_u64              = 68,
        ds_min_i64              = 69,
        ds_max_i64              = 70,
        ds_min_u64              = 71,
        ds_max_u64              = 72,
        ds_and_b64              = 73,
        ds_or_b64               = 74,
        ds_xor_b64              = 75,
        ds_mskor_b64            = 76,
        ds_write_b64            = 77,
        ds_write2_b64           = 78,
        ds_write2st64_b64       = 79,
        ds_cmpst_b64            = 80,
        ds_cmpst_f64            = 81,
        ds_min_f64              = 82,
        ds_max_f64              = 83,
        ds_add_rtn_u64          = 96,
        ds_sub_rtn_u64          = 97,
        ds_rsub_rtn_u64         = 98,
        ds_inc_rtn_u64          = 99,
        ds_dec_rtn_u64          = 100,
        ds_min_rtn_i64          = 101,
        ds_max_rtn_i64          = 102,
        ds_min_rtn_u64          = 103,
        ds_max_rtn_u64          = 104,
        ds_and_rtn_b64          = 105,
        ds_or_rtn_b64           = 106,
        ds_xor_rtn_b64          = 107,
        ds_mskor_rtn_b64        = 108,
        ds_wrxchg_rtn_b64       = 109,
        ds_wrxchg2_rtn_b64      = 110,
        ds_wrxchg2st64_rtn_b64  = 111,
        ds_cmpst_rtn_b64        = 112,
        ds_cmpst_rtn_f64        = 113,
        ds_min_rtn_f64          = 114,
        ds_max_rtn_f64          = 115,
        ds_read_b64             = 118,
        ds_read2_b64            = 119,
        ds_read2st64_b64        = 120,
        ds_condxchg32_rtn_b64   = 126,
        ds_add_src2_u32         = 128,
        ds_sub_src2_u32         = 129,
        ds_rsub_src2_u32        = 130,
        ds_inc_src2_u32         = 131,
        ds_dec_src2_u32         = 132,
        ds_min_src2_i32         = 133,
        ds_max_src2_i32         = 134,
        ds_min_src2_u32         = 135,
        ds_max_src2_u32         = 136,
        ds_and_src2_b32         = 137,
        ds_or_src2_b32          = 138,
        ds_xor_src2_b32         = 139,
        ds_write_src2_b32       = 141,
        ds_min_src2_f32         = 146,
        ds_max_src2_f32         = 147,
        ds_add_src2_f32         = 149,
        ds_gws_sema_release_all = 152,
        ds_gws_init             = 153,
        ds_gws_sema_v           = 154,
        ds_gws_sema_br          = 155,
        ds_gws_sema_p           = 156,
        ds_gws_barrier          = 157,
        ds_consume              = 189,
        ds_append               = 190,
        ds_ordered_count        = 191,
        ds_add_src2_u64         = 192,
        ds_sub_src2_u64         = 193,
        ds_rsub_src2_u64        = 194,
        ds_inc_src2_u64         = 195,
        ds_dec_src2_u64         = 196,
        ds_min_src2_i64         = 197,
        ds_max_src2_i64         = 198,
        ds_min_src2_u64         = 199,
        ds_max_src2_u64         = 200,
        ds_and_src2_b64         = 201,
        ds_or_src2_b64          = 202,
        ds_xor_src2_b64         = 203,
        ds_write_src2_b64       = 205,
        ds_min_src2_f64         = 210,
        ds_max_src2_f64         = 211,
        ds_write_b96            = 222,
        ds_write_b128           = 223,
        ds_read_b96             = 254,
        ds_read_b128            = 255,
        ds_Ilegal               = 256,
    };

    int GetInstructionClockCount()
    {
        int iRet = 4;

        return iRet;
    }

    VIDSInstruction(OFFSET offset0, OFFSET offset1, GDS isGDS, OP op, ADDR addr, DATA data0, DATA data1, VDST vdst): DSInstruction(offset0, offset1, isGDS, addr, data0, data1, vdst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }

    /// Get the OP [27:23]
    OP GetOp() const { return m_op; }

private:
    /// DS operation.
    OP m_op;
};


// DS instruction for VEGA (GFX9)
class G9DSInstruction : public DSInstruction
{

public:
    /// Selector for the DS Instruction
    enum OP
    {
        ds_add_u32              = 0,
        ds_sub_u32              = 1,
        ds_rsub_u32             = 2,
        ds_inc_u32              = 3,
        ds_dec_u32              = 4,
        ds_min_i32              = 5,
        ds_max_i32              = 6,
        ds_min_u32              = 7,
        ds_max_u32              = 8,
        ds_and_b32              = 9,
        ds_or_b32               = 10,
        ds_xor_b32              = 11,
        ds_mskor_b32            = 12,
        ds_write_b32            = 13,
        ds_write2_b32           = 14,
        ds_write2st64_b32       = 15,
        ds_cmpst_b32            = 16,
        ds_cmpst_f32            = 17,
        ds_min_f32              = 18,
        ds_max_f32              = 19,
        ds_nop                  = 20,
        ds_add_f32              = 21,
        ds_write_b8             = 30,
        ds_write_b16            = 31,
        ds_add_rtn_u32          = 32,
        ds_sub_rtn_u32          = 33,
        ds_rsub_rtn_u32         = 34,
        ds_inc_rtn_u32          = 35,
        ds_dec_rtn_u32          = 36,
        ds_min_rtn_i32          = 37,
        ds_max_rtn_i32          = 38,
        ds_min_rtn_u32          = 39,
        ds_max_rtn_u32          = 40,
        ds_and_rtn_b32          = 41,
        ds_or_rtn_b32           = 42,
        ds_xor_rtn_b32          = 43,
        ds_mskor_rtn_b32        = 44,
        ds_wrxchg_rtn_b32       = 45,
        ds_wrxchg2_rtn_b32      = 46,
        ds_wrxchg2st64_rtn_b32  = 47,
        ds_cmpst_rtn_b32        = 48,
        ds_cmpst_rtn_f32        = 49,
        ds_min_rtn_f32          = 50,
        ds_max_rtn_f32          = 51,
        ds_wrap_rtn_b32         = 52,
        ds_add_rtn_f32          = 53,
        ds_read_b32             = 54,
        ds_read2_b32            = 55,
        ds_read2st64_b32        = 56,
        ds_read_i8              = 57,
        ds_read_u8              = 58,
        ds_read_i16             = 59,
        ds_read_u16             = 60,
        ds_swizzle_b32          = 61,
        ds_permute_b32          = 62,
        ds_bpermute_b32         = 63,
        ds_add_u64              = 64,
        ds_sub_u64              = 65,
        ds_rsub_u64             = 66,
        ds_inc_u64              = 67,
        ds_dec_u64              = 68,
        ds_min_i64              = 69,
        ds_max_i64              = 70,
        ds_min_u64              = 71,
        ds_max_u64              = 72,
        ds_and_b64              = 73,
        ds_or_b64               = 74,
        ds_xor_b64              = 75,
        ds_mskor_b64            = 76,
        ds_write_b64            = 77,
        ds_write2_b64           = 78,
        ds_write2st64_b64       = 79,
        ds_cmpst_b64            = 80,
        ds_cmpst_f64            = 81,
        ds_min_f64              = 82,
        ds_max_f64              = 83,
        ds_write_b8_d16_hi      = 84,
        ds_write_b16_d16_hi     = 85,
        ds_read_u8_d16          = 86,
        ds_read_u8_d16_hi       = 87,
        ds_read_i8_d16          = 88,
        ds_read_i8_d16_hi       = 89,
        ds_read_u16_d16         = 90,
        ds_read_u16_d16_hi      = 91,
        ds_add_rtn_u64          = 96,
        ds_sub_rtn_u64          = 97,
        ds_rsub_rtn_u64         = 98,
        ds_inc_rtn_u64          = 99,
        ds_dec_rtn_u64          = 100,
        ds_min_rtn_i64          = 101,
        ds_max_rtn_i64          = 102,
        ds_min_rtn_u64          = 103,
        ds_max_rtn_u64          = 104,
        ds_and_rtn_b64          = 105,
        ds_or_rtn_b64           = 106,
        ds_xor_rtn_b64          = 107,
        ds_mskor_rtn_b64        = 108,
        ds_wrxchg_rtn_b64       = 109,
        ds_wrxchg2_rtn_b64      = 110,
        ds_wrxchg2st64_rtn_b64  = 111,
        ds_cmpst_rtn_b64        = 112,
        ds_cmpst_rtn_f64        = 113,
        ds_min_rtn_f64          = 114,
        ds_max_rtn_f64          = 115,
        ds_read_b64             = 118,
        ds_read2_b64            = 119,
        ds_read2st64_b64        = 120,
        ds_condxchg32_rtn_b64   = 126,
        ds_add_src2_u32         = 128,
        ds_sub_src2_u32         = 129,
        ds_rsub_src2_u32        = 130,
        ds_inc_src2_u32         = 131,
        ds_dec_src2_u32         = 132,
        ds_min_src2_i32         = 133,
        ds_max_src2_i32         = 134,
        ds_min_src2_u32         = 135,
        ds_max_src2_u32         = 136,
        ds_and_src2_b32         = 137,
        ds_or_src2_b32          = 138,
        ds_xor_src2_b32         = 139,
        ds_write_src2_b32       = 141,
        ds_min_src2_f32         = 146,
        ds_max_src2_f32         = 147,
        ds_add_src2_f32         = 149,
        ds_gws_sema_release_all = 152,
        ds_gws_init             = 153,
        ds_gws_sema_v           = 154,
        ds_gws_sema_br          = 155,
        ds_gws_sema_p           = 156,
        ds_gws_barrier          = 157,
        ds_read_addtid_b32      = 182,
        ds_consume              = 189,
        ds_append               = 190,
        ds_ordered_count        = 191,
        ds_add_src2_u64         = 192,
        ds_sub_src2_u64         = 193,
        ds_rsub_src2_u64        = 194,
        ds_inc_src2_u64         = 195,
        ds_dec_src2_u64         = 196,
        ds_min_src2_i64         = 197,
        ds_max_src2_i64         = 198,
        ds_min_src2_u64         = 199,
        ds_max_src2_u64         = 200,
        ds_and_src2_b64         = 201,
        ds_or_src2_b64          = 202,
        ds_xor_src2_b64         = 203,
        ds_write_src2_b64       = 205,
        ds_min_src2_f64         = 210,
        ds_max_src2_f64         = 211,
        ds_write_b96            = 222,
        ds_write_b128           = 223,
        ds_read_b96             = 254,
        ds_read_b128            = 255,
        ds_Ilegal               = 256,
    };

    G9DSInstruction(OFFSET offset0, OFFSET offset1, GDS isGDS, OP op, ADDR addr, DATA data0, DATA data1, VDST vdst): DSInstruction(offset0, offset1, isGDS, addr, data0, data1, vdst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }

    /// Get the OP [27:23]
    OP GetOp() const { return m_op; }

private:
    /// DS operation.
    OP m_op;
};

#endif //__DSINSTRUCTION_H

