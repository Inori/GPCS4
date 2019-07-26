#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "../Gve/GveShader.h"
#include "PsslCompiler.h"
#include "PsslDecoder.h"

namespace pssl
{;

class PsslShaderGenerator
{
public:
	PsslShaderGenerator();
	~PsslShaderGenerator();

	RcPtr<gve::GveShader> compile(const uint32_t* code);

	RcPtr<gve::GveShader> compile(const uint32_t* code, const uint32_t* fsCode);

private:
	void runCompiler(PsslCompiler& compiler, PsslCodeSlice slice);
	void decodeFetchShader(PsslCodeSlice slice, PsslFetchShader& fsShader);

private:


};



}
