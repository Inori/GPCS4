#include "sce_libkernel.h"
#include "sce_kernel_file.h"
#include "MapSlot.h"
#include "Platform/UtilPath.h"
#include <io.h>
#include <fcntl.h>
#include <cstdio>

LOG_CHANNEL(SceModules.SceLibkernel.file);

// this will be more friendly on linux....

#ifdef GPCS4_WINDOWS

#include "dirent/dirent.h"

enum FdType
{
	FD_TYPE_UNKNOWN,
	FD_TYPE_FILE,
	FD_TYPE_DIRECTORY
};

struct FdItem
{
	uintptr_t fd;  // Maybe a file or a directory
	FdType type;
};

bool isEqualFdItem(const FdItem& lhs, const FdItem& rhs)
{
	return (lhs.fd == rhs.fd) && (lhs.type == rhs.type);
}

bool isEmptyFdItem(const FdItem& item)
{
	return (item.fd == 0 && item.type == FD_TYPE_UNKNOWN);
}

// since windows doesn't allow open(directory),
// we record both DIR* and fd in this slot array
MapSlot<FdItem, isEmptyFdItem, isEqualFdItem> g_fdSlots(SCE_FD_MAX);


inline bool getDirName(DIR* dir, char* dirname, int len)
{
	bool bRet = false;
	do
	{
		wchar_t* wfname = dir->wdirp->patt;
		if (!wfname)
		{
			break;
		}

		if (wcstombs(dirname, wfname, len) == (size_t)-1)
		{
			break;
		}
		
		int str_len = strlen(dirname);
		dirname[str_len - 2] = 0;
		dirname[str_len - 1] = 0;
		
		bRet  = true;
	}while(false);
	return bRet;
}

#endif  //GPCS4_WINDOWS


int PS4API scek__write(int fd, const void* buf, size_t size)
{
	LOG_SCE_TRACE("fd %d buf 0x%p size %zu", fd, buf, size);

	do 
	{
		_write(fd, buf, size);

		if (fd != 1 && fd != 2)
		{
			break;
		}

		// If it's stdout/stderr, also log it to emulator logger
		// TODO: Strip newline, log line already adds one
		std::string tempBuf(static_cast<const char*>(buf), 0, size);
		tempBuf.append("\0"); // buf is not guaranteed to be null-terminated, so append null terminator after 'size' chars
		LOG_TRACE("%s", tempBuf.c_str());
		
	} while (false);

	return size;
}


int PS4API sceKernelOpen(const char *path, int flags, SceKernelMode mode)
{
	LOG_SCE_TRACE("path %s flag %x mode %x", path, flags, mode);
	std::string pcPath = UtilPath::PS4PathToPCPath(path);

#ifdef GPCS4_WINDOWS
	int idx = g_fdSlots.GetEmptySlotIndex();
	bool hasError = false;
	if (flags & SCE_KERNEL_O_DIRECTORY)
	{
		DIR* dir = opendir(pcPath.c_str());
		if (!dir)
		{
			LOG_WARN("open dir failed %s", path);
			hasError = true;
		}
		g_fdSlots[idx].fd = (uintptr_t)dir;
		g_fdSlots[idx].type = FD_TYPE_DIRECTORY;

	}
	else
	{
		LOG_ASSERT((flags == SCE_KERNEL_O_RDONLY), "not supported flag.");
		LOG_ASSERT((mode == SCE_KERNEL_S_IRU) || ((mode == SCE_KERNEL_S_INONE) && ((flags & SCE_KERNEL_O_CREAT) == 0)), "not supported mode.");

		int fd = _open(pcPath.c_str(), _O_RDONLY | _O_BINARY, _S_IREAD);
		if (fd == -1)
		{
			LOG_WARN("open file failed %s", path);
			hasError = true;
			g_fdSlots[idx].fd = 0;
			g_fdSlots[idx].type = FD_TYPE_UNKNOWN;
		}
		else
		{
			g_fdSlots[idx].fd = fd;
			g_fdSlots[idx].type = FD_TYPE_FILE;
		}
	}

	int ret_fd = hasError ? -1 : idx;
	return ret_fd;
#endif  //GPCS4_WINDOWS
}


ssize_t PS4API sceKernelRead(int d, void *buf, size_t nbytes)
{
	LOG_SCE_TRACE("d %d buff %p nbytes %x", d, buf, nbytes);
	int fd = g_fdSlots[d].fd;
	return _read(fd, buf, nbytes);
}


ssize_t PS4API sceKernelWrite(int d, const void *buf, size_t nbytes)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


sce_off_t PS4API sceKernelLseek(int fildes, sce_off_t offset, int whence)
{
	LOG_SCE_TRACE("fd %d off %d where %d", fildes, offset, whence);
	int fd = g_fdSlots[fildes].fd;
	return _lseeki64(fd, offset, whence);
}


int PS4API sceKernelClose(int d)
{
	LOG_SCE_TRACE("fd %d", d);

#ifdef GPCS4_WINDOWS
	int ret = -1;
	FdItem& item = g_fdSlots[d];
	if (item.type == FD_TYPE_DIRECTORY)
	{
		DIR* dir = (DIR*)item.fd;
		closedir(dir);
	}
	else
	{
		int fd = item.fd;
		_close(fd);
	}

	g_fdSlots[d] = {0, FD_TYPE_UNKNOWN};
	return ret;
#endif  //GPCS4_WINDOWS
}


inline sce_mode_t getSceFileMode(uint16_t oldMode)
{
	sce_mode_t sceMode = 0;
	if (oldMode & _S_IREAD)
	{
		sceMode = SCE_KERNEL_S_IRU;
	}

	if (oldMode & _S_IWRITE)
	{
		sceMode = SCE_KERNEL_S_IRWU;
	}

	if (oldMode & _S_IFMT & _S_IFREG)
	{
		sceMode |= SCE_KERNEL_S_IFREG;
	}
	else if (oldMode & _S_IFMT & _S_IFDIR)
	{
		sceMode |= SCE_KERNEL_S_IFDIR;
	}
	return sceMode;
}


int PS4API sceKernelStat(const char *path, SceKernelStat *sb)
{
	LOG_SCE_TRACE("path %s sb %p", path, sb);
	std::string pcPath = UtilPath::PS4PathToPCPath(path);

	struct _stat stat;
	int ret = _stat(pcPath.c_str(), &stat);
	sb->st_mode = getSceFileMode(stat.st_mode);
	//sb->st_atim = stat.st_atime;
	//sb->st_mtim = stat.st_mtime;
	//sb->st_ctim = stat.st_ctime;
	sb->st_size = stat.st_size;
	//sb->st_birthtim = stat.st_ctime; //?
	if (stat.st_mode & _S_IFMT & _S_IFDIR)
	{
		sb->st_blocks = UtilPath::FileCountInDirectory(pcPath);
		sb->st_blksize = sizeof(SceKernelDirent);
	}
	else
	{
		sb->st_blocks = stat.st_size / SSD_BLOCK_SIZE + (stat.st_size % SSD_BLOCK_SIZE) ? 1 : 0;
		sb->st_blksize = SSD_BLOCK_SIZE;
	}
	return ret;
}


int PS4API sceKernelFstat(int fd, SceKernelStat *sb)
{
	LOG_SCE_TRACE("fd %d sb %p", fd, sb);

#ifdef GPCS4_WINDOWS
	int ret = -1;
	FdItem& item = g_fdSlots[fd];
	if (item.type == FD_TYPE_DIRECTORY)
	{
		char dir_path[SCE_MAX_PATH] = { 0 };
		getDirName((DIR*)item.fd, dir_path, SCE_MAX_PATH);
		struct _stat stat;
		ret = _stat(dir_path, &stat);
		sb->st_mode = getSceFileMode(stat.st_mode);
		
		//sb->st_atim = stat.st_atime;
		//sb->st_mtim = stat.st_mtime;
		//sb->st_ctim = stat.st_ctime;
		sb->st_size = stat.st_size;
		//sb->st_birthtim = stat.st_ctime; //?
		sb->st_blocks = UtilPath::FileCountInDirectory(dir_path);
		sb->st_blksize = sizeof(SceKernelDirent);
	}
	else
	{
		int pcFd = item.fd;
		struct _stat stat;
		ret = _fstat(pcFd, &stat);
		sb->st_mode = getSceFileMode(stat.st_mode);

		//sb->st_atim = stat.st_atime;
		//sb->st_mtim = stat.st_mtime;
		//sb->st_ctim = stat.st_ctime;
		sb->st_size = stat.st_size;
		//sb->st_birthtim = stat.st_ctime; //?
		sb->st_blocks = stat.st_size / SSD_BLOCK_SIZE + (stat.st_size % SSD_BLOCK_SIZE) ? 1 : 0;
		sb->st_blksize = SSD_BLOCK_SIZE;
	}

	return ret;
#endif  //GPCS4_WINDOWS
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

inline uint8_t getSceFileType(dirent* ent)
{
	uint8_t type = SCE_KERNEL_DT_UNKNOWN;
	if ( (ent->d_namlen == 1 && !strncmp(ent->d_name, ".", 1)) ||
		 (ent->d_namlen == 2 && !strncmp(ent->d_name, ".", 2)) )
	{
		type = SCE_KERNEL_DT_DIR;
	}
	else
	{
		switch (ent->d_type)
		{
		case DT_DIR:
			type = SCE_KERNEL_DT_DIR;
			break;
		case DT_REG:
			type = SCE_KERNEL_DT_REG;
			break;
		default:
			LOG_ERR("found unknown file type. file %s type %x", ent->d_name, ent->d_type);
			break;
		}
	}

	return type;
}

int PS4API sceKernelGetdents(int fd, char *buf, int nbytes)
{
	LOG_SCE_TRACE("fd %d buff %p nbytes %x", fd, buf, nbytes);
#ifdef GPCS4_WINDOWS
	int ret = SCE_KERNEL_ERROR_EBADF;
	do 
	{
		FdItem& item = g_fdSlots[fd];
		if (item.type != FD_TYPE_DIRECTORY)
		{
			ret = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		DIR* dir = (DIR*)item.fd;
		dirent *ent;
		ent = readdir(dir);
		if (!ent)
		{
			ret = 0;  //ends
			break;
		}

		SceKernelDirent* sce_ent = (SceKernelDirent*)buf;
		sce_ent->d_fileno = ent->d_ino;
		sce_ent->d_reclen = sizeof(SceKernelDirent);
		sce_ent->d_type = getSceFileType(ent);
		sce_ent->d_namlen = ent->d_namlen;
		strcpy_s(sce_ent->d_name, SCE_MAX_PATH, ent->d_name);
		
		ret = sizeof(SceKernelDirent);
	} while (false);

	return ret;

#endif  //GPCS4_WINDOWS
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
	LOG_DEBUG("'%s', 0x%x, 0x%x)", path, flags, mode);
	int fd = _open(path, flags, mode);
	return fd;
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



