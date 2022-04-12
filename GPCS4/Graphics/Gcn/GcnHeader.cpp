#include "GcnHeader.h"

LOG_CHANNEL(Graphic.Gcn.GcnHeader);

namespace sce::gcn
{
	GcnHeader::GcnHeader(const uint8_t* shaderCode)
	{
		parseHeader(shaderCode);
	}

	GcnHeader::~GcnHeader()
	{
	}

	const ShaderBinaryInfo& GcnHeader::getShaderBinaryInfo()
	{
	}

	InputUsageSlotTable GcnHeader::getInputUsageSlotTable()
	{
	}

	void GcnHeader::parseHeader(
		const uint8_t* shaderCode)
	{
		const uint32_t* token         = reinterpret_cast<const uint32_t*>(shaderCode);
		const uint32_t  tokenMovVccHi = 0xBEEB03FF;

		// first instruction should be
		// s_mov_b32 vcc_hi, sizeInWords
		// currently I didn't meet other cases,
		// but if it is, we can still search for the header magic 'OrbShdr'
		LOG_ASSERT(token[0] == tokenMovVccHi, "first instruction is not s_mov_b32 vcc_hi, #imm");

		const ShaderBinaryInfo* binaryInfo = reinterpret_cast<const ShaderBinaryInfo*>(token + (token[1] + 1) * 2);
		std::memcpy(&m_binInfo, binaryInfo, sizeof(ShaderBinaryInfo));

		// Get usage masks and input usage slots
		uint32_t const*       usageMasks           = reinterpret_cast<uint32_t const*>((uint8_t const*)binaryInfo - binaryInfo->m_chunkUsageBaseOffsetInDW * 4);
		int32_t               inputUsageSlotsCount = binaryInfo->m_numInputUsageSlots;
		InputUsageSlot const* inputUsageSlots      = (InputUsageSlot const*)usageMasks - inputUsageSlotsCount;

		m_inputUsageSlotTable.reserve(inputUsageSlotsCount);
		for (uint32_t i = 0; i != inputUsageSlotsCount; ++i)
		{
			m_inputUsageSlotTable.emplace_back(inputUsageSlots[i]);
		}
	}

}  // namespace sce::gcn