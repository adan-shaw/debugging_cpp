//编译:
//		g++ -g3 ./boost.pth.cpp -o x -pthread -l boost_system -l boost_thread -l boost_chrono



//本demo 所需的编译依赖库(boost 库用起来还满复杂的):
/*
	-pthread:        boost_thread 也是依赖std::thread 来实现的, 不能离开-pthread 编译选项!!
	-l boost_thread: boost_thread 自身的编译选项
	-l boost_system: boost 库必须的编译选项
	-l boost_chrono: boost 高精度休眠库所需要的编译选项
*/


#include <boost/thread.hpp>
#include <iostream>



//令boost::thread 休眠, 时间单位s 秒
inline void boost_wait_s(unsigned int sec)
{
	boost::this_thread::sleep(boost::posix_time::seconds(sec));
	return;
}

//令boost::thread 休眠, 时间单位ms 毫秒
inline void boost_wait_ms(unsigned int ms)
{
	boost::this_thread::sleep_for(boost::chrono::milliseconds(ms));
	return;
}

void thread1(void)
{
	boost_wait_ms(100);
	std::cout << "thread1() end!!" << std::endl;
	return;
}

void thread2(void)
{
	boost_wait_s(1);
	std::cout << "thread2() end!!" << std::endl;
	return;
}



int main(void)
{
	boost::thread t1(thread1);
	boost::thread t2(thread2);
	t1.join();
	t2.join();
	return 0;
}

