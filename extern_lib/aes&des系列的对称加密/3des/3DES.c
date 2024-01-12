#include "3DES.h"

static unsigned char Sbox(const size_t i, const unsigned char c);
static void CompactPermuteXor(const unsigned char Key[8], const unsigned char L[4], unsigned char Output[4]);
static void ExpandXor(const unsigned char Key[8], const unsigned char R[4], unsigned char Output[8]);
static void LeftShift(const unsigned char Key[4], unsigned char Output[4]);

static unsigned char Sbox(const size_t i, const unsigned char c){

	size_t line, column;

	const unsigned char S[8][4][16] = {
		{{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		 { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		 { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		 {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}},

		{{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		 { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		 { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		 {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}},

		{{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		 {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		 {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		 { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}},

		{{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		 {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		 {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		 { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}},

		{{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		 {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		 { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		 {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}},

		{{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		 {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		 { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		 { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}},

		{{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		 {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		 { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		 { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}},

		{{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		 { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		 { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		 { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}}};

	line = column = 0;

	if(c & 128) line += 2;
	if(c & 4)   line += 1;

	if(c & 64) column += 8;
	if(c & 32) column += 4;
	if(c & 16) column += 2;
	if(c &  8) column += 1;

	return S[i][line][column];
}

static void CompactPermuteXor(const unsigned char Key[8], const unsigned char L[4], unsigned char Output[4]){
	unsigned char tmp[4];

	Output[0] = Output[1] = Output[2] = Output[3] = 0;

	/* Compact with S-Boxes */
	tmp[0]  =  Sbox(0, Key[0]);
	tmp[0] <<= 4;
	tmp[0] |=  Sbox(1, Key[1]);

	tmp[1]  =  Sbox(2, Key[2]);
	tmp[1] <<= 4;
	tmp[1] |=  Sbox(3, Key[3]);

	tmp[2] =   Sbox(4, Key[4]);
	tmp[2] <<= 4;
	tmp[2] |=  Sbox(5, Key[5]);

	tmp[3]  =  Sbox(6, Key[6]);
	tmp[3] <<= 4;
	tmp[3] |=  Sbox(7, Key[7]);

	/* Permute */
	if(tmp[1] &   1) Output[0] |= 128;
	if(tmp[0] &   2) Output[0] |=  64;
	if(tmp[2] &  16) Output[0] |=  32;
	if(tmp[2] &   8) Output[0] |=  16;
	if(tmp[3] &   8) Output[0] |=   8;
	if(tmp[1] &  16) Output[0] |=   4;
	if(tmp[3] &  16) Output[0] |=   2;
	if(tmp[2] & 128) Output[0] |=   1;

	if(tmp[0] & 128) Output[1] |= 128;
	if(tmp[1] &   2) Output[1] |=  64;
	if(tmp[2] &   2) Output[1] |=  32;
	if(tmp[3] &  64) Output[1] |=  16;
	if(tmp[0] &   8) Output[1] |=   8;
	if(tmp[2] &  64) Output[1] |=   4;
	if(tmp[3] &   2) Output[1] |=   2;
	if(tmp[1] &  64) Output[1] |=   1;

	if(tmp[0] &  64) Output[2] |= 128;
	if(tmp[0] &   1) Output[2] |=  64;
	if(tmp[2] &   1) Output[2] |=  32;
	if(tmp[1] &   4) Output[2] |=  16;
	if(tmp[3] &   1) Output[2] |=   8;
	if(tmp[3] &  32) Output[2] |=   4;
	if(tmp[0] &  32) Output[2] |=   2;
	if(tmp[1] & 128) Output[2] |=   1;

	if(tmp[2] &  32) Output[3] |= 128;
	if(tmp[1] &   8) Output[3] |=  64;
	if(tmp[3] &   4) Output[3] |=  32;
	if(tmp[0] &   4) Output[3] |=  16;
	if(tmp[2] &   4) Output[3] |=   8;
	if(tmp[1] &  32) Output[3] |=   4;
	if(tmp[0] &  16) Output[3] |=   2;
	if(tmp[3] & 128) Output[3] |=   1;

	Output[0] ^= L[0];
	Output[1] ^= L[1];
	Output[2] ^= L[2];
	Output[3] ^= L[3];
}

static void ExpandXor(const unsigned char Key[8], const unsigned char R[4], unsigned char Output[8]){

	Output[0] = Output[1] = Output[2] = Output[3] = Output[4] = Output[5] = Output[6] = Output[7] = 0;

	/* E-Bit Selection Table */
	if(R[3] &   1) Output[0] |= 128;
	if(R[0] & 128) Output[0] |=  64;
	if(R[0] &  64) Output[0] |=  32;
	if(R[0] &  32) Output[0] |=  16;
	if(R[0] &  16) Output[0] |=   8;
	if(R[0] &   8) Output[0] |=   4;

	if(R[0] &  16) Output[1] |= 128;
	if(R[0] &   8) Output[1] |=  64;
	if(R[0] &   4) Output[1] |=  32;
	if(R[0] &   2) Output[1] |=  16;
	if(R[0] &   1) Output[1] |=   8;
	if(R[1] & 128) Output[1] |=   4;

	if(R[0] &   1) Output[2] |= 128;
	if(R[1] & 128) Output[2] |=  64;
	if(R[1] &  64) Output[2] |=  32;
	if(R[1] &  32) Output[2] |=  16;
	if(R[1] &  16) Output[2] |=   8;
	if(R[1] &   8) Output[2] |=   4;

	if(R[1] &  16) Output[3] |= 128;
	if(R[1] &   8) Output[3] |=  64;
	if(R[1] &   4) Output[3] |=  32;
	if(R[1] &   2) Output[3] |=  16;
	if(R[1] &   1) Output[3] |=   8;
	if(R[2] & 128) Output[3] |=   4;

	if(R[1] &   1) Output[4] |= 128;
	if(R[2] & 128) Output[4] |=  64;
	if(R[2] &  64) Output[4] |=  32;
	if(R[2] &  32) Output[4] |=  16;
	if(R[2] &  16) Output[4] |=   8;
	if(R[2] &   8) Output[4] |=   4;

	if(R[2] &  16) Output[5] |= 128;
	if(R[2] &   8) Output[5] |=  64;
	if(R[2] &   4) Output[5] |=  32;
	if(R[2] &   2) Output[5] |=  16;
	if(R[2] &   1) Output[5] |=   8;
	if(R[3] & 128) Output[5] |=   4;

	if(R[2] &   1) Output[6] |= 128;
	if(R[3] & 128) Output[6] |=  64;
	if(R[3] &  64) Output[6] |=  32;
	if(R[3] &  32) Output[6] |=  16;
	if(R[3] &  16) Output[6] |=   8;
	if(R[3] &   8) Output[6] |=   4;

	if(R[3] &  16) Output[7] |= 128;
	if(R[3] &   8) Output[7] |=  64;
	if(R[3] &   4) Output[7] |=  32;
	if(R[3] &   2) Output[7] |=  16;
	if(R[3] &   1) Output[7] |=   8;
	if(R[0] & 128) Output[7] |=   4;

	Output[0] ^= Key[0];
	Output[1] ^= Key[1];
	Output[2] ^= Key[2];
	Output[3] ^= Key[3];
	Output[4] ^= Key[4];
	Output[5] ^= Key[5];
	Output[6] ^= Key[6];
	Output[7] ^= Key[7];
}

static void LeftShift(const unsigned char Key[4], unsigned char Output[4]){
	Output[0] = Output[1] = Output[2] = Output[3] = 0;

	if(Key[0] & 128) Output[3] |= 1;
	if(Key[1] & 128) Output[0] |= 1;
	if(Key[2] & 128) Output[1] |= 1;
	if(Key[3] & 128) Output[2] |= 1;

	Output[0] <<= 1;
	Output[1] <<= 1;
	Output[2] <<= 1;
	Output[3] <<= 1;
}

void DES(const unsigned char Message[8], const char Key[8], unsigned char Output[8], int encipher){
	size_t i;

	unsigned char C[16][8] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}};

	unsigned char K[16][8] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}};

	unsigned char L[16][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}};

	unsigned char R[16][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}};

	/* 56-bit permutation (Permuted Choice 1)  */

	if(Key[7] & 128) C[0][0] |= 128;
	if(Key[6] & 128) C[0][0] |=  64;
	if(Key[5] & 128) C[0][0] |=  32;
	if(Key[4] & 128) C[0][0] |=  16;   /* First byte */
	if(Key[3] & 128) C[0][0] |=   8;
	if(Key[2] & 128) C[0][0] |=   4;
	if(Key[1] & 128) C[0][0] |=   2;

	if(Key[0] &  64) C[0][1] |= 128;
	if(Key[7] &  64) C[0][1] |=  64;
	if(Key[6] &  64) C[0][1] |=  32;
	if(Key[5] &  64) C[0][1] |=  16;   /* Second byte */
	if(Key[4] &  64) C[0][1] |=   8;
	if(Key[3] &  64) C[0][1] |=   4;
	if(Key[2] &  64) C[0][1] |=   2;

	if(Key[1] &  64) C[0][2] |= 128;
	if(Key[0] &  64) C[0][2] |=  64;
	if(Key[7] &  32) C[0][2] |=  32;
	if(Key[6] &  32) C[0][2] |=  16;   /* Third byte */
	if(Key[5] &  32) C[0][2] |=   8;
	if(Key[4] &  32) C[0][2] |=   4;
	if(Key[3] &  32) C[0][2] |=   2;

	if(Key[2] &  32) C[0][3] |= 128;
	if(Key[1] &  32) C[0][3] |=  64;
	if(Key[0] &  32) C[0][3] |=  32;
	if(Key[7] &  16) C[0][3] |=  16;   /* Fourth byte */
	if(Key[6] &  16) C[0][3] |=   8;
	if(Key[5] &  16) C[0][3] |=   4;
	if(Key[4] &  16) C[0][3] |=   2;

	if(Key[7] &   2) C[0][4] |= 128;
	if(Key[6] &   2) C[0][4] |=  64;
	if(Key[5] &   2) C[0][4] |=  32;
	if(Key[4] &   2) C[0][4] |=  16;   /* Fifth byte */
	if(Key[3] &   2) C[0][4] |=   8;
	if(Key[2] &   2) C[0][4] |=   4;
	if(Key[1] &   2) C[0][4] |=   2;

	if(Key[0] &   2) C[0][5] |= 128;
	if(Key[7] &   4) C[0][5] |=  64;
	if(Key[6] &   4) C[0][5] |=  32;
	if(Key[5] &   4) C[0][5] |=  16;   /* Sixth byte */
	if(Key[4] &   4) C[0][5] |=   8;
	if(Key[3] &   4) C[0][5] |=   4;
	if(Key[2] &   4) C[0][5] |=   2;

	if(Key[1] &   4) C[0][6] |= 128;
	if(Key[0] &   4) C[0][6] |=  64;
	if(Key[7] &   8) C[0][6] |=  32;
	if(Key[6] &   8) C[0][6] |=  16;   /* Seventh byte */
	if(Key[5] &   8) C[0][6] |=   8;
	if(Key[4] &   8) C[0][6] |=   4;
	if(Key[3] &   8) C[0][6] |=   2;

	if(Key[2] &   8) C[0][7] |= 128;
	if(Key[1] &   8) C[0][7] |=  64;
	if(Key[0] &   8) C[0][7] |=  32;
	if(Key[3] &  16) C[0][7] |=  16;   /* Eighth byte */
	if(Key[2] &  16) C[0][7] |=   8;
	if(Key[1] &  16) C[0][7] |=   4;
	if(Key[0] &  16) C[0][7] |=   2;

	/* C1 */
	LeftShift(&C[0][0], &C[0][0]);
	LeftShift(&C[0][4], &C[0][4]);

	/* C2 */
	LeftShift(&C[0][0], &C[1][0]);
	LeftShift(&C[0][4], &C[1][4]);

	/* C3 */
	LeftShift(&C[1][0], &C[2][0]);
	LeftShift(&C[1][4], &C[2][4]);
	LeftShift(&C[2][0], &C[2][0]);
	LeftShift(&C[2][4], &C[2][4]);

	/* C4 */
	LeftShift(&C[2][0], &C[3][0]);
	LeftShift(&C[2][4], &C[3][4]);
	LeftShift(&C[3][0], &C[3][0]);
	LeftShift(&C[3][4], &C[3][4]);

	/* C5 */
	LeftShift(&C[3][0], &C[4][0]);
	LeftShift(&C[3][4], &C[4][4]);
	LeftShift(&C[4][0], &C[4][0]);
	LeftShift(&C[4][4], &C[4][4]);

	/* C6 */
	LeftShift(&C[4][0], &C[5][0]);
	LeftShift(&C[4][4], &C[5][4]);
	LeftShift(&C[5][0], &C[5][0]);
	LeftShift(&C[5][4], &C[5][4]);

	/* C7 */
	LeftShift(&C[5][0], &C[6][0]);
	LeftShift(&C[5][4], &C[6][4]);
	LeftShift(&C[6][0], &C[6][0]);
	LeftShift(&C[6][4], &C[6][4]);

	/* C8 */
	LeftShift(&C[6][0], &C[7][0]);
	LeftShift(&C[6][4], &C[7][4]);
	LeftShift(&C[7][0], &C[7][0]);
	LeftShift(&C[7][4], &C[7][4]);

	/* C9 */
	LeftShift(&C[7][0], &C[8][0]);
	LeftShift(&C[7][4], &C[8][4]);

	/* C10 */
	LeftShift(&C[8][0], &C[9][0]);
	LeftShift(&C[8][4], &C[9][4]);
	LeftShift(&C[9][0], &C[9][0]);
	LeftShift(&C[9][4], &C[9][4]);

	/* C11 */
	LeftShift(&C[9][0], &C[10][0]);
	LeftShift(&C[9][4], &C[10][4]);
	LeftShift(&C[10][0], &C[10][0]);
	LeftShift(&C[10][4], &C[10][4]);

	/* C12 */
	LeftShift(&C[10][0], &C[11][0]);
	LeftShift(&C[10][4], &C[11][4]);
	LeftShift(&C[11][0], &C[11][0]);
	LeftShift(&C[11][4], &C[11][4]);

	/* C13 */
	LeftShift(&C[11][0], &C[12][0]);
	LeftShift(&C[11][4], &C[12][4]);
	LeftShift(&C[12][0], &C[12][0]);
	LeftShift(&C[12][4], &C[12][4]);

	/* C14 */
	LeftShift(&C[12][0], &C[13][0]);
	LeftShift(&C[12][4], &C[13][4]);
	LeftShift(&C[13][0], &C[13][0]);
	LeftShift(&C[13][4], &C[13][4]);

	/* C15 */
	LeftShift(&C[13][0], &C[14][0]);
	LeftShift(&C[13][4], &C[14][4]);
	LeftShift(&C[14][0], &C[14][0]);
	LeftShift(&C[14][4], &C[14][4]);

	/* C16 */
	LeftShift(&C[14][0], &C[15][0]);
	LeftShift(&C[14][4], &C[15][4]);

	/* 48-bits (Permuted Choice 2) */
	for(i = 0; i < 16; ++i){
		if(C[i][1] &   2) K[i][0] |= 128;
		if(C[i][2] &  32) K[i][0] |=  64;
		if(C[i][1] &  16) K[i][0] |=  32;
		if(C[i][3] &  32) K[i][0] |=  16;   /* First byte */
		if(C[i][0] & 128) K[i][0] |=   8;
		if(C[i][0] &   8) K[i][0] |=   4;

		if(C[i][0] &  32) K[i][1] |= 128;
		if(C[i][3] &   2) K[i][1] |=  64;
		if(C[i][2] & 128) K[i][1] |=  32;
		if(C[i][0] &   4) K[i][1] |=  16;   /* Second byte */
		if(C[i][2] &   2) K[i][1] |=   8;
		if(C[i][1] &  32) K[i][1] |=   4;

		if(C[i][3] &  64) K[i][2] |= 128;
		if(C[i][2] &   8) K[i][2] |=  64;
		if(C[i][1] &   8) K[i][2] |=  32;
		if(C[i][0] &  16) K[i][2] |=  16;   /* Third byte */
		if(C[i][3] &   8) K[i][2] |=   8;
		if(C[i][1] & 128) K[i][2] |=   4;

		if(C[i][2] &  64) K[i][3] |= 128;
		if(C[i][0] &   2) K[i][3] |=  64;
		if(C[i][3] &   4) K[i][3] |=  32;
		if(C[i][2] &   4) K[i][3] |=  16;   /* Fourth byte */
		if(C[i][1] &   4) K[i][3] |=   8;
		if(C[i][0] &  64) K[i][3] |=   4;

		if(C[i][5] &   4) K[i][4] |= 128;
		if(C[i][7] &  32) K[i][4] |=  64;
		if(C[i][4] &  32) K[i][4] |=  32;
		if(C[i][5] &  64) K[i][4] |=  16;   /* Fifth byte */
		if(C[i][6] &   8) K[i][4] |=   8;
		if(C[i][7] &   4) K[i][4] |=   4;

		if(C[i][4] &  64) K[i][5] |= 128;
		if(C[i][5] &   8) K[i][5] |=  64;
		if(C[i][7] &  64) K[i][5] |=  32;
		if(C[i][6] &  32) K[i][5] |=  16;   /* Sixth byte */
		if(C[i][4] &   8) K[i][5] |=   8;
		if(C[i][6] &   4) K[i][5] |=   4;

		if(C[i][6] &  64) K[i][6] |= 128;
		if(C[i][6] &   2) K[i][6] |=  64;
		if(C[i][5] &  16) K[i][6] |=  32;
		if(C[i][7] &   2) K[i][6] |=  16;   /* Seventh byte */
		if(C[i][4] &   4) K[i][6] |=   8;
		if(C[i][7] &  16) K[i][6] |=   4;

		if(C[i][6] &  16) K[i][7] |= 128;
		if(C[i][5] &   2) K[i][7] |=  64;
		if(C[i][7] & 128) K[i][7] |=  32;
		if(C[i][5] & 128) K[i][7] |=  16;   /* Eighth byte */
		if(C[i][4] & 128) K[i][7] |=   8;
		if(C[i][4] &  16) K[i][7] |=   4;
	}

	/* IP */
	L[0][0] = L[0][1] = L[0][2] = L[0][3] =
	R[0][0] = R[0][1] = R[0][2] = R[0][3] = 0;

	if(Message[7] & 64) L[0][0] |= 128;
	if(Message[6] & 64) L[0][0] |=  64;
	if(Message[5] & 64) L[0][0] |=  32;
	if(Message[4] & 64) L[0][0] |=  16;
	if(Message[3] & 64) L[0][0] |=   8;
	if(Message[2] & 64) L[0][0] |=   4;
	if(Message[1] & 64) L[0][0] |=   2;
	if(Message[0] & 64) L[0][0] |=   1;

	if(Message[7] & 16) L[0][1] |= 128;
	if(Message[6] & 16) L[0][1] |=  64;
	if(Message[5] & 16) L[0][1] |=  32;
	if(Message[4] & 16) L[0][1] |=  16;
	if(Message[3] & 16) L[0][1] |=   8;
	if(Message[2] & 16) L[0][1] |=   4;
	if(Message[1] & 16) L[0][1] |=   2;
	if(Message[0] & 16) L[0][1] |=   1;

	if(Message[7] & 4) L[0][2] |= 128;
	if(Message[6] & 4) L[0][2] |=  64;
	if(Message[5] & 4) L[0][2] |=  32;
	if(Message[4] & 4) L[0][2] |=  16;
	if(Message[3] & 4) L[0][2] |=   8;
	if(Message[2] & 4) L[0][2] |=   4;
	if(Message[1] & 4) L[0][2] |=   2;
	if(Message[0] & 4) L[0][2] |=   1;

	if(Message[7] & 1) L[0][3] |= 128;
	if(Message[6] & 1) L[0][3] |=  64;
	if(Message[5] & 1) L[0][3] |=  32;
	if(Message[4] & 1) L[0][3] |=  16;
	if(Message[3] & 1) L[0][3] |=   8;
	if(Message[2] & 1) L[0][3] |=   4;
	if(Message[1] & 1) L[0][3] |=   2;
	if(Message[0] & 1) L[0][3] |=   1;

	if(Message[7] & 128) R[0][0] |= 128;
	if(Message[6] & 128) R[0][0] |=  64;
	if(Message[5] & 128) R[0][0] |=  32;
	if(Message[4] & 128) R[0][0] |=  16;
	if(Message[3] & 128) R[0][0] |=   8;
	if(Message[2] & 128) R[0][0] |=   4;
	if(Message[1] & 128) R[0][0] |=   2;
	if(Message[0] & 128) R[0][0] |=   1;

	if(Message[7] & 32) R[0][1] |= 128;
	if(Message[6] & 32) R[0][1] |=  64;
	if(Message[5] & 32) R[0][1] |=  32;
	if(Message[4] & 32) R[0][1] |=  16;
	if(Message[3] & 32) R[0][1] |=   8;
	if(Message[2] & 32) R[0][1] |=   4;
	if(Message[1] & 32) R[0][1] |=   2;
	if(Message[0] & 32) R[0][1] |=   1;

	if(Message[7] & 8) R[0][2] |= 128;
	if(Message[6] & 8) R[0][2] |=  64;
	if(Message[5] & 8) R[0][2] |=  32;
	if(Message[4] & 8) R[0][2] |=  16;
	if(Message[3] & 8) R[0][2] |=   8;
	if(Message[2] & 8) R[0][2] |=   4;
	if(Message[1] & 8) R[0][2] |=   2;
	if(Message[0] & 8) R[0][2] |=   1;

	if(Message[7] & 2) R[0][3] |= 128;
	if(Message[6] & 2) R[0][3] |=  64;
	if(Message[5] & 2) R[0][3] |=  32;
	if(Message[4] & 2) R[0][3] |=  16;
	if(Message[3] & 2) R[0][3] |=   8;
	if(Message[2] & 2) R[0][3] |=   4;
	if(Message[1] & 2) R[0][3] |=   2;
	if(Message[0] & 2) R[0][3] |=   1;

	if(encipher){
		for(i = 1; i < 16; ++i){
			L[i][0] = R[i-1][0];
			L[i][1] = R[i-1][1];
			L[i][2] = R[i-1][2];
			L[i][3] = R[i-1][3];

			ExpandXor(K[i-1], R[i-1], K[i-1]);
			CompactPermuteXor(K[i-1], L[i-1], R[i]);
		}

		L[15][0] = R[15][0];
		L[15][1] = R[15][1];
		L[15][2] = R[15][2];
		L[15][3] = R[15][3];

		ExpandXor(K[15], R[15], K[15]);
		CompactPermuteXor(K[15], R[14], R[15]);
	}else{
		for(i = 1; i < 16; ++i){
			L[i][0] = R[i-1][0];
			L[i][1] = R[i-1][1];
			L[i][2] = R[i-1][2];
			L[i][3] = R[i-1][3];

			ExpandXor(K[16-i], R[i-1], K[16-i]);
			CompactPermuteXor(K[16-i], L[i-1], R[i]);
		}

		L[15][0] = R[15][0];
		L[15][1] = R[15][1];
		L[15][2] = R[15][2];
		L[15][3] = R[15][3];

		ExpandXor(K[0], R[15], K[0]);
		CompactPermuteXor(K[0], R[14], R[15]);
	}

	/* IP -1 */
	Output[0] = Output[1] = Output[2] = Output[3] = Output[4] = Output[5] = Output[6] = Output[7] = 0;

	if(L[15][0] & 1) Output[0] |= 128;
	if(R[15][0] & 1) Output[0] |=  64;
	if(L[15][1] & 1) Output[0] |=  32;
	if(R[15][1] & 1) Output[0] |=  16;
	if(L[15][2] & 1) Output[0] |=   8;
	if(R[15][2] & 1) Output[0] |=   4;
	if(L[15][3] & 1) Output[0] |=   2;
	if(R[15][3] & 1) Output[0] |=   1;

	if(L[15][0] & 2) Output[1] |= 128;
	if(R[15][0] & 2) Output[1] |=  64;
	if(L[15][1] & 2) Output[1] |=  32;
	if(R[15][1] & 2) Output[1] |=  16;
	if(L[15][2] & 2) Output[1] |=   8;
	if(R[15][2] & 2) Output[1] |=   4;
	if(L[15][3] & 2) Output[1] |=   2;
	if(R[15][3] & 2) Output[1] |=   1;

	if(L[15][0] & 4) Output[2] |= 128;
	if(R[15][0] & 4) Output[2] |=  64;
	if(L[15][1] & 4) Output[2] |=  32;
	if(R[15][1] & 4) Output[2] |=  16;
	if(L[15][2] & 4) Output[2] |=   8;
	if(R[15][2] & 4) Output[2] |=   4;
	if(L[15][3] & 4) Output[2] |=   2;
	if(R[15][3] & 4) Output[2] |=   1;

	if(L[15][0] & 8) Output[3] |= 128;
	if(R[15][0] & 8) Output[3] |=  64;
	if(L[15][1] & 8) Output[3] |=  32;
	if(R[15][1] & 8) Output[3] |=  16;
	if(L[15][2] & 8) Output[3] |=   8;
	if(R[15][2] & 8) Output[3] |=   4;
	if(L[15][3] & 8) Output[3] |=   2;
	if(R[15][3] & 8) Output[3] |=   1;

	if(L[15][0] & 16) Output[4] |= 128;
	if(R[15][0] & 16) Output[4] |=  64;
	if(L[15][1] & 16) Output[4] |=  32;
	if(R[15][1] & 16) Output[4] |=  16;
	if(L[15][2] & 16) Output[4] |=   8;
	if(R[15][2] & 16) Output[4] |=   4;
	if(L[15][3] & 16) Output[4] |=   2;
	if(R[15][3] & 16) Output[4] |=   1;

	if(L[15][0] & 32) Output[5] |= 128;
	if(R[15][0] & 32) Output[5] |=  64;
	if(L[15][1] & 32) Output[5] |=  32;
	if(R[15][1] & 32) Output[5] |=  16;
	if(L[15][2] & 32) Output[5] |=   8;
	if(R[15][2] & 32) Output[5] |=   4;
	if(L[15][3] & 32) Output[5] |=   2;
	if(R[15][3] & 32) Output[5] |=   1;

	if(L[15][0] & 64) Output[6] |= 128;
	if(R[15][0] & 64) Output[6] |=  64;
	if(L[15][1] & 64) Output[6] |=  32;
	if(R[15][1] & 64) Output[6] |=  16;
	if(L[15][2] & 64) Output[6] |=   8;
	if(R[15][2] & 64) Output[6] |=   4;
	if(L[15][3] & 64) Output[6] |=   2;
	if(R[15][3] & 64) Output[6] |=   1;

	if(L[15][0] & 128) Output[7] |= 128;
	if(R[15][0] & 128) Output[7] |=  64;
	if(L[15][1] & 128) Output[7] |=  32;
	if(R[15][1] & 128) Output[7] |=  16;
	if(L[15][2] & 128) Output[7] |=   8;
	if(R[15][2] & 128) Output[7] |=   4;
	if(L[15][3] & 128) Output[7] |=   2;
	if(R[15][3] & 128) Output[7] |=   1;
}

//3des 就是搞三次des
void TripleDES(const unsigned char Message[8], const char Key[24], unsigned char Output[8], int encipher){
	if(encipher){
		DES(Message, &Key[0],  Output, 1);
		DES(Message, &Key[8],  Output, 0);
		DES(Message, &Key[16], Output, 1);
	}else{
		DES(Message, &Key[16], Output, 0);
		DES(Message, &Key[8],  Output, 1);
		DES(Message, &Key[0],  Output, 0);
	}
}
