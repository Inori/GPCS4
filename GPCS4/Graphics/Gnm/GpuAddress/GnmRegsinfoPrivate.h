#pragma once

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
#define GNM_CB_COLOR0_INFO__FAST_CLEAR__MASK              0x00002000L  //size: 1
#define GNM_CB_COLOR0_INFO__COMPRESSION__MASK             0x00004000L  //size: 1
#define GNM_CB_COLOR0_INFO__CMASK_IS_LINEAR__MASK         0x00080000L  //size: 1
#define GNM_CB_COLOR0_INFO__FMASK_COMPRESSION_MODE__MASK  0x0C000000L  //size: 2
#define GNM_CB_COLOR0_INFO__DCC_ENABLE__MASK              0x10000000L  //size: 1
#define GNM_CB_COLOR0_INFO__CMASK_ADDR_TYPE__MASK         0x60000000L  //size: 2
#define GNM_CB_COLOR0_INFO__ALT_TILE_MODE__MASK           0x80000000L  //size: 1
#define GNM_CB_COLOR0_INFO__FAST_CLEAR__SHIFT             13
#define GNM_CB_COLOR0_INFO__COMPRESSION__SHIFT            14
#define GNM_CB_COLOR0_INFO__CMASK_IS_LINEAR__SHIFT        19
#define GNM_CB_COLOR0_INFO__FMASK_COMPRESSION_MODE__SHIFT 26
#define GNM_CB_COLOR0_INFO__DCC_ENABLE__SHIFT             28
#define GNM_CB_COLOR0_INFO__CMASK_ADDR_TYPE__SHIFT        29
#define GNM_CB_COLOR0_INFO__ALT_TILE_MODE__SHIFT          31

#define mmCB_COLOR0_DCC_CONTROL                                                          0xA31E
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_COMPRESSED_BLOCK_SIZE__MASK                       0x00000060L //size: 2
#define GNM_CB_COLOR0_DCC_CONTROL__INDEPENDENT_64B_BLOCKS__MASK                          0x00000200L //size: 1
#define GNM_CB_COLOR0_DCC_CONTROL__MAX_COMPRESSED_BLOCK_SIZE__SHIFT                       5
#define GNM_CB_COLOR0_DCC_CONTROL__INDEPENDENT_64B_BLOCKS__SHIFT                          9

