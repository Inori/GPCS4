#include "GcnHeader.h"
#include "GcnConstants.h"
#include "GcnDecoder.h"
#include "GcnProgramInfo.h"
#include "Gnm/GnmConstant.h"


LOG_CHANNEL(Graphic.Gcn.GcnHeader);

using namespace sce::Gnm;

namespace sce::gcn
{
	GcnBinaryInfo::GcnBinaryInfo(const uint8_t* shaderCode)
	{
		const uint32_t* token         = reinterpret_cast<const uint32_t*>(shaderCode);
		const uint32_t  tokenMovVccHi = 0xBEEB03FF;

		// first instruction should be
		// s_mov_b32 vcc_hi, sizeInWords
		// currently I didn't meet other cases,
		// but if it is, we can still search for the header magic 'OrbShdr'
		LOG_ASSERT(token[0] == tokenMovVccHi, "first instruction is not s_mov_b32 vcc_hi, #imm");

		m_binInfo = reinterpret_cast<const ShaderBinaryInfo*>(token + (token[1] + 1) * 2);
	}

	GcnBinaryInfo::~GcnBinaryInfo()
	{
	}

	GcnHeader::GcnHeader(const uint8_t* shaderCode)
	{
		parseHeader(shaderCode);
		extractResourceTable(shaderCode);
	}

	GcnHeader::~GcnHeader()
	{
	}

	GcnProgramType GcnHeader::type() const
	{
		GcnProgramType   result;
		ShaderBinaryType binType = static_cast<ShaderBinaryType>(m_binInfo.m_type);
		switch (binType)
		{
			case ShaderBinaryType::kPixelShader:
				result = GcnProgramType::PixelShader;
				break;
			case ShaderBinaryType::kVertexShader:
				result = GcnProgramType::VertexShader;
				break;
			case ShaderBinaryType::kComputeShader:
				result = GcnProgramType::ComputeShader;
				break;
			case ShaderBinaryType::kGeometryShader:
				result = GcnProgramType::GeometryShader;
				break;
			case ShaderBinaryType::kHullShader:
				result = GcnProgramType::HullShader;
				break;
			case ShaderBinaryType::kDomainShader:
				result = GcnProgramType::DomainShader;
				break;
			case ShaderBinaryType::kLocalShader:
			case ShaderBinaryType::kExportShader:
			case ShaderBinaryType::kUnknown:
			default:
				LOG_ASSERT(false, "unknown shader type.");
				break;
		}
		return result;
	}

	void GcnHeader::parseHeader(
		const uint8_t* shaderCode)
	{
	
		GcnBinaryInfo info(shaderCode);

		const ShaderBinaryInfo* binaryInfo = info.info();
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

	void GcnHeader::extractResourceTable(const uint8_t* code)
	{
		// We can't distinguish some of the resource type without iterate
		// through all shader instructions.
		// For example, a T# in an ImmResource slot 
		// may be either a sampled image or a storage image.
		// If it is accessed via an IMAGE_LOAD_XXX instruction,
		// then we can say it is a storage image.
		auto typeInfo = analyzeResourceType(code);

		m_resourceTable.reserve(m_inputUsageSlotTable.size());

		for (const auto& slot : m_inputUsageSlotTable)
		{
			GcnShaderResource res = {};

			res.usage         = slot.m_usageType;
			res.inEud         = (slot.m_startRegister >= kMaxUserDataCount);
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
				// If ImmResource is a buffer, it may be pretty large,
				// so we use SSBO.
				res.type = isVSharp
							   ? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
							   : (typeInfo.m_storageImages.find(res.startRegister) != typeInfo.m_storageImages.end()
									  ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
									  : VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);

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
			case kShaderInputUsageImmSampler:
			{
				res.type         = VK_DESCRIPTOR_TYPE_SAMPLER;
				res.sizeInDwords = kDwordSizeSampler;
			}
				break;
			case kShaderInputUsagePtrExtendedUserData:
			case kShaderInputUsageSubPtrFetchShader:
			case kShaderInputUsagePtrVertexBufferTable:
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

	GcnHeader::ResourceTypeInfo GcnHeader::analyzeResourceType(const uint8_t* code)
	{
		const uint32_t* start = reinterpret_cast<const uint32_t*>(code);
		const uint32_t* end   = reinterpret_cast<const uint32_t*>(code + m_binInfo.m_length);
		GcnCodeSlice    slice(start, end);

		GcnDecodeContext decoder;

		GcnHeader::ResourceTypeInfo result;

		while (!slice.atEnd())
		{
			decoder.decodeInstruction(slice);

			auto& ins = decoder.getInstruction();

			if (ins.opcode >= GcnOpcode::IMAGE_LOAD &&
				ins.opcode <= GcnOpcode::IMAGE_ATOMIC_FMAX &&
				ins.opcode != GcnOpcode::IMAGE_GET_RESINFO)
			{
				uint32_t startRegister = ins.src[2].code << 2;
				result.m_storageImages.insert(startRegister);
			}
		}

		return result;
	}



}  // namespace sce::gcn