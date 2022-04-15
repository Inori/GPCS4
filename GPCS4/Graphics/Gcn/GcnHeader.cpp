#include "GcnHeader.h"
#include "GcnConstants.h"
#include "Gnm/GnmConstant.h"

LOG_CHANNEL(Graphic.Gcn.GcnHeader);

using namespace sce::Gnm;

namespace sce::gcn
{
	GcnHeader::GcnHeader(const uint8_t* shaderCode)
	{
		parseHeader(shaderCode);
		extractResourceTable();
	}

	GcnHeader::~GcnHeader()
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

	void GcnHeader::extractResourceTable()
	{
		m_resourceTable.reserve(m_inputUsageSlotTable.size());

		for (const auto& slot : m_inputUsageSlotTable)
		{
			GcnShaderResource res = {};

			res.usage         = slot.m_usageType;
			res.inEud         = (slot.m_startRegister > kMaxUserDataCount);
			res.startRegister = slot.m_startRegister;

			if (res.inEud)
			{
				res.eudOffsetInDwords = slot.m_startRegister - kMaxUserDataCount;
			}

			bool isVSharp = (slot.m_resourceType == 0);

			switch (slot.m_usageType)
			{
			case kShaderInputUsageImmResource:
			{
				res.type = isVSharp ? 
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER : VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				res.sizeInDwords = slot.m_registerCount == 0 ? 4 : 8;
			}
				break;
			case kShaderInputUsageImmRwResource:
			{
				res.type = isVSharp ? 
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER : VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				res.sizeInDwords = slot.m_registerCount == 0 ? 4 : 8;
			}
				break;
			case kShaderInputUsageImmConstBuffer:
			{
				res.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				res.sizeInDwords = kDwordSizeConstantBuffer;
			}
				break;
			case kShaderInputUsagePtrExtendedUserData:
			case kShaderInputUsageSubPtrFetchShader:
			{
				// This is not really a resource
				res.type         = VK_DESCRIPTOR_TYPE_MAX_ENUM;
				res.sizeInDwords = 2;
			}
				break;
			default:
				// Checking for non handled input data
				LOG_ASSERT(false, "Not supported usage type.");
				break;
			}

			m_resourceTable.push_back(res);
		}
	}

}  // namespace sce::gcn