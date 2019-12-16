#include "PsslFetchShader.h"

namespace pssl
{;

enum FetchShaderToken : uint32_t
{
	FS_TOKEN_SEPARATOR = 0xBF8C007F,
	FS_TOKEN_END1 = 0xBF8C0000,
	FS_TOKEN_END2 = 0xBE802000,
};

PsslFetchShader::PsslFetchShader(const uint32_t* code)
{
	// TODO:
	// May cause exception in some cases, should do excetion handling.
	for (uint32_t i = 0; i != UINT_MAX; ++i)
	{
		if (code[i] == FS_TOKEN_END1 && code[i + 1] == FS_TOKEN_END2)
		{
			m_codeLengthDw = i + 2;
			m_numInputSlots = code[m_codeLengthDw];
			break;
		}
	}
}

PsslFetchShader::PsslFetchShader(PsslFetchShader&& other):
	m_codeLengthDw(other.m_codeLengthDw),
	m_numInputSlots(other.m_numInputSlots),
	m_instructionList(std::move(other.m_instructionList))
{

}

PsslFetchShader::PsslFetchShader(PsslFetchShader& other):
	m_codeLengthDw(other.m_codeLengthDw),
	m_numInputSlots(other.m_numInputSlots),
	m_instructionList(std::move(other.m_instructionList))
{

}

}  // pssl