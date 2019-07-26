#pragma once

#include "GPCS4Common.h"
#include "PsslShaderStructure.h"

namespace pssl
{;

// Data is not persistent,
// you can't use this class after code is free.

class PsslProgramInfo
{
public:
	PsslProgramInfo(uint8_t* code);
	~PsslProgramInfo();

	uint32_t getCodeSizeBytes() const;

	uint32_t getCodeSizeDwords() const;

	uint32_t getShaderType() const;

	uint32_t getInputUsageSlotCount() const;

	const InputUsageSlot* getInputUsageSlot(uint32_t idx) const;

private:
	bool initBinaryInfo(uint8_t* code);

private:
	const ShaderBinaryInfo* m_shaderBinaryInfo;
	const InputUsageSlot* m_inputUsageSlots;
};



}
