#pragma once

#include "VltCommon.h"
#include "VltShader.h"
#include "VltHash.h"
#include "VltPipelineState.h"
#include "UtilSync.h"

#include <vector>

namespace vlt
{;

class VltRenderPass;
class VltPipelineManager;
class VltPipelineLayout;


struct VltGraphicsPipelineShaders
{
	RcPtr<VltShader> vs;
	RcPtr<VltShader> fs;

	bool operator == (const VltGraphicsPipelineShaders& other) const
	{
		return vs == other.vs &&
			fs == other.fs;
	}

	size_t hash() const
	{
		VltHashState hash;
		hash.add(vs->key().toUint64());
		hash.add(fs->key().toUint64());
		return hash;
	}
};


class VltGraphicsPipelineInstance
{
public:
	VltGraphicsPipelineInstance(
		VkPipeline                          pipeline,
		const VltGraphicsPipelineStateInfo& state,
		const VltRenderPass&                rp);
	~VltGraphicsPipelineInstance();

	VkPipeline pipeline() const;

	bool isCompatible(
		const VltGraphicsPipelineStateInfo& state,
		const VltRenderPass&                rp) const;

private:
	VkPipeline                   m_pipeline;
	VltGraphicsPipelineStateInfo m_state;
	const VltRenderPass*         m_renderPass;
};

///

class VltGraphicsPipeline
{
public:
	VltGraphicsPipeline(
		VltPipelineManager*               pipeMgr,
		const VltGraphicsPipelineShaders& shaders);
	~VltGraphicsPipeline();

	VkPipeline getPipelineHandle(
		const VltGraphicsPipelineStateInfo& state,
		const VltRenderPass&                rp);

	VltPipelineLayout* layout() const;

private:
	VltGraphicsPipelineInstance* createInstance(
		const VltGraphicsPipelineStateInfo& state,
		const VltRenderPass&                rp);

	VltGraphicsPipelineInstance* findInstance(
		const VltGraphicsPipelineStateInfo& state,
		const VltRenderPass&                rp);

	VkPipeline createPipeline(const VltGraphicsPipelineStateInfo& state,
							  const VltRenderPass&                rp);

	void destroyPipeline(VkPipeline pipeline) const;

private:
	VltPipelineManager*        m_pipelineManager;
	VltGraphicsPipelineShaders m_shaders;

	VltDescriptorSlotMap     m_slotMap;
	RcPtr<VltPipelineLayout> m_layout;

	sync::Spinlock                           m_mutex;
	std::vector<VltGraphicsPipelineInstance> m_pipelines;
};


}  // namespace vlt
