#include "PsslProgramInfo.h"

#define SHADER_BINARY_INFO_SIG "OrbShdr"
#define SHADER_BINARY_INFO_SEARCH_MAX (1024 * 1024 * 5)

namespace pssl
{;


PsslProgramInfo::PsslProgramInfo(uint8_t* code):
	m_shaderBinaryInfo(nullptr),
	m_inputUsageSlots(nullptr)
{
	bool success = initBinaryInfo(code);
	LOG_ASSERT(success, "init ShaderBinaryInfo failed.");
}

PsslProgramInfo::~PsslProgramInfo()
{
}


uint32_t PsslProgramInfo::getCodeSizeBytes() const
{
	return m_shaderBinaryInfo->length;
}

uint32_t PsslProgramInfo::getCodeSizeDwords() const
{
	return getCodeSizeBytes() / sizeof(uint32_t);
}

uint32_t PsslProgramInfo::getShaderType() const
{
	return m_shaderBinaryInfo->type;
}

uint32_t PsslProgramInfo::getInputUsageSlotCount() const
{
	return m_shaderBinaryInfo->numInputUsageSlots;
}

const InputUsageSlot* PsslProgramInfo::getInputUsageSlot(uint32_t idx) const
{
	return &m_inputUsageSlots[idx];
}

bool PsslProgramInfo::initBinaryInfo(uint8_t* code)
{
	bool ret = false;
	do 
	{
		if (!code)
		{
			break;
		}

		const uint32_t sigLen = sizeof(m_shaderBinaryInfo->signature);
		for (uint32_t i = 0; i != SHADER_BINARY_INFO_SEARCH_MAX; ++i)
		{
			if (!std::memcmp(&code[i], SHADER_BINARY_INFO_SIG, sigLen))
			{
				m_shaderBinaryInfo = (ShaderBinaryInfo*)&code[i];
				ret = true;
				break;
			}
		}

		if (!ret)
		{
			break;
		}

		uint32_t const* usageMasks = reinterpret_cast<unsigned int const*>((unsigned char const*)m_shaderBinaryInfo - m_shaderBinaryInfo->chunkUsageBaseOffsetInDW * 4);
		int32_t inputUsageSlotsCount = m_shaderBinaryInfo->numInputUsageSlots;
		m_inputUsageSlots = (InputUsageSlot const*)usageMasks - inputUsageSlotsCount;
		
	} while (false);
	return ret;
}

}