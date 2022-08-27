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
#define memsetEx2(p,c,size,row_max,row_count) ( memset(p,c,size*row_max*row_count) )

#define memcpyEx2(pdest,psrc,size,row_max,row_count) ( memcpy(pdest,psrc,size*row_max*row_count) )


//对三维数组进行优化[体积=row_max*row_count*height]
#define memsetEx3(p,c,size,row_max,row_count,height) ( memset(p,c,size*row_max*row_count*height) )

#define memcpyEx3(pdest,psrc,size,row_max,row_count,height) ( memcpy(pdest,psrc,size*row_max*row_count*height) )

//对四维数组进行优化[体积*时间=row_max*row_count*height*t] -- 现在'宇宙学家'将时间看作第四维
#define memsetEx4(p,c,size,row_max,row_count,height,t) ( memset(p,c,size*row_max*row_count*height*t) )

#define memcpyEx4(pdest,psrc,size,row_max,row_count,height) ( memcpy(pdest,psrc,size*row_max*row_count*height) )


//对五维数组进行优化[未知,还未解释出5维空间具体是什么, passed]

//空间*时间 = 平衡对称宇宙? no !! 宇宙内部本身, 就有大量时间差的存在!!
/*
	黑洞内外就是时间差, 不同星系的时间差是不一样的, 
	有的星系, 自转速度快, 时间就快, 有的星系自转速度慢, 时间就慢;
	宇宙内有那么多星系, 每个星系中心的黑洞, 大小都不一样, 引力也不一样, 宇宙围绕的速度自然也不一样, 时间自然也就不一样(速度可以改变时间);

	所以, 五维空间具体是什么, 现在应该还没有定论, passed;



	(银河系中心其实就是一个巨型黑洞, 在不断吸纳银河系中的物质, 
	然后碾压(将原子核压碎), 激发(以各种极端粒子射线的方式, 将电子抛撒出去, 形成强电子束射线, 粒子束射线, 以释放能量);
	黑洞内部如果是高温的, 但也没有传播介质, 可以将热量传播出去, 连光都没办法逃脱, 任何介质都会被直接吸入黑洞;
	但也证明了一点:
		粒子束的速度, 绝对比光强, 比光更快, 否则粒子束也没办法逃离黑洞, 抛撒出去, 而是照样被黑洞吞噬;
		所以, 未来的宇宙航行时代, 通信工具会变成粒子束!!
		不同星系之间通信, 需要一种比光还要快的传播手段, 必然是粒子束!!
		人类目前也已经会用光纤了, 不过可惜, 没有光计算机or 量子计算机, 光纤调解器还是电能驱动的, 光纤技术还未到达顶点, 
		当人类会用光来做计算机的时候, 光纤或者就成为瓶颈了;
		那时候, 星系之间的通信, 会用粒子束!!
		跟黑洞抛撒粒子束一样, 激发这种粒子束, 必然需要巨大的能量去驱动, 星系之间发电报, 估计耗能很大!!
		(现在卫星跟地面雷达通信, 耗电量也不小)
*/

//对n维数组进行优化[必须是连续内存, 才能用]
void* memsetExn(void* p, int c, unsigned int size, unsigned short val_count, ...){
	va_list valist;
	unsigned int len,tmp;
	unsigned short i;

	va_start(valist, val_count);	//必须输入参数个数, 才能初始化valistv

	for(i=0; i<val_count; i++){
		tmp = va_arg(valist, unsigned int);
		len += tmp;
	}

	va_end(valist);								//释放valist
	return memset(p,c,len*size);
}

void* memcpyExn(void* pdest, void* psrc, unsigned int size, unsigned short val_count, ...){
	va_list valist;
	unsigned int len,tmp;
	unsigned short i;

	va_start(valist, val_count);

	for(i=0; i<val_count; i++){
		tmp = va_arg(valist, unsigned int);
		len += tmp;
	}

	va_end(valist);
	return memcpy(pdest,psrc,len*size);
}
