#include "SceEventFlag.h"
#include "sce_errors.h"


CSceEventFlag::CSceEventFlag(const std::string& name, uint attr, uint64 initPattern) :
	m_name(name),
	m_attr(attr),
	m_bitPattern(initPattern)
{

}

CSceEventFlag::~CSceEventFlag()
{
	std::lock_guard lock(m_mutex);
}

int CSceEventFlag::Set(uint64 bitPattern)
{
	std::lock_guard lock(m_mutex);
	m_bitPattern |= bitPattern;
	return SCE_OK;
}

int CSceEventFlag::Wait(uint64 bitPattern, uint mode, uint64* pResultPat, SceKernelUseconds* pTimeout)
{

}

// TODO:
// implement this
int CSceEventFlag::Poll(uint64 bitPattern, uint mode, uint64* pResultPat)
{
	throw std::logic_error("not implemented");
	return SCE_KERNEL_ERROR_ESRCH;
}

// TODO:
// implement this
int CSceEventFlag::Clear(uint64 bitPattern)
{
	throw std::logic_error("not implemented");
	return SCE_KERNEL_ERROR_ESRCH;
}

// TODO:
// implement this
int CSceEventFlag::Cancel(uint64 setPattern, int* pNumWaitThreads)
{
	throw std::logic_error("not implemented");
	return SCE_KERNEL_ERROR_ESRCH;
}
