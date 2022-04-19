#include "GnmConverter.h"

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
		// clang-format off
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
		// clang-format on
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
		// clang-format off
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
		// clang-format on
		return factor;
	}

	VkBlendOp convertBlendFunc(BlendFunc func)
	{
		// clang-format off

		VkBlendOp op;
		switch (func)
		{
		case kBlendFuncAdd: op = VK_BLEND_OP_ADD; break;
		case kBlendFuncSubtract: op = VK_BLEND_OP_SUBTRACT; break;
		case kBlendFuncMin: op = VK_BLEND_OP_MIN; break;
		case kBlendFuncMax: op = VK_BLEND_OP_MAX; break;
		case kBlendFuncReverseSubtract: op = VK_BLEND_OP_REVERSE_SUBTRACT; break;
		}

		// clang-format on
		return op;
	}

	VkPrimitiveTopology convertPrimitiveType(PrimitiveType primType)
	{
		// clang-format off

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

		// clang-format on
		return topology;
	}

	VkIndexType convertIndexSize(IndexSize indexSize)
	{
		// clang-format off

		VkIndexType indexType;
		switch (indexSize)
		{
		case kIndexSize16: indexType = VK_INDEX_TYPE_UINT16; break;
		case kIndexSize32: indexType = VK_INDEX_TYPE_UINT32; break;
		}
		
		// clang-format on

		return indexType;
	}

}  // namespace sce::Gnm::cvt