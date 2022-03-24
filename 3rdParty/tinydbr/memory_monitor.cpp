#include "memory_monitor.h"
#include "arch/x86/x86_memory_monitor_impl.h"

MemoryCallback::~MemoryCallback()
{
}

//////////////////////////////////////////////////////////////////////////

MemoryMonitor::MemoryMonitor(MonitorFlags flags, MemoryCallback* callback)
{
	m_impl = std::make_unique<X86MemoryMonitorImpl>(flags, callback);
}

MemoryMonitor::~MemoryMonitor()
{
}

void MemoryMonitor::Init(const std::vector<TargetModule>& target_modules, const Options& options)
{
	m_impl->Init(target_modules, options);
}

void MemoryMonitor::Unit()
{
	m_impl->Unit();
}
