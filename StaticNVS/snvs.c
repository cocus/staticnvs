#include "snvs.h"

#include <string.h>

int8_t snvs_store(void				* item,
	uint32_t						size,
	uint32_t						offset,
	snvs_callback_io				callback_io,
	snvs_callback_write				callback_write,
	snvs_callback_read				callback_read
)
{
	uint32_t			pos = offset;
	uint8_t				* byte = NULL;

	int8_t				ret = SNVS_NO_ERROR;

	snvs_header_t		header;

	if (callback_io)
	{
		if (ret = callback_io(size, offset, SNVS_IO_OPEN_DEVICE_WRITE) != SNVS_NO_ERROR)
		{
			return ret;
		}
	}

	if (callback_write == NULL)
	{
		ret = SNVS_BAD_CFG;
		goto _cleanup;
	}

	/* Setup the header */
	header.size = size;
	memcpy(header.MAGIC, SNVS_MAGIC, 4);
	header.crc32 = 0;

	byte = (uint8_t*)&header;
	if (ret = callback_write(pos, sizeof(snvs_header_t), byte) != SNVS_NO_ERROR)
	{
		goto _cleanup;
	}

	pos += sizeof(header);
	byte = (uint8_t*)item;
	if (ret = callback_write(pos, size, byte) != SNVS_NO_ERROR)
	{
		goto _cleanup;
	}

_cleanup:
	if (callback_io)
	{
		if (ret != SNVS_NO_ERROR)
		{
			callback_io(size, offset, SNVS_IO_CLOSE_DEVICE_WRITE);
		}
		else
		{
			ret = callback_io(size, offset, SNVS_IO_CLOSE_DEVICE_WRITE);
		}
	}
	return ret;
}

int8_t snvs_recall(void				* item,
	uint32_t						size,
	uint32_t						offset,
	snvs_callback_io				callback_io,
	snvs_callback_write				callback_write,
	snvs_callback_read				callback_read
)
{
	uint32_t			pos = offset;
	uint8_t				* byte = NULL;

	int8_t				ret = SNVS_NO_ERROR;

	snvs_header_t		header;

	if (callback_io)
	{
		if (ret = callback_io(size, offset, SNVS_IO_OPEN_DEVICE_READ) != SNVS_NO_ERROR)
		{
			return ret;
		}
	}

	if (callback_read == NULL)
	{
		ret = SNVS_BAD_CFG;
		goto _cleanup;
	}

	if (ret = callback_read(pos, sizeof(header), (uint8_t*)&header) != SNVS_NO_ERROR)
	{
		goto _cleanup;
	}

	/* Check the MAGIC header */
	if (memcmp(header.MAGIC, SNVS_MAGIC, 4))
	{
		ret = SNVS_MAGIC_ERR;
		goto _cleanup;
	}

	/* Check the CRC32 */
	
	pos += sizeof(header);
	byte = (uint8_t*)item;
	if (ret = callback_read(pos, size, byte) != SNVS_NO_ERROR)
	{
		goto _cleanup;
	}

_cleanup:
	if (callback_io)
	{
		if (ret != SNVS_NO_ERROR)
		{
			callback_io(size, offset, SNVS_IO_CLOSE_DEVICE_READ);
		}
		else
		{
			ret = callback_io(size, offset, SNVS_IO_CLOSE_DEVICE_READ);
		}
	}
	return ret;
}