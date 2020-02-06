#pragma once

#include "GnmCommon.h"

// Note:
// There are to ways to interpret these buffer types (V# T# S#).
// One is to cast to XSharpBuffer defined in this file, these definitions are hardware related.
// Another is to cast to GnmBuffer, GnmTexture or GnmSampler class defined in correspond header files.
// These definitions are more human friendly.


// V# Vertex Descriptor Buffer
// 128 bits
struct VSharpBuffer
{
	uint64_t base			: 44;  // base byte address (only 40 bits supported)
	uint64_t mtype_L1s		: 2;   // mtype for scalar L1
	uint64_t mtype_L2		: 2;   // mtype for L2
	uint64_t stride			: 14;  // bytes: 0..16383
	uint64_t cache_swizzle  : 1;   // buffer access. optionally swizzle TC L1 cache banks
	uint64_t swizzle_en		: 1;   // swizzle AOS according to stride, index_stride, and element_size, else linear (stride * index + offset)

	uint32_t num_records;  // in units of 'stride'

	// Destination channel select:
	// 0=0, 1=1, 4=R, 5=G, 6=B, 7=A
	uint32_t dst_sel_x : 3;
	uint32_t dst_sel_y : 3;
	uint32_t dst_sel_z : 3;
	uint32_t dst_sel_w : 3;

	uint32_t nfmt			: 3;  // numeric data type (float, int, бн)
	uint32_t dfmt			: 4;  // # of fields, size of each field. Note: dfmt=0 (invalid) is a special case that will disable buffer access via vector memory ops.
	uint32_t element_size	: 2;  // 2, 4, 8, or 16 bytes. Used for swizzled buffer addressing
	uint32_t index_stride	: 2;  // 8, 16, 32, or 64. Used for swizzled buffer addressing
	uint32_t addtid_en		: 1;  // add thread id to the index for addr calc
	uint32_t reserved0		: 1;
	uint32_t hash_en		: 1;  // 1 = buffer addresses are hashed for better cache perf
	uint32_t reserved1		: 1;  
	uint32_t mtype			: 3;  // mtype for L1
	uint32_t type			: 2;  // value == 0 for buf. Overlaps upper 2 bits of 4-bit TYPE field in 128-bit T# resource
};



// T# Texture Descriptor Buffer
// 256 bits
struct TSharpBuffer
{
	uint64_t baseaddr256	: 38;  // base 256-byte aligned address bits [39:8] (top 6 bits are not used)
	uint64_t mtype_L2		: 2;   // mtype for L2
	uint64_t min_lod		: 12;  // fixed point 4.8 minimum LOD (0.0..15.0)
	uint64_t dfmt			: 6;   // texture data format; num components, num bits
	uint64_t nfmt			: 4;   // texture numeric format; value conversion
	uint64_t mtype_lsbs		: 2;   // mtype for L1 (LSBs)

	uint64_t width			: 14;  // texture width (0..16383)
	uint64_t height			: 14;  // texture height (0..16383)
	// Specifies the scale factor applied to the perf_z, perf_mip,
	// aniso_bias, aniso_threshold, lod_bias_sec settings
	// specified in the associated S#:
	// 0=0/16, 1=2/16, 2=5/16, 3=7/16, 4=9/16, 5=11/16, 6=14/16, 7=16/16
	// The result after scaling is rounded down to the nearest
	// representable value for the given S# field.
	// (Note that perf_mod=0 effectively disables these S#
	// settings, while perf_mod=7 essentially eliminates the
	// dependency between the T# and S#.)
	uint64_t perf_mod		: 3;
	uint64_t interlaced		: 1;  // texture is interlaced

	// Destination channel select:
	// 0=0, 1=1, 4=R, 5=G, 6=B, 7=A
	uint64_t dst_sel_x		: 3;
	uint64_t dst_sel_y		: 3;
	uint64_t dst_sel_z		: 3;
	uint64_t dst_sel_w		: 3;
	uint64_t base_level		: 4;
	uint64_t last_level		: 4;
	uint64_t tiling_idx		: 5;  // TileMode
	uint64_t pow2pad		: 1;
	uint64_t mtype_msb		: 1;
	uint64_t reserved0		: 1;
	uint64_t type			: 4;

	uint64_t depth			: 13;
	uint64_t pitch			: 14;  // texture pitch in texels (0..16383); defaults to width
	uint64_t reserved1		: 5;

	uint64_t base_array		: 13;  // first array index (0..16383)
	uint64_t last_array		: 13;  // texture height (0..16383)
	uint64_t reserved2		: 6;

	uint64_t min_lod_warn	: 12;  // min mip level to trigger LWE (LOD warn enable); unsigned fixed point 4.8
	uint64_t counter_bank_id	: 8;  // PRT counter ID
	uint64_t LOD_hdw_cnt_en		: 1;  // PRT hardware counter enable
	uint64_t reserved3			: 43;  // Should be 43 bits, but 52 bits on the manual, an error.
};



// S# Sampler Descriptor Buffer
// 128 bits
struct SSharpBuffer
{
	uint64_t clamp_x : 3;  // Clamp/wrap mode for out of range X coordinate
	uint64_t clamp_y : 3;  // Clamp/wrap mode for out of range Y coordinate
	uint64_t clamp_z : 3;  // Clamp/wrap mode for out of range Z coordinate
	uint64_t max_aniso_ratio	: 3;  // Maximum anisotropy ratio (enum)
	uint64_t depth_compare_func : 3;  // Depth compare function
	uint64_t force_unorm_coords : 1;  // Force unnormalized (texel) address coordinates

	// Threshold before sampling anisotropically(enum)
	// (scaled according to T#.perf_mod setting, hence forced to 0 unless perf_mod != 0)
	uint64_t aniso_threshold	: 3;

	uint64_t mc_coord_trunc		: 1;
	uint64_t force_degamma		: 1;  // Force de-gamma after filtering regardless of format

	// Anisotropy bias factor; unsigned fixed point 1.5
	// (scaled according to T#.perf_mod setting, hence effectively disabled unless perf_mod!=0)
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
	uint64_t z_filter		: 2;  // Filter in Z coordinate direction for volume textures
	uint64_t mip_filter		: 2;  // Filter in LOD coordinate direction for mipped textures
	uint64_t reserved1		: 4;
	uint64_t border_color_ptr	: 12;
	uint64_t reserved2			: 18;
	uint64_t border_color_type	: 2;
};
