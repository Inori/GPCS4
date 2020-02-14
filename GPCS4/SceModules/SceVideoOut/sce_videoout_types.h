#pragma once


struct SceVideoOutResolutionStatus 
{
	uint32_t fullWidth;
	uint32_t fullHeight;
	uint32_t paneWidth;
	uint32_t paneHeight;
	uint64_t refreshRate;
	float screenSizeInInch;
	uint16_t flags;
	uint16_t _reserved0;
	uint32_t _reserved1[3];
};


struct SceVideoOutBufferAttribute 
{
	int32_t pixelFormat;		// SceVideoOutPixelFormat
	int32_t tilingMode;		// SceVideoOutTilingMode
	int32_t aspectRatio;		// SceVideoOutAspectRatio
	uint32_t width;
	uint32_t height;
	uint32_t pitchInPixel;
	uint32_t option;           // SceVideoOutBufferAttributeOption
	uint32_t _reserved0;
	uint64_t _reserved1;
};

struct SceVideoOutFlipStatus
{
	uint64_t count;
	uint64_t processTime;
	uint64_t tsc;
	int64_t flipArg;
	uint64_t submitTsc;
	uint64_t _reserved0;
	int32_t gcQueueNum;
	int32_t flipPendingNum;
	int32_t currentBuffer;
	uint32_t _reserved1;
};

struct SceVideoOutStereoBuffers 
{
	void *left;
	void *right;
};


enum SceVideoOutBufferAttributeOption 
{
	SCE_VIDEO_OUT_BUFFER_ATTRIBUTE_OPTION_NONE = 0,
	SCE_VIDEO_OUT_BUFFER_ATTRIBUTE_OPTION_VR = 7,
	SCE_VIDEO_OUT_BUFFER_ATTRIBUTE_OPTION_STRICT_COLORIMETRY = (1U << 3),

	SCE_VIDEO_OUT_BUFFER_ATTRIBUTE_OPTION_MASK = (1U << 4) - 1,
};


enum SceVideoOutResolutionStatusFlags 
{
	SCE_VIDEO_OUT_RESOLUTION_STATUS_FLAGS_OUTPUT_MASK = (1 << 0),
	SCE_VIDEO_OUT_RESOLUTION_STATUS_FLAGS_OUTPUT_IN_USE = (0 << 0),
	SCE_VIDEO_OUT_RESOLUTION_STATUS_FLAGS_OUTPUT_NOT_IN_USE = (1 << 0),
};


enum SceVideoOutRefreshRate
{
	SCE_VIDEO_OUT_REFRESH_RATE_UNKNOWN = 0,
	SCE_VIDEO_OUT_REFRESH_RATE_23_98HZ = 1,
	SCE_VIDEO_OUT_REFRESH_RATE_50HZ = 2,
	SCE_VIDEO_OUT_REFRESH_RATE_59_94HZ = 3,
	SCE_VIDEO_OUT_REFRESH_RATE_29_97HZ = 6,

	// for VR
	SCE_VIDEO_OUT_REFRESH_RATE_89_91HZ = 35,
	SCE_VIDEO_OUT_REFRESH_RATE_119_88HZ = 13,

	SCE_VIDEO_OUT_REFRESH_RATE_ANY = 0xFFFFFFFFFFFFFFFFull,
};


enum SceVideoOutBusType 
{
	SCE_VIDEO_OUT_BUS_TYPE_MAIN = 0,
};


enum  SceVideoOutPixelFormat 
{

	SCE_VIDEO_OUT_PIXEL_FORMAT_A8R8G8B8_SRGB = 0x80000000, /* MSB first. Blue is at LSB */
	SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB = SCE_VIDEO_OUT_PIXEL_FORMAT_A8R8G8B8_SRGB,  /* alias name in a gnm-friendly order (LSB first) */

	SCE_VIDEO_OUT_PIXEL_FORMAT_A16R16G16B16_FLOAT = 0xC1060000, /* MSB first. Blue is at LSB */
	SCE_VIDEO_OUT_PIXEL_FORMAT_B16_G16_R16_A16_FLOAT = SCE_VIDEO_OUT_PIXEL_FORMAT_A16R16G16B16_FLOAT,  /* alias name in a gnm-friendly order (LSB first) */

	SCE_VIDEO_OUT_PIXEL_FORMAT_A8B8G8R8_SRGB = 0x80002200, /* MSB first. Red is at LSB */
	SCE_VIDEO_OUT_PIXEL_FORMAT_R8_G8_B8_A8_SRGB = SCE_VIDEO_OUT_PIXEL_FORMAT_A8B8G8R8_SRGB,  /* alias name in a gnm-friendly order (LSB first) */

	SCE_VIDEO_OUT_PIXEL_FORMAT_A2R10G10B10 = 0x88060000, /* MSB first. Blue is at LSB */
	SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2 = SCE_VIDEO_OUT_PIXEL_FORMAT_A2R10G10B10,  /* alias name in a gnm-friendly order (LSB first) */

	SCE_VIDEO_OUT_PIXEL_FORMAT_A2R10G10B10_SRGB = 0x88000000, /* MSB first. Blue is at LSB */
	SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_SRGB = SCE_VIDEO_OUT_PIXEL_FORMAT_A2R10G10B10_SRGB,  /* alias name in a gnm-friendly order (LSB first) */

	SCE_VIDEO_OUT_PIXEL_FORMAT_A2R10G10B10_BT2020_PQ = 0x88740000,/* MSB first. Blue is at LSB */
	SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_BT2020_PQ = SCE_VIDEO_OUT_PIXEL_FORMAT_A2R10G10B10_BT2020_PQ,  /* alias name in a gnm-friendly order (LSB first) */
};


enum SceVideoOutFlipMode 
{
	SCE_VIDEO_OUT_FLIP_MODE_VSYNC = 1,  /* on real video out vsync */
	SCE_VIDEO_OUT_FLIP_MODE_HSYNC = 2,  /* ASAP (but not immediate) */
	SCE_VIDEO_OUT_FLIP_MODE_WINDOW = 3, /* similar to vsync but may flip on some windows at the top and the bottom of the display. N/A on Neo mode */
	SCE_VIDEO_OUT_FLIP_MODE_VSYNC_MULTI = 4, /* vsync mode but allows multiple flips per vsync. flipRate is not valid. N/A on Neo mode */
	SCE_VIDEO_OUT_FLIP_MODE_VSYNC_MULTI_2 = 5, /* vsync mode but allows multiple flips per vsync. flipRate is valid */
	SCE_VIDEO_OUT_FLIP_MODE_WINDOW_2 = 6, /* Window mode but the top margin is less accurate than _MODE_WINDOW. The bottom margin must be 0.  */
};

enum SceVideoOutTilingMode 
{
	SCE_VIDEO_OUT_TILING_MODE_TILE = 0, /* 32bpp pixel format only if on Neo mode */
	SCE_VIDEO_OUT_TILING_MODE_LINEAR = 1, /* 32bpp pixel format only */
};

enum SceVideoOutAspectRatio 
{
	SCE_VIDEO_OUT_ASPECT_RATIO_16_9 = 0,
};


enum SceVideoOutEventId 
{
	SCE_VIDEO_OUT_EVENT_FLIP = 0,
	SCE_VIDEO_OUT_EVENT_VBLANK = 1,
	SCE_VIDEO_OUT_EVENT_PRE_VBLANK_START = 2,
};
