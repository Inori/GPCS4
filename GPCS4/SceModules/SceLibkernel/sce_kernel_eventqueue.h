#pragma once


struct sce_kevent 
{
	uintptr_t	ident;		/* identifier for this event */
	short		filter;		/* filter for event */
	uint16_t	flags;
	uint32_t	fflags;
	intptr_t	data;
	void		*udata;		/* opaque user data identifier */
};


typedef void* SceKernelEqueue;
typedef struct sce_kevent SceKernelEvent;