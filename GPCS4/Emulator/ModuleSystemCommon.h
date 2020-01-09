#pragma once

// Module system configs

// Ignore not found modules while loading dependencies.
#define MODSYS_IGNORE_NOT_FOUND_MODULES

// Only use debug stub on unknown functions.
// Unknown function is a function can not be found in either builtin modules nor native modules.
#define MODSYS_USE_STUB_ON_UNKNOWN_ONLY

// Disable LOG_DEBUG macro in ElfMapper.cpp
// This is used to prevent too many logs while loading ELF.
#define MODSYS_DISABLE_MAPPER_LOG_DEBUG
