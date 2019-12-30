#pragma once

#include "GveCommon.h"
#include "GveShader.h"
#include "GveHash.h"
#include "GvePipelineState.h"
#include "UtilSync.h"

#include <vector>

namespace gve
{;

class GveRenderPass;
class GvePipelineManager;
class GvePipelineLayout;


struct GveGraphicsPipelineShaders
{
	RcPtr<GveShader> vs;
	RcPtr<GveShader> fs;

	bool operator == (const GveGraphicsPipelineShaders& other) const
	{
		return *vs == *other.vs &&
			*fs == *other.fs;
	}

	GveHashState hash() const
	{
		GveHashState hash;
		hash.add(vs->key().toUint64());
		hash.add(fs->key().toUint64());
		return hash;
	}
};


class GveGraphicsPipelineInstance
{
public:
	GveGraphicsPipelineInstance(VkPipeline pipeline, 
		const GveGraphicsPipelineStateInfo& state, 
		const GveRenderPass& rp);
	~GveGraphicsPipelineInstance();

	VkPipeline pipeline();

	bool isCompatible(const GveGraphicsPipelineStateInfo& state, const GveRenderPass& rp) const;

private:
	VkPipeline m_pipeline;
	GveGraphicsPipelineStateInfo m_state;
	const GveRenderPass* m_renderPass;
};

///

class GveGraphicsPipeline
{
public:
	GveGraphicsPipeline(GvePipelineManager* pipeMgr, 
		const GveGraphicsPipelineShaders& shaders);
	~GveGraphicsPipeline();

	VkPipeline getPipelineHandle(const GveGraphicsPipelineStateInfo& state, const GveRenderPass& rp);

	GvePipelineLayout* getLayout() const;

private:
	GveGraphicsPipelineInstance* findInstance(const GveGraphicsPipelineStateInfo& state, const GveRenderPass& rp);
	GveGraphicsPipelineInstance* createInstance(const GveGraphicsPipelineStateInfo& state, const GveRenderPass& rp);
	
private:
	
	GvePipelineManager* m_pipelineManager;
	GveGraphicsPipelineShaders m_shaders;

	GveDescriptorSlotMap m_resSlotMap;
	GvePipelineLayout* m_layout;

	Spinlock m_mutex;
	std::vector<GveGraphicsPipelineInstance> m_pipelines;
};


}  // namespace gve
