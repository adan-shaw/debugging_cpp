#include <string.h>
#include <stdarg.h>

//malloc分配的内存空间是连续的吗?
/*
	1.linux内核管理内存空间的分配,
		所有程序对内存空间的申请和其他操作,最终都会交给内核来管理.

	2.linux实现的是"虚拟内存系统",
		对用户而言,所有内存都是虚拟的,
		也就是说程序并不是直接运行在物理内存上,
		而是运行在虚拟内存上,然后由虚拟内存转换到物理内存.

	3.linux将所有的内存都以页为单位进行划分,通常每一页是4KB;

	4.在对虚拟内存地址到物理内存地址进行转换时,内核会对地址的正确性进行检查,
		如果地址是合法的,内核就会提供对应的物理内存分页;
		如果是申请内存空间,内核就会检查空余的物理内存分页,并加以分配,
		如果物理内存空间不足,内核会拒绝此次申请;

	5.使用malloc分配的内存空间在虚拟地址空间上是连续的,
		但转换到物理内存空间上有可能是不连续的,
		因为有可能相邻的两个字节是在不同的物理分页上;
*/


//连续内存的存放概念[必须是连续内存, 才能是连续存放的]:
/*
	二维数组变量的实际存放方式, 是压扁为一维数组连续存放的;
	三维数组变量的实际存放方式, 是压扁为一维数组连续存放的;
	N维数组变量的实际存放方式, 是压扁为一维数组连续存放的;

	声明二维数组a[n][n],则可以用memset(a,0x3f,sizeof(a));
	因为声明的二维数组存储单元是连续的;

	动态声明的二维数组指针int **q,只能够每次获取q[i]后对每行进行初始化,
	因为指针对于每行的存储单元不连续;

	解决办法:
		* 使用局部变量, 静态变量, 来创建连续内存, 而不是使用指针!!

		* 如果一定要使用指针, 请在main() 函数之前, 执行malloc() 申请堆内存, 
			因为这时候申请的堆内存, 正常情况下是连续内存;
*/


//指针的基础概念:
/*
	* 所有指针都不知道自己的终止位置在哪里的, void*, char*, int* 都一样, 超过终止位就指针越界;

	* void* 指针不允许位移, 也就是不允许void* p++;

	* 由于不同指针的位移量不一样, 
		char* 位移1byte, int* 位移4 byte, 
		行指针位移一行[具体一行有多少个byte, 行byte_max=sizof(type)*line_max]
*/


//本模块利用宏, 对二维数组, 三维数组初始化, 拷贝进行优化:
/*
	* row_max 必须大于0, 必须为二维数组单行line_max;
	* row_count 必须大于0[一般是unsigned int 类型], 必须为二维数组的列数;
	* height 必须大于0, 必须为三维数组的height(height*面积=立方);
*/


//memset, memcpy 使用注意事项
/*
	由于memset, memcpy 的位移字节, 是1byte;
	所以对整形int, double, float 等数组进行初始化的时候, 通常只能初始化为0, 全1初始化=256;
	如果要初始化为固定数值, 需要重写优化函数;(思路也是memcpy(), 用途不大, 不写了)
*/



//对二维数组进行优化[面积=row_max*row_count]
#define memsetEx2(p,c,row_max,row_count) ( memset(p,c,row_max*row_count) )

#define memcpyEx2(pdest,psrc,row_max,row_count) ( memcpy(pdest,psrc,row_max*row_count) )


//对三维数组进行优化[体积=row_max*row_count*height]
#define memsetEx3(p,c,row_max,row_count,height) ( memset(p,c,row_max*row_count*height) )

#define memcpyEx3(pdest,psrc,row_max,row_count,height) ( memcpy(pdest,psrc,row_max*row_count*height) )


//对n维数组进行优化[必须是连续内存, 才能用]
void* memsetExn(void* p, int c, unsigned short val_count, ...){
	va_list valist;
	unsigned int len,tmp;
	unsigned short i;

	va_start(valist, val_count);	//必须输入参数个数, 才能初始化valistv

	for(i=0; i<val_count; i++){
		tmp = va_arg(valist, unsigned int);
		len += tmp;
	}

	va_end(valist);								//释放valist
	return memset(p,c,len);
}

void* memcpyExn(void* pdest, void* psrc, unsigned short val_count, ...){
	va_list valist;
	unsigned int len,tmp;
	unsigned short i;

	va_start(valist, val_count);

	for(i=0; i<val_count; i++){
		tmp = va_arg(valist, unsigned int);
		len += tmp;
	}

	va_end(valist);
	return memcpy(pdest,psrc,len);
}
