#pragma once

#include "GPCS4Config.h"

#define PS4API __attribute__((sysv_abi))

#define PS4NORETURN __attribute__((noreturn))

#define PS4NAKED __attribute__((naked))

#define PS4NOINLINE __attribute__((noinline))

#define PS4DEPRECATED __attribute__((deprecated))

#define PS4UNUSED __attribute__((unused))

#define PS4ALIGN(n) __attribute__((__aligned__(n)))

#ifdef GPCS4_WINDOWS
#define PS4EXPORT __declspec(dllexport)
#else   // GPCS4_WINDOWS
#define PS4EXPORT __attribute__((dllexport))
#endif  // GPCS4_WINDOWS
