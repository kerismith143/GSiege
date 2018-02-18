#include <stdint.h>
#include "Update.h"

//uint32_t crc32_table[256];
//
///* included as per crc32 specifications */
//uint32_t reflect(uint32_t ref, uint8_t byte)
//{
//	uint32_t ret = 0;
//	uint8_t n;
//
//	for( n = 1; n < (byte + 1); n++ )
//	{
//		if ( ref & 1 )
//			ret |= 1 << (byte - n);
//		ref >>= 1;
//	}
//	return ret;
//}
//
///* initialize the crc32_table, this must be done before any crc32 generation */
//void crc32_init()
//{
//	uint32_t poly = 0x04C11DB7;
//	uint8_t n, k;
//
//	for( n = 0; n <= 0xFF; n++ )
//	{
//		crc32_table[n] = reflect(n, 8) << 24;
//		for ( k = 0; k < 8; k++ )
//			crc32_table[n] = (crc32_table[n] << 1) ^ (crc32_table[n] & (1 << 31) ? poly : 0);
//		crc32_table[n] = reflect(crc32_table[n], 32);
//	}
//}
//
///* generate running crc32 */
//uint32_t crc32(uint32_t crc, unsigned char *buffer, uint32_t len)
//{
//	crc ^= 0xFFFFFFFF;
//	while ( len-- )
//		crc = (crc >> 8) ^ crc32_table[(crc & 0xff) ^ *buffer++];
//	return (crc ^ 0xFFFFFFFF);
//}
//
//uint8_t CompareCRC(char *lpszCurrentFile, size_t nCurrentFileLen, char *lpszUpdateFile, size_t nUpdateFileLen)
//{
//	uint32_t dwCurrentCRC, dwUpdateCRC;
//	return 1;
//}
//
//uint8_t CheckVersion(char *filename, size_t len)
//{
//	uint32_t crc;
//	uint32_t buffer[1024];
//	uint32_t n;
//	uint32_t dwCurrentLen, dwUpdateLen;
//
//	crc32_init();
//
//	crc = 0;
//
//	for ( n = 0; n < dwCurrentLen; n++ )
//		crc32(crc, ..., len);
//
//	return 1;
//}