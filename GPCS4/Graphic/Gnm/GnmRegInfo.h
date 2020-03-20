#pragma once


#define SCE_GNM_GET_FIELD(src, registername, field) \
	(((src) & (GNM_ ## registername ## __ ## field ## __MASK)) >> (GNM_ ## registername ## __ ## field ## __SHIFT))


#define mmSQ_BUF_RSRC_WORD0                                                              0x23C0
#define GNM_SQ_BUF_RSRC_WORD0__BASE_ADDRESS__MASK                                        0xffffffffL //size:32
#define GNM_SQ_BUF_RSRC_WORD0__BASE_ADDRESS__SHIFT                                        0

#define mmSQ_BUF_RSRC_WORD1                                                              0x23C1
#define GNM_SQ_BUF_RSRC_WORD1__BASE_ADDRESS_HI__MASK                                     0x00000fffL //size:12
#define GNM_SQ_BUF_RSRC_WORD1__STRIDE__MASK                                              0x3fff0000L //size:14
#define GNM_SQ_BUF_RSRC_WORD1__SWIZZLE_ENABLE__MASK                                      0x80000000L //size: 1
#define GNM_SQ_BUF_RSRC_WORD1__BASE_ADDRESS_HI__SHIFT                                     0
#define GNM_SQ_BUF_RSRC_WORD1__STRIDE__SHIFT                                             16
#define GNM_SQ_BUF_RSRC_WORD1__SWIZZLE_ENABLE__SHIFT                                     31

#define mmSQ_BUF_RSRC_WORD2                                                              0x23C2
#define GNM_SQ_BUF_RSRC_WORD2__NUM_RECORDS__MASK                                         0xffffffffL //size:32
#define GNM_SQ_BUF_RSRC_WORD2__NUM_RECORDS__SHIFT                                         0

#define mmSQ_BUF_RSRC_WORD3                                                              0x23C3
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_X__MASK                                           0x00000007L //size: 3
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_Y__MASK                                           0x00000038L //size: 3
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_Z__MASK                                           0x000001c0L //size: 3
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_W__MASK                                           0x00000e00L //size: 3
#define GNM_SQ_BUF_RSRC_WORD3__ELEMENT_SIZE__MASK                                        0x00180000L //size: 2
#define GNM_SQ_BUF_RSRC_WORD3__INDEX_STRIDE__MASK                                        0x00600000L //size: 2
#define GNM_SQ_BUF_RSRC_WORD3__TYPE__MASK                                                0xc0000000L //size: 2
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_X__SHIFT                                           0
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_Y__SHIFT                                           3
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_Z__SHIFT                                           6
#define GNM_SQ_BUF_RSRC_WORD3__DST_SEL_W__SHIFT                                           9
#define GNM_SQ_BUF_RSRC_WORD3__ELEMENT_SIZE__SHIFT                                       19
#define GNM_SQ_BUF_RSRC_WORD3__INDEX_STRIDE__SHIFT                                       21
#define GNM_SQ_BUF_RSRC_WORD3__TYPE__SHIFT                                               30

#define mmSQ_IMG_RSRC_WORD1                                                              0x23C5
#define GNM_SQ_IMG_RSRC_WORD1__MIN_LOD__MASK                                             0x000fff00L //size:12
#define GNM_SQ_IMG_RSRC_WORD1__DATA_FORMAT__MASK                                         0x03f00000L //size: 6
#define GNM_SQ_IMG_RSRC_WORD1__NUM_FORMAT__MASK                                          0x3c000000L //size: 4
#define GNM_SQ_IMG_RSRC_WORD1__MIN_LOD__SHIFT                                             8
#define GNM_SQ_IMG_RSRC_WORD1__DATA_FORMAT__SHIFT                                        20
#define GNM_SQ_IMG_RSRC_WORD1__NUM_FORMAT__SHIFT                                         26

#define mmSQ_IMG_RSRC_WORD2                                                              0x23C6
#define GNM_SQ_IMG_RSRC_WORD2__WIDTH__MASK                                               0x00003fffL //size:14
#define GNM_SQ_IMG_RSRC_WORD2__HEIGHT__MASK                                              0x0fffc000L //size:14
#define GNM_SQ_IMG_RSRC_WORD2__PERF_MOD__MASK                                            0x70000000L //size: 3
#define GNM_SQ_IMG_RSRC_WORD2__WIDTH__SHIFT                                               0
#define GNM_SQ_IMG_RSRC_WORD2__HEIGHT__SHIFT                                             14
#define GNM_SQ_IMG_RSRC_WORD2__PERF_MOD__SHIFT                                           28

#define mmSQ_IMG_RSRC_WORD3                                                              0x23C7
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_X__MASK                                           0x00000007L //size: 3
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_Y__MASK                                           0x00000038L //size: 3
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_Z__MASK                                           0x000001c0L //size: 3
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_W__MASK                                           0x00000e00L //size: 3
#define GNM_SQ_IMG_RSRC_WORD3__BASE_LEVEL__MASK                                          0x0000f000L //size: 4
#define GNM_SQ_IMG_RSRC_WORD3__LAST_LEVEL__MASK                                          0x000f0000L //size: 4
#define GNM_SQ_IMG_RSRC_WORD3__TILING_INDEX__MASK                                        0x01f00000L //size: 5
#define GNM_SQ_IMG_RSRC_WORD3__POW2_PAD__MASK                                            0x02000000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD3__TYPE__MASK                                                0xf0000000L //size: 4
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_X__SHIFT                                           0
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_Y__SHIFT                                           3
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_Z__SHIFT                                           6
#define GNM_SQ_IMG_RSRC_WORD3__DST_SEL_W__SHIFT                                           9
#define GNM_SQ_IMG_RSRC_WORD3__BASE_LEVEL__SHIFT                                         12
#define GNM_SQ_IMG_RSRC_WORD3__LAST_LEVEL__SHIFT                                         16
#define GNM_SQ_IMG_RSRC_WORD3__TILING_INDEX__SHIFT                                       20
#define GNM_SQ_IMG_RSRC_WORD3__POW2_PAD__SHIFT                                           25
#define GNM_SQ_IMG_RSRC_WORD3__TYPE__SHIFT                                               28

#define mmSQ_IMG_RSRC_WORD4                                                              0x23C8
#define GNM_SQ_IMG_RSRC_WORD4__DEPTH__MASK                                               0x00001fffL //size:13
#define GNM_SQ_IMG_RSRC_WORD4__PITCH__MASK                                               0x07ffe000L //size:14
#define GNM_SQ_IMG_RSRC_WORD4__DEPTH__SHIFT                                               0
#define GNM_SQ_IMG_RSRC_WORD4__PITCH__SHIFT                                              13

#define mmSQ_IMG_RSRC_WORD5                                                              0x23C9
#define GNM_SQ_IMG_RSRC_WORD5__BASE_ARRAY__MASK                                          0x00001fffL //size:13
#define GNM_SQ_IMG_RSRC_WORD5__LAST_ARRAY__MASK                                          0x03ffe000L //size:13
#define GNM_SQ_IMG_RSRC_WORD5__BASE_ARRAY__SHIFT                                          0
#define GNM_SQ_IMG_RSRC_WORD5__LAST_ARRAY__SHIFT                                         13

#define mmSQ_IMG_RSRC_WORD6                                                              0x23CA
#define GNM_SQ_IMG_RSRC_WORD6__MIN_LOD_WARN__MASK                                        0x00000fffL //size:12
#define GNM_SQ_IMG_RSRC_WORD6__COUNTER_BANK_ID__MASK                                     0x000ff000L //size: 8
#define GNM_SQ_IMG_RSRC_WORD6__LOD_HDW_CNT_EN__MASK                                      0x00100000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD6__COMPRESSION_EN__MASK                                      0x00200000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD6__ALPHA_IS_ON_MSB__MASK                                     0x00400000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD6__COLOR_TRANSFORM__MASK                                     0x00800000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD6__ALT_TILE_MODE__MASK                                       0x01000000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD6__MIN_LOD_WARN__SHIFT                                        0
#define GNM_SQ_IMG_RSRC_WORD6__COUNTER_BANK_ID__SHIFT                                    12
#define GNM_SQ_IMG_RSRC_WORD6__LOD_HDW_CNT_EN__SHIFT                                     20
#define GNM_SQ_IMG_RSRC_WORD6__COMPRESSION_EN__SHIFT                                     21
#define GNM_SQ_IMG_RSRC_WORD6__ALPHA_IS_ON_MSB__SHIFT                                    22
#define GNM_SQ_IMG_RSRC_WORD6__COLOR_TRANSFORM__SHIFT                                    23
#define GNM_SQ_IMG_RSRC_WORD6__ALT_TILE_MODE__SHIFT                                      24

#define mmSQ_IMG_RSRC_WORD7                                                              0x23CB
#define GNM_SQ_IMG_RSRC_WORD7__META_DATA_ADDRESS__MASK                                   0xffffffffL //size:32
#define GNM_SQ_IMG_RSRC_WORD7__META_DATA_ADDRESS__SHIFT                                   0

#define mmSQ_IMG_SAMP_WORD0                                                              0x23CC
#define GNM_SQ_IMG_SAMP_WORD0__CLAMP_X__MASK                                             0x00000007L //size: 3
#define GNM_SQ_IMG_SAMP_WORD0__CLAMP_Y__MASK                                             0x00000038L //size: 3
#define GNM_SQ_IMG_SAMP_WORD0__CLAMP_Z__MASK                                             0x000001c0L //size: 3
#define GNM_SQ_IMG_SAMP_WORD0__MAX_ANISO_RATIO__MASK                                     0x00000e00L //size: 3
#define GNM_SQ_IMG_SAMP_WORD0__DEPTH_COMPARE_FUNC__MASK                                  0x00007000L //size: 3
#define GNM_SQ_IMG_SAMP_WORD0__FORCE_UNNORMALIZED__MASK                                  0x00008000L //size: 1
#define GNM_SQ_IMG_SAMP_WORD0__ANISO_THRESHOLD__MASK                                     0x00070000L //size: 3
#define GNM_SQ_IMG_SAMP_WORD0__FORCE_DEGAMMA__MASK                                       0x00100000L //size: 1
#define GNM_SQ_IMG_SAMP_WORD0__ANISO_BIAS__MASK                                          0x07e00000L //size: 6
#define GNM_SQ_IMG_SAMP_WORD0__TRUNC_COORD__MASK                                         0x08000000L //size: 1
#define GNM_SQ_IMG_SAMP_WORD0__DISABLE_CUBE_WRAP__MASK                                   0x10000000L //size: 1
#define GNM_SQ_IMG_SAMP_WORD0__FILTER_MODE__MASK                                         0x60000000L //size: 2
#define GNM_SQ_IMG_SAMP_WORD0__CLAMP_X__SHIFT                                             0
#define GNM_SQ_IMG_SAMP_WORD0__CLAMP_Y__SHIFT                                             3
#define GNM_SQ_IMG_SAMP_WORD0__CLAMP_Z__SHIFT                                             6
#define GNM_SQ_IMG_SAMP_WORD0__MAX_ANISO_RATIO__SHIFT                                     9
#define GNM_SQ_IMG_SAMP_WORD0__DEPTH_COMPARE_FUNC__SHIFT                                 12
#define GNM_SQ_IMG_SAMP_WORD0__FORCE_UNNORMALIZED__SHIFT                                 15
#define GNM_SQ_IMG_SAMP_WORD0__ANISO_THRESHOLD__SHIFT                                    16
#define GNM_SQ_IMG_SAMP_WORD0__FORCE_DEGAMMA__SHIFT                                      20
#define GNM_SQ_IMG_SAMP_WORD0__ANISO_BIAS__SHIFT                                         21
#define GNM_SQ_IMG_SAMP_WORD0__TRUNC_COORD__SHIFT                                        27
#define GNM_SQ_IMG_SAMP_WORD0__DISABLE_CUBE_WRAP__SHIFT                                  28
#define GNM_SQ_IMG_SAMP_WORD0__FILTER_MODE__SHIFT                                        29

#define mmSQ_IMG_SAMP_WORD1                                                              0x23CD
#define GNM_SQ_IMG_SAMP_WORD1__MIN_LOD__MASK                                             0x00000fffL //size:12
#define GNM_SQ_IMG_SAMP_WORD1__MAX_LOD__MASK                                             0x00fff000L //size:12
#define GNM_SQ_IMG_SAMP_WORD1__PERF_MIP__MASK                                            0x0f000000L //size: 4
#define GNM_SQ_IMG_SAMP_WORD1__PERF_Z__MASK                                              0xf0000000L //size: 4
#define GNM_SQ_IMG_SAMP_WORD1__MIN_LOD__SHIFT                                             0
#define GNM_SQ_IMG_SAMP_WORD1__MAX_LOD__SHIFT                                            12
#define GNM_SQ_IMG_SAMP_WORD1__PERF_MIP__SHIFT                                           24
#define GNM_SQ_IMG_SAMP_WORD1__PERF_Z__SHIFT                                             28

#define mmSQ_IMG_SAMP_WORD2                                                              0x23CE
#define GNM_SQ_IMG_SAMP_WORD2__LOD_BIAS__MASK                                            0x00003fffL //size:14
#define GNM_SQ_IMG_SAMP_WORD2__LOD_BIAS_SEC__MASK                                        0x000fc000L //size: 6
#define GNM_SQ_IMG_SAMP_WORD2__XY_MAG_FILTER__MASK                                       0x00300000L //size: 2
#define GNM_SQ_IMG_SAMP_WORD2__XY_MIN_FILTER__MASK                                       0x00c00000L //size: 2
#define GNM_SQ_IMG_SAMP_WORD2__Z_FILTER__MASK                                            0x03000000L //size: 2
#define GNM_SQ_IMG_SAMP_WORD2__MIP_FILTER__MASK                                          0x0c000000L //size: 2
#define GNM_SQ_IMG_SAMP_WORD2__LOD_BIAS__SHIFT                                            0
#define GNM_SQ_IMG_SAMP_WORD2__LOD_BIAS_SEC__SHIFT                                       14
#define GNM_SQ_IMG_SAMP_WORD2__XY_MAG_FILTER__SHIFT                                      20
#define GNM_SQ_IMG_SAMP_WORD2__XY_MIN_FILTER__SHIFT                                      22
#define GNM_SQ_IMG_SAMP_WORD2__Z_FILTER__SHIFT                                           24
#define GNM_SQ_IMG_SAMP_WORD2__MIP_FILTER__SHIFT                                         26

#define mmSQ_IMG_SAMP_WORD3                                                              0x23CF
#define GNM_SQ_IMG_SAMP_WORD3__BORDER_COLOR_PTR__MASK                                    0x00000fffL //size:12
#define GNM_SQ_IMG_SAMP_WORD3__BORDER_COLOR_TYPE__MASK                                   0xc0000000L //size: 2
#define GNM_SQ_IMG_SAMP_WORD3__BORDER_COLOR_PTR__SHIFT                                    0
#define GNM_SQ_IMG_SAMP_WORD3__BORDER_COLOR_TYPE__SHIFT                                  30

#define mmDB_DEPTH_VIEW                                                                  0xA002
#define GNM_DB_DEPTH_VIEW__SLICE_START__MASK                                             0x000007ffL //size:11
#define GNM_DB_DEPTH_VIEW__SLICE_MAX__MASK                                               0x00ffe000L //size:11
#define GNM_DB_DEPTH_VIEW__SLICE_START__SHIFT                                             0
#define GNM_DB_DEPTH_VIEW__SLICE_MAX__SHIFT                                              13

#define mmDB_HTILE_DATA_BASE                                                             0xA005
#define GNM_DB_HTILE_DATA_BASE__BASE_256B__MASK                                          0xffffffffL //size:32
#define GNM_DB_HTILE_DATA_BASE__BASE_256B__SHIFT                                          0

#define mmDB_DEPTH_INFO                                                                  0xA00F
#define GNM_DB_DEPTH_INFO__PIPE_CONFIG__MASK                                             0x00001f00L //size: 5
#define GNM_DB_DEPTH_INFO__PIPE_CONFIG__SHIFT                                             8
#define GNM_DB_DEPTH_INFO__ADDR5_SWIZZLE_MASK__MASK                                       0xf
#define GNM_DB_DEPTH_INFO__ADDR5_SWIZZLE_MASK__SHIFT                                      0x0
#define GNM_DB_DEPTH_INFO__ARRAY_MODE__MASK                                               0xf0
#define GNM_DB_DEPTH_INFO__ARRAY_MODE__SHIFT                                              0x4
#define GNM_DB_DEPTH_INFO__BANK_WIDTH__MASK                                               0x6000
#define GNM_DB_DEPTH_INFO__BANK_WIDTH__SHIFT                                              0xd
#define GNM_DB_DEPTH_INFO__BANK_HEIGHT__MASK                                              0x18000
#define GNM_DB_DEPTH_INFO__BANK_HEIGHT__SHIFT                                             0xf
#define GNM_DB_DEPTH_INFO__MACRO_TILE_ASPECT__MASK                                        0x60000
#define GNM_DB_DEPTH_INFO__MACRO_TILE_ASPECT__SHIFT                                       0x11
#define GNM_DB_DEPTH_INFO__NUM_BANKS__MASK                                                0x180000
#define GNM_DB_DEPTH_INFO__NUM_BANKS__SHIFT                                               0x13


#define mmDB_Z_INFO                                                                      0xA010
#define GNM_DB_Z_INFO__FORMAT__MASK                                                      0x00000003L //size: 2
#define GNM_DB_Z_INFO__NUM_SAMPLES__MASK                                                 0x0000000cL //size: 2
#define GNM_DB_Z_INFO__TILE_MODE_INDEX__MASK                                             0x00700000L //size: 3
#define GNM_DB_Z_INFO__ALLOW_EXPCLEAR__MASK                                              0x08000000L //size: 1
#define GNM_DB_Z_INFO__TILE_SURFACE_ENABLE__MASK                                         0x20000000L //size: 1
#define GNM_DB_Z_INFO__ZRANGE_PRECISION__MASK                                            0x80000000L //size: 1
#define GNM_DB_Z_INFO__FORMAT__SHIFT                                                      0
#define GNM_DB_Z_INFO__NUM_SAMPLES__SHIFT                                                 2
#define GNM_DB_Z_INFO__TILE_MODE_INDEX__SHIFT                                            20
#define GNM_DB_Z_INFO__ALLOW_EXPCLEAR__SHIFT                                             27
#define GNM_DB_Z_INFO__TILE_SURFACE_ENABLE__SHIFT                                        29
#define GNM_DB_Z_INFO__ZRANGE_PRECISION__SHIFT                                           31

#define mmDB_STENCIL_INFO                                                                0xA011
#define GNM_DB_STENCIL_INFO__FORMAT__MASK                                                0x00000001L //size: 1
#define GNM_DB_STENCIL_INFO__TILE_MODE_INDEX__MASK                                       0x00700000L //size: 3
#define GNM_DB_STENCIL_INFO__ALLOW_EXPCLEAR__MASK                                        0x08000000L //size: 1
#define GNM_DB_STENCIL_INFO__TILE_STENCIL_DISABLE__MASK                                  0x20000000L //size: 1
#define GNM_DB_STENCIL_INFO__FORMAT__SHIFT                                                0
#define GNM_DB_STENCIL_INFO__TILE_MODE_INDEX__SHIFT                                      20
#define GNM_DB_STENCIL_INFO__ALLOW_EXPCLEAR__SHIFT                                       27
#define GNM_DB_STENCIL_INFO__TILE_STENCIL_DISABLE__SHIFT                                 29

#define mmDB_Z_READ_BASE                                                                 0xA012
#define GNM_DB_Z_READ_BASE__BASE_256B__MASK                                              0xffffffffL //size:32
#define GNM_DB_Z_READ_BASE__BASE_256B__SHIFT                                              0

#define mmDB_STENCIL_READ_BASE                                                           0xA013
#define GNM_DB_STENCIL_READ_BASE__BASE_256B__MASK                                        0xffffffffL //size:32
#define GNM_DB_STENCIL_READ_BASE__BASE_256B__SHIFT                                        0

#define mmDB_Z_WRITE_BASE                                                                0xA014
#define GNM_DB_Z_WRITE_BASE__BASE_256B__MASK                                             0xffffffffL //size:32
#define GNM_DB_Z_WRITE_BASE__BASE_256B__SHIFT                                             0

#define mmDB_STENCIL_WRITE_BASE                                                          0xA015
#define GNM_DB_STENCIL_WRITE_BASE__BASE_256B__MASK                                       0xffffffffL //size:32
#define GNM_DB_STENCIL_WRITE_BASE__BASE_256B__SHIFT                                       0

#define mmDB_DEPTH_SIZE                                                                  0xA016
#define GNM_DB_DEPTH_SIZE__PITCH_TILE_MAX__MASK                                          0x000007ffL //size:11
#define GNM_DB_DEPTH_SIZE__HEIGHT_TILE_MAX__MASK                                         0x003ff800L //size:11
#define GNM_DB_DEPTH_SIZE__PITCH_TILE_MAX__SHIFT                                          0
#define GNM_DB_DEPTH_SIZE__HEIGHT_TILE_MAX__SHIFT                                        11

#define mmDB_DEPTH_SLICE                                                                 0xA017
#define GNM_DB_DEPTH_SLICE__SLICE_TILE_MAX__MASK                                         0x003fffffL //size:22
#define GNM_DB_DEPTH_SLICE__SLICE_TILE_MAX__SHIFT                                         0

#define mmDB_HTILE_SURFACE                                                               0xA2AF
#define GNM_DB_HTILE_SURFACE__LINEAR__MASK                                               0x00000001L //size: 1
#define GNM_DB_HTILE_SURFACE__TC_COMPATIBLE__MASK                                        0x00020000L //size: 1
#define GNM_DB_HTILE_SURFACE__LINEAR__SHIFT                                               0
#define GNM_DB_HTILE_SURFACE__TC_COMPATIBLE__SHIFT                                       17

#define mmCB_COLOR0_BASE                                                                 0xA318
#define GNM_CB_COLOR0_BASE__BASE_256B__MASK                                              0xffffffffL //size:32
#define GNM_CB_COLOR0_BASE__BASE_256B__SHIFT                                              0

#define mmCB_COLOR0_PITCH                                                                0xA319
#define GNM_CB_COLOR0_PITCH__TILE_MAX__MASK                                              0x000007ffL //size:11
#define GNM_CB_COLOR0_PITCH__FMASK_TILE_MAX__MASK                                        0x7ff00000L //size:11
#define GNM_CB_COLOR0_PITCH__TILE_MAX__SHIFT                                              0
#define GNM_CB_COLOR0_PITCH__FMASK_TILE_MAX__SHIFT                                       20

#define mmCB_COLOR0_SLICE                                                                0xA31A
#define GNM_CB_COLOR0_SLICE__TILE_MAX__MASK                                              0x003fffffL //size:22
#define GNM_CB_COLOR0_SLICE__TILE_MAX__SHIFT                                              0

#define mmCB_COLOR0_VIEW                                                                 0xA31B
#define GNM_CB_COLOR0_VIEW__SLICE_START__MASK                                            0x000007ffL //size:11
#define GNM_CB_COLOR0_VIEW__SLICE_MAX__MASK                                              0x00ffe000L //size:11
#define GNM_CB_COLOR0_VIEW__SLICE_START__SHIFT                                            0
#define GNM_CB_COLOR0_VIEW__SLICE_MAX__SHIFT                                             13

#define mmCB_COLOR0_INFO                                                                 0xA31C
#define GNM_CB_COLOR0_INFO__FAST_CLEAR__MASK                                             0x00002000L //size: 1
#define GNM_CB_COLOR0_INFO__COMPRESSION__MASK                                            0x00004000L //size: 1
#define GNM_CB_COLOR0_INFO__CMASK_IS_LINEAR__MASK                                        0x00080000L //size: 1
#define GNM_CB_COLOR0_INFO__FMASK_COMPRESSION_MODE__MASK                                 0x0C000000L //size: 2
#define GNM_CB_COLOR0_INFO__DCC_ENABLE__MASK                                             0x10000000L //size: 1
#define GNM_CB_COLOR0_INFO__CMASK_ADDR_TYPE__MASK                                        0x60000000L //size: 2
#define GNM_CB_COLOR0_INFO__ALT_TILE_MODE__MASK                                          0x80000000L //size: 1
#define GNM_CB_COLOR0_INFO__FAST_CLEAR__SHIFT                                            13
#define GNM_CB_COLOR0_INFO__COMPRESSION__SHIFT                                           14
#define GNM_CB_COLOR0_INFO__CMASK_IS_LINEAR__SHIFT                                       19
#define GNM_CB_COLOR0_INFO__FMASK_COMPRESSION_MODE__SHIFT                                26
#define GNM_CB_COLOR0_INFO__DCC_ENABLE__SHIFT                                            28
#define GNM_CB_COLOR0_INFO__CMASK_ADDR_TYPE__SHIFT                                       29
#define GNM_CB_COLOR0_INFO__ALT_TILE_MODE__SHIFT                                         31
#define GNM_CB_COLOR0_INFO__ENDIAN__MASK                                                 0x3
#define GNM_CB_COLOR0_INFO__ENDIAN__SHIFT                                                0x0
#define GNM_CB_COLOR0_INFO__FORMAT__MASK                                                 0x7c
#define GNM_CB_COLOR0_INFO__FORMAT__SHIFT                                                0x2
#define GNM_CB_COLOR0_INFO__LINEAR_GENERAL__MASK                                         0x80
#define GNM_CB_COLOR0_INFO__LINEAR_GENERAL__SHIFT                                        0x7
#define GNM_CB_COLOR0_INFO__NUMBER_TYPE__MASK                                            0x700
#define GNM_CB_COLOR0_INFO__NUMBER_TYPE__SHIFT                                           0x8
#define GNM_CB_COLOR0_INFO__COMP_SWAP__MASK                                              0x1800
#define GNM_CB_COLOR0_INFO__COMP_SWAP__SHIFT                                             0xb

#define mmCB_COLOR0_ATTRIB                                                               0xA31D
#define GNM_CB_COLOR0_ATTRIB__TILE_MODE_INDEX__MASK                                      0x0000001fL //size: 5
#define GNM_CB_COLOR0_ATTRIB__FMASK_TILE_MODE_INDEX__MASK                                0x000003e0L //size: 5
#define GNM_CB_COLOR0_ATTRIB__NUM_SAMPLES__MASK                                          0x00007000L //size: 3
#define GNM_CB_COLOR0_ATTRIB__NUM_FRAGMENTS__MASK                                        0x00018000L //size: 2
#define GNM_CB_COLOR0_ATTRIB__FORCE_DST_ALPHA_1__MASK                                    0x00020000L //size: 1
#define GNM_CB_COLOR0_ATTRIB__TILE_MODE_INDEX__SHIFT                                      0
#define GNM_CB_COLOR0_ATTRIB__FMASK_TILE_MODE_INDEX__SHIFT                                5
#define GNM_CB_COLOR0_ATTRIB__NUM_SAMPLES__SHIFT                                         12
#define GNM_CB_COLOR0_ATTRIB__NUM_FRAGMENTS__SHIFT                                       15
#define GNM_CB_COLOR0_ATTRIB__FORCE_DST_ALPHA_1__SHIFT                                   17


#define mmCB_COLOR0_DCC_CONTROL                                                          0xA31E
#define GNM_CB_COLOR0_DCC_CONTROL__OVERWRITE_COMBINER_DISABLE__MASK                      0x00000001L //size: 1
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_UNCOMPRESSED_BLOCK_SIZE__MASK                     0x0000000cL //size: 2
#define GNM_CB_COLOR0_DCC_CONTROL__MIN_COMPRESSED_BLOCK_SIZE__MASK                       0x00000010L //size: 1
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_COMPRESSED_BLOCK_SIZE__MASK                       0x00000060L //size: 2
#define GNM_CB_COLOR0_DCC_CONTROL__COLOR_TRANSFORM__MASK                                 0x00000180L //size: 2
#define GNM_CB_COLOR0_DCC_CONTROL__INDEPENDENT_64B_BLOCKS__MASK                          0x00000200L //size: 1
#define GNM_CB_COLOR0_DCC_CONTROL__OVERWRITE_COMBINER_DISABLE__SHIFT                      0
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_UNCOMPRESSED_BLOCK_SIZE__SHIFT                     2
#define GNM_CB_COLOR0_DCC_CONTROL__MIN_COMPRESSED_BLOCK_SIZE__SHIFT                       4
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_COMPRESSED_BLOCK_SIZE__SHIFT                       5
#define GNM_CB_COLOR0_DCC_CONTROL__COLOR_TRANSFORM__SHIFT                                 7
#define GNM_CB_COLOR0_DCC_CONTROL__INDEPENDENT_64B_BLOCKS__SHIFT                          9

#define mmCB_COLOR0_CMASK                                                                0xA31F
#define GNM_CB_COLOR0_CMASK__BASE_256B__MASK                                             0xffffffffL //size:32
#define GNM_CB_COLOR0_CMASK__BASE_256B__SHIFT                                             0

#define mmCB_COLOR0_CMASK_SLICE                                                          0xA320
#define GNM_CB_COLOR0_CMASK_SLICE__TILE_MAX__MASK                                        0x00003fffL //size:14
#define GNM_CB_COLOR0_CMASK_SLICE__TILE_MAX__SHIFT                                        0

#define mmCB_COLOR0_FMASK                                                                0xA321
#define GNM_CB_COLOR0_FMASK__BASE_256B__MASK                                             0xffffffffL //size:32
#define GNM_CB_COLOR0_FMASK__BASE_256B__SHIFT                                             0

#define mmCB_COLOR0_FMASK_SLICE                                                          0xA322
#define GNM_CB_COLOR0_FMASK_SLICE__TILE_MAX__MASK                                        0x003fffffL //size:22
#define GNM_CB_COLOR0_FMASK_SLICE__TILE_MAX__SHIFT                                        0

#define mmCB_COLOR0_CLEAR_WORD0                                                          0xA323
#define GNM_CB_COLOR0_CLEAR_WORD0__CLEAR_WORD0__MASK                                     0xffffffffL //size:32
#define GNM_CB_COLOR0_CLEAR_WORD0__CLEAR_WORD0__SHIFT                                     0

#define mmCB_COLOR0_CLEAR_WORD1                                                          0xA324
#define GNM_CB_COLOR0_CLEAR_WORD1__CLEAR_WORD1__MASK                                     0xffffffffL //size:32
#define GNM_CB_COLOR0_CLEAR_WORD1__CLEAR_WORD1__SHIFT                                     0

#define mmCB_COLOR0_DCC_BASE                                                             0xA325
#define GNM_CB_COLOR0_DCC_BASE__BASE_256B__MASK                                          0xffffffffL //size:32
#define GNM_CB_COLOR0_DCC_BASE__BASE_256B__SHIFT                                          0

// Private

#define mmSQ_IMG_RSRC_WORD6                                                              0x23CA
#define GNM_SQ_IMG_RSRC_WORD6__ALT_TILE_MODE__MASK                                       0x01000000L //size: 1
#define GNM_SQ_IMG_RSRC_WORD6__ALT_TILE_MODE__SHIFT                                      24

#define mmGB_TILE_MODE0                                                                  0x2644
#define GNM_GB_TILE_MODE0__ARRAY_MODE__MASK                                              0x0000003cL //size: 4
#define GNM_GB_TILE_MODE0__PIPE_CONFIG__MASK                                             0x000007c0L //size: 5
#define GNM_GB_TILE_MODE0__TILE_SPLIT__MASK                                              0x00003800L //size: 3
#define GNM_GB_TILE_MODE0__MICRO_TILE_MODE_NEW__MASK                                     0x01c00000L //size: 3
#define GNM_GB_TILE_MODE0__SAMPLE_SPLIT__MASK                                            0x06000000L //size: 2
#define GNM_GB_TILE_MODE0__ALT_PIPE_CONFIG__MASK                                         0xf8000000L //size: 5
#define GNM_GB_TILE_MODE0__ARRAY_MODE__SHIFT                                              2
#define GNM_GB_TILE_MODE0__PIPE_CONFIG__SHIFT                                             6
#define GNM_GB_TILE_MODE0__TILE_SPLIT__SHIFT                                             11
#define GNM_GB_TILE_MODE0__MICRO_TILE_MODE_NEW__SHIFT                                    22
#define GNM_GB_TILE_MODE0__SAMPLE_SPLIT__SHIFT                                           25
#define GNM_GB_TILE_MODE0__ALT_PIPE_CONFIG__SHIFT                                        27

#define mmGB_MACROTILE_MODE0                                                             0x2664
#define GNM_GB_MACROTILE_MODE0__BANK_WIDTH__MASK                                         0x00000003L //size: 2
#define GNM_GB_MACROTILE_MODE0__BANK_HEIGHT__MASK                                        0x0000000cL //size: 2
#define GNM_GB_MACROTILE_MODE0__MACRO_TILE_ASPECT__MASK                                  0x00000030L //size: 2
#define GNM_GB_MACROTILE_MODE0__NUM_BANKS__MASK                                          0x000000c0L //size: 2
#define GNM_GB_MACROTILE_MODE0__ALT_BANK_HEIGHT__MASK                                    0x00000300L //size: 2
#define GNM_GB_MACROTILE_MODE0__ALT_MACRO_TILE_ASPECT__MASK                              0x00000c00L //size: 2
#define GNM_GB_MACROTILE_MODE0__ALT_NUM_BANKS__MASK                                      0x00003000L //size: 2
#define GNM_GB_MACROTILE_MODE0__BANK_WIDTH__SHIFT                                         0
#define GNM_GB_MACROTILE_MODE0__BANK_HEIGHT__SHIFT                                        2
#define GNM_GB_MACROTILE_MODE0__MACRO_TILE_ASPECT__SHIFT                                  4
#define GNM_GB_MACROTILE_MODE0__NUM_BANKS__SHIFT                                          6
#define GNM_GB_MACROTILE_MODE0__ALT_BANK_HEIGHT__SHIFT                                    8
#define GNM_GB_MACROTILE_MODE0__ALT_MACRO_TILE_ASPECT__SHIFT                             10
#define GNM_GB_MACROTILE_MODE0__ALT_NUM_BANKS__SHIFT                                     12

#define mmDB_DEPTH_INFO                                                                  0xA00F
#define GNM_DB_DEPTH_INFO__PIPE_CONFIG__MASK                                             0x00001f00L //size: 5
#define GNM_DB_DEPTH_INFO__PIPE_CONFIG__SHIFT                                             8

#define mmCB_COLOR0_INFO                                                                 0xA31C
#define GNM_CB_COLOR0_INFO__ALT_TILE_MODE__MASK                                          0x80000000L //size: 1
#define GNM_CB_COLOR0_INFO__ALT_TILE_MODE__SHIFT                                         31

#define mmCB_COLOR0_DCC_CONTROL                                                          0xA31E
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_COMPRESSED_BLOCK_SIZE__MASK                       0x00000060L //size: 2
#define GNM_CB_COLOR0_DCC_CONTROL__INDEPENDENT_64B_BLOCKS__MASK                          0x00000200L //size: 1
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_COMPRESSED_BLOCK_SIZE__SHIFT                       5
#define GNM_CB_COLOR0_DCC_CONTROL__INDEPENDENT_64B_BLOCKS__SHIFT                          9

