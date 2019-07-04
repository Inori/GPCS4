#include "sce_libkernel.h"
#include "pthreads4w/pthread.h"

int pthreadErrorToSceError(int perror);

int PS4API scek_pthread_cond_init(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API scek_pthread_cond_destroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_cond_signal(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_cond_timedwait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_cond_wait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////

int PS4API scek_pthread_create(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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
int PS4API scek_pthread_mutex_init(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_mutex_destroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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
