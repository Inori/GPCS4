#pragma once

#include "GnmCommon.h"


namespace pssl
{;
struct VsStageRegisters;
struct PsStageRegisters;
struct CsStageRegisters;
}  // namespace pssl

/// We may use these meta information to initialize
/// some shader properties and specialization constants


struct GnmShaderMetaVs
{
	uint32_t userSgprCount;
};

struct GnmShaderMetaPs
{
	uint32_t userSgprCount;
};

struct GnmShaderMetaCs
{
	uint32_t userSgprCount;

	uint32_t threadGroupX;
	uint32_t threadGroupY;
	uint32_t threadGroupZ;
};


namespace shader
{;
	
void parseShaderRegVs(const pssl::VsStageRegisters* reg, GnmShaderMetaVs& meta);

void parseShaderRegPs(const pssl::PsStageRegisters* reg, GnmShaderMetaPs& meta);

void parseShaderRegCs(const pssl::CsStageRegisters* reg, GnmShaderMetaCs& meta);

}  // namespace shader
