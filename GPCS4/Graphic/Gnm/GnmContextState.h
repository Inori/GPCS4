#pragma once

#include "GnmBuffer.h"
#include "GnmCommon.h"
#include "GnmShaderMeta.h"

#include "../Violet/VltContextState.h"
#include "../Violet/VltLimit.h"
#include "../Pssl/PsslEnums.h"
#include "../Pssl/PsslShaderModule.h"
#include "../Pssl/PsslShaderStructure.h"

namespace vlt
{
;
class VltShader;
class VltImageView;
}  // namespace vlt

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
	vlt::VltRenderTargets                                 renderTargets = {};
	std::array<GnmRenderTarget, vlt::MaxNumRenderTargets> colorTargets  = {};
	GnmDepthRenderTarget                                  depthTarget   = {};

	std::array<VkClearColorValue, vlt::MaxNumRenderTargets> colorClearValues = {};
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