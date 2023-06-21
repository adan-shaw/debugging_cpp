//编译:
//		g++ -g3 ./test_clock_nanosleep.cpp -o x 



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>



static void sigintHandler(int sig){
	printf("sigintHandler() event\n");
	//Just interrupt nanosleep()
	return;
}



int main(void){
	struct timeval start, finish;
	struct timespec request, remain;
	struct sigaction sa;
	int s;
	int flags, sec,nsec;



	//Allow SIGINT handler to interrupt nanosleep()
	//允许nanosleep() 休眠期间, 处理收到的信号, 仍然可以执行信号函数;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sigintHandler;
	if(sigaction(SIGINT, &sa, NULL) == -1){
		perror("sigaction() failed");
		exit(EXIT_FAILURE);
	}



	//设置sleep() 休眠的时间
	sec = 1;									 //1 秒
	nsec = 50;								 //50 纳秒
	flags = 0;								 //相对sleep
	//flags = TIMER_ABSTIME;	 //绝对sleep(不知道怎样用绝对时间休眠, 有点难用)

	if(flags == TIMER_ABSTIME){//abstime sleep (绝对时间)TIMER_ABSTIME == 1
		//获取当前clock 真实时间(realtime)
		if(clock_gettime(CLOCK_REALTIME, &request) == -1){
			perror("clock_gettime() failed");
			exit(EXIT_FAILURE);
		}

		printf("Initial CLOCK_REALTIME value: %ld.%09ld\n",\
				(long) request.tv_sec, (long) request.tv_nsec);

		request.tv_sec = sec;
		request.tv_nsec = nsec;
		if(request.tv_nsec >= 1000000000){
			request.tv_sec += request.tv_nsec / 1000000000;
			request.tv_nsec %= 1000000000;
		}
	}
	else{											 //Relative sleep (相对时间)
		request.tv_sec = sec;
		request.tv_nsec = nsec;
	}



	//记录nanosleep()休眠开始前的时间(统计一共休眠了多久)
	if(gettimeofday(&start, NULL) == -1){
		perror("gettimeofday() failed");
		exit(EXIT_FAILURE);
	}
	//执行sleep
	for(;;){
		//request = 请求休眠的时间(输入), remain = 剩余的休眠时间(返回)
		s = clock_nanosleep(CLOCK_REALTIME, flags, &request, &remain);
		if(s != 0 && s != EINTR){
			perror("clock_nanosleep() failed");
			break;
		}
		if(s == EINTR){
			perror("Interrupted, clock_nanosleep() == EINTR");
			break;
		}
		if(s == 0)
			break;//休眠一次就足够了, nanosleep() completed

		//如果休眠一次不能解决问题, 就要继续第二次的休眠
		//Next sleep is with remaining time 打印剩余的总休眠时间
		printf("Remaining: %2ld sec, %09ld usec\n", (long)remain.tv_sec,remain.tv_nsec);
		//重置休眠请求request的值
		request.tv_sec = remain.tv_sec;
		request.tv_nsec = remain.tv_nsec;
	}
	//记录nanosleep()休眠结束后的时间(统计一共休眠了多久)
	if(gettimeofday(&finish, NULL) == -1){
		perror("gettimeofday() failed");
		exit(EXIT_FAILURE);
	}

	//打印准确的休眠时间统计结果
	printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec +
			(finish.tv_usec - start.tv_usec) / 1000000.0);



	printf("Sleep complete\n");
	exit(EXIT_SUCCESS);
}
