#include "sce_libc.h"
#include <mutex>
#include "pthreads4w/pthread.h"

LOG_CHANNEL(SceModules.SceLibc.cxa);

// at least 32
// currently give a large value to prevent atexit failed
#define SCE_CXA_EXIT_HANDLER_MAX 0x1000 

// note:
// when dso_handler and arg are both NULL
// we should call exit_handler with no arg
// this is strange, but it seems it's the standard
// http://refspecs.linuxbase.org/LSB_3.1.0/LSB-Core-generic/LSB-Core-generic/baselib---cxa-atexit.html
// also, all handlers are sysv abi, of course.

struct CAX_EXIT_HANDLER_INFO
{
	pfunc_cxa_exit_handler exit_handler;
	void* dso_handler;
	void* arg;
};

struct CXA_EXIT_HANDLER_ARRAY
{
	std::mutex mutex;
	uint32_t count;
	CAX_EXIT_HANDLER_INFO handler_array[SCE_CXA_EXIT_HANDLER_MAX];
};

CXA_EXIT_HANDLER_ARRAY g_cxa_exit_handlers;

//////////////////////////////////////////////////////////////////////////
// the following code based on Apple's implementation
// https://opensource.apple.com/source/libcppabi/libcppabi-14/src/cxa_guard.cxx



// Note don't use function local statics to avoid use of cxa functions...
static pthread_mutex_t __guard_mutex;
static pthread_once_t __once_control = PTHREAD_ONCE_INIT;

static void makeRecusiveMutex() // 将 __guard_mutex 初始化为递归锁
{
	pthread_mutexattr_t recursiveMutexAttr;
	pthread_mutexattr_init(&recursiveMutexAttr);
	pthread_mutexattr_settype(&recursiveMutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&__guard_mutex, &recursiveMutexAttr);
}


PS4NOINLINE
static pthread_mutex_t* guard_mutex()
{
	pthread_once(&__once_control, &makeRecusiveMutex); // init once __guard_mutex
	return &__guard_mutex;
}

// helper functions for getting/setting flags in guard_object
static bool initializerHasRun(uint64_t* guard_object)
{
	// the lowest byte marks if it has been inited
	return (*((uint8_t*)guard_object) != 0);
}

static void setInitializerHasRun(uint64_t* guard_object)
{
	*((uint8_t*)guard_object) = 1;
}

static bool inUse(uint64_t* guard_object)
{
	// the second lowest byte marks if it's being used by a thread
	return (((uint8_t*)guard_object)[1] != 0);
}

static void setInUse(uint64_t* guard_object)
{
	((uint8_t*)guard_object)[1] = 1;
}

static void setNotInUse(uint64_t* guard_object)
{
	((uint8_t*)guard_object)[1] = 0;
}

//////////////////////////////////////////////////////////////////////////

int PS4API scec___cxa_atexit(pfunc_cxa_exit_handler func, void * arg, void * dso_handle)
{
	LOG_SCE_TRACE("func %p arg %p dso %p", func, arg, dso_handle);

	// TODO:
	// currently we just record, these should be executed on program exit

	int ret = -1;
	std::lock_guard<std::mutex> lock(g_cxa_exit_handlers.mutex);
	do 
	{
		if (g_cxa_exit_handlers.count >= SCE_CXA_EXIT_HANDLER_MAX)
		{
			break;
		}

		uint32_t& idx = g_cxa_exit_handlers.count;
		g_cxa_exit_handlers.handler_array[idx].exit_handler = func;
		g_cxa_exit_handlers.handler_array[idx].dso_handler = dso_handle;
		g_cxa_exit_handlers.handler_array[idx].arg = arg;
		++idx;

		ret = 0;
	} while (false);

	return ret;
}


int PS4API scec___cxa_guard_acquire(uint64_t* guard_object)
{
	LOG_SCE_TRACE("guard_object %p", guard_object);

	// Double check that the initializer has not already been run
	if (initializerHasRun(guard_object)) // check if it's been inited
		return 0;

	// We now need to acquire a lock that allows only one thread
	// to run the initializer.  If a different thread calls
	// __cxa_guard_acquire() with the same guard object, we want 
	// that thread to block until this thread is done running the 
	// initializer and calls __cxa_guard_release().  But if the same
	// thread calls __cxa_guard_acquire() with the same guard object,
	// we want to abort.  
	// To implement this we have one global pthread recursive mutex 
	// shared by all guard objects, but only one at a time.  

	int result = ::pthread_mutex_lock(guard_mutex());
	if (result != 0) {
		LOG_ERR("__cxa_guard_acquire(): pthread_mutex_lock "
			"failed with %d\n", result);
	}
	// At this point all other threads will block in __cxa_guard_acquire()

	// Check if another thread has completed initializer run
	if (initializerHasRun(guard_object)) { // chech again if other thread init it
		int result = ::pthread_mutex_unlock(guard_mutex());
		if (result != 0) {
			LOG_ERR("__cxa_guard_acquire(): pthread_mutex_unlock "
				"failed with %d\n", result);
		}
		return 0;
	}

	// The pthread mutex is recursive to allow other lazy initialized
	// function locals to be evaluated during evaluation of this one.
	// But if the same thread can call __cxa_guard_acquire() on the 
	// *same* guard object again, we call abort();
	if (inUse(guard_object)) {
		LOG_ERR("__cxa_guard_acquire(): initializer for function "
			"local static variable called enclosing function\n");
	}

	// mark this guard object as being in use
	setInUse(guard_object);

	// return non-zero to tell caller to run initializer
	return 1;
}


void PS4API scec___cxa_guard_release(uint64_t* guard_object)
{
	LOG_SCE_TRACE("guard_object %p", guard_object);
	// first mark initalizer as having been run, so 
	// other threads won't try to re-run it.
	setInitializerHasRun(guard_object);

	// release global mutex
	int result = ::pthread_mutex_unlock(guard_mutex());
	if (result != 0) {
		LOG_ERR("__cxa_guard_acquire(): pthread_mutex_unlock "
			"failed with %d\n", result);
	}
}


int PS4API scec___cxa_pure_virtual(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API scec___catchReturnFromMain(void)
{
	LOG_SCE_DUMMY_IMPL();
	return SCE_OK;
}
