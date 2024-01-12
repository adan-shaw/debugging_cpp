#include <stdio.h>

#include "3DES.h"

int main(void){
	size_t i;
	unsigned char Output[8];

	const unsigned char Message[8] = {0x67, 0x5A, 0x69, 0x67, 0x5E, 0x5A, 0x6B, 0x5A};
	const char Key[8] = {0x5B, 0x5A, 0x57, 0x67, 0x6A, 0x56, 0x67, 0x6E};

	printf("\n\t Testing DES........ ");
	DES(Message, Key, Output, 1);

	if((Output[0] != 0x9B) || (Output[1] != 0xA5) || (Output[2] != 0x96) || (Output[3] != 0x9B) || (Output[4] != 0xAD) || (Output[5] != 0xA5) || (Output[6] != 0x97) || (Output[7] != 0xA5)){
		printf("FAIL\n");
		return 1;
	}

	printf("passed\n");
	return 0;
}
