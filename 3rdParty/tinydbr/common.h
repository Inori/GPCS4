/*
Copyright 2020 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

https ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <list>

#include "structure.h"



enum {
  /* 00 */ FAULT_NONE,
  /* 01 */ FAULT_TMOUT,
  /* 02 */ FAULT_CRASH,
  /* 03 */ FAULT_ERROR,
  /* 04 */ FAULT_NOINST,
  /* 05 */ FAULT_NOBITS
};

#ifdef _DEBUG
#define DBG_BREAK __debugbreak()
#else
#define DBG_BREAK
#endif

#define SAY(...)    printf(__VA_ARGS__)

#define WARN(...) do { \
    SAY("[!] WARNING: " __VA_ARGS__); \
    SAY("\n"); \
  } while (0)

#define FATAL(...) do { \
    SAY("[-] PROGRAM ABORT : " __VA_ARGS__); \
    SAY("         Location : %s(), %s:%u\n\n", \
         __FUNCTION__, __FILE__, __LINE__); \
	DBG_BREAK; \
    exit(1); \
  } while (0)

#define USAGE_CHECK(condition, message) if(!(condition)) FATAL("%s\n", message);


// gets time in milliseconds
uint64_t GetCurTime(void);

uint32_t GetCurTid(void);

bool PauseResumeThreadList(uint32_t dwOwnerPID, bool bResumeThread);

void* GetModuleEntrypoint(void* base_address);

std::string UnicodeToAnsi(const std::wstring& wstr, unsigned int code_page);


#endif
