#pragma once

typedef unsigned int        SceKernelUseconds;

typedef struct _SceKernelSemaOptParam {
	size_t	size;
} SceKernelSemaOptParam;


typedef void* SceKernelSema;

// based on freebsd 9
// we plus "sce" prefix to prevent conflict with native definition

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
typedef	uint32_t	__sceuid_t;
typedef	long		__scesuseconds_t;	/* microseconds (signed) */
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


typedef	__sceclockid_t	sceclockid_t;
typedef	__scetime_t		scetime_t;

struct sce_timespec {
	scetime_t	tv_sec;		/* seconds */
	long	tv_nsec;	/* and nanoseconds */
};

typedef	__sceuseconds_t	sceuseconds_t;	/* microseconds (unsigned) */
