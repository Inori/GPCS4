#include "GnmShaderMeta.h"

#include "../Pssl/PsslShaderRegister.h"

using namespace pssl;

namespace shader
{;

// A complete PSSL shader binary file
// output by PSSL shader compiler directly
// contains some meta data,
// including user_sgpr_count, user_sgpr_usage, ps_persp_center_en
// and resource declaration information and so on.
//
// But during shader binding, the runtime will
// rip these meta data fields out, leaving only
// the instruction field and input slot field.
// These meta data are then encoded into XXStageRegisters.
//
// Hence we need to decode these register values,
// recover the original meta info at our needs,
// e.g. to initialize shader SGPRs and VGPRs.


void parseShaderRegVs(const pssl::VsStageRegisters* reg, PsslShaderMetaVs& meta)
{
	const SPI_SHADER_PGM_RSRC2_VS* rsrc2 = reinterpret_cast<const SPI_SHADER_PGM_RSRC2_VS*>(&reg->spiShaderPgmRsrc2Vs);
	meta.userSgprCount                   = rsrc2->user_sgpr;
}

void parseShaderRegPs(const pssl::PsStageRegisters* reg, PsslShaderMetaPs& meta)
{
	const SPI_SHADER_PGM_RSRC2_PS* rsrc2 = reinterpret_cast<const SPI_SHADER_PGM_RSRC2_PS*>(&reg->spiShaderPgmRsrc2Ps);
	const SPI_PS_INPUT_ADDR*       addr  = reinterpret_cast<const SPI_PS_INPUT_ADDR*>(&reg->spiPsInputAddr);
	meta.userSgprCount                   = rsrc2->user_sgpr;
	meta.spiPsInputAddr                  = *reinterpret_cast<const uint32_t*>(addr);
}

void parseShaderRegCs(const pssl::CsStageRegisters* reg, PsslShaderMetaCs& meta)
{
	const COMPUTE_PGM_RSRC2* rsrc2 = reinterpret_cast<const COMPUTE_PGM_RSRC2*>(&reg->computePgmRsrc2);
	meta.userSgprCount             = rsrc2->user_sgpr;

	// LDS is allocated in 128 dword blocks for SEA ISLANDS AMDGPU.
	const uint32_t ldsAlignShift = 9;
	meta.ldsSize                 = rsrc2->lds_size << ldsAlignShift;

	meta.computePgmRsrc2 = reg->computePgmRsrc2;

	meta.threadGroupX              = reg->computeNumThreadX;
	meta.threadGroupY              = reg->computeNumThreadY;
	meta.threadGroupZ              = reg->computeNumThreadZ;
}

}  // namespace shader
