#include "VltContext.h"

#include "VltBuffer.h"
#include "VltDevice.h"
#include "VltImage.h"

namespace sce::vlt
{
	VltContext::VltContext(VltDevice* device) :
		m_device(device),
		m_common(&device->m_objects),
		m_execBarriers(VltCmdType::ExecBuffer)
	{
	}

	VltContext::~VltContext()
	{
	}

	void VltContext::beginRecording(const Rc<VltCommandList>& cmdList)
	{
		m_cmd = cmdList;
		m_cmd->beginRecording();

		// The current state of the internal command buffer is
		// undefined, so we have to bind and set up everything
		// before any draw or dispatch command is recorded.
		m_flags.clr(
			VltContextFlag::GpRenderingActive,
			VltContextFlag::GpXfbActive,
			VltContextFlag::GpClearRenderTargets);

		m_flags.set(
			VltContextFlag::GpDirtyPipeline,
			VltContextFlag::GpDirtyPipelineState,
			VltContextFlag::GpDirtyResources,
			VltContextFlag::GpDirtyVertexBuffers,
			VltContextFlag::GpDirtyIndexBuffer,
			VltContextFlag::GpDirtyXfbBuffers,
			VltContextFlag::GpDirtyBlendConstants,
			VltContextFlag::GpDirtyStencilRef,
			VltContextFlag::GpDirtyViewport,
			VltContextFlag::GpDirtyDepthBias,
			VltContextFlag::GpDirtyDepthBounds,
			VltContextFlag::CpDirtyPipeline,
			VltContextFlag::CpDirtyPipelineState,
			VltContextFlag::CpDirtyResources,
			VltContextFlag::DirtyDrawBuffer);
	}

	Rc<VltCommandList> VltContext::endRecording()
	{
		m_execBarriers.recordCommands(m_cmd);

		m_cmd->endRecording();
		return std::exchange(m_cmd, nullptr);
	}

	void VltContext::bindRenderTarget(
		uint32_t             slot,
		const VltAttachment& target)
	{
		m_state.cb.renderTargets.color[slot] = target;
	}
	
	void VltContext::bindDepthRenderTarget(
		const VltAttachment& depthTarget)
	{
		m_state.cb.renderTargets.depth = depthTarget;
	}

	void VltContext::transformImage(
		const Rc<VltImage>&            dstImage,
		const VkImageSubresourceRange& dstSubresources,
		VkImageLayout                  srcLayout,
		VkImageLayout                  dstLayout)
	{
		if (srcLayout != dstLayout)
		{
			m_execBarriers.recordCommands(m_cmd);

			m_execBarriers.accessImage(
				dstImage, dstSubresources,
				srcLayout,
				dstImage->info().stages,
				dstImage->info().access,
				dstLayout,
				dstImage->info().stages,
				dstImage->info().access);

			m_cmd->trackResource<VltAccess::Write>(dstImage);
		}
	}

	void VltContext::transformImage(
		const Rc<VltImage>&            dstImage,
		const VkImageSubresourceRange& dstSubresources,
		VkImageLayout                  srcLayout,
		VkPipelineStageFlags2          srcStages,
		VkAccessFlags2                 srcAccess,
		VkImageLayout                  dstLayout,
		VkPipelineStageFlags2          dstStages,
		VkAccessFlags2                 dstAccess)
	{
		if (srcLayout != dstLayout)
		{
			m_execBarriers.recordCommands(m_cmd);

			m_execBarriers.accessImage(
				dstImage, dstSubresources,
				srcLayout,
				srcStages,
				srcAccess,
				dstLayout,
				dstStages,
				dstAccess);

			m_cmd->trackResource<VltAccess::Write>(dstImage);
		}
	}

	template <bool Indexed, bool Indirect>
	bool VltContext::commitGraphicsState()
	{
		if (m_flags.test(VltContextFlag::GpDirtyPipeline))
		{
			if (unlikely(!this->updateGraphicsPipeline()))
				return false;
		}

		return true;
	}

	bool VltContext::updateGraphicsPipeline()
	{
		m_state.gp.pipeline = lookupGraphicsPipeline(m_state.gp.shaders);
		if (unlikely(m_state.gp.pipeline == nullptr))
		{
			m_state.gp.flags = VltGraphicsPipelineFlags();
			return false;
		}

		m_flags.clr(VltContextFlag::GpDirtyPipeline);
		return true;
	}

	VltGraphicsPipeline* VltContext::lookupGraphicsPipeline(
		const VltGraphicsPipelineShaders& shaders)
	{
		auto idx = shaders.hash() % m_gpLookupCache.size();

		if (unlikely(!m_gpLookupCache[idx] || !shaders.eq(m_gpLookupCache[idx]->shaders())))
			m_gpLookupCache[idx] = m_common->pipelineManager().createGraphicsPipeline(shaders);

		return m_gpLookupCache[idx];
	}

	void VltContext::setViewports(
		uint32_t          viewportCount,
		const VkViewport* viewports)
	{
		if (m_state.gp.state.rs.viewportCount() != viewportCount)
		{
			m_state.gp.state.rs.setViewportCount(viewportCount);
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}

		for (uint32_t i = 0; i < viewportCount; i++)
		{
			m_state.vp.viewports[i] = viewports[i];

			// Vulkan viewports are not allowed to have a width
			// of zero (but zero height is allowed),
			// so we fall back to a dummy viewport
			// and instead set an empty scissor rect, which is legal.
			if (viewports[i].width == 0.0f)
			{
				m_state.vp.viewports[i] = VkViewport{
					0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
				};
				m_state.vp.scissorRects[i] = VkRect2D{
					VkOffset2D{ 0, 0 },
					VkExtent2D{ 0, 0 }
				};
			}
		}

		m_flags.set(VltContextFlag::GpDirtyViewport);
	}

	void VltContext::setScissors(
		uint32_t        scissorCount,
		const VkRect2D* scissorRects)
	{
		// Assume count of scissor and viewport are always equal.
		// In fact, these's only one scissor for Gnm
		if (m_state.gp.state.rs.viewportCount() != scissorCount)
		{
			m_state.gp.state.rs.setViewportCount(scissorCount);
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}

		for (uint32_t i = 0; i < scissorCount; i++)
		{
			m_state.vp.scissorRects[i] = scissorRects[i];
		}

		m_flags.set(VltContextFlag::GpDirtyScissor);
	}

	void VltContext::bindShader(
		VkShaderStageFlagBits stage,
		const Rc<VltShader>&  shader)
	{
		Rc<VltShader>* shaderStage;

		// clang-format off
		switch (stage) 
		{
		  case VK_SHADER_STAGE_VERTEX_BIT:                  shaderStage = &m_state.gp.shaders.vs;  break;
		  case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    shaderStage = &m_state.gp.shaders.tcs; break;
		  case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: shaderStage = &m_state.gp.shaders.tes; break;
		  case VK_SHADER_STAGE_GEOMETRY_BIT:                shaderStage = &m_state.gp.shaders.gs;  break;
		  case VK_SHADER_STAGE_FRAGMENT_BIT:                shaderStage = &m_state.gp.shaders.fs;  break;
		  case VK_SHADER_STAGE_COMPUTE_BIT:                 shaderStage = &m_state.cp.shaders.cs;  break;
		  default: return;
		}
		// clang-format on

		*shaderStage = shader;

		if (stage == VK_SHADER_STAGE_COMPUTE_BIT)
		{
			m_flags.set(
				VltContextFlag::CpDirtyPipeline,
				VltContextFlag::CpDirtyPipelineState,
				VltContextFlag::CpDirtyResources);
		}
		else
		{
			m_flags.set(
				VltContextFlag::GpDirtyPipeline,
				VltContextFlag::GpDirtyPipelineState,
				VltContextFlag::GpDirtyResources);
		}
	}

}  // namespace sce::vlt