#pragma once

#include "GveCommon.h"
#include "GveShader.h"
#include "GveHash.h"
#include "GveRenderState.h"
#include "SyncHelper.h"

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


class GvePipelineInstance
{
public:
	GvePipelineInstance(VkPipeline pipeline, 
		const GveRenderState& state, 
		GveRenderPass* rp);
	~GvePipelineInstance();

	VkPipeline pipeline();

	bool isCompatible(const GveRenderState& state, const GveRenderPass& rp) const;

private:
	VkPipeline m_pipeline;
	GveRenderState m_state;
	GveRenderPass* m_renderPass;
};

///

class GveGraphicsPipeline
{
public:
	GveGraphicsPipeline(GvePipelineManager* pipeMgr, 
		const GveGraphicsPipelineShaders& shaders);
	~GveGraphicsPipeline();

	VkPipeline getPipelineHandle(const GveRenderState& state, GveRenderPass& rp);

	GvePipelineLayout* getLayout() const;

private:
	GvePipelineInstance* findInstance(const GveRenderState& state, GveRenderPass& rp);
	GvePipelineInstance* createInstance(const GveRenderState& state, GveRenderPass& rp);
private:
	
	GvePipelineManager* m_pipelineManager;
	GveGraphicsPipelineShaders m_shaders;

	GveDescriptorSlotMap m_resSlotMap;
	GvePipelineLayout* m_layout;

	Spinlock m_mutex;
	std::vector<GvePipelineInstance> m_pipelines;
};


}  // namespace gve
