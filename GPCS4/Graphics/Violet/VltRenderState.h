#pragma once

#include "UtilBit.h"
#include "VltBindMask.h"
#include "VltCommon.h"
#include "VltUtil.h"
#include <cstring>

namespace sce::vlt
{
	/**
     * \brief Packed input assembly state
     *
     * Stores the primitive topology
     * and primitive restart info.
     */
	class VltIaInfo
	{

	public:
		VltIaInfo() = default;

		VltIaInfo(
			VkPrimitiveTopology primitiveTopology,
			VkBool32            primitiveRestart,
			uint32_t            patchVertexCount) :
			m_primitiveTopology(uint16_t(primitiveTopology)),
			m_primitiveRestart(uint16_t(primitiveRestart)),
			m_patchVertexCount(uint16_t(patchVertexCount)),
			m_reserved(0)
		{
		}

		VkPrimitiveTopology primitiveTopology() const
		{
			return m_primitiveTopology <= VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
					   ? VkPrimitiveTopology(m_primitiveTopology)
					   : VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
		}

		VkBool32 primitiveRestart() const
		{
			return VkBool32(m_primitiveRestart);
		}

		uint32_t patchVertexCount() const
		{
			return m_patchVertexCount;
		}

	private:
		uint16_t m_primitiveTopology : 4;
		uint16_t m_primitiveRestart : 1;
		uint16_t m_patchVertexCount : 6;
		uint16_t m_reserved : 5;
	};

	/**
     * \brief Packed input layout metadata
     * 
     * Stores the number of vertex attributes
     * and bindings in one byte each.
     */
	class VltIlInfo
	{

	public:
		VltIlInfo() = default;

		VltIlInfo(
			uint32_t attributeCount,
			uint32_t bindingCount) :
			m_attributeCount(uint8_t(attributeCount)),
			m_bindingCount(uint8_t(bindingCount))
		{
		}

		uint32_t attributeCount() const
		{
			return m_attributeCount;
		}

		uint32_t bindingCount() const
		{
			return m_bindingCount;
		}

	private:
		uint8_t m_attributeCount;
		uint8_t m_bindingCount;
	};

	/**
     * \brief Packed vertex attribute
     *
     * Stores a vertex attribute description. Assumes
     * that all vertex formats have numerical values
     * of 127 or less (i.e. fit into 7 bits).
     */
	class VltIlAttribute
	{

	public:
		VltIlAttribute() = default;

		VltIlAttribute(
			uint32_t location,
			uint32_t binding,
			VkFormat format,
			uint32_t offset) :
			m_location(uint32_t(location)),
			m_binding(uint32_t(binding)),
			m_format(uint32_t(format)),
			m_offset(uint32_t(offset)),
			m_reserved(0)
		{
		}

		uint32_t location() const
		{
			return m_location;
		}

		uint32_t binding() const
		{
			return m_binding;
		}

		VkFormat format() const
		{
			return VkFormat(m_format);
		}

		uint32_t offset() const
		{
			return m_offset;
		}

		VkVertexInputAttributeDescription description() const
		{
			VkVertexInputAttributeDescription result;
			result.location = m_location;
			result.binding  = m_binding;
			result.format   = VkFormat(m_format);
			result.offset   = m_offset;
			return result;
		}

	private:
		uint32_t m_location : 5;
		uint32_t m_binding : 5;
		uint32_t m_format : 7;
		uint32_t m_offset : 11;
		uint32_t m_reserved : 4;
	};

	/**
     * \brief Packed vertex binding
     *
     * Stores a vertex binding description,
     * including the 32-bit divisor.
     */
	class VltIlBinding
	{

	public:
		VltIlBinding() = default;

		VltIlBinding(
			uint32_t          binding,
			uint32_t          stride,
			VkVertexInputRate inputRate,
			uint32_t          divisor) :
			m_binding(uint32_t(binding)),
			m_stride(uint32_t(stride)),
			m_inputRate(uint32_t(inputRate)),
			m_reserved(0),
			m_divisor(divisor)
		{
		}

		uint32_t binding() const
		{
			return m_binding;
		}

		uint32_t stride() const
		{
			return m_stride;
		}

		VkVertexInputRate inputRate() const
		{
			return VkVertexInputRate(m_inputRate);
		}

		uint32_t divisor() const
		{
			return m_divisor;
		}

		VkVertexInputBindingDescription description() const
		{
			VkVertexInputBindingDescription result;
			result.binding   = m_binding;
			result.stride    = m_stride;
			result.inputRate = VkVertexInputRate(m_inputRate);
			return result;
		}

		void setStride(uint32_t stride)
		{
			m_stride = stride;
		}

	private:
		uint32_t m_binding : 5;
		uint32_t m_stride : 12;
		uint32_t m_inputRate : 1;
		uint32_t m_reserved : 14;
		uint32_t m_divisor;
	};

	/**
     * \brief Packed rasterizer state
     *
     * Stores a bunch of flags and parameters
     * related to rasterization in four bytes.
     */
	class VltRsInfo
	{

	public:
		VltRsInfo() = default;

		VltRsInfo(
			VkBool32           depthClipEnable,
			VkBool32           depthBiasEnable,
			VkPolygonMode      polygonMode,
			VkCullModeFlags    cullMode,
			VkFrontFace        frontFace,
			uint32_t           viewportCount,
			VkSampleCountFlags sampleCount) :
			m_depthClipEnable(uint32_t(depthClipEnable)),
			m_depthBiasEnable(uint32_t(depthBiasEnable)),
			m_polygonMode(uint32_t(polygonMode)),
			m_cullMode(uint32_t(cullMode)),
			m_frontFace(uint32_t(frontFace)),
			m_viewportCount(uint32_t(viewportCount)),
			m_sampleCount(uint32_t(sampleCount)),
			m_reserved(0)
		{
		}

		VkBool32 depthClipEnable() const
		{
			return VkBool32(m_depthClipEnable);
		}

		VkBool32 depthBiasEnable() const
		{
			return VkBool32(m_depthBiasEnable);
		}

		VkPolygonMode polygonMode() const
		{
			return VkPolygonMode(m_polygonMode);
		}

		VkCullModeFlags cullMode() const
		{
			return VkCullModeFlags(m_cullMode);
		}

		VkFrontFace frontFace() const
		{
			return VkFrontFace(m_frontFace);
		}

		uint32_t viewportCount() const
		{
			return m_viewportCount;
		}

		VkSampleCountFlags sampleCount() const
		{
			return VkSampleCountFlags(m_sampleCount);
		}

		void setViewportCount(uint32_t viewportCount)
		{
			m_viewportCount = viewportCount;
		}

	private:
		uint32_t m_depthClipEnable : 1;
		uint32_t m_depthBiasEnable : 1;
		uint32_t m_polygonMode : 2;
		uint32_t m_cullMode : 2;
		uint32_t m_frontFace : 1;
		uint32_t m_viewportCount : 5;
		uint32_t m_sampleCount : 5;
		uint32_t m_reserved : 15;
	};

	/**
     * \brief Packed multisample info
     *
     * Stores the sample mask, sample count override
     * and alpha-to-coverage state in four bytes.
     */
	class VltMsInfo
	{

	public:
		VltMsInfo() = default;

		VltMsInfo(
			VkSampleCountFlags sampleCount,
			uint32_t           sampleMask,
			VkBool32           enableAlphaToCoverage) :
			m_sampleCount(uint16_t(sampleCount)),
			m_enableAlphaToCoverage(uint16_t(enableAlphaToCoverage)),
			m_reserved(0),
			m_sampleMask(uint16_t(sampleMask))
		{
		}

		VkSampleCountFlags sampleCount() const
		{
			return VkSampleCountFlags(m_sampleCount);
		}

		uint32_t sampleMask() const
		{
			return m_sampleMask;
		}

		VkBool32 enableAlphaToCoverage() const
		{
			return VkBool32(m_enableAlphaToCoverage);
		}

		void setSampleCount(VkSampleCountFlags sampleCount)
		{
			m_sampleCount = uint16_t(sampleCount);
		}

	private:
		uint16_t m_sampleCount : 5;
		uint16_t m_enableAlphaToCoverage : 1;
		uint16_t m_reserved : 10;
		uint16_t m_sampleMask;
	};

	/**
     * \brief Packed depth-stencil metadata
     *
     * Stores some flags and the depth-compare op in
     * two bytes. Stencil ops are stored separately.
     */
	class VltDsInfo
	{

	public:
		VltDsInfo() = default;

		VltDsInfo(
			VkBool32    enableDepthTest,
			VkBool32    enableDepthWrite,
			VkBool32    enableDepthBoundsTest,
			VkBool32    enableStencilTest,
			VkCompareOp depthCompareOp) :
			m_enableDepthTest(uint16_t(enableDepthTest)),
			m_enableDepthWrite(uint16_t(enableDepthWrite)),
			m_enableDepthBoundsTest(uint16_t(enableDepthBoundsTest)),
			m_enableStencilTest(uint16_t(enableStencilTest)),
			m_depthCompareOp(uint16_t(depthCompareOp)),
			m_reserved(0)
		{
		}

		VkBool32 enableDepthTest() const
		{
			return VkBool32(m_enableDepthTest);
		}

		VkBool32 enableDepthWrite() const
		{
			return VkBool32(m_enableDepthWrite);
		}

		VkBool32 enableDepthBoundsTest() const
		{
			return VkBool32(m_enableDepthBoundsTest);
		}

		VkBool32 enableStencilTest() const
		{
			return VkBool32(m_enableStencilTest);
		}

		VkCompareOp depthCompareOp() const
		{
			return VkCompareOp(m_depthCompareOp);
		}

		void setEnableDepthBoundsTest(VkBool32 enableDepthBoundsTest)
		{
			m_enableDepthBoundsTest = VkBool32(enableDepthBoundsTest);
		}

	private:
		uint16_t m_enableDepthTest : 1;
		uint16_t m_enableDepthWrite : 1;
		uint16_t m_enableDepthBoundsTest : 1;
		uint16_t m_enableStencilTest : 1;
		uint16_t m_depthCompareOp : 3;
		uint16_t m_reserved : 9;
	};

	/**
     * \brief Packed stencil op
     *
     * Stores various stencil op parameters
     * for one single face in four bytes.
     */
	class VltDsStencilOp
	{

	public:
		VltDsStencilOp() = default;

		VltDsStencilOp(VkStencilOpState state) :
			m_failOp(uint32_t(state.failOp)),
			m_passOp(uint32_t(state.passOp)),
			m_depthFailOp(uint32_t(state.depthFailOp)),
			m_compareOp(uint32_t(state.compareOp)),
			m_reserved(0),
			m_compareMask(uint32_t(state.compareMask)),
			m_writeMask(uint32_t(state.writeMask))
		{
		}

		VkStencilOpState state() const
		{
			VkStencilOpState result;
			result.failOp      = VkStencilOp(m_failOp);
			result.passOp      = VkStencilOp(m_passOp);
			result.depthFailOp = VkStencilOp(m_depthFailOp);
			result.compareOp   = VkCompareOp(m_compareOp);
			result.compareMask = m_compareMask;
			result.writeMask   = m_writeMask;
			result.reference   = 0;
			return result;
		}

	private:
		uint32_t m_failOp : 3;
		uint32_t m_passOp : 3;
		uint32_t m_depthFailOp : 3;
		uint32_t m_compareOp : 3;
		uint32_t m_reserved : 4;
		uint32_t m_compareMask : 8;
		uint32_t m_writeMask : 8;
	};

	
	/**
	 * \brief Packed render target formats
	 *
	 * Compact representation of depth-stencil and color attachments,
	 * as well as the read-only mask for the depth-stencil attachment,
	 * which needs to be known at pipeline compile time.
	 */
	class VltRtInfo
	{

	public:
		VltRtInfo() = default;

		VltRtInfo(
			uint32_t           colorFormatCount,
			const VkFormat*    colorFormats,
			VkFormat           depthStencilFormat,
			VkImageAspectFlags depthStencilReadOnlyAspects) :
			m_packedData(0ull)
		{
			m_packedData |= encodeDepthStencilFormat(depthStencilFormat);
			m_packedData |= encodeDepthStencilAspects(depthStencilReadOnlyAspects);

			for (uint32_t i = 0; i < colorFormatCount; i++)
				m_packedData |= encodeColorFormat(colorFormats[i], i);
		}

		VkFormat getColorFormat(uint32_t index) const
		{
			return decodeColorFormat(m_packedData, index);
		}

		VkFormat getDepthStencilFormat() const
		{
			return decodeDepthStencilFormat(m_packedData);
		}

		VkImageAspectFlags getDepthStencilReadOnlyAspects() const
		{
			return decodeDepthStencilAspects(m_packedData);
		}

	private:
		uint64_t m_packedData;

		static uint64_t encodeDepthStencilAspects(VkImageAspectFlags aspects)
		{
			return uint64_t(aspects) << 61;
		}

		static uint64_t encodeDepthStencilFormat(VkFormat format)
		{
			return format
					   ? (uint64_t(format) - uint64_t(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32)) << 56
					   : (uint64_t(0));
		}

		static uint64_t encodeColorFormat(VkFormat format, uint32_t index)
		{
			uint64_t value = uint64_t(format);

			if (value >= uint64_t(VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT))
			{
				value -= uint64_t(VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT);
				value += uint64_t(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32) + 1;
			}
			else if (value > uint64_t(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32))
			{
				value = 0;
			}

			return value << (7 * index);
		}

		static VkImageAspectFlags decodeDepthStencilAspects(uint64_t value)
		{
			return VkImageAspectFlags((value >> 61) & (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT));
		}

		static VkFormat decodeDepthStencilFormat(uint64_t value)
		{
			value = (value >> 56) & 0x1F;

			return value
					   ? VkFormat(value + uint64_t(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32))
					   : VkFormat(VK_FORMAT_UNDEFINED);
		}

		static VkFormat decodeColorFormat(uint64_t value, uint32_t index)
		{
			value = (value >> (7 * index)) & 0x7F;

			if (value > uint64_t(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32))
			{
				value -= uint64_t(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32) + 1ull;
				value += uint64_t(VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT);
			}

			return VkFormat(value);
		}
	};


	/**
     * \brief Packed color blend metadata
     *
     * Stores the logic op state in two bytes.
     * Blend modes are stored separately.
     */
	class VltCbInfo
	{

	public:
		VltCbInfo() = default;

		VltCbInfo(
			VkBool32  enableLogicOp,
			VkLogicOp logicOp) :
			m_enableLogicOp(uint16_t(enableLogicOp)),
			m_logicOp(uint16_t(logicOp)),
			m_reserved(0)
		{
		}

		VkBool32 enableLogicOp() const
		{
			return VkBool32(m_enableLogicOp);
		}

		VkLogicOp logicOp() const
		{
			return VkLogicOp(m_logicOp);
		}

	private:
		uint16_t m_enableLogicOp : 1;
		uint16_t m_logicOp : 4;
		uint16_t m_reserved : 11;
	};

	/**
     * \brief Packed attachment blend mode
     *
     * Stores blendig parameters for a single
     * color attachment in four bytes.
     */
	class VltCbAttachmentBlend
	{

	public:
		VltCbAttachmentBlend() = default;

		VltCbAttachmentBlend(
			VkBool32              blendEnable,
			VkBlendFactor         srcColorBlendFactor,
			VkBlendFactor         dstColorBlendFactor,
			VkBlendOp             colorBlendOp,
			VkBlendFactor         srcAlphaBlendFactor,
			VkBlendFactor         dstAlphaBlendFactor,
			VkBlendOp             alphaBlendOp,
			VkColorComponentFlags colorWriteMask) :
			m_blendEnable(uint32_t(blendEnable)),
			m_srcColorBlendFactor(uint32_t(srcColorBlendFactor)),
			m_dstColorBlendFactor(uint32_t(dstColorBlendFactor)),
			m_colorBlendOp(uint32_t(colorBlendOp)),
			m_srcAlphaBlendFactor(uint32_t(srcAlphaBlendFactor)),
			m_dstAlphaBlendFactor(uint32_t(dstAlphaBlendFactor)),
			m_alphaBlendOp(uint32_t(alphaBlendOp)),
			m_colorWriteMask(uint32_t(colorWriteMask)),
			m_reserved(0)
		{
		}

		VkBool32 blendEnable() const
		{
			return m_blendEnable;
		}

		VkBlendFactor srcColorBlendFactor() const
		{
			return VkBlendFactor(m_srcColorBlendFactor);
		}

		VkBlendFactor dstColorBlendFactor() const
		{
			return VkBlendFactor(m_dstColorBlendFactor);
		}

		VkBlendOp colorBlendOp() const
		{
			return VkBlendOp(m_colorBlendOp);
		}

		VkBlendFactor srcAlphaBlendFactor() const
		{
			return VkBlendFactor(m_srcAlphaBlendFactor);
		}

		VkBlendFactor dstAlphaBlendFactor() const
		{
			return VkBlendFactor(m_dstAlphaBlendFactor);
		}

		VkBlendOp alphaBlendOp() const
		{
			return VkBlendOp(m_alphaBlendOp);
		}

		VkColorComponentFlags colorWriteMask() const
		{
			return VkColorComponentFlags(m_colorWriteMask);
		}

		void setColorWriteMask(VkColorComponentFlags mask)
		{
			m_colorWriteMask = mask;
		}

		VkPipelineColorBlendAttachmentState state() const
		{
			VkPipelineColorBlendAttachmentState result;
			result.blendEnable         = VkBool32(m_blendEnable);
			result.srcColorBlendFactor = VkBlendFactor(m_srcColorBlendFactor);
			result.dstColorBlendFactor = VkBlendFactor(m_dstColorBlendFactor);
			result.colorBlendOp        = VkBlendOp(m_colorBlendOp);
			result.srcAlphaBlendFactor = VkBlendFactor(m_srcAlphaBlendFactor);
			result.dstAlphaBlendFactor = VkBlendFactor(m_dstAlphaBlendFactor);
			result.alphaBlendOp        = VkBlendOp(m_alphaBlendOp);
			result.colorWriteMask      = VkColorComponentFlags(m_colorWriteMask);
			return result;
		}

	private:
		uint32_t m_blendEnable : 1;
		uint32_t m_srcColorBlendFactor : 5;
		uint32_t m_dstColorBlendFactor : 5;
		uint32_t m_colorBlendOp : 3;
		uint32_t m_srcAlphaBlendFactor : 5;
		uint32_t m_dstAlphaBlendFactor : 5;
		uint32_t m_alphaBlendOp : 3;
		uint32_t m_colorWriteMask : 4;
		uint32_t m_reserved : 1;
	};

	/**
     * \brief Packed attachment swizzle
     *
     * Stores the component mapping for one
     * single color attachment in one byte.
     */
	class VltCbAttachmentSwizzle
	{

	public:
		VltCbAttachmentSwizzle() = default;

		VltCbAttachmentSwizzle(VkComponentMapping mapping) :
			m_r(vutil::getComponentIndex(mapping.r, 0)),
			m_g(vutil::getComponentIndex(mapping.g, 1)),
			m_b(vutil::getComponentIndex(mapping.b, 2)),
			m_a(vutil::getComponentIndex(mapping.a, 3))
		{
		}

		uint32_t rIndex() const
		{
			return m_r;
		}
		uint32_t gIndex() const
		{
			return m_g;
		}
		uint32_t bIndex() const
		{
			return m_b;
		}
		uint32_t aIndex() const
		{
			return m_a;
		}

		VkComponentMapping mapping() const
		{
			VkComponentMapping result;
			result.r = decodeSwizzle(m_r);
			result.g = decodeSwizzle(m_g);
			result.b = decodeSwizzle(m_b);
			result.a = decodeSwizzle(m_a);
			return result;
		}

	private:
		uint8_t m_r : 2;
		uint8_t m_g : 2;
		uint8_t m_b : 2;
		uint8_t m_a : 2;

		static VkComponentSwizzle decodeSwizzle(uint8_t swizzle)
		{
			return VkComponentSwizzle(uint32_t(swizzle) + uint32_t(VK_COMPONENT_SWIZZLE_R));
		}
	};

	/**
       * \brief Specialization constant state
       *
       * Stores the raw 32-bit spec constant values.
       */
	struct VltScInfo
	{
		uint32_t specConstants[VltLimits::MaxNumSpecConstants];
	};

	/**
       * \brief Packed graphics pipeline state
       *
       * Stores a compressed representation of the full
       * graphics pipeline state which is optimized for
       * lookup performance.
       */
	struct alignas(32) VltGraphicsPipelineStateInfo
	{
		VltGraphicsPipelineStateInfo()
		{
			std::memset(this, 0, sizeof(*this));
		}

		VltGraphicsPipelineStateInfo(const VltGraphicsPipelineStateInfo& other)
		{
			std::memcpy(this, &other, sizeof(*this));
		}

		VltGraphicsPipelineStateInfo& operator=(const VltGraphicsPipelineStateInfo& other)
		{
			std::memcpy(this, &other, sizeof(*this));
			return *this;
		}

		bool operator==(const VltGraphicsPipelineStateInfo& other) const
		{
			return util::bit::bcmpeq(this, &other);
		}

		bool operator!=(const VltGraphicsPipelineStateInfo& other) const
		{
			return !util::bit::bcmpeq(this, &other);
		}

		bool useDynamicStencilRef() const
		{
			return ds.enableStencilTest();
		}

		bool useDynamicDepthBias() const
		{
			return rs.depthBiasEnable();
		}

		bool useDynamicDepthBounds() const
		{
			return ds.enableDepthBoundsTest();
		}

		bool useDynamicBlendConstants() const
		{
			bool result = false;

			for (uint32_t i = 0; i < MaxNumRenderTargets && !result; i++)
			{
				result |= cbBlend[i].blendEnable() &&
						  (vutil::isBlendConstantBlendFactor(cbBlend[i].srcColorBlendFactor()) ||
						   vutil::isBlendConstantBlendFactor(cbBlend[i].dstColorBlendFactor()) ||
						   vutil::isBlendConstantBlendFactor(cbBlend[i].srcAlphaBlendFactor()) ||
						   vutil::isBlendConstantBlendFactor(cbBlend[i].dstAlphaBlendFactor()));
			}

			return result;
		}

		VltBindingMask         bsBindingMask;
		VltIaInfo              ia;
		VltIlInfo              il;
		VltRsInfo              rs;
		VltMsInfo              ms;
		VltDsInfo              ds;
		VltCbInfo              cb;
		VltRtInfo              rt;
		VltScInfo              sc;
		VltDsStencilOp         dsFront;
		VltDsStencilOp         dsBack;
		VltCbAttachmentSwizzle cbSwizzle[VltLimits::MaxNumRenderTargets];
		VltCbAttachmentBlend   cbBlend[VltLimits::MaxNumRenderTargets];
		VltIlAttribute         ilAttributes[VltLimits::MaxNumVertexAttributes];
		VltIlBinding           ilBindings[VltLimits::MaxNumVertexBindings];
	};

	/**
     * \brief Compute pipeline state info
     */
	struct alignas(32) VltComputePipelineStateInfo
	{
		VltComputePipelineStateInfo()
		{
			std::memset(this, 0, sizeof(*this));
		}

		VltComputePipelineStateInfo(const VltComputePipelineStateInfo& other)
		{
			std::memcpy(this, &other, sizeof(*this));
		}

		VltComputePipelineStateInfo& operator=(const VltComputePipelineStateInfo& other)
		{
			std::memcpy(this, &other, sizeof(*this));
			return *this;
		}

		bool operator==(const VltComputePipelineStateInfo& other) const
		{
			return util::bit::bcmpeq(this, &other);
		}

		bool operator!=(const VltComputePipelineStateInfo& other) const
		{
			return !util::bit::bcmpeq(this, &other);
		}

		VltBindingMask bsBindingMask;
		VltScInfo      sc;
	};

}  // namespace sce::vlt
