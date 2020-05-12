#include "VltComputePipeline.h"

#include "VltDevice.h"
#include "VltPipelineLayout.h"

LOG_CHANNEL(Graphic.Violet.VltComputePipeline);

namespace vlt
{;


VltComputePipeline::VltComputePipeline(
	VltPipelineManager*       pipeMgr,
	VltComputePipelineShaders shaders) :
	m_pipeMgr(pipeMgr),
	m_shaders(std::move(shaders))
{
	m_shaders.cs->defineResourceSlots(m_slotMap);

	m_layout = new VltPipelineLayout(
		pipeMgr->m_device,
		m_slotMap,
		VK_PIPELINE_BIND_POINT_COMPUTE);
}

VltComputePipeline::~VltComputePipeline()
{
	for (const auto& instance : m_pipelines)
	{
		destroyPipeline(instance.pipeline());
	}
}

VkPipeline VltComputePipeline::getPipelineHandle(
	const VltComputePipelineStateInfo& state)
{
	VkPipeline pipeline = VK_NULL_HANDLE;

	do
	{
		std::lock_guard<sync::Spinlock> lock(m_mutex);

		auto instance = findInstance(state);
		if (instance)
		{
			pipeline = instance->pipeline();
			break;
		}

		instance = createInstance(state);
		if (!instance)
		{
			break;
		}
		pipeline = instance->pipeline();

	} while (false);
	return pipeline;
}

VltComputePipelineInstance* VltComputePipeline::createInstance(
	const VltComputePipelineStateInfo& state)
{
	VkPipeline pipeline = createPipeline(state);
	return &m_pipelines.emplace_back(pipeline, state);
}

VltComputePipelineInstance* VltComputePipeline::findInstance(
	const VltComputePipelineStateInfo& state)
{
	VltComputePipelineInstance* instance = nullptr;
	for (auto& pipeInst : m_pipelines)
	{
		if (pipeInst.isCompatible(state))
		{
			instance = &pipeInst;
			break;
		}
	}
	return instance;
}

VkPipeline VltComputePipeline::createPipeline(
	const VltComputePipelineStateInfo& state) const
{
	VkPipeline pipeline = VK_NULL_HANDLE;
	do 
	{
		VkDevice device = *(m_pipeMgr->m_device);

		auto csm = m_shaders.cs->createShaderModule(m_pipeMgr->m_device, m_slotMap);

		VkComputePipelineCreateInfo info;
		info.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		info.pNext              = nullptr;
		info.flags              = 0;
		info.stage              = csm.stageInfo(nullptr);
		info.layout             = m_layout->pipelineLayout();
		info.basePipelineHandle = VK_NULL_HANDLE;
		info.basePipelineIndex  = -1;

		if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline) != VK_SUCCESS)
		{
			LOG_ERR("failed to create compute pipeline!");
			break;
		}

	} while (false);
	return pipeline;
}

void VltComputePipeline::destroyPipeline(
	VkPipeline pipeline)
{
	VkDevice device = *(m_pipeMgr->m_device);
	vkDestroyPipeline(device, pipeline, nullptr);
}

}  // namespace vlt

