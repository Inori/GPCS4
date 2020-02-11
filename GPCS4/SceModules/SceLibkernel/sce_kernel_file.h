#pragma once
#include "sce_kernel_types.h"

// maximum file descriptor count, including DIR* pointers
#define SCE_FD_MAX 2048

#define SCE_MAX_PATH 255

#define SSD_BLOCK_SIZE 4096



typedef uint16_t SceKernelMode;


#define	SCE_S_IRWXU	0000700			/* RWX mask for owner */
#define	SCE_S_IRUSR	0000400			/* R for owner */
#define	SCE_S_IWUSR	0000200			/* W for owner */
#define	SCE_S_IXUSR	0000100			/* X for owner */

#define	SCE_S_IRWXG	0000070			/* RWX mask for group */
#define	SCE_S_IRGRP	0000040			/* R for group */
#define	SCE_S_IWGRP	0000020			/* W for group */
#define	SCE_S_IXGRP	0000010			/* X for group */
#define	SCE_S_IRWXO	0000007			/* RWX mask for other */
#define	SCE_S_IROTH	0000004			/* R for other */
#define	SCE_S_IWOTH	0000002			/* W for other */
#define	SCE_S_IXOTH	0000001			/* X for other */

#define	SCE_S_IFMT	 0170000		/* type of file mask */
#define	SCE_S_IFIFO	 0010000		/* named pipe (fifo) */
#define	SCE_S_IFCHR	 0020000		/* character special */
#define	SCE_S_IFDIR	 0040000		/* directory */
#define	SCE_S_IFBLK	 0060000		/* block special */
#define	SCE_S_IFREG	 0100000		/* regular */
#define	SCE_S_IFLNK	 0120000		/* symbolic link */
#define	SCE_S_IFSOCK 0140000		/* socket */
#define	SCE_S_ISVTX	 0001000		/* save swapped text even after use */

// open mode
#define SCE_KERNEL_S_INONE         0

#define SCE_KERNEL_S_IRUSR         (SCE_S_IRUSR | SCE_S_IRGRP | SCE_S_IROTH | SCE_S_IXUSR | \
									SCE_S_IXGRP | SCE_S_IXOTH)
#define SCE_KERNEL_S_IWUSR         (SCE_S_IWUSR | SCE_S_IWGRP | SCE_S_IWOTH | SCE_S_IXUSR | \
									SCE_S_IXGRP | SCE_S_IXOTH)
#define SCE_KERNEL_S_IXUSR         (SCE_S_IXUSR | SCE_S_IXGRP | SCE_S_IXOTH)
#define SCE_KERNEL_S_IRWXU         (SCE_KERNEL_S_IRUSR | SCE_KERNEL_S_IWUSR)
// read write
#define SCE_KERNEL_S_IRWU          (SCE_KERNEL_S_IRUSR | SCE_KERNEL_S_IWUSR)
// read
#define SCE_KERNEL_S_IRU           (SCE_KERNEL_S_IRUSR)

#define SCE_KERNEL_S_IFMT          SCE_S_IFMT
#define SCE_KERNEL_S_IFDIR         SCE_S_IFDIR
#define SCE_KERNEL_S_IFREG         SCE_S_IFREG

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


typedef uint32_t		__sce_dev_t;	/* device number */
typedef	__sce_ino_t		sce_ino_t;		/* inode number */
typedef	__sce_mode_t	sce_mode_t;		/* permissions */
typedef __sce_nlink_t	sce_nlink_t;       /* link count */
typedef __sce_uid_t		sce_uid_t;               /* user id */
typedef __sce_gid_t		sce_gid_t;           /* group id */
typedef __sce_blkcnt_t	sce_blkcnt_t;
typedef __sce_blksize_t	sce_blksize_t;
typedef __sce_fflags_t	sce_fflags_t; /* file flags */

struct sce_stat 
{
	__sce_dev_t   st_dev;		/* inode's device */
	sce_ino_t	  st_ino;		/* inode's number */
	sce_mode_t	  st_mode;		/* inode protection mode */
	sce_nlink_t	  st_nlink;		/* number of hard links */
	sce_uid_t	  st_uid;		/* user ID of the file's owner */
	sce_gid_t	  st_gid;		/* group ID of the file's group */
	__sce_dev_t   st_rdev;		/* device type */
	struct	sce_timespec st_atim;	/* time of last access */
	struct	sce_timespec st_mtim;	/* time of last data modification */
	struct	sce_timespec st_ctim;	/* time of last file status change */
	sce_off_t	  st_size;		/* file size, in bytes */
	sce_blkcnt_t  st_blocks;		/* blocks allocated for file */
	sce_blksize_t st_blksize;		/* optimal blocksize for I/O */
	sce_fflags_t  st_flags;		/* user defined flags for file */
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

struct sce_dirent 
{
	uint32_t d_fileno;		/* file number of entry */
	uint16_t d_reclen;		/* length of this record */
	uint8_t  d_type; 		/* file type, see below */
	uint8_t  d_namlen;		/* length of string in d_name */
	char	 d_name[SCE_MAX_PATH + 1];	/* name must be no longer than this */
};

typedef struct sce_dirent SceKernelDirent;
