//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __VINTRPINSTRUCTION_H
#define __VINTRPINSTRUCTION_H

#include "Instruction.h"
/// Interpolate data for the pixel shader.
/// Opcode :
///        VSRC [7:0]
///        ATTRCHAN [9:8]
///        ATTR [15:10]
///        OP [17:16]
///        VDST [25:18]

class VINTRPInstruction : public Instruction
{
public:


    typedef char VSRC;
    typedef char ATTRCHAN;
    typedef char ATTR;
    typedef char VDST;

private:
    /// Vector General-Purpose Registers (VGPR) containing the i/j coordinate by which
    ///to multiply one of the parameter components.
    VSRC m_vsrc;

    /// Attribute component to interpolate.
    ATTRCHAN m_attrchan;

    /// Attribute to interpolate.
    ATTR m_attr;

    /// Vector General-Purpose Registers VGPR [255:0] to which results are written, and,
    ///optionally, from which they are read when accumulating results.
    VDST m_vdst;

    /// VINTRP Instruction Width in bits
    static const unsigned int VSRCInstructionWidth = 32;
public:
    //
    // Public member functions
    //

    /// ctor
    VINTRPInstruction(VSRC vsrc, ATTRCHAN attrchan, ATTR attr, VDST vdst,
		InstructionClass insClass = VectorInterpFpCache):
		Instruction(VSRCInstructionWidth, VectorInterpolation, InstructionSet_VINTRP, insClass),
        m_vsrc(vsrc), m_attrchan(attrchan), m_attr(attr), m_vdst(vdst) {}

    /// dtor
    ~VINTRPInstruction() {}

    /// Get the VSRC [7:0]
    VSRC GetVSRC() const { return m_vsrc; }

    /// Get the ATTRCHAN [9:8]
    ATTRCHAN GetATTRCHAN() const { return m_attrchan; }

    /// Get the ATTR [15:10]
    ATTR GetATTR() const { return m_attr; }

    /// Get the VDST
    VDST GetVDST() const { return m_vdst; }
};

class SIVINTRPInstruction : public VINTRPInstruction
{
public:
    /// Selector for the VINTRP Instruction
    enum OP
    {
        ///  D = P10 * S + P0; parameter interpolation.
        V_INTERP_P1_F32,
        ///  D = P20 * S + D; parameter interpolation.
        V_INTERP_P2_F32,
        ///  D = {P10,P20,P0}[S]; parameter load.
        V_INTERP_MOV_F32,
        /// Reserved
        V_INTERP_RESERVED
    };

    /// Get the OP [17:16]
    OP GetOp() const { return m_op; }

    /// ctor
    SIVINTRPInstruction(VSRC vsrc, ATTRCHAN attrchan, ATTR attr, OP op, VDST vdst):
		VINTRPInstruction(vsrc, attrchan, attr, vdst, VectorInterpFpCache), m_op(op) {}


private:
    /// VINTRP operation.
    OP m_op;
};

class VIVINTRPInstruction : public VINTRPInstruction
{
public:
    /// Selector for the VINTRP Instruction
    enum OP
    {
        ///  D = P10 * S + P0; parameter interpolation.
        V_INTERP_P1_F32,
        ///  D = P20 * S + D; parameter interpolation.
        V_INTERP_P2_F32,
        ///  D = {P10,P20,P0}[S]; parameter load.
        V_INTERP_MOV_F32,
        /// Illegal
        v_ILLEGAL,
    };

    /// Get the OP [17:16]
    OP GetOp() const { return m_op; }

    /// ctor
    VIVINTRPInstruction(VSRC vsrc, ATTRCHAN attrchan, ATTR attr, OP op, VDST vdst): VINTRPInstruction(vsrc, attrchan, attr, vdst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }


private:
    /// VINTRP operation.
    OP m_op;
};

#endif //__VINTRPINSTRUCTION_H

