#pragma once

#include "GPCS4Common.h"

namespace pssl
{;

// These structure definitions can be found in
// Radeon Sea Islands 3D/Compute 
// Register Reference Guide 


struct SPI_SHADER_PGM_RSRC2_VS
{
	uint32_t scratch_en : 1;    // This wave uses scratch space for register spilling
	uint32_t user_sgpr : 5;     // Number of USER_DATA terms that should be initialized by SPI. Range is 0-16.
	uint32_t trap_present : 1;  // Enables trap processing. Sets trap_en bit to SQ and causes SPI to alloc 16 extra SGPR and write TBA/TMA values to SGPR.
	uint32_t oc_lds_en : 1;     // Enables loading of offchip related info to SGPR. See shader pgm guide for details
	uint32_t so_base0_en : 1;   // Enables loading of streamout base0 to SGPR. See shader pgm guide for details
	uint32_t so_base1_en : 1;   // Enables loading of streamout base1 to SGPR. See shader pgm guide for details
	uint32_t so_base2_en : 1;   // Enables loading of streamout base2 to SGPR. See shader pgm guide for details
	uint32_t so_base3_en : 1;   // Enables loading of streamout base3 to SGPR. See shader pgm guide for details
	uint32_t so_en : 1;         // Enables loading of streamout buffer config to SGPR. See shader pgm guide for details
	uint32_t excp_en : 9;       // Drives excp bits in spi_sq newWave cmd
	uint32_t reserved : 10;     //
};

struct SPI_SHADER_PGM_RSRC2_PS
{
	uint32_t scratch_en : 1;      // This wave uses scratch space for register spilling
	uint32_t user_sgpr : 5;       // Number of USER_DATA terms that should be initialized by SPI. Range is 0-16.
	uint32_t trap_present : 1;    // Enables trap processing. Sets trap_en bit to SQ and causes SPI to alloc 16 extra SGPR and write TBA/TMA values to SGPR.
	uint32_t wave_cnt_en : 1;     // Causes SPI to increment a per-wave count for PS and load the counter value into an SGPR.
	uint32_t extra_lds_size : 8;  // Amount of extra LDS space (in addition to attribute space) to alloc for each PS. Granularity 128, max register setting is 127, have to make sure extra + attr space <= 16K dwords.
	uint32_t excp_en : 9;         // Drives excp bits in spi_sq newWave cmd
	uint32_t reserved : 7;        //
};

struct COMPUTE_PGM_RSRC2
{
	uint32_t scratch_en : 1;      // This wave uses scratch space for register spilling
	uint32_t user_sgpr : 5;       // Number of USER_DATA terms that should be initialized by SPI. Range is 0-16.
	uint32_t trap_present : 1;    // Enables trap processing. Sets trap_en bit to SQ and causes SPI to alloc 16 extra SGPR and write TBA/TMA values to SGPR.
	uint32_t tgid_x_en : 1;       // Enables loading of TGID.X into SGPR
	uint32_t tgid_y_en : 1;       // Enables loading of TGID.Y into SGPR
	uint32_t tgid_z_en : 1;       // Enables loading of TGID.Z into SGPR
	uint32_t tg_size_en : 1;      // Enables loading of threadgroup related info to SGPR. See shader pgm guide for details
	uint32_t tidig_comp_cnt : 2;  // Specifies how many thread_id_in_group terms to write to VGPR. 0=X, 1=XY, 2=XYZ, 3=Undefined
	uint32_t excp_en_msb : 2;     //
	uint32_t lds_size : 9;        // Amount of LDS space to alloc for each threadgroup. Granularity 128, range is 0 to 128 which allocates 0 to 16K dwords.
	uint32_t excp_en : 7;         // Drives excp bits in spi_sq new Wave cmd
	uint32_t reserved : 1;        //
};

struct CsStageRegisters
{
	uint32_t  computePgmLo;
	uint32_t  computePgmHi;
	uint32_t  computePgmRsrc1;
	uint32_t  computePgmRsrc2;
	uint32_t  computeNumThreadX;
	uint32_t  computeNumThreadY;
	uint32_t  computeNumThreadZ;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(computePgmHi) << 40 | uintptr_t(computePgmLo) << 8);
	}
};


struct VsStageRegisters
{
	uint32_t spiShaderPgmLoVs;
	uint32_t spiShaderPgmHiVs;
	uint32_t spiShaderPgmRsrc1Vs;
	uint32_t spiShaderPgmRsrc2Vs;
	uint32_t spiVsOutConfig;
	uint32_t spiShaderPosFormat;
	uint32_t paClVsOutCntl;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(spiShaderPgmHiVs) << 40 | uintptr_t(spiShaderPgmLoVs) << 8);
	}
};


struct PsStageRegisters
{
	uint32_t spiShaderPgmLoPs;
	uint32_t spiShaderPgmHiPs;
	uint32_t spiShaderPgmRsrc1Ps;
	uint32_t spiShaderPgmRsrc2Ps;
	uint32_t spiShaderZFormat;
	uint32_t spiShaderColFormat;
	uint32_t spiPsInputEna;
	uint32_t spiPsInputAddr;
	uint32_t spiPsInControl;
	uint32_t spiBarycCntl;
	uint32_t dbShaderControl;
	uint32_t cbShaderMask;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(spiShaderPgmHiPs) << 40 | uintptr_t(spiShaderPgmLoPs) << 8);
	}
};



}  // pssl