#pragma once

#include <cstdint>
#include <memory>

#include "structure.h"
#include "memory_callback.h"

class MemoryMonitorImpl;

enum MonitorFlag : uint64_t
{
	IgnoreCode        = 1 << 0,  // e.g. call [0x1234], jmp [0x1234]
	IgnoreStack       = 1 << 1,  // e.g. mov rax, [rsp + 0x8]
	IgnoreRipRelative = 1 << 2,  // e.g. mov rax, [rip + 0x8]

	// save processor extended states, e.g. xmm, ymm, fpu, mxcsr
	// before call into user callback.
	SaveExtendedState = 1 << 3,  // this is a very costive operation which will slow down the program severely
};

typedef uint64_t MonitorFlags;



class MemoryMonitor
{
public:
	MemoryMonitor(MonitorFlags flags, MemoryCallback* callback);
	~MemoryMonitor();

	void Init(const std::vector<TargetModule>& target_modules,
			  const Options&                   options);

	void Unit();

private:
	
	std::unique_ptr<MemoryMonitorImpl> m_impl;
};

