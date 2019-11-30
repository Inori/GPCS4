#include "sce_libkernel.h"
#include "pthreads4w/pthread.h"
#include "Platform/PlatformUtils.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libkernel
//////////////////////////////////////////////////////////////////////////

int PS4API __error(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __stack_chk_fail(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __stack_chk_guard(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __tls_get_addr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __pthread_cxa_finalize(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelClockGettime(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetCpumode(void)
{
	LOG_SCE_DUMMY_IMPL();
	return SCE_KERNEL_CPUMODE_7CPU_NORMAL;
}



// Is PS4 Pro
int PS4API sceKernelIsNeoMode(void)
{
	int isNeoMode = 1;
	LOG_SCE_TRACE("return %d", isNeoMode);
	return isNeoMode;
}



int PS4API sceKernelUsleep(SceKernelUseconds microseconds)
{
	//LOG_SCE_TRACE("ms %d", microseconds);
	UtilTime::MicroSleep(microseconds);
	return SCE_OK;
}



int PS4API sceKernelBatchMap(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelCheckedReleaseDirectMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelDlsym(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetGPI(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelGettimeofday(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelLoadStartModule(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelSetPrtAperture(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelUuidCreate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



//////////////////////////////////////////////////////////////////////////
// library: libSceCoredump
//////////////////////////////////////////////////////////////////////////

int PS4API sceCoredumpAttachMemoryRegion(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceCoredumpRegisterCoredumpHandler(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceCoredumpWriteUserData(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API scek_sched_yield(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_close(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_connect(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_recv(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_select(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_destroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_init(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_post(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_timedwait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_wait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_send(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_shutdown(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_getpid(void)
{
	int pid = 0x1337;
	LOG_SCE_TRACE("return %d", pid);
	return pid;
}


int PS4API scek_getppid(void)
{
	int pid = 0x1;
	LOG_SCE_TRACE("return %d", pid);
	return pid;
}





