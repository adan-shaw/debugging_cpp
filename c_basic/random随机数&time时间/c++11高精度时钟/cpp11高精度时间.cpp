//编译:
//		g++ -g3 ./cpp11高精度时间.cpp -o x
//		cl.exe ./cpp11高精度时间.cpp -EHsc -w -Zi -Ox -link -out:x.exe



#include <cstdio>

//判断编译器是否支持c++11 线程库
#ifdef __STDC_NO_THREADS__
	#error [ Not Support c++11 <thread> ]
#else
	#pragma message("Support c++11 <thread>")
	#include <thread>
	//std::chrono::system_clock 头文件
	#include <chrono>
	//std::put_time 头文件
	#include <iomanip>
#endif

//判断编译器是否支持c++11 原子操作库
#ifdef __STDC_NO_ATOMICS__
	#error [ Not Support c++11 <atomic> ]
#else
	#pragma message("Support c++11 <atomic>")
	#include <atomic>
#endif



/*
时间粒度的区分:
	1s = 1000ms
	1ms = 1000us
	1us = 1000ns

*/
int main(void){
	//c++ 的高精度时钟timer, 默认是ns 为单位的, 例如:
	std::chrono::high_resolution_clock::time_point start, end;
	//c++ 的时差计算器, 倒是可以转换为: double + s/ms/us/ns
	start=std::chrono::high_resolution_clock::now();
	end=std::chrono::high_resolution_clock::now();

	//直接强转, 不需要时差容器
	double s=std::chrono::duration<double>(end-start).count();							//double + 秒s
	double ms=std::chrono::duration<double,std::milli>(end-start).count();	//double + 毫秒级ms
	double us=std::chrono::duration<double,std::micro>(end-start).count();	//double + 微妙级us
	double ns=std::chrono::duration<double,std::nano>(end-start).count();		//double + 纳秒级ns
	printf("s=%lf, ms=%lf, us=%lf, ns=%lf\n", s, ms, us, ns);

	//时差容器
	std::chrono::duration<double> elapsed_s=end-start;											//double + 秒s
	std::chrono::duration<double, std::milli> elapsed_ms=end-start;					//double + 毫秒级ms
	std::chrono::duration<double, std::micro> elapsed_us=end-start;					//double + 微妙级us
	std::chrono::duration<double, std::nano> elapsed_ns=end-start;					//double + 纳秒级ns
	printf("s=%lf, ms=%lf, us=%lf, ns=%lf\n", elapsed_s.count(), elapsed_ms.count(), elapsed_us.count(), elapsed_ns.count());

	//休眠
	std::this_thread::sleep_for(std::chrono::seconds(1));										//休眠1s
	std::this_thread::sleep_for(std::chrono::milliseconds(1));							//休眠1ms
	std::this_thread::sleep_for(std::chrono::microseconds(1));							//休眠1us
	std::this_thread::sleep_for(std::chrono::nanoseconds(1));								//休眠1ns
	/*
		std::chrono::nanoseconds
		std::chrono::microseconds
		std::chrono::milliseconds
		std::chrono::seconds
		std::chrono::minutes
		std::chrono::hours
	*/

	return 0;
}

