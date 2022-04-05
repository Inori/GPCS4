#pragma once

#include "VltCommon.h"
#include "VltCompute.h"
#include "VltGraphics.h"
#include "VltHash.h"

#include <mutex>
#include <unordered_map>

namespace sce::vlt
{
	class VltDevice;

	/**
     * \brief Pipeline count
     * 
     * Stores number of graphics and
     * compute pipelines, individually.
     */
	struct VltPipelineCount
	{
		uint32_t numGraphicsPipelines;
		uint32_t numComputePipelines;
	};

	/**
     * \brief Pipeline manager
     * 
     * Creates and stores graphics pipelines and compute
     * pipelines for each combination of shaders that is
     * used within the application. This is necessary
     * because DXVK does not expose the concept of shader
     * pipeline objects to the client API.
     */
	class VltPipelineManager
	{
		friend class VltComputePipeline;
		friend class VltGraphicsPipeline;

	public:
		VltPipelineManager(
			VltDevice* device);

		~VltPipelineManager();

		/**
         * \brief Retrieves a compute pipeline object
         * 
         * If a pipeline for the given shader stage object
         * already exists, it will be returned. Otherwise,
         * a new pipeline will be created.
         * \param [in] shaders Shaders for the pipeline
         * \returns Compute pipeline object
         */
		VltComputePipeline* createComputePipeline(
			const VltComputePipelineShaders& shaders);

		/**
         * \brief Retrieves a graphics pipeline object
         * 
         * If a pipeline for the given shader stage objects
         * already exists, it will be returned. Otherwise,
         * a new pipeline will be created.
         * \param [in] shaders Shaders for the pipeline
         * \returns Graphics pipeline object
         */
		VltGraphicsPipeline* createGraphicsPipeline(
			const VltGraphicsPipelineShaders& shaders);

		/**
         * \brief Retrieves total pipeline count
         * \returns Number of compute/graphics pipelines
         */
		VltPipelineCount getPipelineCount() const;

	private:
		VltDevice* m_device;

		std::atomic<uint32_t> m_numComputePipelines  = { 0 };
		std::atomic<uint32_t> m_numGraphicsPipelines = { 0 };

		std::mutex m_mutex;

		std::unordered_map<
			VltComputePipelineShaders,
			VltComputePipeline,
			VltHash,
			VltEq>
			m_computePipelines;

		std::unordered_map<
			VltGraphicsPipelineShaders,
			VltGraphicsPipeline,
			VltHash,
			VltEq>
			m_graphicsPipelines;
	};
}  // namespace sce::vlt