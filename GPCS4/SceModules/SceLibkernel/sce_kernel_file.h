#pragma once


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
