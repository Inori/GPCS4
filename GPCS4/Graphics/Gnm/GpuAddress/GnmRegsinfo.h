#pragma once


#include "GnmRegsinfoPrivate.h"

#ifndef SCE_GNM_GET_FIELD
#define SCE_GNM_GET_FIELD(src, registername, field) \
	(((src) & (GNM_##registername##__##field##__MASK)) >> (GNM_##registername##__##field##__SHIFT))
#endif  // !SCE_GNM_GET_FIELD

#ifndef SCE_GNM_COMPOSE_FIELD
#define SCE_GNM_COMPOSE_FIELD(registername, field, value) \
	(((value) << GNM_ ## registername ## __ ## field ## __SHIFT) & (GNM_ ## registername ## __ ## field ## __MASK))
#endif  // !SCE_GNM_COMPOSE_FIELD

#ifndef SCE_GNM_COMPOSE_FIELD2
#define SCE_GNM_COMPOSE_FIELD2(registername, field1, value1, field2, value2) \
	((((value1) << GNM_ ## registername ## __ ## field1 ## __SHIFT) & (GNM_ ## registername ## __ ## field1 ## __MASK)) | \
	(((value2) << GNM_ ## registername ## __ ## field2 ## __SHIFT) & (GNM_ ## registername ## __ ## field2 ## __MASK)))
#endif  // !SCE_GNM_COMPOSE_FIELD2

#ifndef SCE_GNM_SET_FIELD
#define SCE_GNM_SET_FIELD(dest, registername, field, value)                                                      \
	(dest) = ((dest) & ~(GNM_ ## registername ## __ ## field ## __MASK)) |                                            \
	(((value) << GNM_ ## registername ## __ ## field ## __SHIFT) & (GNM_ ## registername ## __ ## field ## __MASK)        \
	)
#endif  // !SCE_GNM_SET_FIELD

#ifndef SCE_GNM_SET_FIELD2
#define SCE_GNM_SET_FIELD2(dest, registername, field1, value1, field2, value2)                                   \
	(dest) = ( ((dest) & ~(GNM_ ## registername ## __ ## field1 ## __MASK | GNM_ ## registername ## __ ## field2 ## __MASK)) | \
	(((value1) << GNM_ ## registername ## __ ## field1 ## __SHIFT) & (GNM_ ## registername ## __ ## field1 ## __MASK)) |  \
	(((value2) << GNM_ ## registername ## __ ## field2 ## __SHIFT) & (GNM_ ## registername ## __ ## field2 ## __MASK))    \
	)
#endif  // !SCE_GNM_SET_FIELD2

#ifndef SCE_GNM_SET_FIELD3
#define SCE_GNM_SET_FIELD3(dest, registername, field1, value1, field2, value2, field3, value3)                   \
	(dest) = ( ((dest) & ~(GNM_ ## registername ## __ ## field1 ## __MASK | GNM_ ## registername ## __ ## field2 ## __MASK |   \
	GNM_ ## registername ## __ ## field3 ## __MASK)) |                                           \
	(((value1) << GNM_ ## registername ## __ ## field1 ## __SHIFT) & (GNM_ ## registername ## __ ## field1 ## __MASK)) |  \
	(((value2) << GNM_ ## registername ## __ ## field2 ## __SHIFT) & (GNM_ ## registername ## __ ## field2 ## __MASK)) |  \
	(((value3) << GNM_ ## registername ## __ ## field3 ## __SHIFT) & (GNM_ ## registername ## __ ## field3 ## __MASK))    \
	)
#endif  // !SCE_GNM_SET_FIELD3

#ifndef SCE_GNM_SET_FIELD4
#define SCE_GNM_SET_FIELD4(dest, registername, field1, value1, field2, value2, field3, value3, field4, value4)   \
	(dest) = ( ((dest) & ~(GNM_ ## registername ## __ ## field1 ## __MASK | GNM_ ## registername ## __ ## field2 ## __MASK |   \
	GNM_ ## registername ## __ ## field3 ## __MASK | GNM_ ## registername ## __ ## field4 ## __MASK)) |   \
	(((value1) << GNM_ ## registername ## __ ## field1 ## __SHIFT) & (GNM_ ## registername ## __ ## field1 ## __MASK)) |  \
	(((value2) << GNM_ ## registername ## __ ## field2 ## __SHIFT) & (GNM_ ## registername ## __ ## field2 ## __MASK)) |  \
	(((value3) << GNM_ ## registername ## __ ## field3 ## __SHIFT) & (GNM_ ## registername ## __ ## field3 ## __MASK)) |  \
	(((value4) << GNM_ ## registername ## __ ## field4 ## __SHIFT) & (GNM_ ## registername ## __ ## field4 ## __MASK))    \
	)
#endif  // !SCE_GNM_SET_FIELD4
