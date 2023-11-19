#include <assert.h>
#include <string.h>



//arr_stack_mini 说明:
/*
	* 由于#define 宏只能做简单的字符替换, 没办法区分'.'/'->',
		因此, arr_stack_mini 基本上只能使用'->' 指针模式;
		需要在栈上创建arr_stack_mini, 请使用: alloca()
		需要在堆上创建arr_stack_mini, 请使用: malloc()/new()

	* 没有必要搞批量操作, 更没有必要搞批量copy cache-buffer, 
		因为你拷贝进copy cache-buffer的时候, 本身就消耗了一个for 循环了, 很不划算, 
		就算后面直接一次性memcpy, 也抵消不了性能消耗;
		还不如直接, 来一个fd, 直接压入一个fd, 这样更方便, 程序更简单(类似于无锁队列优于批量加锁);

	* arr_stack_mini不支持优先插入, 如果优先插入, 需要整体元素后移;
		要实现优先插入后移, 必须有两个buf 缓冲区, 直接使用memcpy 进行buf拷贝, 避开for() 循环后移, 性能更好;
		(但这样做, 面对游标为-1 时, 也极难处理, 不推荐使用)

	* 一定保留assert() 方便调试

	* 没有泛型编程, as 的类型不能随意变动(只能手动更改, 不能动态更改), 不方便stl 容器之间内嵌!!
		所以, 搞算法必须使用c++, c语言只适合做基础建设, 不适合做大规模算法迭代(弱项)

	* 插入出错, 根本没有报错, 所以最好你确保容器永远不会因为元素已满, 而插不进元素;
		其实开启__assertEx() 断言, 很有意思, 很好用;
*/



#ifndef __arr_stack_mini
	#define __arr_stack_mini (1)

	//定义as 容器的数据类型(一般支持: int, double, void* )
	//#define as_type unsigned int
	//#define as_type unsigned double
	#define as_type void*


	//开启优先插入(一般情况下, 不允许开启优先插入)
	#define __as_prio (1)



	//#define pos_max (8192)[dead]
	#define pos_min (-1)			//不对外公开
	typedef struct as_info{
		int pos_cur;						//不对外公开(外部永远不要访问这个元素)
		//int cur_size;					//应该调用as_cur_size()
		int pos_max;						//不对外公开
		//int as_size_max;			//应该调用as_size_max()
		as_type* buf;						//数据缓冲区
		#ifdef __as_prio
			as_type* buf_bak;			//for 优先插入
			as_type* swap_tmp;		//交换buf时用到的tmp指针(为了实现'全宏定义化',tmp变量也加进来了)
		#endif
	}as_t;



	#ifdef __as_prio
		//初始化栈
		#define as_init(pas, n) { pas->buf=malloc(n * sizeof(as_type)); \
			if(pas->buf != NULL) { pas->buf_bak=malloc(n * sizeof(as_type)); \
				if(pas->buf_bak != NULL) { pas->pos_cur=pos_min; pas->pos_max=n-1; } \
				else{ free(pas->buf); } } }

		//摧毁栈
		#define as_destroy(pas) { free(pas->buf); free(pas->buf_bak); pas->pos_cur=pos_min; }

				//优先从栈头插入一个元素(memcpy 拷贝完毕之后, 就交换指针)
				#define as_push_prio1(pas, val) { \
					if(pas->pos_cur <= pas->pos_max) { \
						if(pas->pos_cur == pos_min) \
							pas->buf[++pas->pos_cur]=(as_type)val; \
						else{ \
							pas->buf_bak[0]=(as_type)val; \
							memcpy(pas->buf_bak[1], pas->buf, (++pas->pos_cur)*sizeof(as_type)); \
							pas->swap_tmp=pas->buf_bak; pas->buf_bak=pas->buf; pas->buf=pas->swap_tmp; } } \
					else assert(0); }

				//优先从栈头插入n个元素
				#define as_push_prioN(pas, pval, N) { \
					if(pas->pos_cur+N <= pas->pos_max) { \
						memcpy(pas->buf_bak, pval, N*sizeof(as_type)); \
						memcpy(pas->buf_bak[N-1], pas->buf, (++pas->pos_cur)*sizeof(as_type)); \
						pas->swap_tmp=pas->buf_bak; pas->buf_bak=pas->buf; pas->buf=pas->swap_tmp; \
						pas->pos_cur=(pas->pos_cur-1)+N; } \
					else assert(0); }

				//优先从栈头弹出一个元素(没办法用宏来实现, 只能用inline 内联函数实现)
				//优先从栈头弹出n个元素(没办法用宏来实现, 只能用inline 内联函数实现)
	#else
		#define as_init(pas, n) { pas->buf=malloc(n * sizeof(as_type)); if(pas->buf != NULL) { pas->pos_cur=pos_min; pas->pos_max=n-1; } }
		#define as_destroy(pas) { free(pas->buf); pas->pos_cur=pos_min; }
	#endif



	//****************************共有的部分****************************

	//压一个元素入栈[ val 或会有编译器警告, 无需在意, 这个警告是必然会出现的, 类型与值指定错乱, 没办法避免 ]
	#define as_push(pas, val) { \
		if(pas->pos_cur <= pas->pos_max) \
			pas->buf[++pas->pos_cur]=val; \
		else assert(0); }

	//弹一个元素出栈(先进后出)
	#define as_pop(pas, val) { \
		if(pas->pos_cur > pos_min) \
			val=pas->buf[pas->pos_cur--]; \
		else assert(0); }

	//批量插入n 个元素(栈尾插入)
	#define as_pushN(pas, pval, N) { \
		if(pas->pos_cur+N <= pas->pos_max){\
			memcpy(pas->buf, pval, N*sizeof(as_type)); pas->pos_cur+=N; } \
		else assert(0); }

	//批量弹空栈
	#define as_popALL(pas, pval) { \
		if(pas->pos_cur > pos_min){ \
			memcpy(pval, pas->buf, (++pas->pos_cur)*sizeof(as_type));pas->pos_cur=pos_min;} \
		else assert(0); }

	//返回栈当前的元素个数
	#define as_cur_size(pas) (pas->pos_cur+1)

	//返回栈元素个数的max
	#define as_size_max(pas) (pas->pos_max+1)

	//返回栈当前的pos(私有数据, 永远不可访问, 否则访问到-1 就很麻烦了)
	//#define as_pos_cur(pas) (pas->pos_cur)
	//****************************共有的部分****************************

#endif
