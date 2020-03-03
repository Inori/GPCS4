#pragma once

//
#define SCE_PHYSICAL_PAGE_SIZE	4096

// logical page is commonly used in ps4 dev
// it consists of four physical pages 
#define SCE_LOGICAL_PAGE_SIZE	(SCE_PHYSICAL_PAGE_SIZE * 4)



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


// memory map flags
#define SCE_KERNEL_MAP_FIXED		0x0010
#define SCE_KERNEL_MAP_NO_OVERWRITE	0x0080
#define SCE_KERNEL_MAP_DMEM_COMPAT	0x0400
#define SCE_KERNEL_MAP_NO_COALESCE	0x400000


// direct memory size
// TODO:
// this should be decreased as direct memory being alloced
// but currently we use fixed size
// 
// always 0x180000000 bytes
#define  SCE_KERNEL_MAIN_DMEM_SIZE 0x180000000u


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
	void	 *start;
	void	 *end;
	sce_off_t	  offset;
	int	  protection;
	int	  memoryType;
	unsigned  isFlexibleMemory : 1;
	unsigned  isDirectMemory : 1;
	unsigned  isStack : 1;
	unsigned  isPooledMemory : 1;
	unsigned  isCommitted : 1;
	char      name[SCE_KERNEL_VIRTUAL_RANGE_NAME_SIZE];
} SceKernelVirtualQueryInfo;