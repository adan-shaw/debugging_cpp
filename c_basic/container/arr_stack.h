#include <stdbool.h>



//没有泛型编程, as 的类型不能随意变动(只能手动更改, 不能动态更改), 不方便stl 容器之间内嵌!!
//所以, 搞算法必须使用c++, c语言只适合做基础建设, 不适合做大规模算法迭代(弱项)
#define as_type unsigned int
#define as_buf_max (8192)
#define as_buf_min (0)
#define as_cache_max (128)



typedef struct as_info{
	unsigned int cur_size;
	unsigned int errno;
	as_type buf[as_buf_max];
	as_type cache[as_cache_max];
}as_t;


//初始化栈
void as_init(as_t* pas);

//压一个元素入栈
void as_push(as_t* pas, as_type val);

//弹一个元素出栈(先进后出)
as_type as_pop(as_t* pas);

//弹元素出栈(先进先出) -- 本容器不支持'先进先出', 如果是频繁先进先出, 请用arr_queue;
//										 如果是批量'先进先出', 一次弹空整个栈的, 可以直接操作as.cache[] 数组;
//										 操作完之后, 再次执行as.as_init(); 即可

//返回栈当前的元素个数
#define as_cur_size(pas) (pas->cur_size)


//压栈n 个(需要先将N 个元素放进pas->cache 缓冲区, 然后传入n 元素个数)
bool as_pushN(as_t* pas, unsigned int N);


//弹栈n 个(传入n 元素个数, 将N 个元素弹入pas->cache 缓冲区, 然后自己取走)
bool as_popN(as_t* pas, unsigned int N);
