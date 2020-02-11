#include "sce_libkernel.h"
#include "sce_pthread_common.h"
#include "sce_kernel_scepthread.h"
#include "MapSlot.h"

#include "Platform/PlatformUtils.h"
#include "Emulator/TLSHandler.h"

#include <utility>

LOG_CHANNEL(SceModules.SceLibkernel.scepthread);

//////////////////////////////////////////////////////////////////////////

int pthreadErrorToSceError(int perror)
{
	int sceError = SCE_KERNEL_ERROR_UNKNOWN;

	switch (perror)
	{
	case 0:
		sceError = SCE_OK;
		break;
	case EPERM:
		sceError = SCE_KERNEL_ERROR_EPERM;
		break;
	case ENOENT:
		sceError = SCE_KERNEL_ERROR_ENOENT;
		break;
	case ESRCH:
		sceError = SCE_KERNEL_ERROR_ESRCH;
		break;
	case EINTR:
		sceError = SCE_KERNEL_ERROR_EINTR;
		break;
	case EIO:
		sceError = SCE_KERNEL_ERROR_EIO;
		break;
	case ENXIO:
		sceError = SCE_KERNEL_ERROR_ENXIO;
		break;
	case E2BIG:
		sceError = SCE_KERNEL_ERROR_E2BIG;
		break;
	case ENOEXEC:
		sceError = SCE_KERNEL_ERROR_ENOEXEC;
		break;
	case EBADF:
		sceError = SCE_KERNEL_ERROR_EBADF;
		break;
	case ECHILD:
		sceError = SCE_KERNEL_ERROR_ECHILD;
		break;
	case EAGAIN:
		sceError = SCE_KERNEL_ERROR_EAGAIN;
		break;
	case ENOMEM:
		sceError = SCE_KERNEL_ERROR_ENOMEM;
		break;
	case EACCES:
		sceError = SCE_KERNEL_ERROR_EACCES;
		break;
	case EFAULT:
		sceError = SCE_KERNEL_ERROR_EFAULT;
		break;
	case EBUSY:
		sceError = SCE_KERNEL_ERROR_EBUSY;
		break;
	case EEXIST:
		sceError = SCE_KERNEL_ERROR_EEXIST;
		break;
	case EXDEV:
		sceError = SCE_KERNEL_ERROR_EXDEV;
		break;
	case ENODEV:
		sceError = SCE_KERNEL_ERROR_ENODEV;
		break;
	case ENOTDIR:
		sceError = SCE_KERNEL_ERROR_ENOTDIR;
		break;
	case EISDIR:
		sceError = SCE_KERNEL_ERROR_EISDIR;
		break;
	case EINVAL:
		sceError = SCE_KERNEL_ERROR_EINVAL;
		break;
	case ENFILE:
		sceError = SCE_KERNEL_ERROR_ENFILE;
		break;
	case EMFILE:
		sceError = SCE_KERNEL_ERROR_EMFILE;
		break;
	case ENOTTY:
		sceError = SCE_KERNEL_ERROR_ENOTTY;
		break;
	case EFBIG:
		sceError = SCE_KERNEL_ERROR_EFBIG;
		break;
	case ENOSPC:
		sceError = SCE_KERNEL_ERROR_ENOSPC;
		break;
	case ESPIPE:
		sceError = SCE_KERNEL_ERROR_ESPIPE;
		break;
	case EROFS:
		sceError = SCE_KERNEL_ERROR_EROFS;
		break;
	case EMLINK:
		sceError = SCE_KERNEL_ERROR_EMLINK;
		break;
	case EPIPE:
		sceError = SCE_KERNEL_ERROR_EPIPE;
		break;
	case EDOM:
		sceError = SCE_KERNEL_ERROR_EDOM;
		break;
	case EDEADLK:
		sceError = SCE_KERNEL_ERROR_EDEADLK;
		break;
	case ENAMETOOLONG:
		sceError = SCE_KERNEL_ERROR_ENAMETOOLONG;
		break;
	case ENOLCK:
		sceError = SCE_KERNEL_ERROR_ENOLCK;
		break;
	case ENOSYS:
		sceError = SCE_KERNEL_ERROR_ENOSYS;
		break;
	case ENOTEMPTY:
		sceError = SCE_KERNEL_ERROR_ENOTEMPTY;
		break;
	default:
	{
		LOG_WARN("not supported pthread error %X", perror);
		sceError = perror;
	}
		break;
	}

	return sceError;
}


int PS4API scePthreadMutexInit(ScePthreadMutex *mutex, const ScePthreadMutexattr *attr, const char *name)
{
	LOG_SCE_TRACE("mutex %p attr %p name %s", mutex, attr, name);
	// TODO:
	// pthread mutex do not have names
	// if this is necessary, we could add a name field
	// in pthread_mutex_t of pthread4w
	if (name)
	{
		LOG_FIXME("set name is not supported yet.")
	}
	int err = 0;
	if (attr == nullptr)
	{
		// If attr is nullptr then default will be used which is PTHREAD_MUTEX_ERRORCHECK on PS4
		// so we make sure we set PTHREAD_MUTEX_ERRORCHECK here to match behaviour.
		pthread_mutexattr_t errorCheckMutexAttr;
		pthread_mutexattr_init(&errorCheckMutexAttr);
		pthread_mutexattr_settype(&errorCheckMutexAttr, PTHREAD_MUTEX_ERRORCHECK);
		err = pthread_mutex_init((pthread_mutex_t*)mutex, &errorCheckMutexAttr);
		pthread_mutexattr_destroy(&errorCheckMutexAttr);
	}
	else
	{
		err = pthread_mutex_init((pthread_mutex_t*)mutex, (pthread_mutexattr_t*)attr);
	}
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexDestroy(ScePthreadMutex *mutex)
{
	LOG_SCE_TRACE("mutex %p", mutex);
	int err = pthread_mutex_destroy((pthread_mutex_t*)mutex);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexLock(ScePthreadMutex *mutex)
{
	LOG_SCE_TRACE("mutex %p", mutex);
	int err = pthread_mutex_lock((pthread_mutex_t*)mutex);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexUnlock(ScePthreadMutex *mutex)
{
	LOG_SCE_TRACE("mutex %p", mutex);
	int err = pthread_mutex_unlock((pthread_mutex_t*)mutex);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexattrInit(ScePthreadMutexattr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_mutexattr_init((pthread_mutexattr_t*)attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexattrDestroy(ScePthreadMutexattr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_mutexattr_destroy((pthread_mutexattr_t*)attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexattrSetprotocol(ScePthreadMutexattr *attr, int protocol)
{
	LOG_SCE_TRACE("attr %p prot %d", attr, protocol);
	// TODO:
	// pthread4w do not support this call
	// but this seems doesn't matter
	// winpthread from MinGW64 do not actually implement this either
	LOG_FIXME("mutex attr set protocal not implenmet.");
	return SCE_OK;
}

int sceMutexAttrTypeToPthreadType(int sceType)
{
	int pthreadType = -1;
	switch (sceType)
	{
	case SCE_PTHREAD_MUTEX_ERRORCHECK:
		pthreadType = PTHREAD_MUTEX_ERRORCHECK;
		break;
	case SCE_PTHREAD_MUTEX_RECURSIVE:
		pthreadType = PTHREAD_MUTEX_RECURSIVE;
		break;
	case SCE_PTHREAD_MUTEX_NORMAL:
		pthreadType = PTHREAD_MUTEX_NORMAL;
		break;
	default:
		LOG_ERR("not supported mutex attr type %d", sceType);
		break;
	}
	return pthreadType;
}

int PS4API scePthreadMutexattrSettype(ScePthreadMutexattr *attr, int type)
{
	LOG_SCE_TRACE("attr %p type %d", attr, type);
	int ptype = sceMutexAttrTypeToPthreadType(type);
	int err = pthread_mutexattr_settype((pthread_mutexattr_t*)attr, ptype);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadMutexGetprioceiling(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadMutexTimedlock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadMutexTrylock(ScePthreadMutex *mutex)
{
	LOG_SCE_TRACE("mutex %p", mutex);
	int err = pthread_mutex_trylock((pthread_mutex_t*)mutex);
	return pthreadErrorToSceError(err);
}

//////////////////////////////////////////////////////////////////////////

int PS4API scePthreadAttrInit(ScePthreadAttr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_attr_init(attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadAttrDestroy(ScePthreadAttr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_attr_destroy(attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadAttrGetschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API scePthreadAttrSetaffinity(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

inline int sceDetachStateToPthreadState(int oldState)
{
	int newState = -1;
	switch (oldState)
	{
	case SCE_PTHREAD_CREATE_DETACHED:
		newState = PTHREAD_CREATE_DETACHED;
		break;
	case SCE_PTHREAD_CREATE_JOINABLE:
		newState = PTHREAD_CREATE_JOINABLE;
		break;
	default:
		LOG_ERR("unknown state %d", oldState);
		break;
	}
	return newState;
}


int PS4API scePthreadAttrSetdetachstate(ScePthreadAttr *attr, int state)
{
	LOG_SCE_TRACE("attr %p state %x", attr, state);
	int pthreadState = sceDetachStateToPthreadState(state);
	int err = pthread_attr_setdetachstate(attr, pthreadState);
	return pthreadErrorToSceError(err);
}

inline int sceAttrInheritSchedToPthreadInheritSched(int oldIS)
{
	int newIS = -1;
	switch (oldIS)
	{
	case SCE_PTHREAD_EXPLICIT_SCHED:
		newIS = PTHREAD_EXPLICIT_SCHED;
		break;
	case SCE_PTHREAD_INHERIT_SCHED:
		newIS = PTHREAD_INHERIT_SCHED;
		break;
	default:
		LOG_ERR("unknown inheritSched %d", oldIS);
		break;
	}
	return newIS;
}

int PS4API scePthreadAttrSetinheritsched(ScePthreadAttr *attr, int inheritSched)
{
	LOG_SCE_TRACE("attr %p inheritSched %d", attr, inheritSched);
	int pthreadIS = sceAttrInheritSchedToPthreadInheritSched(inheritSched);
	int err = pthread_attr_setinheritsched(attr, pthreadIS);
	return pthreadErrorToSceError(err);
}

#ifdef GPCS4_WINDOWS
// we need to map
// 256 -> 700 > 767
// to
// 15 -> 0 -> -15
#define THREAD_PRIORITY_IDLE			(-15)
#define THREAD_PRIORITY_NORMAL			0
#define THREAD_PRIORITY_TIME_CRITICAL	15

int scePriorityToPthreadPriority(int oldPriority)
{
	int newPriority = THREAD_PRIORITY_NORMAL;
	if (SCE_KERNEL_PRIO_FIFO_HIGHEST <= oldPriority && SCE_KERNEL_PRIO_FIFO_DEFAULT > oldPriority)
	{
		newPriority = THREAD_PRIORITY_TIME_CRITICAL -
			((float)(oldPriority - SCE_KERNEL_PRIO_FIFO_HIGHEST) / (float)(SCE_KERNEL_PRIO_FIFO_DEFAULT - SCE_KERNEL_PRIO_FIFO_HIGHEST)) *
			(float)THREAD_PRIORITY_TIME_CRITICAL;
	}
	else if (SCE_KERNEL_PRIO_FIFO_DEFAULT <= oldPriority && SCE_KERNEL_PRIO_FIFO_LOWEST >= oldPriority)
	{
		newPriority = THREAD_PRIORITY_IDLE *
			(float)(oldPriority - SCE_KERNEL_PRIO_FIFO_DEFAULT) / (float)(SCE_KERNEL_PRIO_FIFO_LOWEST - SCE_KERNEL_PRIO_FIFO_DEFAULT);
	}
	else
	{
		LOG_ERR("error sce thread priority %d", oldPriority);
	}
	return newPriority;
}

#endif

int PS4API scePthreadAttrSetschedparam(ScePthreadAttr *attr, const SceKernelSchedParam *param)
{
	LOG_SCE_TRACE("attr %p priority %d", attr, param->sched_priority);
	sched_param pthreadParam;
	pthreadParam.sched_priority = scePriorityToPthreadPriority(param->sched_priority);
	int err = pthread_attr_setschedparam(attr, &pthreadParam);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadAttrSetschedpolicy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadAttrSetstacksize(ScePthreadAttr *attr, size_t stackSize)
{
	LOG_SCE_TRACE("attr %p stackSize %d", attr, stackSize);
	int err = pthread_attr_setstacksize(attr, stackSize);
	return pthreadErrorToSceError(err);
}

//////////////////////////////////////////////////////////////////////////
ScePthread PS4API scePthreadSelf(void)
{
	pthread_t pt = pthread_self();
	ScePthread tid = g_threadSlot.GetItemIndex(pt);
	if (tid == 0)
	{
		tid = g_threadSlot.GetEmptySlotIndex();
		g_threadSlot[tid] = pt;
	}
	LOG_SCE_TRACE("thread self %d", tid);
	return tid;
}


int PS4API scePthreadSetaffinity(ScePthread thread, const SceKernelCpumask mask)
{
	LOG_SCE_TRACE("mask %x", mask);
	cpu_set_t cpuset;
	// TODO:
	// should limit cpu count according to running machine
	for (int i = 0; i != SCE_KERNEL_CPU_MAX; ++i)
	{
		if (util::isBitSet(mask, i))
		{
			CPU_SET(i, &cpuset);
		}
	}

	int err = pthread_setaffinity_np(g_threadSlot[thread], sizeof(cpu_set_t), &cpuset);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondInit(ScePthreadCond *cond, const ScePthreadCondattr *attr, const char *name)
{
	LOG_SCE_TRACE("cond %p attr %p name %s", cond, attr, name);
	if (name)
	{
		LOG_FIXME("cond name is not supported yet");
	}
	int err = pthread_cond_init(cond, attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondDestroy(ScePthreadCond *cond)
{
	LOG_SCE_TRACE("cond %p", cond);
	int err = pthread_cond_destroy(cond);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondBroadcast(ScePthreadCond *cond)
{
	LOG_SCE_TRACE("cond %p", cond);
	int err = pthread_cond_broadcast(cond);
	return pthreadErrorToSceError(err);
}

int PS4API scePthreadCondSignal(ScePthreadCond *cond)
{
	LOG_SCE_TRACE("cond %p", cond);
	int err = pthread_cond_signal(cond);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondWait(ScePthreadCond *cond, ScePthreadMutex *mutex)
{
	LOG_SCE_TRACE("cond %p mutex %p", cond, mutex);
	int err = pthread_cond_wait(cond, mutex);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondattrInit(ScePthreadCondattr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_condattr_init(attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondattrDestroy(ScePthreadCondattr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_condattr_destroy(attr);
	return pthreadErrorToSceError(err);
}

//////////////////////////////////////////////////////////////////////////


// TODO:
// this implementation is very wrong.
// if one thread create several thread, these threads' id will be same
int PS4API scePthreadCreate(ScePthread *thread, const ScePthreadAttr *attr, void *(PS4API *entry) (void *), void *arg, const char *name)
{
	LOG_SCE_TRACE("thread %p attr %p entry %p arg %p name %s", thread, attr, entry, arg,"placeholder" /*name*/);
	if (name)
	{
		LOG_FIXME("thread name is not supported.");
	}

	SCE_THREAD_PARAM* param = new SCE_THREAD_PARAM();
	param->entry = (void*)entry;
	param->arg = arg;

	pthread_t pthread;
	int err = pthread_create(&pthread, attr, newThreadWrapper, param);

	ScePthread tid = g_threadSlot.GetEmptySlotIndex();
	g_threadSlot[tid] = pthread;
	*thread = tid;

	return pthreadErrorToSceError(err);
}


void PS4API scePthreadExit(void *value_ptr)
{
	LOG_SCE_TRACE("value %p", value_ptr);

	// do clear
	pthread_t emptyPt = { 0 };
	ScePthread tid = scePthreadSelf();
	g_threadSlot.SetItemAt(tid, emptyPt);

	// TODO:
	// I formerly compiled pthread4w using VCE(using C++ exceptions) option,
	// and we really should do that.
	// but in that option, pthread_exit will throw an exception which will never be caught,
	// that's strange, and I don't know why.
	// so I change the compile option to use setjmp/longjmp, and this works,
	// but without stack unwinding.
	// we should fix this later...
	pthread_exit(value_ptr);

	LOG_WARN("shoudn't reach here %d", tid);
}


int PS4API scePthreadGetaffinity(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

thread_local static int t_thread_prio = 0;
int PS4API scePthreadSetprio(ScePthread thread, int prio)
{
	//LOG_SCE_TRACE("pthread %d prio %d", thread, prio);
	LOG_SCE_DUMMY_IMPL();
	t_thread_prio = prio;
	return SCE_OK;
}


int PS4API scePthreadGetprio(ScePthread thread, int *prio)
{
	LOG_SCE_DUMMY_IMPL();
	*prio = t_thread_prio;
	return SCE_OK;
}


int PS4API scePthreadJoin(ScePthread thread, void **value)
{
	LOG_SCE_TRACE("thread %d value %p", thread, value);
	int err = pthread_join(g_threadSlot[thread], value);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadRename(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


void PS4API scePthreadYield(void)
{
	LOG_SCE_TRACE("");
	UtilThread::ThreadYield();
}


int PS4API scePthreadCancel(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadDetach(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetname(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetspecific(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadSetschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadSetspecific(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////
int PS4API scePthreadRwlockattrInit(ScePthreadRwlockattr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_rwlockattr_init(attr);
	return pthreadErrorToSceError(err);
}

int PS4API scePthreadRwlockattrDestroy(ScePthreadRwlockattr *attr)
{
	LOG_SCE_TRACE("attr %p", attr);
	int err = pthread_rwlockattr_destroy(attr);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadRwlockInit(ScePthreadRwlock *rwlock, const ScePthreadRwlockattr *attr, const char *name)
{
	LOG_SCE_TRACE("rwlock %p attr %p name %p", rwlock, attr, name);
	// Note:
	// pthread4w do not support attr
	int err = pthread_rwlock_init(rwlock, NULL);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadRwlockDestroy(ScePthreadRwlock *rwlock)
{
	LOG_SCE_TRACE("rwlock %p", rwlock);
	int err = pthread_rwlock_destroy(rwlock);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadRwlockRdlock(ScePthreadRwlock *rwlock)
{
	LOG_SCE_TRACE("rwlock %p", rwlock);
	int err = pthread_rwlock_rdlock(rwlock);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadRwlockWrlock(ScePthreadRwlock *rwlock)
{
	LOG_SCE_TRACE("rwlock %p", rwlock);
	int err = pthread_rwlock_wrlock(rwlock);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadRwlockUnlock(ScePthreadRwlock *rwlock)
{
	LOG_SCE_TRACE("rwlock %p", rwlock);
	int err = pthread_rwlock_unlock(rwlock);
	return pthreadErrorToSceError(err);
}

//////////////////////////////////////////////////////////////////////////


int PS4API scePthreadKeyCreate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



