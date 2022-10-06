#include <pthread.h>

//初始化cond条件变量(快捷宏)
#define pthread_cond_initEx(cond,mutex) { \
pthread_mutex_init(&mutex,NULL); pthread_cond_init(&cond,NULL); }

//wait死等cond条件变量(快捷宏)
#define pthread_cond_waitEx(cond,mutex) { pthread_mutex_lock(&mutex); \
pthread_cond_wait(&cond,&mutex); pthread_mutex_unlock(&mutex); }

//signal唤醒cond条件变量(快捷宏)
#define pthread_cond_signalEx(cond,mutex) { pthread_mutex_lock(&mutex); \
pthread_cond_signal(&cond); pthread_mutex_unlock(&mutex); }

//broadcast广播唤醒cond条件变量(快捷宏)
#define pthread_cond_broadcastEx(cond,mutex) { pthread_mutex_lock(&mutex); \
pthread_cond_broadcast(&cond); pthread_mutex_unlock(&mutex); }

//销毁cond条件变量(快捷宏)
#define pthread_cond_destroyEx(cond,mutex) { pthread_mutex_lock(&mutex); \
pthread_cond_broadcast(&cond); pthread_cond_destroy(&cond); pthread_mutex_unlock(&mutex); }
