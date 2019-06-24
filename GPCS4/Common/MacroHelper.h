#pragma once

#define IS_ALIGNED(x, a) (!((x)%(a)))

#define ALIGN_DOWN(x, a) ((((x) / (a)) + (((x) % (a)) ? 1 : 0)) * (a))


