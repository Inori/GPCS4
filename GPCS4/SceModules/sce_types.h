#pragma once

// based on freebsd 9
// we plus "sce" prefix to prevent conflict with native definition

 /*
  * Standard type definitions.
  */
typedef uint32_t	__sce_blksize_t;    /* file block size */
typedef int64_t		__sce_blkcnt_t;      /* file block count */
typedef int32_t		__sce_clockid_t;     /* clock_gettime()... */
typedef uint64_t	__sce_cap_rights_t; /* capability rights */
typedef uint32_t	__sce_fflags_t;     /* file flags */
typedef uint64_t	__sce_fsblkcnt_t;
typedef uint64_t	__sce_fsfilcnt_t;
typedef uint32_t	__sce_gid_t;
typedef int64_t		__sce_id_t;    /* can hold a gid_t, pid_t, or uid_t */
typedef uint32_t	__sce_ino_t;  /* inode number */
typedef ps4_long	__sce_key_t;  /* IPC key (for Sys V IPC) */
typedef int32_t		__sce_lwpid_t; /* Thread ID (a.k.a. LWP) */
typedef uint16_t	__sce_mode_t; /* permissions */
typedef int			__sce_accmode_t;   /* access permissions */
typedef int			__sce_nl_item;
typedef uint16_t	__sce_nlink_t; /* link count */
typedef int64_t		__sce_off_t;    /* file offset */
typedef int32_t		__sce_pid_t;    /* process [group] */
typedef int64_t		__sce_rlim_t;   /* resource limit - intentionally */
								/* signed, because of legacy code */
								/* that uses -1 for RLIM_INFINITY */
typedef uint8_t		__sce_sa_family_t;
typedef uint32_t	__sce_socklen_t;
typedef uint32_t	__sce_uid_t;
typedef ps4_long	__sce_suseconds_t; /* microseconds (signed) */
typedef uint32_t	__sce_useconds_t;  /* microseconds (unsigned) */
typedef int			__sce_cpuwhich_t;       /* which parameter for cpuset. */
typedef int			__sce_cpulevel_t;       /* level parameter for cpuset. */
typedef int			__sce_cpusetid_t;       /* cpuset identifier. */

typedef int32_t		__sce_clock_t; /* clock()... */
typedef int64_t		__sce_critical_t;
typedef double		__sce_double_t;
typedef float		__sce_float_t;
typedef int64_t		__sce_intfptr_t;
typedef int64_t		__sce_intmax_t;
typedef int64_t		__sce_intptr_t;
typedef int32_t		__sce_int_fast8_t;
typedef int32_t		__sce_int_fast16_t;
typedef int32_t		__sce_int_fast32_t;
typedef int64_t		__sce_int_fast64_t;
typedef int8_t		__sce_int_least8_t;
typedef int16_t		__sce_int_least16_t;
typedef int32_t		__sce_int_least32_t;
typedef int64_t		__sce_int_least64_t;
typedef int64_t		__sce_ptrdiff_t; /* ptr1 - ptr2 */
typedef int64_t		__sce_register_t;
typedef int64_t		__sce_segsz_t; /* segment size (in pages) */
typedef uint64_t	__sce_size_t; /* sizeof() */
typedef int64_t		__sce_ssize_t; /* byte count or error */
typedef int64_t		__sce_time_t;  /* time()... */
typedef uint64_t	__sce_uintfptr_t;
typedef uint64_t	__sce_uintmax_t;
typedef uint64_t	__sce_uintptr_t;
typedef uint32_t	__sce_uint_fast8_t;
typedef uint32_t	__sce_uint_fast16_t;
typedef uint32_t	__sce_uint_fast32_t;
typedef uint64_t	__sce_uint_fast64_t;
typedef uint8_t		__sce_uint_least8_t;
typedef uint16_t	__sce_uint_least16_t;
typedef uint32_t	__sce_uint_least32_t;
typedef uint64_t	__sce_uint_least64_t;
typedef uint64_t	__sce_u_register_t;
typedef uint64_t	__sce_vm_offset_t;
typedef int64_t		__sce_vm_ooffset_t;
typedef uint64_t	__sce_vm_paddr_t;
typedef uint64_t	__sce_vm_pindex_t;
typedef uint64_t	__sce_vm_size_t;


typedef	__sce_time_t		sce_time_t;
typedef	__sce_clockid_t		sce_clockid_t;
typedef __sce_useconds_t	sce_useconds_t; /* microseconds (unsigned) */

struct sce_timespec
{
	sce_time_t tv_sec;  /* seconds */
	ps4_long   tv_nsec; /* and nanoseconds */
};

struct sce_div_t
{	/* result of int divide */
	int quot;
	int rem;
} ;

// this should be off_t, but conflict with c++'s header
// I don't know why, fuck that 
typedef int64_t sce_off_t;

typedef int64_t ssize_t;


//////////////////////////////////////////////////////////////////////////

#define SCE_DUMMY_USERID 0x12345

typedef int SceUserServiceUserId;

