#include <stdbool.h>
#include <pthread.h>


//没有任务的时候, 任务线程阻塞等待时间
#define wait_mission_timeout (2)


//线程池的访问状态控制
typedef struct condition{
	pthread_mutex_t pmutex;
	pthread_cond_t pcond;
}condition_t;


//'单个任务'信息载体
typedef struct task{
	void *(*run)(void *args); //函数指针, 需要执行的任务
	void *arg;								//函数参数指针
	struct task *next;				//下一个任务信息载体
}task_t;


//线程池信息载体
typedef struct threadpool{
	condition_t ready;	//线程池的访问状态控制
	task_t *first;			//任务队列中第一个任务(链型队列)
	task_t *last;				//任务队列中最后一个任务
	int counter;				//线程池中已有线程数
	int idle;						//线程池中空闲线程数
	int max_threads;		//线程池最大线程数
	bool quit;					//是否退出标志
}threadpool_t;


//线程池初始化
bool threadpool_init(threadpool_t *pool, int threads);

//往线程池中加入任务
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg);

//摧毁线程池
bool threadpool_destroy(threadpool_t *pool);
