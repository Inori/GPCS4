#pragma once

#include "tinydbr.h"
#include "structure.h"
#include "memory_monitor.h"

class MemoryMonitorImpl : public TinyDBR
{

public:
	MemoryMonitorImpl(MonitorFlags flags, MemoryCallback* callback);
	virtual ~MemoryMonitorImpl();

protected:
	// These functions will be called from generated assemble code.

	// memory read
	// this will be called before the read instruction
	void OnMemoryRead(void* address, size_t size);

	// memory write
	// this will be called after the write instruction
	void OnMemoryWrite(void* address, size_t size);

protected:
	MonitorFlags m_flags = 0;
	MemoryCallback* m_callback;
};


