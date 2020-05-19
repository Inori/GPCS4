#pragma once

#include "GnmCommon.h"
#include "../Pssl/PsslShaderStructure.h"

namespace pssl
{;
struct VsStageRegisters;
struct PsStageRegisters;
struct CsStageRegisters;
}  // namespace pssl


namespace shader
{;
	
void parseShaderRegVs(const pssl::VsStageRegisters* reg, pssl::PsslShaderMetaVs& meta);

void parseShaderRegPs(const pssl::PsStageRegisters* reg, pssl::PsslShaderMetaPs& meta);

void parseShaderRegCs(const pssl::CsStageRegisters* reg, pssl::PsslShaderMetaCs& meta);

}  // namespace shader
