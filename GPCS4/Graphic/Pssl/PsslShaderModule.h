#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "../Gve/GveShader.h"
#include "GCNCompiler.h"
#include "GCNDecoder.h"
#include "GCNAnalyzer.h"

namespace pssl
{;

class PsslShaderModule
{
public:
	PsslShaderModule(const uint32_t* code);
	PsslShaderModule(const uint32_t* code, const uint32_t* fsCode);
	~PsslShaderModule();

	std::vector<VertexInputSemantic> vsInputSemantic();

	std::vector<InputUsageSlot> inputUsageSlots();

	PsslKey key();

	RcPtr<gve::GveShader> compile();

private:

	RcPtr<gve::GveShader> compileWithFS();
	RcPtr<gve::GveShader> compileNoFS();

	void runAnalyzer(GCNAnalyzer& analyzer, GCNCodeSlice slice);

	void runCompiler(GCNCompiler& compiler, GCNCodeSlice slice);

	// Fetch Shader parsing functions
	void parseFetchShader(const uint32_t* fsCode);
	void decodeFetchShader(GCNCodeSlice slice, PsslFetchShader& fsShader);
	void extractInputSemantic(PsslFetchShader& fsShader);

	// Debug only
	void dumpShader(PsslProgramType type, const uint8_t* code, uint32_t size);
private:
	const uint32_t* m_code;

	PsslProgramInfo m_progInfo;

	std::vector<VertexInputSemantic> m_vsInputSemantic;
};



}
