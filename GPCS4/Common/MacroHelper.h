#pragma once

#define IS_ALIGNED(x, a) (!((x)%(a)))

#define ALIGN_DOWN(x, a) ((((x) / (a)) + (((x) % (a)) ? 1 : 0)) * (a))

#define ALIGN_ROUND(size, align) (((size) + (align) - 1) & ~((align) - 1))

#define BIT_IS_SET(x, n) ((x) & (1 << (n)))

#define BUILD_QWORD(high, low) (((unsigned long long)(high) << 32) | (low))


