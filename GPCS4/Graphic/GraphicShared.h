#pragma once

#include "GPCS4Common.h"
#include <memory>
// shared staffs among graphic components

namespace sce
{
	class SceVideoOut;
	class SceGnmDriver;
}

struct GfxContext
{
	std::shared_ptr<sce::SceVideoOut> videoOut;
	std::shared_ptr<sce::SceGnmDriver> gnmDriver;
};


#define SCE_VIDEO_HANDLE_MAIN 1
#define SCE_VIDEO_HANDLE_MAX 3

extern GfxContext g_VideoOutHanleMap[SCE_VIDEO_HANDLE_MAX];


//////////////////////////////////////////////////////////////////////////
// helper functions

inline void setGfxContext(int videoHandle, GfxContext& gfxContext)
{
	do 
	{
		if (videoHandle == 0 || videoHandle >= SCE_VIDEO_HANDLE_MAX)
		{
			break;
		}

		g_VideoOutHanleMap[videoHandle] = gfxContext;
	} while (false);
}

inline GfxContext* getGfxContext(int videoHandle)
{
	GfxContext* ctx = NULL;
	do 
	{
		if (videoHandle == 0 || videoHandle >= SCE_VIDEO_HANDLE_MAX)
		{
			break;
		}

		ctx = &g_VideoOutHanleMap[videoHandle];
	} while (false);
	return ctx;
}

inline std::shared_ptr<sce::SceVideoOut> getVideoOut(int videoHandle = SCE_VIDEO_HANDLE_MAIN)
{
	GfxContext* ctx = getGfxContext(videoHandle);
	return ctx ? ctx->videoOut : NULL;
}

inline std::shared_ptr<sce::SceGnmDriver> getGnmDriver(int videoHandle = SCE_VIDEO_HANDLE_MAIN)
{
	GfxContext* ctx = getGfxContext(videoHandle);
	return ctx ? ctx->gnmDriver : NULL;
}
