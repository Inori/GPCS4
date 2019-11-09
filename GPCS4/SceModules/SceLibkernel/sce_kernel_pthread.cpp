//#include "sce_libkernel.h"
#include "sce_pthread_common.h"
//#include "pthreads4w/pthread.h"
#include "MapSlot.h"

int pthreadErrorToSceError(int perror);

int PS4API scek_pthread_cond_init(pthread_cond_t *cond,
								  const pthread_condattr_t *attr)
{
	//LOG_FIXME("Not implemented");
	auto rc = pthread_cond_init(cond, attr);
	return rc;
}

int PS4API scek_pthread_cond_destroy(pthread_cond_t *cond)
{
	//LOG_FIXME("Not implemented");
	auto rc = pthread_cond_destroy(cond);
	return rc;
}


int PS4API scek_pthread_cond_signal(pthread_cond_t *cond)
{
	//LOG_FIXME("Not implemented");
	auto rc = pthread_cond_signal(cond);
	return rc;
	//return SCE_OK;
}


int PS4API scek_pthread_cond_timedwait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
	//auto rc = pthread_cond_timedwait()
	//return SCE_OK;
}


int PS4API scek_pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
	LOG_FIXME("Not implemented");
	auto rc = pthread_cond_wait(cond, mutex);
	return rc;
	//return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////
int PS4API scek_pthread_create(ScePthread *thread, const pthread_attr_t *attr,  void *(PS4API *entry) (void *), void *args)
{
	//LOG_FIXME("Not implemented");
	int rc        = 0;

	do
	{
		auto param = new SCE_THREAD_PARAM{};
		param->entry = reinterpret_cast<void*>(entry);
		param->arg    = args;

		pthread_t pth = {};

		rc = pthread_create(&pth, attr, newThreadWrapper, param);
		if (rc != 0)
		{
			delete param;	
			break;
		}

		ScePthread tid = g_threadSlot.GetEmptySlotIndex();
		g_threadSlot[tid] = pth;
		*thread = tid;
	} while (false);

	return pthreadErrorToSceError(rc);
}


int PS4API scek_pthread_equal(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_join(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////
int PS4API scek_pthread_mutex_init(pthread_mutex_t *mutex,
									const pthread_mutexattr_t *attr)
{
	//LOG_FIXME("Not implemented");
	auto ret =  pthread_mutex_init(mutex, attr);
	return ret;
}


int PS4API scek_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	//LOG_FIXME("Not implemented");
	auto ret = pthread_mutex_destroy(mutex);
	return ret;
}


int PS4API scek_pthread_mutex_lock(pthread_mutex_t* mtx)
{
	LOG_SCE_TRACE("mtx %p", mtx);
	int err = pthread_mutex_lock(mtx);
	return pthreadErrorToSceError(err);
}


int PS4API scek_pthread_mutex_unlock(pthread_mutex_t* mtx)
{
	LOG_SCE_TRACE("mtx %p", mtx);
	int err = pthread_mutex_unlock(mtx);
	return pthreadErrorToSceError(err);
}


int PS4API scek_pthread_mutexattr_destroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_mutexattr_init(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_mutexattr_settype(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_self(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_setschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}
