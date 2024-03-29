#pragma once

#include "GcnCommon.h"
#include "GcnShaderBinary.h"
#include "GcnConstants.h"
#include "Gnm/GnmConstant.h"

#include <array>

namespace sce::gcn
{
	// TODO:
	// Currently, we assume the V# T# and S# bound to shader
	// remains permanent during rendering (except for address).
	// But they can be changed in theory,
	// If that happens,
	// we have to to store these information
	// in resource buffers, like push constants
	// or uniform buffers,
	// then parse them at shader runtime.
	// That would be much more complicated.

	struct GcnBufferMeta
	{
		uint32_t               stride;
		uint32_t               numRecords;
		Gnm::BufferFormat      dfmt;
		Gnm::BufferChannelType nfmt;
		bool                   isSwizzle;
		uint32_t               indexStride;
		uint32_t               elementSize;
	};

	struct GcnTextureMeta
	{
		Gnm::TextureType        textureType;
		Gnm::TextureChannelType channelType;
		bool                    isDepth;
	};

	struct GcnMetaCommon
	{
		uint32_t userSgprCount;

		std::array<GcnBufferMeta, 128>  bufferInfos;
		std::array<GcnTextureMeta, 128> textureInfos;
	};

	struct GcnMetaVS : public GcnMetaCommon
	{
		uint32_t                                               inputSemanticCount;
		std::array<VertexInputSemantic, kMaxVertexBufferCount> inputSemanticTable;
	};

	struct GcnMetaPS : public GcnMetaCommon
	{
		uint32_t                                                inputSemanticCount;
		std::array<PixelSemanticMapping, kMaxPsInputUsageCount> semanticMapping;

		bool perspSampleEn;
		bool perspCenterEn;
		bool perspCentroidEn;
		bool perspPullModelEn;
		bool linearSampleEn;
		bool linearCenterEn;
		bool linearCentroidEn;
		bool posXEn;
		bool posYEn;
		bool posZEn;
		bool posWEn;
	};

	struct GcnMetaCS : public GcnMetaCommon
	{
		uint32_t computeNumThreadX;
		uint32_t computeNumThreadY;
		uint32_t computeNumThreadZ;

		bool enableTgidX;
		bool enableTgidY;
		bool enableTgidZ;
		bool enableTgSize;
		bool enableScratch;

		uint32_t threadIdInGroupCount;

		uint32_t ldsSize;
	};

	struct GcnMetaGS : public GcnMetaCommon
	{
	};

	struct GcnMetaHS : public GcnMetaCommon
	{
	};

	struct GcnMetaDS : public GcnMetaCommon
	{
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
		GcnMetaGS gs;
		GcnMetaHS hs;
		GcnMetaDS ds;
	};

}  // namespace sce::gcn