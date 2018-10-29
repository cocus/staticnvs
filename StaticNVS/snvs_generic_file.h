#ifndef __SNVS_GENERIC_FILE_H
#define __SNVS_GENERIC_FILE_H

#include "snvs.h"

#ifdef _MSC_VER
#include <stdio.h>

#define SNVS_GENERIC_FILE_IO(prefix,path)							\
FILE * prefix##_f;													\
int8_t prefix##_io(uint32_t total_size,								\
	uint32_t				starting_address,						\
	SNVS_IO					io)										\
{																	\
	int8_t ret = SNVS_NO_ERROR;										\
	switch (io)														\
	{																\
		case SNVS_IO_OPEN_DEVICE_WRITE:								\
		{															\
			fopen_s(&prefix##_f, path, "wb");						\
			ret = prefix##_f != NULL ? SNVS_NO_ERROR : SNVS_IO_ERR;	\
			break;													\
		}															\
		case SNVS_IO_OPEN_DEVICE_READ:								\
		{															\
			fopen_s(&prefix##_f, path, "rb");						\
			ret = prefix##_f != NULL ? SNVS_NO_ERROR : SNVS_IO_ERR;	\
			break;													\
		}															\
		case SNVS_IO_CLOSE_DEVICE_WRITE:							\
		{															\
			fclose(prefix##_f);										\
			break;													\
		}															\
		case SNVS_IO_CLOSE_DEVICE_READ:								\
		{															\
			fclose(prefix##_f);										\
			break;													\
		}															\
	}																\
	return ret;														\
}																	\
int8_t prefix##_write(uint32_t address,								\
	uint32_t size,													\
	uint8_t * byte)													\
{																	\
	if (fseek( prefix##_f, address, SEEK_SET))						\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	if (fwrite(byte, 1, size, prefix##_f) != size)					\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	return SNVS_NO_ERROR;											\
}																	\
int8_t prefix##_read(uint32_t address,								\
	uint32_t size,													\
	uint8_t * byte)													\
{																	\
	if (fseek( prefix##_f, address, SEEK_SET))						\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	if (fread(byte, 1, size, prefix##_f) != size)					\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	return SNVS_NO_ERROR;											\
}
#elif defined(__GNUC__)
#include <stdio.h>

#define SNVS_GENERIC_FILE_IO(prefix,path)							\
FILE * prefix##_f;													\
int8_t prefix##_io(uint32_t total_size,								\
	uint32_t				starting_address,						\
	SNVS_IO					io)										\
{																	\
	int8_t ret = SNVS_NO_ERROR;										\
	switch (io)														\
	{																\
		case SNVS_IO_OPEN_DEVICE_WRITE:								\
		{															\
			&prefix##_f = fopen_s(path, "wb");						\
			ret = prefix##_f != NULL ? SNVS_NO_ERROR : SNVS_IO_ERR;	\
			break;													\
		}															\
		case SNVS_IO_OPEN_DEVICE_READ:								\
		{															\
			&prefix##_f = fopen_s(path, "rb");						\
			ret = prefix##_f != NULL ? SNVS_NO_ERROR : SNVS_IO_ERR;	\
			break;													\
		}															\
		case SNVS_IO_CLOSE_DEVICE_WRITE:							\
		{															\
			fclose(prefix##_f);										\
			break;													\
		}															\
		case SNVS_IO_CLOSE_DEVICE_READ:								\
		{															\
			fclose(prefix##_f);										\
			break;													\
		}															\
	}																\
	return ret;														\
}																	\
int8_t prefix##_write(uint32_t address,								\
	uint32_t size,													\
	uint8_t * byte)													\
{																	\
	if (fseek( prefix##_f, address, SEEK_SET))						\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	if (fwrite(byte, 1, size, prefix##_f) != size)					\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	return SNVS_NO_ERROR;											\
}																	\
int8_t prefix##_read(uint32_t address,								\
	uint32_t size,													\
	uint8_t * byte)													\
{																	\
	if (fseek( prefix##_f, address, SEEK_SET))						\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	if (fread(byte, 1, size, prefix##_f) != size)					\
	{																\
		return SNVS_IO_BOUND_ERR;									\
	}																\
	return SNVS_NO_ERROR;											\
}
#endif

#endif /* __SNVS_GENERIC_FILE_H */