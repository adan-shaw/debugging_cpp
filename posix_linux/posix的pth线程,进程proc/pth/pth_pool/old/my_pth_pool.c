//编译:
//		gcc my_pth_pool.c -lpthread -g3 -o x 

//my固定版本的线程池(可参考!!)

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>



#define pthread_pool_max (4)
unsigned long pid_pool[pthread_pool_max];

bool is_quit_all = false;
bool is_startWork_all = false;

#define wait_all_finish_element (10000)	//1w ns = 10 ms
#define wait_all_finish (100)						//一个单位 = 10 ms, 100 = 1s
#define wait_all_quit (2)								//主线程wait all quit 等待时间(单位:秒)



//重新初始化全局变量
void reset_all_gval(void){
	memset(&pid_pool, 0, pthread_pool_max);
	is_quit_all = false;
	is_startWork_all = false;
}

//启动所有线程
void start_working_all(void){
	is_quit_all = false;
	is_startWork_all = true;
}

//线程清理函数
void pthread_exit_cleanup(void* arg){

}

//线程函数
void* pth_func(void* arg){
	//继承参数arg

	//****************************************************************************
	//做到1s 内创建所有线程(不应该高于16 个, 否则要扩大到2 s)
	//有一条创建失败, 所有线程都退出
	int wait_all_time = wait_all_finish_element;
	int wait_all_count = wait_all_finish;
	int i = 0;
	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = wait_all_time;//10ms
	nanosleep(&req, NULL);			//等待detach脱离

	for(;i < wait_all_count;i++){
		nanosleep(&req, NULL);		//10ms
		if(is_startWork_all)
			break;									//退出等待=全部创建成功/失败? 但可以肯定初始化结束
	}
	if(is_quit_all){
		is_quit_all = true;				//肯定退出的操作
		is_startWork_all = true;
		return NULL;							//线程池创建不完善, 且等待超时, 所有线程退出
	}
	//****************************************************************************

	pthread_cleanup_push(pthread_exit_cleanup, arg);//压入清理函数
	//******线程执行体 start ******
	printf("pid: %d start working\n", pthread_self());

	//******线程执行体	end	******
	pthread_cleanup_pop(1);			//释放清理函数
	return NULL;
}

//创建join 模式的线程池
bool create_pth_pool_join(void* arg){
	int i = 0;
	pthread_t thid;
	for(;i < pthread_pool_max;i++){
		if(pthread_create(&thid,NULL,pth_func,arg) != 0){
			printf("pthread_create() fail, errno: %d\n",errno);
			killall_pth_pool_join();
			return false;
		}
		pid_pool[i] = thid;
	}
	//打印线程池与编号
	printf("pthread pool info:\n");
	for(i = 0;i < pthread_pool_max;i++)
		printf("pth_num_%u: (pid = %d)\n", i, pid_pool[i]);

	start_working_all();				//启动所有线程
	return true;
}

//销毁join 模式的线程池
void killall_pth_pool_join(void){
	int i = 0;
	void* cancel_status;
	is_quit_all = true;
	is_startWork_all = true;
	sleep(wait_all_quit);

	//简单地从内核清除线程结束状态信息
	//pthread_join(pid_pool[i],NULL);

	//获取线程结束状态并打印, 将结束信息从内核中清除
	for(i = 0;i < pthread_pool_max;i++){
		cancel_status = NULL;			//线程函数结束时的返回值
		if(pthread_join(pid_pool[i], &cancel_status) != 0)
			printf("Thread No.%u pthread_join() fail, errno:%d\n",pid_pool[i],errno);
		else
			printf("Thread No.%u was canceled, cance return value = %d\n",pid_pool[i], cancel_status);
	}
	reset_all_gval();
	return ;
}


//创建detach 模式的线程池
bool create_pth_pool_detach(void* arg){
	int i = 0;
	pthread_t thid;
	for(;i < pthread_pool_max;i++){
		if(pthread_create(&thid,NULL,pth_func,arg) != 0){
			printf("pthread_create() fail, errno: %d\n",errno);
			killall_pth_pool_detach();
			return false;
		}
		if(pthread_detach(thid) != 0){
			printf("pthread_detach() fail, errno: %d\n",errno);
			killall_pth_pool_detach();
			return false;
		}
		pid_pool[i] = thid;
	}
	//打印线程池与编号
	printf("pthread pool info:\n");
	for(i = 0;i < pthread_pool_max;i++)
		printf("pth_num_%d: (pid = %u)\n", i, pid_pool[i]);

	start_working_all();				//命令所有线程开始工作
	return true;
}

//销毁detach 模式的线程池
void killall_pth_pool_detach(void){
	is_quit_all = true;
	is_startWork_all = true;
	sleep(wait_all_quit);
	reset_all_gval();
}

//测试函数(线程的启动顺序是无序的, 但启动过程是安全的)
int main(void){
	printf("create_pth_pool_detach() 测试:\n");
	create_pth_pool_detach(NULL);
	sleep(2);										//启动之后不能马上关闭线程池, 至少间隔2秒
	killall_pth_pool_detach();


	printf("\n\ncreate_pth_pool_join() 测试:\n");
	create_pth_pool_join(NULL);
	sleep(2);
	killall_pth_pool_join();
	return 0;
}
