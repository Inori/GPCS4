#include "sce_videoout.h"
#include <cstring>
#include "Graphic/GraphicShared.h"
#include "Graphic/Sce/SceVideoOut.h"
#include "Graphic/Sce/SceGnmDriver.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceVideoOut);

//////////////////////////////////////////////////////////////////////////
// library: libSceVideoOut
//////////////////////////////////////////////////////////////////////////




int PS4API sceVideoOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, const void *param)
{
	LOG_SCE_GRAPHIC("user id %d", userId);
	LOG_ASSERT((type == SCE_VIDEO_OUT_BUS_TYPE_MAIN), "not supported videoout type %d", type);

	GfxContext gfxCtx;
	gfxCtx.videoOut =  std::make_shared<sce::SceVideoOut>(sce::kVideoOutDefaultWidth, sce::kVideoOutDefaultHeight);
	gfxCtx.gnmDriver = std::make_shared<sce::SceGnmDriver>(gfxCtx.videoOut);
	setGfxContext(SCE_VIDEO_HANDLE_MAIN, gfxCtx);
	return SCE_VIDEO_HANDLE_MAIN;
}


int PS4API sceVideoOutClose(int32_t handle)
{
	LOG_SCE_GRAPHIC("handle %d", handle);
	int ret = -1;
	do 
	{
		std::shared_ptr<sce::SceVideoOut> videoOut = getVideoOut(handle);
		if (videoOut)
		{
			videoOut.reset();
		}
		std::shared_ptr<sce::SceGnmDriver> gnmDriver = getGnmDriver(handle);
		if (gnmDriver)
		{
			gnmDriver.reset();
		}
		// clear
		GfxContext gfxCtx = { nullptr };
		setGfxContext(handle, gfxCtx);

		ret = SCE_OK;
	} while (false);
	return ret;
}


int PS4API sceVideoOutGetResolutionStatus(int32_t handle, SceVideoOutResolutionStatus *status)
{
	LOG_SCE_GRAPHIC("handle %d", handle);

	std::shared_ptr<sce::SceVideoOut> videoOut = getVideoOut(handle);

	status->fullWidth = videoOut->width();
	status->fullHeight = videoOut->height();
	status->paneWidth = videoOut->width();
	status->paneHeight = videoOut->height();
	status->refreshRate = SCE_VIDEO_OUT_REFRESH_RATE_59_94HZ;
	status->screenSizeInInch = 32;
	status->flags = SCE_VIDEO_OUT_RESOLUTION_STATUS_FLAGS_OUTPUT_IN_USE;
	return SCE_OK;
}


int PS4API sceVideoOutSetFlipRate(int32_t handle, int32_t rate)
{
	LOG_SCE_GRAPHIC("handle %d rate %d", handle, rate);
	int ret = SCE_GNM_ERROR_UNKNOWN;
	const uint32_t rateTable[3] = { 60, 30, 20 };
	do 
	{
		if (rate < 0 || rate > 2)
		{
			ret = SCE_VIDEO_OUT_ERROR_INVALID_VALUE;
			break;
		}

		std::shared_ptr<sce::SceVideoOut> videoOut = getVideoOut(handle);
		if (!videoOut)
		{
			ret = SCE_VIDEO_OUT_ERROR_INVALID_HANDLE;
			break;
		}

		uint32_t realRate = rateTable[rate];
		videoOut->setFlipRate(realRate);

		ret = SCE_OK;
	} while (false);
	return ret;
}


int PS4API sceVideoOutSetBufferAttribute(SceVideoOutBufferAttribute *attribute, 
	uint32_t pixelFormat, uint32_t tilingMode, uint32_t aspectRatio,
	uint32_t width, uint32_t height, uint32_t pitchInPixel)
{
	LOG_SCE_DUMMY_IMPL();
	memset(attribute, 0, sizeof(*attribute));
	attribute->pixelFormat = pixelFormat;
	attribute->tilingMode = tilingMode;
	attribute->aspectRatio = SCE_VIDEO_OUT_ASPECT_RATIO_16_9;
	attribute->width = width;
	attribute->height = height;
	attribute->pitchInPixel = pitchInPixel;
	attribute->option = SCE_VIDEO_OUT_BUFFER_ATTRIBUTE_OPTION_NONE;
	return SCE_OK;
}


int PS4API sceVideoOutRegisterBuffers(int32_t handle, int32_t startIndex, void * const *addresses, 
	int32_t bufferNum, const SceVideoOutBufferAttribute *attribute)
{
	LOG_SCE_GRAPHIC("handle %d addr %p num %d attr %p", handle, addresses, bufferNum, attribute);
	int ret = SCE_GNM_ERROR_UNKNOWN;
	do 
	{
		std::shared_ptr<sce::SceVideoOut> videoOut = getVideoOut(handle);
		std::shared_ptr<sce::SceGnmDriver> gnmDriver = getGnmDriver(handle);

		if (!videoOut || !gnmDriver)
		{
			break;
		}

		// Maybe need more params, such as attribute...
		if (!videoOut->registerBuffers(startIndex, bufferNum))
		{
			break;
		}
		
		if (!gnmDriver->initDriver(bufferNum))
		{
			break;
		}
		
		ret = SCE_OK;
	} while (false);
	return ret;
}


int PS4API sceVideoOutRegisterStereoBuffers(int32_t handle, int32_t startIndex, const SceVideoOutStereoBuffers *buffers,
	int32_t bufferNum, const SceVideoOutBufferAttribute *attribute)
{
	LOG_SCE_GRAPHIC("handle %d buffers %p num %d attr %p", handle, buffers, bufferNum, attribute);
	int ret = SCE_GNM_ERROR_UNKNOWN;
	do 
	{
		std::shared_ptr<sce::SceVideoOut> videoOut = getVideoOut(handle);
		std::shared_ptr<sce::SceGnmDriver> gnmDriver = getGnmDriver(handle);

		if (!videoOut || !gnmDriver)
		{
			break;
		}

		// Maybe need more params, such as attribute...
		if (!videoOut->registerBuffers(startIndex, bufferNum))
		{
			break;
		}
		
		if (!gnmDriver->initDriver(bufferNum))
		{
			break;
		}
		
		ret = SCE_OK;
	} while (false);
	return ret;
}


int PS4API sceVideoOutAddFlipEvent(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutAdjustColor_(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutColorSettingsSetGamma_(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutConfigureOutputMode_(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutGetDeviceCapabilityInfo_(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutGetEventData(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutGetFlipStatus(int32_t handle, SceVideoOutFlipStatus *status)
{
	LOG_SCE_GRAPHIC("Dummy Implementation");

	static uint64_t sCount = 1;

	*status = {};
	status->count = sCount++;

	return SCE_OK;
}


int PS4API sceVideoOutIsFlipPending(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutModeSetAny_(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutSetWindowModeMargins(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutSubmitChangeBufferAttribute(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutSubmitFlip(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceVideoOutWaitVblank(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

