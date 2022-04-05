#pragma once

#include "VltCommon.h"
#include "VltShader.h"
#include "VltState.h"

namespace sce::vlt
{
	class VltDevice;
	class VltPipelineManager;

	/**
     * \brief Shaders used in compute pipelines
     */
	struct VltComputePipelineShaders
	{
		Rc<VltShader> cs;

		bool eq(const VltComputePipelineShaders& other) const
		{
			return cs == other.cs;
		}

		size_t hash() const
		{
			return VltShader::getHash(cs);
		}
	};

	/**
     * \brief Compute pipeline instance
     */
	class VltComputePipelineInstance
	{

	public:
		VltComputePipelineInstance() :
			m_stateVector(),
			m_pipeline(VK_NULL_HANDLE)
		{
		}

		VltComputePipelineInstance(
			const VltComputePipelineStateInfo& state,
			VkPipeline                         pipe) :
			m_stateVector(state),
			m_pipeline(pipe)
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
		VltComputePipelineStateInfo m_stateVector;
		VkPipeline                  m_pipeline;
	};

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

		/**
         * \brief Compiles a pipeline
         * 
         * Asynchronously compiles the given pipeline
         * and stores the result for future use.
         * \param [in] state Pipeline state
         */
		void compilePipeline(
			const VltComputePipelineStateInfo& state);

	private:
		VltDevice*          m_device;
		VltPipelineManager* m_pipeMgr;

		VltComputePipelineShaders m_shaders;
		VltDescriptorSlotMapping  m_slotMapping;

		Rc<VltPipelineLayout> m_layout;

		util::sync::Spinlock                    m_mutex;
		std::vector<VltComputePipelineInstance> m_pipelines;

		VltComputePipelineInstance* createInstance(
			const VltComputePipelineStateInfo& state);

		VltComputePipelineInstance* findInstance(
			const VltComputePipelineStateInfo& state);

		VkPipeline createPipeline(
			const VltComputePipelineStateInfo& state) const;

		void destroyPipeline(
			VkPipeline pipeline);
	};
}  // namespace sce::vlt