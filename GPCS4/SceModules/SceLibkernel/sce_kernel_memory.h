#pragma once

//
#define SCE_PHYSICAL_PAGE_SIZE	4096


// memory map protections
#define	SCE_KERNEL_PROT_CPU_READ  0x01
#define	SCE_KERNEL_PROT_CPU_RW	  0x02
#define	SCE_KERNEL_PROT_CPU_WRITE	SCE_KERNEL_PROT_CPU_RW
#define	SCE_KERNEL_PROT_CPU_EXEC  0x04
#define	SCE_KERNEL_PROT_CPU_ALL	  0x07
#define SCE_KERNEL_PROT_GPU_READ  0x10
#define SCE_KERNEL_PROT_GPU_WRITE 0x20
#define SCE_KERNEL_PROT_GPU_RW	  0x30
#define SCE_KERNEL_PROT_GPU_ALL	  0x30

#define SCE_KERNEL_PAGE_SIZE	  16384
#define SCE_LOGICAL_PAGE_SIZE	  SCE_KERNEL_PAGE_SIZE


// memory map flags
#define SCE_KERNEL_MAP_FIXED		0x0010
#define SCE_KERNEL_MAP_NO_OVERWRITE	0x0080
#define SCE_KERNEL_MAP_DMEM_COMPAT	0x0400
#define SCE_KERNEL_MAP_NO_COALESCE	0x400000


// Total physical memory size on chip
// The application program can use a total of 5056 MiB(5568 MiB in NEO mode) physical memory
#define SCE_KERNEL_PHYSICAL_MEMORY_SIZE (5568 * 1024 * 1024ULL)
// If there is no specification, 448 MiB will be assigned as flexible memory
#define SCE_KERNEL_FLEXIBLE_MEMORY_SIZE (448 * 1024 * 1024ULL)
// direct memory size
#define SCE_KERNEL_MAIN_DMEM_SIZE \
	(SCE_KERNEL_PHYSICAL_MEMORY_SIZE - SCE_KERNEL_FLEXIBLE_MEMORY_SIZE)



// the memory address space which the target game process can use
// must be within 
// SCE_KERNEL_APP_MAP_AREA_START_ADDR and SCE_KERNEL_APP_MAP_AREA_END_ADDR
// or
// SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR and SCE_KERNEL_SYS_MANAGE_AREA_END_ADDR

// system managed area
#define SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR 0x400000ULL
#define SCE_KERNEL_SYS_MANAGE_AREA_END_ADDR   0x7ffffc000ULL
#define SCE_KERNEL_SYS_MANAGE_AREA_SIZE \
	(SCE_KERNEL_SYS_MANAGE_AREA_END_ADDR - SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR)

// user area
#define SCE_KERNEL_APP_MAP_AREA_START_ADDR 0x1000000000ULL
#define SCE_KERNEL_APP_MAP_AREA_END_ADDR   0xfc00000000ULL
#define SCE_KERNEL_APP_MAP_AREA_SIZE \
	(SCE_KERNEL_APP_MAP_AREA_END_ADDR - SCE_KERNEL_APP_MAP_AREA_START_ADDR)


#define SCE_KERNEL_VIRTUAL_ADDRESS_END_ADDR 0xffffffffffULL



// memory types
typedef enum 
{
	SCE_KERNEL_WB_ONION = 0,
	SCE_KERNEL_WC_GARLIC = 3,
	SCE_KERNEL_WB_GARLIC = 10,
	SCE_KERNEL_MEMORY_TYPE_END = 11
} SceKernelMemoryType;



#define SCE_KERNEL_VIRTUAL_RANGE_NAME_SIZE	32

typedef struct
{
	void*     start;
	void*     end;
	sce_off_t offset;
	int       protection;
	int       memoryType;
	unsigned  isFlexibleMemory : 1;
	unsigned  isDirectMemory : 1;
	unsigned  isStack : 1;
	unsigned  isPooledMemory : 1;
	unsigned  isCommitted : 1;
	char      name[SCE_KERNEL_VIRTUAL_RANGE_NAME_SIZE];
} SceKernelVirtualQueryInfo;