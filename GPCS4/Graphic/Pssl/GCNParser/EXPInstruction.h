//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __EXPINSTRUCTION_H
#define __EXPINSTRUCTION_H

#include "Instruction.h"
/// Export (output) pixel color, pixel depth, vertex position, or vertex parameter data. Two words.
/// Opcode :
///        EN [3:0]
///        TGT [9:4]
///        COMPR [10:10]
///        DONE [11:11]
///        VM [12:12]
///        reserved [25:13]
///        VSRC0 [39:32]
///        VSRC1 [47:40]
///        VSRC2 [55:48]
///        VSRC3 [63:56]

class EXPInstruction : public Instruction
{
public:

#define X(SYM) TGT##SYM
#define X_INIT(SYM,VAL) TGT##SYM = VAL
    /// Export target based on the enumeration below.
    enum TGT
    {
        /// Output to color MRT 0. Increment from here for additional MRTs.
        ///There are EXP_NUM_MRT MRTs in total.
        X_INIT(ExpMRTMin, 0),
        X(ExpMRT),
        X_INIT(ExpMRTMax, 7),
        /// Output to Z.
        X(ExpMRTZ),
        /// Output to NULL.
        X(ExpNULL),
        /// Output to position 0. Increment from here for additional positions.
        /// There are EXP_NUM_POS positions in total.
        X_INIT(ExpPosMin, 12),
        X(ExpPos),
        X_INIT(ExpPosMax, 15),
        /// Output to parameter 0. Increment from here for additional parameters.
        /// There are EXP_NUM_PARAM parameters in total.
        X_INIT(ExpParamMin, 32),
        X(ExpParam),
        X_INIT(ExpParamMax, 63),
        /// All other values are reserved.
        X(ExpReserved)
    };
#undef X_INIT
#undef X

    /// This bitmask determines which VSRC registers export data.
    /// When COMPR is 0: VSRC0 only exports data when en[0] is set to 1; VSRC1 when en[1], VSRC2 when en[2], and VSRC3 when en[3].
    /// When COMPR is 1: VSRC0 contains two 16-bit data and only exports when en[0] is set to 1;
    /// VSRC1 only exports when en[2] is set to 1; en[1] and en[3] are ignored when COMPR is 1.
    typedef char EN;

    /// Boolean. If true, data is exported in float16 format; if false, data is 32 bit.
    typedef char COMPR;

    /// If set, this is the last export of a given type. If this is set for a colour export (PS only),
    /// then the valid mask must be present in the EXEC register.
    typedef char DONE;

    /// Mask contains valid-mask when set; otherwise, mask is just write-mask. Used
    /// only for pixel(mrt) exports.
    typedef char VM;

    /// VGPR of the data source to export.
    typedef char VSRC;

private:
    /// This bitmask determines which VSRC registers export data.
    EN m_en;

    /// Export target based on the enumeration below.
    TGT m_tgt;

    /// Boolean. If true, data is exported in float16 format; if false, data is 32 bit.
    COMPR m_compr;

    /// If set, this is the last export of a given type. If this is set for a colour export (PS only),
    /// then the valid mask must be present in the EXEC register.
    DONE m_done;

    /// Mask contains valid-mask when set; otherwise, mask is just write-mask. Used
    /// only for pixel(mrt) exports.
    VM m_vm;

    /// VGPR of the first/second/third/fourth data to export.
    VSRC m_vsrc[4];

    /// EXP Instruction Width in bits
    static const unsigned int EXPInstructionWidth = 64;
public:
    //
    // Public member functions
    //

    /// ctor
    EXPInstruction(EN en, TGT tgt, COMPR compr, DONE done, VM vm, VSRC vsrc0, VSRC vsrc1, VSRC vsrc2, VSRC vsrc3): 
		Instruction(EXPInstructionWidth, Export, InstructionSet_EXP, Exp),
        m_en(en), m_tgt(tgt), m_compr(compr), m_done(done), m_vm(vm)
    {
        m_vsrc[0] = vsrc0;
        m_vsrc[1] = vsrc1;
        m_vsrc[2] = vsrc2;
        m_vsrc[3] = vsrc3;
    }

    /// dtor
    ~EXPInstruction() {}

    /// Get the EN [3:0]
    EN GetEn() const { return m_en; }

    /// Get the TGT [9:4]
    TGT GetTGT() const { return m_tgt; }

    /// Get the COMPR [10:10]
    COMPR GetCOMPR() const { return m_compr; }

    /// Get the DONE [11:11]
    DONE GetDone() const { return m_done; }

    /// Get the VM [12:12]
    VM GetVM() const { return m_vm; }

    /// Get the VSRC
    /// \param[in]  the index of the VSRC data to export.
    VSRC GetVSRC(const unsigned int vsrcIdx) const { return m_vsrc[vsrcIdx];}
};
#endif //__EXPINSTRUCTION_H

