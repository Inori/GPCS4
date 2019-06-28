#pragma once

// maximum file descriptor count, including DIR* pointers
#define SCE_FD_MAX 2048

#define SCE_MAX_PATH 256

// based on freebsd 9
// we plus "sce" prefix to prevent conflict with native defination

 /*
  * Standard type definitions.
  */
typedef	uint32_t	__sceblksize_t;	/* file block size */
typedef	int64_t		__sceblkcnt_t;	/* file block count */
typedef	int32_t		__sceclockid_t;	/* clock_gettime()... */
typedef	uint64_t	__scecap_rights_t;	/* capability rights */
typedef	uint32_t	__scefflags_t;	/* file flags */
typedef	uint64_t	__scefsblkcnt_t;
typedef	uint64_t	__scefsfilcnt_t;
typedef	uint32_t	__scegid_t;
typedef	int64_t		__sceid_t;		/* can hold a gid_t, pid_t, or uid_t */
typedef	uint32_t	__sce_ino_t;	/* inode number */
typedef	long		__scekey_t;	/* IPC key (for Sys V IPC) */
typedef	int32_t		__scelwpid_t;	/* Thread ID (a.k.a. LWP) */
typedef	uint16_t	__scemode_t;	/* permissions */
typedef	int			__sceaccmode_t;	/* access permissions */
typedef	int			__scenl_item;
typedef	uint16_t	__scenlink_t;	/* link count */
typedef	int64_t		__sceoff_t;	/* file offset */
typedef	int32_t		__scepid_t;	/* process [group] */
typedef	int64_t		__scerlim_t;	/* resource limit - intentionally */
					/* signed, because of legacy code */
					/* that uses -1 for RLIM_INFINITY */
typedef	uint8_t		__scesa_family_t;
typedef	uint32_t	__scesocklen_t;
typedef	long		__scesuseconds_t;	/* microseconds (signed) */
typedef	uint32_t	__sceuid_t;
typedef	unsigned int	__sceuseconds_t;	/* microseconds (unsigned) */
typedef	int		__scecpuwhich_t;	/* which parameter for cpuset. */
typedef	int		__scecpulevel_t;	/* level parameter for cpuset. */
typedef int		__scecpusetid_t;	/* cpuset identifier. */


typedef	int32_t	__sceclock_t;		/* clock()... */
typedef	int64_t	__scecritical_t;
typedef	double	__scedouble_t;
typedef	float	__scefloat_t;
typedef	int64_t	__sceintfptr_t;
typedef	int64_t	__sceintmax_t;
typedef	int64_t	__sceintptr_t;
typedef	int32_t	__sceint_fast8_t;
typedef	int32_t	__sceint_fast16_t;
typedef	int32_t	__sceint_fast32_t;
typedef	int64_t	__sceint_fast64_t;
typedef	int8_t	__sceint_least8_t;
typedef	int16_t	__sceint_least16_t;
typedef	int32_t	__sceint_least32_t;
typedef	int64_t	__sceint_least64_t;
typedef	int64_t	__sceptrdiff_t;		/* ptr1 - ptr2 */
typedef	int64_t	__sceregister_t;
typedef	int64_t	__scesegsz_t;		/* segment size (in pages) */
typedef	uint64_t	__scesize_t;		/* sizeof() */
typedef	int64_t		__scessize_t;		/* byte count or error */
typedef	int64_t		__scetime_t;		/* time()... */
typedef	uint64_t	__sceuintfptr_t;
typedef	uint64_t	__sceuintmax_t;
typedef	uint64_t	__sceuintptr_t;
typedef	uint32_t	__sceuint_fast8_t;
typedef	uint32_t	__sceuint_fast16_t;
typedef	uint32_t	__sceuint_fast32_t;
typedef	uint64_t	__sceuint_fast64_t;
typedef	uint8_t		__sceuint_least8_t;
typedef	uint16_t	__sceuint_least16_t;
typedef	uint32_t	__sceuint_least32_t;
typedef	uint64_t	__sceuint_least64_t;
typedef	uint64_t	__sceu_register_t;
typedef	uint64_t	__scevm_offset_t;
typedef	int64_t		__scevm_ooffset_t;
typedef	uint64_t	__scevm_paddr_t;
typedef	uint64_t	__scevm_pindex_t;
typedef	uint64_t	__scevm_size_t;


typedef uint16_t SceKernelMode;
// open mode
#define SCE_KERNEL_S_IRUSR         (S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | \
									S_IXGRP | S_IXOTH)
#define SCE_KERNEL_S_IWUSR         (S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | \
									S_IXGRP | S_IXOTH)
#define SCE_KERNEL_S_IXUSR         (S_IXUSR | S_IXGRP | S_IXOTH)
#define SCE_KERNEL_S_IRWXU         (SCE_KERNEL_S_IRUSR | SCE_KERNEL_S_IWUSR)
// read write
#define SCE_KERNEL_S_IRWU          (SCE_KERNEL_S_IRUSR | SCE_KERNEL_S_IWUSR)
// read
#define SCE_KERNEL_S_IRU           (SCE_KERNEL_S_IRUSR)



// open flags
#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */
#define	O_NONBLOCK	0x0004		/* no delay */
#define	O_APPEND	0x0008		/* set append mode */
#define	O_CREAT		0x0200		/* create if nonexistent */
#define	O_TRUNC		0x0400		/* truncate to zero length */
#define	O_EXCL		0x0800		/* error if already exists */
#define	O_DSYNC		0x1000		/* synchronous data writes(omit inode writes) */
#define	O_NOCTTY	0x8000		/* don't assign controlling terminal */
#define	O_FSYNC		0x0080		/* synchronous writes */
#define	O_SYNC		0x0080		/* POSIX synonym for O_FSYNC */
#define O_DIRECT	0x00010000  /* Attempt to bypass buffer cache */
								/* Defined by POSIX Extended API Set Part 2 */
#define	O_DIRECTORY	0x00020000	/* Fail if not directory */

#define SCE_KERNEL_O_RDONLY        O_RDONLY
#define SCE_KERNEL_O_WRONLY        O_WRONLY 
#define SCE_KERNEL_O_RDWR          O_RDWR
#define SCE_KERNEL_O_NONBLOCK      O_NONBLOCK
#define SCE_KERNEL_O_APPEND        O_APPEND
#define SCE_KERNEL_O_CREAT         O_CREAT
#define SCE_KERNEL_O_TRUNC         O_TRUNC
#define SCE_KERNEL_O_EXCL          O_EXCL
#define SCE_KERNEL_O_DIRECT        O_DIRECT
#define SCE_KERNEL_O_FSYNC         O_FSYNC
#define SCE_KERNEL_O_SYNC          O_SYNC
#define SCE_KERNEL_O_DSYNC         O_DSYNC
#define SCE_KERNEL_O_DIRECTORY     O_DIRECTORY


typedef uint32_t		__scedev_t;	/* device number */
typedef	__sce_ino_t		sceino_t;		/* inode number */
typedef	__scemode_t		scemode_t;		/* permissions */
typedef	__scenlink_t	scenlink_t;	/* link count */
typedef	__sceuid_t		sceuid_t;		/* user id */
typedef	__scegid_t		scegid_t;		/* group id */
typedef	__scetime_t		scetime_t;
typedef	__sceblkcnt_t	sceblkcnt_t;
typedef	__sceblksize_t	sceblksize_t;
typedef	__scefflags_t	scefflags_t;	/* file flags */


struct sce_timespec {
	scetime_t	tv_sec;		/* seconds */
	long	tv_nsec;	/* and nanoseconds */
};

struct sce_stat {
	__scedev_t   st_dev;		/* inode's device */
	sceino_t	  st_ino;		/* inode's number */
	scemode_t	  st_mode;		/* inode protection mode */
	scenlink_t	  st_nlink;		/* number of hard links */
	sceuid_t	  st_uid;		/* user ID of the file's owner */
	scegid_t	  st_gid;		/* group ID of the file's group */
	__scedev_t   st_rdev;		/* device type */
	struct	sce_timespec st_atim;	/* time of last access */
	struct	sce_timespec st_mtim;	/* time of last data modification */
	struct	sce_timespec st_ctim;	/* time of last file status change */
	sceoff_t	  st_size;		/* file size, in bytes */
	sceblkcnt_t st_blocks;		/* blocks allocated for file */
	sceblksize_t st_blksize;		/* optimal blocksize for I/O */
	scefflags_t  st_flags;		/* user defined flags for file */
	uint32_t st_gen;		/* file generation number */
	int32_t st_lspare;
	struct sce_timespec st_birthtim;	/* time of file creation */
	/*
	 * Explicitly pad st_birthtim to 16 bytes so that the size of
	 * struct stat is backwards compatible.  We use bitfields instead
	 * of an array of chars so that this doesn't require a C99 compiler
	 * to compile if the size of the padding is 0.  We use 2 bitfields
	 * to cover up to 64 bits on 32-bit machines.  We assume that
	 * CHAR_BIT is 8...
	 */
	unsigned int :(8 / 2) * (16 - (int)sizeof(struct sce_timespec));
	unsigned int :(8 / 2) * (16 - (int)sizeof(struct sce_timespec));
};

typedef struct sce_stat SceKernelStat;


#define SCE_KERNEL_DT_UNKNOWN      0
#define SCE_KERNEL_DT_DIR          4
#define SCE_KERNEL_DT_REG          8

struct sce_dirent {
	uint32_t d_fileno;		/* file number of entry */
	uint16_t d_reclen;		/* length of this record */
	uint8_t  d_type; 		/* file type, see below */
	uint8_t  d_namlen;		/* length of string in d_name */
	char	d_name[255 + 1];	/* name must be no longer than this */
};

typedef struct sce_dirent SceKernelDirent;
