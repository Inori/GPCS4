#pragma once

/*
 * Error number definition
 */

/**
 * @j 不正な引数によって操作は失敗しました。 @ej
 * @e The operation failed due to an invalid argument. @ee
 */
#define SCE_GPU_ADDRESS_ERROR_STATUS_INVALID_ARGUMENT -2133655552	/* 0x80D30000 */

/**
 * @j タイリングエラーによって操作は失敗しました。 @ej
 * @e The operation failed due to an internal tiling error. @ee
 */
#define SCE_GPU_ADDRESS_ERROR_STATUS_INTERNAL_TILING -2133655551	/* 0x80D30001 */


