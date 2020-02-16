#pragma once

#include "PsslCommon.h"
#include "PsslProgramInfo.h"
#include "PsslShaderStructure.h"
#include "GCNDecoder.h"

namespace gve
{;
class GveShader;
}  // namespace gve

namespace pssl
{;

struct PsslFetchShader;

class GCNCompiler;
class GCNAnalyzer;


class PsslShaderModule
{
public:
	PsslShaderModule(const uint32_t* code);

	~PsslShaderModule();

	std::vector<VertexInputSemantic> vsInputSemantic();

	std::vector<InputUsageSlot> inputUsageSlots();

	void defineFetchShader(const uint32_t* fsCode);

	void defineShaderInput(const std::vector<PsslShaderResource>& shaderInputTab);

	const GcnShaderResources& getShaderResources();

	PsslKey key();

	RcPtr<gve::GveShader> compile();

	static std::vector<GcnShaderResourceInstance> 
		linearlizeShaderResources(const GcnShaderResources& nestedResources);

private:

	void runAnalyzer(GCNAnalyzer& analyzer, GCNCodeSlice slice);

	void runCompiler(GCNCompiler& compiler, GCNCodeSlice slice);

	// Fetch Shader parsing functions
	void parseFetchShader(const uint32_t* fsCode);
	void decodeFetchShader(GCNCodeSlice slice, PsslFetchShader& fsShader);
	void extractInputSemantic(PsslFetchShader& fsShader);

	const void* findShaderResourceInUserData(uint32_t startRegister);
	const void* findShaderResourceInEUD(uint32_t eudOffsetInDword);
	const uint32_t* findShaderResourceByType(ShaderInputUsageType usageType);

	bool parseShaderInput();
	void parseResImm();
	void parseResEud();
	void parseResPtrTable();
	bool checkUnhandledRes();

	// Debug only
	void dumpShader(PsslProgramType type, const uint8_t* code, uint32_t size);
private:
	const uint32_t* m_code;

	PsslProgramInfo m_progInfo;

	std::vector<VertexInputSemantic> m_vsInputSemantic;

	// Shader input backup received from the game.
	std::vector<PsslShaderResource> m_shaderInputTable;

	// shader input contains SRT, EUD and other Table type resources,
	// we need to parse the shader input slots 
	// and extract these resource definitions from the tables.
	GcnShaderResources m_shaderResources;

	const uint32_t* m_eudTable       = nullptr;

	static const uint8_t m_shaderResourceSizeInDwords[kShaderInputUsageImmDispatchDrawInstances + 1];
};



}
