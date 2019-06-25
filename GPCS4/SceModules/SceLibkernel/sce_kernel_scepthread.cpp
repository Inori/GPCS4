#include "sce_libkernel.h"
#include "sce_kernel_scepthread.h"
#include <utility>
#include "Platform/PlatformUtils.h"

// for non pointer type, we need to build a map to fit the type
thread_local std::pair<ScePthread, pthread_t> t_threadTMap;


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
	LOG_FIXME("set name is not supported yet.")
	int err = pthread_mutex_init((pthread_mutex_t*)mutex, (pthread_mutexattr_t*)attr);
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


//////////////////////////////////////////////////////////////////////////

int PS4API scePthreadAttrInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadAttrDestroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


int PS4API scePthreadAttrSetdetachstate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadAttrSetinheritsched(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadAttrSetschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadAttrSetschedpolicy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadAttrSetstacksize(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////
ScePthread PS4API scePthreadSelf(void)
{
	LOG_SCE_TRACE("");
	if (t_threadTMap.first == 0)
	{
		ScePthread st = UtilThread::GetThreadId();
		pthread_t pt = pthread_self();
		t_threadTMap.first = st;
		t_threadTMap.second = pt;
	}

	return t_threadTMap.first;
}


int PS4API scePthreadSetaffinity(ScePthread thread, const SceKernelCpumask mask)
{
	LOG_SCE_TRACE("mask %x", mask);
	cpu_set_t cpuset;

	// TODO:
	// should limit cpu count according to running machine
	for (int i = 0; i != SCE_KERNEL_CPU_MAX; ++i)
	{
		if (BIT_IS_SET(mask, i))
		{
			CPU_SET(i, &cpuset);
		}
	}

	int err = pthread_setaffinity_np(t_threadTMap.second, sizeof(cpu_set_t), &cpuset);
	return pthreadErrorToSceError(err);
}


int PS4API scePthreadCondInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCondDestroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCondBroadcast(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API scePthreadCondSignal(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCondWait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCondattrDestroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCondattrInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCreate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadExit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetaffinity(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetprio(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadJoin(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API scePthreadRename(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockDestroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockRdlock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockUnlock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockWrlock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockattrDestroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadRwlockattrInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}




int PS4API scePthreadSetprio(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadYield(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

