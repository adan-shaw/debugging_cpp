//编译:
//		gcc pth_key线程私有数据.c -g3 -lpthread -o x 



//pth_key线程私有数据用途:
/*
	在多线程的环境下, 进程内的所有线程'共享进程的数据空间', 因此'全局/静态/进程堆变量'为所有线程共享;

	有时需要保存线程自己的'全局/静态'变量, 这种特殊的变量仅在'线程内部有效';

	可以通过创建线程的私有数据(TSD thread specific data)来解决这类问题,
	在线程内部, 私有数据可以被各个函数访问, 但他对其他线程是屏蔽的;

	用途:
		为每个线程配备一个stack
		为每个线程配备一个queue
		为每个线程配备一块buf
		(这些可以直接定义为local 局部变量, func函数使用时, 不好索引, 需要传递指针到函数参数)
		...
*/



//pth_key线程私有数据API:
/*
	int pthread_key_create(pthread_key_t *key, void (*destr_function) (void*));
	int pthread_key_delete(pthread_key_t key);
	int pthread_setspecific(pthread_key_t key, const void *pointer);
	void *pthread_getspecific(pthread_key_t key);

	线程私有数据采用了一键多值的技术, 即一个键对应多个值;
	访问数据时都是通过键值来访问, 好像是对一个变量进行访问, 其实是在访问不同的数据;

	跟共享内存一样, 一个线程私有数据, 只能指向一块内存, 
	具体这块内存怎么细分, 那需要你自己详细规划;
	(不能通过创建线程时, 通过线程属性指定, 只能使用pthread_key_create() 进行创建)
*/



#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct info{
	int i;
	float k;
};

//pthread_key_t 是需要共享的, 每个线程, 根据key 获取内存块;
//(你也可以通过线程函数的指针参数, 传递pthread_key_t, 但这是个愚蠢的做法)
pthread_key_t key;

void* child1(void *arg){
	struct info data, *pdata;
	data.i = 10;
	data.k = 3.1415;
	pthread_setspecific(key, &data);	//把线程key初始化为结构体
	printf("child1--&data=0x%p\n", &data);
	printf("child1--pthread_getspecific(key)=0x%p\n", (struct info*)pthread_getspecific(key));
	pdata = (struct info*)pthread_getspecific(key);
	printf("child1--pdata->i:%d, pdata->k: %f\n", pdata->i, pdata->k);
	return NULL;
}

void* child2(void *arg){
	int temp = 20, *ptemp;
	pthread_setspecific(key, &temp);	//把线程key初始化为int
	printf("child2--&temp=0x%p\n", &temp);
	printf("child2--pthread_getspecific(key)=0x%p\n", (int*)pthread_getspecific(key));
	ptemp = (int*)pthread_getspecific(key);
	printf("child2--ptemp=%d\n", *ptemp);
	return NULL;
}


int main(void){
	pthread_t thid1, thid2;

	pthread_key_create(&key, NULL);		//创建线程key(私有变量)

	if(pthread_create(&thid1, NULL, (void*)child1, NULL) != 0){
		perror("pthread_create()");
		return -1;
	}
	if(pthread_create(&thid2, NULL, (void*)child2, NULL) != 0){
		perror("pthread_create()");
		return -1;
	}

	sleep(1);

	if(pthread_join(thid1,NULL) != 0){
		perror("pthread_join()");
		return -1;
	}
	if(pthread_join(thid2,NULL) != 0){
		perror("pthread_join()");
		return -1;
	}

	pthread_key_delete(key);					//销毁线程key(私有变量)

	return 0;
}
