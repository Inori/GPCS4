/*
 *  CLRadeonExtender - Unofficial OpenCL Radeon Extensions Library
 *  Copyright (C) 2014-2018 Mateusz Szpakowski
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __CLRX_GCNINTERNALS_H__
#define __CLRX_GCNINTERNALS_H__

#include <CLRX/Config.h>
#include <cstdint>
#include <string>
#include <CLRX/utils/Utilities.h>
#include <CLRX/utils/GPUId.h>

namespace CLRX
{

// enums for GCN encodings
enum : cxbyte
{
    GCNENC_NONE,
    GCNENC_SOPC,    /* 0x17e<<23, opcode = (7bit)<<16 */
    GCNENC_SOPP,    /* 0x17f<<23, opcode = (7bit)<<16 */
    GCNENC_SOP1,    /* 0x17d<<23, opcode = (8bit)<<8 */
    GCNENC_SOP2,    /* 0x2<<30,   opcode = (7bit)<<23 */
    GCNENC_SOPK,    /* 0xb<<28,   opcode = (5bit)<<23 */
    GCNENC_SMRD,    /* 0x18<<27,  opcode = (6bit)<<22 */
    GCNENC_SMEM = GCNENC_SMRD,    /* 0x18<<27,  opcode = (6bit)<<22 */
    GCNENC_VOPC,    /* 0x3e<<25,  opcode = (8bit)<<17 */
    GCNENC_VOP1,    /* 0x3f<<25,  opcode = (8bit)<<9 */
    GCNENC_VOP2,    /* 0x0<<31,   opcode = (6bit)<<25 */
    GCNENC_VOP3A,   /* 0x34<<26,  opcode = (9bit)<<17 */
    GCNENC_VOP3B,   /* 0x34<<26,  opcode = (9bit)<<17 */
    GCNENC_VINTRP,  /* 0x32<<26,  opcode = (2bit)<<16 */
    GCNENC_DS,      /* 0x36<<26,  opcode = (8bit)<<18 */
    GCNENC_MUBUF,   /* 0x38<<26,  opcode = (7bit)<<18 */
    GCNENC_MTBUF,   /* 0x3a<<26,  opcode = (3bit)<<16 */
    GCNENC_MIMG,    /* 0x3c<<26,  opcode = (7bit)<<18 */
    GCNENC_EXP,     /* 0x3e<<26,  opcode = none */
    GCNENC_FLAT,    /* 0x37<<26,  opcode = (8bit)<<18 (???8bit) */
    GCNENC_VOP3P,
    GCNENC_MAXVAL = GCNENC_FLAT
};


typedef uint32_t GCNInsnMode;

// modes for GCN instructions
enum : GCNInsnMode
{
    GCN_STDMODE = 0,    /// standard mode
    GCN_REG_ALL_64 = 15,    /// all register operand is 64-bit
    GCN_REG_DST_64 = 1,     /// destination is 64-bit
    GCN_REG_SRC0_64 = 2,    /// source0 is 64-bit
    GCN_REG_SRC1_64 = 4,    /// source1 is 64-bit
    GCN_REG_SRC2_64 = 8,    /// source2 is 64-bit
    GCN_REG_DS0_64 = 3,     /// destination and source0 is 64-bit
    GCN_REG_DS1_64 = 5,     /// destination and source1 is 64-bit
    GCN_REG_DS2_64 = 9,     /// destination and source2 is 64-bit
    /* SOP */
    GCN_IMM_NONE = 0x10, // used in Scall insns
    GCN_ARG_NONE = 0x20,    /// no arguments (operands)
    GCN_DST_NONE = 0x20,  /// omit destination argument
    GCN_IMM_REL = 0x30,     /// SOPK, immediate as relative address
    GCN_IMM_LOCKS = 0x40,   /// SOPK, s_waitcnt locks
    GCN_IMM_MSGS = 0x50,    /// SOPK, s_sendmsg* message function
    GCN_IMM_SREG = 0x60,    /// SOPK, hwreg function
    GCN_SRC_NONE = 0x70,    /// SOP1, omit source
    GCN_DST_SRC = 0x80,     /// SOPK, SDST is first source
    GCN_IMM_DST = 0x100,    /// immediate is first (destplace), destination is second
    GCN_SOPK_CONST = 0x200, /// constant instead destination
    GCN_SOPK_SRIMM32 = 0x300, /// GCN_IMM_DST and GCN_SOPK_CONST together
    /* SOPC */
    GCN_SRC1_IMM = 0x10,    /// treat source1 as immediate value
    /* VOP */
    GCN_SRC2_NONE = 0x10,   /// omit source2
    GCN_DS2_VCC = 0x20,     /// extra sgpr or VCC in extra destination and in source2
    GCN_SRC12_NONE = 0x30,  /// omit source1 and source2
    GCN_ARG1_IMM = 0x40,    /// immediate value as second operand
    GCN_ARG2_IMM = 0x50,    /// immediate value as third operand
    GCN_S0EQS12 = 0x60,     /// source0 must be equal to source1 or source2
    GCN_DST_VCC = 0x70,     /// extra sgpr or VCC in extra destination
    GCN_SRC2_VCC = 0x80,    /// sgpr or VCC in source2
    GCN_DST_VCC_VSRC2 = 0x90,   /// sgpr or VCC as dest operand and source2
    GCN_DS1_SGPR = 0xa0,    /// destination and source1 is SGPR
    GCN_SRC1_SGPR = 0xb0,   /// source1 is SGPR
    GCN_DST_SGPR = 0xc0,    /// destination is SGPR
    GCN_VOP_ARG_NONE = 0xd0,    /// no argument for VOP encodings
    GCN_NEW_OPCODE = 0xe0,  /// unique opcode that doesn't exists in VINTRP
    GCN_P0_P10_P20 = 0xf0,  /// second operand is param (P0,P10,P20)
    GCN_VOPC_NOVCC = 0x80,  /// no VCC in VOPC instruction
    GCN_VOP3_VOP2 = 0x100,  /// VOP2 encoded as VOP3
    GCN_VOP3_VOP1 = 0x200,  /// VOP1 encoded as VOP3
    GCN_VOP3_VINTRP = 0x300,    /// VINTRP encoded as VOP3
    GCN_VOP3_DS2_128 = 0x7000,   /// VOP3 with DST 128 and SRC2 128
    GCN_VOP3_VINTRP_NEW = 0x3e0,    /// new VINTRP instructions encoded as VOP3 
    GCN_VOP3_VOP2_DS01 = 0x110, /// VOP2 in VOP3, destination and two sources
    GCN_VOP3_VOP1_DS0 = 0x230,  /// VOP1 in VOP3, destination and one source
    GCN_VOP3_DST_SGPR = 0x400,  /// VOPX in VOP3, and dst is SGPR (if mask1 used)
    GCN_VOP3_SRC1_SGPR = 0x800,   /// VOPX in VOP3, and src1 is SGPR (if mask1 used)
    GCN_VOP3_DS1_SGPR = 0xc00, /// VOPX in VOP3, and dst and src1 is SGPR (if mask1 used)
    GCN_VOP3_MASK2 = 0x8300,    // mask for VOPx in VOP2 encodings
    GCN_VINTRP_SRC2 = 0x1000,   /// VOP3/VINTRP with source2 (third source)
    GCN_VOP3_MASK3 = 0x3000,    /// mask for VINTRP in VOP2 encodings
    GCN_VOP3_VOP3P = 0x8000,    /// VOP3P encoding
    GCN_VOP3_NODST = 0x1000000,  /// VOP3 - no DST
    GCN_VOP_NODPP = 0x10000,    /// VOP instruction can not have DPP
    GCN_VOP_NOSDWA = 0x20000,    /// VOP instruction can not have SDWA
    GCN_VOP_NODPPSDWA = 0x30000,    /// VOP instruction can not have DPP and SDWA
    GCN_VOP_NOSDWAVEGA = 0x40000,    /// VOP instruction can not have SDWA for VEGA
    GCN_VCC_IMPL_READ = 0x80000,    /// VOP instruction with implicit VCC read
    GCN_VCC_IMPL_WRITE = 0x100000,   /// VOP instruction with implicit VCC write
    GCN_VOP_NOWVSZ = 0x200000,      // VOP SDST/VCC size doesn't not depend on wavesize
    // DS encoding modes
    GCN_ADDR_STD = 0x0,    /// standard place of address
    GCN_ADDR_DST = 0x10,    /// address operand in destination place
    GCN_ADDR_SRC = 0x20,    /// address operand in source
    GCN_ADDR_DST64 = 0x1f,  /// address operand in destination place and all is 64-bit
    GCN_ADDR_SRC64 = 0x2f,  /// address operand in source and all is 64-bit
    GCN_ADDR_SRC_D64 = 0x21,    /// address operand in source and dest is bit (not addr)
    GCN_2OFFSETS = 0x100,  /// two 8-bit offsets
    GCN_VDATA2 = 0x140, /* two datas, two offsets */
    GCN_NOSRC = 0x80, /* only address, no source */
    GCN_2SRCS = 0x40, // two (source or dest) datas (DATA), DATA1)
    GCN_NOSRC_2OFF = 0x180, /* only address */
    GCN_SRC_ADDR2  = 0x200, /// for ds_XXX_src2_XXX instructions
    GCN_SRC_ADDR2_64  = 0x20f, /// for ds_XXX_src2_XXX instructions, all is 64-bit
    GCN_DS_96  = 0x800, /// 96-bit dest and source
    GCN_DS_128  = 0x1000,    /// 128-bit dest and source
    GCN_ONLYDST = 0x400, /* only vdst */
    GCN_DSMASK = 0x3f0, // mask for DS encoding
    GCN_DSMASK2 = 0x3c0,     // mask for DS encoding 2
    GCN_SRCS_MASK = 0xc0,   /// only srcs mask
    GCN_ONLYGDS = 0x2000,   /// instruction must have GDS
    GCN_DST128 = 0x4000,   // dest is 128-bit
    GCN_ONLY_SRC = 0x8000,
    // others
    GCN_SBASE4 = 0x10,  /// SBASE requires 4 registers
    GCN_FLOATLIT = 0x40000000U,   /// float literal
    GCN_F16LIT = 0x80000000U, /// half literal
    GCN_LITMASK = 0xc0000000U,
    GCN_SMRD_ONLYDST = 0x30,    // only destination (no other operands)
    GCN_SMEM_SDATA_IMM = 0x40,  // treat SDATA as immediate
    GCN_SMEM_NOSDATA = 0x80,  // no destination
    GCN_MEMOP_MX1 = 0x0,    /// sdst/sdata requires 1 register
    GCN_MEMOP_MX2 = 0x100,     /// sdst/sdata requires 2 registers
    GCN_MEMOP_MX4 = 0x200,     /// sdst/sdata requires 4 registers
    GCN_MEMOP_MX8 = 0x300,     /// sdst/sdata requires 8 registers
    GCN_MEMOP_MX16 = 0x400,    /// sdst/sdata requires 16 registers
    GCN_MUBUF_X = 0x0,      /// vdata requires 1 register
    GCN_MUBUF_NOVAD = 0x10, // no vaddr and vdata
    GCN_MUBUF_XY = 0x100,   /// vdata requires 2 registers
    GCN_MUBUF_XYZ = 0x200,   /// vdata requires 3 registers
    GCN_MUBUF_XYZW = 0x300,   /// vdata requires 4 registers
    GCN_MUBUF_MX1 = 0x0,    /// vdata requires 1 register
    GCN_MUBUF_MX2 = 0x100,  /// vdata requires 2 registers
    GCN_MUBUF_MX3 = 0x200,  /// vdata requires 3 registers
    GCN_MUBUF_MX4 = 0x300,  /// vdata requires 4 registers
    GCN_MUBUF_D16 = 0x800,  /// vdata size depends on arch (GCN 1.4 - half of size)
    GCN_MUBUF_X_D16 = 0x800,    /// vdata requires 1 register
    GCN_MUBUF_XY_D16 = 0x900,  /// vdata requires 2 registers (or 1 for GCN 1.4)
    GCN_MUBUF_XYZ_D16 = 0xa00,  /// vdata requires 3 registers (or 2 for GCN 1.4)
    GCN_MUBUF_XYZW_D16 = 0xb00,  /// vdata requires 4 registers (or 2 for GCN 1.4)
    GCN_MIMG_SAMPLE = 0x100,    /// last operand is SSAMP (4 sregisters)
    GCN_MIMG_VDATA4 = 0x200,    /// gather requires 4 vdata registers
    GCN_MIMG_GATHER = 0x300,    /// gather requires 4 vdata registers
    GCN_MIMG_VAGE1 = 0x0,    /// vaddr requires 1 or more registers
    GCN_MIMG_VAGE2 = 0x1,  /// vaddr requires 2 or more registers
    GCN_MIMG_VAGE3 = 0x2,  /// vaddr requires 3 or more registers
    GCN_MIMG_VAGE4 = 0x3,  /// vaddr requires 4 or more registers
    GCN_MIMG_VAGE5 = 0x4,  /// vaddr requires 5 or more registers
    GCN_MIMG_VAGE6 = 0x5,  /// vaddr requires 6 or more registers
    GCN_MIMG_VADERIV = 0x10,  /// vaddr holds user derivatives
    GCN_MIMG_VAGE2D = 0x11,  /// vaddr requires 2 or more registers and holds user derivs
    GCN_MIMG_VAGE3D = 0x12,  /// vaddr requires 3 or more registers and holds user derivs
    GCN_MIMG_VAGE4D = 0x13,  /// vaddr requires 4 or more registers and holds user derivs
    GCN_MIMG_VAGE5D = 0x14,  /// vaddr requires 5 or more registers and holds user derivs
    GCN_MIMG_VAGE6D = 0x15,  /// vaddr requires 6 or more registers and holds user derivs
    GCN_MIMG_VA_O = 0x20,  // vaddr *O*
    GCN_MIMG_VA_B = 0x40,  // vaddr *B*
    GCN_MIMG_VA_C = 0x80,  // vaddr *C*
    GCN_MIMG_VA_L = 0x400,  // vaddr *L*
    GCN_MIMG_VA_CL = 0x800,  // vaddr *CL*
    GCN_MIMG_VA_B_O = GCN_MIMG_VA_B|GCN_MIMG_VA_O, // vaddr *B* and *O*
    GCN_MIMG_VA_B_CL = GCN_MIMG_VA_B|GCN_MIMG_VA_CL, // vaddr *B* and *CL*
     // vaddr *B* and *CL* and *O*
    GCN_MIMG_VA_B_CL_O = GCN_MIMG_VA_B|GCN_MIMG_VA_CL|GCN_MIMG_VA_O,
    GCN_MIMG_VA_C_B = GCN_MIMG_VA_C|GCN_MIMG_VA_B,  // vaddr *C* and *B*
    // vaddr *C* and *B* and *CL*
    GCN_MIMG_VA_C_B_CL = GCN_MIMG_VA_C|GCN_MIMG_VA_B|GCN_MIMG_VA_CL,
    GCN_MIMG_VA_L_O = GCN_MIMG_VA_L|GCN_MIMG_VA_O,  // vaddr *L* anc *O*
    GCN_MIMG_VA_C_L = GCN_MIMG_VA_C|GCN_MIMG_VA_L,  // vaddr *C* and *L*
    GCN_MIMG_VA_C_CL = GCN_MIMG_VA_C|GCN_MIMG_VA_CL,  // vaddr *C* and *CLL*
    GCN_MIMG_VA_C_O = GCN_MIMG_VA_C|GCN_MIMG_VA_O,  // vaddr *C* and *O*
    GCN_MIMG_VA_CL_O = GCN_MIMG_VA_CL|GCN_MIMG_VA_O,  // vaddr *CL* and *O*
    // vaddr *C* and *CL* and *O*
    GCN_MIMG_VA_C_CL_O = GCN_MIMG_VA_C|GCN_MIMG_VA_CL|GCN_MIMG_VA_O,
    // vaddr *C* and *CL* and *B* and *O*
    GCN_MIMG_VA_C_B_CL_O = GCN_MIMG_VA_C|GCN_MIMG_VA_CL|GCN_MIMG_VA_B|GCN_MIMG_VA_O,
    // vaddr *C* and *L* and *O*
    GCN_MIMG_VA_C_L_O = GCN_MIMG_VA_C|GCN_MIMG_VA_L|GCN_MIMG_VA_O,
    // vaddr *C* and *B* and *O*
    GCN_MIMG_VA_C_B_O = GCN_MIMG_VA_C|GCN_MIMG_VA_B|GCN_MIMG_VA_O,
    GCN_MIMG_VA_MIP = 0x10000,  // vaddr _MIP
    GCN_MIMG_VA_MASK = 0xf,
    GCN_MLOAD = 0x1000, // instruction load data to vgprs
    GCN_MATOMIC = 0x2000, // instruction perform atomics and returns data if glc==1
    GCN_MHALFWRITE = 0x4000,
    GCN_MCMPSWAP = 0x6000,
    GCN_FLAT_DDST = 0x00,   // destination as first operand
    GCN_FLAT_ADST = 0x10,   /// first address, second is DST
    GCN_FLAT_NODATA = 0x20, /// omit DATA
    GCN_FLAT_NODST = 0x40,  /// omit DST
    GCN_FLAT_STORE = 0x50,  /// store instruction
    GCN_CMPSWAP =  0x80,    /// ???
    GCN_ACMPSWAP =  0x6080,    /// ???
    GCN_FLAT_FLAT = 0,
    GCN_FLAT_SCRATCH = 1,
    GCN_FLAT_GLOBAL = 2,
    GCN_FLAT_MODEMASK = 7,
    GCN_MASK1 = 0xf0,
    GCN_MASK2 = 0xf00,
    GCN_DSIZE_MASK = 0x700, /// dsize mask
    GCN_SHIFT2 = 8
};

struct CLRX_INTERNAL GCNInstruction
{
    const char* mnemonic;
    cxbyte encoding;
    GCNInsnMode mode;
    uint16_t code;
    GPUArchMask archMask; // mask of architectures whose have instruction
};

// version GCNInstruction for assembler (with two code: for VOPX and VOP3)
struct CLRX_INTERNAL GCNAsmInstruction
{
    const char* mnemonic;
    cxbyte encoding;
    GCNInsnMode mode;
    uint16_t code1, code2; // code1 - first code, code2 - VOP3 encoding code
    GPUArchMask archMask; // mask of architectures whose have instruction
};

CLRX_INTERNAL extern const GCNInstruction gcnInstrsTable[];

};

#endif
