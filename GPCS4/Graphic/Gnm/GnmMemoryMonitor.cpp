#include "GnmMemoryMonitor.h"

#include "UtilMemory.h"

GnmMemoryMonitor::GnmMemoryMonitor(const GnmMemoryCallback& callback):
	m_callback(callback)
{
	 install();
}

GnmMemoryMonitor::~GnmMemoryMonitor()
{
}

void GnmMemoryMonitor::traceMemory(GnmResourceMemory& block)
{
	m_blockRecords.insert(block);
	monitor(block);
}

void GnmMemoryMonitor::untraceMemory(GnmResourceMemory& block)
{
	neglect(block);
	m_blockRecords.erase(block);
}

void GnmMemoryMonitor::install()
{
	UtilException::ExceptionHandler handler;
	handler.callback = GnmMemoryMonitor::exceptionCallbackStatic;
	handler.param    = this;
	UtilException::addExceptionHandler(handler);
}

void GnmMemoryMonitor::monitor(GnmResourceMemory& block)
{
	GnmMemoryRange resRange     = block.range();
	GnmMemoryRange monitorRange = getMonitorRange(resRange);

	UtilMemory::VM_PROTECT_FLAG flag;

	GnmMemoryProtect prot = block.protect();
	switch (prot)
	{
	case GnmMemoryProtect::GpuReadOnly:
		// If the memory won't be updated on GPU, 
		// then we only care about CPU write.
		flag = UtilMemory::VMPF_CPU_READ;
		break;
	case GnmMemoryProtect::GpuReadWrite:
		// If the memory maybe wrote on GPU, 
		// then we need to flush the memory upon CPU read.
		flag = UtilMemory::VMPF_NOACCESS;
		break;
	}

	UtilMemory::VMProtect(monitorRange.start, monitorRange.size, flag);
}

void GnmMemoryMonitor::neglect(GnmResourceMemory& block)
{

}

GnmMemoryRange GnmMemoryMonitor::getMonitorRange(GnmMemoryRange& block)
{
	// Calculate memory range in unit of pages 
	// containing the block in question.

	GnmMemoryRange result = {};
	uintptr_t      alignedStart = util::alignUp(
		reinterpret_cast<uintptr_t>(block.start), 
		UtilException::GnmPageSize);
	result.start                = reinterpret_cast<void*>(alignedStart);
	result.size                 = util::align(block.size, UtilException::GnmPageSize);

	return result;
}

UtilException::ExceptionAction GnmMemoryMonitor::exceptionCallback(UtilException::ExceptionRecord* record)
{
}

UtilException::ExceptionAction GnmMemoryMonitor::exceptionCallbackStatic(UtilException::ExceptionRecord* record, void* param)
{
	GnmMemoryMonitor* pthis = reinterpret_cast<GnmMemoryMonitor*>(param);
	return pthis->exceptionCallback(record);
}