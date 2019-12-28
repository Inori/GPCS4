#pragma once

typedef unsigned int        SceKernelUseconds;

typedef struct _SceKernelSemaOptParam {
	size_t	size;
} SceKernelSemaOptParam;

// TODO find out field names
typedef struct _SubProcParam
{
	uint64_t size{ 0x40 };
	uint64_t p1{};
	uint64_t p2{};
	uint64_t p3{};
	uint64_t p4{};
	uint64_t p5{};
	uint64_t p6{};
	uint64_t p7{};
	uint64_t p8{};
	uint64_t p9{};
	uint64_t p10{};
	uint64_t p11{};
	uint64_t* p12{ nullptr };
} SubProcParam;

typedef struct _ProcParam
{
	uint64_t size{ 0x40 };
	uint64_t p1{};
	uint64_t p2{};
	uint64_t p3{};
	uint64_t p4{};
	uint64_t p5{};
	uint64_t p6{};
	SubProcParam* subProcParam{};
} ProcParam;

typedef void* SceKernelSema;



#define SCE_KERNEL_CPUMODE_6CPU          0
#define SCE_KERNEL_CPUMODE_7CPU_LOW      1
#define SCE_KERNEL_CPUMODE_7CPU_NORMAL   5


