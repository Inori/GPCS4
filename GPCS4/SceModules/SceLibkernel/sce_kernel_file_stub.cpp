#include "sce_libkernel.h"
#include "sce_kernel_file.h"
#include "MapSlot.h"
#include "Platform/UtilPath.h"
//#include <io.h>
#include <unistd.h>
#include <sys/io.h>
#include <fcntl.h>
#include <cstdio>

LOG_CHANNEL(SceModules.SceLibkernel.file);

int PS4API scek__write(int fd, const void* buf, size_t size)
{
	return 0;
}


int PS4API sceKernelOpen(const char *path, int flags, SceKernelMode mode)
{
	return 0;
}


ssize_t PS4API sceKernelRead(int d, void *buf, size_t nbytes)
{
	return 0;
}


ssize_t PS4API sceKernelWrite(int d, const void *buf, size_t nbytes)
{
	return 0;
}


sce_off_t PS4API sceKernelLseek(int fildes, sce_off_t offset, int whence)
{
	return 0;
}


int PS4API sceKernelClose(int d)
{
	return 0;
}


inline sce_mode_t getSceFileMode(uint16_t oldMode)
{
	return 0;
}


int PS4API sceKernelStat(const char *path, SceKernelStat *sb)
{
	return 0;
}


int PS4API sceKernelFstat(int fd, SceKernelStat *sb)
{
	return 0;
}


int PS4API sceKernelFsync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelFtruncate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

inline uint8_t getSceFileType(void* ent)
{
	return 0;
}

int PS4API sceKernelGetdents(int fd, char *buf, int nbytes)
{
	return 0;
}


int PS4API sceKernelCheckReachability(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelChmod(void) 
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMkdir(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelRename(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelUnlink(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek__open(const char* path, int flags, SceKernelMode mode)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API scek_shm_open(const char *name, int oflag, SceKernelMode mode)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_shm_unlink(const char *name)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



