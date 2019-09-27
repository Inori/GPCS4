#pragma once

#include "GPCS4Common.h"
#include "PsslShaderStructure.h"
#include "PsslEnums.h"
#include "PsslKey.h"
#include "vulkan/vulkan.h"
#include "../SpirV/spirv.hpp"

namespace pssl
{;


class PsslProgramInfo
{
public:
	PsslProgramInfo(const uint8_t* code);
	~PsslProgramInfo();

	uint32_t codeSizeBytes() const;

	uint32_t codeSizeDwords() const;

	bool hasFetchShader();

	PsslProgramType shaderType() const;

	PsslKey key() const;

	uint32_t inputUsageSlotCount() const;

	const InputUsageSlot* inputUsageSlot(uint32_t idx) const;

	std::vector<InputUsageSlot> inputUsageSlot() const;

	VkShaderStageFlagBits shaderStage() const;

	spv::ExecutionModel executionModel() const;

private:
	bool initBinaryInfo(const uint8_t* code);
	bool initShaderType();

private:
	ShaderBinaryInfo m_shaderBinaryInfo;
	std::vector<InputUsageSlot> m_inputUsageSlots;
	PsslProgramType m_type;
};

// return UINT_MAX means no fetch shader
uint32_t getFetchShaderStartRegister(const PsslProgramInfo& progInfo);

uint32_t getFetchShaderStartRegister(const uint8_t* code);
}
