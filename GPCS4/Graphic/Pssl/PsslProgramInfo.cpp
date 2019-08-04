#include "PsslProgramInfo.h"

#define SHADER_BINARY_INFO_SIG "OrbShdr"
#define SHADER_BINARY_INFO_SEARCH_MAX (1024 * 1024 * 5)

namespace pssl
{;


PsslProgramInfo::PsslProgramInfo(const uint8_t* code) :
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

bool PsslProgramInfo::hasFetchShader()
{
	bool hasFs = false;
	uint32_t slotCount = getInputUsageSlotCount();

	for (uint32_t i = 0; i != slotCount; ++i)
	{
		const InputUsageSlot* slot = getInputUsageSlot(i);
		if (slot->usageType != kShaderInputUsageSubPtrFetchShader)
		{
			continue;
		}
		hasFs = true;
		break;
	}

	return hasFs;
}

PsslProgramType PsslProgramInfo::getShaderType() const
{
	PsslProgramType type = UnknownShader;
	switch (m_shaderBinaryInfo->type)
	{
	case kShaderTypePs:
		type = PixelShader;
		break;
	case kShaderTypeVsVs:
		type = VertexShader;
		break;
	case kShaderTypeCs:
		type = ComputeShader;
		break;
	case kShaderTypeGs:
		type = GeometryShader;
		break;
	case kShaderTypeHs:
		type = HullShader;
		break;
	case kShaderTypeDsVs:
		type = DomainShader;
		break;
	case kShaderTypeVsEs:
	case kShaderTypeVsLs:
		LOG_FIXME("LS and ES stage is not supported yet.");
		break;
	default:
		LOG_ERR("Error shader type %d", m_shaderBinaryInfo->type);
		break;
	}
	return type;
}

PsslKey PsslProgramInfo::getKey() const
{
	return PsslKey(m_shaderBinaryInfo->crc32, m_shaderBinaryInfo->shaderHash0);
}

uint32_t PsslProgramInfo::getInputUsageSlotCount() const
{
	return m_shaderBinaryInfo->numInputUsageSlots;
}

const InputUsageSlot* PsslProgramInfo::getInputUsageSlot(uint32_t idx) const
{
	return &m_inputUsageSlots[idx];
}

bool PsslProgramInfo::initBinaryInfo(const uint8_t* code)
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

uint32_t getFetchShaderStartRegister(const PsslProgramInfo& progInfo)
{
	uint32_t startReg = UINT_MAX;
	uint32_t slotCount = progInfo.getInputUsageSlotCount();

	for (uint32_t i = 0; i != slotCount; ++i)
	{
		const InputUsageSlot* slot = progInfo.getInputUsageSlot(i);
		if (slot->usageType != kShaderInputUsageSubPtrFetchShader)
		{
			continue;
		}

		startReg = slot->startRegister;
		break;
	}

	return startReg;
}

uint32_t getFetchShaderStartRegister(const uint8_t* code)
{
	PsslProgramInfo progInfo(code);
	return getFetchShaderStartRegister(progInfo);
}

}