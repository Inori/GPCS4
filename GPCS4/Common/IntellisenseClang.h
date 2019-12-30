#pragma once

// Prevent IntelliSense from throwing errors 
// due to not understanding clang-specific directives, like "__attribute__"

// "__INTELLISENSE__" macro works for me using VS2017,
// but may not work in other situations, use `__clang__` macro as a workaround

#if defined(__INTELLISENSE__) || !defined(__clang__)

#define __attribute__(x) 

#endif  // __INTELLISENSE__

