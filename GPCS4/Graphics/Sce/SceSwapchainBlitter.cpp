#include "SceSwapchainBlitter.h"

#include "SpirV/SpirvCodeBuffer.h"
#include "Violet/VltContext.h"
#include "Violet/VltDevice.h"
#include "Violet/VltImage.h"

using namespace sce::vlt;
using namespace sce::gcn;

LOG_CHANNEL(Graphic.Sce.SceSwapchainBlitter);

namespace sce
{
	// clang-format off
	static const uint32_t sce_present_vert[]      = 
	{ 
		#include "Shaders/sce_present_vert.h" 
	};

	static const uint32_t sce_present_frag[]      = 
	{
		#include "Shaders/sce_present_frag.h"
	};

	static const uint32_t sce_present_frag_blit[] = 
	{
		#include "Shaders/sce_present_frag_blit.h"
	};
	// clang-format on

	SceSwapchainBlitter::SceSwapchainBlitter(vlt::VltDevice* device) :
		m_device(device)
	{
		this->createSampler();
		this->createShaders();
	}

	SceSwapchainBlitter::~SceSwapchainBlitter()
	{
	}

	void SceSwapchainBlitter::presentImage(
		VltContext*             ctx,
		const Rc<VltImageView>& dstView,
		VkRect2D                dstRect,
		const Rc<VltImageView>& srcView,
		VkRect2D                srcRect)
	{
		// Fix up default present areas if necessary
		if (!dstRect.extent.width || !dstRect.extent.height)
		{
			dstRect.offset = { 0, 0 };
			dstRect.extent = {
				dstView->imageInfo().extent.width,
				dstView->imageInfo().extent.height
			};
		}

		if (!srcRect.extent.width || !srcRect.extent.height)
		{
			srcRect.offset = { 0, 0 };
			srcRect.extent = {
				srcView->imageInfo().extent.width,
				srcView->imageInfo().extent.height
			};
		}

		bool sameSize = dstRect.extent == srcRect.extent;

		if (srcView->imageInfo().sampleCount == VK_SAMPLE_COUNT_1_BIT)
		{
			this->draw(ctx, sameSize ? m_fsCopy : m_fsBlit,
					   dstView, dstRect, srcView, srcRect);
		}
		else if (sameSize)
		{
			LOG_ASSERT(false, "TOOD: support multisample.");
		}
		else
		{
			LOG_ASSERT(false, "TOOD: support resolve image.");
		}
	}

	void SceSwapchainBlitter::draw(
		VltContext*             ctx,
		const Rc<VltShader>&    fs,
		const Rc<VltImageView>& dstView,
		VkRect2D                dstRect,
		const Rc<VltImageView>& srcView,
		VkRect2D                srcRect)
	{
		VltInputAssemblyState iaState;
		iaState.primitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		iaState.primitiveRestart  = VK_FALSE;
		iaState.patchVertexCount  = 0;
		ctx->setInputAssemblyState(iaState);
		ctx->setInputLayout(0, nullptr, 0, nullptr);

		VltRasterizerState rsState;
		rsState.polygonMode      = VK_POLYGON_MODE_FILL;
		rsState.cullMode         = VK_CULL_MODE_BACK_BIT;
		rsState.frontFace        = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rsState.depthClipEnable  = VK_FALSE;
		rsState.depthBiasEnable  = VK_FALSE;
		rsState.sampleCount      = VK_SAMPLE_COUNT_1_BIT;
		rsState.conservativeMode = VK_CONSERVATIVE_RASTERIZATION_MODE_DISABLED_EXT;
		ctx->setRasterizerState(rsState);

		VltMultisampleState msState;
		msState.sampleMask            = 0xffffffff;
		msState.enableAlphaToCoverage = VK_FALSE;
		ctx->setMultisampleState(msState);

		VkStencilOpState stencilOp;
		stencilOp.failOp      = VK_STENCIL_OP_KEEP;
		stencilOp.passOp      = VK_STENCIL_OP_KEEP;
		stencilOp.depthFailOp = VK_STENCIL_OP_KEEP;
		stencilOp.compareOp   = VK_COMPARE_OP_ALWAYS;
		stencilOp.compareMask = 0xFFFFFFFF;
		stencilOp.writeMask   = 0xFFFFFFFF;
		stencilOp.reference   = 0;

		VltDepthStencilState dsState;
		dsState.enableDepthTest   = VK_FALSE;
		dsState.enableDepthWrite  = VK_FALSE;
		dsState.enableStencilTest = VK_FALSE;
		dsState.depthCompareOp    = VK_COMPARE_OP_ALWAYS;
		dsState.stencilOpFront    = stencilOp;
		dsState.stencilOpBack     = stencilOp;
		ctx->setDepthStencilState(dsState);

		VltLogicOpState loState;
		loState.enableLogicOp = VK_FALSE;
		loState.logicOp       = VK_LOGIC_OP_NO_OP;
		ctx->setLogicOpState(loState);

		VltBlendMode blendMode;
		blendMode.enableBlending = VK_FALSE;
		blendMode.colorSrcFactor = VK_BLEND_FACTOR_ONE;
		blendMode.colorDstFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendMode.colorBlendOp   = VK_BLEND_OP_ADD;
		blendMode.alphaSrcFactor = VK_BLEND_FACTOR_ONE;
		blendMode.alphaDstFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendMode.alphaBlendOp   = VK_BLEND_OP_ADD;
		blendMode.writeMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		ctx->setBlendMode(0, blendMode);

		VkViewport viewport;
		viewport.x        = float(dstRect.offset.x);
		viewport.y        = float(dstRect.offset.y);
		viewport.width    = float(dstRect.extent.width);
		viewport.height   = float(dstRect.extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		ctx->setViewports(1, &viewport, &dstRect);

		VltRenderTargets renderTargets;
		renderTargets.color[0].view   = dstView;
		renderTargets.color[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		ctx->bindRenderTargets(renderTargets);

		ctx->clearRenderTarget(
			dstView, VK_IMAGE_ASPECT_COLOR_BIT, VkClearValue());
		ctx->transformImage(
			dstView->image(),
			dstView->subresources(),
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

		ctx->bindResourceSampler(BindingIds::Image, m_samplerPresent);
		ctx->bindResourceView(BindingIds::Image, srcView, nullptr);

		ctx->bindShader(VK_SHADER_STAGE_VERTEX_BIT, m_vs);
		ctx->bindShader(VK_SHADER_STAGE_FRAGMENT_BIT, fs);

		PresenterArgs args;
		args.srcOffset = srcRect.offset;

		if (dstRect.extent == srcRect.extent)
			args.dstOffset = dstRect.offset;
		else
			args.srcExtent = srcRect.extent;

		ctx->pushConstants(0, sizeof(args), &args);

		ctx->draw(3, 1, 0, 0);
	}

	void SceSwapchainBlitter::createSampler()
	{
		VltSamplerCreateInfo samplerInfo;
		samplerInfo.magFilter      = VK_FILTER_LINEAR;
		samplerInfo.minFilter      = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode     = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.mipmapLodBias  = 0.0f;
		samplerInfo.mipmapLodMin   = 0.0f;
		samplerInfo.mipmapLodMax   = 0.0f;
		samplerInfo.useAnisotropy  = VK_FALSE;
		samplerInfo.maxAnisotropy  = 1.0f;
		samplerInfo.addressModeU   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.addressModeV   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.addressModeW   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.compareToDepth = VK_FALSE;
		samplerInfo.compareOp      = VK_COMPARE_OP_ALWAYS;
		samplerInfo.borderColor    = VkClearColorValue();
		samplerInfo.usePixelCoord  = VK_TRUE;
		m_samplerPresent           = m_device->createSampler(samplerInfo);
	}

	void SceSwapchainBlitter::createShaders()
	{
		const SpirvCodeBuffer vsCode(sce_present_vert);
		const SpirvCodeBuffer fsCodeCopy(sce_present_frag);
		const SpirvCodeBuffer fsCodeBlit(sce_present_frag_blit);

		const std::array<VltResourceSlot, 2> fsResourceSlots = { {
			{ BindingIds::Image, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_IMAGE_VIEW_TYPE_2D },
		} };

		m_vs = m_device->createShader(
			VK_SHADER_STAGE_VERTEX_BIT,
			0, nullptr, { 0u, 1u },
			vsCode);

		m_fsCopy = m_device->createShader(
			VK_SHADER_STAGE_FRAGMENT_BIT,
			fsResourceSlots.size(),
			fsResourceSlots.data(),
			{ 0u, 1u, 0u, sizeof(PresenterArgs) },
			fsCodeCopy);

		m_fsBlit = m_device->createShader(
			VK_SHADER_STAGE_FRAGMENT_BIT,
			fsResourceSlots.size(),
			fsResourceSlots.data(),
			{ 1u, 1u, 0u, sizeof(PresenterArgs) },
			fsCodeBlit);
	}
}  // namespace sce