#pragma once

#include "VltCommon.h"
#include "VltShader.h"
#include "VltHash.h"
#include "VltPipelineState.h"
#include "UtilSync.h"

namespace vlt
{;

class VltPipelineManager;
class VltPipelineLayout;


struct VltComputePipelineShaders
{
	RcPtr<VltShader> cs;

	bool operator==(const VltComputePipelineShaders& other) const
	{
		return cs == other.cs;
	}

	size_t hash() const
	{
		return static_cast<size_t>(cs->key().toUint64());
	}
};


/**
 * \brief Compute pipeline instance
 */
class VltComputePipelineInstance
{

public:
	VltComputePipelineInstance() :
		m_pipeline(VK_NULL_HANDLE),
		m_stateVector()
	{
	}

	VltComputePipelineInstance(
		VkPipeline                         pipe,
		const VltComputePipelineStateInfo& state) :
		m_pipeline(pipe),
		m_stateVector(state)
	{
	}

	/**
     * \brief Checks for matching pipeline state
     * 
     * \param [in] stateVector Graphics pipeline state
     * \param [in] renderPass Render pass handle
     * \returns \c true if the specialization is compatible
     */
	bool isCompatible(const VltComputePipelineStateInfo& state) const
	{
		return m_stateVector == state;
	}

	/**
     * \brief Retrieves pipeline
     * \returns The pipeline handle
     */
	VkPipeline pipeline() const
	{
		return m_pipeline;
	}

private:
	VkPipeline                  m_pipeline;
	VltComputePipelineStateInfo m_stateVector;
};

///

/**
   * \brief Compute pipeline
   * 
   * Stores a compute pipeline object and the corresponding
   * pipeline layout. Unlike graphics pipelines, compute
   * pipelines do not need to be recompiled against any sort
   * of pipeline state.
   */
class VltComputePipeline
{

public:
	VltComputePipeline(
		VltPipelineManager*       pipeMgr,
		VltComputePipelineShaders shaders);

	~VltComputePipeline();

	/**
     * \brief Shaders used by the pipeline
     * \returns Shaders used by the pipeline
     */
	const VltComputePipelineShaders& shaders() const
	{
		return m_shaders;
	}

	/**
     * \brief Pipeline layout
     * 
     * Stores the pipeline layout and the descriptor set
     * layout, as well as information on the resource
     * slots used by the pipeline.
     * \returns Pipeline layout
     */
	VltPipelineLayout* layout() const
	{
		return m_layout.ptr();
	}

	/**
     * \brief Retrieves pipeline handle
     * 
     * \param [in] state Pipeline state
     * \returns Pipeline handle
     */
	VkPipeline getPipelineHandle(
		const VltComputePipelineStateInfo& state);

private:
	VltComputePipelineInstance* createInstance(
		const VltComputePipelineStateInfo& state);

	VltComputePipelineInstance* findInstance(
		const VltComputePipelineStateInfo& state);

	VkPipeline createPipeline(
		const VltComputePipelineStateInfo& state) const;

	void destroyPipeline(
		VkPipeline pipeline);

private:
	VltPipelineManager* m_pipeMgr;

	VltComputePipelineShaders m_shaders;
	VltDescriptorSlotMap      m_slotMap;

	RcPtr<VltPipelineLayout> m_layout;

	sync::Spinlock                          m_mutex;
	std::vector<VltComputePipelineInstance> m_pipelines;
};




}  // namespace vlt