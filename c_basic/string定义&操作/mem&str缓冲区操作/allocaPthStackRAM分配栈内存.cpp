//编译:
//		g++ -g3 ./allocaPthStackRAM分配栈内存.cpp -o x 



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> // for getpagesize()
#include <string.h> // for memset()
#include <alloca.h> // for alloca()



//错误测试, 看看alloca()的申请的堆栈内存, 在函数返回后, 能不能再用.
void *p = NULL;
void *p2 = NULL;
void *p3 = NULL;



//1.alloca()测试(以默认堆栈大小8192kb 为例)
void alloca_test(void){
	//申请8kb 线程栈内存
	void *tmp = alloca(8192);
	void *tmp2 = alloca(8192);
	void *tmp3 = alloca(8192);

	snprintf((char*)tmp,8192,"%s_%d_%s_%lf",\
			"hello",123456,"fuck",0.999);
	snprintf((char*)tmp2,8192,"%s_%d_%s_%lf",\
			"hello2",123456,"fuck",0.999);
	snprintf((char*)tmp3,8192,"%s_%d_%s_%lf",\
			"hello3",123456,"fuck",0.999);

	printf("%s\n",(char*)tmp);
	printf("%s\n",(char*)tmp2);
	printf("%s\n",(char*)tmp3);

	//错误测试需要用(将线程栈申请的内存,用全局变量指针记住,看看函数返回之后,会不会被自动释放)
	p = tmp;
	p2 = tmp2;
	p3 = tmp3;

	return;
}



//2.测试'线程栈溢出'错误
void xx(void){
	//字符串会被编译器改写成常量const!! 所以不会'线程栈溢出'
	//char x[1024*1024*1024];

	//int 整形数, 不会被编译器改写成常量, 容易造成'线程栈溢出'
	int x[1024*1024*2-1024-128] = {0};

	//int x[7] = {0};
	//printf("xx() okay\n");
	return;
}





int main(void){
	//1.alloca()测试
	alloca_test();

	//2.测试'线程栈溢出'错误
	xx();


	//3.错误测试: 函数返回后, 调用alloca()申请的堆栈内存
	printf("%s\n",(char*)p);
	//free(p);//肯定出错!!
	printf("%s\n",(char*)p2);
	//free(p2);
	printf("%s\n",(char*)p3);
	//free(p3);

	return 0;
}







//
//1.线程栈, 进程堆的区别:
//
/*
	什么叫堆? 线程栈的本质是啥?
		在线程结构的容器上, 实现完全结构的二叉树, 就叫堆.
		线程栈, 实际上虽然是栈, 但是也是堆.
		所以, 有些书籍也叫这个作: 堆栈

		但是线程栈仍然逃脱不了:
			先进先出,
			压栈, 清栈, 栈挤压的原则, 这是栈操作的最基本原则!!

			实现堆只是为了更快查找数据而已, 操作还是栈的操作.



	进程堆的本质:
		线程栈是一个完全二叉树,
		进程堆则是一个不完全的二叉树.
		但是进程堆本身并没有基于栈来实现二叉树, 进程堆是一个真二叉树.

		进程堆里面存放的元素, 是一根一根指针而已, 方便增加和删除.
		可供频繁新增, 删除, 
		任意位置删除, 
		高速查找能力.

		堆的'实体容器', 就是操作系统的内容总量.
		一般情况下, 操作系统都是限制一个进程的最大堆内存是多少.



	线程栈, 进程堆的区别:
		进程堆, 进程内的线程共享, 每个进程唯一;
		手动malloc()分配, 手动free()释放的, 接收ptmalloc 管理.
		进程结束时, 内核会强制进行'堆内存'清算的时候, 防止堆内存泄漏.

		线程栈, alloca()申请的动态栈内存,
		跟普通局部变量, 常量一样, 会被常规入栈, 出栈,
		所以'线程栈'申请的动态内存, 只需alloca()申请, 
		不需要释放, 函数返回时, 自动释放.
*/





//
//2.线程栈API 简介:
//
/*
	头文件#include<malloc.h>

	函数原型:
		void* _cdecl alloca(size_t);
		参数size_t(单位字节):
			申请分配内存的字节数, 
		返回值:
			分配到的内存地址.
		alloca()主要的特征是:
			在线程栈上开辟的空间, 当它作用域结束时, 会自动释放内存,
			(不用像malloc()那样, 要用free()动态释放空间)
			[malloc()申请的内存, 需要调用memset()来进行初始化,alloca()则自动初始化为0]


	缺点:
		1,如果alloca()容易导致栈溢出.

		2,可移植性很差, 对编译器和平台有很强的依赖性,
			在很多系统上的实现是有bug的, 所以并不鼓励使用.
			32v、pwb、pwb.2、3bsd、4bsd支持这个函数的实现,
			Linux使用的是GUN版本, 在POSIX和SUSv3系统上是不支持该函数的.
*/





//
//3.线程栈与cpu 缓存的关系:
//
/*
	cpu 缓冲绝对跟线程栈毫无关系, 但是绑定进程到cache 中,
	进程内的所有线程, 提取数据的命中率会提高.
	cpu cache 是独立的, 程序员不能操作其命中率
*/








//
//测试: 默认线程栈的MAX
//结论:(一般Linux 默认栈为8MB, 可以用命令查看:ulimit -s)
//
/*
	linux 查看修改线程默认栈空间大小: ulimit -s
	linux 查看系统各种限制数据: ulimit -a

	可以使用:
		pthread_attr_setstack() 如上修改栈大小为16MB
*/

/*
int main(void){
	//vs 调试模式下的默认极限
	char xx[1000][1024];

	//gcc 程序默认极限
	//char x[2048][1024];

	return 0;
}
*/


