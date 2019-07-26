#include "PsslFetchShader.h"

namespace pssl
{;

FetchShader::FetchShader(const uint32_t* code, uint32_t lengthDw):
	m_numInputSlots(0),
	m_vertexBaseToken(0),
	m_instanceBaseToken(0)
{
	extractTokens(code, lengthDw);
}

FetchShader::~FetchShader()
{
}

bool FetchShader::getFetchShaderBuildState(FetchShaderBuildState& fsbs)
{
	bool ret = false;
	do
	{
		
		ret  = true;
	}while(false);
	return ret;
}

uint32_t FetchShader::getFetchShaderLengthDw(const uint32_t* code)
{
	uint32_t lengthDw = 0;
	while (code[lengthDw] != FS_TOKEN_END1 && code[lengthDw + 1] != FS_TOKEN_END2)
	{
		++lengthDw;
	}
	++lengthDw;
	return lengthDw;
}

void FetchShader::extractTokens(const uint32_t* code, uint32_t lengthDw)
{
	uint32_t idx = 0;
	bool parseSemaIdx = true;

	if ((code[idx] & FS_TOKEN_VERTEX_BASE_USGPR) == FS_TOKEN_VERTEX_BASE_USGPR)
	{
		m_vertexBaseToken = code[idx];
		++idx;
	}

	if ((code[idx] & FS_TOKEN_INSTANCE_BASE_USGPR) == FS_TOKEN_INSTANCE_BASE_USGPR)
	{
		m_instanceBaseToken = code[idx];
		++idx;
	}

	while (true)
	{
		if (idx + 2 >= lengthDw)
		{
			break;
		}

		if (code[idx] == FS_TOKEN_END1 && code[idx + 1] == FS_TOKEN_END2)
		{
			m_numInputSlots = code[idx + 2];
			break;
		}

		if (code[idx] == FS_TOKEN_SEPARATOR)
		{
			parseSemaIdx = false;
			++idx;
			continue;
		}

		if (parseSemaIdx)
		{
			m_semaIdxTokens.push_back(code[idx]);
			++idx;
		}
		else
		{
			m_semaInfoTokens.push_back(std::make_pair(code[idx], code[idx + 1]));
			idx += 2;
		}
	}
}

}  // pssl