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

	VkPolygonMode convertPolygonMode(PrimitiveSetupPolygonMode polyMode)
	{
		// clang-format off

		VkPolygonMode mode;
		switch (polyMode)
		{
		case kPrimitiveSetupPolygonModePoint:	mode = VK_POLYGON_MODE_POINT; break;
		case kPrimitiveSetupPolygonModeLine:	mode = VK_POLYGON_MODE_LINE; break;
		case kPrimitiveSetupPolygonModeFill:	mode = VK_POLYGON_MODE_FILL; break;
		}

		// clang-format on
		return mode;
	}

	VkCullModeFlags convertCullMode(PrimitiveSetupCullFaceMode cullMode)
	{
		// clang-format off

		VkCullModeFlags mode;
		switch (cullMode)
		{
		case kPrimitiveSetupCullFaceNone:  mode = VK_CULL_MODE_NONE; break;
		case kPrimitiveSetupCullFaceFront: mode = VK_CULL_MODE_FRONT_BIT; break;
		case kPrimitiveSetupCullFaceBack:  mode = VK_CULL_MODE_BACK_BIT; break;
		case kPrimitiveSetupCullFaceFrontAndBack: mode = VK_CULL_MODE_FRONT_AND_BACK; break;
		}

		// clang-format on
		return mode;
	}

	VkImageType convertTextureType(TextureType textureType)
	{
		VkImageType type = VK_IMAGE_TYPE_MAX_ENUM;

		// clang-format off

		switch (textureType)
		{
		case Gnm::kTextureType1d: type = VK_IMAGE_TYPE_1D; break;
		case Gnm::kTextureType2d: type = VK_IMAGE_TYPE_2D; break;
		case Gnm::kTextureType3d: type = VK_IMAGE_TYPE_3D; break;
		case Gnm::kTextureTypeCubemap:
		case Gnm::kTextureType1dArray:
		case Gnm::kTextureType2dArray:
		case Gnm::kTextureType2dMsaa:
		case Gnm::kTextureType2dArrayMsaa:
		default:
			LOG_ASSERT(false, "texture type not supported.");
			break;
		}

		// clang-format on

		return type;
	}

	VkImageViewType convertTextureTypeView(TextureType textureType)
	{
		VkImageViewType type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;

		// clang-format off

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

		// clang-format on

		return type;
	}

	VkFilter convertFilterMode(FilterMode mode)
	{
		VkFilter filter = VK_FILTER_MAX_ENUM;
		// clang-format off

		switch (mode)
		{
		case Gnm::kFilterModePoint:		filter = VK_FILTER_NEAREST; break;
		case Gnm::kFilterModeBilinear:	filter = VK_FILTER_LINEAR; break;
		// Vulkan do not have exact matching filters, use similar ones instead.
		case Gnm::kFilterModeAnisoPoint:	filter = VK_FILTER_NEAREST; break;
		case Gnm::kFilterModeAnisoBilinear: filter = VK_FILTER_LINEAR; break;
		}
		// clang-format on
		return filter;
	}

	VkSamplerMipmapMode convertMipFilterMode(MipFilterMode mode)
	{
		VkSamplerMipmapMode mipmode = VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;

		// clang-format off
		switch (mode)
		{
		case Gnm::kMipFilterModeNone: mipmode = VK_SAMPLER_MIPMAP_MODE_NEAREST; break;
		case Gnm::kMipFilterModePoint: mipmode = VK_SAMPLER_MIPMAP_MODE_NEAREST; break;
		case Gnm::kMipFilterModeLinear: mipmode = VK_SAMPLER_MIPMAP_MODE_LINEAR; break;
		}
		// clang-format on
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
		// clang-format off
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
		// clang-format on
		return op;
	}

	spv::Dim convertTextureTypeToDim(TextureType textureType)
	{
		using namespace spv;

		Dim dim = DimMax;

		// clang-format off

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

		// clang-format on

		return dim;
	}

}  // namespace sce::Gnm::cvt