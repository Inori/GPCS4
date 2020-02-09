#pragma once

typedef unsigned int SceKernelUseconds;

struct SceKernelSemaOptParam
{
	size_t	size;
};

typedef void* SceKernelSema;



#define SCE_KERNEL_CPUMODE_6CPU          0
#define SCE_KERNEL_CPUMODE_7CPU_LOW      1
#define SCE_KERNEL_CPUMODE_7CPU_NORMAL   5


