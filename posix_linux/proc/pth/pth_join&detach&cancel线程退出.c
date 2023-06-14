//编译:
//		gcc pth_join\&detach\&cancel线程退出.c -g3 -lpthread -o x 



//pthread线程常用到的API list:
/*
	pthread_t pthread_self(void);												//获取自己的pthread_t
	int pthread_equal(pthread_t t1, pthread_t t2);			//比较两个pthread_t 是否相等

	void pthread_exit(void *retval);										//强制退出:自己
	int pthread_cancel(pthread_t thread);								//强制退出:指定pthread_t
	int pthread_detach(pthread_t thread);								//脱离:指定pthread_t
	int pthread_join(pthread_t thread, void **retval);	//询问结束状态:指定pthread_t
*/



//linux pthread的两种退出状态(线程属性的一种)
/*
	'linux pthread'有两种退出状态joinable可链接/unjoinable不可链接:
		joinable(默认选项):
			pthread 线程退出后, 可以通过pthread_join()询问退出状态;
			linux 内核会为线程保留退出状态记录, 也保留pthread_t 编号(pid), 暂时不释放;
			(但堆栈数据, 程序段等数据, 早就释放了, 只是占着pid和线程退出记录等少量数据未释放)

		unjoinable:
			pthread 线程退出后, 自动释放所有资源, 包括退出状态记录&pthread_t 编号(pid)等;
			不允许使用pthread_join()询问退出状态&&释放'pthread退出状态记录'所占用的资源;


	'linux pthread'的退出状态, 可以在pthread_create()时, 在attr属性中指定;
	也可以通过pthread_detach()函数, 放弃保留pthread退出状态记录;

	ps:
		不过保留了pthread退出状态记录, 但你又不调用pthread_join()释放'pthread退出状态记录'所占用的资源;
		则pthread线程会成为僵尸线程;
		(linux 没有真正意义上的进程, 线程即进程)

		但最终, 如果父进程结束了, 子线程会因为与父进程共享CLONE_VM属性的缘故, 子线程一定会被内核强制终止;
*/



//pthread线程的强制退出
/*
	void exit(int status);															//强制退出整个进程
	void pthread_exit(void *retval);										//强制退出:自己
	int pthread_cancel(pthread_t thread);								//强制退出:指定pthread_t

	无论你使用哪种方式退出线程, 都需要调用pthread_join(), 清除pthread退出状态记录;
	退出方式, 并不能改变线程属性;
	只要你设置了线程属性: 退出后保留pthread退出状态记录, 就一定要调用pthread_join();
	否则只能是:
		父进程结束, 内核在清扫父进程时, 连带pthread尸体一起清除;


	pthread_exit(): 退出线程可以返回一个void*, 调用pthread_join()的时候, 可以获取该值;
	pthread_cancel(): 退出线程不能指定返回任何值, pthread_join()获取到的返回值, 永远为-1;
*/



//修改线程属性: 不保留pthread退出状态记录
/*
	1.创建线程时, 放弃保留
		pthread_attr_t attr;
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//保留
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//不保留

	2.int pthread_detach(pthread_t thread);							//手动将线程属性设置为unjoinable状态
		常见组合用法:
			pthread_detach(pthread_self());									//线程自己脱离, 放弃保留pthread退出状态记录的权利
*/



//获取pthread退出状态记录
/*
	int pthread_join(pthread_t thread, void **retval);	//获取pthread退出状态记录

	如果使用了pthread_cancel()强制退出其它线程, 则void **retval返回值永远为-1;
*/



#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>



//1.线程函数
void* pth_func(void* arg){
	pthread_t thid = pthread_self();
	while(1){
		printf("pth_func(%d) alive\n", thid);
		sleep(1);
	}
	pthread_exit(9999);	//等同于return 9999;
	return 9999;				//等同于pthread_exit(9999);
}

int main(void){
	pthread_t pid;
	void* pson_ret = NULL;

	//主线程自己pthread_detach()自己
	if(pthread_detach(pthread_self()) != 0){
		perror("pthread_detach()");
		return -1;
	}

	if(pthread_create(&pid,NULL,pth_func,NULL) == -1){
		perror("pthread_create()");
		return -1;
	}

	//强制取消指定线程, 放弃获取其返回值
	sleep(1);
	if(pthread_cancel(pid) != 0){
		perror("pthread_cancel()");
		return -1;
	}

	//获取指定'线程的退出状态', 顺便释放'线程的退出状态'所占用的资源
	if(pthread_join(pid,&pson_ret) != 0){
		perror("pthread_join()");
		return -1;
	}

	printf("pthread_join(): exit return = %d\n", (int*)pson_ret);
	return 0;
}


