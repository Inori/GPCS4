#pragma once

#include "GnmCommon.h"
#include "GnmShaderMeta.h"

#include "../Gve/GveContextState.h"
#include "../Pssl/PsslEnums.h"
#include "../Pssl/PsslShaderStructure.h"

namespace gve
{;
class GveShader;
class GveImageView;
class GveSharpResourceManager;
}  // namespace gve


struct GnmShaderContext
{
	const void*                           code   = nullptr;
	RcPtr<gve::GveShader>                 shader = nullptr;
	std::vector<pssl::PsslShaderResource> userDataSlotTable;
	GnmShaderMeta                         meta;
};

struct GnmShaderContextGroup
{
	GnmShaderContext vs;
	GnmShaderContext ps;
	GnmShaderContext cs;
};

//////////////////////////////////////////////////////////////////////////

struct GnmInputAssemblerState
{
	VkIndexType indexType = VK_INDEX_TYPE_UINT16;
};

struct GnmVertexShaderState
{
	uint32_t placeHolder;
};

struct GnmHullShaderState
{
	uint32_t placeHolder;
};

struct GnmTessellatorState
{
	uint32_t placeHolder;
};

struct GnmDomainShaderState
{
	uint32_t placeHolder;
};

struct GnmGeometryShaderState
{
	uint32_t placeHolder;
};

struct GnmRasterizationState
{
	uint32_t placeHolder;
};

struct GnmPixelShaderState
{
	uint32_t placeHolder;
};

struct GnmOutputMergerState
{
	gve::GveRenderTargets renderTargets = {};
};


struct GnmContextState
{
	GnmInputAssemblerState ia;
	GnmVertexShaderState   vs;
	GnmHullShaderState     hs;
	GnmTessellatorState    tl;
	GnmDomainShaderState   ds;
	GnmGeometryShaderState gs;
	GnmRasterizationState  rs;
	GnmPixelShaderState    ps;
	GnmOutputMergerState   om;
};