#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "../Gve/GveShader.h"
#include "PsslCompiler.h"
#include "PsslDecoder.h"
#include <optional>

namespace pssl
{;

class PsslShaderModule
{
public:
	PsslShaderModule(uint32_t* code);
	PsslShaderModule(uint32_t* code, uint32_t* fsCode);
	~PsslShaderModule();

	RcPtr<gve::GveShader> compile(uint32_t* code);

private:
	void initModule(uint32_t* code);
	void runCompiler(PsslCompiler& compiler, PsslCodeSlice slice);
private:

	std::optional<uint32_t*> m_fsCode;
};



}
