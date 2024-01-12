/*
	//1.[用md5 防修改]
		这个也是固定的, 所有md5 计算出来的值, 都是一样的.
		只是防止修改, 如果是数据, 生成一个摘要, 防止修改.

	//2.[用md5 存储密码, 加密]
		如果是密码, 直接将密码加密成摘要, 将摘要存储起来.
		破解时, 需要很大的内存空间才能破解这串密码, 基本很多值都是固定不变的.

		这里将一些初始化的表, 全部写实, 性能高, 但是占用内存多.[]
*/


#ifdef __cplusplus
extern "C" {
#endif

#include <memory.h>


//打开'寄存器占用加速'开关
#define md5_c_register_speed_up (1)

//md5 '散列运算'操作宏前置声明
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }



//md5 计算中间结构体容器
typedef struct{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];
}MD5_CTX;



//常量表1(其实是MD5Transform()里面的block[64]的初始值, 同一种算法保持一致即可)
const unsigned char PADDING[]={
                           0x80,0x86,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x86,0x80};



//md5 'MD5_CTX' 计算中间结构体容器
void MD5Init(MD5_CTX *context){
	context->count[0] = 0;
	context->count[1] = 0;
	//一般可以修改这里的初始化值, 防止md5算法雷同
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
	//context->state[0] = 0x68452301;
	//context->state[1] = 0xEFCDAB89;
	//context->state[2] = 0x98BADCFE;
	//context->state[3] = 0x10425476;

	return;
}



void MD5Encode(unsigned char *output,const unsigned int *input,unsigned int len){
	#ifdef md5_c_register_speed_up
		register unsigned int i = 0,j = 0;
	#else
		unsigned int i = 0,j = 0;
	#endif
	while(j < len){
		output[j] = input[i] & 0xFF;
		output[j+1] = (input[i] >> 8) & 0xFF;
		output[j+2] = (input[i] >> 16) & 0xFF;
		output[j+3] = (input[i] >> 24) & 0xFF;
		i++;
		j+=4;
	}

	return;
}



void MD5Decode(unsigned int *output,const unsigned char *input,unsigned int len){
	#ifdef md5_c_register_speed_up
		register unsigned int i = 0,j = 0;
	#else
		unsigned int i = 0,j = 0;
	#endif
	while(j < len){
		output[i] = (input[j]) |
								(input[j+1] << 8) |
								(input[j+2] << 16) |
								(input[j+3] << 24);
		i++;
		j+=4;
	}

	return;
}



//MD5Transform() 可以修改Round 1-4 中的计算默认码
void MD5Transform(unsigned int state[4],const unsigned char block[64]){
	#ifdef md5_c_register_speed_up
		register unsigned int a = state[0];
		register unsigned int b = state[1];
		register unsigned int c = state[2];
		register unsigned int d = state[3];
	#else
		unsigned int a = state[0];
		unsigned int b = state[1];
		unsigned int c = state[2];
		unsigned int d = state[3];
	#endif
	unsigned int x[64];

	MD5Decode(x,block,64);

	//Round 1
	FF(a, b, c, d, x[ 0], 7, 0xd76aa478);  /* 1 */
	FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
	FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[ 4], 7, 0xf57c0faf);  /* 5 */
	FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[ 8], 7, 0x698098d8);  /* 9 */
	FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], 7, 0x6b901122);  /* 13 */
	FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */

	//Round 2
	GG(a, b, c, d, x[ 1], 5, 0xf61e2562);  /* 17 */
	GG(d, a, b, c, x[ 6], 9, 0xc040b340);  /* 18 */
	GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[ 5], 5, 0xd62f105d);  /* 21 */
	GG(d, a, b, c, x[10], 9,  0x2441453);  /* 22 */
	GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[ 9], 5, 0x21e1cde6);  /* 25 */
	GG(d, a, b, c, x[14], 9, 0xc33707d6);  /* 26 */
	GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], 5, 0xa9e3e905);  /* 29 */
	GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8);  /* 30 */
	GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */

	//Round 3
	HH(a, b, c, d, x[ 5], 4, 0xfffa3942);  /* 33 */
	HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[ 1], 4, 0xa4beea44);  /* 37 */
	HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], 4, 0x289b7ec6);  /* 41 */
	HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
	HH(a, b, c, d, x[ 9], 4, 0xd9d4d039);  /* 45 */
	HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */

	//Round 4
	II(a, b, c, d, x[ 0], 6, 0xf4292244);  /* 49 */
	II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], 6, 0x655b59c3);  /* 53 */
	II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[ 8], 6, 0x6fa87e4f);  /* 57 */
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[ 4], 6, 0xf7537e82);  /* 61 */
	II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	//第二种做法:
	/*
		如果你创建内存表, 存储这些变量实体, 的确是好看了, 但速度不会提升, 只是代码更精简了.
		下面的for() 循环, 如果没有表, 是不能用的, 你需要建两个表:
		constMove[] 常量移动表, constTable[] 常量表

		既然是常量, 直接宏替换也没问题, 但是内存容量会爆增加.
	*/
	/*
		const uint32_t constTable[64] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

		// r specifies the per-round shift amounts
		const uint32_t constMove[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                                  5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                                  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

		for(i=0;i<4;i++)
			for(j=0;j<4;j++){
				pF[i](a, b, c, d, x[k[i][4*j]], constMove[4*i],constTable[i][4*j]);
				pF[i](d, a, b, c, x[k[i][4*j+1]], constMove[4*i+1],constTable[i][4*j+1]);
				pF[i](c, d, a, b, x[k[i][4*j+2]], constMove[4*i+2],constTable[i][4*j+2]);
				pF[i](b, c, d, a, x[k[i][4*j+3]], constMove[4*i+3],constTable[i][4*j+3]);
			}
	*/
	return;
}



void MD5Update(MD5_CTX *context,const unsigned char *input,const unsigned int inputlen){
	#ifdef md5_c_register_speed_up
		register unsigned int i = 0,index = 0,partlen = 0;
	#else
		unsigned int i = 0,index = 0,partlen = 0;
	#endif

	index = (context->count[0] >> 3) & 0x3F;
	partlen = 64 - index;
	context->count[0] += inputlen << 3;
	if(context->count[0] < (inputlen << 3))
		context->count[1]++;
	context->count[1] += inputlen >> 29;

	if(inputlen >= partlen){
		memcpy(&context->buffer[index],input,partlen);
		MD5Transform(context->state,context->buffer);
		for(i = partlen;i+64 <= inputlen;i+=64)
			MD5Transform(context->state,&input[i]);
		index = 0;
	}
	else
		i = 0;

	memcpy(&context->buffer[index],&input[i],inputlen-i);

	return;
}


void MD5Final(MD5_CTX *context,unsigned char digest[16]){
	unsigned int index = 0,padlen = 0;
	unsigned char bits[8];

	index = (context->count[0] >> 3) & 0x3F;
	padlen = (index < 56)?(56-index):(120-index);

	MD5Encode(bits,context->count,8);
	MD5Update(context,PADDING,padlen);
	MD5Update(context,bits,8);
	MD5Encode(digest,context->state,16);

	return;
}



//出口函数(对const char*数据,计算md5校验数)
unsigned char* MD5(const unsigned char* data,unsigned int data_len,unsigned char* md5_result){
	MD5_CTX tmp;
	MD5Init(&tmp);
	//MD5Update(&tmp,data,strnlen((const char*)data,data_len));
	MD5Update(&tmp,data,data_len);//(自己保证字符串长度的准确性,确保不会溢出buf)
	MD5Final(&tmp,md5_result);

	return md5_result;
}



#ifdef __cplusplus
}
#endif

