#pragma once

#include "GPCS4Common.h"

// shared staffs among graphic components


class SceVideoOut;
class SceGnmDriver;

struct GfxContext
{
	SceVideoOut* videoOut;
	SceGnmDriver* gnmDriver;
};


#define SCE_VIDEO_PORT_MAIN 1
#define SCE_VIDEO_PORT_MAX 3

extern GfxContext g_VideoOutHanleMap[SCE_VIDEO_PORT_MAX];


//////////////////////////////////////////////////////////////////////////
// helper functions

inline void setGfxContext(int videoPort, GfxContext& gfxContext)
{
	do 
	{
		if (videoPort == 0 || videoPort >= SCE_VIDEO_PORT_MAX)
		{
			break;
		}

		g_VideoOutHanleMap[videoPort] = gfxContext;
	} while (false);
}

inline GfxContext* getGfxContext(int videoPort)
{
	GfxContext* ctx = NULL;
	do 
	{
		if (videoPort == 0 || videoPort >= SCE_VIDEO_PORT_MAX)
		{
			break;
		}

		ctx = &g_VideoOutHanleMap[videoPort];
	} while (false);
	return ctx;
}

inline SceVideoOut* getVideoOut(int videoPort)
{
	GfxContext* ctx = getGfxContext(videoPort);
	return ctx ? ctx->videoOut : NULL;
}

inline SceGnmDriver* getGnmDriver(int videoPort)
{
	GfxContext* ctx = getGfxContext(videoPort);
	return ctx ? ctx->gnmDriver : NULL;
}
