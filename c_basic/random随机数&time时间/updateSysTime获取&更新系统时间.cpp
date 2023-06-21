//编译:
//		g++ -g3 ./updateSysTime获取\&更新系统时间.cpp -o x 


//
//0.使用说明:
//
/*
	这类函数, 一本都是用来对比和纠正时间差的,
	只有对时间准度极度依赖的场合, 才需要用到.

	修正时间绝对准确, 对齐, 哪怕是微妙级别的对齐.
*/





//
//1.即时更改型time() 函数
//
/*
	//获取'当前距离1970年的系统时间', 返回秒数和微妙数
	int gettimeofday(struct timeval *tv, struct timezone *tz);
	//设置'当前距离1970年的系统时间', 输入秒数和微妙数
	int settimeofday(const struct timeval *tv, const struct timezone *tz);

	ps:
		第二参数, tz 已经被废弃, 这里只需设置为NULL 即可
		(永远设置为NULL,以前是用来表示时差的)
		所以只需输入第一参数struct timeval *tv;
*/





//
//2.渐进式改变系统时间: adjtime()
//
/*
	int adjtime(const struct timeval *delta, struct timeval *olddelta);

	第一参数, 用来指定'校正的目标时间': struct timeval *delta;
	第二参数, 用来指定'还剩下的校正时差': struct timeval *olddelta;
	具体系统如何进行渐进式校正, 程序员不得干涉.
*/



#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>



//1.获取系统时间
void getSysTime_test(void){
	int iTmp;
	struct timeval stTime;



	//获取系统时间
	iTmp = gettimeofday(&stTime,NULL);
	if(iTmp == -1){
		printf("gettimeofday() failed, errno = %d\n", errno);
		return;
	}

	printf("stTime.tv_sec(秒):%d\n",stTime.tv_sec);
	printf("stTime.tv_usec(微秒):%d\n",stTime.tv_usec);

	return;
}



//2.获取时间差(微秒级别)
void diff_time(void){
	float time_use=0;
	struct timeval start, end;
	int i = 100000000;



	gettimeofday(&start,NULL);//gettimeofday(&start,&tz);结果一样
	printf("start.tv_sec:%d\n",start.tv_sec);
	printf("start.tv_usec:%d\n",start.tv_usec);

	while(i--)
		;

	gettimeofday(&end,NULL);
	printf("end.tv_sec:%d\n",end.tv_sec);
	printf("end.tv_usec:%d\n",end.tv_usec);

	//微秒级别求时间差
	time_use = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	printf("time_use(单位:微秒) is %.10f\n",time_use);

	return;
}



//3.修正系统时间(支持微秒级别的修改, 渐进式修改)
void setSysTime_test(void){

	return;
}





int main(void){
	//1.获取系统时间
	getSysTime_test();

	//2.获取时间差(微秒级别)
	diff_time();

	//3.修正系统时间(支持微秒级别的修改, 渐进式修改)
	setSysTime_test();

	return 0;
}







