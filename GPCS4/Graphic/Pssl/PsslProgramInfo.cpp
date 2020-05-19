#include "PsslProgramInfo.h"

#define SHADER_BINARY_INFO_SIG "OrbShdr"
#define SHADER_BINARY_INFO_SEARCH_MAX (1024 * 1024 * 5)

LOG_CHANNEL(Graphic.Pssl.PsslProgramInfo);

namespace pssl
{;


PsslProgramInfo::PsslProgramInfo(const uint8_t* code) 
{
	bool success = initBinaryInfo(code);
	LOG_ASSERT(success, "init ShaderBinaryInfo failed.");

	success = initShaderType();
	LOG_ASSERT(success, "init shader type failed.");
}

PsslProgramInfo::~PsslProgramInfo()
{
}


uint32_t PsslProgramInfo::codeSizeBytes() const
{
	return m_shaderBinaryInfo.length;
}

uint32_t PsslProgramInfo::codeSizeDwords() const
{
	return codeSizeBytes() / sizeof(uint32_t);
}

bool PsslProgramInfo::hasFetchShader()
{
	bool hasFs = false;

	for (auto& slot : m_inputUsageSlots)
	{
		if (slot.usageType != kShaderInputUsageSubPtrFetchShader)
		{
			continue;
		}
		hasFs = true;
		break;
	}

	return hasFs;
}

PsslProgramType PsslProgramInfo::shaderType() const
{
	return m_type;
}

PsslKey PsslProgramInfo::key() const
{
	return PsslKey(m_shaderBinaryInfo.crc32, m_shaderBinaryInfo.shaderHash0);
}

uint32_t PsslProgramInfo::inputUsageSlotCount() const
{
	return m_shaderBinaryInfo.numInputUsageSlots;
}

const InputUsageSlot* PsslProgramInfo::inputUsageSlot(uint32_t idx) const
{
	return &m_inputUsageSlots[idx];
}

std::vector<InputUsageSlot> PsslProgramInfo::inputUsageSlot() const
{
	return m_inputUsageSlots;
}

VkShaderStageFlagBits PsslProgramInfo::shaderStage() const
{
	switch (m_type) 
	{
		case PsslProgramType::PixelShader: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case PsslProgramType::VertexShader: return VK_SHADER_STAGE_VERTEX_BIT;
		case PsslProgramType::GeometryShader: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case PsslProgramType::HullShader: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case PsslProgramType::DomainShader: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case PsslProgramType::ComputeShader: return VK_SHADER_STAGE_COMPUTE_BIT;
	}
}

spv::ExecutionModel PsslProgramInfo::executionModel() const
{
	switch (m_type) 
	{
		case PsslProgramType::PixelShader: return spv::ExecutionModelFragment;
		case PsslProgramType::VertexShader: return spv::ExecutionModelVertex;
		case PsslProgramType::GeometryShader: return spv::ExecutionModelGeometry;
		case PsslProgramType::HullShader: return spv::ExecutionModelTessellationControl;
		case PsslProgramType::DomainShader: return spv::ExecutionModelTessellationEvaluation;
		case PsslProgramType::ComputeShader: return spv::ExecutionModelGLCompute;
	}
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

		ShaderBinaryInfo* info = NULL;
		const uint32_t sigLen = sizeof(m_shaderBinaryInfo.signature);
		for (uint32_t i = 0; i != SHADER_BINARY_INFO_SEARCH_MAX; ++i)
		{
			if (!std::memcmp(&code[i], SHADER_BINARY_INFO_SIG, sigLen))
			{
				info = (ShaderBinaryInfo*)&code[i];
				memcpy(&m_shaderBinaryInfo, info, sizeof(m_shaderBinaryInfo));
				ret = true;
				break;
			}
		}

		if (!ret)
		{
			break;
		}

		uint32_t const* usageMasks = reinterpret_cast<unsigned int const*>((unsigned char const*)info - info->chunkUsageBaseOffsetInDW * 4);
		int32_t inputUsageSlotsCount = info->numInputUsageSlots;
		const InputUsageSlot* inputUsageSlots = (InputUsageSlot const*)usageMasks - inputUsageSlotsCount;
		for (uint32_t j = 0; j != inputUsageSlotsCount; ++j)
		{
			m_inputUsageSlots.push_back(inputUsageSlots[j]);
		}
		
	} while (false);
	return ret;
}

bool PsslProgramInfo::initShaderType()
{
	m_type = PsslProgramType::ShaderTypeCount;
	switch (m_shaderBinaryInfo.type)
	{
	case kShaderTypePs:
		m_type = PsslProgramType::PixelShader;
		break;
	case kShaderTypeVsVs:
		m_type = PsslProgramType::VertexShader;
		break;
	case kShaderTypeCs:
		m_type = PsslProgramType::ComputeShader;
		break;
	case kShaderTypeGs:
		m_type = PsslProgramType::GeometryShader;
		break;
	case kShaderTypeHs:
		m_type = PsslProgramType::HullShader;
		break;
	case kShaderTypeDsVs:
		m_type = PsslProgramType::DomainShader;
		break;
	case kShaderTypeVsEs:
	case kShaderTypeVsLs:
		LOG_FIXME("LS and ES stage is not supported yet.");
		break;
	default:
		LOG_ERR("Error shader type %d", m_shaderBinaryInfo.type);
		break;
	}
	return (m_type != PsslProgramType::ShaderTypeCount);
}

uint32_t getFetchShaderStartRegister(const PsslProgramInfo& progInfo)
{
	uint32_t startReg = UINT_MAX;
	uint32_t slotCount = progInfo.inputUsageSlotCount();

	for (uint32_t i = 0; i != slotCount; ++i)
	{
		const InputUsageSlot* slot = progInfo.inputUsageSlot(i);
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