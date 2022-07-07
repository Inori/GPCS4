#include "GnmConverter.h"
#include "SpirV/spirv.hpp"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gnm.GnmConverter);

namespace sce::Gnm::cvt
{
	
	VkFormat convertZFormat(ZFormat zfmt)
	{
		VkFormat format = VK_FORMAT_UNDEFINED;
		switch (zfmt)
		{
		case kZFormatInvalid:
			format = VK_FORMAT_UNDEFINED;
			break;
		case kZFormat16:
			// seems there's no half float point image format in vulkan.
			LOG_WARN("half float z format detected.");
			format = VK_FORMAT_D16_UNORM;
			break;
		case kZFormat32Float:
			format = VK_FORMAT_D32_SFLOAT;
			break;
		}
		return format;
	}

	VkFormat convertDataFormat(DataFormat dataFormat)
	{
		VkFormat format = VK_FORMAT_UNDEFINED;

		// TODO:
		// These key-value pairs are auto generated using sublime text,
		// most of the value parts (VK_FORMAT_XXXX) are not exist,
		// so I comment it out.
		// Should be fixed to right format value.
		const static std::unordered_map<DataFormat, VkFormat, DataFormatHash> formatMap = {

			// Predefined data formats
			{ kDataFormatInvalid, VK_FORMAT_UNDEFINED },
			{ kDataFormatR32G32B32A32Float, VK_FORMAT_R32G32B32A32_SFLOAT },
			{ kDataFormatB32G32R32A32Float, VK_FORMAT_R32G32B32A32_SFLOAT },
			{ kDataFormatR32G32B32X32Float, VK_FORMAT_R32G32B32A32_SFLOAT },
			{ kDataFormatB32G32R32X32Float, VK_FORMAT_R32G32B32A32_SFLOAT },
			{ kDataFormatR32G32B32A32Uint, VK_FORMAT_R32G32B32A32_UINT },
			{ kDataFormatR32G32B32A32Sint, VK_FORMAT_R32G32B32A32_SINT },
			{ kDataFormatR32G32B32Float, VK_FORMAT_R32G32B32_SFLOAT },
			{ kDataFormatR32G32B32Uint, VK_FORMAT_R32G32B32_UINT },
			{ kDataFormatR32G32B32Sint, VK_FORMAT_R32G32B32_SINT },
			//{ kDataFormatR16G16B16A16Float, VK_FORMAT_R16G16B16A16_FLOAT },
			//{ kDataFormatR16G16B16X16Float, VK_FORMAT_R16G16B16X16_FLOAT },
			//{ kDataFormatB16G16R16X16Float, VK_FORMAT_B16G16R16X16_FLOAT },
			{ kDataFormatR16G16B16A16Uint, VK_FORMAT_R16G16B16A16_UINT },
			{ kDataFormatR16G16B16A16Sint, VK_FORMAT_R16G16B16A16_SINT },
			{ kDataFormatR16G16B16A16Unorm, VK_FORMAT_R16G16B16A16_UNORM },
			//{ kDataFormatB16G16R16A16Unorm, VK_FORMAT_B16G16R16A16_UNORM },
			//{ kDataFormatR16G16B16X16Unorm, VK_FORMAT_R16G16B16X16_UNORM },
			//{ kDataFormatB16G16R16X16Unorm, VK_FORMAT_B16G16R16X16_UNORM },
			{ kDataFormatR16G16B16A16Snorm, VK_FORMAT_R16G16B16A16_SNORM },
			//{ kDataFormatL32A32Float, VK_FORMAT_L32A32_FLOAT },
			{ kDataFormatR32G32Float, VK_FORMAT_R32G32_SFLOAT },
			{ kDataFormatR32G32Uint, VK_FORMAT_R32G32_UINT },
			{ kDataFormatR32G32Sint, VK_FORMAT_R32G32_SINT },
			{ kDataFormatR11G11B10Float, VK_FORMAT_B10G11R11_UFLOAT_PACK32 },  // ------ Not sure.
			{ kDataFormatR8G8B8A8Unorm, VK_FORMAT_R8G8B8A8_UNORM },
			//{ kDataFormatR8G8B8X8Unorm, VK_FORMAT_R8G8B8X8_UNORM },
			{ kDataFormatR8G8B8A8UnormSrgb, VK_FORMAT_R8G8B8A8_SRGB },
			//{ kDataFormatR8G8B8X8UnormSrgb, VK_FORMAT_R8G8B8X8_UNORMSRGB },
			{ kDataFormatR8G8B8A8Uint, VK_FORMAT_R8G8B8A8_UINT },
			{ kDataFormatR8G8B8A8Snorm, VK_FORMAT_R8G8B8A8_SNORM },
			{ kDataFormatR8G8B8A8Sint, VK_FORMAT_R8G8B8A8_SINT },
			//{ kDataFormatL16A16Float, VK_FORMAT_L16A16_FLOAT },
			//{ kDataFormatR16G16Float, VK_FORMAT_R16G16_FLOAT },
			//{ kDataFormatL16A16Unorm, VK_FORMAT_L16A16_UNORM },
			{ kDataFormatR16G16Unorm, VK_FORMAT_R16G16_UNORM },
			{ kDataFormatR16G16Uint, VK_FORMAT_R16G16_UINT },
			{ kDataFormatR16G16Snorm, VK_FORMAT_R16G16_SNORM },
			{ kDataFormatR16G16Sint, VK_FORMAT_R16G16_SINT },
			{ kDataFormatR32Float, VK_FORMAT_R32_SFLOAT },
			//{ kDataFormatL32Float, VK_FORMAT_L32_FLOAT },
			//{ kDataFormatA32Float, VK_FORMAT_A32_FLOAT },
			{ kDataFormatR32Uint, VK_FORMAT_R32_UINT },
			{ kDataFormatR32Sint, VK_FORMAT_R32_SINT },
			{ kDataFormatR8G8Unorm, VK_FORMAT_R8G8_UNORM },
			{ kDataFormatR8G8Uint, VK_FORMAT_R8G8_UINT },
			{ kDataFormatR8G8Snorm, VK_FORMAT_R8G8_SNORM },
			{ kDataFormatR8G8Sint, VK_FORMAT_R8G8_SINT },
			//{ kDataFormatL8A8Unorm, VK_FORMAT_L8A8_UNORM },
			//{ kDataFormatL8A8UnormSrgb, VK_FORMAT_L8A8_UNORMSRGB },
			//{ kDataFormatR16Float, VK_FORMAT_R16_FLOAT },
			//{ kDataFormatL16Float, VK_FORMAT_L16_FLOAT },
			//{ kDataFormatA16Float, VK_FORMAT_A16_FLOAT },
			{ kDataFormatR16Unorm, VK_FORMAT_R16_UNORM },
			//{ kDataFormatL16Unorm, VK_FORMAT_L16_UNORM },
			//{ kDataFormatA16Unorm, VK_FORMAT_A16_UNORM },
			{ kDataFormatR16Uint, VK_FORMAT_R16_UINT },
			{ kDataFormatR16Snorm, VK_FORMAT_R16_SNORM },
			{ kDataFormatR16Sint, VK_FORMAT_R16_SINT },
			{ kDataFormatR8Unorm, VK_FORMAT_R8_UNORM },
			//{ kDataFormatL8Unorm, VK_FORMAT_L8_UNORM },
			//{ kDataFormatL8UnormSrgb, VK_FORMAT_L8_UNORMSRGB },
			{ kDataFormatR8Uint, VK_FORMAT_R8_UINT },
			{ kDataFormatR8Snorm, VK_FORMAT_R8_SNORM },
			{ kDataFormatR8Sint, VK_FORMAT_R8_SINT },
			{ kDataFormatA8Unorm, VK_FORMAT_R8_UNORM },
			//{ kDataFormatR1Unorm, VK_FORMAT_R1_UNORM },
			//{ kDataFormatL1Unorm, VK_FORMAT_L1_UNORM },
			//{ kDataFormatA1Unorm, VK_FORMAT_A1_UNORM },
			//{ kDataFormatR1Uint, VK_FORMAT_R1_UINT },
			//{ kDataFormatL1Uint, VK_FORMAT_L1_UINT },
			//{ kDataFormatA1Uint, VK_FORMAT_A1_UINT },
			//{ kDataFormatR1ReversedUnorm, VK_FORMAT_R1_REVERSEDUNORM },
			//{ kDataFormatL1ReversedUnorm, VK_FORMAT_L1_REVERSEDUNORM },
			//{ kDataFormatA1ReversedUnorm, VK_FORMAT_A1_REVERSEDUNORM },
			//{ kDataFormatR1ReversedUint, VK_FORMAT_R1_REVERSEDUINT },
			//{ kDataFormatL1ReversedUint, VK_FORMAT_L1_REVERSEDUINT },
			//{ kDataFormatA1ReversedUint, VK_FORMAT_A1_REVERSEDUINT },
			//{ kDataFormatBc1Unorm, VK_FORMAT_BC1_UNORM },
			//{ kDataFormatBc1UBNorm, VK_FORMAT_BC1_UBNORM },
			//{ kDataFormatBc1UnormSrgb, VK_FORMAT_BC1_UNORMSRGB },
			//{ kDataFormatBc2Unorm, VK_FORMAT_BC2_UNORM },
			//{ kDataFormatBc2UBNorm, VK_FORMAT_BC2_UBNORM },
			//{ kDataFormatBc2UnormSrgb, VK_FORMAT_BC2_UNORMSRGB },
			{ kDataFormatBc3Unorm, VK_FORMAT_BC3_UNORM_BLOCK },
			//{ kDataFormatBc3UBNorm, VK_FORMAT_BC3_UBNORM },
			{ kDataFormatBc3UnormSrgb, VK_FORMAT_BC3_SRGB_BLOCK },
			//{ kDataFormatBc4Unorm, VK_FORMAT_BC4_UNORM },
			//{ kDataFormatBc4Snorm, VK_FORMAT_BC4_SNORM },
			//{ kDataFormatBc5Unorm, VK_FORMAT_BC5_UNORM },
			//{ kDataFormatBc5Snorm, VK_FORMAT_BC5_SNORM },
			//{ kDataFormatBc6Unorm, VK_FORMAT_BC6_UNORM },
			//{ kDataFormatBc6Snorm, VK_FORMAT_BC6_SNORM },
			//{ kDataFormatBc6Uf16, VK_FORMAT_BC6UF16_ },
			//{ kDataFormatBc6Sf16, VK_FORMAT_BC6SF16_ },
			//{ kDataFormatBc7Unorm, VK_FORMAT_BC7_UNORM },
			//{ kDataFormatBc7UBNorm, VK_FORMAT_BC7_UBNORM },
			//{ kDataFormatBc7UnormSrgb, VK_FORMAT_BC7_UNORMSRGB },
			//{ kDataFormatB5G6R5Unorm, VK_FORMAT_B5G6R5_UNORM },
			//{ kDataFormatR5G5B5A1Unorm, VK_FORMAT_R5G5B5A1_UNORM },
			//{ kDataFormatB5G5R5A1Unorm, VK_FORMAT_B5G5R5A1_UNORM },
			{ kDataFormatB8G8R8A8Unorm, VK_FORMAT_B8G8R8A8_UNORM },
			//{ kDataFormatB8G8R8X8Unorm, VK_FORMAT_B8G8R8X8_UNORM },
			//{ kDataFormatB8G8R8A8UnormSrgb, VK_FORMAT_B8G8R8A8_UNORMSRGB },
			//{ kDataFormatB8G8R8X8UnormSrgb, VK_FORMAT_B8G8R8X8_UNORMSRGB },
			//{ kDataFormatR4G4B4A4Unorm, VK_FORMAT_R4G4B4A4_UNORM },
			//{ kDataFormatB4G4R4A4Unorm, VK_FORMAT_B4G4R4A4_UNORM },
			//{ kDataFormatB4G4R4X4Unorm, VK_FORMAT_B4G4R4X4_UNORM },
			//{ kDataFormatB5G5R5X1Unorm, VK_FORMAT_B5G5R5X1_UNORM },
			//{ kDataFormatR5G6B5Unorm, VK_FORMAT_R5G6B5_UNORM },
			//{ kDataFormatB10G10R10A2Unorm, VK_FORMAT_B10G10R10A2_UNORM },
			//{ kDataFormatR10G10B10A2Unorm, VK_FORMAT_R10G10B10A2_UNORM },
			//{ kDataFormatR10G10B10A2Uint, VK_FORMAT_R10G10B10A2_UINT },
			//{ kDataFormatB10G10R10A2Uint, VK_FORMAT_B10G10R10A2_UINT },
			//{ kDataFormatB16G16R16A16Float, VK_FORMAT_B16G16R16A16_FLOAT },
			//{ kDataFormatR11G11B10Unorm, VK_FORMAT_R11G11B10_UNORM },
			//{ kDataFormatR11G11B10Snorm, VK_FORMAT_R11G11B10_SNORM },
			//{ kDataFormatB10G11R11Unorm, VK_FORMAT_B10G11R11_UNORM },
			//{ kDataFormatB10G11R11Snorm, VK_FORMAT_B10G11R11_SNORM },
			//{ kDataFormatR9G9B9E5Float, VK_FORMAT_R9G9B9E5_FLOAT },
			//{ kDataFormatB8G8R8G8Unorm, VK_FORMAT_B8G8R8G8_UNORM },
			//{ kDataFormatG8B8G8R8Unorm, VK_FORMAT_G8B8G8R8_UNORM },
			//{ kDataFormatBc1UnormNoAlpha, VK_FORMAT_BC1_UNORMNOALPHA },
			//{ kDataFormatBc1UnormSrgbNoAlpha, VK_FORMAT_BC1_UNORMSRGBNOALPHA },
			//{ kDataFormatBc7UnormNoAlpha, VK_FORMAT_BC7_UNORMNOALPHA },
			//{ kDataFormatBc7UnormSrgbNoAlpha, VK_FORMAT_BC7_UNORMSRGBNOALPHA },
			//{ kDataFormatBc3UnormRABG, VK_FORMAT_BC3_UNORMRABG },

			// Custom data formats
			{ kDataFormatL32FloatXXXX, VK_FORMAT_R32_SFLOAT },
		};

		auto iter = formatMap.find(dataFormat);
		if (iter == formatMap.end())
		{
			LOG_WARN("data format not found %s", dataFormatName(dataFormat));

			// Reverse the W component and try again.
			DataFormat retryFormat = DataFormat::build(
				(SurfaceFormat)dataFormat.m_bits.m_surfaceFormat,
				(TextureChannelType)dataFormat.m_bits.m_channelType,
				(TextureChannel)dataFormat.m_bits.m_channelX,
				(TextureChannel)dataFormat.m_bits.m_channelY,
				(TextureChannel)dataFormat.m_bits.m_channelZ,
				(TextureChannel)(dataFormat.m_bits.m_channelW == kTextureChannelConstant0
									 ? kTextureChannelConstant1
									 : kTextureChannelConstant0));

			iter = formatMap.find(retryFormat);
			LOG_ASSERT(iter != formatMap.end(), "data format still not found with retry.");

			format = iter->second;
		}
		else
		{
			format = iter->second;
		}

		return format;
	}

	VkSampleCountFlagBits convertNumFragments(NumFragments numFragments)
	{
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
		switch (numFragments)
		{
		case kNumFragments1:
			sampleCount = VK_SAMPLE_COUNT_1_BIT;
			break;
		case kNumFragments2:
			sampleCount = VK_SAMPLE_COUNT_2_BIT;
			break;
		case kNumFragments4:
			sampleCount = VK_SAMPLE_COUNT_4_BIT;
			break;
		case kNumFragments8:
			sampleCount = VK_SAMPLE_COUNT_8_BIT;
			break;
		}
		return sampleCount;
	}

	VkFormat convertVideoOutPixelFormat(SceVideoOutPixelFormat format)
	{
		VkFormat result = VK_FORMAT_UNDEFINED;
		switch (format)
		{
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB:
			result = VK_FORMAT_B8G8R8A8_SRGB;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B16_G16_R16_A16_FLOAT:
			result = VK_FORMAT_R16G16B16A16_SFLOAT;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_R8_G8_B8_A8_SRGB:
			result = VK_FORMAT_A8B8G8R8_SRGB_PACK32;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2:
			result = VK_FORMAT_A2R10G10B10_UINT_PACK32;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_SRGB:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_BT2020_PQ:
			LOG_ASSERT(false, "format not supported.");
			break;
		}
		return result;
	}

	DataFormat convertDataFormatFromVideoOutPixelFormat(SceVideoOutPixelFormat format)
	{
		DataFormat result = {};
		switch (format)
		{
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB:
			result = kDataFormatB8G8R8A8UnormSrgb;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B16_G16_R16_A16_FLOAT:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_R8_G8_B8_A8_SRGB:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_SRGB:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_BT2020_PQ:
			LOG_ASSERT(false, "format not supported.");
			break;
		}
		return result;
	}

	VkCompareOp convertCompareFunc(CompareFunc cmpFunc)
	{
		
		VkCompareOp op;
		switch (cmpFunc)
		{
		case kCompareFuncNever:        op = VK_COMPARE_OP_NEVER; break;
		case kCompareFuncLess:         op = VK_COMPARE_OP_LESS; break;
		case kCompareFuncEqual:        op = VK_COMPARE_OP_EQUAL; break;
		case kCompareFuncLessEqual:    op = VK_COMPARE_OP_LESS_OR_EQUAL; break;
		case kCompareFuncGreater:      op = VK_COMPARE_OP_GREATER; break;
		case kCompareFuncNotEqual:     op = VK_COMPARE_OP_NOT_EQUAL; break;
		case kCompareFuncGreaterEqual: op = VK_COMPARE_OP_GREATER_OR_EQUAL; break;
		case kCompareFuncAlways:       op = VK_COMPARE_OP_ALWAYS; break;
		}
		return op;
		
	}

	std::array<VkColorComponentFlags, MaxNumRenderTargets>
	convertRenderTargetMask(uint32_t mask)
	{
		std::array<VkColorComponentFlags, VltLimits::MaxNumRenderTargets> result = {};

		const uint32_t bitsPerSlot = 4;

		static_assert(
			sizeof(uint32_t) * 8 / bitsPerSlot == MaxNumRenderTargets, "render target channel not match.");

		for (uint32_t i = 0; i != VltLimits::MaxNumRenderTargets; ++i)
		{
			uint8_t slotMask = (mask >> (i * bitsPerSlot)) & 0xF;
			result[i]        = static_cast<VkColorComponentFlags>(slotMask);
		}

		return result;
	}

	VkBlendFactor convertBlendMultiplier(BlendMultiplier blendMul)
	{
		
		VkBlendFactor factor = {};
		switch (blendMul)
		{
		case kBlendMultiplierZero:					factor = VK_BLEND_FACTOR_ZERO; break;
		case kBlendMultiplierOne:					factor = VK_BLEND_FACTOR_ONE; break;
		case kBlendMultiplierSrcColor:				factor = VK_BLEND_FACTOR_SRC_COLOR; break;
		case kBlendMultiplierOneMinusSrcColor:		factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR; break;
		case kBlendMultiplierSrcAlpha:				factor = VK_BLEND_FACTOR_SRC_ALPHA; break;
		case kBlendMultiplierOneMinusSrcAlpha:		factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; break;
		case kBlendMultiplierDestAlpha:				factor = VK_BLEND_FACTOR_DST_ALPHA; break;
		case kBlendMultiplierOneMinusDestAlpha:		factor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA; break;
		case kBlendMultiplierDestColor:				factor = VK_BLEND_FACTOR_DST_COLOR; break;
		case kBlendMultiplierOneMinusDestColor:		factor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR; break;
		case kBlendMultiplierSrcAlphaSaturate:		factor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE; break;
		case kBlendMultiplierConstantColor:			factor = VK_BLEND_FACTOR_CONSTANT_COLOR; break;
		case kBlendMultiplierOneMinusConstantColor: factor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR; break;
		case kBlendMultiplierSrc1Color:				factor = VK_BLEND_FACTOR_SRC1_COLOR; break;
		case kBlendMultiplierInverseSrc1Color:		factor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR; break;
		case kBlendMultiplierSrc1Alpha:				factor = VK_BLEND_FACTOR_SRC1_ALPHA; break;
		case kBlendMultiplierInverseSrc1Alpha:		factor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA; break;
		case kBlendMultiplierConstantAlpha:			factor = VK_BLEND_FACTOR_CONSTANT_ALPHA; break;
		case kBlendMultiplierOneMinusConstantAlpha: factor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA; break;
		}
		
		return factor;
	}

	VkBlendOp convertBlendFunc(BlendFunc func)
	{
		

		VkBlendOp op;
		switch (func)
		{
		case kBlendFuncAdd: op = VK_BLEND_OP_ADD; break;
		case kBlendFuncSubtract: op = VK_BLEND_OP_SUBTRACT; break;
		case kBlendFuncMin: op = VK_BLEND_OP_MIN; break;
		case kBlendFuncMax: op = VK_BLEND_OP_MAX; break;
		case kBlendFuncReverseSubtract: op = VK_BLEND_OP_REVERSE_SUBTRACT; break;
		}

		
		return op;
	}

	VkPrimitiveTopology convertPrimitiveType(PrimitiveType primType)
	{
		VkPrimitiveTopology topology;
		switch (primType)
		{
			case kPrimitiveTypeNone: topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM; break;
			case kPrimitiveTypePointList: topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; break;
			case kPrimitiveTypeLineList: topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; break;
			case kPrimitiveTypeLineStrip: topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; break;
			case kPrimitiveTypeTriList: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
			case kPrimitiveTypeTriFan: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN; break;
			case kPrimitiveTypeTriStrip: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; break;
			case kPrimitiveTypePatch: topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST; break;
			case kPrimitiveTypeLineListAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY; break;
			case kPrimitiveTypeLineStripAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY; break;
			case kPrimitiveTypeTriListAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY; break;
			case kPrimitiveTypeTriStripAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY; break;
			// Not supported by vulkan
			//case kPrimitiveTypeRectList: topology = ; break;
			//case kPrimitiveTypeLineLoop: topology = ; break;
			//case kPrimitiveTypeQuadList: topology = ; break;
			//case kPrimitiveTypeQuadStrip: topology = ; break;
			//case kPrimitiveTypePolygon: topology = ; break;
		default:
			topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
			LOG_ERR("unsupported PrimitiveType %d", primType);
			break;
		}

		
		return topology;
	}

	VkIndexType convertIndexSize(IndexSize indexSize)
	{
		

		VkIndexType indexType;
		switch (indexSize)
		{
		case kIndexSize16: indexType = VK_INDEX_TYPE_UINT16; break;
		case kIndexSize32: indexType = VK_INDEX_TYPE_UINT32; break;
		}
		
		

		return indexType;
	}

	VkPolygonMode convertPolygonMode(PrimitiveSetupPolygonMode polyMode)
	{
		

		VkPolygonMode mode;
		switch (polyMode)
		{
		case kPrimitiveSetupPolygonModePoint:	mode = VK_POLYGON_MODE_POINT; break;
		case kPrimitiveSetupPolygonModeLine:	mode = VK_POLYGON_MODE_LINE; break;
		case kPrimitiveSetupPolygonModeFill:	mode = VK_POLYGON_MODE_FILL; break;
		}

		
		return mode;
	}

	VkCullModeFlags convertCullMode(PrimitiveSetupCullFaceMode cullMode)
	{
		

		VkCullModeFlags mode;
		switch (cullMode)
		{
		case kPrimitiveSetupCullFaceNone:  mode = VK_CULL_MODE_NONE; break;
		case kPrimitiveSetupCullFaceFront: mode = VK_CULL_MODE_FRONT_BIT; break;
		case kPrimitiveSetupCullFaceBack:  mode = VK_CULL_MODE_BACK_BIT; break;
		case kPrimitiveSetupCullFaceFrontAndBack: mode = VK_CULL_MODE_FRONT_AND_BACK; break;
		}

		
		return mode;
	}

	VkImageType convertTextureType(TextureType textureType)
	{
		VkImageType type = VK_IMAGE_TYPE_MAX_ENUM;

		switch (textureType)
		{
		case Gnm::kTextureType1d: 
		case Gnm::kTextureType1dArray:
			type = VK_IMAGE_TYPE_1D; 
			break;
		case Gnm::kTextureType2d: 
		case Gnm::kTextureType2dArray:
		case Gnm::kTextureType2dMsaa:
		case Gnm::kTextureType2dArrayMsaa:
		case Gnm::kTextureTypeCubemap:
			type = VK_IMAGE_TYPE_2D; 
			break;
		case Gnm::kTextureType3d:
			type = VK_IMAGE_TYPE_3D; 
			break;
		default:
			LOG_ASSERT(false, "texture type not supported.");
			break;
		}

		return type;
	}

	VkImageViewType convertTextureTypeView(TextureType textureType)
	{
		VkImageViewType type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;

		switch (textureType)
		{
		case Gnm::kTextureType1d: type = VK_IMAGE_VIEW_TYPE_1D; break;
		case Gnm::kTextureType2d: type = VK_IMAGE_VIEW_TYPE_2D; break;
		case Gnm::kTextureType3d: type = VK_IMAGE_VIEW_TYPE_3D; break;
		case Gnm::kTextureTypeCubemap: type = VK_IMAGE_VIEW_TYPE_CUBE; break;
		case Gnm::kTextureType1dArray: type = VK_IMAGE_VIEW_TYPE_1D_ARRAY; break;
		case Gnm::kTextureType2dArray: type = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
		case Gnm::kTextureType2dMsaa:  type = VK_IMAGE_VIEW_TYPE_2D; break;
		case Gnm::kTextureType2dArrayMsaa: type = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
		default:
			LOG_ASSERT(false, "texture type not supported.");
			break;
		}
		return type;
	}

	VkFilter convertFilterMode(FilterMode mode)
	{
		VkFilter filter = VK_FILTER_MAX_ENUM;
		

		switch (mode)
		{
		case Gnm::kFilterModePoint:		filter = VK_FILTER_NEAREST; break;
		case Gnm::kFilterModeBilinear:	filter = VK_FILTER_LINEAR; break;
		// Vulkan do not have exact matching filters, use similar ones instead.
		case Gnm::kFilterModeAnisoPoint:	filter = VK_FILTER_NEAREST; break;
		case Gnm::kFilterModeAnisoBilinear: filter = VK_FILTER_LINEAR; break;
		}
		
		return filter;
	}

	VkSamplerMipmapMode convertMipFilterMode(MipFilterMode mode)
	{
		VkSamplerMipmapMode mipmode = VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;

		
		switch (mode)
		{
		case Gnm::kMipFilterModeNone: mipmode = VK_SAMPLER_MIPMAP_MODE_NEAREST; break;
		case Gnm::kMipFilterModePoint: mipmode = VK_SAMPLER_MIPMAP_MODE_NEAREST; break;
		case Gnm::kMipFilterModeLinear: mipmode = VK_SAMPLER_MIPMAP_MODE_LINEAR; break;
		}
		
		return mipmode;
	}

	VkSamplerAddressMode convertWrapMode(WrapMode mode)
	{
		VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
		switch (mode)
		{
		case Gnm::kWrapModeWrap:
			addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			break;
		case Gnm::kWrapModeMirror:
			addressMode = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			break;
		case Gnm::kWrapModeClampLastTexel:
			addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		case Gnm::kWrapModeMirrorOnceLastTexel:
			addressMode = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			LOG_WARN("sampler address mode not match for %d, use %d", mode, addressMode);
			break;
		case Gnm::kWrapModeClampHalfBorder:
			addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			LOG_WARN("sampler address mode not match for %d, use %d", mode, addressMode);
			break;
		case Gnm::kWrapModeMirrorOnceHalfBorder:
			addressMode = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
			LOG_WARN("sampler address mode not match for %d, use %d", mode, addressMode);
			break;
		case Gnm::kWrapModeClampBorder:
			addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			break;
		case Gnm::kWrapModeMirrorOnceBorder:
			addressMode = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
			LOG_WARN("sampler address mode not match for %d, use %d", mode, addressMode);
			break;
		}
		return addressMode;
	}

	VkCompareOp convertDepthCompare(DepthCompare compare)
	{
		VkCompareOp op = VK_COMPARE_OP_MAX_ENUM;
		
		switch (compare)
		{
		case Gnm::kDepthCompareNever:	op = VK_COMPARE_OP_NEVER; break;
		case Gnm::kDepthCompareLess:	op = VK_COMPARE_OP_LESS; break;
		case Gnm::kDepthCompareEqual:	op = VK_COMPARE_OP_EQUAL; break;
		case Gnm::kDepthCompareLessEqual:		op = VK_COMPARE_OP_LESS_OR_EQUAL; break;
		case Gnm::kDepthCompareGreater:			op = VK_COMPARE_OP_GREATER; break;
		case Gnm::kDepthCompareNotEqual:		op = VK_COMPARE_OP_NOT_EQUAL; break;
		case Gnm::kDepthCompareGreaterEqual:	op = VK_COMPARE_OP_GREATER_OR_EQUAL; break;
		case Gnm::kDepthCompareAlways:			op = VK_COMPARE_OP_ALWAYS; break;
		}
		
		return op;
	}

	spv::Dim convertTextureTypeToDim(TextureType textureType)
	{
		using namespace spv;

		Dim dim = DimMax;

		

		switch (textureType)
		{
		case Gnm::kTextureType1d: dim = Dim1D; break;
		case Gnm::kTextureType2d: dim = Dim2D; break;
		case Gnm::kTextureType3d: dim = Dim3D; break;
		case Gnm::kTextureTypeCubemap:	dim = DimCube; break;
		case Gnm::kTextureType1dArray:	dim = Dim1D; break;
		case Gnm::kTextureType2dArray:	dim = Dim2D; break;
		case Gnm::kTextureType2dMsaa:	dim = Dim2D; break;
		case Gnm::kTextureType2dArrayMsaa:	dim = Dim2D; break;
		default:
			LOG_ASSERT(false, "texture type not supported.");
			break;
		}

		

		return dim;
	}

	VkLogicOp convertRasterOp(RasterOp op)
	{
		VkLogicOp lo = VK_LOGIC_OP_MAX_ENUM;
		switch (op)
		{
			case Gnm::kRasterOpBlackness:
				lo = VK_LOGIC_OP_CLEAR;
				break;
			case Gnm::kRasterOpNor:
				lo = VK_LOGIC_OP_NOR;
				break;
			case Gnm::kRasterOpAndInverted:
				lo = VK_LOGIC_OP_AND_INVERTED;
				break;
			case Gnm::kRasterOpCopyInverted:
				lo = VK_LOGIC_OP_COPY_INVERTED;
				break;
			case Gnm::kRasterOpAndReverse:
				lo = VK_LOGIC_OP_AND_REVERSE;
				break;
			case Gnm::kRasterOpInvert:
				lo = VK_LOGIC_OP_INVERT;
				break;
			case Gnm::kRasterOpXor:
				lo = VK_LOGIC_OP_XOR;
				break;
			case Gnm::kRasterOpNand:
				lo = VK_LOGIC_OP_NAND;
				break;
			case Gnm::kRasterOpAnd:
				lo = VK_LOGIC_OP_AND;
				break;
			case Gnm::kRasterOpEquiv:
				lo = VK_LOGIC_OP_EQUIVALENT;
				break;
			case Gnm::kRasterOpNoop:
				lo = VK_LOGIC_OP_NO_OP;
				break;
			case Gnm::kRasterOpOrInverted:
				lo = VK_LOGIC_OP_OR_INVERTED;
				break;
			case Gnm::kRasterOpCopy:
				lo = VK_LOGIC_OP_COPY;
				break;
			case Gnm::kRasterOpOrReverse:
				lo = VK_LOGIC_OP_OR_REVERSE;
				break;
			case Gnm::kRasterOpOr:
				lo = VK_LOGIC_OP_OR;
				break;
			case Gnm::kRasterOpSet:
				lo = VK_LOGIC_OP_SET;
				break;
		}
		return lo;
	}

}  // namespace sce::Gnm::cvt