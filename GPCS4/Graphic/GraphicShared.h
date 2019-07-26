#pragma once

#include "GPCS4Common.h"

// shared staffs among graphic components

namespace sce
{
	class SceVideoOut;
	class SceGnmDriver;
}

struct GfxContext
{
	sce::SceVideoOut* videoOut;
	sce::SceGnmDriver* gnmDriver;
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

inline sce::SceVideoOut* getVideoOut(int videoHandle)
{
	GfxContext* ctx = getGfxContext(videoHandle);
	return ctx ? ctx->videoOut : NULL;
}

inline sce::SceGnmDriver* getGnmDriver(int videoHandle)
{
	GfxContext* ctx = getGfxContext(videoHandle);
	return ctx ? ctx->gnmDriver : NULL;
}
