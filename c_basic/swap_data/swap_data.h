//2 个变量交换'4 字节32bit * N'的数据(弃用)
/*
	(被交换的数据, 必须是对齐数据: (unsigned int* 指针)*N, N 至少大于1;)
	少于4 字节32bit, 会出现乱码, 扰乱数据的准确性!!
	非对齐数据'4 字节32bit * N', 同样会扰乱数据的准确性
	(交换后, buf头部/尾部, 会有少量错乱数据, 影响交换操作的准确性)
*/
static inline void sw_4byteN(unsigned int *pa, unsigned int *pb, unsigned int N){
	//N 少于1 不会执行交换, 直接退出for{}
	for(; N>0; N--,pa++,pb++){
		*pa = (*pa)^(*pb);
		*pb = (*pa)^(*pb);
		*pa = (*pa)^(*pb);
	}
	return;
}

//2 个变量交换'1 字节8bit * N'的数据(启用)[内联函数有一定消耗, 频繁使用请考虑去除inline]
static inline void sw_1byteN(unsigned char *pa, unsigned char *pb, unsigned int N){
	for(; N>0; N--,pa++,pb++){
		*pa = (*pa)^(*pb);
		*pb = (*pa)^(*pb);
		*pa = (*pa)^(*pb);
	}
	return;
}



//
// 交换2 个number 数据()
//
//交换宏(优点: 支持所有number, 整形浮点都可以用, 性能最好; 
//			缺点: 数值大容易溢出, 你必须保证x+y 不会溢出值域, 不太安全, 慎用!!)
#define SWAP_NUM_OLD(x, y) { (*x)=(*x)+(*y);(*y)=(*x)-(*y);(*x)=(*x)-(*y); }

//(整形/浮点)通用number交换宏(需要tmp 变量, 做完美交换)
#define SWAP_NUM(a,b,tmp) {(tmp)=(a); (a)=(b); (b)=(tmp)}



/*
交换两个常规变量的数据sw_4byteN()函数的设计纲要:
	32/64 bit 系统差异:
		指针无论是int*,char*,void* 都一样, 只会根据系统32/64bit 变化而变化, 
		你在设计时, 没办法避免这样问题,
		但由于int 类型, 是永恒4 字节32bit 的!!
		char 类型, 也是永恒1 字节8bit 的
		因此char* 指针, 每次只移动1 字节8bit.
		(即便在64bit 系统中, char* 指针每次后移, 也是1 字节8bit)

		因此, 可以选定unsigned char* 为'交换指针', 交换粒度是: 1 字节8bit
		每次交换的数据量, 必须是:
				8bit * N [8bit 的N 倍, 最小粒度=8bit]
				1byte * N


	两个变量也需要相等的buf空间, 
	或者'两个变量中,最小的buf空间' > '交换空间量max'


	宏定义方法(失败):
		宏一般情况下, 只做常量定义or 公式定义, 不能进行变量运算的,
		所以宏定义绝对不能执行循环(循环体必然有变量控制exit), 不能定义变量!!
		所以复杂操作, 还是inline 函数好用.
*/



//仅c++ 支持在头文件中使用inline 函数(c 编译器不支持)
#ifdef __cplusplus
	//2 个变量交换'4 字节32bit * N'的数据(弃用)
	/*
		(被交换的数据, 必须是对齐数据: (unsigned int* 指针)*N, N 至少大于1;)
		少于4 字节32bit, 会出现乱码, 扰乱数据的准确性!!
		非对齐数据'4 字节32bit * N', 同样会扰乱数据的准确性
		(交换后, buf头部/尾部, 会有少量错乱数据, 影响交换操作的准确性)
	*/
	inline void sw_4byteN(unsigned int *pa, unsigned int *pb, unsigned int N){
		//N 少于1 不会执行交换, 直接退出for{}
		for(; N>0; N--,pa++,pb++){
			*pa = (*pa)^(*pb);
			*pb = (*pa)^(*pb);
			*pa = (*pa)^(*pb);
		}
		return;
	}

	//2 个变量交换'1 字节8bit * N'的数据(启用)[内联函数有一定消耗, 频繁使用请考虑去除inline]
	inline void sw_1byteN(unsigned char *pa, unsigned char *pb, unsigned int N){
		for(; N>0; N--,pa++,pb++){
			*pa = (*pa)^(*pb);
			*pb = (*pa)^(*pb);
			*pa = (*pa)^(*pb);
		}
		return;
	}
#endif
