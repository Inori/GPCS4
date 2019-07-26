#pragma once

#include "GPCS4Common.h"
#include "PsslShaderStructure.h"
#include "PsslShaderInstruction.h"
#include <vector>

namespace pssl 
{;

struct PsslFetchShader
{
	uint32_t m_codeLengthDw;  // shader code length excluding the final padding numInputSlots dword.
	uint32_t m_numInputSlots;
	std::vector<PsslShaderInstruction> m_instructionList;

	PsslFetchShader(const uint32_t* code);
};

}  // pssl

