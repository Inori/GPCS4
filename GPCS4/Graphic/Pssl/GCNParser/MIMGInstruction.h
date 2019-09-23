//=============================================================
// Copyright (c) 2013 Advanced Micro Devices, Inc.
//=============================================================

#ifndef __MIMGINSTRUCTION_H
#define __MIMGINSTRUCTION_H
#include "Instruction.h"

/// Image memory buffer operations. Two words.
/// Opcode :
///        DMASK  [11:8]
///        UNORM  [12:12]
///        GLC    [13:13]
///        DA     [14:14]
///        R128   [15:15]
///        TFE    [16:16]
///        LWE    [17:17]
///        OP     [24:18]
///        SLC    [25:25]
///        VADDR  [39:32]
///        VDATA  [47:40]
///        SRSRC  [52:48]
///        SSAMP  [57:53]

class MIMGInstruction : public Instruction
{
public:

    typedef short DMASK;
    typedef char UNORM;
    typedef char GLC;
    typedef char DA;
    typedef char R128;
    typedef char TFE;
    typedef char LWE;
    typedef char SLC;
    typedef char VADDR;
    typedef char VDATA;
    typedef char SRSRC;
    typedef char SSAMP;


private:
    /// Enable mask for image read/write data components. bit0 = red, 1 = green,
    /// 2 = blue, 3 = alpha. At least one bit must be on. Data is assumed to be packed
    /// into consecutive VGPRs.
    DMASK m_dmask;

    /// IWhen set to 1, forces the address to be un-normalized, regardless of T#. Must be
    /// set to 1 for image stores and atomics
    UNORM m_unorm;


    /// If set, operation is globally coherent.
    GLC m_glc;

    /// Declare an Array.
    /// 1 Kernel has declared this resource to be an array of texture maps.
    /// 0 Kernel has declared this resource to be a single texture map.
    DA m_da;

    /// Texture resource size: 1 = 128b, 0 = 256b.
    R128 m_r128;

    /// Texture Fail Enable (for partially resident textures).
    TFE m_tfe;

    /// LOD Warning Enable (for partially resident textures).
    LWE m_lwe;

    /// System Level Coherent.
    SLC m_slc;

    /// VGPR address source. Can carry an offset or an index.
    VADDR m_vaddr;

    /// Vector GPR to read/write result to.
    VDATA m_vdata;

    /// Scalar GPR that specifies the resource constant, in units of four SGPRs.
    SRSRC m_srsrc;

    /// Scalar GPR that specifies the sampler constant, in units of four SGPRs.
    SSAMP m_ssamp;

    /// MTBUF Instruction Width in bits
    static const unsigned int MIMGInstructionWidth = 64;
public:
    //
    // Public member functions
    //

    /// ctor
    MIMGInstruction(DMASK dmask, UNORM unorm, GLC glc, DA da, R128 r128, TFE tfe, LWE lwe, SLC slc, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                    SSAMP ssamp, Instruction::InstructionCategory inst, InstructionClass insClass = InstructionClassUnknown): 
		Instruction(MIMGInstructionWidth, inst, InstructionSet_MIMG, insClass),
        m_dmask(dmask), m_unorm(unorm), m_glc(glc), m_da(da), m_r128(r128), m_tfe(tfe), m_lwe(lwe),
        m_slc(slc), m_vaddr(vaddr), m_vdata(vdata), m_srsrc(srsrc), m_ssamp(ssamp) {}

    /// dtor
    virtual ~MIMGInstruction() {}

    /// Get the DMASK  [11:8]
    DMASK GetDMASK() const { return m_dmask; }

    /// Get the UNORM  [12:12]
    UNORM GetUNORM() const { return m_unorm; }

    /// Get the GLC    [14:14]
    GLC GetGLC() const { return m_glc; }

    /// Get the  DA     [14:14]
    DA GetDA() const { return m_da; }

    /// Get the R128   [15:15]
    R128 GetR128() const { return m_r128; }

    /// Get the TFE    [16:16]
    TFE GetTFE() const { return m_tfe; }

    /// Get the LWE    [17:17]
    LWE GetLWE() const { return m_lwe; }

    /// Get the SLC    [25:25]
    SLC GetSLC() const { return m_slc; }

    /// Get the VADDR  [39:32]
    VADDR GetVADDR() const { return m_vaddr; }

    /// Get the VDATA  [47:40]
    VDATA GetVDATA() const { return m_vdata; }

    /// Get the SRSRC  [52:48]
    SRSRC GetSRSRC() const { return m_srsrc; }

    /// Get the SSAMP  [57:53]
    SSAMP GetSSAMP() const { return m_ssamp; }
};

class SIMIMGInstruction : public MIMGInstruction
{
public:
    /// Selector for the MIMG Instruction
    enum OP
    {
        ///  Image memory load with format conversion specified in T#. No
        /// sampler.
        IMAGE_LOAD,
        ///  Image memory load with user-supplied mip level. No sampler.
        IMAGE_LOAD_MIP,
        ///  Image memory load with no format conversion. No sampler.
        IMAGE_LOAD_PCK,
        ///  Image memory load with with no format conversion
        /// and sign extension. No sampler.
        IMAGE_LOAD_PCK_SGN,
        ///  Image memory load with user-supplied mip level, no
        /// format conversion. No sampler.
        IMAGE_LOAD_MIP_PCK,
        ///  Image memory load with user-supplied mip
        /// level, no format conversion and with sign extension. No sampler.
        IMAGE_LOAD_MIP_PCK_SGN,
        ///  Image memory store with format conversion specified in T#.
        /// No sampler.
        IMAGE_STORE,
        ///  Image memory store with format conversion specified in
        /// T# to user specified mip level. No sampler.
        IMAGE_STORE_MIP,
        ///  Image memory store of packed data without format conversion.
        /// No sampler.
        IMAGE_STORE_PCK,
        ///  Image memory store of packed data without format
        /// conversion to user-supplied mip level. No sampler.
        IMAGE_STORE_MIP_PCK,
        ///  return resource info. No sampler.
        IMAGE_GET_RESINFO,
        ///  dst=src, returns previous value if glc==1.
        IMAGE_ATOMIC_SWAP,
        ///  dst = (dst==cmp) ? src : dst. Returns previous
        /// value if glc==1.
        IMAGE_ATOMIC_CMPSWAP,
        ///  dst += src. Returns previous value if glc==1.
        IMAGE_ATOMIC_ADD,
        ///  dst -= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_SUB,
        ///  dst = src-dst. Returns previous value if glc==1.
        IMAGE_ATOMIC_RSUB,
        ///  dst = (src < dst) ? src : dst (signed). Returns previous
        /// value if glc==1.
        IMAGE_ATOMIC_SMIN,
        ///  dst = (src < dst) ? src : dst (unsigned). Returns previous
        /// value if glc==1.
        IMAGE_ATOMIC_UMIN,
        ///  dst = (src > dst) ? src : dst (signed). Returns previous
        /// value if glc==1.
        IMAGE_ATOMIC_SMAX,
        ///  dst = (src > dst) ? src : dst (unsigned). Returns previous
        /// value if glc==1.
        IMAGE_ATOMIC_UMAX,
        ///  dst &= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_AND,
        ///  dst |= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_OR,
        ///  dst ^= src. Returns previous value if glc==1.
        IMAGE_ATOMIC_XOR,
        ///  dst = (dst >= src) ? 0 : dst+1. Returns previous value if
        /// glc==1.
        IMAGE_ATOMIC_INC,
        ///  dst = ((dst==0 || (dst > src)) ? src : dst-1. Returns previous
        /// value if glc==1.
        IMAGE_ATOMIC_DEC,
        ///  dst = (dst == cmp) ? src : dst, returns previous
        /// value of dst if glc==1 - double and float atomic compare swap. Obeys floating
        /// point compare rules for special values.
        IMAGE_ATOMIC_FCMPSWAP,
        ///  dst = (src < dst) ? src : dst, returns previous value of
        /// dst if glc==1 - double and float atomic min (handles NaN/INF/denorm).
        IMAGE_ATOMIC_FMIN,
        ///  dst = (src > dst) ? src : dst, returns previous value of
        /// dst if glc==1 - double and float atomic min (handles NaN/INF/denorm).
        IMAGE_ATOMIC_FMAX,
        ///  sample texture map.
        IMAGE_SAMPLE,
        ///  sample texture map, with LOD clamp specified in
        /// shader.
        IMAGE_SAMPLE_CL,
        ///  sample texture map, with user derivatives.
        IMAGE_SAMPLE_D,
        ///  sample texture map, with LOD clamp specified in
        /// shader, with user derivatives.
        IMAGE_SAMPLE_D_CL,
        ///  sample texture map, with user LOD.
        IMAGE_SAMPLE_L,
        ///  sample texture map, with lod bias.
        IMAGE_SAMPLE_B,
        ///  sample texture map, with LOD clamp specified in
        /// shader, with lod bias.
        IMAGE_SAMPLE_B_CL,
        ///  sample texture map, from level 0.
        IMAGE_SAMPLE_LZ,
        ///  sample texture map, with PCF.
        IMAGE_SAMPLE_C,
        ///  SAMPLE_C, with LOD clamp specified in shader.
        IMAGE_SAMPLE_C_CL,
        ///  SAMPLE_C, with user derivatives.
        IMAGE_SAMPLE_C_D,
        ///  SAMPLE_C, with LOD clamp specified in shader,
        /// with user derivatives.
        IMAGE_SAMPLE_C_D_CL,
        ///  SAMPLE_C, with user LOD.
        IMAGE_SAMPLE_C_L,
        ///  SAMPLE_C, with lod bias.
        IMAGE_SAMPLE_C_B,
        ///  SAMPLE_C, with LOD clamp specified in shader,
        /// with lod bias.
        IMAGE_SAMPLE_C_B_CL,
        ///  SAMPLE_C, from level 0.
        IMAGE_SAMPLE_C_LZ,
        ///  sample texture map, with user offsets.
        IMAGE_SAMPLE_O,
        ///  SAMPLE_O with LOD clamp specified in shader.
        IMAGE_SAMPLE_CL_O,
        ///  SAMPLE_O, with user derivatives.
        IMAGE_SAMPLE_D_O,
        ///  SAMPLE_O, with LOD clamp specified in shader,
        /// with user derivatives.
        IMAGE_SAMPLE_D_CL_O,
        ///  SAMPLE_O, with user LOD.
        IMAGE_SAMPLE_L_O,
        ///  SAMPLE_O, with lod bias.
        IMAGE_SAMPLE_B_O,
        ///  SAMPLE_O, with LOD clamp specified in shader,
        /// with lod bias.
        IMAGE_SAMPLE_B_CL_O,
        ///  SAMPLE_O, from level 0.
        IMAGE_SAMPLE_LZ_O,
        ///  SAMPLE_C with user specified offsets.
        IMAGE_SAMPLE_C_O,
        ///  SAMPLE_C_O, with LOD clamp specified in
        /// shader.
        IMAGE_SAMPLE_C_CL_O,
        ///  SAMPLE_C_O, with user derivatives.
        IMAGE_SAMPLE_C_D_O,
        ///  SAMPLE_C_O, with LOD clamp specified in
        /// shader, with user derivatives.
        IMAGE_SAMPLE_C_D_CL_O,
        ///  SAMPLE_C_O, with user LOD.
        IMAGE_SAMPLE_C_L_O,
        ///  SAMPLE_C_O, with lod bias.
        IMAGE_SAMPLE_C_B_O,
        ///  SAMPLE_C_O, with LOD clamp specified in
        /// shader, with lod bias.
        IMAGE_SAMPLE_C_B_CL_O,
        ///  SAMPLE_C_O, from level 0.
        IMAGE_SAMPLE_C_LZ_O,
        ///  gather 4 single component elements (2x2).
        IMAGE_GATHER4,
        ///  gather 4 single component elements (2x2) with user
        /// LOD clamp.
        IMAGE_GATHER4_CL,
        ///  gather 4 single component elements (2x2) with user
        /// LOD.
        IMAGE_GATHER4_L,
        ///  gather 4 single component elements (2x2) with user
        /// bias.
        IMAGE_GATHER4_B,
        ///  gather 4 single component elements (2x2) with user
        /// bias and clamp.
        IMAGE_GATHER4_B_CL,
        ///  gather 4 single component elements (2x2) at level 0.
        IMAGE_GATHER4_LZ,
        ///  gather 4 single component elements (2x2) with PCF.
        IMAGE_GATHER4_C,
        ///  gather 4 single component elements (2x2) with user
        /// LOD clamp and PCF.
        IMAGE_GATHER4_C_CL,
        ///  gather 4 single component elements (2x2) with user
        /// LOD and PCF.
        IMAGE_GATHER4_C_L,
        ///  gather 4 single component elements (2x2) with user
        /// bias and PCF.
        IMAGE_GATHER4_C_B,
        ///  gather 4 single component elements (2x2) with
        /// user bias, clamp and PCF.
        IMAGE_GATHER4_C_B_CL,
        ///  gather 4 single component elements (2x2) at level 0,
        /// with PCF.
        IMAGE_GATHER4_C_LZ,
        ///  GATHER4, with user offsets.
        IMAGE_GATHER4_O,
        ///  GATHER4_CL, with user offsets.
        IMAGE_GATHER4_CL_O,
        ///  GATHER4_L, with user offsets.
        IMAGE_GATHER4_L_O,
        ///  GATHER4_B, with user offsets.
        IMAGE_GATHER4_B_O,
        ///  GATHER4_B_CL, with user offsets.
        IMAGE_GATHER4_B_CL_O,
        ///  GATHER4_LZ, with user offsets.
        IMAGE_GATHER4_LZ_O,
        ///  GATHER4_C, with user offsets.
        IMAGE_GATHER4_C_O,
        ///  GATHER4_C_CL, with user offsets.
        IMAGE_GATHER4_C_CL_O,
        ///  GATHER4_C_L, with user offsets.
        IMAGE_GATHER4_C_L_O,
        ///  GATHER4_B, with user offsets.
        IMAGE_GATHER4_C_B_O,
        ///  GATHER4_B_CL, with user offsets.
        IMAGE_GATHER4_C_B_CL_O,
        ///  GATHER4_C_LZ, with user offsets.
        IMAGE_GATHER4_C_LZ_O,
        ///  Return calculated LOD.
        IMAGE_GET_LOD,
        ///  sample texture map, with user derivatives (LOD per
        /// quad)
        IMAGE_SAMPLE_CD,
        ///  sample texture map, with LOD clamp specified in
        /// shader, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_CD_CL,
        ///  SAMPLE_C, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_C_CD,
        ///  SAMPLE_C, with LOD clamp specified in shader,
        /// with user derivatives (LOD per quad).
        IMAGE_SAMPLE_C_CD_CL,
        ///  SAMPLE_O, with user derivatives (LOD per quad).
        IMAGE_SAMPLE_CD_O,
        ///  SAMPLE_O, with LOD clamp specified in shader,
        /// with user derivatives (LOD per quad).
        IMAGE_SAMPLE_CD_CL_O,
        ///  SAMPLE_C_O, with user derivatives (LOD per
        /// quad).
        IMAGE_SAMPLE_C_CD_O,
        ///  SAMPLE_C_O, with LOD clamp specified in
        /// shader, with user derivatives (LOD per quad).
        /// All other values are reserved.
        IMAGE_SAMPLE_C_CD_CL_O,
        /// Reserved
        IMAGE_RESERVED
    };

    /// Get the     OP [24:18]
    OP GetOp() const { return m_op; }

    /// ctor
    SIMIMGInstruction(DMASK dmask, UNORM unorm, GLC glc, DA da, R128 r128, TFE tfe, LWE lwe, OP op, SLC slc, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                      SSAMP ssamp, Instruction::InstructionCategory inst, InstructionClass insClass):
        MIMGInstruction(dmask, unorm, glc, da, r128, tfe, lwe, slc, vaddr, vdata, srsrc, ssamp, inst, insClass), m_op(op) {}

private:
    /// MTBUF operation.
    OP m_op;

};

class VIMIMGInstruction : public MIMGInstruction
{
public:
    /// Selector for the MIMG Instruction
    enum OP
    {
        image_load              = 0,
        image_load_mip          = 1,
        image_load_pck          = 2,
        image_load_pck_sgn      = 3,
        image_load_mip_pck      = 4,
        image_load_mip_pck_sgn  = 5,
        image_store             = 8,
        image_store_mip         = 9,
        image_store_pck         = 10,
        image_store_mip_pck     = 11,
        image_get_resinfo       = 14,
        image_atomic_swap       = 16,
        image_atomic_cmpswap    = 17,
        image_atomic_add        = 18,
        image_atomic_sub        = 19,
        image_atomic_smin       = 20,
        image_atomic_umin       = 21,
        image_atomic_smax       = 22,
        image_atomic_umax       = 23,
        image_atomic_and        = 24,
        image_atomic_or         = 25,
        image_atomic_xor        = 26,
        image_atomic_inc        = 27,
        image_atomic_dec        = 28,
        image_sample            = 32,
        image_sample_cl         = 33,
        image_sample_d          = 34,
        image_sample_d_cl       = 35,
        image_sample_l          = 36,
        image_sample_b          = 37,
        image_sample_b_cl       = 38,
        image_sample_lz         = 39,
        image_sample_c          = 40,
        image_sample_c_cl       = 41,
        image_sample_c_d        = 42,
        image_sample_c_d_cl     = 43,
        image_sample_c_l        = 44,
        image_sample_c_b        = 45,
        image_sample_c_b_cl     = 46,
        image_sample_c_lz       = 47,
        image_sample_o          = 48,
        image_sample_cl_o       = 49,
        image_sample_d_o        = 50,
        image_sample_d_cl_o     = 51,
        image_sample_l_o        = 52,
        image_sample_b_o        = 53,
        image_sample_b_cl_o     = 54,
        image_sample_lz_o       = 55,
        image_sample_c_o        = 56,
        image_sample_c_cl_o     = 57,
        image_sample_c_d_o      = 58,
        image_sample_c_d_cl_o   = 59,
        image_sample_c_l_o      = 60,
        image_sample_c_b_o      = 61,
        image_sample_c_b_cl_o   = 62,
        image_sample_c_lz_o     = 63,
        image_gather4           = 64,
        image_gather4_cl        = 65,
        image_gather4_l         = 68,
        image_gather4_b         = 69,
        image_gather4_b_cl      = 70,
        image_gather4_lz        = 71,
        image_gather4_c         = 72,
        image_gather4_c_cl      = 73,
        image_gather4_c_l       = 76,
        image_gather4_c_b       = 77,
        image_gather4_c_b_cl    = 78,
        image_gather4_c_lz      = 79,
        image_gather4_o         = 80,
        image_gather4_cl_o      = 81,
        image_gather4_l_o       = 84,
        image_gather4_b_o       = 85,
        image_gather4_b_cl_o    = 86,
        image_gather4_lz_o      = 87,
        image_gather4_c_o       = 88,
        image_gather4_c_cl_o    = 89,
        image_gather4_c_l_o     = 92,
        image_gather4_c_b_o     = 93,
        image_gather4_c_b_cl_o  = 94,
        image_gather4_c_lz_o    = 95,
        image_get_lod           = 96,
        image_sample_cd         = 104,
        image_sample_cd_cl      = 105,
        image_sample_c_cd       = 106,
        image_sample_c_cd_cl    = 107,
        image_sample_cd_o       = 108,
        image_sample_cd_cl_o    = 109,
        image_sample_c_cd_o     = 110,
        image_sample_c_cd_cl_o  = 111,
        image_Ilegal = 112,
    };

    /// Get the     OP [24:18]
    OP GetOp() const { return m_op; }

    /// ctor
    VIMIMGInstruction(DMASK dmask, UNORM unorm, GLC glc, DA da, R128 r128, TFE tfe, LWE lwe, OP op, SLC slc, VADDR vaddr, VDATA vdata, SRSRC srsrc,
                      SSAMP ssamp, Instruction::InstructionCategory inst): MIMGInstruction(dmask, unorm, glc, da, r128, tfe, lwe, slc, vaddr, vdata, srsrc, ssamp, inst), m_op(op)
    {
        m_HwGen = GDT_HW_GENERATION_VOLCANICISLAND;
    }

private:
    /// MTBUF operation.
    OP m_op;

};
#endif //__MIMGINSTRUCTION_H

