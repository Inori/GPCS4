#pragma once

#include "GnmBuffer.h"
#include "GnmCommon.h"
#include "GnmShaderMeta.h"
#include "UtilFlag.h"

#include "../Pssl/PsslEnums.h"
#include "../Pssl/PsslShaderModule.h"
#include "../Pssl/PsslShaderStructure.h"
#include "../Violet/VltContextState.h"
#include "../Violet/VltLimit.h"

namespace vlt
{
;
class VltShader;
class VltImageView;
}  // namespace vlt

//////////////////////////////////////////////////////////////////////////
/**
 * \brief Gnm Context Flags
 *
 * Some objects/states/operations are combined together in vulkan 
 * while they are processed separately in gnm.
 * For example, render targets. 
 * In vulkan, both color and depth targets
 * are abstracted into one framebuffer object. 
 * But in gnm, there is no framebuffer abstraction,
 * the color and depth targets are set by different calls.
 * Thus we need to delay the work until we gathered all the required informations,
 * and then check the flags to make sure, 
 * whether we need to call Violet respectively.
 */
enum class GnmContexFlag : uint32_t
{
	GpClearDepthTarget,  ///< There is pending depth clear operation.

	GpDirtyRenderTarget,  ///< RenderTarget is out of data. Here RenderTarget includes both color and depth target.
};

using GnmContexFlags = Flags<GnmContexFlag>;

//////////////////////////////////////////////////////////////////////////
struct GnmShaderContext
{
	const void*                           code   = nullptr;
	RcPtr<pssl::PsslShaderModule>         shader = nullptr;
	std::vector<pssl::PsslShaderResource> userDataSlotTable;
};

struct GnmShaderContextVS : public GnmShaderContext
{
	GnmShaderMetaVs meta;
};

struct GnmShaderContextPS : public GnmShaderContext
{
	GnmShaderMetaPs meta;
};

struct GnmShaderContextCS : public GnmShaderContext
{
	GnmShaderMetaCs meta;
};

struct GnmShaderContextGroup
{
	GnmShaderContextVS vs;
	GnmShaderContextPS ps;
	GnmShaderContextCS cs;
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

	GnmDepthRenderTarget depthTarget     = {};
	VkClearValue         depthClearValue = {};
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