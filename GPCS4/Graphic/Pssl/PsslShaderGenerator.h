#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "../Gve/GveShader.h"
#include "GCNCompiler.h"
#include "GCNDecoder.h"

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
	void runCompiler(GCNCompiler& compiler, GCNCodeSlice slice);
	void decodeFetchShader(GCNCodeSlice slice, PsslFetchShader& fsShader);

private:


};



}
