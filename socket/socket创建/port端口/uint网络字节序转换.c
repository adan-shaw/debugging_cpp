//编译:
//		gcc -g3 ./uint网络字节序转换.c -o x 



#include <arpa/inet.h>
#include <stdio.h>



//前置声明:
/*
	网络字节序, 仅对2 byte 以上的int 整形有效, 对于8 bit = 1 byte 的int 无效;
	char      = 8 bit  = 1 byte = 0-255 整形(不受网络字节序限制)
	short     = 16 bit = 2 byte = [0-65536]
	int       = 32 bit = 4 byte = [略, 值域不谈]
	long long = 64 bit = 8 byte = [略, 值域不谈]

	因此, 填充tcp/ip 时, 看到8 bit, 1 byte 的整形数据, 是不需要使用'网络字节序'转换函数的;
	任何大于1 byte 的整形数据, 都需要使用'网络字节序'转换函数, 这是一个界限;
*/



//1.'网络字节序'转换函数list(只有unsigned int 数据需要转序,char/int不需要):
/*
	uint32_t htonl(uint32_t);		//host字节序->net字节序  unsigned long int
	uint16_t htons(uint16_t);		//host字节序->net字节序  unsigned short
	uint32_t ntohl(uint32_t);		//net字节序 ->host字节序 unsigned long int
	uint16_t ntohs(uint16_t);		//net字节序 ->host字节序 unsigned short

	struct sockaddr_in 中的sin_port 为uint16_t 整形;
*/



//2.为什么需要'网络字节序'转换函数:
/*
	不同的CPU, 不同的操作系统,
	unsigned int数据在网络传输后, 可能会发生偏差(这并不是传输出错, 而是操作系统识别出错)
	因此, 需要'网络字节序'转换函数;

	不同的CPU, 字节序可能是不同的;
	不同的操作系统, 字节序也可能是不同的, 参见下表:
		处理器			操作系统	字节排序
		Alpha			全部			Little endian
		HP-PA			NT			Little endian
		HP-PA			UNIX		Big endian
		Intelx86	全部			Little endian

	power8 内存高位=数的高位, 内存低位=数的低位
	intel  内存高位=数的低位, 内存低位=数的高位
*/



//3.如何发送安全数据:
/*
	如果你直接发送'unsigned int数据'到一个异构系统里面, 会出现数据错乱的;
	你需要'网络字节序'转换函数, 来维持'unsigned int数据'的准确性;
	另外:
		字符串数据块, 就不需要'网络字节序'转换函数了;
		所以internet 都以byte 文本为基础, 传输文本数据;
		(xml 和json 都是以字符串来传输&解析的)
*/



int main(void){
	uint32_t u32 = 1234;										//转换前的数据
	uint16_t u16 = 1234;
	uint32_t u32_ret = 0;										//转换后的数据(可能会出现异常数据, 但这种异常是可逆的就行了)
	uint16_t u16_ret = 0;



	u32_ret = htonl(u32);										//1.'主机序'转"网络字节序"
	u16_ret = htons(u16);
	printf("\n1.'主机序'转\"网络字节序\"[转换]:\n\tu32=%d, u16=%d\n\n", u32_ret, u16_ret);


	u32_ret = ntohl(u32_ret);								//2."网络字节序"转'主机序'
	u16_ret = ntohs(u16_ret);
	printf("2.\"网络字节序\"转'主机序'[还原]:\n\tu32=%d, u16=%d\n\n\n", u32_ret, u16_ret);


	if(u32_ret != u32 || u16_ret != u16)		//3.检验能不能还原为'主机序'的初始化值
		printf("互转还原失败 !!\n");
	else
		printf("互转还原成功 !!\n");
	return 0;
}







