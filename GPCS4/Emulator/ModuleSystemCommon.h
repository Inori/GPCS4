#pragma once

// Module system configs

// Ignore not found modules while loading dependencies.
#define MODSYS_IGNORE_NOT_FOUND_MODULES

// Disable LOG_DEBUG macro in ElfMapper.cpp
// This is used to prevent too many logs while loading ELF.
// #define MODSYS_DISABLE_MAPPER_LOG_DEBUG

// Output NIDs of functions that are not implemented in HLE.
#define MODSYS_OUTPUT_NOT_IMPLEMENTED_HLE

// Functions can be separated into three types: builtin, native and unknown
// builtin : Functions which GPCS4 implemented
// native  : Functions in loaded modules
// unknown : Functions which can not be found in either builtin nor native, typically nullptr, means an error occur.

// Disable debug stub on all types of functions
// #define MODSYS_STUB_DISABLE

// Use debug stub on native
// #define MODSYS_STUB_ON_NATIVE

// Use debug stub on unknown
#define MODSYS_STUB_ON_UNKNOWN


