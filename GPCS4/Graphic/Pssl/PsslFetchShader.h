#pragma once

#include "GPCS4Common.h"
#include "PsslShaderStructure.h"
#include <vector>

namespace pssl 
{;


class PsslFetchShader
{
private:
	enum FetchShaderToken : uint32_t
	{
		FS_TOKEN_VERTEX_BASE_USGPR = 0x4A000000,
		FS_TOKEN_INSTANCE_BASE_USGPR = 0x4A060600,
		FS_TOKEN_SEPARATOR = 0xBF8C007F,
		FS_TOKEN_END1 = 0xBF8C0000,
		FS_TOKEN_END2 = 0xBF8C0000,
	};

public:
	PsslFetchShader(const uint32_t* code, uint32_t length);
	~PsslFetchShader();

	bool getFetchShaderBuildState(FetchShaderBuildState& fsbs);

	static uint32_t getFetchShaderLengthDw(const uint32_t* code);

private:
	void extractTokens(const uint32_t* code, uint32_t length);

private:
	uint32_t m_numInputSlots;
	uint32_t m_vertexBaseToken;
	uint32_t m_instanceBaseToken;
	std::vector<uint32_t> m_semaIdxTokens;
	std::vector<std::pair<uint32_t, uint32_t>> m_semaInfoTokens;
};

}  // pssl

