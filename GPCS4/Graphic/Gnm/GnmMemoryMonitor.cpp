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
	GnmMemoryRange monitorRange = getMonitorRange(block.range());
	UtilMemory::VMProtect(monitorRange.start, monitorRange.size, UtilMemory::VMPF_READ_WRITE);
}

GnmMemoryRange GnmMemoryMonitor::getMonitorRange(const GnmMemoryRange& block)
{
	// Calculate memory range in unit of pages 
	// containing the block in question.

	GnmMemoryRange result       = {};
	uintptr_t      alignedStart = util::alignUp(
        reinterpret_cast<size_t>(block.start),
        UtilException::GnmPageSize);
	result.start = reinterpret_cast<void*>(alignedStart);

	size_t extendedSize = reinterpret_cast<size_t>(block.start) - alignedStart + block.size;
	result.size         = util::align(extendedSize, UtilException::GnmPageSize);

	return result;
}

void GnmMemoryMonitor::pendSingleStep(UtilException::ExceptionRecord* record)
{
}

UtilException::ExceptionAction GnmMemoryMonitor::onMemoryAccess(UtilException::ExceptionRecord* record)
{
	do 
	{
		void* address = reinterpret_cast<void*>(record->info.virtualAddress);

		auto iter = m_blockRecords.find(address);
		if (iter != m_blockRecords.end())
		{
			auto& block  = iter->get();

			// We already know the memory is accessed and it's access type,
			// so we don't need to monitor the memory anymore.
			neglect(block);

			auto  access = record->info.access;
			if (access == UtilException::EXCEPTION_READ)
			{
				m_callback.read(block);
			}
			else
			{
				m_callback.write(block);
			}
		}
		else
		{
			// If the accessed memory is not gpu memory,
			// (typically the pad memory below page boundary)
			// we need to single step the access instruction,
			// until there's no exception or we catch a gpu memory access.
			pendSingleStep(record);
		}
	} while (false);
}

UtilException::ExceptionAction GnmMemoryMonitor::onSingleStep(UtilException::ExceptionRecord* record)
{
}

UtilException::ExceptionAction GnmMemoryMonitor::exceptionCallback(UtilException::ExceptionRecord* record)
{
	UtilException::ExceptionAction action = UtilException::ExceptionAction::CONTINUE_SEARCH;
	do 
	{
		if (!record)
		{
			break;
		}

		auto code = record->code;
		if (code == UtilException::EXCEPTION_ACCESS_VIOLATION)
		{
			action = onMemoryAccess(record);
		}
		else if (code == UtilException::EXCEPTION_SINGLE_STEP)
		{
			action = onSingleStep(record);
		}
		else
		{
			break;
		}

	} while (false);
	return action;
}

UtilException::ExceptionAction GnmMemoryMonitor::exceptionCallbackStatic(UtilException::ExceptionRecord* record, void* param)
{
	GnmMemoryMonitor* pthis = reinterpret_cast<GnmMemoryMonitor*>(param);
	return pthis->exceptionCallback(record);
}