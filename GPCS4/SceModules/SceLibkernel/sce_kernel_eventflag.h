#pragma once


#define SCE_KERNEL_EVF_ATTR_TH_FIFO   0x01
#define SCE_KERNEL_EVF_ATTR_TH_PRIO   0x02
#define SCE_KERNEL_EVF_ATTR_SINGLE    0x10
#define SCE_KERNEL_EVF_ATTR_MULTI     0x20

#define SCE_KERNEL_EVF_WAITMODE_AND        0x01
#define SCE_KERNEL_EVF_WAITMODE_OR         0x02
#define SCE_KERNEL_EVF_WAITMODE_CLEAR_ALL  0x10
#define SCE_KERNEL_EVF_WAITMODE_CLEAR_PAT  0x20


typedef struct _SceKernelEventFlagOptParam {
	size_t size;
} SceKernelEventFlagOptParam;

typedef void* SceKernelEventFlag;

#define SCE_KERNEL_EVF_ID_INVALID     ((SceKernelEventFlag)-1)
