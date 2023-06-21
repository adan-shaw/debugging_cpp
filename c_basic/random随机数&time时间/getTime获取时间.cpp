//编译: 
//	g++ -lm -g3 ./getTime获取时间.cpp -o x 



//
// time.h 获取时间类API
//
//声明: [时间类API基本不可能出错, 因此不用检查返回值,
//			不要怀疑linux内核的基础功能, 否则你吞口水都会呛死]



/*
1.0 time_t = long int, 因为time() 失败会返回-1, 
		所以有负数, 不能用unsigned long int.

		clock_t = long int, 不知道为什么, 
		但是long int的长度, 已经够用了, 可能为了对齐, 方便做转换

		在32bit中, long 类型只有32bit, 
			因此在32bit 系统中, time() 返回一个unsigned long 类型, 防止数值溢出.
		在64bit中, long 类型只有64bit, 
			因此在64bit 系统中, time() 返回一个signed long 类型.


		CLOCKS_PER_SEC 表示一秒钟内, CPU运行的'时钟周期数'



1.1 计算两个时间变量之间的秒数差:
double difftime(time_t time1, time_t time0);
		注意顺序: 
			后面的time0 - 前面的time1, 
		所以, 返回值有可能有负数, 
		因此, 还要加绝对值函数abs()



1.2 返回'1970年1月1日至今'所经历的时间(以秒为单位):
time_t time(time_t* tloc);
		不要用一年有多少秒来推算月,日,时,分,秒.
		虽然1年=31536000秒, 但是润年, 多一天;
		有时候有些特殊年, 间隔比较特殊.
		所以实际'1970年1月1日至今', 具体经过了多少秒, 这个只有实际走过的时间才能确定.
		因此, 这里一定要用'timeTypeSwitch' API, 进行转换操作.

		//非线程安全的调用方式(time()函数内部的容器是static变量实现的)
		time(NULL);

		//线程安全调用方式
		time(&tmp_time);

		time()缺点:
			单位时间内多次调用, 数值相同, 不具备随机性.



1.3 返回当前API占用CPU的时间(一般相当于几个指令周期):
clock_t clock(void);
		最简单的随机数种子是:
			unsigned int uiTmp = (unsigned int) ( clock() % 1000 ) * ( clock() % 1000 );
		如果觉得不够复杂, 可以再乘一个:(最大值=1000*1000*1000, int 不会溢出, 但cpu消耗有点大)
			unsigned int uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );

		int MAX = 2^32 =  4 294 967 296 > 4 000 000 000(2000*2000*1000)
*/



#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>



int main(void){
	long int liTmp,liTmp2;
	double dTmp;



	//1.time()
	//非线程安全
	printf("time(NULL)=%d\n", time(NULL));
	//线程安全
	printf("time(&liTmp)=%d\n", time(&liTmp));



	//2.clock()
	liTmp = clock();
	printf("clock() = %d\n", liTmp);

	dTmp = liTmp/CLOCKS_PER_SEC;
	printf("clock()/CLOCKS_PER_SEC = %lf\n", dTmp);



	//3.difftime()
	liTmp = time(&liTmp);
	sleep(1);//制造时间空隙
	liTmp2 = time(&liTmp2);

	//后面的时间liTmp2 - 前面的时间liTmp
	dTmp = difftime(liTmp,liTmp2);

	//对double 求绝对值
	dTmp = fabs(dTmp);
	printf("difftime(time(),time()) = %lf\n", dTmp);



	//4.数据类型长度检测: 8-8-8 byte = (8bit)*8=64bit
	printf("sizeof(time_t) = %d byte\n",sizeof(time_t));
	printf("sizeof(clock_t) = %d byte\n",sizeof(clock_t));
	printf("sizeof(long) = %d byte\n",sizeof(long));



	return 0;
}
