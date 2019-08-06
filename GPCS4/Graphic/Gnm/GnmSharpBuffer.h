#pragma once

#include "GPCS4Common.h"

// V# Vertex Descriptor Buffer
// 128 bits
struct VSharpBuffer
{
	uint64_t base			: 44;
	uint64_t mtype_L1s		: 2;
	uint64_t mtype_L2		: 2;
	uint64_t stride			: 14;
	uint64_t cache_swizzle	: 1;
	uint64_t swizzle_en		: 1;

	uint32_t num_records;

	uint32_t dst_sel_x : 3;
	uint32_t dst_sel_y : 3;
	uint32_t dst_sel_z : 3;
	uint32_t dst_sel_w : 3;
	uint32_t nfmt			: 3;
	uint32_t dfmt			: 4;
	uint32_t element_size	: 2;
	uint32_t index_stride	: 2;
	uint32_t addtid_en		: 1;
	uint32_t reserved0		: 1;
	uint32_t hash_en		: 1;
	uint32_t reserved1		: 1;
	uint32_t mtype			: 3;
	uint32_t type			: 2;
};

// T# Texture Descriptor Buffer
// 256 bits
struct TSharpBuffer
{
	uint64_t baseaddr256	: 38;
	uint64_t mtype_L2		: 2;
	uint64_t min_lod		: 12;
	uint64_t dfmt			: 6;
	uint64_t nfmt			: 4;
	uint64_t mtype_lsbs		: 2;

	uint64_t width			: 14;
	uint64_t height			: 14;
	uint64_t perf_mod		: 3;
	uint64_t interlaced		: 1;
	uint64_t dst_sel_x		: 3;
	uint64_t dst_sel_y		: 3;
	uint64_t dst_sel_z		: 3;
	uint64_t dst_sel_w		: 3;
	uint64_t base_level		: 4;
	uint64_t last_level		: 4;
	uint64_t tiling_idx		: 5;
	uint64_t pow2pad		: 1;
	uint64_t mtype_msb		: 1;
	uint64_t reserved0		: 1;
	uint64_t type			: 4;

	uint64_t depth			: 13;
	uint64_t pitch			: 14;
	uint64_t reserved1		: 5;
	uint64_t base_array		: 13;
	uint64_t last_array		: 13;
	uint64_t reserved2		: 6;

	uint64_t min_lod_warn	: 12;
	uint64_t counter_bank_id	: 8;
	uint64_t LOD_hdw_cnt_en		: 1;
	uint64_t reserved3			: 52;
};

// S# Sampler Descriptor Buffer
// 128 bits
struct SSharpBuffer
{
	uint64_t clamp_x : 3;
	uint64_t clamp_y : 3;
	uint64_t clamp_z : 3;
	uint64_t max_aniso_ratio	: 3;
	uint64_t depth_compare_func : 3;
	uint64_t force_unorm_coords : 1;
	uint64_t aniso_threshold	: 3;
	uint64_t mc_coord_trunc		: 1;
	uint64_t force_degamma		: 1;
	uint64_t aniso_bias			: 6;
	uint64_t trunc_coord		: 1;
	uint64_t disable_cube_wrap	: 1;
	uint64_t filter_mode		: 2;
	uint64_t reserved0			: 1;
	uint64_t min_lod			: 12;
	uint64_t max_lod			: 12;
	uint64_t perf_mip			: 4;
	uint64_t perf_z				: 4;

	uint64_t lod_bias		: 14;
	uint64_t lod_bias_sec	: 6;
	uint64_t xy_mag_filter	: 2;
	uint64_t xy_min_filter	: 2;
	uint64_t z_filter		: 2;
	uint64_t mip_filter		: 2;
	uint64_t reserved1		: 4;
	uint64_t border_color_ptr	: 12;
	uint64_t reserved2			: 18;
	uint64_t border_color_type	: 2;
};
