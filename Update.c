#include "includes.h"

//unsigned long crc32_table[256];
//
///* included as per crc32 specifications */
//unsigned long reflect(unsigned long ref, char byte)
//{
//	unsigned long ret = 0;
//	int n;
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
//	unsigned long poly = 0x04c11db7;
//	int n, k;
//
//	for( n = 0; n <= 0xff; n++ )
//	{
//		crc32_table[n] = reflect(n, 8) << 24;
//		for ( k = 0; k < 8; k++ )
//			crc32_table[n] = (crc32_table[n] << 1) ^ (crc32_table[n] & (1 << 31) ? poly : 0);
//		crc32_table[n] = reflect(crc32_table[n], 32);
//	}
//}
//
///* generate running crc32 */
//unsigned long crc32(unsigned long crc, unsigned char *buffer, unsigned long len)
//{
//	crc ^= 0xffffffff;
//	while ( len-- )
//		crc = (crc >> 8) ^ crc32_table[(crc & 0xff) ^ *buffer++];
//	return (crc ^ 0xffffffff);
//}
//
//int CompareCRC(char *lpszCurrentFile, size_t nCurrentFileLen, char *lpszUpdateFile, size_t nUpdateFileLen)
//{
//	unsigned long dwCurrentCRC, dwUpdateCRC;
//	return 1;
//}
//
//int CheckVersion(char *filename, size_t len)
//{
//	unsigned long crc;
//	unsigned char buffer[1024];
//	unsigned long n;
//	unsigned long dwCurrentLen, dwUpdateLen;
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