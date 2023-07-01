//编译:
//		g++ -std=c++11 -g3 -pthread ./c++线程库与c11线程库的使用区别.cpp -o x 



//1.'c++线程库与c11线程库'的使用区别:
/*
	'c11线程库'主要是c 语言风格, 包含头文件就可以直接使用function 了, 例如:
	包含头文件:
		<stdatomic.h>
		<threads.h>
	调用函数:
		thrd_create()
		mtx_lock()

	'c++线程库'主要是c++ 语言风格, 包含头文件后, 还需要使用namespace 进行识别, 例如:
	包含头文件:
		<atomic>
		<thread>
	调用函数:
		std::thrd_create()
		std::mtx_lock()

	主要的使用区别:
		* 包含的头文件不同
		* 调用是否需要加std:: 命名空间前缀;
*/



//2.'c++线程库与c11线程库'的API区别:
/*
	'c11线程库'和'c++线程库'的线程库, 基本上是没有区别的, 'c++线程库' 是'c11线程库'的超集, 
	连函数名都是一样的, 只不过调用方式不一样而已(调用方法的区别, 请看上面的第1点);

	c++14, c++17, c++20, 都有对'c11线程库', 进行补集拓展, 
	所以主要的'c++线程库与c11线程库'的API区别:
		'c++线程库'是c11线程库的超集;
*/



//3.创建原子变量的区别:
/*
	'c11线程库':
		atomic_int tmp;
		tmp = 10;

	'c++线程库':
		std::atomic<int> tmp;
		tmp = 10;
*/



//4.c11原子操作库bug:
/*
	c11原子操作库的支持非常差, c/c++ 不能通用, 甚至包含头文件都会出错, 非常无语:
		#include <stdatomic.h>

	'c11线程库'头文件可以用:
		#include <threads.h>

	(推荐彻底区分c/c++, c语言用c语言的库, c++用c++的线程库)
*/



#include <cstdio>

//'c11线程库'
//#include <stdatomic.h>					//c++不能用c11 的原子库头文件, 不知道为何(一包含这个头文件就一大堆报错)
#include <threads.h>							//可以用

//'c++线程库'
#include <atomic>
#include <thread>

void c11(void){										//弃用(c++ 不支持c11 原子库, 非常无语)
	/*
	atomic_int tmp;
	tmp = 10;
	printf("tmp = %d\n", (int)tmp);
	printf("atomic_int tmp is lock free? %s\n", atomic_is_lock_free(&tmp) ? "true" : "false");
	*/
	return;
}

void cpp(void){
	std::atomic<int> tmp;
	//std::atomic<int> tmp = 10;		//原子变量不允许在定义过程中初始化
	tmp = 10;												//单独初始化原子变量
	printf("tmp = %d\n", (int)tmp);	//打印输出原子变量的值时, 需要进行type强转
	printf("atomic_int tmp is lock free? %s\n", std::atomic_is_lock_free(&tmp) ? "true" : "false");
	return;
}

int main(void){
	c11();
	cpp();
	return 0;
}






