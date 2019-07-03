#pragma once

typedef unsigned int        SceKernelUseconds;

typedef struct _SceKernelSemaOptParam {
	size_t	size;
} SceKernelSemaOptParam;


typedef void* SceKernelSema;


typedef	long		__scesuseconds_t;	/* microseconds (signed) */
typedef	unsigned int	__sceuseconds_t;	/* microseconds (unsigned) */

typedef	__sceuseconds_t	sceuseconds_t;	/* microseconds (unsigned) */
