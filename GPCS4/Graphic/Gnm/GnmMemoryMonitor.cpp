#include "GnmMemoryMonitor.h"


GnmMemoryMonitor::GnmMemoryMonitor(const GnmMemoryCallback& callback):
	m_callback(callback)
{
	 install();
}

GnmMemoryMonitor::~GnmMemoryMonitor()
{
}

void GnmMemoryMonitor::traceMemory(GnmMemoryRange& block)
{
	m_blockRecords.insert(block);
}

void GnmMemoryMonitor::untraceMemory(GnmMemoryRange& block)
{
	m_blockRecords.erase(block);
}

void GnmMemoryMonitor::install()
{
	UtilException::ExceptionHandler handler;
	handler.callback = GnmMemoryMonitor::exceptionCallbackStatic;
	handler.param    = this;
	UtilException::addExceptionHandler(handler);
}

UtilException::ExceptionAction GnmMemoryMonitor::exceptionCallback(UtilException::ExceptionRecord* record)
{
}

UtilException::ExceptionAction GnmMemoryMonitor::exceptionCallbackStatic(UtilException::ExceptionRecord* record, void* param)
{
	GnmMemoryMonitor* pthis = reinterpret_cast<GnmMemoryMonitor*>(param);
	return pthis->exceptionCallback(record);
}