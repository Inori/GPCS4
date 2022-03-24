#include "memory_monitor.h"
#include "arch/x86/x86_memory_monitor_impl.h"

//////////////////////////////////////////////////////////////////////////

MemoryMonitor::MemoryMonitor(MonitorFlags flags, MemoryCallback* callback)
{
#if defined(__x86_64__) || defined(_M_X64)
	m_impl = std::make_unique<X86MemoryMonitorImpl>(flags, callback);
#else
	// TODO:
#endif
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
