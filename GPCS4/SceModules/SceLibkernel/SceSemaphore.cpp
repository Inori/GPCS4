#include "SceSemaphore.h"
#include "sce_errors.h"
#include <chrono>

CSceSemaphore::CSceSemaphore(const std::string& name, int initCount, int maxCount, uint32_t mode):
	m_name(name),
	m_count(initCount),
	m_maxCount(maxCount)
{

}

CSceSemaphore::~CSceSemaphore()
{
	std::lock_guard lock(m_mutex);
}

int CSceSemaphore::Signal(int count)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	std::lock_guard lock(m_mutex);
	do 
	{
		if (m_count + count > m_maxCount)
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		m_count += count;
		m_cond.notify_one();

		err = SCE_OK;
	} while (false);
	return err;
}

int CSceSemaphore::Poll(int count)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	std::lock_guard lock(m_mutex);
	do 
	{
		if (count < 1 || count > m_maxCount)
		{
			err = SCE_KERNEL_ERROR_EBUSY;
			break;
		}

		m_count -= count;

		err = SCE_OK;
	} while (false);
	return err;
}

int CSceSemaphore::Wait(int count, uint32_t* pTimeOut)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	std::unique_lock<std::mutex> lock(m_mutex);
	do 
	{
		if (count < 1 || count > m_maxCount)
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		auto pred = [this, count] { return m_count >= count; };
		if (!pTimeOut)  // infinite
		{
			m_cond.wait(lock, pred);
			m_count -= count;
		}
		else
		{
			auto timeMs = std::chrono::microseconds(*pTimeOut);
			auto start = std::chrono::high_resolution_clock::now();

			bool notExpired = m_cond.wait_for(lock, timeMs, pred);

			auto end = std::chrono::high_resolution_clock::now();
			if (notExpired)
			{
				auto dura = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				auto timeLeft = timeMs - dura;
				*pTimeOut = timeLeft.count();
				m_count -= count;
			}
			else
			{
				*pTimeOut = 0;
				err = SCE_KERNEL_ERROR_ETIMEDOUT;
				break;
			}

		}

		err = SCE_OK;
	} while (false);
	return err;
}

// TODO:
// implement Cancel
int CSceSemaphore::Cancel(int setCount, uint32_t* pNumWaitThreads)
{
	return SCE_KERNEL_ERROR_ESRCH;
}
