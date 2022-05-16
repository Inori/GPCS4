#pragma once

namespace sce::gcn
{
	struct GcnOptions
	{
		// A GCN wavefront has 64 lanes,
		// but the host GPU may not fit the case,
		// especially for Nvidia GPU,
		// of which a warp has 32 lanes,
		// in such case we need to take smaller 
		// subgroup size into consideration,
		// and separate subgroups while compiling.
		bool separateSubgroup;
	};


	/**
	 * \brief Tessellation info
	 *
	 * Stores the maximum tessellation factor
	 * to export from tessellation shaders.
	 */
	struct GcnTessInfo
	{
		float maxTessFactor;
	};

	/**
	 * \brief Xfb capture entry
	 *
	 * Stores an output variable to capture,
	 * as well as the buffer to write it to.
	 */
	struct GcnXfbEntry
	{
		const char* semanticName;
		uint32_t    semanticIndex;
		uint32_t    componentIndex;
		uint32_t    componentCount;
		uint32_t    streamId;
		uint32_t    bufferId;
		uint32_t    offset;
	};

	/**
	 * \brief Xfb info
	 *
	 * Stores capture entries and output buffer
	 * strides. This structure must only be
	 * defined if \c entryCount is non-zero.
	 */
	struct GcnXfbInfo
	{
		uint32_t    entryCount;
		GcnXfbEntry entries[128];
		uint32_t    strides[4];
		int32_t     rasterizedStream;
	};

	/**
	 * \brief Shader module info
	 *
	 * Stores information which may affect shader compilation.
	 * This data can be supplied by the client API implementation.
	 */
	struct GcnModuleInfo
	{
		GcnOptions options;
	};
}  // namespace sce::gcn