#pragma once

#include "GnmCommon.h"
#include "GnmShaderMeta.h"
#include "GnmBuffer.h"

#include "../Gve/GveContextState.h"
#include "../Gve/GveLimit.h"
#include "../Pssl/PsslEnums.h"
#include "../Pssl/PsslShaderModule.h"
#include "../Pssl/PsslShaderStructure.h"


namespace gve
{;
class GveShader;
class GveImageView;
}  // namespace gve


struct GnmShaderContext
{
	const void*                           code   = nullptr;
	RcPtr<pssl::PsslShaderModule>         shader = nullptr;
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
	GnmIndexBuffer indexBuffer;
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

	std::array<VkClearColorValue, gve::MaxNumRenderTargets> colorClearValues = {};
	VkClearDepthStencilValue                                depthClearValue  = {};
};

struct GnmGraphicsContextState
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

struct GnmComputeContextState
{
	uint32_t placeHolder;
};

struct GnmContextState
{
	GnmGraphicsContextState gp;
	GnmComputeContextState  cp;
};