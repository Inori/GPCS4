#include "memory_monitor_impl.h"

MemoryMonitorImpl::MemoryMonitorImpl(MonitorFlags flags, MemoryCallback* callback) :
	m_flags(flags),
	m_callback(callback)
{
}

MemoryMonitorImpl::~MemoryMonitorImpl()
{
}

void MemoryMonitorImpl::OnMemoryRead(void* address, size_t size)
{
	m_callback->OnMemoryRead(address, size);
}

void MemoryMonitorImpl::OnMemoryWrite(void* address, size_t size)
{
	m_callback->OnMemoryWrite(address, size);
}

