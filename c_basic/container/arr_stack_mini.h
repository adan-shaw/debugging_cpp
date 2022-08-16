#include <assert.h>



//没有泛型编程, as 的类型不能随意变动(只能手动更改, 不能动态更改), 不方便stl 容器之间内嵌!!
//所以, 搞算法必须使用c++, c语言只适合做基础建设, 不适合做大规模算法迭代(弱项)
#define as_type unsigned int
#define as_buf_max (8192)
#define as_buf_min (0)



//开启批量操作API
#define __as_pushN (1)



typedef struct as_info{
	unsigned int cur_size;
	#ifdef __as_pushN
	#define as_cache_max (128)
	unsigned int errno;
	as_type cache[as_cache_max];
	#endif
	as_type buf[as_buf_max];
}as_t;

//伪造有返回的assert(), 欺骗编译器, 让三木运算符有返回值, 实现全stack 都用宏操作
#ifndef __assertEx
	#define __assertEx (1)
	static as_type assertEx(void){
		assert(0);
		return 0;
	}
#endif


//初始化栈
#define as_init(pas) { pas->cur_size = 0; }

//压一个元素入栈
#define as_push(pas, val) { if(pas->cur_size<as_buf_max) pas->buf[pas->cur_size++] = val; }

//弹一个元素出栈(先进后出)
#define as_pop(pas) (pas->cur_size>as_buf_min ? pas->buf[--pas->cur_size] : assertEx())

//弹元素出栈(先进先出) -- 本容器不支持'先进先出', 如果是频繁先进先出, 请用arr_queue;
//										 如果是批量'先进先出', 一次弹空整个栈的, 可以直接操作as.cache[] 数组;
//										 操作完之后, 再次执行as.as_init(); 即可

//返回栈当前的元素个数
#define as_cur_size(pas) (pas->cur_size)
