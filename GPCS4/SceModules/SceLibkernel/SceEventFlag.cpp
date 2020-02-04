#include "SceEventFlag.h"
#include "sce_errors.h"
#include "sce_kernel_eventflag.h"
#include "Platform/UtilThread.h"
#include <functional>

#include <functional>

LOG_CHANNEL(SceModules.SceLibkernel.SceEventFlag);

CSceEventFlag::CSceEventFlag(const std::string& name, uint32_t attr, uint64_t initPattern) :
	m_attr(attr),
	m_name(name),
	m_bitPattern(initPattern),
	m_anyWaiting(false)
{

}

CSceEventFlag::~CSceEventFlag()
{
	std::lock_guard lock(m_mutex);
}

int CSceEventFlag::Set(uint64_t bitPattern)
{
	std::lock_guard lock(m_mutex);
	m_bitPattern |= bitPattern;
	if (m_attr & SCE_KERNEL_EVF_ATTR_SINGLE)
	{
		m_cond.notify_one();
	}
	else
	{
		m_cond.notify_all();
	}
	return SCE_OK;
}

int CSceEventFlag::Wait(uint64_t bitPattern, uint32_t mode, uint64_t* pResultPat, SceKernelUseconds* pTimeout)
{
	int err = SCE_KERNEL_ERROR_ESRCH;
	bool condMet = false;
	std::unique_lock<std::mutex> lock(m_mutex);
	do 
	{
		if (IsSingleMode())
		{
			if (m_anyWaiting)
			{
				err = SCE_KERNEL_ERROR_EPERM;
				break;
			}
			else
			{
				m_anyWaiting = true;
			}
		}

		if (IsConditionMet(mode, bitPattern))
		{
			err = SCE_OK;
			condMet = true;
			break;
		}

		std::function<bool (void)> pred;
		if (mode & SCE_KERNEL_EVF_WAITMODE_AND)
		{
			pred = [this, bitPattern] { return (m_bitPattern & bitPattern) == bitPattern; };
		}
		else if (mode & SCE_KERNEL_EVF_WAITMODE_OR)
		{
			pred = [this, bitPattern] { return (m_bitPattern & bitPattern) != 0; };
		}
		else
		{
			LOG_ERR("invalid mode %x", mode);
		}

	
		if (!pTimeout)
		{
			m_cond.wait(lock, pred);
			condMet = true;
		}
		else
		{
			auto timeMs = std::chrono::microseconds(*pTimeout);
			auto start = std::chrono::high_resolution_clock::now();

			bool notExpired = m_cond.wait_for(lock, timeMs, pred);

			auto end = std::chrono::high_resolution_clock::now();
			if (notExpired)
			{
				auto dura = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				auto timeLeft = timeMs - dura;
				*pTimeout = timeLeft.count();
				condMet = true;
			}
			else
			{
				*pTimeout = 0;
				err = SCE_KERNEL_ERROR_ETIMEDOUT;
				condMet = false;
				break;
			}
		}
		//LOG_DEBUG("thread awake %d", UtilThread::GetThreadId());

		err = SCE_OK;
	} while (false);

	if (IsSingleMode() && (err != SCE_KERNEL_ERROR_EPERM))
	{
		m_anyWaiting = false;
	}

	if (pResultPat)
	{
		*pResultPat = m_bitPattern;
	}

	if (condMet)
	{
		if (mode & SCE_KERNEL_EVF_WAITMODE_CLEAR_ALL)
		{
			m_bitPattern = 0;
		}
		else if (mode & SCE_KERNEL_EVF_WAITMODE_CLEAR_PAT)
		{
			m_bitPattern &= ~bitPattern;
		}
	}

	return err;
}

// TODO:
// implement this
int CSceEventFlag::Poll(uint64_t bitPattern, uint32_t mode, uint64_t* pResultPat)
{
	//throw std::logic_error("not implemented");
	return SCE_KERNEL_ERROR_ESRCH;
}

// TODO:
// implement this
int CSceEventFlag::Clear(uint64_t bitPattern)
{
	throw std::logic_error("not implemented");
	return SCE_KERNEL_ERROR_ESRCH;
}

// TODO:
// implement this
int CSceEventFlag::Cancel(uint64_t setPattern, int* pNumWaitThreads)
{
	throw std::logic_error("not implemented");
	return SCE_KERNEL_ERROR_ESRCH;
}

bool CSceEventFlag::IsConditionMet(uint32_t mode, uint64_t bitPattern)
{
	bool bMet = false;
	if (mode & SCE_KERNEL_EVF_WAITMODE_AND)
	{
		bMet = ((m_bitPattern & bitPattern) == m_bitPattern);
	}
	else if (mode & SCE_KERNEL_EVF_WAITMODE_OR)
	{
		bMet = ((m_bitPattern & bitPattern) != 0);
	}
	else
	{
		LOG_ERR("invalid mode %x", mode);
	}
	return bMet;
}

bool CSceEventFlag::IsSingleMode()
{
	return m_attr & SCE_KERNEL_EVF_ATTR_SINGLE;
}
