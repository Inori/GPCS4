#pragma once

// Module system configs

// Ignore not found modules while loading dependencies.
#define MODSYS_IGNORE_NOT_FOUND_MODULES

// Force using debug stub on all module functions
#define MODSYS_FORCE_USING_STUB_FUNCTION

// Disable LOG_DEBUG macro in ElfMapper.cpp
// This is used to prevent too many logs while loading ELF.
#define MODSYS_DISABLE_MAPPER_LOG_DEBUG
