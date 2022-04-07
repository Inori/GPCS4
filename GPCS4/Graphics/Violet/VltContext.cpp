#include "VltContext.h"
#include "VltDevice.h"
#include "VltBuffer.h"
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
	}

	Rc<VltCommandList> VltContext::endRecording()
	{
		m_execBarriers.recordCommands(m_cmd);

		m_cmd->endRecording();
		return std::exchange(m_cmd, nullptr);
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

}  // namespace sce::vlt