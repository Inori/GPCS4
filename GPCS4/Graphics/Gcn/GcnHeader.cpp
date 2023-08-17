#include "GcnHeader.h"
#include "GcnConstants.h"
#include "GcnDecoder.h"
#include "GcnProgramInfo.h"
#include "GcnInstructionUtil.h"
#include "Gnm/GnmConstant.h"

#include <cstring>


LOG_CHANNEL(Graphic.Gcn.GcnHeader);

using namespace sce::Gnm;

namespace sce::gcn
{
	GcnBinaryInfo::GcnBinaryInfo(const void* shaderCode)
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

	VkShaderStageFlagBits GcnBinaryInfo::stage() const
	{
		VkShaderStageFlagBits result = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		ShaderBinaryType binType = static_cast<ShaderBinaryType>(m_binInfo->m_type);
		switch (binType)
		{
			case ShaderBinaryType::kPixelShader:
				result = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case ShaderBinaryType::kVertexShader:
				result = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case ShaderBinaryType::kComputeShader:
				result = VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			case ShaderBinaryType::kGeometryShader:
				result = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case ShaderBinaryType::kHullShader:
				result = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			case ShaderBinaryType::kDomainShader:
				result = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				break;
			case ShaderBinaryType::kLocalShader:
			case ShaderBinaryType::kExportShader:
			case ShaderBinaryType::kUnknown:
			default:
				LOG_ASSERT(false, "unknown shader stage.");
				break;
		}
		return result;
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
		GcnProgramType result = GcnProgramType::VertexShader;
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
			case kShaderInputUsageImmRwResource:
			{
				if (isVSharp)
				{
					// We use ssbo instead of ubo no matter 
					// it is read-only or read-write,
					// since the buffer could be pretty large.
					res.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				}
				else
				{
					auto iter  = typeInfo.uavs.find(res.startRegister);
					bool isUav = iter != typeInfo.uavs.end();
					if (!isUav)
					{
						res.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					}
					else
					{
						// It's annoying to translate image_load_mip
						// instruction.
						// Normally, we should use OpImageRead/Write,
						// to access UAVs, but these two opcodes doesn't
						// support LOD operands.
						// On AMD GPU, we have SPV_AMD_shader_image_load_store_lod to ease things,
						// but no identical method for Nvidia GPU.
						// So we need to use OpImageFetch to replace OpImageRead,
						// thus to declare the image as sampled.
						// (Well OpImageFetch will be translated back to image_load_mip
						// eventually on AMD GPU, tested on RadeonGPUAnalyzer,
						// not sure for Nvidia.)
						// For image_store_mip, I don't have a good idea as of now.
						bool isUavRead = iter->second;
						if (isUavRead)
						{
							res.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
						}
						else
						{
							res.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
						}
					}
				}
			
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

			if (isImageAccessNoSampling(ins))
			{
				uint32_t startRegister     = ins.src[2].code << 2;
				result.uavs[startRegister] = isUavReadAccess(ins);
			}
		}

		return result;
	}



}  // namespace sce::gcn