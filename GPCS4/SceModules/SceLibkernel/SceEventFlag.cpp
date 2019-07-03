#include "SceEventFlag.h"
#include "sce_kernel_error.h"


CSceEventFlag::CSceEventFlag(const std::string& name, uint attr, uint64 initPattern) :
	m_name(name),
	m_attr(attr),
	m_bitPattern(initPattern)
{

}

CSceEventFlag::~CSceEventFlag()
{

}

int CSceEventFlag::Set(uint64 bitPattern)
{

}

int CSceEventFlag::Wait(uint64 bitPattern, uint mode, uint64* pResultPat, SceKernelUseconds* pTimeout)
{

}

// TODO:
// implement this
int CSceEventFlag::Poll(uint64 bitPattern, uint mode, uint64* pResultPat)
{
	return SCE_KERNEL_ERROR_ESRCH;
}

// TODO:
// implement this
int CSceEventFlag::Clear(uint64 bitPattern)
{
	return SCE_KERNEL_ERROR_ESRCH;
}

// TODO:
// implement this
int CSceEventFlag::Cancel(uint64 setPattern, int* pNumWaitThreads)
{
	return SCE_KERNEL_ERROR_ESRCH;
}
