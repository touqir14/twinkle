#pragma once

/*
	The order in which nytes are stored on a platform is referred to as the 
	platform's endianness. 
	- Little-endian platforms store multibytes numbers with the low-order bytes at the 
	lowest memory address.
	- Big-endian platforms sotre the most significant byte in the lowest memory address.

	Consider the value: 0x12345678
	Little-endian:		0x78 0x56 0x34 0x12
	Big-endian:			0x12 0x34 0x56 0x78
*/

#include "common.h"

namespace twinkle
{
	bool BIG_ENDIAN = (1 == htons(1));
	bool LITTLE_ENDIAN = !BIG_ENDIAN;

	// swap word from big-endian to little-endian
	void bswap()
	{
				
	}
}