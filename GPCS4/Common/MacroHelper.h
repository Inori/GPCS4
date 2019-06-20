#pragma once

#define ALIGN_DOWN(x, a) ((((x) / (a)) + (((x) % (a)) ? 1 : 0)) * (a))
