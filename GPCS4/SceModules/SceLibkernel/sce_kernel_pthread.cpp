//#include "sce_libkernel.h"
#include "sce_pthread_common.h"
//#include "pthreads4w/pthread.h"
#include "MapSlot.h"

LOG_CHANNEL(SceModules.SceLibkernel.pthread);

int sceMutexAttrTypeToPthreadType(int type);

int PS4API scek_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
	LOG_SCE_TRACE("cond %p attr %p", cond, attr);
	auto rc = pthread_cond_init(cond, attr);
	return rc;
}

int PS4API scek_pthread_cond_destroy(pthread_cond_t *cond)
{
	LOG_SCE_TRACE("cond %p", cond);
	auto rc = pthread_cond_destroy(cond);
	return rc;
}


int PS4API scek_pthread_cond_signal(pthread_cond_t *cond)
{
	LOG_SCE_TRACE("cond %p", cond);
	auto rc = pthread_cond_signal(cond);
	return rc;
}


int PS4API scek_pthread_cond_timedwait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
	LOG_SCE_TRACE("cond %p mutex %p", cond, mutex);
	auto rc = pthread_cond_wait(cond, mutex);
	return rc;
}

//////////////////////////////////////////////////////////////////////////
int PS4API scek_pthread_create(ScePthread *thread, const pthread_attr_t *attr,  void *(PS4API *entry) (void *), void *args)
{
	//LOG_FIXME("Not implemented");
	int rc = 0;

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

	return rc;
}


int PS4API scek_pthread_equal(ScePthread thread1, ScePthread thread2)
{
	LOG_SCE_TRACE("thread1 = %zu, thread2 = %zu", thread1, thread2);
	return thread1 == thread2;
}


int PS4API scek_pthread_join(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////
int PS4API scek_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	LOG_SCE_TRACE("mutex %p attr %p", mutex, attr);
	return scePthreadMutexInit(mutex, attr, nullptr);
}


int PS4API scek_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	LOG_SCE_TRACE("mutex %p", mutex);
	auto ret = pthread_mutex_destroy(mutex);
	return ret;
}


int PS4API scek_pthread_mutex_lock(pthread_mutex_t* mtx)
{
	LOG_SCE_TRACE("mtx %p", mtx);
	int err = pthread_mutex_lock(mtx);
	return err;
}


int PS4API scek_pthread_mutex_trylock(pthread_mutex_t *mtx) 
{
	LOG_SCE_TRACE("mtx %p", mtx);
	int err = pthread_mutex_trylock(mtx);
	return err;
}


int PS4API scek_pthread_mutex_unlock(pthread_mutex_t* mtx)
{
	LOG_SCE_TRACE("mtx %p", mtx);
	int err = pthread_mutex_unlock(mtx);
	return err;
}


int PS4API scek_pthread_mutexattr_destroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_mutexattr_init(pthread_mutexattr_t * attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_mutexattr_init(attr);
	return err;
}

int PS4API scek_pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type)
{
	LOG_SCE_TRACE("attr %p type %d", attr, type);
	int ptype = sceMutexAttrTypeToPthreadType(type);
	int err = pthread_mutexattr_settype((pthread_mutexattr_t*)attr, ptype);
	return err;
}

// For PS4 system, ScePthread and pthread_t are same.
ScePthread PS4API scek_pthread_self(void)
{
	LOG_SCE_TRACE("");
	return scePthreadSelf();
}

void PS4API scek_pthread_exit(void *value_ptr)
{
	LOG_SCE_TRACE("");
	scePthreadExit(value_ptr);
}


int PS4API scek_pthread_setschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API scek_pthread_setspecific(pthread_key_t key, const void *value)
{
	LOG_SCE_DUMMY_IMPL();
	LOG_DEBUG("key %p value %p", key, value);
	//int err = pthread_setspecific(key, value);
	return SCE_OK;
}

void * PS4API scek_pthread_getspecific(pthread_key_t key) 
{
	LOG_SCE_TRACE("key %p", key);
	return pthread_getspecific(key);
}


int PS4API scek_pthread_key_create(pthread_key_t *key, void(*destructor)(void*))
{
	LOG_SCE_DUMMY_IMPL();
	LOG_DEBUG("key %p destructor address %p", key, destructor);
	//int err = pthread_key_create(key, destructor);
	return SCE_OK;
}
