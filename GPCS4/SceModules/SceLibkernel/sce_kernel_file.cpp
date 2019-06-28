#include "sce_libkernel.h"
#include "sce_kernel_file.h"
#include "Platform/UtilPath.h"
#include <io.h>

#define SCE_FD_MAX 2048

int PS4API sceKernelOpen(const char *path, int flags, SceKernelMode mode)
{
	LOG_SCE_TRACE("path %s flag %x mode %x", path, flags, mode);
	std::string pcPath = UtilPath::PS4PathToPCPath(path);
	int fd = open(pcPath.c_str(), flags, mode);
	int e = errno;
	
	return 234;
}


ssize_t PS4API sceKernelRead(int d, void *buf, size_t nbytes)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


ssize_t PS4API sceKernelWrite(int d, const void *buf, size_t nbytes)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelLseek(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelClose(int d)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API sceKernelFstat(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
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


int PS4API sceKernelGetdents(void)
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


int PS4API sceKernelStat(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelUnlink(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

