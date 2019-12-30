#pragma once

typedef unsigned int        SceKernelUseconds;

typedef struct _SceKernelSemaOptParam 
{
	size_t	size;
} SceKernelSemaOptParam;

typedef void* SceKernelSema;



#define SCE_KERNEL_CPUMODE_6CPU          0
#define SCE_KERNEL_CPUMODE_7CPU_LOW      1
#define SCE_KERNEL_CPUMODE_7CPU_NORMAL   5


