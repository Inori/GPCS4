#pragma once

#define IS_ALIGNED(x, a) (!((x)%(a)))

#define ALIGN_DOWN(x, a) ((((x) / (a)) + (((x) % (a)) ? 1 : 0)) * (a))

#define ALIGN_ROUND(size, align) (((size) + (align) - 1) & ~((align) - 1))

#define BIT_IS_SET(x, n) ((x) & (1 << (n)))

#define BUILD_QWORD(high, low) (((unsigned long long)(high) << 32) | (low))

// TODO:
// Some addresses in some gnm structures are GPU relative,
// I'm not sure they are relative to what,
// maybe relative to the garlic memory base address.
#define GNM_GPU_ABS_ADDR(refAddr, relaAddr) ( (void*)(((uint64_t)(refAddr) & 0x0000FF0000000000) | uint64_t(relaAddr)) )


