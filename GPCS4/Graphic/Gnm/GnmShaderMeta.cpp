#include "GnmShaderMeta.h"

#include "../Pssl/PsslShaderRegister.h"

using namespace pssl;

namespace shader
{;

void parseShaderRegVs(const pssl::VsStageRegisters* reg, GnmShaderMetaVs& meta)
{
	const SPI_SHADER_PGM_RSRC2_VS* rsrc2 = reinterpret_cast<const SPI_SHADER_PGM_RSRC2_VS*>(&reg->spiShaderPgmRsrc2Vs);
	meta.userSgprCount                   = rsrc2->user_sgpr;
}

void parseShaderRegPs(const pssl::PsStageRegisters* reg, GnmShaderMetaPs& meta)
{
	const SPI_SHADER_PGM_RSRC2_PS* rsrc2 = reinterpret_cast<const SPI_SHADER_PGM_RSRC2_PS*>(&reg->spiShaderPgmRsrc2Ps);
	meta.userSgprCount                   = rsrc2->user_sgpr;
}

void parseShaderRegCs(const pssl::CsStageRegisters* reg, GnmShaderMetaCs& meta)
{
	const COMPUTE_PGM_RSRC2* rsrc2 = reinterpret_cast<const COMPUTE_PGM_RSRC2*>(&reg->computePgmRsrc2);
	meta.userSgprCount             = rsrc2->user_sgpr;
	meta.threadGroupX              = reg->computeNumThreadX;
	meta.threadGroupY              = reg->computeNumThreadY;
	meta.threadGroupZ              = reg->computeNumThreadZ;
}

}  // namespace shader
