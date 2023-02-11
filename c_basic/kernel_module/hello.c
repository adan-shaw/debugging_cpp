//编译:
/*
	由于需要依赖kernel 自带的编译环境, 不能使用默认gcc 编译, 也不能使用默认的glibc 环境, 
	所以, 请使用Makefile 编译kernel 模块(必须的),
	详情, 自己查看Makefile

	编译kernel 模块, 需要指明linux kernel project源码路径, 
	而且不能用系统的gcc, 需要使用make -C 指定编译环境, 再使用M=$(shell pwd) 指定编译
*/



#include <linux/init.h>							//指定初始化&清理函数, 如: module_init(),module_exit()
#include <linux/module.h>						//包含了大量'kernel模块需要的函数和符号'的定义, 如: MODULE_LICENSE(), MODULE_AUTHOR()
#include <linux/kernel.h>



MODULE_LICENSE("GPL");							//必须指定kernel 模块的授权协议, 否则编译不通过!!
//MODULE_AUTHOR("ADAN_SHAW"); 			//作者简介, 可以注释
MODULE_ALIAS("hello");							//kernel 模块的alias 别名



int hello_init(void){
  printk(KERN_ALERT"Hello kernel module init\n");
  return 0;
}

void hello_exit(void){
  printk(KERN_ALERT"Hello kernel module exit\n");
}



module_init(hello_init);						//指定kernel模块初始化函数(insmod 命令执行后, 首先运行的函数)



module_exit(hello_exit);						//指定kernel模块退出函数(rmmod 命令执行后, 最后运行的函数)



//总结:
/*
	linux驱动程序, 可以编译成哪几种exe二进制文件:
		1)内部编译: 将驱动程序源码放在内核源码目录中进行编译
		2)外部编译: 将驱动程序源码放在内核源码目录外进行编译
		3)静态编译: 编译进uImage中
		4)动态编译: (.ko文件,动态加载驱模块)

	kernel 模块编码, 其实很简单, 无论你编译成哪种exe二进制文件, 它都只是一个kernel 模块, 并不是一个独立程序;
	因此, kernel 模块是不能独立运行的, 只能依赖于linux 系统运行过程中, 使用命令: insmod装载kernel模块, rmmod卸载kernel模块;

	所以, kernel 模块的程序入口就是: module_init(), 等价于main();
	所以, kernel 模块的程序出口就是: module_exit(), 等价于exit();

	ps:
		二进制程序, 也是只有'入口main()/出口exit()', 其他都是'.so 共享链接库';
		kernel 模块其实也一样的, '入口module_init()/出口module_exit()', 其他都是.ko 共享链接库;
		只不过, kernel 模块所使用的编译器(最原始版,没有glibc的gcc), '.ko 共享链接库', 与exe 二进制程序的'全功能&带glibc的gcc', '.so 共享链接库', 有点不一样;
		由此可见, kernel 模块和exe 二进制程序, 本质上编码手法, 其实都差不多, 只是所用的编码环境不一样而已;
*/


