#pragma once

#include "GPCS4Config.h"
#include "Common/GPCS4Types.h"
#include "Common/GPCS4Log.h"
#include "Common/MacroHelper.h"
#include "Common/ReferenceCount.h"


// Prevent Intellisense throwing errors due to not understanding "__attribute__" (clang-specific directive)
#if IS_INTELLISENSE
#define PS4API
#define PS4NORETURN
#define PS4NAKED
#define PS4NOINLINE
#define PS4DEPRECATED
#define PS4UNUSED
#define PS4ALIGN
#else
#define PS4API __attribute__((sysv_abi))
#define PS4NORETURN __attribute__((noreturn))
#define PS4NAKED __attribute__((naked))
#define PS4NOINLINE __attribute__((noinline))
#define PS4DEPRECATED __attribute__((deprecated))
#define PS4UNUSED __attribute__((unused))
#define PS4ALIGN(n) __attribute__((__aligned__(n)))
#endif
