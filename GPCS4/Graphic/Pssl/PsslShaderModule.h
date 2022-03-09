#pragma once

#include "PsslCommon.h"
#include "PsslProgramInfo.h"
#include "PsslShaderStructure.h"
#include "GCNDecoder.h"


namespace vlt
{;
class VltShader;
}  // namespace vlt

namespace pssl
{;

struct PsslFetchShader;
class GCNCompiler;
class GCNAnalyzer;

class PsslShaderModule : public RcObject
{
public:
	PsslShaderModule(
		const PsslShaderMeta& meta,
		const uint32_t*       code);

	~PsslShaderModule();

	void defineFetchShader(const uint32_t* fsCode);

	void defineShaderInput(
		const PsslShaderResourceTable& shaderInputTab);

	const GcnShaderResourceDeclaration& getShaderResourceDeclaration();

	std::vector<VertexInputSemantic> vsInputSemantic();

	std::vector<InputUsageSlot> inputUsageSlots();

	PsslKey key();

	RcPtr<vlt::VltShader> compile();

	static std::vector<GcnShaderResourceInstance>
	flattenShaderResources(const GcnShaderResourceDeclaration& nestedResources);

private:

	void runAnalyzer(GCNAnalyzer& analyzer, GCNCodeSlice slice);

	void runCompiler(GCNCompiler& compiler, GCNCodeSlice slice);

	// Fetch Shader parsing functions
	void parseFetchShader(const uint32_t* fsCode);
	void decodeFetchShader(GCNCodeSlice slice, PsslFetchShader& fsShader);
	void extractInputSemantic(PsslFetchShader& fsShader);
	GcnUserDataRegister populateUserSgpr();

	const void* findShaderResourceInUserData(uint32_t startRegister);
	const void* findShaderResourceInEUD(uint32_t eudOffsetInDword);
	const uint32_t* findShaderResourceByType(ShaderInputUsageType usageType);

	bool parseShaderInput();
	void parseResImm();
	void parseResEud();
	void parseResPtrTable();
	bool checkUnhandledRes();

	void parseVertexInputAttribute(const InputUsageSlot& vertexSlot);

	// Debug only
	void dumpShader(PsslProgramType type, const uint8_t* code, uint32_t size);
private:
	PsslShaderMeta  m_meta;
	const uint32_t* m_code;

	PsslProgramInfo m_progInfo;

	std::vector<VertexInputSemantic> m_vsInputSemantic;

	// Shader input backup received from the game.
	PsslShaderResourceTable m_shaderInputTable;

	// shader input contains SRT, EUD and other Table type resources,
	// we need to parse the shader input slots 
	// and extract these resource definitions from the tables.
	GcnShaderResourceDeclaration m_shaderResourceDcl;

	const uint32_t* m_eudTable       = nullptr;

	static const uint8_t m_shaderResourceSizeInDwords[kShaderInputUsageImmDispatchDrawInstances + 1];
};



}
