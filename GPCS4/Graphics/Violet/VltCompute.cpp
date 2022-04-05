#include "VltCompute.h"

#include "VltDevice.h"
#include "VltPipeManager.h"

namespace sce::vlt
{

	VltComputePipeline::VltComputePipeline(
		VltPipelineManager*       pipeMgr,
		VltComputePipelineShaders shaders) :
		m_device(pipeMgr->m_device),
		m_pipeMgr(pipeMgr),
		m_shaders(std::move(shaders))
	{
		m_shaders.cs->defineResourceSlots(m_slotMapping);

		m_layout = new VltPipelineLayout(m_device,
										 m_slotMapping, VK_PIPELINE_BIND_POINT_COMPUTE);
	}

	VltComputePipeline::~VltComputePipeline()
	{
		for (const auto& instance : m_pipelines)
			this->destroyPipeline(instance.pipeline());
	}

	VkPipeline VltComputePipeline::getPipelineHandle(
		const VltComputePipelineStateInfo& state)
	{
		VltComputePipelineInstance* instance = nullptr;

		{
			std::lock_guard<util::sync::Spinlock> lock(m_mutex);

			instance = this->findInstance(state);

			if (instance)
				return instance->pipeline();

			// If no pipeline instance exists with the given state
			// vector, create a new one and add it to the list.
			instance = this->createInstance(state);
		}

		if (!instance)
			return VK_NULL_HANDLE;

		return instance->pipeline();
	}

	void VltComputePipeline::compilePipeline(
		const VltComputePipelineStateInfo& state)
	{
		std::lock_guard<util::sync::Spinlock> lock(m_mutex);

		if (!this->findInstance(state))
			this->createInstance(state);
	}

	VltComputePipelineInstance* VltComputePipeline::createInstance(
		const VltComputePipelineStateInfo& state)
	{
		VkPipeline newPipelineHandle = this->createPipeline(state);

		m_pipeMgr->m_numComputePipelines += 1;
		return &m_pipelines.emplace_back(state, newPipelineHandle);
	}

	VltComputePipelineInstance* VltComputePipeline::findInstance(
		const VltComputePipelineStateInfo& state)
	{
		for (auto& instance : m_pipelines)
		{
			if (instance.isCompatible(state))
				return &instance;
		}

		return nullptr;
	}

	VkPipeline VltComputePipeline::createPipeline(
		const VltComputePipelineStateInfo& state) const
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		if (Logger::logLevel() <= LogLevel::Debug)
		{
			Logger::debug("Compiling compute pipeline...");
			Logger::debug(util::str::formatex("  cs  : ", m_shaders.cs->debugName()));
		}

		DxvkShaderModuleCreateInfo moduleInfo;
		moduleInfo.fsDualSrcBlend = false;

		auto csm = m_shaders.cs->createShaderModule(m_device, m_slotMapping, moduleInfo);

		VkComputePipelineCreateInfo info;
		info.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		info.pNext              = nullptr;
		info.flags              = 0;
		info.stage              = csm.stageInfo(nullptr);
		info.layout             = m_layout->pipelineLayout();
		info.basePipelineHandle = VK_NULL_HANDLE;
		info.basePipelineIndex  = -1;

		VkPipeline pipeline = VK_NULL_HANDLE;
		if (vkCreateComputePipelines(m_device->handle(),
									 VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS)
		{
			Logger::err("DxvkComputePipeline: Failed to compile pipeline");
			Logger::err(util::str::formatex("  cs  : ", m_shaders.cs->debugName()));
			return VK_NULL_HANDLE;
		}

		return pipeline;
	}

	void VltComputePipeline::destroyPipeline(VkPipeline pipeline)
	{
		vkDestroyPipeline(m_device->handle(), pipeline, nullptr);
	}

}  // namespace sce::vlt