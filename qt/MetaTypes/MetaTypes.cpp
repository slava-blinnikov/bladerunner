#include "MetaTypes.h"

unsigned int crc32(unsigned char *buf, unsigned long len, unsigned int *crc_table)
{
	unsigned int  crc = 0xFFFFFFFFUL;
	while (len--)
	{
		crc = crc_table[(crc ^ (*buf)) & 0xFF] ^ (crc >> 8);
		buf++;
	}
	return crc ^ 0xFFFFFFFFUL;
}


void initCrc32Table(unsigned int *crc_table)
{
	unsigned int crc;
	for (int i = 0; i < 256; i++)
	{
		crc = i;
		for (int j = 0; j < 8; j++)
		{
			crc = (crc & 1) ? ((crc >> 1) ^ 0xEDB88320UL) : (crc >> 1);
		}
		crc_table[i] = crc;
	}
}
