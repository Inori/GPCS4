#include "GnmTiler.h"
#include "GnmGpuAddress.h"
#include "GnmGpuAddressTool.h"
#include "GnmDataFormat.h"
#include "GnmTexture.h"
#include "GnmTilerSSE2.h"

#include <algorithm>

LOG_CHANNEL(Graphic.Gnm.GpuAddress);

namespace GpuAddress
{;

struct uint128_t
{
	uint64_t m_data[2];
};
struct uint256_t
{
	uint64_t m_data[4];
};

static const uint16_t g_pitches[15][7] = {
	{
		32,
		16,
		8,
		8,
		8,
		32,
		32,
	},
	{
		32,
		16,
		8,
		8,
		8,
		32,
		32,
	},
	{
		32,
		16,
		8,
		8,
		8,
		32,
		32,
	},
	{
		32,
		16,
		8,
		8,
		8,
		32,
		32,
	},
	{
		16,
		16,
		16,
		16,
		16,
		32,
		32,
	},
	{
		32,
		32,
		32,
		32,
		32,
		32,
		32,
	},
	{
		64,
		64,
		64,
		64,
		64,
		64,
		64,
	},
	{
		128,
		128,
		128,
		128,
		128,
		128,
		128,
	},
	{
		256,
		256,
		256,
		256,
		256,
		256,
		256,
	},
	{
		512,
		512,
		512,
		512,
		512,
		512,
		512,
	},
	{
		1024,
		1024,
		1024,
		1024,
		1024,
		1024,
		1024,
	},
	{
		2048,
		2048,
		2048,
		2048,
		2048,
		2048,
		2048,
	},
	{
		4096,
		4096,
		4096,
		4096,
		4096,
		4096,
		4096,
	},
	{
		8192,
		8192,
		8192,
		8192,
		8192,
		8192,
		8192,
	},
	{
		16384,
		16384,
		16384,
		16384,
		16384,
		16384,
		16384,
	},
};

static const uint32_t g_offsetSizes[15][7][15][2] = {
	{
		{
			{ 0, 1 },
		},
		{
			{ 0, 1 },
		},
		{
			{ 0, 1 },
		},
		{
			{ 0, 2 },
		},
		{
			{ 0, 4 },
		},
		{
			{ 0, 2 },
		},
		{
			{ 0, 4 },
		},
	},
	{
		{
			{ 0, 1 },
			{ 1, 1 },
		},
		{
			{ 0, 1 },
			{ 1, 1 },
		},
		{
			{ 0, 1 },
			{ 1, 1 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
		},
	},
	{
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
		},
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
		},
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
			{ 4, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
			{ 8, 4 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
			{ 4, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
			{ 8, 4 },
		},
	},
	{
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
			{ 3, 1 },
		},
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
			{ 3, 1 },
		},
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
			{ 3, 1 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
			{ 4, 2 },
			{ 6, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
			{ 8, 4 },
			{ 12, 4 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
			{ 4, 2 },
			{ 6, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
			{ 8, 4 },
			{ 12, 4 },
		},
	},
	{
		{
			{ 0, 1 },
			{ 1, 1 },
			{ 2, 1 },
			{ 3, 1 },
			{ 4, 1 },
		},
		{
			{ 0, 2 },
			{ 2, 1 },
			{ 3, 1 },
			{ 4, 1 },
			{ 5, 1 },
		},
		{
			{ 0, 4 },
			{ 4, 1 },
			{ 5, 1 },
			{ 6, 1 },
			{ 7, 1 },
		},
		{
			{ 0, 8 },
			{ 8, 2 },
			{ 10, 2 },
			{ 12, 2 },
			{ 14, 2 },
		},
		{
			{ 0, 16 },
			{ 16, 4 },
			{ 20, 4 },
			{ 24, 4 },
			{ 28, 4 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
			{ 4, 2 },
			{ 6, 2 },
			{ 8, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
			{ 8, 4 },
			{ 12, 4 },
			{ 16, 4 },
		},
	},
	{
		{
			{ 0, 4 },
			{ 4, 1 },
			{ 5, 1 },
			{ 6, 1 },
			{ 7, 1 },
			{ 8, 1 },
		},
		{
			{ 0, 8 },
			{ 8, 2 },
			{ 10, 1 },
			{ 11, 1 },
			{ 12, 1 },
			{ 13, 1 },
		},
		{
			{ 0, 16 },
			{ 16, 4 },
			{ 20, 1 },
			{ 21, 1 },
			{ 22, 1 },
			{ 23, 1 },
		},
		{
			{ 0, 32 },
			{ 32, 8 },
			{ 40, 2 },
			{ 42, 2 },
			{ 44, 2 },
			{ 46, 2 },
		},
		{
			{ 0, 64 },
			{ 64, 16 },
			{ 80, 4 },
			{ 84, 4 },
			{ 88, 4 },
			{ 92, 4 },
		},
		{
			{ 0, 2 },
			{ 2, 2 },
			{ 4, 2 },
			{ 6, 2 },
			{ 8, 2 },
			{ 10, 2 },
		},
		{
			{ 0, 4 },
			{ 4, 4 },
			{ 8, 4 },
			{ 12, 4 },
			{ 16, 4 },
			{ 20, 4 },
		},
	},
	{
		{
			{ 0, 16 },
			{ 16, 4 },
			{ 20, 1 },
			{ 21, 1 },
			{ 22, 1 },
			{ 23, 1 },
			{ 24, 1 },
		},
		{
			{ 0, 32 },
			{ 32, 8 },
			{ 40, 2 },
			{ 42, 1 },
			{ 43, 1 },
			{ 44, 1 },
			{ 45, 1 },
		},
		{
			{ 0, 64 },
			{ 64, 16 },
			{ 80, 4 },
			{ 84, 1 },
			{ 85, 1 },
			{ 86, 1 },
			{ 87, 1 },
		},
		{
			{ 0, 128 },
			{ 128, 32 },
			{ 160, 8 },
			{ 168, 2 },
			{ 170, 2 },
			{ 172, 2 },
			{ 174, 2 },
		},
		{
			{ 0, 256 },
			{ 256, 64 },
			{ 320, 16 },
			{ 336, 4 },
			{ 340, 4 },
			{ 344, 4 },
			{ 348, 4 },
		},
		{
			{ 0, 8 },
			{ 8, 2 },
			{ 10, 2 },
			{ 12, 2 },
			{ 14, 2 },
			{ 16, 2 },
			{ 18, 2 },
		},
		{
			{ 0, 16 },
			{ 16, 4 },
			{ 20, 4 },
			{ 24, 4 },
			{ 28, 4 },
			{ 32, 4 },
			{ 36, 4 },
		},
	},
	{
		{
			{ 0, 64 },
			{ 64, 16 },
			{ 80, 4 },
			{ 84, 1 },
			{ 85, 1 },
			{ 86, 1 },
			{ 87, 1 },
			{ 88, 1 },
		},
		{
			{ 0, 128 },
			{ 128, 32 },
			{ 160, 8 },
			{ 168, 2 },
			{ 170, 1 },
			{ 171, 1 },
			{ 172, 1 },
			{ 173, 1 },
		},
		{
			{ 0, 256 },
			{ 256, 64 },
			{ 320, 16 },
			{ 336, 4 },
			{ 340, 1 },
			{ 341, 1 },
			{ 342, 1 },
			{ 343, 1 },
		},
		{
			{ 0, 512 },
			{ 512, 128 },
			{ 640, 32 },
			{ 672, 8 },
			{ 680, 2 },
			{ 682, 2 },
			{ 684, 2 },
			{ 686, 2 },
		},
		{
			{ 0, 1024 },
			{ 1024, 256 },
			{ 1280, 64 },
			{ 1344, 16 },
			{ 1360, 4 },
			{ 1364, 4 },
			{ 1368, 4 },
			{ 1372, 4 },
		},
		{
			{ 0, 32 },
			{ 32, 8 },
			{ 40, 2 },
			{ 42, 2 },
			{ 44, 2 },
			{ 46, 2 },
			{ 48, 2 },
			{ 50, 2 },
		},
		{
			{ 0, 64 },
			{ 64, 16 },
			{ 80, 4 },
			{ 84, 4 },
			{ 88, 4 },
			{ 92, 4 },
			{ 96, 4 },
			{ 100, 4 },
		},
	},
	{
		{
			{ 0, 256 },
			{ 256, 64 },
			{ 320, 16 },
			{ 336, 4 },
			{ 340, 1 },
			{ 341, 1 },
			{ 342, 1 },
			{ 343, 1 },
			{ 344, 1 },
		},
		{
			{ 0, 512 },
			{ 512, 128 },
			{ 640, 32 },
			{ 672, 8 },
			{ 680, 2 },
			{ 682, 1 },
			{ 683, 1 },
			{ 684, 1 },
			{ 685, 1 },
		},
		{
			{ 0, 1024 },
			{ 1024, 256 },
			{ 1280, 64 },
			{ 1344, 16 },
			{ 1360, 4 },
			{ 1364, 1 },
			{ 1365, 1 },
			{ 1366, 1 },
			{ 1367, 1 },
		},
		{
			{ 0, 2048 },
			{ 2048, 512 },
			{ 2560, 128 },
			{ 2688, 32 },
			{ 2720, 8 },
			{ 2728, 2 },
			{ 2730, 2 },
			{ 2732, 2 },
			{ 2734, 2 },
		},
		{
			{ 0, 4096 },
			{ 4096, 1024 },
			{ 5120, 256 },
			{ 5376, 64 },
			{ 5440, 16 },
			{ 5456, 4 },
			{ 5460, 4 },
			{ 5464, 4 },
			{ 5468, 4 },
		},
		{
			{ 0, 128 },
			{ 128, 32 },
			{ 160, 8 },
			{ 168, 2 },
			{ 170, 2 },
			{ 172, 2 },
			{ 174, 2 },
			{ 176, 2 },
			{ 178, 2 },
		},
		{
			{ 0, 256 },
			{ 256, 64 },
			{ 320, 16 },
			{ 336, 4 },
			{ 340, 4 },
			{ 344, 4 },
			{ 348, 4 },
			{ 352, 4 },
			{ 356, 4 },
		},
	},
	{
		{
			{ 0, 1024 },
			{ 1024, 256 },
			{ 1280, 64 },
			{ 1344, 16 },
			{ 1360, 4 },
			{ 1364, 1 },
			{ 1365, 1 },
			{ 1366, 1 },
			{ 1367, 1 },
			{ 1368, 1 },
		},
		{
			{ 0, 2048 },
			{ 2048, 512 },
			{ 2560, 128 },
			{ 2688, 32 },
			{ 2720, 8 },
			{ 2728, 2 },
			{ 2730, 1 },
			{ 2731, 1 },
			{ 2732, 1 },
			{ 2733, 1 },
		},
		{
			{ 0, 4096 },
			{ 4096, 1024 },
			{ 5120, 256 },
			{ 5376, 64 },
			{ 5440, 16 },
			{ 5456, 4 },
			{ 5460, 1 },
			{ 5461, 1 },
			{ 5462, 1 },
			{ 5463, 1 },
		},
		{
			{ 0, 8192 },
			{ 8192, 2048 },
			{ 10240, 512 },
			{ 10752, 128 },
			{ 10880, 32 },
			{ 10912, 8 },
			{ 10920, 2 },
			{ 10922, 2 },
			{ 10924, 2 },
			{ 10926, 2 },
		},
		{
			{ 0, 16384 },
			{ 16384, 4096 },
			{ 20480, 1024 },
			{ 21504, 256 },
			{ 21760, 64 },
			{ 21824, 16 },
			{ 21840, 4 },
			{ 21844, 4 },
			{ 21848, 4 },
			{ 21852, 4 },
		},
		{
			{ 0, 512 },
			{ 512, 128 },
			{ 640, 32 },
			{ 672, 8 },
			{ 680, 2 },
			{ 682, 2 },
			{ 684, 2 },
			{ 686, 2 },
			{ 688, 2 },
			{ 690, 2 },
		},
		{
			{ 0, 1024 },
			{ 1024, 256 },
			{ 1280, 64 },
			{ 1344, 16 },
			{ 1360, 4 },
			{ 1364, 4 },
			{ 1368, 4 },
			{ 1372, 4 },
			{ 1376, 4 },
			{ 1380, 4 },
		},
	},
	{
		{
			{ 0, 4096 },
			{ 4096, 1024 },
			{ 5120, 256 },
			{ 5376, 64 },
			{ 5440, 16 },
			{ 5456, 4 },
			{ 5460, 1 },
			{ 5461, 1 },
			{ 5462, 1 },
			{ 5463, 1 },
			{ 5464, 1 },
		},
		{
			{ 0, 8192 },
			{ 8192, 2048 },
			{ 10240, 512 },
			{ 10752, 128 },
			{ 10880, 32 },
			{ 10912, 8 },
			{ 10920, 2 },
			{ 10922, 1 },
			{ 10923, 1 },
			{ 10924, 1 },
			{ 10925, 1 },
		},
		{
			{ 0, 16384 },
			{ 16384, 4096 },
			{ 20480, 1024 },
			{ 21504, 256 },
			{ 21760, 64 },
			{ 21824, 16 },
			{ 21840, 4 },
			{ 21844, 1 },
			{ 21845, 1 },
			{ 21846, 1 },
			{ 21847, 1 },
		},
		{
			{ 0, 32768 },
			{ 32768, 8192 },
			{ 40960, 2048 },
			{ 43008, 512 },
			{ 43520, 128 },
			{ 43648, 32 },
			{ 43680, 8 },
			{ 43688, 2 },
			{ 43690, 2 },
			{ 43692, 2 },
			{ 43694, 2 },
		},
		{
			{ 0, 65536 },
			{ 65536, 16384 },
			{ 81920, 4096 },
			{ 86016, 1024 },
			{ 87040, 256 },
			{ 87296, 64 },
			{ 87360, 16 },
			{ 87376, 4 },
			{ 87380, 4 },
			{ 87384, 4 },
			{ 87388, 4 },
		},
		{
			{ 0, 2048 },
			{ 2048, 512 },
			{ 2560, 128 },
			{ 2688, 32 },
			{ 2720, 8 },
			{ 2728, 2 },
			{ 2730, 2 },
			{ 2732, 2 },
			{ 2734, 2 },
			{ 2736, 2 },
			{ 2738, 2 },
		},
		{
			{ 0, 4096 },
			{ 4096, 1024 },
			{ 5120, 256 },
			{ 5376, 64 },
			{ 5440, 16 },
			{ 5456, 4 },
			{ 5460, 4 },
			{ 5464, 4 },
			{ 5468, 4 },
			{ 5472, 4 },
			{ 5476, 4 },
		},
	},
	{
		{
			{ 0, 16384 },
			{ 16384, 4096 },
			{ 20480, 1024 },
			{ 21504, 256 },
			{ 21760, 64 },
			{ 21824, 16 },
			{ 21840, 4 },
			{ 21844, 1 },
			{ 21845, 1 },
			{ 21846, 1 },
			{ 21847, 1 },
			{ 21848, 1 },
		},
		{
			{ 0, 32768 },
			{ 32768, 8192 },
			{ 40960, 2048 },
			{ 43008, 512 },
			{ 43520, 128 },
			{ 43648, 32 },
			{ 43680, 8 },
			{ 43688, 2 },
			{ 43690, 1 },
			{ 43691, 1 },
			{ 43692, 1 },
			{ 43693, 1 },
		},
		{
			{ 0, 65536 },
			{ 65536, 16384 },
			{ 81920, 4096 },
			{ 86016, 1024 },
			{ 87040, 256 },
			{ 87296, 64 },
			{ 87360, 16 },
			{ 87376, 4 },
			{ 87380, 1 },
			{ 87381, 1 },
			{ 87382, 1 },
			{ 87383, 1 },
		},
		{
			{ 0, 131072 },
			{ 131072, 32768 },
			{ 163840, 8192 },
			{ 172032, 2048 },
			{ 174080, 512 },
			{ 174592, 128 },
			{ 174720, 32 },
			{ 174752, 8 },
			{ 174760, 2 },
			{ 174762, 2 },
			{ 174764, 2 },
			{ 174766, 2 },
		},
		{
			{ 0, 262144 },
			{ 262144, 65536 },
			{ 327680, 16384 },
			{ 344064, 4096 },
			{ 348160, 1024 },
			{ 349184, 256 },
			{ 349440, 64 },
			{ 349504, 16 },
			{ 349520, 4 },
			{ 349524, 4 },
			{ 349528, 4 },
			{ 349532, 4 },
		},
		{
			{ 0, 8192 },
			{ 8192, 2048 },
			{ 10240, 512 },
			{ 10752, 128 },
			{ 10880, 32 },
			{ 10912, 8 },
			{ 10920, 2 },
			{ 10922, 2 },
			{ 10924, 2 },
			{ 10926, 2 },
			{ 10928, 2 },
			{ 10930, 2 },
		},
		{
			{ 0, 16384 },
			{ 16384, 4096 },
			{ 20480, 1024 },
			{ 21504, 256 },
			{ 21760, 64 },
			{ 21824, 16 },
			{ 21840, 4 },
			{ 21844, 4 },
			{ 21848, 4 },
			{ 21852, 4 },
			{ 21856, 4 },
			{ 21860, 4 },
		},
	},
	{
		{
			{ 0, 65536 },
			{ 65536, 16384 },
			{ 81920, 4096 },
			{ 86016, 1024 },
			{ 87040, 256 },
			{ 87296, 64 },
			{ 87360, 16 },
			{ 87376, 4 },
			{ 87380, 1 },
			{ 87381, 1 },
			{ 87382, 1 },
			{ 87383, 1 },
			{ 87384, 1 },
		},
		{
			{ 0, 131072 },
			{ 131072, 32768 },
			{ 163840, 8192 },
			{ 172032, 2048 },
			{ 174080, 512 },
			{ 174592, 128 },
			{ 174720, 32 },
			{ 174752, 8 },
			{ 174760, 2 },
			{ 174762, 1 },
			{ 174763, 1 },
			{ 174764, 1 },
			{ 174765, 1 },
		},
		{
			{ 0, 262144 },
			{ 262144, 65536 },
			{ 327680, 16384 },
			{ 344064, 4096 },
			{ 348160, 1024 },
			{ 349184, 256 },
			{ 349440, 64 },
			{ 349504, 16 },
			{ 349520, 4 },
			{ 349524, 1 },
			{ 349525, 1 },
			{ 349526, 1 },
			{ 349527, 1 },
		},
		{
			{ 0, 524288 },
			{ 524288, 131072 },
			{ 655360, 32768 },
			{ 688128, 8192 },
			{ 696320, 2048 },
			{ 698368, 512 },
			{ 698880, 128 },
			{ 699008, 32 },
			{ 699040, 8 },
			{ 699048, 2 },
			{ 699050, 2 },
			{ 699052, 2 },
			{ 699054, 2 },
		},
		{
			{ 0, 1048576 },
			{ 1048576, 262144 },
			{ 1310720, 65536 },
			{ 1376256, 16384 },
			{ 1392640, 4096 },
			{ 1396736, 1024 },
			{ 1397760, 256 },
			{ 1398016, 64 },
			{ 1398080, 16 },
			{ 1398096, 4 },
			{ 1398100, 4 },
			{ 1398104, 4 },
			{ 1398108, 4 },
		},
		{
			{ 0, 32768 },
			{ 32768, 8192 },
			{ 40960, 2048 },
			{ 43008, 512 },
			{ 43520, 128 },
			{ 43648, 32 },
			{ 43680, 8 },
			{ 43688, 2 },
			{ 43690, 2 },
			{ 43692, 2 },
			{ 43694, 2 },
			{ 43696, 2 },
			{ 43698, 2 },
		},
		{
			{ 0, 65536 },
			{ 65536, 16384 },
			{ 81920, 4096 },
			{ 86016, 1024 },
			{ 87040, 256 },
			{ 87296, 64 },
			{ 87360, 16 },
			{ 87376, 4 },
			{ 87380, 4 },
			{ 87384, 4 },
			{ 87388, 4 },
			{ 87392, 4 },
			{ 87396, 4 },
		},
	},
	{
		{
			{ 0, 262144 },
			{ 262144, 65536 },
			{ 327680, 16384 },
			{ 344064, 4096 },
			{ 348160, 1024 },
			{ 349184, 256 },
			{ 349440, 64 },
			{ 349504, 16 },
			{ 349520, 4 },
			{ 349524, 1 },
			{ 349525, 1 },
			{ 349526, 1 },
			{ 349527, 1 },
			{ 349528, 1 },
		},
		{
			{ 0, 524288 },
			{ 524288, 131072 },
			{ 655360, 32768 },
			{ 688128, 8192 },
			{ 696320, 2048 },
			{ 698368, 512 },
			{ 698880, 128 },
			{ 699008, 32 },
			{ 699040, 8 },
			{ 699048, 2 },
			{ 699050, 1 },
			{ 699051, 1 },
			{ 699052, 1 },
			{ 699053, 1 },
		},
		{
			{ 0, 1048576 },
			{ 1048576, 262144 },
			{ 1310720, 65536 },
			{ 1376256, 16384 },
			{ 1392640, 4096 },
			{ 1396736, 1024 },
			{ 1397760, 256 },
			{ 1398016, 64 },
			{ 1398080, 16 },
			{ 1398096, 4 },
			{ 1398100, 1 },
			{ 1398101, 1 },
			{ 1398102, 1 },
			{ 1398103, 1 },
		},
		{
			{ 0, 2097152 },
			{ 2097152, 524288 },
			{ 2621440, 131072 },
			{ 2752512, 32768 },
			{ 2785280, 8192 },
			{ 2793472, 2048 },
			{ 2795520, 512 },
			{ 2796032, 128 },
			{ 2796160, 32 },
			{ 2796192, 8 },
			{ 2796200, 2 },
			{ 2796202, 2 },
			{ 2796204, 2 },
			{ 2796206, 2 },
		},
		{
			{ 0, 4194304 },
			{ 4194304, 1048576 },
			{ 5242880, 262144 },
			{ 5505024, 65536 },
			{ 5570560, 16384 },
			{ 5586944, 4096 },
			{ 5591040, 1024 },
			{ 5592064, 256 },
			{ 5592320, 64 },
			{ 5592384, 16 },
			{ 5592400, 4 },
			{ 5592404, 4 },
			{ 5592408, 4 },
			{ 5592412, 4 },
		},
		{
			{ 0, 131072 },
			{ 131072, 32768 },
			{ 163840, 8192 },
			{ 172032, 2048 },
			{ 174080, 512 },
			{ 174592, 128 },
			{ 174720, 32 },
			{ 174752, 8 },
			{ 174760, 2 },
			{ 174762, 2 },
			{ 174764, 2 },
			{ 174766, 2 },
			{ 174768, 2 },
			{ 174770, 2 },
		},
		{
			{ 0, 262144 },
			{ 262144, 65536 },
			{ 327680, 16384 },
			{ 344064, 4096 },
			{ 348160, 1024 },
			{ 349184, 256 },
			{ 349440, 64 },
			{ 349504, 16 },
			{ 349520, 4 },
			{ 349524, 4 },
			{ 349528, 4 },
			{ 349532, 4 },
			{ 349536, 4 },
			{ 349540, 4 },
		},
	},
	{
		{
			{ 0, 1048576 },
			{ 1048576, 262144 },
			{ 1310720, 65536 },
			{ 1376256, 16384 },
			{ 1392640, 4096 },
			{ 1396736, 1024 },
			{ 1397760, 256 },
			{ 1398016, 64 },
			{ 1398080, 16 },
			{ 1398096, 4 },
			{ 1398100, 1 },
			{ 1398101, 1 },
			{ 1398102, 1 },
			{ 1398103, 1 },
			{ 1398104, 1 },
		},
		{
			{ 0, 2097152 },
			{ 2097152, 524288 },
			{ 2621440, 131072 },
			{ 2752512, 32768 },
			{ 2785280, 8192 },
			{ 2793472, 2048 },
			{ 2795520, 512 },
			{ 2796032, 128 },
			{ 2796160, 32 },
			{ 2796192, 8 },
			{ 2796200, 2 },
			{ 2796202, 1 },
			{ 2796203, 1 },
			{ 2796204, 1 },
			{ 2796205, 1 },
		},
		{
			{ 0, 4194304 },
			{ 4194304, 1048576 },
			{ 5242880, 262144 },
			{ 5505024, 65536 },
			{ 5570560, 16384 },
			{ 5586944, 4096 },
			{ 5591040, 1024 },
			{ 5592064, 256 },
			{ 5592320, 64 },
			{ 5592384, 16 },
			{ 5592400, 4 },
			{ 5592404, 1 },
			{ 5592405, 1 },
			{ 5592406, 1 },
			{ 5592407, 1 },
		},
		{
			{ 0, 8388608 },
			{ 8388608, 2097152 },
			{ 10485760, 524288 },
			{ 11010048, 131072 },
			{ 11141120, 32768 },
			{ 11173888, 8192 },
			{ 11182080, 2048 },
			{ 11184128, 512 },
			{ 11184640, 128 },
			{ 11184768, 32 },
			{ 11184800, 8 },
			{ 11184808, 2 },
			{ 11184810, 2 },
			{ 11184812, 2 },
			{ 11184814, 2 },
		},
		{
			{ 0, 16777216 },
			{ 16777216, 4194304 },
			{ 20971520, 1048576 },
			{ 22020096, 262144 },
			{ 22282240, 65536 },
			{ 22347776, 16384 },
			{ 22364160, 4096 },
			{ 22368256, 1024 },
			{ 22369280, 256 },
			{ 22369536, 64 },
			{ 22369600, 16 },
			{ 22369616, 4 },
			{ 22369620, 4 },
			{ 22369624, 4 },
			{ 22369628, 4 },
		},
		{
			{ 0, 524288 },
			{ 524288, 131072 },
			{ 655360, 32768 },
			{ 688128, 8192 },
			{ 696320, 2048 },
			{ 698368, 512 },
			{ 698880, 128 },
			{ 699008, 32 },
			{ 699040, 8 },
			{ 699048, 2 },
			{ 699050, 2 },
			{ 699052, 2 },
			{ 699054, 2 },
			{ 699056, 2 },
			{ 699058, 2 },
		},
		{
			{ 0, 1048576 },
			{ 1048576, 262144 },
			{ 1310720, 65536 },
			{ 1376256, 16384 },
			{ 1392640, 4096 },
			{ 1396736, 1024 },
			{ 1397760, 256 },
			{ 1398016, 64 },
			{ 1398080, 16 },
			{ 1398096, 4 },
			{ 1398100, 4 },
			{ 1398104, 4 },
			{ 1398108, 4 },
			{ 1398112, 4 },
			{ 1398116, 4 },
		},
	},
};


static inline int width(const SurfaceRegion surfaceRegion)
{
	return surfaceRegion.m_right - surfaceRegion.m_left;
}
static inline int height(const SurfaceRegion surfaceRegion)
{
	return surfaceRegion.m_bottom - surfaceRegion.m_top;
}
static inline int depth(const SurfaceRegion surfaceRegion)
{
	return surfaceRegion.m_back - surfaceRegion.m_front;
}

static inline bool hasTexels(const SurfaceRegion surfaceRegion)
{
	return (width(surfaceRegion) > 0) && (height(surfaceRegion) > 0) && (depth(surfaceRegion) > 0);
}

static inline int32_t getTiledElementByteOffset(const Tiler1d* tiler, uint64_t* outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t /*fragmentIndex*/)
{
	return tiler->getTiledElementByteOffset(outTiledByteOffset, x, y, z);
}

static uint32_t getElementIndex(uint32_t x, uint32_t y, uint32_t z, uint32_t bitsPerElement, MicroTileMode microTileMode, ArrayMode arrayMode)
{
	uint32_t elem = 0;

	if (microTileMode == kMicroTileModeDisplay)
	{
		switch (bitsPerElement)
		{
		case 8:
			elem |= ((x >> 0) & 0x1) << 0;
			elem |= ((x >> 1) & 0x1) << 1;
			elem |= ((x >> 2) & 0x1) << 2;
			elem |= ((y >> 1) & 0x1) << 3;
			elem |= ((y >> 0) & 0x1) << 4;
			elem |= ((y >> 2) & 0x1) << 5;
			break;
		case 16:
			elem |= ((x >> 0) & 0x1) << 0;
			elem |= ((x >> 1) & 0x1) << 1;
			elem |= ((x >> 2) & 0x1) << 2;
			elem |= ((y >> 0) & 0x1) << 3;
			elem |= ((y >> 1) & 0x1) << 4;
			elem |= ((y >> 2) & 0x1) << 5;
			break;
		case 32:
			elem |= ((x >> 0) & 0x1) << 0;
			elem |= ((x >> 1) & 0x1) << 1;
			elem |= ((y >> 0) & 0x1) << 2;
			elem |= ((x >> 2) & 0x1) << 3;
			elem |= ((y >> 1) & 0x1) << 4;
			elem |= ((y >> 2) & 0x1) << 5;
			break;
		case 64:
			elem |= ((x >> 0) & 0x1) << 0;
			elem |= ((y >> 0) & 0x1) << 1;
			elem |= ((x >> 1) & 0x1) << 2;
			elem |= ((x >> 2) & 0x1) << 3;
			elem |= ((y >> 1) & 0x1) << 4;
			elem |= ((y >> 2) & 0x1) << 5;
			break;
		default:
			LOG_ERR("Unsupported bitsPerElement (%u) for displayable surface.", bitsPerElement);
		}
	}
	else if (microTileMode == kMicroTileModeThin || microTileMode == kMicroTileModeDepth)
	{
		elem |= ((x >> 0) & 0x1) << 0;
		elem |= ((y >> 0) & 0x1) << 1;
		elem |= ((x >> 1) & 0x1) << 2;
		elem |= ((y >> 1) & 0x1) << 3;
		elem |= ((x >> 2) & 0x1) << 4;
		elem |= ((y >> 2) & 0x1) << 5;
		// Use Z too, if the array mode is Thick/XThick
		switch (arrayMode)
		{
		case kArrayMode2dTiledXThick:
		case kArrayMode3dTiledXThick:
			elem |= ((z >> 2) & 0x1) << 8;
			// Intentional fall-through
		case kArrayMode1dTiledThick:
		case kArrayMode2dTiledThick:
		case kArrayMode3dTiledThick:
		case kArrayModeTiledThickPrt:
		case kArrayMode2dTiledThickPrt:
		case kArrayMode3dTiledThickPrt:
			elem |= ((z >> 0) & 0x1) << 6;
			elem |= ((z >> 1) & 0x1) << 7;
		default:
			break;  // no other thick modes
		}
	}
	else if (microTileMode == kMicroTileModeThick)  // thick/xthick
	{
		switch (arrayMode)
		{
		case kArrayMode2dTiledXThick:
		case kArrayMode3dTiledXThick:
			elem |= ((z >> 2) & 0x1) << 8;
			// intentional fall-through
		case kArrayMode1dTiledThick:
		case kArrayMode2dTiledThick:
		case kArrayMode3dTiledThick:
		case kArrayModeTiledThickPrt:
		case kArrayMode2dTiledThickPrt:
		case kArrayMode3dTiledThickPrt:
			if (bitsPerElement == 8 || bitsPerElement == 16)
			{
				elem |= ((x >> 0) & 0x1) << 0;
				elem |= ((y >> 0) & 0x1) << 1;
				elem |= ((x >> 1) & 0x1) << 2;
				elem |= ((y >> 1) & 0x1) << 3;
				elem |= ((z >> 0) & 0x1) << 4;
				elem |= ((z >> 1) & 0x1) << 5;
				elem |= ((x >> 2) & 0x1) << 6;
				elem |= ((y >> 2) & 0x1) << 7;
			}
			else if (bitsPerElement == 32)
			{
				elem |= ((x >> 0) & 0x1) << 0;
				elem |= ((y >> 0) & 0x1) << 1;
				elem |= ((x >> 1) & 0x1) << 2;
				elem |= ((z >> 0) & 0x1) << 3;
				elem |= ((y >> 1) & 0x1) << 4;
				elem |= ((z >> 1) & 0x1) << 5;
				elem |= ((x >> 2) & 0x1) << 6;
				elem |= ((y >> 2) & 0x1) << 7;
			}
			else if (bitsPerElement == 64 || bitsPerElement == 128)
			{
				elem |= ((x >> 0) & 0x1) << 0;
				elem |= ((y >> 0) & 0x1) << 1;
				elem |= ((z >> 0) & 0x1) << 2;
				elem |= ((x >> 1) & 0x1) << 3;
				elem |= ((y >> 1) & 0x1) << 4;
				elem |= ((z >> 1) & 0x1) << 5;
				elem |= ((x >> 2) & 0x1) << 6;
				elem |= ((y >> 2) & 0x1) << 7;
			}
			else
			{
				LOG_ERR("Invalid bitsPerElement (%u) for microTileMode=kMicroTileModeThick.", bitsPerElement);
			}
			break;
		default:
			LOG_ERR("Invalid arrayMode (0x%02X) for thick/xthick microTileMode=kMicroTileModeThick.", arrayMode);
		}
	}
	// TODO: rotated

	return elem;
}

int32_t computeLinearElementByteOffset(
	uint64_t* outUntiledByteOffset, 
	uint32_t x, uint32_t y, uint32_t z, 
	uint32_t fragmentIndex, uint32_t pitch, 
	uint32_t SlicePitchElems, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	LOG_ASSERT_RETURN(isPowerOfTwo(numFragmentsPerPixel) && numFragmentsPerPixel <= 8, 
		kStatusInvalidArgument, 
		"numFragmentsPerPixel (%u) must be 1, 2, 4, or 8.", numFragmentsPerPixel);

	uint64_t absolute_element_index = z * SlicePitchElems + y * pitch + x;
	*outUntiledByteOffset           = (absolute_element_index * bitsPerElement * numFragmentsPerPixel) + (bitsPerElement * fragmentIndex);
	*outUntiledByteOffset /= 8;
	return kStatusSuccess;
}

int32_t computeTiledSurfaceSize(uint64_t* outSize, AlignmentType* outAlign, const TilingParameters* tp)
{
	SurfaceInfo surfInfoOut = { 0 };
	int32_t status          = computeSurfaceInfo(&surfInfoOut, tp);
	if (status != kStatusSuccess)
		return status;

	*outSize  = surfInfoOut.m_surfaceSize;
	*outAlign = (AlignmentType)surfInfoOut.m_baseAlign;
	return kStatusSuccess;
}

int32_t computeTotalTiledTextureSize(uint64_t* outSize, AlignmentType* outAlign, const GnmTexture* texture)
{
	const auto isCubemap = (texture->getTextureType() == kTextureTypeCubemap);
	const auto isVolume  = (texture->getTextureType() == kTextureType3d);
	auto arraySliceCount = texture->getTotalArraySliceCount();
	if (isCubemap)
		arraySliceCount *= 6;  // cube maps store six faces per array slice
	else if (isVolume)
		arraySliceCount = 1;  // volume textures can't be arrays
	if (texture->isPaddedToPow2())
		arraySliceCount = nextPowerOfTwo((uint32_t)arraySliceCount);  // array slice counts are padded to a power of two

	switch (texture->getTextureType())
	{
	case kTextureType2d:
	case kTextureType2dArray:
	case kTextureTypeCubemap:
		if ((texture->getTileMode() == kTileModeThin_1dThin) && isPowerOfTwo(texture->getWidth()) && (texture->getWidth() == texture->getHeight()))
		{
			const auto xy = fastIntLog2(texture->getWidth());
			const auto f  = fastIntLog2(texture->getDataFormat().getTotalBytesPerElement()) + (texture->getDataFormat().isBlockCompressedFormat() ? 2 : 0);
			if (g_pitches[xy][f] == texture->getPitch())
			{
				const auto mipLevel = texture->getLastMipLevel();
				const auto offset   = uint64_t(g_offsetSizes[xy][f][mipLevel][0]) << 8;
				const auto size     = uint64_t(g_offsetSizes[xy][f][mipLevel][1]) << 8;
				*outSize            = (offset + size) * arraySliceCount;
				*outAlign           = 256;
				return kStatusSuccess;
			}
		}
		break;
	default:
		break;
	}

	*outSize  = 0;
	*outAlign = (AlignmentType)1;
	TilingParameters tpBase;
	tpBase.initFromTexture(texture, 0, 0);
	TilingParameters tpCopy;  // so we can modify the mipLevel field inside the loop
	memcpy(&tpCopy, &tpBase, sizeof(tpCopy));
	uint64_t mipLevelCount = texture->getLastMipLevel() + 1;
	for (uint32_t iMip = 0; iMip < mipLevelCount; ++iMip)
	{
		tpCopy.m_linearWidth    = std::max(tpBase.m_linearWidth >> iMip, 1U);
		tpCopy.m_linearHeight   = std::max(tpBase.m_linearHeight >> iMip, 1U);
		tpCopy.m_linearDepth    = std::max(tpBase.m_linearDepth >> iMip, 1U);
		tpCopy.m_baseTiledPitch = texture->getPitch();
		tpCopy.m_mipLevel       = iMip;
		uint64_t mipSize;
		AlignmentType mipAlign;
		int32_t mipStatus = computeTiledSurfaceSize(&mipSize, &mipAlign, &tpCopy);
		if (mipStatus != kStatusSuccess)
			return mipStatus;
		*outSize += arraySliceCount * mipSize;  // TODO: special-case for cubemaps, where mips >0 have numSlices padded?
		*outAlign = std::max(*outAlign, mipAlign);
		if (tpCopy.m_linearWidth == 1 && tpCopy.m_linearHeight == 1 && tpCopy.m_linearDepth == 1)
			break;  // early-out if we've hit the smallest mip
	}

	return kStatusSuccess;
}

template <typename T, typename ELEMENT, typename DOUBLE_ELEMENT>
void slowDetileOneFragment(const T* t, const SurfaceRegion region, const SurfaceRegion subRegion, int fragment, int destPitch, int destSlicePitch, uint8_t* __restrict out_bytes, const uint8_t* __restrict in_bytes)
{
	const int dx = subRegion.m_left - region.m_left;
	const int dy = subRegion.m_top - region.m_top;
	const int dz = subRegion.m_front - region.m_front;
	for (auto z = 0; z < depth(subRegion); ++z)
		for (auto y = 0; y < height(subRegion); ++y)
		{
			uint64_t linear_offset, tiled_offset;
			auto x = 0;
			computeLinearElementByteOffset(&linear_offset, dx + x, dy + y, dz + z, 0, destPitch, destSlicePitch, sizeof(ELEMENT) * 8, 1);
			if (subRegion.m_left & 1)
			{
				getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
				*static_cast<ELEMENT*>(static_cast<void*>(out_bytes + linear_offset)) = *static_cast<const ELEMENT*>(static_cast<const void*>(in_bytes + tiled_offset));
				linear_offset += sizeof(ELEMENT);
				++x;
			}
			for (; x + 1 < width(subRegion); x += 2)
			{
				getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
				*static_cast<DOUBLE_ELEMENT*>(static_cast<void*>(out_bytes + linear_offset)) = *static_cast<const DOUBLE_ELEMENT*>(static_cast<const void*>(in_bytes + tiled_offset));
				linear_offset += sizeof(DOUBLE_ELEMENT);
			}
			if (x < width(subRegion))
			{
				getTiledElementByteOffset(t, &tiled_offset, subRegion.m_left + x, subRegion.m_top + y, subRegion.m_front + z, fragment);
				*static_cast<ELEMENT*>(static_cast<void*>(out_bytes + linear_offset)) = *static_cast<const ELEMENT*>(static_cast<const void*>(in_bytes + tiled_offset));
				linear_offset += sizeof(ELEMENT);
			}
		}
}

template <typename T>
void slowDetileOneFragment(const T* t, const SurfaceRegion region, const SurfaceRegion subRegion, int fragment, int destPitch, int destSlicePitch, uint8_t* __restrict out_bytes, const uint8_t* __restrict in_bytes, int bytesPerElement)
{
	if (!hasTexels(subRegion))
		return;
	switch (bytesPerElement)
	{
	case 1:
		slowDetileOneFragment<T, uint8_t, uint16_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
		break;
	case 2:
		slowDetileOneFragment<T, uint16_t, uint32_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
		break;
	case 4:
		slowDetileOneFragment<T, uint32_t, uint64_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
		break;
	case 8:
		slowDetileOneFragment<T, uint64_t, uint128_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
		break;
	case 16:
		slowDetileOneFragment<T, uint128_t, uint256_t>(t, region, subRegion, fragment, destPitch, destSlicePitch, out_bytes, in_bytes);
		break;
	default:
		break;
	}
}

static MicroTileFunc getDetileFuncSse2(const MicroTileMode microTileMode, const uint32_t bitsPerElement)
{
	switch (microTileMode)
	{
	//case kMicroTileModeDisplay:
	//	if (bitsPerElement == 8)
	//		return detile8bppDisplaySse2;
	//	if (bitsPerElement == 16)
	//		return detile16bppDisplaySse2;
	//	if (bitsPerElement == 32)
	//		return detile32bppDisplaySse2;
	//	if (bitsPerElement == 64)
	//		return detile64bppDisplaySse2;
	//	return NULL;
	case kMicroTileModeDepth:
	case kMicroTileModeThin:
		if (bitsPerElement == 8)
			return detile8bppThinSse2;
		if (bitsPerElement == 16)
			return detile16bppThinSse2;
		if (bitsPerElement == 32)
			return detile32bppThinSse2;
		if (bitsPerElement == 64)
			return detile64bppThinSse2;
		if (bitsPerElement == 128)
			return detile128bppThinSse2;
		return NULL;
	//case kMicroTileModeRotated:
	//	return NULL;
	//case kMicroTileModeThick:
	//	if (bitsPerElement == 8)
	//		return detile8bppThickSse2;
	//	if (bitsPerElement == 16)
	//		return detile16bppThickSse2;
	//	if (bitsPerElement == 32)
	//		return detile32bppThickSse2;
	//	if (bitsPerElement == 64)
	//		return detile64bppThickSse2;
	//	if (bitsPerElement == 128)
	//		return detile128bppThickSse2;
	//	return NULL;
	default:
		LOG_ERR("Unknown microTileMode %d", microTileMode);
		return NULL;
	}
}

struct Regions
{
	SurfaceRegion m_aligned;
	SurfaceRegion m_unaligned[6];
	int m_unaligneds;
	void Init(const SurfaceRegion region, int tileThickness)
	{
		const auto t       = tileThickness - 1;
		m_aligned          = region;
		m_aligned.m_left   = (m_aligned.m_left + 7) & ~7;
		m_aligned.m_right  = (m_aligned.m_right) & ~7;
		m_aligned.m_top    = (m_aligned.m_top + 7) & ~7;
		m_aligned.m_bottom = (m_aligned.m_bottom) & ~7;
		m_aligned.m_front  = (m_aligned.m_front + t) & ~t;
		m_aligned.m_back   = (m_aligned.m_back) & ~t;

		m_unaligneds = 0;
		if (m_aligned == region)
			return;
		if (!hasTexels(m_aligned))
		{
			memset(&m_aligned, 0, sizeof(m_aligned));
			m_unaligned[0] = region;
			m_unaligneds   = 1;
			return;
		}

		SurfaceRegion temp = region;

		if (temp.m_left < m_aligned.m_left)
		{
			m_unaligned[m_unaligneds] = temp;
			temp.m_left = m_unaligned[m_unaligneds].m_right = m_aligned.m_left;
			++m_unaligneds;
		}
		if (temp.m_right > m_aligned.m_right)
		{
			m_unaligned[m_unaligneds] = temp;
			temp.m_right = m_unaligned[m_unaligneds].m_left = m_aligned.m_right;
			++m_unaligneds;
		}

		if (temp.m_top < m_aligned.m_top)
		{
			m_unaligned[m_unaligneds] = temp;
			temp.m_top = m_unaligned[m_unaligneds].m_bottom = m_aligned.m_top;
			++m_unaligneds;
		}
		if (temp.m_bottom > m_aligned.m_bottom)
		{
			m_unaligned[m_unaligneds] = temp;
			temp.m_bottom = m_unaligned[m_unaligneds].m_top = m_aligned.m_bottom;
			++m_unaligneds;
		}

		if (temp.m_front < m_aligned.m_front)
		{
			m_unaligned[m_unaligneds] = temp;
			temp.m_front = m_unaligned[m_unaligneds].m_back = m_aligned.m_front;
			++m_unaligneds;
		}
		if (temp.m_back > m_aligned.m_back)
		{
			m_unaligned[m_unaligneds] = temp;
			temp.m_back = m_unaligned[m_unaligneds].m_front = m_aligned.m_back;
			++m_unaligneds;
		}
	}
};


int32_t TilingParameters::initFromTexture(const GnmTexture* texture, uint32_t mipLevel, uint32_t arraySlice)
{
	int32_t status = kStatusInvalidArgument;
	do 
	{
		if (!texture || mipLevel > texture->getLastMipLevel())
		{
			break;
		}

		bool isCubemap = (texture->getTextureType() == kTextureTypeCubemap);
		bool isVolume  = (texture->getTextureType() == kTextureType3d);
		
		m_surfaceFlags.m_value = 0;
		MicroTileMode microTileMode;
		status = getMicroTileMode(&microTileMode, texture->getTileMode());
		if (status != kStatusSuccess)
		{
			break;
		}

		m_surfaceFlags.m_depthTarget   = (!isVolume && (microTileMode == kMicroTileModeDepth) && (texture->getDataFormat().getZFormat() != kZFormatInvalid)) ? 1 : 0;
		m_surfaceFlags.m_stencilTarget = (!isVolume && (microTileMode == kMicroTileModeDepth) && (texture->getDataFormat().getStencilFormat() != kStencilInvalid)) ? 1 : 0;
		m_surfaceFlags.m_cube          = isCubemap ? 1 : 0;
		m_surfaceFlags.m_volume        = isVolume ? 1 : 0;
		m_surfaceFlags.m_pow2Pad       = texture->isPaddedToPow2() ? 1 : 0;
		if (texture->getMinimumGpuMode() == kGpuModeNeo)
		{
			m_surfaceFlags.m_texCompatible = 1;
		}
		m_tileMode                 = texture->getTileMode();
		m_minGpuMode               = texture->getMinimumGpuMode();
		DataFormat dataFormat      = texture->getDataFormat();
		m_bitsPerFragment          = dataFormat.getTotalBitsPerElement() / dataFormat.getTexelsPerElement();
		m_isBlockCompressed        = (dataFormat.getTexelsPerElement() > 1);
		m_tileSwizzleMask          = texture->getTileSwizzleMask();
		m_linearWidth              = std::max(texture->getWidth() >> mipLevel, 1U);
		m_linearHeight             = std::max(texture->getHeight() >> mipLevel, 1U);
		m_linearDepth              = m_surfaceFlags.m_volume ? std::max(texture->getDepth() >> mipLevel, 1U) : 1;
		m_numFragmentsPerPixel     = 1 << texture->getNumFragments();
		m_baseTiledPitch           = texture->getPitch();
		m_mipLevel                 = mipLevel;

		// arraySlice of volume textures must be 0 
		if (m_surfaceFlags.m_volume && arraySlice != 0)
		{
			break;
		}
		
		uint32_t arraySliceCount = texture->getTotalArraySliceCount();
		if (isCubemap)
		{
			arraySliceCount *= 6;
		}
		else if (isVolume)
		{
			arraySliceCount = 1;
		}
			
		if (texture->isPaddedToPow2())
		{
			arraySliceCount = nextPowerOfTwo(arraySliceCount);
		}
			
		if (arraySlice >= arraySliceCount)
		{
			break;
		}
		m_arraySlice            = arraySlice;

		SurfaceInfo surfInfoOut = { 0 };
		status                  = computeSurfaceInfo(&surfInfoOut, this);
		if (status != kStatusSuccess)
		{
			break;
		}
		status = adjustTileMode(m_minGpuMode, &m_tileMode, m_tileMode, surfInfoOut.m_arrayMode);
		if (status != kStatusSuccess)
		{
			break;
		}

		status = kStatusSuccess;
	} while (false);
	return status;
}

int32_t detileSurface(void* outUntiledPixels, const void* tiledPixels, const TilingParameters* tp)
{
	SurfaceRegion srcRegion;
	srcRegion.m_left = srcRegion.m_top = srcRegion.m_front = 0;
	srcRegion.m_right                                      = tp->m_linearWidth;
	srcRegion.m_bottom                                     = tp->m_linearHeight;
	srcRegion.m_back                                       = tp->m_linearDepth;
	uint32_t elemWidth                                     = tp->m_linearWidth;
	uint32_t elemHeight                                    = tp->m_linearHeight;
	if (tp->m_isBlockCompressed)
	{
		switch (tp->m_bitsPerFragment)
		{
		case 1:
			srcRegion.m_left  = (srcRegion.m_left + 7) / 8;
			srcRegion.m_right = (srcRegion.m_right + 7) / 8;
			elemWidth         = (elemWidth + 7) / 8;
			break;
		case 4:
		case 8:
			srcRegion.m_left   = (srcRegion.m_left + 3) / 4;
			srcRegion.m_top    = (srcRegion.m_top + 3) / 4;
			srcRegion.m_right  = (srcRegion.m_right + 3) / 4;
			srcRegion.m_bottom = (srcRegion.m_bottom + 3) / 4;
			elemWidth          = (elemWidth + 3) / 4;
			elemHeight         = (elemHeight + 3) / 4;
			// srcRegion.m_back doesn't need to be rescaled; BCn blocks don't have thickness.
			break;
		case 16:
			// TODO
			break;
		default:
			LOG_ASSERT_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument, "Unknown bit depth %u for block-compressed format", tp->m_bitsPerFragment);
			break;
		}
	}
	return detileSurfaceRegion(outUntiledPixels, tiledPixels, tp, &srcRegion, elemWidth, elemWidth * elemHeight);
}

int32_t detileSurfaceRegion(void* outUntiledPixels, const void* tiledPixels, const TilingParameters* tp, const SurfaceRegion* srcRegion, uint32_t destPitch, uint32_t destSlicePitchElems)
{
	SurfaceInfo surfInfoOut = { 0 };
	int32_t status          = computeSurfaceInfo(&surfInfoOut, tp);
	if (status != kStatusSuccess)
		return status;

	TilingParameters correctedTP;
	memcpy(&correctedTP, tp, sizeof(correctedTP));
	status = adjustTileMode(correctedTP.m_minGpuMode, &correctedTP.m_tileMode, tp->m_tileMode, surfInfoOut.m_arrayMode);
	if (status != kStatusSuccess)
		return status;

	ArrayMode arrayMode;
	status = getArrayMode(&arrayMode, correctedTP.m_tileMode);
	if (status != kStatusSuccess)
		return status;
	switch (arrayMode)
	{
	case kArrayMode1dTiledThin:
	case kArrayMode1dTiledThick:
	{
		Tiler1d tiler(&correctedTP);
		return tiler.detileSurfaceRegion(outUntiledPixels, tiledPixels, srcRegion, destPitch, destSlicePitchElems);
	}
	case kArrayModeLinearGeneral:
	case kArrayModeLinearAligned:
	case kArrayMode2dTiledThin:
	case kArrayMode2dTiledThick:
	case kArrayMode2dTiledXThick:
	case kArrayMode3dTiledThin:
	case kArrayMode3dTiledThick:
	case kArrayMode3dTiledXThick:
	case kArrayModeTiledThinPrt:
	case kArrayModeTiledThickPrt:
	case kArrayMode2dTiledThinPrt:
	case kArrayMode2dTiledThickPrt:
	case kArrayMode3dTiledThinPrt:
	case kArrayMode3dTiledThickPrt:
	default:
		// Unsupported
		LOG_FIXME("Invalid corrected tile mode (0x%02X).", correctedTP.m_tileMode);
		return kStatusInvalidArgument;
	}
}


Tiler1d::Tiler1d()
{
}

Tiler1d::Tiler1d(const TilingParameters* tp)
{
	int32_t status = init(tp);
	LOG_ASSERT(status == kStatusSuccess, "Tiler1d initialization failed with error code %d", status);
}

Tiler1d::~Tiler1d()
{
}

int32_t Tiler1d::init(const TilingParameters* tp)
{
	LOG_ASSERT_RETURN(tp != 0, kStatusInvalidArgument, "tp must not be NULL.");
	LOG_ASSERT_RETURN(tp->m_numFragmentsPerPixel == 1, kStatusInvalidArgument, 
		"tp->m_numFragmentsPerPixel must be 1 for 1D-tiled surfaces.");
	
	SurfaceInfo surfInfoOut = { 0 };
	int32_t status          = computeSurfaceInfo(&surfInfoOut, tp);
	LOG_ASSERT_RETURN(status == kStatusSuccess, status, "computeSurfaceInfo() failed: %d", status);

	m_minGpuMode = tp->m_minGpuMode;
	MicroTileMode microTileMode;
	getMicroTileMode(&microTileMode, tp->m_tileMode);
	m_tileMode       = tp->m_tileMode;
	m_arrayMode      = surfInfoOut.m_arrayMode;
	m_microTileMode  = microTileMode;
	m_linearWidth    = tp->m_linearWidth;
	m_linearHeight   = tp->m_linearHeight;
	m_linearDepth    = tp->m_linearDepth;
	m_bitsPerElement = tp->m_bitsPerFragment;
	m_paddedWidth    = surfInfoOut.m_pitch;
	m_paddedHeight   = surfInfoOut.m_height;
	m_paddedDepth    = surfInfoOut.m_depth;

	if (tp->m_isBlockCompressed)
	{
		switch (tp->m_bitsPerFragment)
		{
		case 1:
			LOG_ASSERT_RETURN(m_microTileMode == kMicroTileModeDisplay, kStatusInvalidArgument, 
				"1bpp surfaces must use kMicroTileModeDisplay");
			m_bitsPerElement *= 8;
			m_linearWidth = std::max((m_linearWidth + 7) / 8, 1U);
			m_paddedWidth = std::max((m_paddedWidth + 7) / 8, 1U);
			break;
		case 4:
		case 8:
			m_bitsPerElement *= 16;
			m_linearWidth  = std::max((m_linearWidth + 3) / 4, 1U);
			m_linearHeight = std::max((m_linearHeight + 3) / 4, 1U);
			m_paddedWidth  = std::max((m_paddedWidth + 3) / 4, 1U);
			m_paddedHeight = std::max((m_paddedHeight + 3) / 4, 1U);
			break;
		case 16:
			// TODO
			break;
		default:
			LOG_ASSERT_RETURN(!tp->m_isBlockCompressed, kStatusInvalidArgument,
				"Unknown bit depth %u for block-compressed format", m_bitsPerElement);
			break;
		}
	}
	m_linearSizeBytes = (m_linearWidth * m_linearHeight * m_linearDepth * m_bitsPerElement + 7) / 8;
	m_tiledSizeBytes  = static_cast<uint32_t>(surfInfoOut.m_surfaceSize);

	m_tileThickness = (m_arrayMode == kArrayMode1dTiledThick) ? 4 : 1;
	m_tileBytes     = (kMicroTileWidth * kMicroTileHeight * m_tileThickness * m_bitsPerElement + 7) / 8;
	m_tilesPerRow   = m_paddedWidth / kMicroTileWidth;
	m_tilesPerSlice = std::max(m_tilesPerRow * (m_paddedHeight / kMicroTileHeight), 1U);

	// If any of these asserts failed, then computeSurfaceInfo() is not correct.
	LOG_ASSERT_RETURN(m_paddedWidth % kMicroTileWidth == 0, kStatusInternalTilingError, "internal consistency check failed.");
	LOG_ASSERT_RETURN(m_paddedHeight % kMicroTileHeight == 0, kStatusInternalTilingError, "internal consistency check failed.");
	LOG_ASSERT_RETURN(m_paddedDepth % m_tileThickness == 0, kStatusInternalTilingError, "internal consistency check failed.");
	LOG_ASSERT_RETURN(((m_paddedHeight * m_paddedWidth * m_tileThickness * m_bitsPerElement / 8) % kPipeInterleaveBytes) == 0, kStatusInternalTilingError, "internal consistency check failed.");
	return kStatusSuccess;
}

int32_t Tiler1d::detileSurface(void* outUntiledPixels, const void* tiledPixels)
{
	SurfaceRegion srcRegion;
	srcRegion.m_left = srcRegion.m_top = srcRegion.m_front = 0;
	srcRegion.m_right                                      = m_linearWidth;
	srcRegion.m_bottom                                     = m_linearHeight;
	srcRegion.m_back                                       = m_linearDepth;
	return detileSurfaceRegion(outUntiledPixels, tiledPixels, &srcRegion, m_linearWidth, m_linearWidth * m_linearHeight);
}

int32_t Tiler1d::detileSurfaceRegion(void* outUntiledPixels, const void* inTiledPixels, const SurfaceRegion* srcRegion, uint32_t destPitch, uint32_t destSlicePitch)
{
	LOG_ASSERT_RETURN(outUntiledPixels != 0, kStatusInvalidArgument, "outUntiledPixels must not be NULL.");
	LOG_ASSERT_RETURN(inTiledPixels != 0, kStatusInvalidArgument, "inTiledPixels must not be NULL.");
	LOG_ASSERT_RETURN(srcRegion != 0, kStatusInvalidArgument, "srcRegion must not be NULL.");
	const auto region = *srcRegion;
	if (!hasTexels(region))
		return kStatusSuccess;  // Zero-area region; nothing to do.
	LOG_ASSERT_RETURN(region.m_right <= m_linearWidth, kStatusInvalidArgument, "srcRegion m_right (%u) must not exceed destination surface's width (%u).", region.m_right, m_linearWidth);
	LOG_ASSERT_RETURN(region.m_bottom <= m_linearHeight, kStatusInvalidArgument, "srcRegion m_bottom (%u) must not exceed destination surface's height (%u).", region.m_bottom, m_linearHeight);
	LOG_ASSERT_RETURN(region.m_back <= m_linearDepth, kStatusInvalidArgument, "srcRegion m_back (%u) must not exceed destination surface's depth (%u).", region.m_back, m_linearDepth);
	LOG_ASSERT_RETURN(width(region) <= int(destPitch), kStatusInvalidArgument, "srcRegion width (%u) must not exceed destPitch (%u).", width(region), destPitch);
	LOG_ASSERT_RETURN(width(region) * height(region) <= int(destSlicePitch), kStatusInvalidArgument, "srcRegion X*Y dimensions (%ux%u) must not exceed destSlicePitch (%u).", width(region), height(region), destSlicePitch);

	const auto in_bytes        = static_cast<const uint8_t*>(inTiledPixels);
	const auto out_bytes       = static_cast<uint8_t*>(outUntiledPixels);
	const auto bytesPerElement = m_bitsPerElement / 8;

	const auto detileFunc = getDetileFuncSse2(m_microTileMode, m_bitsPerElement);
	if (nullptr != detileFunc && (intptr_t(in_bytes) % 16) == 0)
	{
		Regions regions;
		regions.Init(region, m_tileThickness);
		if (hasTexels(regions.m_aligned))
		{
			const auto dx = regions.m_aligned.m_left - srcRegion->m_left;
			const auto dy = regions.m_aligned.m_top - srcRegion->m_top;
			const auto dz = regions.m_aligned.m_front - srcRegion->m_front;
			for (auto z = 0; z < depth(regions.m_aligned); z += m_tileThickness)
				for (auto y = 0; y < height(regions.m_aligned); y += kMicroTileHeight)
				{
					uint64_t inBaseOffset = 0, outBaseOffset = 0;
					getTiledElementByteOffset(&inBaseOffset, regions.m_aligned.m_left + 0, regions.m_aligned.m_top + y, regions.m_aligned.m_front + z);
					computeLinearElementByteOffset(&outBaseOffset, dx + 0, dy + y, dz + z, 0, destPitch, destSlicePitch, m_bitsPerElement, 1);
					for (auto x = 0; x < width(regions.m_aligned); x += kMicroTileWidth)
					{
						detileFunc(out_bytes + outBaseOffset, in_bytes + inBaseOffset, destPitch, destSlicePitch);
						inBaseOffset += m_tileBytes;
						outBaseOffset += kMicroTileWidth * bytesPerElement;
					}
				}
			for (auto i = 0; i < regions.m_unaligneds; ++i)
				slowDetileOneFragment<Tiler1d>(this, region, regions.m_unaligned[i], 0, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
		}
	}
	slowDetileOneFragment<Tiler1d>(this, region, region, 0, destPitch, destSlicePitch, out_bytes, in_bytes, bytesPerElement);
	return kStatusSuccess;
}

int32_t Tiler1d::getTiledElementByteOffset(uint64_t* outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const
{
	uint64_t bitOffset  = 0;
	int32_t status      = getTiledElementBitOffset(&bitOffset, x, y, z);
	*outTiledByteOffset = bitOffset / 8;
	return status;
}

int32_t Tiler1d::getTiledElementBitOffset(uint64_t* outTiledBitOffset, uint32_t x, uint32_t y, uint32_t z) const
{
	uint64_t element_index = getElementIndex(x, y, z, m_bitsPerElement, m_microTileMode, m_arrayMode);

	uint64_t slice_offset = (z / m_tileThickness) * m_tilesPerSlice * m_tileBytes;

	uint64_t tile_row_index    = y / kMicroTileHeight;
	uint64_t tile_column_index = x / kMicroTileWidth;
	uint64_t tile_offset       = ((tile_row_index * m_tilesPerRow) + tile_column_index) * m_tileBytes;

	uint64_t element_offset = element_index * m_bitsPerElement;

	uint64_t final_offset = (slice_offset + tile_offset) * 8 + element_offset;

	*outTiledBitOffset = final_offset;
	return kStatusSuccess;
}

}  // namespace GpuAddress