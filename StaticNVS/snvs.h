#ifndef __SNVS_H
#define __SNVS_H

#include "map.h"
#include "packing.h"
#include <stdint.h>

#define SNVS_MAGIC			"NVS!"


#define SNVS_NO_ERROR		0
#define SNVS_IO_ERR			-1
#define SNVS_IO_BOUND_ERR	-2
#define SNVS_MAGIC_ERR		-3
#define SNVS_BAD_CFG		-4


typedef enum
{
	SNVS_IO_OPEN_DEVICE_WRITE = 0,
	SNVS_IO_OPEN_DEVICE_READ = 1,
	SNVS_IO_CLOSE_DEVICE_WRITE = 2,
	SNVS_IO_CLOSE_DEVICE_READ = 3,
} SNVS_IO;

#define SNVS_HEADER_STRUCTURE_ITEMS		\
		uint8_t			MAGIC[4];		\
		uint32_t		size;			\
		uint32_t		crc32;

PACKED
(
	typedef struct
	{
		SNVS_HEADER_STRUCTURE_ITEMS
	} snvs_header_t
);



typedef int8_t(*snvs_callback_io)		(uint32_t total_size, uint32_t starting_address, SNVS_IO io);
typedef int8_t(*snvs_callback_write)	(uint32_t address, uint32_t size, uint8_t * start);
typedef int8_t(*snvs_callback_read)		(uint32_t address, uint32_t size, uint8_t * byte);

int8_t snvs_store
(
	void							* item,
	uint32_t						size,
	uint32_t						offset,
	snvs_callback_io				callback_io,
	snvs_callback_write				callback_write,
	snvs_callback_read				callback_read
);

int8_t snvs_recall
(
	void							* item,
	uint32_t						size,
	uint32_t						offset,
	snvs_callback_io				callback_io,
	snvs_callback_write				callback_write,
	snvs_callback_read				callback_read
);

#define SNVS_SIZEOF(C)		0 C(+ sizeof)

#define SNVS_STRUCT_ITEM(__struct__name__)	__struct__name__	__struct__name__##_;

#define SNVS_SPAWN_STRUCTURE(C)			\
PACKED									\
(										\
	typedef struct						\
	{									\
		C(SNVS_STRUCT_ITEM)				\
	} SNVS_##C							\
);

#define SNVS_SPAWN_GLOBAL_VARIABLES(C)	\
static SNVS_##C _ ## C ;

#define SNVS_SPAWN_LOCAL_VARIABLES(C)	\
SNVS_##C C##_struct;

#define SNVS_SPAWN_CONSTS(C,io,w,r)

#define SNVS_SPAWN_LOCAL_METHODS(C,off,io,w,r)																	\
static inline int8_t C##_STORE(SNVS_##C * s)	{ return snvs_store((void*)s, SNVS_SIZEOF(C), off, io, w, r); }	\
static inline int8_t C##_RECALL(SNVS_##C * s)	{ return snvs_recall((void*)s, SNVS_SIZEOF(C), off, io, w, r); }\

#define SNVS_SPAWN_GLOBAL_METHODS(C,off,io, w, r)																\
static inline int8_t C##_STORE()	{ return snvs_store((void*)&_ ## C, SNVS_SIZEOF(C), off, io, w, r); }		\
static inline int8_t C##_RECALL()	{ return snvs_recall((void*)&_ ## C, SNVS_SIZEOF(C), off, io, w, r); }		\

#define SNVS_SPAWN_GLOBAL_CONFIG(C, offset, io, write, read)		\
		SNVS_SPAWN_STRUCTURE(C)										\
		SNVS_SPAWN_GLOBAL_VARIABLES(C)								\
		SNVS_SPAWN_CONSTS(C, io, write, read)						\
		SNVS_SPAWN_GLOBAL_METHODS(C, offset, io, write, read)

#define SNVS_SPAWN_LOCAL_CONFIG(C, offset, io, write, read)			\
		SNVS_SPAWN_STRUCTURE(C)										\
		SNVS_SPAWN_CONSTS(C, io, write, read)						\
		SNVS_SPAWN_LOCAL_METHODS(C, offset, io, write, read)




#endif /* __SNVS_H */