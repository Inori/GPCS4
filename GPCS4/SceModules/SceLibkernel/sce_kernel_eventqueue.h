#pragma once


struct sce_kevent {
	uint_ptr	ident;		/* identifier for this event */
	short		filter;		/* filter for event */
	ushort		flags;
	uint		fflags;
	intptr_t	data;
	void		*udata;		/* opaque user data identifier */
};


typedef void* SceKernelEqueue;
typedef struct sce_kevent SceKernelEvent;