#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"
#include "GcnConstants.h"

namespace sce::gcn
{
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
		GcnTessInfo* tess;
		GcnXfbInfo*  xfb;
	};

	struct GcnMetaVS
	{
		uint32_t            inputSemanticCount;
		VertexInputSemantic inputSemanticTable[kMaxVertexBufferCount];
	};

	struct GcnMetaPS
	{
		uint32_t placeHolder;
	};

	struct GcnMetaCS
	{
		uint32_t computeNumThreadX;
		uint32_t computeNumThreadY;
		uint32_t computeNumThreadZ;
	};

	/**
	 * \brief Shader meta information
	 * 
	 * Stores some meta information of the compiled
	 * shader binary file but is ripped while uploading
	 * to GPU.
	 * These meta information is provided at runtime.
	 * 
	 */
	union GcnShaderMeta
	{
		GcnMetaVS vs;
		GcnMetaPS ps;
		GcnMetaCS cs;
	};

}  // namespace sce::gcn