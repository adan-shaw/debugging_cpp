#ifndef TRIPLEDES_H
#define TRIPLEDES_H

	#include <stdlib.h>

	/* Check if it is a C++ compiler */
	#ifdef __cplusplus
		extern "C" {
	#endif

	inline void DES(const unsigned char Message[8], const char Key[8], unsigned char Output[8], int encipher);
	void TripleDES(const unsigned char Message[8], const char Key[24], unsigned char Output[8], int encipher);

	#ifdef __cplusplus
		}
	#endif

#endif
