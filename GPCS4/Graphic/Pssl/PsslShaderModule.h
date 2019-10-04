#pragma once

#include "GPCS4Common.h"

#include "PsslProgramInfo.h"
#include "GCNCompiler.h"
#include "GCNDecoder.h"
#include "GCNAnalyzer.h"

#include "../Gve/GveShader.h"

namespace pssl
{;



class PsslShaderModule
{
public:
	PsslShaderModule(
		const uint32_t* code,
		const std::vector<PsslShaderResource>& shaderResTab);

	PsslShaderModule(
		const uint32_t* code, 
		const uint32_t* fsCode, 
		const std::vector<PsslShaderResource>& shaderResTab);

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
	std::vector<GcnResourceBuffer> findResourceBuffers();
	bool findShaderResource(uint32_t startSlot, PsslShaderResource& outRes);

	// Debug only
	void dumpShader(PsslProgramType type, const uint8_t* code, uint32_t size);
private:
	const uint32_t* m_code;

	PsslProgramInfo m_progInfo;

	std::vector<VertexInputSemantic> m_vsInputSemantic;

	std::vector<PsslShaderResource> m_shaderResourceTable;
};



}
