#include "memory_monitor.h"

MemoryCallback::~MemoryCallback()
{
}


MemoryMonitor::MemoryMonitor(MonitorFlags flags, MemoryCallback* callback) :
	m_flags(flags),
	m_callback(callback)
{
}

MemoryMonitor::~MemoryMonitor()
{
}

void MemoryMonitor::OnMemoryRead(void* address, size_t size)
{
	m_callback->OnMemoryRead(address, size);
}

void MemoryMonitor::OnMemoryWrite(void* address, size_t size)
{
	m_callback->OnMemoryWrite(address, size);
}

