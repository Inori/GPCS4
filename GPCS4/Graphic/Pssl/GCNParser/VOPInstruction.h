//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __VOPINSTRUCTION_H
#define __VOPINSTRUCTION_H



#include "Instruction.h"


// Note:
// Implementation is not complete
// VI and G9 generation VOP is not fully supported.


/// -----------------------------------------------------------------------------------------------
/// \class Name: VOPInstruction : public Instruction
/// \brief Description:  This is vector ALU general Instruction ,which represents the following instruction :
//                       VOP2,VOP1,VOPC,VOP3 (3 input, one output),VOP3 (3 input, two output)
/// -----------------------------------------------------------------------------------------------
class VOPInstruction : public Instruction
{
public:

#define X(SYM) SRC##SYM
#define X_INIT(SYM,VAL) SRC##SYM = VAL
	/// Destination for scalar memory read instruction
	enum SRC : unsigned short
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


	typedef unsigned char VSRC;

	typedef unsigned char VDST;

	typedef unsigned char OMOD;

	typedef unsigned char NEG;

	typedef unsigned char ABS;

	typedef unsigned char CLMP;


    /// Selector for the Instruction Type
    enum Encoding
    {
        Encoding_VOP1  = 0x0000003F, // [31:25] enum(7) Must be 0 1 1 1 1 1 1.
        Encoding_VOP2  = 0x00000000, // [31]    enum(1) Must be 0.
        Encoding_VOP3  = 0x00000034, // [31:26] enum(6) Must be 1 1 0 1 0 0. (VOP3a)
        Encoding_VOPC  = 0x0000003E, // [31:25] enum(7) 0 1 1 1 1 1 0. // Single Vector Compare Operations
        Encoding_VOP3P = 0x000001A7, // [31:23] Must be  1 1 0 1 0 0 1 1 1.
        Encoding_Illegal
    };

    /// VOP Instruction`s encodeing masks
    enum VOPMask
    {
        VOPMask_VOP1  = 0x0000003F << 25, // [31:25] enum(7) Must be 0 1 1 1 1 1 1.
        VOPMask_VOP2  = 0x00000000 << 31, // [31]    enum(1) Must be 0.
        VOPMask_VOP3  = 0x00000034 << 26, // [31:26] enum(6) Must be 1 1 0 1 0 0.
        VOPMask_VOPC  = 0x0000003E << 25, // [31:25] enum(7) 0 1 1 1 1 1 0.
        VOPMask_VOP3P = 0x000001A7 << 23,
        VOP_UNDEFINE,
    };

    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        VOPInstruction
    /// \brief Description: ctor
    /// \param[in]          instructionWidth
    /// \param[in]          instructionType
    /// \return
    /// -----------------------------------------------------------------------------------------------
    VOPInstruction(SRC src0, VDST vdst, 
		unsigned int ridx0, unsigned int vdstRidx,
		unsigned int instructionWidth, 
		InstructionSet instructionEncoding, 
		InstructionClass insClass = InstructionClassUnknown, 
		OperandType operandType = TypeNone) : 
		Instruction(instructionWidth, VectorALU, instructionEncoding, insClass, operandType),
	m_src0(src0), m_vdst(vdst), m_ridx0(ridx0), m_vdstRidx(vdstRidx) { }
    /// -----------------------------------------------------------------------------------------------
    /// \brief Name:        ~VOPInstruction
    /// \brief Description: dtor
    /// \return
    /// -----------------------------------------------------------------------------------------------
    ~VOPInstruction() { }

	SRC GetSRC0() const { return m_src0; }

	VDST GetVDST() const { return m_vdst; }

	/// Get the (scalar) register`s index.
	/// Note : Relevant only if m_ssrc0 == ScalarGPR or m_ssrc0 == ScalarTtmp
	unsigned int GetSRidx0() const { return m_ridx0; }

	/// Get the (vector) register`s index.
	unsigned int GetVDSTRidx() const { return m_vdstRidx; }

protected:

	/// Source 0. First operand for the instruction.
	SRC m_src0;
	/// Vector destination for instruction..
	/// Note : For VOPC encoding, vdst means vsrc1
	VDST m_vdst;

	unsigned int m_ridx0;
	/// Registers index (m_sdst).
	/// Note : Relevant only if m_sdst == ScalarGPR or m_sdst == ScalarTtmp
	unsigned int m_vdstRidx;

};

class SIVOP1Instruction : public VOPInstruction
{
public:

    enum VOP1_OP
    {
        V_NOP               =   0,  //  do nothing.
        V_MOV_B32           =   1,  //  D.u = S0.u.
        V_READFIRSTLANE_B32 =   2,     //  copy one VGPR value to one SGPR. Dst = SGPRdest, Src0 = Source Data (VGPR# or M0(lds-direct)), Lane# = FindFirst1fromLSB(exec) (lane = 0 if exec is zero). Ignores exec mask.
        V_CVT_I32_F64       =   3,  //  D.i = (int)S0.d.
        V_CVT_F64_I32       =   4,  //  D.f = (float)S0.i.
        V_CVT_F32_I32       =   5,  //  D.f = (float)S0.i.
        V_CVT_F32_U32       =   6,  //  D.f = (float)S0.u.
        V_CVT_U32_F32       =   7,  //  D.u = (unsigned)S0.f.
        V_CVT_I32_F32       =   8,  //  D.i = (int)S0.f.
        V_MOV_FED_B32       =   9,  //  D.u = S0.u, introduce edc double error upon write to dest vgpr without causing an exception.
        V_CVT_F16_F32       =   10,    //  D.f16 = flt32_to_flt16(S0.f).
        V_CVT_F32_F16       =   11,    //  D.f = flt16_to_flt32(S0.f16).
        V_CVT_RPI_I32_F32   =   12, //  D.i = (int)floor(S0.f + 0.5).
        V_CVT_FLR_I32_F32   =   13, //  D.i = (int)floor(S0.f).
        V_CVT_OFF_F32_I4    =   14, //  4-bit signed int to 32-bit float. For interpolation in shader.
        V_CVT_F32_F64       =   15, //  D.f = (float)S0.d.
        V_CVT_F64_F32       =   16, //  D.d = (double)S0.f.
        V_CVT_F32_UBYTE0    =   17, //  D.f = UINT2FLT(S0.u[7           // 0]).
        V_CVT_F32_UBYTE1    =   18, //  D.f = UINT2FLT(S0.u[15           // 8]).
        V_CVT_F32_UBYTE2    =   19, //  D.f = UINT2FLT(S0.u[23           // 16]).
        V_CVT_F32_UBYTE3    =   20, //  D.f = UINT2FLT(S0.u[31           // 24]).
        V_CVT_U32_F64       =   21, //  D.u = (uint)S0.d.
        V_CVT_F64_U32       =   22, //  D.d = (double)S0.u.
        // 23 � 31 reserved.
        V_FRACT_F32     =   32, //  D.f = S0.f - floor(S0.f).
        V_TRUNC_F32     =   33, //  D.f = trunc(S0.f), return integer part of S0.
        V_CEIL_F32      =   34, //  D.f = ceil(S0.f). Implemented as           //  D.f = trunc(S0.f); if (S0 > 0.0 && S0 != D), D += 1.0.
        V_RNDNE_F32     =   35, //  D.f = round_nearest_even(S0.f).
        V_FLOOR_F32     =   36, //  D.f = trunc(S0); if ((S0 < 0.0) && (S0 != D)) D += -1.0.
        V_EXP_F32       =   37, //  D.f = pow(2.0, S0.f).
        V_LOG_CLAMP_F32 =   38, //  D.f = log2(S0.f), clamp -infinity to -max_float.
        V_LOG_F32       =   39, //  D.f = log2(S0.f).
        V_RCP_CLAMP_F32 =   40, //  D.f = 1.0 / S0.f, result clamped to +-max_float.
        V_RCP_LEGACY_F32 =   41, //  D.f = 1.0 / S0.f, +-infinity result clamped to +-0.0.
        V_RCP_F32       =   42, //  D.f = 1.0 / S0.f.
        V_RCP_IFLAG_F32 =   43, //  D.f = 1.0 / S0.f, only integer div_by_zero flag can be raised.
        V_RSQ_CLAMP_F32 =   44, //  D.f = 1.0 / sqrt(S0.f), result clamped to +-max_float.
        V_RSQ_LEGACY_F32 =   45, //  D.f = 1.0 / sqrt(S0.f).
        V_RSQ_F32       =   46, //  D.f = 1.0 / sqrt(S0.f).
        V_RCP_F64       =   47, //  D.d = 1.0 / (S0.d).
        V_RCP_CLAMP_F64 =   48, //  D.f = 1.0 / (S0.f), result clamped to +-max_float.
        V_RSQ_F64       =   49, //  D.f = 1.0 / sqrt(S0.f).
        V_RSQ_CLAMP_F64 =   50, //  D.d = 1.0 / sqrt(S0.d), result clamped to +-max_float.
        V_SQRT_F32      =   51, //  D.f = sqrt(S0.f).
        V_SQRT_F64      =   52, //  D.d = sqrt(S0.d).
        V_SIN_F32       =   53, //  D.f = sin(S0.f).
        V_COS_F32       =   54, //  D.f = cos(S0.f).
        V_NOT_B32       =   55, //  D.u = ~S0.u.
        V_BFREV_B32     =   56, //  D.u[31           // 0] = S0.u[0           // 31], bitfield reverse.
        V_FFBH_U32      =   57, //  D.u = position of first 1 in S0 from MSB; D=0xFFFFFFFF if S0==0.
        V_FFBL_B32      =   58, //  D.u = position of first 1 in S0 from LSB; D=0xFFFFFFFF if S0==0.
        V_FFBH_I32      =   59, //  D.u = position of first bit different from sign bit in S0 from MSB; D=0xFFFFFFFF if S0==0 or 0xFFFFFFFF.
        V_FREXP_EXP_I32_F64 = 60, //  See V_FREXP_EXP_I32_F32.
        V_FREXP_MANT_F64    = 61,   //  See V_FREXP_MANT_F32.
        V_FRACT_F64         = 62,   //  S0.d - floor(S0.d).
        V_FREXP_EXP_I32_F32 = 63,   //  If (S0.f == INF || S0.f == NAN), then D.i = 0; else D.i = TwosComplement(Exponent(S0.f) - 127 + 1). Returns exponent of single precision float input, such that S0.f = significand * (2 ** exponent). See also FREXP_MANT_F32, which returns the significand.
        V_FREXP_MANT_F32    = 64,   //  if (S0.f == INF || S0.f == NAN) then D.f = S0.f; else D.f = Mantissa(S0.f). Result range is in (-1.0,-0.5][0.5,1.0) in normal cases. Returns binary significand of single precision float input, such that S0.f = significand * (2 ** exponent). See also FREXP_EXP_I32_F32, which returns integer exponent.
        V_CLREXCP           = 65,   //  Clear wave's exception state in SIMD(SP).
        V_MOVRELD_B32       = 66,   //  VGPR[D.u + M0.u] = VGPR[S0.u].
        V_MOVRELS_B32       = 67,   //  VGPR[D.u] = VGPR[S0.u + M0.u].
        V_MOVRELSD_B32      = 68,   //  VGPR[D.u + M0.u] = VGPR[S0.u + M0.u].
        VOP1_UNDEFINE       = 999,
    };


    /// -----------------------------------------------------------------------------------------------
    SIVOP1Instruction(SRC src0, VDST vdst, 
		unsigned int ridx0, unsigned int vdstRidx,
		unsigned int instructionWidth, Encoding instructionEncoding, VOP1_OP op,
		InstructionClass insClass = InstructionClassUnknown,
		OperandType operandType = TypeNone) :
		VOPInstruction(src0, vdst, ridx0, vdstRidx, instructionWidth, InstructionSet_VOP1, insClass, operandType),
		m_op(op), m_instructionEncoding(instructionEncoding) { }
    /// -----------------------------------------------------------------------------------------------

    ~SIVOP1Instruction() {}

    /// Get the OP
    VOP1_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

private:

    /// VOP operation.
    VOP1_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;

    int GetInstructionClockCount()
    {
        int iRet = 4;

        switch (m_op)
        {
            case    V_NOP:
            case    V_MOV_B32:
            case    V_READFIRSTLANE_B32: // guess
            case    V_CVT_F32_I32:
            case    V_CVT_F32_U32:
            case    V_CVT_U32_F32:
            case    V_CVT_I32_F32:
            case    V_MOV_FED_B32: // guess
            case    V_CVT_F16_F32:
            case    V_CVT_F32_F16:
            case    V_CVT_RPI_I32_F32:
            case    V_CVT_FLR_I32_F32:
            case    V_CVT_OFF_F32_I4:
            case    V_CVT_F32_UBYTE0:
            case    V_CVT_F32_UBYTE1:
            case    V_CVT_F32_UBYTE2:
            case    V_CVT_F32_UBYTE3:
            case    V_FRACT_F32:
            case    V_TRUNC_F32:
            case    V_CEIL_F32:
            case    V_RNDNE_F32:
            case    V_FLOOR_F32:
            case    V_RCP_IFLAG_F32:// guess
            case    V_SQRT_F32:// guess
            case    V_NOT_B32:
            case    V_BFREV_B32:
            case    V_FFBH_U32:
            case    V_FFBL_B32:
            case    V_FFBH_I32:
            case    V_FREXP_EXP_I32_F32:
            case    V_FREXP_MANT_F32:
            case    V_CLREXCP:
            case    V_MOVRELD_B32: // guess
            case    V_MOVRELS_B32: // guess
            case    V_MOVRELSD_B32:// guess

                iRet = 4;
                break;

            case    V_CVT_I32_F64:
            case    V_CVT_F64_I32:
            case    V_CVT_F32_F64:
            case    V_CVT_F64_F32:
            case    V_CVT_U32_F64:
            case    V_CVT_F64_U32:
            case    V_FRACT_F64:

                iRet = 8;
                break;

            case    V_EXP_F32:
            case    V_LOG_CLAMP_F32:
            case    V_LOG_F32:
            case    V_RCP_CLAMP_F32:
            case    V_RCP_LEGACY_F32:
            case    V_RCP_F32:
            case    V_RSQ_CLAMP_F32:
            case    V_RSQ_LEGACY_F32:
            case    V_RSQ_F32:
            case    V_RCP_F64:
            case    V_RCP_CLAMP_F64:
            case    V_RSQ_F64:
            case    V_RSQ_CLAMP_F64:
            case    V_SQRT_F64:
            case    V_SIN_F32:
            case    V_COS_F32:
            case    V_FREXP_EXP_I32_F64:
            case    V_FREXP_MANT_F64:
                iRet = 16;
                break;

            default:
                break;
        }

        return iRet;
    }
};

class SIVOP2Instruction : public VOPInstruction
{
public:
    enum VOP2_OP
    {
        V_CNDMASK_B32      =     0,     //  D.u = VCC[i] ? S1.u : S0.u (i = threadID in wave).
        V_READLANE_B32     =     1,      //  copy one VGPR value to one SGPR. Dst = SGPR-dest, Src0 = Source Data (VGPR# or M0(lds-direct)), Src1 = Lane Select (SGPRor M0). Ignores exec mask.
        V_WRITELANE_B32    =     2,       //  Write value into one VGPR one one lane. Dst = VGPRdest,       Src0 = Source Data (sgpr, m0, exec or constants), Src1 = Lane Select (SGPR or M0). Ignores exec mask.
        V_ADD_F32           =   3,  //  D.f = S0.f + S1.f.
        V_SUB_F32           =   4,  //  D.f = S0.f - S1.f.
        V_SUBREV_F32       =     5,    //  D.f = S1.f - S0.f.
        V_MAC_LEGACY_F32   =     6,        //  D.f = S0.F * S1.f + D.f.
        V_MUL_LEGACY_F32   =     7,        //  D.f = S0.f * S1.f (DX9 rules, 0.0*x = 0.0).
        V_MUL_F32           =   8,  //  D.f = S0.f * S1.f.
        V_MUL_I32_I24      =     9,     //  D.i = S0.i[23              // 0] * S1.i[23:0].
        V_MUL_HI_I32_I24   =     10,       //  D.i = (S0.i[23:0] * S1.i[23:0])>>32.
        V_MUL_U32_U24      =     11,    //  D.u = S0.u[23:0] * S1.u[23:0].
        V_MUL_HI_U32_U24   =     12,       //  D.i = (S0.u[23:0] * S1.u[23:0])>>32.
        V_MIN_LEGACY_F32   =     13,       //  D.f = min(S0.f, S1.f) (DX9 rules for NaN).
        V_MAX_LEGACY_F32   =     14,       //  D.f = max(S0.f, S1.f) (DX9 rules for NaN).
        V_MIN_F32           =   15, //  D.f = min(S0.f, S1.f).
        V_MAX_F32           =   16, //  D.f = max(S0.f, S1.f).
        V_MIN_I32           =   17, //  D.i = min(S0.i, S1.i).
        V_MAX_I32           =   18, //  D.i = max(S0.i, S1.i).
        V_MIN_U32           =   19, //  D.u = min(S0.u, S1.u).
        V_MAX_U32           =   20, //  D.u = max(S0.u, S1.u).
        V_LSHR_B32          =   21, //  D.u = S0.u >> S1.u[4:0].
        V_LSHRREV_B32      =     22,    //  D.u = S1.u >> S0.u[4:0].
        V_ASHR_I32          =   23, //  D.i = S0.i >> S1.i[4:0].
        V_ASHRREV_I32      =     24,    //  D.i = S1.i >> S0.i[4:0].
        V_LSHL_B32          =   25, //  D.u = S0.u << S1.u[4:0].
        V_LSHLREV_B32      =     26,   //  D.u = S1.u << S0.u[4:0].
        V_AND_B32           =   27, //  D.u = S0.u & S1.u.
        V_OR_B32                =   28, //  D.u = S0.u | S1.u.
        V_XOR_B32           =   29, //  D.u = S0.u ^ S1.u.
        V_BFM_B32           =   30, //  D.u = ((1<<S0.u[4:0])-1) << S1.u[4:0]; S0=bitfield_width, S1=bitfield_offset.
        V_MAC_F32           =   31, //  D.f = S0.f * S1.f + D.f.
        V_MADMK_F32          =   32, //  D.f = S0.f * K + S1.f; K is a 32-bit inline constant.
        V_MADAK_F32          =   33,  //  D.f = S0.f * S1.f + K; K is a 32-bit inline constant.
        V_BCNT_U32_B32       =   34,     //  D.u = CountOneBits(S0.u) + S1.u. Bit count.
        V_MBCNT_LO_U32_B32   =   35,         //  ThreadMask = (1 << ThreadPosition) - 1; D.u = CountOneBits(S0.u & ThreadMask[31:0]) + S1.u. Masked bit count, ThreadPosition is the position of this thread in the wavefront (in 0 63).
        V_MBCNT_HI_U32_B32   =   36,         //  ThreadMask = (1 << ThreadPosition) - 1; D.u = CountOneBits(S0.u & ThreadMask[63:32]) + S1.u. Masked bit count, ThreadPosition is the position of this thread in the wavefront (in 0..63).
        V_ADD_I32           =   37, //  D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V_SUB_I32           =   38, //  D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V_SUBREV_I32         =   39,   //  D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V_ADDC_U32          =   40, //  D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carryout, S2.u=carry-in).
        V_SUBB_U32          =   41, //  D.u = S0.u - S1.u - VCC; VCC=carry-out (VOP3:sgpr=carry-out, S2.u=carry-in).
        V_SUBBREV_U32        =   42,    //  D.u = S1.u - S0.u - VCC; VCC=carry-out (VOP3:sgpr=carryout, S2.u=carry-in).
        V_LDEXP_F32          =   43,  //  D.d = pow(S0.f, S1.i).
        V_CVT_PKACCUM_U8_F32    =   44, // f32->u8(s0.f), pack into byte(s1.u), of dst.
        V_CVT_PKNORM_I16_F32 =   45,           //  D = {(snorm)S1.f, (snorm)S0.f}.
        V_CVT_PKNORM_U16_F32 =   46,           //  D = {(unorm)S1.f, (unorm)S0.f}.
        V_CVT_PKRTZ_F16_F32  =   47,          //  D = {flt32_to_flt16(S1.f),flt32_to_flt16(S0.f)}, with round-toward-zero.
        V_CVT_PK_U16_U32     =   48,       //  D = {(u32->u16)S1.u, (u32->u16)S0.u}.
        V_CVT_PK_I16_I32     =   49,       //  D = {(i32->i16)S1.i, (i32->i16)S0.i}.
        VOP2_UNDEFINE       = 999,
    };


    /// -----------------------------------------------------------------------------------------------
    SIVOP2Instruction(SRC src0, VSRC vsrc1, VDST vdst, 
		unsigned int ridx0, unsigned int vridx1, unsigned int vdstRidx,
		unsigned int instructionWidth, 
		Encoding instructionEncoding, 
		VOP2_OP op,	  
		InstructionClass insClass, 
		OperandType operandType = TypeNone) :
		VOPInstruction(
			src0, vdst, ridx0, vdstRidx, 
			instructionWidth, 
			InstructionSet_VOP2, 
			insClass, 
			operandType),
		m_op(op), m_vsrc1(vsrc1), m_vridx1(vridx1), m_instructionEncoding(instructionEncoding) { }
    /// -----------------------------------------------------------------------------------------------

    ~SIVOP2Instruction() {}

    /// Get the OP
    VOP2_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

    int GetInstructionClockCount()
    {
        int iRet = 4;
        return iRet;
    }

	VSRC GetVSRC1() const { return m_vsrc1; }

	unsigned int GetVRidx1() const { return m_vridx1; }


private:

    /// VOP operation.
    VOP2_OP m_op;

	VSRC m_vsrc1;

	unsigned int m_vridx1;

    //Instruction Type
    Encoding m_instructionEncoding;

};

class SIVOP3Instruction : public VOPInstruction
{
public:

    enum VOP3_OP
    {
		// Signal on sNaN input only.
		V3_CMP_F_F32 = 0,
		V3_CMP_LT_F32 = 1,
		V3_CMP_EQ_F32 = 2,
		V3_CMP_LE_F32 = 3,
		V3_CMP_GT_F32 = 4,
		V3_CMP_LG_F32 = 5,
		V3_CMP_GE_F32 = 6,
		V3_CMP_O_F32 = 7,
		V3_CMP_U_F32 = 8,
		V3_CMP_NGE_F32 = 9,
		V3_CMP_NLG_F32 = 10,
		V3_CMP_NGT_F32 = 11,
		V3_CMP_NLE_F32 = 12,
		V3_CMP_NEQ_F32 = 13,
		V3_CMP_NLT_F32 = 14,
		V3_CMP_TRU_F32 = 15,

		// Signal on sNaN input only. Also write EXEC.
		V3_CMPX_F_F32 = 16,
		V3_CMPX_LT_F32 = 17,
		V3_CMPX_EQ_F32 = 18,
		V3_CMPX_LE_F32 = 19,
		V3_CMPX_GT_F32 = 20,
		V3_CMPX_LG_F32 = 21,
		V3_CMPX_GE_F32 = 22,
		V3_CMPX_O_F32 = 23,
		V3_CMPX_U_F32 = 24,
		V3_CMPX_NGE_F32 = 25,
		V3_CMPX_NLG_F32 = 26,
		V3_CMPX_NGT_F32 = 27,
		V3_CMPX_NLE_F32 = 28,
		V3_CMPX_NEQ_F32 = 29,
		V3_CMPX_NLT_F32 = 30,
		V3_CMPX_TRU_F32 = 31,

		// Signal on sNaN input only.
		V3_CMP_F_F64 = 32,
		V3_CMP_LT_F64 = 33,
		V3_CMP_EQ_F64 = 34,
		V3_CMP_LE_F64 = 35,
		V3_CMP_GT_F64 = 36,
		V3_CMP_LG_F64 = 37,
		V3_CMP_GE_F64 = 38,
		V3_CMP_O_F64 = 39,
		V3_CMP_U_F64 = 40,
		V3_CMP_NGE_F64 = 41,
		V3_CMP_NLG_F64 = 42,
		V3_CMP_NGT_F64 = 43,
		V3_CMP_NLE_F64 = 44,
		V3_CMP_NEQ_F64 = 45,
		V3_CMP_NLT_F64 = 46,
		V3_CMP_TRU_F64 = 47,

		// Signal on sNaN input only. Also write EXEC.
		V3_CMPX_F_F64 = 48,
		V3_CMPX_LT_F64 = 49,
		V3_CMPX_EQ_F64 = 50,
		V3_CMPX_LE_F64 = 51,
		V3_CMPX_GT_F64 = 52,
		V3_CMPX_LG_F64 = 53,
		V3_CMPX_GE_F64 = 54,
		V3_CMPX_O_F64 = 55,
		V3_CMPX_U_F64 = 56,
		V3_CMPX_NGE_F64 = 57,
		V3_CMPX_NLG_F64 = 58,
		V3_CMPX_NGT_F64 = 59,
		V3_CMPX_NLE_F64 = 60,
		V3_CMPX_NEQ_F64 = 61,
		V3_CMPX_NLT_F64 = 62,
		V3_CMPX_TRU_F64 = 63,

		// Signal on any NaN.
		V3_CMPS_F_F32 = 64,
		V3_CMPS_LT_F32 = 65,
		V3_CMPS_EQ_F32 = 66,
		V3_CMPS_LE_F32 = 67,
		V3_CMPS_GT_F32 = 68,
		V3_CMPS_LG_F32 = 69,
		V3_CMPS_GE_F32 = 70,
		V3_CMPS_O_F32 = 71,
		V3_CMPS_U_F32 = 72,
		V3_CMPS_NGE_F32 = 73,
		V3_CMPS_NLG_F32 = 74,
		V3_CMPS_NGT_F32 = 75,
		V3_CMPS_NLE_F32 = 76,
		V3_CMPS_NEQ_F32 = 77,
		V3_CMPS_NLT_F32 = 78,
		V3_CMPS_TRU_F32 = 79,

		//Signal on any NaN. Also write EXEC.
		V3_CMPSX_F_F32 = 80,
		V3_CMPSX_LT_F32 = 81,
		V3_CMPSX_EQ_F32 = 82,
		V3_CMPSX_LE_F32 = 83,
		V3_CMPSX_GT_F32 = 84,
		V3_CMPSX_LG_F32 = 85,
		V3_CMPSX_GE_F32 = 86,
		V3_CMPSX_O_F32 = 87,
		V3_CMPSX_U_F32 = 88,
		V3_CMPSX_NGE_F32 = 89,
		V3_CMPSX_NLG_F32 = 90,
		V3_CMPSX_NGT_F32 = 91,
		V3_CMPSX_NLE_F32 = 92,
		V3_CMPSX_NEQ_F32 = 93,
		V3_CMPSX_NLT_F32 = 94,
		V3_CMPSX_TRU_F32 = 95,

		//ignal on any NaN.
		V3_CMPS_F_F64 = 96,
		V3_CMPS_LT_F64 = 97,
		V3_CMPS_EQ_F64 = 98,
		V3_CMPS_LE_F64 = 99,
		V3_CMPS_GT_F64 = 100,
		V3_CMPS_LG_F64 = 101,
		V3_CMPS_GE_F64 = 102,
		V3_CMPS_O_F64 = 103,
		V3_CMPS_U_F64 = 104,
		V3_CMPS_NGE_F64 = 105,
		V3_CMPS_NLG_F64 = 106,
		V3_CMPS_NGT_F64 = 107,
		V3_CMPS_NLE_F64 = 108,
		V3_CMPS_NEQ_F64 = 109,
		V3_CMPS_NLT_F64 = 110,
		V3_CMPS_TRU_F64 = 111,

		//Signal on any NaN. Also write EXEC.
		V3_CMPSX_F_F64 = 112,
		V3_CMPSX_LT_F64 = 113,
		V3_CMPSX_EQ_F64 = 114,
		V3_CMPSX_LE_F64 = 115,
		V3_CMPSX_GT_F64 = 116,
		V3_CMPSX_LG_F64 = 117,
		V3_CMPSX_GE_F64 = 118,
		V3_CMPSX_O_F64 = 119,
		V3_CMPSX_U_F64 = 120,
		V3_CMPSX_NGE_F64 = 121,
		V3_CMPSX_NLG_F64 = 122,
		V3_CMPSX_NGT_F64 = 123,
		V3_CMPSX_NLE_F64 = 124,
		V3_CMPSX_NEQ_F64 = 125,
		V3_CMPSX_NLT_F64 = 126,
		V3_CMPSX_TRU_F64 = 127,

		// On 32-bit integers.
		V3_CMP_F_I32 = 128,
		V3_CMP_LT_I32 = 129,
		V3_CMP_EQ_I32 = 130,
		V3_CMP_LE_I32 = 131,
		V3_CMP_GT_I32 = 132,
		V3_CMP_NE_I32 = 133,
		V3_CMP_GE_I32 = 134,
		V3_CMP_T_I32 = 135,
		V3_CMP_CLASS_F32 = 136,  // D = IEEE numeric class function specified in S1.u, performed on S0.f.
		V3_CMP_LT_I16 = 137,
		V3_CMP_EQ_I16 = 138,
		V3_CMP_LE_I16 = 139,
		V3_CMP_GT_I16 = 140,
		V3_CMP_NE_I16 = 141,
		V3_CMP_GE_I16 = 142,
		V3_CMP_CLASS_F16 = 143,

		// Also write EXEC.
		V3_CMPX_F_I32 = 144,
		V3_CMPX_LT_I32 = 145,
		V3_CMPX_EQ_I32 = 146,
		V3_CMPX_LE_I32 = 147,
		V3_CMPX_GT_I32 = 148,
		V3_CMPX_NE_I32 = 149,
		V3_CMPX_GE_I32 = 150,
		V3_CMPX_T_I32 = 151,
		V3_CMPX_CLASS_F32 = 152,  // D = IEEE numeric class function specified in S1.u, performed on S0.f. Also write EXEC.
		V3_CMPX_LT_I16 = 153,
		V3_CMPX_EQ_I16 = 154,
		V3_CMPX_LE_I16 = 155,
		V3_CMPX_GT_I16 = 156,
		V3_CMPX_NE_I16 = 157,
		V3_CMPX_GE_I16 = 158,
		V3_CMPX_CLASS_F16 = 159,

		// On 64-bit integers.
		V3_CMP_F_I64 = 160,
		V3_CMP_LT_I64 = 161,
		V3_CMP_EQ_I64 = 162,
		V3_CMP_LE_I64 = 163,
		V3_CMP_GT_I64 = 164,
		V3_CMP_NE_I64 = 165,
		V3_CMP_GE_I64 = 166,
		V3_CMP_T_I64 = 167,
		V3_CMP_CLASS_F64 = 168,  // D = IEEE numeric class function specified in S1.u, performed on S0.d.
		V3_CMP_LT_U16 = 169,
		V3_CMP_EQ_U16 = 170,
		V3_CMP_LE_U16 = 171,
		V3_CMP_GT_U16 = 172,
		V3_CMP_NE_U16 = 173,
		V3_CMP_GE_U16 = 174,

		// Also write EXEC.
		V3_CMPX_F_I64 = 176,
		V3_CMPX_LT_I64 = 177,
		V3_CMPX_EQ_I64 = 178,
		V3_CMPX_LE_I64 = 179,
		V3_CMPX_GT_I64 = 180,
		V3_CMPX_NE_I64 = 181,
		V3_CMPX_GE_I64 = 182,
		V3_CMPX_T_I64 = 183,
		V3_CMPX_CLASS_F64 = 184,  // D = IEEE numeric class function specified in S1.u, performed on S0.d. Also write EXEC.
		V3_CMPX_LT_U16 = 185,
		V3_CMPX_EQ_U16 = 186,
		V3_CMPX_LE_U16 = 187,
		V3_CMPX_GT_U16 = 188,
		V3_CMPX_NE_U16 = 189,
		V3_CMPX_GE_U16 = 190,

		// On unsigned 32-bit intergers.
		V3_CMP_F_U32 = 192,
		V3_CMP_LT_U32 = 193,
		V3_CMP_EQ_U32 = 194,
		V3_CMP_LE_U32 = 195,
		V3_CMP_GT_U32 = 196,
		V3_CMP_NE_U32 = 197,
		V3_CMP_GE_U32 = 198,
		V3_CMP_T_U32 = 199,
		V3_CMP_F_F16 = 200,
		V3_CMP_LT_F16 = 201,
		V3_CMP_EQ_F16 = 202,
		V3_CMP_LE_F16 = 203,
		V3_CMP_GT_F16 = 204,
		V3_CMP_LG_F16 = 205,
		V3_CMP_GE_F16 = 206,
		V3_CMP_O_F16 = 207,

		// Also write EXEC.
		V3_CMPX_F_U32 = 208,
		V3_CMPX_LT_U32 = 209,
		V3_CMPX_EQ_U32 = 210,
		V3_CMPX_LE_U32 = 211,
		V3_CMPX_GT_U32 = 212,
		V3_CMPX_NE_U32 = 213,
		V3_CMPX_GE_U32 = 214,
		V3_CMPX_T_U32 = 215,
		V3_CMPX_F_F16 = 216,
		V3_CMPX_LT_F16 = 217,
		V3_CMPX_EQ_F16 = 218,
		V3_CMPX_LE_F16 = 219,
		V3_CMPX_GT_F16 = 220,
		V3_CMPX_LG_F16 = 221,
		V3_CMPX_GE_F16 = 222,
		V3_CMPX_O_F16 = 223,

		// On unsigned 64-bit integers.
		V3_CMP_F_U64 = 224,
		V3_CMP_LT_U64 = 225,
		V3_CMP_EQ_U64 = 226,
		V3_CMP_LE_U64 = 227,
		V3_CMP_GT_U64 = 228,
		V3_CMP_NE_U64 = 229,
		V3_CMP_GE_U64 = 230,
		V3_CMP_T_U64 = 231,
		V3_CMP_U_F16 = 232,
		V3_CMP_NGE_F16 = 233,
		V3_CMP_NLG_F16 = 234,
		V3_CMP_NGT_F16 = 235,
		V3_CMP_NLE_F16 = 236,
		V3_CMP_NEQ_F16 = 237,
		V3_CMP_NLT_F16 = 238,
		V3_CMP_TRU_F16 = 239,

		// Also write EXEC.
		V3_CMPX_F_U64 = 240,
		V3_CMPX_LT_U64 = 241,
		V3_CMPX_EQ_U64 = 242,
		V3_CMPX_LE_U64 = 243,
		V3_CMPX_GT_U64 = 244,
		V3_CMPX_NE_U64 = 245,
		V3_CMPX_GE_U64 = 246,
		V3_CMPX_T_U64 = 247,

        V3_CNDMASK_B32      = 256,       //  D.u = S2[i] ? S1.u : S0.u (i = threadID in wave).
        V3_READLANE_B32     = 257,       //  copy one VGPR value to one SGPR. Dst = SGPR-dest, Src0 = Source Data (VGPR# or M0(lds-direct)), Src1 = Lane Select (SGPR or M0). Ignores exec mask.
        V3_WRITELANE_B32    = 258,       //  Write value into one VGPR one one lane. Dst = VGPRdest,  Src0 = Source Data (sgpr, m0, exec or constants), Src1 = Lane Select (SGPR or M0). Ignores exec mask.
        V3_ADD_F32          = 259,      //  D.f = S0.f + S1.f.
        V3_SUB_F32          = 260,      //  D.f = S0.f - S1.f.
        V3_SUBREV_F32       = 261,      //  D.f = S1.f - S0.f.
        V3_MAC_LEGACY_F32   = 262,       //  D.f = S0.F * S1.f + D.f.
        V3_MUL_LEGACY_F32   = 263,       //  D.f = S0.f * S1.f (DX9 rules, 0.0*x = 0.0).
        V3_MUL_F32          = 264,      //  D.f = S0.f * S1.f.
        V3_MUL_I32_I24      = 265,       //  D.i = S0.i[23           // 0] * S1.i[23:0].
        V3_MUL_HI_I32_I24   = 266,       //  D.i = (S0.i[23:0] * S1.i[23:0])>>32.
        V3_MUL_U32_U24      = 267,       //  D.u = S0.u[23:0] * S1.u[23:0].
        V3_MUL_HI_U32_U24   = 268,       //  D.i = (S0.u[23:0] * S1.u[23:0])>>32.
        V3_MIN_LEGACY_F32   = 269,       //  D.f = min(S0.f, S1.f) (DX9 rules for NaN).
        V3_MAX_LEGACY_F32   = 270,       //  D.f = max(S0.f, S1.f) (DX9 rules for NaN).
        V3_MIN_F32          = 271,      //  D.f = min(S0.f, S1.f).
        V3_MAX_F32          = 272,      //  D.f = max(S0.f, S1.f).
        V3_MIN_I32          = 273,      //  D.i = min(S0.i, S1.i).

        V3_MAX_I32         =   274,     //  D.i = max(S0.i, S1.i).
        V3_MIN_U32         =   275,     //  D.u = min(S0.u, S1.u).
        V3_MAX_U32         =   276,     //  D.u = max(S0.u, S1.u).
        V3_LSHR_B32        =   277,     //  D.u = S0.u >> S1.u[4:0].
        V3_LSHRREV_B32     =   278,     //  D.u = S1.u >> S0.u[4:0].
        V3_ASHR_I32        =   279,     //  D.i = S0.i >> S1.i[4:0].
        V3_ASHRREV_I32     =   280,     //  D.i = S1.i >> S0.i[4:0].
        V3_LSHL_B32        =   281,     //  D.u = S0.u << S1.u[4:0].
        V3_LSHLREV_B32     =   282,     //  D.u = S1.u << S0.u[4:0].
        V3_AND_B32         =   283,     //  D.u = S0.u & S1.u.
        V3_OR_B32          =   284,    //  D.u = S0.u | S1.u.
        V3_XOR_B32         =   285,     //  D.u = S0.u ^ S1.u.
        V3_BFM_B32         =   286,     //  D.u = ((1<<S0.u[4:0])-1) << S1.u[4:0]; S0=bitfield_width, S1=bitfield_offset.
        V3_MAC_F32         =   287,     //  D.f = S0.f * S1.f + D.f.
        V3_MADMK_F32       =   288,     //  D.f = S0.f * K + S1.f; K is a 32-bit inline constant.
        V3_MADAK_F32       =   289,     //  D.f = S0.f * S1.f + K; K is a 32-bit inline constant.
        V3_BCNT_U32_B32    =   290,     //  D.u = CountOneBits(S0.u) + S1.u. Bit count.
        V3_MBCNT_LO_U32_B32 =   291,    //  ThreadMask = (1 << ThreadPosition) - 1; D.u = CountOneBits(S0.u & ThreadMask[31              // 0]) + S1.u. Masked bit count, ThreadPosition is the position of this thread in the wavefront (in 0..63).
        V3_MBCNT_HI_U32_B32 =   292,    //  ThreadMask = (1 << ThreadPosition) - 1; D.u = CountOneBits(S0.u & ThreadMask[63:32]) + S1.u. Masked bit count, ThreadPosition is the position of this thread in the wavefront (in 0..63).

        // 293 � 298 See corresponding opcode numbers in VOP3b (3 in, 2 out).
        V3_ADD_I32      = 293,         //  D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V3_SUB_I32      = 294,        //  D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V3_SUBREV_I32   = 295,         //  D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out).
        V3_ADDC_U32     = 296,         //  D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carryout,      S2.u=carry-in).
        V3_SUBB_U32     = 297,         //  D.u = S0.u - S1.u - VCC; VCC=carry-out (VOP3:sgpr=carry-out, S2.u=carry-in).
        V3_SUBBREV_U32  = 298,         //  D.u = S1.u - S0.u - VCC; VCC=carry-out (VOP3:sgpr=carryout, S2.u=carry-in).

        V3_LDEXP_F32          =  299,   //  D.d = pow(S0.f, S1.i).
        V3_CVT_PKACCUM_U8_F32 =  300,            //  f32->u8(s0.f), pack into byte(s1.u), of dst.
        V3_CVT_PKNORM_I16_F32 =  301,            //  D = {(snorm)S1.f, (snorm)S0.f}.
        V3_CVT_PKNORM_U16_F32 =  302,            //  D = {(unorm)S1.f, (unorm)S0.f}.
        V3_CVT_PKRTZ_F16_F32  =  303,           //  D = {flt32_to_flt16(S1.f),flt32_to_flt16(S0.f)}, with round-toward-zero.
        V3_CVT_PK_U16_U32     =  304,        //  D = {(u32->u16)S1.u, (u32->u16)S0.u}.
        V3_CVT_PK_I16_I32     =  305,        //  D = {(i32->i16)S1.i, (i32->i16)S0.i}.
        //318 � 319 Do not use (maps to VOP1 and VOPC).
        //320 � 372 Are VOP3a-only opcodes.
        V3_MAD_LEGACY_F32   =  320,         // D.f = S0.f * S1.f + S2.f (DX9 rules, 0.0*x = 0.0).
        V3_MAD_F32          =  321,         // D.f = S0.f * S1.f + S2.f.
        V3_MAD_I32_I24      =  322,         // D.i = S0.i * S1.i + S2.iD.i = S0.i * S1.i + S2.i.
        V3_MAD_U32_U24      =  323,         // D.u = S0.u * S1.u + S2.u.
        V3_CUBEID_F32       =  324,         // Rm.w <- Rn,x, Rn,y, Rn.z.
        V3_CUBESC_F32       =  325,         // Rm.y <- Rn,x, Rn,y, Rn.z.
        V3_CUBETC_F32       =  326,         // Rm.x <- Rn,x, Rn,y, Rn.z.
        V3_CUBEMA_F32       =  327,         // Rm.z <- Rn,x, Rn,y, Rn.z
        V3_BFE_U32          =  328  ,       // D.u = (S0.u>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        V3_BFE_I32          =  329,         // D.i = (S0.i>>S1.u[4:0]) & ((1<<S2.u[4:0])-1); bitfield extract, S0=data, S1=field_offset, S2=field_width.
        V3_BFI_B32          =  330,         // D.u = (S0.u & S1.u) | (~S0.u & S2.u); bitfield insert.
        V3_FMA_F32          =  331,         // D.f = S0.f * S1.f + S2.f
        V3_FMA_F64          =  332,         // D.d = S0.d * S1.d + S2.d.

        V3_LERP_U8          =  333,         // D.u = ((S0.u[31:24] + S1.u[31:24] + S2.u[24]) >> 1) << 24 +
        V3_ALIGNBIT_B32     =  334,         // D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
        V3_ALIGNBYTE_B32        =  335,         // D.u = ({S0,S1} >> (8*S2.u[4:0])) & 0xFFFFFFFF.
        V3_MULLIT_F32       =  336,         // D.f = S0.f * S1.f, replicate result into 4 components (0.0 * x = 0.0; special INF, NaN, overflow rules).
        V3_MIN3_F32             =  337,         //D.f = min(S0.f, S1.f, S2.f).
        V3_MIN3_I32             =  338,         //D.i = min(S0.i, S1.i, S2.i).
        V3_MIN3_U32             =  339,         //0x153 D.u = min(S0.u, S1.u, S2.u).
        V3_MAX3_F32             =  340,         //D.f = max(S0.f, S1.f, S2.f).
        V3_MAX3_I32             =  341,         //D.i = max(S0.i, S1.i, S2.i).
        V3_MAX3_U32             =  342,         //D.u = max(S0.u, S1.u, S2.u).
        V3_MED3_F32             =  343,         //D.f = median(S0.f, S1.f, S2.f).
        V3_MED3_I32             =  344,         //D.i = median(S0.i, S1.i, S2.i).
        V3_MED3_U32         =  345,         // D.u = median(S0.u, S1.u, S2.u).
        V3_SAD_U8           =  346,         //D.u = Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        V3_SAD_HI_U8            =  347,         //D.u = Byte SAD with accum_hi(S0.u, S1.u, S2.u).
        V3_SAD_U16          =  348,         //D.u = Word SAD with accum(S0.u, S1.u, S2.u).
        V3_SAD_U32          =  349,         //D.u = Dword SAD with accum(S0.u, S1.u, S2.u).
        V3_CVT_PK_U8_F32        =  350,         //f32->u8(s0.f), pack into byte(s1.u), of dword(s2).
        V3_DIV_FIXUP_F32        =  351,         //D.f = Special case divide fixup and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        V3_DIV_FIXUP_F64        =  352,         //D.d = Special case divide fixup and flags(s0.d = Quotient, s1.d = Denominator, s2.d = Numerator).
        V3_LSHL_B64         =  353,         //D = S0.u << S1.u[4:0].
        V3_LSHR_B64         =  354  ,       //D = S0.u >> S1.u[4:0].
        V3_ASHR_I64         =  355,         //D = S0.u >> S1.u[4:0].
        V3_ADD_F64          =  356,         //D.d = S0.d + S1.d.
        V3_MUL_F64          =  357,         //D.d = S0.d * S1.d.
        V3_MIN_F64          =  358,         //D.d = min(S0.d, S1.d).
        V3_MAX_F64          =  359,         //D.d = max(S0.d, S1.d).
        V3_LDEXP_F64            =  360,         //D.d = pow(S0.d, S1.i[31:0]).
        V3_MUL_LO_U32       =  361,         //D.u = S0.u * S1.u.
        V3_MUL_HI_U32       =  362,         //D.u = (S0.u * S1.u)>>32.
        V3_MUL_LO_I32       =  363,         //D.i = S0.i * S1.i.
        V3_MUL_HI_I32       =  364,         //D.i = (S0.i * S1.i)>>32.
        //365 � 366 See corresponding opcode numbers in VOP3 (3 in, 2 out), (VOP3b).
        V3_DIV_SCALE_F32 = 365, //D.f = Special case divide preop and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator) s0 must equal s1 or s2.
        V3_DIV_SCALE_F64 = 366, //D.d = Special case divide preop and flags(s0.d = Quotient, s1.d = Denominator, s2.d = Numerator) s0 must equal s1 or s2.

        V3_DIV_FMAS_F32     =   367,            //D.f = Special case divide FMA with scale and flags(s0.f = Quotient, s1.f = Denominator, s2.f = Numerator).
        V3_DIV_FMAS_F64     =   368,            //D.d = Special case divide FMA with scale and flags(s0.d = Quotient, s1.d = Denominator, s2.d = Numerator).
        V3_MSAD_U8          =   369,            //D.u = Masked Byte SAD with accum_lo(S0.u, S1.u, S2.u).
        V3_QSAD_U8          =   370,            //D.u = Quad-Byte SAD with accum_lo/hiu(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        V3_MQSAD_U8         =   371,            //D.u = Masked Quad-Byte SAD with accum_lo/hi(S0.u[63:0], S1.u[31:0], S2.u[63:0]).
        V3_TRIG_PREOP_F64   =   372,            //D.d = Look Up 2/PI (S0.d) with segment select S1.u[4:0].

        // 384 � 452 Are the VOP1 opcodes when VOP3 encoding is required. For example, V_OP1_OFFSET + V_MOV_B32 generates the VOP3 version of MOV.
        V3_NOP                =  384,   //  do nothing.
        V3_MOV_B32             =  385,   //  D.u = S0.u.
        V3_READFIRSTLANE_B32   =  386,             //  copy one VGPR value to one SGPR. Dst = SGPRdest, Src0 = Source Data (VGPR# or M0(lds-direct)), Lane# = FindFirst1fromLSB(exec) (lane = 0 if exec is zero). Ignores exec mask.
        V3_CVT_I32_F64         =  387,       //  D.i = (int)S0.d.
        V3_CVT_F64_I32         =  388,       //  D.f = (float)S0.i.
        V3_CVT_F32_I32         =  389,       //  D.f = (float)S0.i.
        V3_CVT_F32_U32         =  390,       //  D.f = (float)S0.u.
        V3_CVT_U32_F32         =  391,       //  D.u = (unsigned)S0.f.
        V3_CVT_I32_F32         =  392,       //  D.i = (int)S0.f.
        V3_MOV_FED_B32         =  393,       //  D.u = S0.u, introduce edc double error upon write to dest vgpr without causing an exception.
        V3_CVT_F16_F32         =  394,       //  D.f16 = flt32_to_flt16(S0.f).
        V3_CVT_F32_F16         =  395,       //  D.f = flt16_to_flt32(S0.f16).
        V3_CVT_RPI_I32_F32     =  396,           //  D.i = (int)floor(S0.f + 0.5).
        V3_CVT_FLR_I32_F32     =  397,           //  D.i = (int)floor(S0.f).
        V3_CVT_OFF_F32_I4      =  398,          //  4-bit signed int to 32-bit float. For interpolation in shader.
        V3_CVT_F32_F64         =  399,       //  D.f = (float)S0.d.
        V3_CVT_F64_F32         =  400,       //  D.d = (double)S0.f.
        V3_CVT_F32_UBYTE0      =  401,          //  D.f = UINT2FLT(S0.u[7              // 0]).
        V3_CVT_F32_UBYTE1      =  402,          //  D.f = UINT2FLT(S0.u[15:8]).
        V3_CVT_F32_UBYTE2      =  403,          //  D.f = UINT2FLT(S0.u[23:16]).
        V3_CVT_F32_UBYTE3      =  404,          //  D.f = UINT2FLT(S0.u[31:24]).
        V3_CVT_U32_F64         =  405,      //  D.u = (uint)S0.d.
        V3_CVT_F64_U32         =  406,      //  D.d = (double)S0.u.
        //407 � 415 reserved.
        V3_FRACT_F32           =  416,     //  D.f = S0.f - floor(S0.f).
        V3_TRUNC_F32           =  417,     //  D.f = trunc(S0.f), return integer part of S0.
        V3_CEIL_F32            =  418,    //  D.f = ceil(S0.f). Implemented as: D.f = trunc(S0.f); if (S0 > 0.0 && S0 != D), D += 1.0.
        V3_RNDNE_F32           =  419,     //  D.f = round_nearest_even(S0.f).
        V3_FLOOR_F32           =  420,     //  D.f = trunc(S0); if ((S0 < 0.0) && (S0 != D)) D += -1.0.
        V3_EXP_F32             =  421,   //  D.f = pow(2.0, S0.f).
        V3_LOG_CLAMP_F32       =  422,         //  D.f = log2(S0.f), clamp -infinity to -max_float.
        V3_LOG_F32             =  423,   //  D.f = log2(S0.f).
        V3_RCP_CLAMP_F32       =  424,         //  D.f = 1.0 / S0.f, result clamped to +-max_float.
        V3_RCP_LEGACY_F32      =  425,          //  D.f = 1.0 / S0.f, +-infinity result clamped to +-0.0.
        V3_RCP_F32             =  426,   //  D.f = 1.0 / S0.f.
        V3_RCP_IFLAG_F32       =  427,         //  D.f = 1.0 / S0.f, only integer div_by_zero flag can be           raised.
        V3_RSQ_CLAMP_F32       =  428,         //  D.f = 1.0 / sqrt(S0.f), result clamped to +-max_float.
        V3_RSQ_LEGACY_F32      =  429,          //  D.f = 1.0 / sqrt(S0.f).
        V3_RSQ_F32             =  430,   //  D.f = 1.0 / sqrt(S0.f).
        V3_RCP_F64             =  431,   //  D.d = 1.0 / (S0.d).
        V3_RCP_CLAMP_F64       =  432,         //  D.f = 1.0 / (S0.f), result clamped to +-max_float.
        V3_RSQ_F64             =  433,   //  D.f = 1.0 / sqrt(S0.f).
        V3_RSQ_CLAMP_F64       =  434,         //  D.d = 1.0 / sqrt(S0.d), result clamped to +-max_float.
        V3_SQRT_F32            =  435,    //  D.f = sqrt(S0.f).
        V3_SQRT_F64            =  436,    //  D.d = sqrt(S0.d).
        V3_SIN_F32             =  437,   //  D.f = sin(S0.f).
        V3_COS_F32             =  438,   //  D.f = cos(S0.f).
        V3_NOT_B32             =  439,   //  D.u = ~S0.u.
        V3_BFREV_B32           =  440,     //  D.u[31:0] = S0.u[0:31], bitfield reverse.
        V3_FFBH_U32            =  441,    //  D.u = position of first 1 in S0 from MSB; D=0xFFFFFFFF if             S0==0.
        V3_FFBL_B32            =  442,    //  D.u = position of first 1 in S0 from LSB; D=0xFFFFFFFF if             S0==0.
        V3_FFBH_I32            =  443,    //  D.u = position of first bit different from sign bit in S0 from MSB;       D=0xFFFFFFFF if S0==0 or 0xFFFFFFFF.
        V3_FREXP_EXP_I32_F64   =  444,             //  See V_FREXP_EXP_I32_F32.
        V3_FREXP_MANT_F64      =  445,          //  See V_FREXP_MANT_F32.
        V3_FRACT_F64           =  446,     //  S0.d - floor(S0.d).
        V3_FREXP_EXP_I32_F32   =  447,             //  If (S0.f == INF || S0.f == NAN), then D.i = 0; else D.i = TwosComplement(Exponent(S0.f) - 127 + 1). Returns exponent of single precision float input, such that S0.f = significand * (2 ** exponent). See also FREXP_MANT_F32, which returns the significand.
        V3_FREXP_MANT_F32      =  448,          //  if (S0.f == INF || S0.f == NAN) then D.f = S0.f; else D.f = Mantissa(S0.f). Result range is in (-1.0,-0.5][0.5,1.0) in normal cases.
        V3_CLREXCP             =  449,   //  Clear wave's exception state in shader processor SIMD.
        V3_MOVRELD_B32         =  450,       //  VGPR[D.u + M0.u] = VGPR[S0.u].
        V3_MOVRELS_B32         =  451,       //  VGPR[D.u] = VGPR[S0.u + M0.u].
        V3_MOVRELSD_B32        =  452,        //  VGPR[D.u + M0.u] = VGPR[S0.u + M0.u].

        VOP3_UNDEFINE = 999,
    } ;// end VOP3

    /// -----------------------------------------------------------------------------------------------
    SIVOP3Instruction(SRC src0, SRC src1, SRC src2, VDST vdst, SDST sdst,
		unsigned int ridx0, unsigned int ridx1, unsigned int ridx2, 
		unsigned int vdstRidx, unsigned int sdstRidx,
		CLMP clmp, ABS abs, NEG neg, OMOD omod,
		unsigned int instructionWidth, VOP3_OP op, 
		Encoding instructionEncoding, 
		InstructionClass insClass = InstructionClassUnknown, 
		OperandType operandType = TypeNone) :
		VOPInstruction(src0, vdst, ridx0, vdstRidx, instructionWidth, InstructionSet_VOP3, insClass, operandType),
		m_op(op), m_instructionEncoding(instructionEncoding),
		m_src1(src1), m_src2(src2), m_sdst(sdst),
		m_ridx1(ridx1), m_ridx2(ridx2), m_sdstRidx(sdstRidx),
		m_clmp(clmp), m_abs(abs), m_neg(neg), m_omod(omod)
	{ }
    /// -----------------------------------------------------------------------------------------------

    ~SIVOP3Instruction() {}

    /// Get the OP
    VOP3_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

	SRC GetSRC1() const { return m_src1; }

	SRC GetSRC2() const { return m_src2; }

	SDST GetSDST() const { return m_sdst; }

	OMOD GetOMOD() const { return m_omod; }

	NEG GetNEG() const { return m_neg; }

	ABS GetABS() const { return m_abs; }

	CLMP GetCLMP() const { return m_clmp; }

	uint32_t GetRidx1() const { return m_ridx1; }

	uint32_t GetRidx2() const { return m_ridx2; }

	uint32_t GetSDSTRidx() const { return m_sdstRidx; }

private:

    /// VOP operation.
    VOP3_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;

	SRC m_src1;

	SRC m_src2;

	SDST m_sdst;

	unsigned int m_ridx1;

	unsigned int m_ridx2;

	unsigned int m_sdstRidx;


	CLMP m_clmp;

	ABS m_abs;

	NEG m_neg;

	OMOD m_omod;

    int GetInstructionClockCount()
    {
        int iRet = 4;

        switch (m_op)
        {
			case    V3_CMP_F_F32:
			case    V3_CMP_LT_F32:
			case    V3_CMP_EQ_F32:
			case    V3_CMP_LE_F32:
			case    V3_CMP_GT_F32:
			case    V3_CMP_LG_F32:
			case    V3_CMP_GE_F32:
			case    V3_CMP_O_F32:
			case    V3_CMP_U_F32:
			case    V3_CMP_NGE_F32:
			case    V3_CMP_NLG_F32:
			case    V3_CMP_NGT_F32:
			case    V3_CMP_NLE_F32:
			case    V3_CMP_NEQ_F32:
			case    V3_CMP_NLT_F32:
			case    V3_CMP_TRU_F32:
			case    V3_CMPX_F_F32:
			case    V3_CMPX_LT_F32:
			case    V3_CMPX_EQ_F32:
			case    V3_CMPX_LE_F32:
			case    V3_CMPX_GT_F32:
			case    V3_CMPX_LG_F32:
			case    V3_CMPX_GE_F32:
			case    V3_CMPX_O_F32:
			case    V3_CMPX_U_F32:
			case    V3_CMPX_NGE_F32:
			case    V3_CMPX_NLG_F32:
			case    V3_CMPX_NGT_F32:
			case    V3_CMPX_NLE_F32:
			case    V3_CMPX_NEQ_F32:
			case    V3_CMPX_NLT_F32:
			case    V3_CMPX_TRU_F32:
			case    V3_CMPS_F_F32:
			case    V3_CMPS_LT_F32:
			case    V3_CMPS_EQ_F32:
			case    V3_CMPS_LE_F32:
			case    V3_CMPS_GT_F32:
			case    V3_CMPS_LG_F32:
			case    V3_CMPS_GE_F32:
			case    V3_CMPS_O_F32:
			case    V3_CMPS_U_F32:
			case    V3_CMPS_NGE_F32:
			case    V3_CMPS_NLG_F32:
			case    V3_CMPS_NGT_F32:
			case    V3_CMPS_NLE_F32:
			case    V3_CMPS_NEQ_F32:
			case    V3_CMPS_NLT_F32:
			case    V3_CMPS_TRU_F32:
			case    V3_CMPSX_F_F32:
			case    V3_CMPSX_LT_F32:
			case    V3_CMPSX_EQ_F32:
			case    V3_CMPSX_LE_F32:
			case    V3_CMPSX_GT_F32:
			case    V3_CMPSX_LG_F32:
			case    V3_CMPSX_GE_F32:
			case    V3_CMPSX_O_F32:
			case    V3_CMPSX_U_F32:
			case    V3_CMPSX_NGE_F32:
			case    V3_CMPSX_NLG_F32:
			case    V3_CMPSX_NGT_F32:
			case    V3_CMPSX_NLE_F32:
			case    V3_CMPSX_NEQ_F32:
			case    V3_CMPSX_NLT_F32:
			case    V3_CMPSX_TRU_F32:
			case    V3_CMP_F_I32:
			case    V3_CMP_LT_I32:
			case    V3_CMP_EQ_I32:
			case    V3_CMP_LE_I32:
			case    V3_CMP_GT_I32:
			case    V3_CMP_NE_I32:
			case    V3_CMP_GE_I32:
			case    V3_CMP_T_I32:
			case    V3_CMP_CLASS_F32:
			case    V3_CMPX_F_I32:
			case    V3_CMPX_LT_I32:
			case    V3_CMPX_EQ_I32:
			case    V3_CMPX_LE_I32:
			case    V3_CMPX_GT_I32:
			case    V3_CMPX_NE_I32:
			case    V3_CMPX_GE_I32:
			case    V3_CMPX_T_I32:
			case    V3_CMPX_CLASS_F32:
			case    V3_CMP_F_U32:
			case    V3_CMP_LT_U32:
			case    V3_CMP_EQ_U32:
			case    V3_CMP_LE_U32:
			case    V3_CMP_GT_U32:
			case    V3_CMP_NE_U32:
			case    V3_CMP_GE_U32:
			case    V3_CMP_T_U32:
			case    V3_CMPX_F_U32:
			case    V3_CMPX_LT_U32:
			case    V3_CMPX_EQ_U32:
			case    V3_CMPX_LE_U32:
			case    V3_CMPX_GT_U32:
			case    V3_CMPX_NE_U32:
			case    V3_CMPX_GE_U32:
			case    V3_CMPX_T_U32:
            case    V3_CNDMASK_B32   :
            case    V3_READLANE_B32  :
            case    V3_WRITELANE_B32 :
            case    V3_ADD_F32       :
            case    V3_SUB_F32       :
            case    V3_SUBREV_F32    :
            case    V3_MAC_LEGACY_F32:
            case    V3_MUL_LEGACY_F32:
            case    V3_MUL_F32       :
            case    V3_MUL_I32_I24   :
            case    V3_MUL_HI_I32_I24:
            case    V3_MUL_U32_U24   :
            case    V3_MUL_HI_U32_U24:
            case    V3_MIN_LEGACY_F32:
            case    V3_MAX_LEGACY_F32:
            case    V3_MIN_F32       :
            case    V3_MAX_F32       :
            case    V3_MIN_I32       :
            case    V3_ADD_I32       :
            case    V3_SUB_I32       :
            case    V3_SUBREV_I32    :
            case    V3_ADDC_U32      :
            case    V3_SUBB_U32      :
            case    V3_SUBBREV_U32   :
            case    V3_LDEXP_F32     :
            case    V3_CVT_PKACCUM_U8_F32:
            case    V3_CVT_PKNORM_I16_F32:
            case    V3_CVT_PKNORM_U16_F32:
            case    V3_CVT_PKRTZ_F16_F32:
            case    V3_CVT_PK_U16_U32:
            case    V3_CVT_PK_I16_I32:
            case    V3_MAD_LEGACY_F32:
            case    V3_MAD_F32:
            case    V3_MAD_I32_I24  :
            case    V3_MAD_U32_U24  :
            case    V3_CUBEID_F32   :
            case    V3_CUBESC_F32   :
            case    V3_CUBETC_F32   :
            case    V3_CUBEMA_F32   :
            case    V3_BFE_U32      :
            case    V3_BFE_I32      :
            case    V3_BFI_B32      :
            case    V3_FMA_F32      :
            case    V3_LERP_U8      :
            case    V3_ALIGNBIT_B32 :
            case    V3_ALIGNBYTE_B32:
            case    V3_MULLIT_F32   :
            case    V3_MIN3_F32     :
            case    V3_MIN3_I32     :
            case    V3_MIN3_U32     :
            case    V3_MAX3_F32     :
            case    V3_MAX3_I32     :
            case    V3_MAX3_U32     :
            case    V3_MED3_F32     :
            case    V3_MED3_I32     :
            case    V3_MED3_U32     :
            case    V3_SAD_U8       :
            case    V3_SAD_HI_U8    :
            case    V3_SAD_U16      :
            case    V3_SAD_U32      :
            case    V3_CVT_PK_U8_F32:
            case    V3_DIV_FIXUP_F32:
            case    V3_MUL_LO_U32   :
            case    V3_MUL_HI_U32   :
            case    V3_MUL_LO_I32   :
            case    V3_MUL_HI_I32   :
            case    V3_DIV_SCALE_F32:
            case    V3_DIV_FMAS_F32 :
            case    V3_MSAD_U8      :
            case    V3_QSAD_U8      :
            case    V3_MQSAD_U8     :
            case    V3_NOP          :
            case    V3_MOV_B32      :
            case    V3_READFIRSTLANE_B32:
            case    V3_CVT_F64_I32  :
            case    V3_CVT_F32_I32  :
            case    V3_CVT_F32_U32  :
            case    V3_CVT_U32_F32  :
            case    V3_CVT_I32_F32  :
            case    V3_MOV_FED_B32  :
            case    V3_CVT_F16_F32    :
            case    V3_CVT_F32_F16    :
            case    V3_CVT_RPI_I32_F32:
            case    V3_CVT_FLR_I32_F32:
            case    V3_CVT_OFF_F32_I4 :
            case    V3_CVT_F32_UBYTE0 :
            case    V3_CVT_F32_UBYTE1 :
            case    V3_CVT_F32_UBYTE2 :
            case    V3_CVT_F32_UBYTE3 :
            case    V3_FRACT_F32      :
            case    V3_TRUNC_F32      :
            case    V3_CEIL_F32       :
            case    V3_RNDNE_F32      :
            case    V3_FLOOR_F32      :
            case    V3_EXP_F32        :
            case    V3_LOG_CLAMP_F32  :
            case    V3_LOG_F32        :
            case    V3_RCP_CLAMP_F32  :
            case    V3_RCP_LEGACY_F32 :
            case    V3_RCP_F32        :
            case    V3_RCP_IFLAG_F32  :
            case    V3_RSQ_CLAMP_F32  :
            case    V3_RSQ_LEGACY_F32 :
            case    V3_RSQ_F32       :
            case    V3_SQRT_F32       :
            case    V3_SIN_F32        :
            case    V3_COS_F32        :
            case    V3_NOT_B32        :
            case    V3_BFREV_B32      :
            case    V3_FFBH_U32       :
            case    V3_FFBL_B32       :
            case    V3_FFBH_I32       :
            case    V3_FREXP_EXP_I32_F32:
            case    V3_FREXP_MANT_F32:
            case    V3_CLREXCP        :
            case    V3_MOVRELD_B32    :
            case    V3_MOVRELS_B32    :
            case    V3_MOVRELSD_B32   :
                iRet = 4;
                break;

			case    V3_CMP_F_F64:
			case    V3_CMP_LT_F64:
			case    V3_CMP_EQ_F64:
			case    V3_CMP_LE_F64:
			case    V3_CMP_GT_F64:
			case    V3_CMP_LG_F64:
			case    V3_CMP_GE_F64:
			case    V3_CMP_O_F64:
			case    V3_CMP_U_F64:
			case    V3_CMP_NGE_F64:
			case    V3_CMP_NLG_F64:
			case    V3_CMP_NGT_F64:
			case    V3_CMP_NLE_F64:
			case    V3_CMP_NEQ_F64:
			case    V3_CMP_NLT_F64:
			case    V3_CMP_TRU_F64:
			case    V3_CMPX_F_F64:
			case    V3_CMPX_LT_F64:
			case    V3_CMPX_EQ_F64:
			case    V3_CMPX_LE_F64:
			case    V3_CMPX_GT_F64:
			case    V3_CMPX_LG_F64:
			case    V3_CMPX_GE_F64:
			case    V3_CMPX_O_F64:
			case    V3_CMPX_U_F64:
			case    V3_CMPX_NGE_F64:
			case    V3_CMPX_NLG_F64:
			case    V3_CMPX_NGT_F64:
			case    V3_CMPX_NLE_F64:
			case    V3_CMPX_NEQ_F64:
			case    V3_CMPX_NLT_F64:
			case    V3_CMPX_TRU_F64:
			case    V3_CMPS_F_F64:
			case    V3_CMPS_LT_F64:
			case    V3_CMPS_EQ_F64:
			case    V3_CMPS_LE_F64:
			case    V3_CMPS_GT_F64:
			case    V3_CMPS_LG_F64:
			case    V3_CMPS_GE_F64:
			case    V3_CMPS_O_F64:
			case    V3_CMPS_U_F64:
			case    V3_CMPS_NGE_F64:
			case    V3_CMPS_NLG_F64:
			case    V3_CMPS_NGT_F64:
			case    V3_CMPS_NLE_F64:
			case    V3_CMPS_NEQ_F64:
			case    V3_CMPS_NLT_F64:
			case    V3_CMPS_TRU_F64:
			case    V3_CMPSX_F_F64:
			case    V3_CMPSX_LT_F64:
			case    V3_CMPSX_EQ_F64:
			case    V3_CMPSX_LE_F64:
			case    V3_CMPSX_GT_F64:
			case    V3_CMPSX_LG_F64:
			case    V3_CMPSX_GE_F64:
			case    V3_CMPSX_O_F64:
			case    V3_CMPSX_U_F64:
			case    V3_CMPSX_NGE_F64:
			case    V3_CMPSX_NLG_F64:
			case    V3_CMPSX_NGT_F64:
			case    V3_CMPSX_NLE_F64:
			case    V3_CMPSX_NEQ_F64:
			case    V3_CMPSX_NLT_F64:
			case    V3_CMPSX_TRU_F64:
			case    V3_CMP_F_I64:
			case    V3_CMP_LT_I64:
			case    V3_CMP_EQ_I64:
			case    V3_CMP_LE_I64:
			case    V3_CMP_GT_I64:
			case    V3_CMP_NE_I64:
			case    V3_CMP_GE_I64:
			case    V3_CMP_T_I64:
			case    V3_CMP_CLASS_F64:
			case    V3_CMPX_F_I64:
			case    V3_CMPX_LT_I64:
			case    V3_CMPX_EQ_I64:
			case    V3_CMPX_LE_I64:
			case    V3_CMPX_GT_I64:
			case    V3_CMPX_NE_I64:
			case    V3_CMPX_GE_I64:
			case    V3_CMPX_T_I64:
			case    V3_CMPX_CLASS_F64:
			case    V3_CMP_F_U64:
			case    V3_CMP_LT_U64:
			case    V3_CMP_EQ_U64:
			case    V3_CMP_LE_U64:
			case    V3_CMP_GT_U64:
			case    V3_CMP_NE_U64:
			case    V3_CMP_GE_U64:
			case    V3_CMP_T_U64:
			case    V3_CMPX_F_U64:
			case    V3_CMPX_LT_U64:
			case    V3_CMPX_EQ_U64:
			case    V3_CMPX_LE_U64:
			case    V3_CMPX_GT_U64:
			case    V3_CMPX_NE_U64:
			case    V3_CMPX_GE_U64:
			case    V3_CMPX_T_U64:
            case    V3_FMA_F64          :
            case    V3_DIV_FIXUP_F64 :
            case    V3_LSHL_B64      :
            case    V3_LSHR_B64      :
            case    V3_ASHR_I64      :
            case    V3_ADD_F64       :
            case    V3_MUL_F64       :
            case    V3_MIN_F64       :
            case    V3_MAX_F64       :
            case    V3_LDEXP_F64     :
            case    V3_DIV_FMAS_F64  :
            case    V3_TRIG_PREOP_F64:
            case    V3_CVT_I32_F64   :
            case    V3_CVT_F32_F64   :
            case    V3_CVT_F64_F32   :
            case    V3_CVT_U32_F64   :
            case    V3_CVT_F64_U32   :
            case    V3_RCP_F64       :
            case    V3_RCP_CLAMP_F64:
            case    V3_RSQ_F64      :
            case    V3_RSQ_CLAMP_F64:
            case    V3_SQRT_F64      :
            case    V3_FREXP_EXP_I32_F64   :
            case    V3_FREXP_MANT_F64:
            case    V3_FRACT_F64:
                iRet = 8;
                break;

            default:
                break;
        }

        return iRet;
    }
};

class SIVOPCInstruction : public VOPInstruction
{
public:

    enum VOPC_OP // Single Vector Compare Operations
    {
		// Signal on sNaN input only.
		V_CMP_F_F32 = 0,
		V_CMP_LT_F32 = 1,
		V_CMP_EQ_F32 = 2,
		V_CMP_LE_F32 = 3,
		V_CMP_GT_F32 = 4,
		V_CMP_LG_F32 = 5,
		V_CMP_GE_F32 = 6,
		V_CMP_O_F32 = 7,
		V_CMP_U_F32 = 8,
		V_CMP_NGE_F32 = 9,
		V_CMP_NLG_F32 = 10,
		V_CMP_NGT_F32 = 11,
		V_CMP_NLE_F32 = 12,
		V_CMP_NEQ_F32 = 13,
		V_CMP_NLT_F32 = 14,
		V_CMP_TRU_F32 = 15,

		// Signal on sNaN input only. Also write EXEC.
		V_CMPX_F_F32 = 16,
		V_CMPX_LT_F32 = 17,
		V_CMPX_EQ_F32 = 18,
		V_CMPX_LE_F32 = 19,
		V_CMPX_GT_F32 = 20,
		V_CMPX_LG_F32 = 21,
		V_CMPX_GE_F32 = 22,
		V_CMPX_O_F32 = 23,
		V_CMPX_U_F32 = 24,
		V_CMPX_NGE_F32 = 25,
		V_CMPX_NLG_F32 = 26,
		V_CMPX_NGT_F32 = 27,
		V_CMPX_NLE_F32 = 28,
		V_CMPX_NEQ_F32 = 29,
		V_CMPX_NLT_F32 = 30,
		V_CMPX_TRU_F32 = 31,

		// Signal on sNaN input only.
		V_CMP_F_F64 = 32,
		V_CMP_LT_F64 = 33,
		V_CMP_EQ_F64 = 34,
		V_CMP_LE_F64 = 35,
		V_CMP_GT_F64 = 36,
		V_CMP_LG_F64 = 37,
		V_CMP_GE_F64 = 38,
		V_CMP_O_F64 = 39,
		V_CMP_U_F64 = 40,
		V_CMP_NGE_F64 = 41,
		V_CMP_NLG_F64 = 42,
		V_CMP_NGT_F64 = 43,
		V_CMP_NLE_F64 = 44,
		V_CMP_NEQ_F64 = 45,
		V_CMP_NLT_F64 = 46,
		V_CMP_TRU_F64 = 47,

		// Signal on sNaN input only. Also write EXEC.
		V_CMPX_F_F64 = 48,
		V_CMPX_LT_F64 = 49,
		V_CMPX_EQ_F64 = 50,
		V_CMPX_LE_F64 = 51,
		V_CMPX_GT_F64 = 52,
		V_CMPX_LG_F64 = 53,
		V_CMPX_GE_F64 = 54,
		V_CMPX_O_F64 = 55,
		V_CMPX_U_F64 = 56,
		V_CMPX_NGE_F64 = 57,
		V_CMPX_NLG_F64 = 58,
		V_CMPX_NGT_F64 = 59,
		V_CMPX_NLE_F64 = 60,
		V_CMPX_NEQ_F64 = 61,
		V_CMPX_NLT_F64 = 62,
		V_CMPX_TRU_F64 = 63,

		// Signal on any NaN.
		V_CMPS_F_F32 = 64,
		V_CMPS_LT_F32 = 65,
		V_CMPS_EQ_F32 = 66,
		V_CMPS_LE_F32 = 67,
		V_CMPS_GT_F32 = 68,
		V_CMPS_LG_F32 = 69,
		V_CMPS_GE_F32 = 70,
		V_CMPS_O_F32 = 71,
		V_CMPS_U_F32 = 72,
		V_CMPS_NGE_F32 = 73,
		V_CMPS_NLG_F32 = 74,
		V_CMPS_NGT_F32 = 75,
		V_CMPS_NLE_F32 = 76,
		V_CMPS_NEQ_F32 = 77,
		V_CMPS_NLT_F32 = 78,
		V_CMPS_TRU_F32 = 79,

		//Signal on any NaN. Also write EXEC.
		V_CMPSX_F_F32 = 80,
		V_CMPSX_LT_F32 = 81,
		V_CMPSX_EQ_F32 = 82,
		V_CMPSX_LE_F32 = 83,
		V_CMPSX_GT_F32 = 84,
		V_CMPSX_LG_F32 = 85,
		V_CMPSX_GE_F32 = 86,
		V_CMPSX_O_F32 = 87,
		V_CMPSX_U_F32 = 88,
		V_CMPSX_NGE_F32 = 89,
		V_CMPSX_NLG_F32 = 90,
		V_CMPSX_NGT_F32 = 91,
		V_CMPSX_NLE_F32 = 92,
		V_CMPSX_NEQ_F32 = 93,
		V_CMPSX_NLT_F32 = 94,
		V_CMPSX_TRU_F32 = 95,

		//ignal on any NaN.
		V_CMPS_F_F64 = 96,
		V_CMPS_LT_F64 = 97,
		V_CMPS_EQ_F64 = 98,
		V_CMPS_LE_F64 = 99,
		V_CMPS_GT_F64 = 100,
		V_CMPS_LG_F64 = 101,
		V_CMPS_GE_F64 = 102,
		V_CMPS_O_F64 = 103,
		V_CMPS_U_F64 = 104,
		V_CMPS_NGE_F64 = 105,
		V_CMPS_NLG_F64 = 106,
		V_CMPS_NGT_F64 = 107,
		V_CMPS_NLE_F64 = 108,
		V_CMPS_NEQ_F64 = 109,
		V_CMPS_NLT_F64 = 110,
		V_CMPS_TRU_F64 = 111,

		//Signal on any NaN. Also write EXEC.
		V_CMPSX_F_F64 = 112,
		V_CMPSX_LT_F64 = 113,
		V_CMPSX_EQ_F64 = 114,
		V_CMPSX_LE_F64 = 115,
		V_CMPSX_GT_F64 = 116,
		V_CMPSX_LG_F64 = 117,
		V_CMPSX_GE_F64 = 118,
		V_CMPSX_O_F64 = 119,
		V_CMPSX_U_F64 = 120,
		V_CMPSX_NGE_F64 = 121,
		V_CMPSX_NLG_F64 = 122,
		V_CMPSX_NGT_F64 = 123,
		V_CMPSX_NLE_F64 = 124,
		V_CMPSX_NEQ_F64 = 125,
		V_CMPSX_NLT_F64 = 126,
		V_CMPSX_TRU_F64 = 127,

		// On 32-bit integers.
		V_CMP_F_I32 = 128,
		V_CMP_LT_I32 = 129,
		V_CMP_EQ_I32 = 130,
		V_CMP_LE_I32 = 131,
		V_CMP_GT_I32 = 132,
		V_CMP_NE_I32 = 133,
		V_CMP_GE_I32 = 134,
		V_CMP_T_I32 = 135,
		V_CMP_CLASS_F32 = 136,  // D = IEEE numeric class function specified in S1.u, performed on S0.f.
		V_CMP_LT_I16 = 137,
		V_CMP_EQ_I16 = 138,
		V_CMP_LE_I16 = 139,
		V_CMP_GT_I16 = 140,
		V_CMP_NE_I16 = 141,
		V_CMP_GE_I16 = 142,
		V_CMP_CLASS_F16 = 143,

		// Also write EXEC.
		V_CMPX_F_I32 = 144,
		V_CMPX_LT_I32 = 145,
		V_CMPX_EQ_I32 = 146,
		V_CMPX_LE_I32 = 147,
		V_CMPX_GT_I32 = 148,
		V_CMPX_NE_I32 = 149,
		V_CMPX_GE_I32 = 150,
		V_CMPX_T_I32 = 151,
		V_CMPX_CLASS_F32 = 152,  // D = IEEE numeric class function specified in S1.u, performed on S0.f. Also write EXEC.
		V_CMPX_LT_I16 = 153,
		V_CMPX_EQ_I16 = 154,
		V_CMPX_LE_I16 = 155,
		V_CMPX_GT_I16 = 156,
		V_CMPX_NE_I16 = 157,
		V_CMPX_GE_I16 = 158,
		V_CMPX_CLASS_F16 = 159,

		// On 64-bit integers.
		V_CMP_F_I64 = 160,
		V_CMP_LT_I64 = 161,
		V_CMP_EQ_I64 = 162,
		V_CMP_LE_I64 = 163,
		V_CMP_GT_I64 = 164,
		V_CMP_NE_I64 = 165,
		V_CMP_GE_I64 = 166,
		V_CMP_T_I64 = 167,
		V_CMP_CLASS_F64 = 168,  // D = IEEE numeric class function specified in S1.u, performed on S0.d.
		V_CMP_LT_U16 = 169,
		V_CMP_EQ_U16 = 170,
		V_CMP_LE_U16 = 171,
		V_CMP_GT_U16 = 172,
		V_CMP_NE_U16 = 173,
		V_CMP_GE_U16 = 174,

		// Also write EXEC.
		V_CMPX_F_I64 = 176,
		V_CMPX_LT_I64 = 177,
		V_CMPX_EQ_I64 = 178,
		V_CMPX_LE_I64 = 179,
		V_CMPX_GT_I64 = 180,
		V_CMPX_NE_I64 = 181,
		V_CMPX_GE_I64 = 182,
		V_CMPX_T_I64 = 183,
		V_CMPX_CLASS_F64 = 184,  // D = IEEE numeric class function specified in S1.u, performed on S0.d. Also write EXEC.
		V_CMPX_LT_U16 = 185,
		V_CMPX_EQ_U16 = 186,
		V_CMPX_LE_U16 = 187,
		V_CMPX_GT_U16 = 188,
		V_CMPX_NE_U16 = 189,
		V_CMPX_GE_U16 = 190,

		// On unsigned 32-bit intergers.
		V_CMP_F_U32 = 192,
		V_CMP_LT_U32 = 193,
		V_CMP_EQ_U32 = 194,
		V_CMP_LE_U32 = 195,
		V_CMP_GT_U32 = 196,
		V_CMP_NE_U32 = 197,
		V_CMP_GE_U32 = 198,
		V_CMP_T_U32 = 199,
		V_CMP_F_F16 = 200,
		V_CMP_LT_F16 = 201,
		V_CMP_EQ_F16 = 202,
		V_CMP_LE_F16 = 203,
		V_CMP_GT_F16 = 204,
		V_CMP_LG_F16 = 205,
		V_CMP_GE_F16 = 206,
		V_CMP_O_F16 = 207,

		// Also write EXEC.
		V_CMPX_F_U32 = 208,
		V_CMPX_LT_U32 = 209,
		V_CMPX_EQ_U32 = 210,
		V_CMPX_LE_U32 = 211,
		V_CMPX_GT_U32 = 212,
		V_CMPX_NE_U32 = 213,
		V_CMPX_GE_U32 = 214,
		V_CMPX_T_U32 = 215,
		V_CMPX_F_F16 = 216,
		V_CMPX_LT_F16 = 217,
		V_CMPX_EQ_F16 = 218,
		V_CMPX_LE_F16 = 219,
		V_CMPX_GT_F16 = 220,
		V_CMPX_LG_F16 = 221,
		V_CMPX_GE_F16 = 222,
		V_CMPX_O_F16 = 223,

		// On unsigned 64-bit integers.
		V_CMP_F_U64 = 224,
		V_CMP_LT_U64 = 225,
		V_CMP_EQ_U64 = 226,
		V_CMP_LE_U64 = 227,
		V_CMP_GT_U64 = 228,
		V_CMP_NE_U64 = 229,
		V_CMP_GE_U64 = 230,
		V_CMP_T_U64 = 231,
		V_CMP_U_F16 = 232,
		V_CMP_NGE_F16 = 233,
		V_CMP_NLG_F16 = 234,
		V_CMP_NGT_F16 = 235,
		V_CMP_NLE_F16 = 236,
		V_CMP_NEQ_F16 = 237,
		V_CMP_NLT_F16 = 238,
		V_CMP_TRU_F16 = 239,

		// Also write EXEC.
		V_CMPX_F_U64 = 240,
		V_CMPX_LT_U64 = 241,
		V_CMPX_EQ_U64 = 242,
		V_CMPX_LE_U64 = 243,
		V_CMPX_GT_U64 = 244,
		V_CMPX_NE_U64 = 245,
		V_CMPX_GE_U64 = 246,
		V_CMPX_T_U64 = 247,
		V_CMPX_U_F16 = 248,
		V_CMPX_NGE_F16 = 249,
		V_CMPX_NLG_F16 = 250,
		V_CMPX_NGT_F16 = 251,
		V_CMPX_NLE_F16 = 252,
		V_CMPX_NEQ_F16 = 253,
		V_CMPX_NLT_F16 = 254,
		V_CMPX_TRU_F16 = 255,

		VOPC_UNDEFINE = 999,
    };

    /// -----------------------------------------------------------------------------------------------
    SIVOPCInstruction(SRC src0, VSRC vsrc1, 
		unsigned int ridx0, unsigned int vsrc1Ridx,
		unsigned int instructionWidth, VOPC_OP op, 
		Encoding instructionEncoding, 
		InstructionClass insClass, 
		OperandType operandType = TypeNone) :
		VOPInstruction(src0, 0, ridx0, 0, instructionWidth, InstructionSet_VOPC, insClass, operandType),
		m_op(op), 
		m_vsrc1(vsrc1),
		m_vridx1(vsrc1Ridx),
		m_instructionEncoding(instructionEncoding) { }
    /// -----------------------------------------------------------------------------------------------

    ~SIVOPCInstruction() {}

    /// Get the OP
    VOPC_OP GetOp() const { return m_op; }

	VSRC GetVSRC1() const
	{
		return m_vsrc1;
	}

	unsigned int GetVRidx1() const
	{
		return m_vridx1;
	}

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

    int GetInstructionClockCount()
    {
        int iRet = 4;

        switch (m_op)
        {
			case V_CMP_F_F32:
			case V_CMP_LT_F32:
			case V_CMP_EQ_F32:
			case V_CMP_LE_F32:
			case V_CMP_GT_F32:
			case V_CMP_LG_F32:
			case V_CMP_GE_F32:
			case V_CMP_O_F32:
			case V_CMP_U_F32:
			case V_CMP_NGE_F32:
			case V_CMP_NLG_F32:
			case V_CMP_NGT_F32:
			case V_CMP_NLE_F32:
			case V_CMP_NEQ_F32:
			case V_CMP_NLT_F32:
			case V_CMP_TRU_F32:
			case V_CMPX_F_F32:
			case V_CMPX_LT_F32:
			case V_CMPX_EQ_F32:
			case V_CMPX_LE_F32:
			case V_CMPX_GT_F32:
			case V_CMPX_LG_F32:
			case V_CMPX_GE_F32:
			case V_CMPX_O_F32:
			case V_CMPX_U_F32:
			case V_CMPX_NGE_F32:
			case V_CMPX_NLG_F32:
			case V_CMPX_NGT_F32:
			case V_CMPX_NLE_F32:
			case V_CMPX_NEQ_F32:
			case V_CMPX_NLT_F32:
			case V_CMPX_TRU_F32:
			case V_CMPS_F_F32:
			case V_CMPS_LT_F32:
			case V_CMPS_EQ_F32:
			case V_CMPS_LE_F32:
			case V_CMPS_GT_F32:
			case V_CMPS_LG_F32:
			case V_CMPS_GE_F32:
			case V_CMPS_O_F32:
			case V_CMPS_U_F32:
			case V_CMPS_NGE_F32:
			case V_CMPS_NLG_F32:
			case V_CMPS_NGT_F32:
			case V_CMPS_NLE_F32:
			case V_CMPS_NEQ_F32:
			case V_CMPS_NLT_F32:
			case V_CMPS_TRU_F32:
			case V_CMPSX_F_F32:
			case V_CMPSX_LT_F32:
			case V_CMPSX_EQ_F32:
			case V_CMPSX_LE_F32:
			case V_CMPSX_GT_F32:
			case V_CMPSX_LG_F32:
			case V_CMPSX_GE_F32:
			case V_CMPSX_O_F32:
			case V_CMPSX_U_F32:
			case V_CMPSX_NGE_F32:
			case V_CMPSX_NLG_F32:
			case V_CMPSX_NGT_F32:
			case V_CMPSX_NLE_F32:
			case V_CMPSX_NEQ_F32:
			case V_CMPSX_NLT_F32:
			case V_CMPSX_TRU_F32:
			case V_CMP_F_I32:
			case V_CMP_LT_I32:
			case V_CMP_EQ_I32:
			case V_CMP_LE_I32:
			case V_CMP_GT_I32:
			case V_CMP_NE_I32:
			case V_CMP_GE_I32:
			case V_CMP_T_I32:
			case V_CMP_CLASS_F32:
			case V_CMPX_F_I32:
			case V_CMPX_LT_I32:
			case V_CMPX_EQ_I32:
			case V_CMPX_LE_I32:
			case V_CMPX_GT_I32:
			case V_CMPX_NE_I32:
			case V_CMPX_GE_I32:
			case V_CMPX_T_I32:
			case V_CMPX_CLASS_F32:
			case V_CMP_F_U32:
			case V_CMP_LT_U32:
			case V_CMP_EQ_U32:
			case V_CMP_LE_U32:
			case V_CMP_GT_U32:
			case V_CMP_NE_U32:
			case V_CMP_GE_U32:
			case V_CMP_T_U32:
			case V_CMPX_F_U32:
			case V_CMPX_LT_U32:
			case V_CMPX_EQ_U32:
			case V_CMPX_LE_U32:
			case V_CMPX_GT_U32:
			case V_CMPX_NE_U32:
			case V_CMPX_GE_U32:
			case V_CMPX_T_U32:
                iRet = 4;
                break;

			case V_CMP_F_F64:
			case V_CMP_LT_F64:
			case V_CMP_EQ_F64:
			case V_CMP_LE_F64:
			case V_CMP_GT_F64:
			case V_CMP_LG_F64:
			case V_CMP_GE_F64:
			case V_CMP_O_F64:
			case V_CMP_U_F64:
			case V_CMP_NGE_F64:
			case V_CMP_NLG_F64:
			case V_CMP_NGT_F64:
			case V_CMP_NLE_F64:
			case V_CMP_NEQ_F64:
			case V_CMP_NLT_F64:
			case V_CMP_TRU_F64:
			case V_CMPX_F_F64:
			case V_CMPX_LT_F64:
			case V_CMPX_EQ_F64:
			case V_CMPX_LE_F64:
			case V_CMPX_GT_F64:
			case V_CMPX_LG_F64:
			case V_CMPX_GE_F64:
			case V_CMPX_O_F64:
			case V_CMPX_U_F64:
			case V_CMPX_NGE_F64:
			case V_CMPX_NLG_F64:
			case V_CMPX_NGT_F64:
			case V_CMPX_NLE_F64:
			case V_CMPX_NEQ_F64:
			case V_CMPX_NLT_F64:
			case V_CMPX_TRU_F64:
			case V_CMPS_F_F64:
			case V_CMPS_LT_F64:
			case V_CMPS_EQ_F64:
			case V_CMPS_LE_F64:
			case V_CMPS_GT_F64:
			case V_CMPS_LG_F64:
			case V_CMPS_GE_F64:
			case V_CMPS_O_F64:
			case V_CMPS_U_F64:
			case V_CMPS_NGE_F64:
			case V_CMPS_NLG_F64:
			case V_CMPS_NGT_F64:
			case V_CMPS_NLE_F64:
			case V_CMPS_NEQ_F64:
			case V_CMPS_NLT_F64:
			case V_CMPS_TRU_F64:
			case V_CMPSX_F_F64:
			case V_CMPSX_LT_F64:
			case V_CMPSX_EQ_F64:
			case V_CMPSX_LE_F64:
			case V_CMPSX_GT_F64:
			case V_CMPSX_LG_F64:
			case V_CMPSX_GE_F64:
			case V_CMPSX_O_F64:
			case V_CMPSX_U_F64:
			case V_CMPSX_NGE_F64:
			case V_CMPSX_NLG_F64:
			case V_CMPSX_NGT_F64:
			case V_CMPSX_NLE_F64:
			case V_CMPSX_NEQ_F64:
			case V_CMPSX_NLT_F64:
			case V_CMPSX_TRU_F64:
			case V_CMP_F_I64:
			case V_CMP_LT_I64:
			case V_CMP_EQ_I64:
			case V_CMP_LE_I64:
			case V_CMP_GT_I64:
			case V_CMP_NE_I64:
			case V_CMP_GE_I64:
			case V_CMP_T_I64:
			case V_CMP_CLASS_F64:
			case V_CMPX_F_I64:
			case V_CMPX_LT_I64:
			case V_CMPX_EQ_I64:
			case V_CMPX_LE_I64:
			case V_CMPX_GT_I64:
			case V_CMPX_NE_I64:
			case V_CMPX_GE_I64:
			case V_CMPX_T_I64:
			case V_CMPX_CLASS_F64:
			case V_CMP_F_U64:
			case V_CMP_LT_U64:
			case V_CMP_EQ_U64:
			case V_CMP_LE_U64:
			case V_CMP_GT_U64:
			case V_CMP_NE_U64:
			case V_CMP_GE_U64:
			case V_CMP_T_U64:
			case V_CMPX_F_U64:
			case V_CMPX_LT_U64:
			case V_CMPX_EQ_U64:
			case V_CMPX_LE_U64:
			case V_CMPX_GT_U64:
			case V_CMPX_NE_U64:
			case V_CMPX_GE_U64:
			case V_CMPX_T_U64:
                iRet = 8;
                break;

            default:
                break;
        }

        return iRet;
    }

private:

    /// VOP operation.
    VOPC_OP m_op;

	VSRC m_vsrc1;

	unsigned int m_vridx1;

    //Instruction Type
    Encoding m_instructionEncoding;
};

class VIVOP1Instruction : public VOPInstruction
{
public:

    enum VOP1_OP
    {
        v_nop                = 0,
        v_mov_b32            = 1,
        v_readfirstlane_b32  = 2,
        v_cvt_i32_f64        = 3,
        v_cvt_f64_i32        = 4,
        v_cvt_f32_i32        = 5,
        v_cvt_f32_u32        = 6,
        v_cvt_u32_f32        = 7,
        v_cvt_i32_f32        = 8,
        v_mov_fed_b32        = 9,
        v_cvt_f16_f32        = 10,
        v_cvt_f32_f16        = 11,
        v_cvt_rpi_i32_f32    = 12,
        v_cvt_flr_i32_f32    = 13,
        v_cvt_off_f32_i4     = 14,
        v_cvt_f32_f64        = 15,
        v_cvt_f64_f32        = 16,
        v_cvt_f32_ubyte0     = 17,
        v_cvt_f32_ubyte1     = 18,
        v_cvt_f32_ubyte2     = 19,
        v_cvt_f32_ubyte3     = 20,
        v_cvt_u32_f64        = 21,
        v_cvt_f64_u32        = 22,
        v_trunc_f64          = 23,
        v_ceil_f64           = 24,
        v_rndne_f64          = 25,
        v_floor_f64          = 26,
        v_fract_f32          = 27,
        v_trunc_f32          = 28,
        v_ceil_f32           = 29,
        v_rndne_f32          = 30,
        v_floor_f32          = 31,
        v_exp_f32            = 32,
        v_log_f32            = 33,
        v_rcp_f32            = 34,
        v_rcp_iflag_f32      = 35,
        v_rsq_f32            = 36,
        v_rcp_f64            = 37,
        v_rsq_f64            = 38,
        v_sqrt_f32           = 39,
        v_sqrt_f64           = 40,
        v_sin_f32            = 41,
        v_cos_f32            = 42,
        v_not_b32            = 43,
        v_bfrev_b32          = 44,
        v_ffbh_u32           = 45,
        v_ffbl_b32           = 46,
        v_ffbh_i32           = 47,
        v_frexp_exp_i32_f64  = 48,
        v_frexp_mant_f64     = 49,
        v_fract_f64          = 50,
        v_frexp_exp_i32_f32  = 51,
        v_frexp_mant_f32     = 52,
        v_clrexcp            = 53,
        v_cvt_f16_u16        = 57,
        v_cvt_f16_i16        = 58,
        v_cvt_u16_f16        = 59,
        v_cvt_i16_f16        = 60,
        v_rcp_f16            = 61,
        v_sqrt_f16           = 62,
        v_rsq_f16            = 63,
        v_log_f16            = 64,
        v_exp_f16            = 65,
        v_frexp_mant_f16     = 66,
        v_frexp_exp_i16_f16  = 67,
        v_floor_f16          = 68,
        v_ceil_f16           = 69,
        v_trunc_f16          = 70,
        v_rndne_f16          = 71,
        v_fract_f16          = 72,
        v_sin_f16            = 73,
        v_cos_f16            = 74,
        v_exp_legacy_f32     = 75,
        v_log_legacy_f32     = 76,
        /// Illegal
        v_ILLEGAL            = 77,
    };


    /// -----------------------------------------------------------------------------------------------
    VIVOP1Instruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP1_OP op) : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP1), m_op(op), m_instructionEncoding(instructionEncoding) { }
    /// -----------------------------------------------------------------------------------------------

    ~VIVOP1Instruction() {}

    /// Get the OP
    VOP1_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

private:

    /// VOP operation.
    VOP1_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;

    int GetInstructionClockCount()
    {
        int iRet = 4;
        return iRet;
    }
};

class VIVOP2Instruction : public VOPInstruction
{
public:
    enum VOP2_OP
    {
        v_cndmask_b32      = 0,
        v_add_f32          = 1,
        v_sub_f32          = 2,
        v_subrev_f32       = 3,
        v_mul_legacy_f32   = 4,
        v_mul_f32          = 5,
        v_mul_i32_i24      = 6,
        v_mul_hi_i32_i24   = 7,
        v_mul_u32_u24      = 8,
        v_mul_hi_u32_u24   = 9,
        v_min_f32          = 10,
        v_max_f32          = 11,
        v_min_i32          = 12,
        v_max_i32          = 13,
        v_min_u32          = 14,
        v_max_u32          = 15,
        v_lshrrev_b32      = 16,
        v_ashrrev_i32      = 17,
        v_lshlrev_b32      = 18,
        v_and_b32          = 19,
        v_or_b32           = 20,
        v_xor_b32          = 21,
        v_mac_f32          = 22,
        v_madmk_f32        = 23,
        v_madak_f32        = 24,
        v_add_u32          = 25,
        v_sub_u32          = 26,
        v_subrev_u32       = 27,
        v_addc_u32         = 28,
        v_subb_u32         = 29,
        v_subbrev_u32      = 30,
        v_add_f16          = 31,
        v_sub_f16          = 32,
        v_subrev_f16       = 33,
        v_mul_f16          = 34,
        v_mac_f16          = 35,
        v_madmk_f16        = 36,
        v_madak_f16        = 37,
        v_add_u16          = 38,
        v_sub_u16          = 39,
        v_subrev_u16       = 40,
        v_mul_lo_u16       = 41,
        v_lshlrev_b16      = 42,
        v_lshrrev_b16      = 43,
        v_ashrrev_i16      = 44,
        v_max_f16          = 45,
        v_min_f16          = 46,
        v_max_u16          = 47,
        v_max_i16          = 48,
        v_min_u16          = 49,
        v_min_i16          = 50,
        v_ldexp_f16        = 51,
        /// Illegal
        v_ILLEGAL          = 52,
    };


    /// -----------------------------------------------------------------------------------------------
    VIVOP2Instruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP2_OP op) : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP2), m_op(op), m_instructionEncoding(instructionEncoding)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }
    /// -----------------------------------------------------------------------------------------------

    ~VIVOP2Instruction() {}

    /// Get the OP
    VOP2_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

    int GetInstructionClockCount()
    {
        int iRet = 4;
        return iRet;
    }

private:

    /// VOP operation.
    VOP2_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;
};

class VIVOP3Instruction : public VOPInstruction
{
public:

    enum VOP3_OP
    {
        v_mad_legacy_f32      = 448,
        v_mad_f32             = 449,
        v_mad_i32_i24         = 450,
        v_mad_u32_u24         = 451,
        v_cubeid_f32          = 452,
        v_cubesc_f32          = 453,
        v_cubetc_f32          = 454,
        v_cubema_f32          = 455,
        v_bfe_u32             = 456,
        v_bfe_i32             = 457,
        v_bfi_b32             = 458,
        v_fma_f32             = 459,
        v_fma_f64             = 460,
        v_lerp_u8             = 461,
        v_alignbit_b32        = 462,
        v_alignbyte_b32       = 463,
        v_min3_f32            = 464,
        v_min3_i32            = 465,
        v_min3_u32            = 466,
        v_max3_f32            = 467,
        v_max3_i32            = 468,
        v_max3_u32            = 469,
        v_med3_f32            = 470,
        v_med3_i32            = 471,
        v_med3_u32            = 472,
        v_sad_u8              = 473,
        v_sad_hi_u8           = 474,
        v_sad_u16             = 475,
        v_sad_u32             = 476,
        v_cvt_pk_u8_f32       = 477,
        v_div_fixup_f32       = 478,
        v_div_fixup_f64       = 479,
        v_div_scale_f32       = 480,
        v_div_scale_f64       = 481,
        v_div_fmas_f32        = 482,
        v_div_fmas_f64        = 483,
        v_msad_u8             = 484,
        v_qsad_pk_u16_u8      = 485,
        v_mqsad_pk_u16_u8     = 486,
        v_mqsad_u32_u8        = 487,
        v_mad_u64_u32         = 488,
        v_mad_i64_i32         = 489,
        v_mad_f16             = 490,
        v_mad_u16             = 491,
        v_mad_i16             = 492,
        v_perm_b32            = 493,
        v_fma_f16             = 494,
        v_div_fixup_f16       = 495,
        v_cvt_pkaccum_u8_f32  = 496,
        v_interp_p1ll_f16     = 628,
        v_interp_p1lv_f16     = 629,
        v_interp_p2_f16       = 630,
        v_add_f64             = 640,
        v_mul_f64             = 641,
        v_min_f64             = 642,
        v_max_f64             = 643,
        v_ldexp_f64          = 644,
        v_mul_lo_u32         = 645,
        v_mul_hi_u32         = 646,
        v_mul_hi_i32         = 647,
        v_ldexp_f32          = 648,
        v_readlane_b32       = 649,
        v_writelane_b32      = 650,
        v_bcnt_u32_b32       = 651,
        v_mbcnt_lo_u32_b32   = 652,
        v_mbcnt_hi_u32_b32   = 653,
        v_lshlrev_b64        = 655,
        v_lshrrev_b64        = 656,
        v_ashrrev_i64        = 657,
        v_trig_preop_f64     = 658,
        v_bfm_b32            = 659,
        v_cvt_pknorm_i16_f32 = 660,
        v_cvt_pknorm_u16_f32 = 661,
        v_cvt_pkrtz_f16_f32  = 662,
        v_cvt_pk_u16_u32     = 663,
        v_cvt_pk_i16_i32     = 664,
        /// Illegal
        v_ILLEGAL            = 665,

    } ;// end VOP3

    /// -----------------------------------------------------------------------------------------------
    VIVOP3Instruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP3_OP op) : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP3), m_op(op), m_instructionEncoding(instructionEncoding)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }
    /// -----------------------------------------------------------------------------------------------

    ~VIVOP3Instruction() {}

    /// Get the OP
    VOP3_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

private:

    /// VOP operation.
    VOP3_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;

    int GetInstructionClockCount()
    {
        int iRet = 4;
        return iRet;
    }
};

class VIVOPCInstruction : public VOPInstruction
{
public:

    enum VOPC_OP // Single Vector Compare Operations
    {
        v_cmp_class_f32     = 16,
        v_cmpx_class_f32    = 17,
        v_cmp_class_f64     = 18,
        v_cmpx_class_f64    = 19,
        v_cmp_class_f16     = 20,
        v_cmpx_class_f16    = 21,
        v_cmp_f_f16         = 32,
        v_cmp_lt_f16        = 33,
        v_cmp_eq_f16        = 34,
        v_cmp_le_f16        = 35,
        v_cmp_gt_f16        = 36,
        v_cmp_lg_f16        = 37,
        v_cmp_ge_f16        = 38,
        v_cmp_o_f16         = 39,
        v_cmp_u_f16         = 40,
        v_cmp_nge_f16       = 41,
        v_cmp_nlg_f16       = 42,
        v_cmp_ngt_f16       = 43,
        v_cmp_nle_f16       = 44,
        v_cmp_neq_f16       = 45,
        v_cmp_nlt_f16       = 46,
        v_cmp_tru_f16       = 47,
        v_cmpx_f_f16        = 48,
        v_cmpx_lt_f16       = 49,
        v_cmpx_eq_f16       = 50,
        v_cmpx_le_f16       = 51,
        v_cmpx_gt_f16       = 52,
        v_cmpx_lg_f16       = 53,
        v_cmpx_ge_f16       = 54,
        v_cmpx_o_f16        = 55,
        v_cmpx_u_f16        = 56,
        v_cmpx_nge_f16      = 57,
        v_cmpx_nlg_f16      = 58,
        v_cmpx_ngt_f16      = 59,
        v_cmpx_nle_f16      = 60,
        v_cmpx_neq_f16      = 61,
        v_cmpx_nlt_f16      = 62,
        v_cmpx_tru_f16      = 63,
        v_cmp_f_f32         = 64,
        v_cmp_lt_f32        = 65,
        v_cmp_eq_f32        = 66,
        v_cmp_le_f32        = 67,
        v_cmp_gt_f32        = 68,
        v_cmp_lg_f32        = 69,
        v_cmp_ge_f32        = 70,
        v_cmp_o_f32         = 71,
        v_cmp_u_f32         = 72,
        v_cmp_nge_f32       = 73,
        v_cmp_nlg_f32       = 74,
        v_cmp_ngt_f32       = 75,
        v_cmp_nle_f32       = 76,
        v_cmp_neq_f32       = 77,
        v_cmp_nlt_f32       = 78,
        v_cmp_tru_f32       = 79,
        v_cmpx_f_f32        = 80,
        v_cmpx_lt_f32       = 81,
        v_cmpx_eq_f32       = 82,
        v_cmpx_le_f32       = 83,
        v_cmpx_gt_f32       = 84,
        v_cmpx_lg_f32       = 85,
        v_cmpx_ge_f32       = 86,
        v_cmpx_o_f32        = 87,
        v_cmpx_u_f32        = 88,
        v_cmpx_nge_f32      = 89,
        v_cmpx_nlg_f32      = 90,
        v_cmpx_ngt_f32      = 91,
        v_cmpx_nle_f32      = 92,
        v_cmpx_neq_f32      = 93,
        v_cmpx_nlt_f32      = 94,
        v_cmpx_tru_f32      = 95,
        v_cmp_f_f64         = 96,
        v_cmp_lt_f64        = 97,
        v_cmp_eq_f64        = 98,
        v_cmp_le_f64        = 99,
        v_cmp_gt_f64        = 100,
        v_cmp_lg_f64        = 101,
        v_cmp_ge_f64        = 102,
        v_cmp_o_f64         = 103,
        v_cmp_u_f64         = 104,
        v_cmp_nge_f64       = 105,
        v_cmp_nlg_f64       = 106,
        v_cmp_ngt_f64       = 107,
        v_cmp_nle_f64       = 108,
        v_cmp_neq_f64       = 109,
        v_cmp_nlt_f64       = 110,
        v_cmp_tru_f64       = 111,
        v_cmpx_f_f64        = 112,
        v_cmpx_lt_f64       = 113,
        v_cmpx_eq_f64       = 114,
        v_cmpx_le_f64       = 115,
        v_cmpx_gt_f64       = 116,
        v_cmpx_lg_f64       = 117,
        v_cmpx_ge_f64       = 118,
        v_cmpx_o_f64        = 119,
        v_cmpx_u_f64        = 120,
        v_cmpx_nge_f64      = 121,
        v_cmpx_nlg_f64      = 122,
        v_cmpx_ngt_f64      = 123,
        v_cmpx_nle_f64      = 124,
        v_cmpx_neq_f64      = 125,
        v_cmpx_nlt_f64      = 126,
        v_cmpx_tru_f64      = 127,
        v_cmp_f_i16         = 160,
        v_cmp_lt_i16        = 161,
        v_cmp_eq_i16        = 162,
        v_cmp_le_i16        = 163,
        v_cmp_gt_i16        = 164,
        v_cmp_ne_i16        = 165,
        v_cmp_ge_i16        = 166,
        v_cmp_t_i16         = 167,
        v_cmp_f_u16         = 168,
        v_cmp_lt_u16        = 169,
        v_cmp_eq_u16        = 170,
        v_cmp_le_u16        = 171,
        v_cmp_gt_u16        = 172,
        v_cmp_ne_u16        = 173,
        v_cmp_ge_u16        = 174,
        v_cmp_t_u16         = 175,
        v_cmpx_f_i16        = 176,
        v_cmpx_lt_i16       = 177,
        v_cmpx_eq_i16       = 178,
        v_cmpx_le_i16       = 179,
        v_cmpx_gt_i16       = 180,
        v_cmpx_ne_i16       = 181,
        v_cmpx_ge_i16       = 182,
        v_cmpx_t_i16        = 183,
        v_cmpx_f_u16        = 184,
        v_cmpx_lt_u16       = 185,
        v_cmpx_eq_u16       = 186,
        v_cmpx_le_u16       = 187,
        v_cmpx_gt_u16       = 188,
        v_cmpx_ne_u16       = 189,
        v_cmpx_ge_u16       = 190,
        v_cmpx_t_u16        = 191,
        v_cmp_f_i32         = 192,
        v_cmp_lt_i32        = 193,
        v_cmp_eq_i32        = 194,
        v_cmp_le_i32        = 195,
        v_cmp_gt_i32        = 196,
        v_cmp_ne_i32        = 197,
        v_cmp_ge_i32        = 198,
        v_cmp_t_i32         = 199,
        v_cmp_f_u32         = 200,
        v_cmp_lt_u32        = 201,
        v_cmp_eq_u32        = 202,
        v_cmp_le_u32        = 203,
        v_cmp_gt_u32        = 204,
        v_cmp_ne_u32        = 205,
        v_cmp_ge_u32        = 206,
        v_cmp_t_u32         = 207,
        v_cmpx_f_i32        = 208,
        v_cmpx_lt_i32       = 209,
        v_cmpx_eq_i32       = 210,
        v_cmpx_le_i32       = 211,
        v_cmpx_gt_i32       = 212,
        v_cmpx_ne_i32       = 213,
        v_cmpx_ge_i32       = 214,
        v_cmpx_t_i32        = 215,
        v_cmpx_f_u32        = 216,
        v_cmpx_lt_u32       = 217,
        v_cmpx_eq_u32       = 218,
        v_cmpx_le_u32       = 219,
        v_cmpx_gt_u32       = 220,
        v_cmpx_ne_u32       = 221,
        v_cmpx_ge_u32       = 222,
        v_cmpx_t_u32        = 223,
        v_cmp_f_i64         = 224,
        v_cmp_lt_i64        = 225,
        v_cmp_eq_i64        = 226,
        v_cmp_le_i64        = 227,
        v_cmp_gt_i64        = 228,
        v_cmp_ne_i64        = 229,
        v_cmp_ge_i64        = 230,
        v_cmp_t_i64         = 231,
        v_cmp_f_u64         = 232,
        v_cmp_lt_u64        = 233,
        v_cmp_eq_u64        = 234,
        v_cmp_le_u64        = 235,
        v_cmp_gt_u64        = 236,
        v_cmp_ne_u64        = 237,
        v_cmp_ge_u64        = 238,
        v_cmp_t_u64         = 239,
        v_cmpx_f_i64        = 240,
        v_cmpx_lt_i64       = 241,
        v_cmpx_eq_i64       = 242,
        v_cmpx_le_i64       = 243,
        v_cmpx_gt_i64       = 244,
        v_cmpx_ne_i64       = 245,
        v_cmpx_ge_i64       = 246,
        v_cmpx_t_i64        = 247,
        v_cmpx_f_u64        = 248,
        v_cmpx_lt_u64       = 249,
        v_cmpx_eq_u64       = 250,
        v_cmpx_le_u64       = 251,
        v_cmpx_gt_u64       = 252,
        v_cmpx_ne_u64       = 253,
        v_cmpx_ge_u64       = 254,
        v_cmpx_t_u64        = 255,
        /// Illegal
        v_ILLEGAL           = 256,
    };

    /// -----------------------------------------------------------------------------------------------
    VIVOPCInstruction(unsigned int instructionWidth, Encoding instructionEncoding, VOPC_OP op) : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOPC), m_op(op), m_instructionEncoding(instructionEncoding)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }
    /// -----------------------------------------------------------------------------------------------

    ~VIVOPCInstruction() {}

    /// Get the OP
    VOPC_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

    int GetInstructionClockCount()
    {
        int iRet = 4;
        return iRet;
    }

private:

    /// VOP operation.
    VOPC_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;
};


// VOP1 instructions for VEGA (GFX9)
class G9VOP1Instruction : public VOPInstruction
{
public:

    enum VOP1_OP
    {
        v_nop                = 0,
        v_mov_b32            = 1,
        v_readfirstlane_b32  = 2,
        v_cvt_i32_f64        = 3,
        v_cvt_f64_i32        = 4,
        v_cvt_f32_i32        = 5,
        v_cvt_f32_u32        = 6,
        v_cvt_u32_f32        = 7,
        v_cvt_i32_f32        = 8,
        v_mov_fed_b32        = 9,
        v_cvt_f16_f32        = 10,
        v_cvt_f32_f16        = 11,
        v_cvt_rpi_i32_f32    = 12,
        v_cvt_flr_i32_f32    = 13,
        v_cvt_off_f32_i4     = 14,
        v_cvt_f32_f64        = 15,
        v_cvt_f64_f32        = 16,
        v_cvt_f32_ubyte0     = 17,
        v_cvt_f32_ubyte1     = 18,
        v_cvt_f32_ubyte2     = 19,
        v_cvt_f32_ubyte3     = 20,
        v_cvt_u32_f64        = 21,
        v_cvt_f64_u32        = 22,
        v_trunc_f64          = 23,
        v_ceil_f64           = 24,
        v_rndne_f64          = 25,
        v_floor_f64          = 26,
        v_fract_f32          = 27,
        v_trunc_f32          = 28,
        v_ceil_f32           = 29,
        v_rndne_f32          = 30,
        v_floor_f32          = 31,
        v_exp_f32            = 32,
        v_log_f32            = 33,
        v_rcp_f32            = 34,
        v_rcp_iflag_f32      = 35,
        v_rsq_f32            = 36,
        v_rcp_f64            = 37,
        v_rsq_f64            = 38,
        v_sqrt_f32           = 39,
        v_sqrt_f64           = 40,
        v_sin_f32            = 41,
        v_cos_f32            = 42,
        v_not_b32            = 43,
        v_bfrev_b32          = 44,
        v_ffbh_u32           = 45,
        v_ffbl_b32           = 46,
        v_ffbh_i32           = 47,
        v_frexp_exp_i32_f64  = 48,
        v_frexp_mant_f64     = 49,
        v_fract_f64          = 50,
        v_frexp_exp_i32_f32  = 51,
        v_frexp_mant_f32     = 52,
        v_clrexcp            = 53,
        v_screen_partition_4se_b32 = 55,
        v_cvt_f16_u16        = 57,
        v_cvt_f16_i16        = 58,
        v_cvt_u16_f16        = 59,
        v_cvt_i16_f16        = 60,
        v_rcp_f16            = 61,
        v_sqrt_f16           = 62,
        v_rsq_f16            = 63,
        v_log_f16            = 64,
        v_exp_f16            = 65,
        v_frexp_mant_f16     = 66,
        v_frexp_exp_i16_f16  = 67,
        v_floor_f16          = 68,
        v_ceil_f16           = 69,
        v_trunc_f16          = 70,
        v_rndne_f16          = 71,
        v_fract_f16          = 72,
        v_sin_f16            = 73,
        v_cos_f16            = 74,
        v_exp_legacy_f32     = 75,
        v_log_legacy_f32     = 76,
        v_cvt_norm_i16_f16   = 77,
        v_cvt_norm_u16_f16   = 78,
        v_sat_pk_u8_i16      = 79,
        v_swap_b32           = 81,
        /// Illegal
        v_ILLEGAL            = 82,
    };


    /// -----------------------------------------------------------------------------------------------
    G9VOP1Instruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP1_OP op)
        : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP1), m_op(op), m_instructionEncoding(instructionEncoding) { }
    /// -----------------------------------------------------------------------------------------------

    ~G9VOP1Instruction() {}

    /// Get the OP
    VOP1_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

private:

    /// VOP operation.
    VOP1_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;

//    int GetInstructionClockCount()
//    {
//        int iRet = 4;
//        return iRet;
//    }
};


// VOP2 instructions for VEGA (GFX9)
class G9VOP2Instruction : public VOPInstruction
{
public:
    enum VOP2_OP
    {
        v_cndmask_b32      = 0,
        v_add_f32          = 1,
        v_sub_f32          = 2,
        v_subrev_f32       = 3,
        v_mul_legacy_f32   = 4,
        v_mul_f32          = 5,
        v_mul_i32_i24      = 6,
        v_mul_hi_i32_i24   = 7,
        v_mul_u32_u24      = 8,
        v_mul_hi_u32_u24   = 9,
        v_min_f32          = 10,
        v_max_f32          = 11,
        v_min_i32          = 12,
        v_max_i32          = 13,
        v_min_u32          = 14,
        v_max_u32          = 15,
        v_lshrrev_b32      = 16,
        v_ashrrev_i32      = 17,
        v_lshlrev_b32      = 18,
        v_and_b32          = 19,
        v_or_b32           = 20,
        v_xor_b32          = 21,
        v_mac_f32          = 22,
        v_madmk_f32        = 23,
        v_madak_f32        = 24,
        v_add_co_u32       = 25,
        v_sub_co_u32       = 26,
        v_subrev_co_u32    = 27,
        v_addc_co_u32      = 28,
        v_subb_co_u32      = 29,
        v_subbrev_co_u32   = 30,
        v_add_f16          = 31,
        v_sub_f16          = 32,
        v_subrev_f16       = 33,
        v_mul_f16          = 34,
        v_mac_f16          = 35,
        v_madmk_f16        = 36,
        v_madak_f16        = 37,
        v_add_u16          = 38,
        v_sub_u16          = 39,
        v_subrev_u16       = 40,
        v_mul_lo_u16       = 41,
        v_lshlrev_b16      = 42,
        v_lshrrev_b16      = 43,
        v_ashrrev_i16      = 44,
        v_max_f16          = 45,
        v_min_f16          = 46,
        v_max_u16          = 47,
        v_max_i16          = 48,
        v_min_u16          = 49,
        v_min_i16          = 50,
        v_ldexp_f16        = 51,
        v_add_u32          = 52,
        v_sub_u32          = 53,
        v_subrev_u32       = 54,
        /// Illegal
        v_ILLEGAL          = 55
    };


    /// -----------------------------------------------------------------------------------------------
    G9VOP2Instruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP2_OP op)
        : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP2), m_op(op), m_instructionEncoding(instructionEncoding)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }
    /// -----------------------------------------------------------------------------------------------

    ~G9VOP2Instruction() {}

    /// Get the OP
    VOP2_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

    int GetInstructionClockCount()
    {
        int iRet = 4;
        return iRet;
    }

private:

    /// VOP operation.
    VOP2_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;
};


// VOP3 for VEGA (GFX9)
class G9VOP3Instruction : public VOPInstruction
{
public:

    enum VOP3_OP
    {
        v_mad_legacy_f32      = 448,
        v_mad_f32             = 449,
        v_mad_i32_i24         = 450,
        v_mad_u32_u24         = 451,
        v_cubeid_f32          = 452,
        v_cubesc_f32          = 453,
        v_cubetc_f32          = 454,
        v_cubema_f32          = 455,
        v_bfe_u32             = 456,
        v_bfe_i32             = 457,
        v_bfi_b32             = 458,
        v_fma_f32             = 459,
        v_fma_f64             = 460,
        v_lerp_u8             = 461,
        v_alignbit_b32        = 462,
        v_alignbyte_b32       = 463,
        v_min3_f32            = 464,
        v_min3_i32            = 465,
        v_min3_u32            = 466,
        v_max3_f32            = 467,
        v_max3_i32            = 468,
        v_max3_u32            = 469,
        v_med3_f32            = 470,
        v_med3_i32            = 471,
        v_med3_u32            = 472,
        v_sad_u8              = 473,
        v_sad_hi_u8           = 474,
        v_sad_u16             = 475,
        v_sad_u32             = 476,
        v_cvt_pk_u8_f32       = 477,
        v_div_fixup_f32       = 478,
        v_div_fixup_f64       = 479,
        v_div_scale_f32       = 480,
        v_div_scale_f64       = 481,
        v_div_fmas_f32        = 482,
        v_div_fmas_f64        = 483,
        v_msad_u8             = 484,
        v_qsad_pk_u16_u8      = 485,
        v_mqsad_pk_u16_u8     = 486,
        v_mqsad_u32_u8        = 487,
        v_mad_u64_u32         = 488,
        v_mad_i64_i32         = 489,
        v_mad_legacy_f16      = 490,
        v_mad_legacy_u16      = 491,
        v_mad_legacy_i16      = 492,
        v_perm_b32            = 493,
        v_fma_legacy_f16      = 494,
        v_div_fixup_legacy_f16  = 495,
        v_cvt_pkaccum_u8_f32  = 496,
        v_mad_u32_u16         = 497,
        v_mad_i32_i16         = 498,
        v_xad_u32             = 499,
        v_min3_f16            = 500,
        v_min3_i16            = 501,
        v_min3_u16            = 502,
        v_max3_f16            = 503,
        v_max3_i16            = 504,
        v_max3_u16            = 505,
        v_med3_f16            = 506,
        v_med3_i16            = 507,
        v_med3_u16            = 508,
        v_lshl_add_b32        = 509,
        v_add_lshl_b32        = 510,
        v_add3_u32            = 511,
        v_lshl_or_b32         = 512,
        v_and_or_b32          = 513,
        v_or3_b32             = 514,
        v_mad_f16             = 515,
        v_mad_u16             = 516,
        v_mad_i16             = 517,
        v_fma_f16             = 518,
        v_div_fixup_f16       = 519,
        v_interp_p1_f32       = 624,
        v_interp_p2_f32       = 625,
        v_interp_mov_f32      = 626,
        v_interp_p1ll_f16     = 628,
        v_interp_p1lv_f16     = 629,
        v_interp_p2_legacy_f16  = 630,
        v_interp_p2_f16       = 631,
        v_add_f64             = 640,
        v_mul_f64             = 641,
        v_min_f64             = 642,
        v_max_f64             = 643,
        v_ldexp_f64          = 644,
        v_mul_lo_u32         = 645,
        v_mul_hi_u32         = 646,
        v_mul_hi_i32         = 647,
        v_ldexp_f32          = 648,
        v_readlane_b32       = 649,
        v_writelane_b32      = 650,
        v_bcnt_u32_b32       = 651,
        v_mbcnt_lo_u32_b32   = 652,
        v_mbcnt_hi_u32_b32   = 653,
        v_lshlrev_b64        = 655,
        v_lshrrev_b64        = 656,
        v_ashrrev_i64        = 657,
        v_trig_preop_f64     = 658,
        v_bfm_b32            = 659,
        v_cvt_pknorm_i16_f32 = 660,
        v_cvt_pknorm_u16_f32 = 661,
        v_cvt_pkrtz_f16_f32  = 662,
        v_cvt_pk_u16_u32     = 663,
        v_cvt_pk_i16_i32     = 664,
        v_cvt_pknorm_i16_f16 = 665,
        v_cvt_pknorm_u16_f16 = 666,
        v_readlane_regrd_b32 = 667,
        v_add_i32            = 668,
        v_sub_i32            = 669,
        v_add_i16            = 670,
        v_sub_i16            = 671,
        v_pack_b32_f16       = 672,
        /// Illegal
        v_ILLEGAL            = 673
    } ;// end VOP3

    /// -----------------------------------------------------------------------------------------------
    G9VOP3Instruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP3_OP op)
        : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP3), m_op(op), m_instructionEncoding(instructionEncoding)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }
    /// -----------------------------------------------------------------------------------------------

    ~G9VOP3Instruction() {}

    /// Get the OP
    VOP3_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

private:

    /// VOP operation.
    VOP3_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;
};


// VOP3 for VEGA (GFX9)
class G9VOP3PInstruction : public VOPInstruction
{
public:

    enum VOP3P_OP
    {
        v_pk_mad_i16         = 0,
        v_pk_mul_lo_u16      = 1,
        v_pk_add_i16         = 2,
        v_pk_sub_i16         = 3,
        v_pk_lshlrev_b16_i16 = 4,
        v_pk_lshrrev_b16_i16 = 5,
        v_pk_ashrrev_b16_i16 = 6,
        v_pk_max_i16         = 7,
        v_pk_min_i16         = 8,
        v_pk_mad_u16         = 9,
        v_pk_add_u16         = 10,
        v_pk_sub_u16         = 11,
        v_pk_max_u16         = 12,
        v_pk_min_u16         = 13,
        v_pk_fma_f16         = 14,
        v_pk_add_f16         = 15,
        v_pk_mul_f16         = 16,
        v_pk_min_f16         = 17,
        v_pk_max_f16         = 18,
        v_mad_mix_f32        = 32,
        v_mad_mixlo_f16      = 33,
        v_mad_mixhi_f15      = 34,
        /// Illegal
        v_ILLEGAL            = 35
    } ;// end VOP3P

    /// -----------------------------------------------------------------------------------------------
    G9VOP3PInstruction(unsigned int instructionWidth, Encoding instructionEncoding, VOP3P_OP op)
        : VOPInstruction((SRC)0, (VDST)0, 0, 0, instructionWidth, InstructionSet_VOP3), m_op(op), m_instructionEncoding(instructionEncoding)
    {
        m_HwGen = GDT_HW_GENERATION_GFX9;
    }
    /// -----------------------------------------------------------------------------------------------

    ~G9VOP3PInstruction() {}

    /// Get the OP
    VOP3P_OP GetOp() const { return m_op; }

    // return the instruction encoding
    Encoding GetInstructionType() const { return m_instructionEncoding; }

private:

    /// VOP operation.
    VOP3P_OP m_op;

    //Instruction Type
    Encoding m_instructionEncoding;
};


#endif //__VOPINSTRUCTION_H

