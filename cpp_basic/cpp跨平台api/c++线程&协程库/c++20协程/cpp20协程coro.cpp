//编译:
//		(废弃, 除非你的gcc 版本是9.5.0 后面的gcc 版本)g++ -std=c++2a -g3 ./cpp20协程coro.cpp -o x
//		(debian10 不太支持coroutine 协程, 使用boost 库跳过去!!)clang-14 -std=c++20 -g3 ./cpp20协程coro.cpp -o x

//		boost 库1.67.0 也是需要glibc 3.4.x 才能使用c++20 的coroutine 协程, 放弃这部分内容了!!
//		clang-14 -std=c++20 -stdlib=libc++ -lboost_coroutine -g3 ./cpp20协程coro.cpp -o x



//gcc9 或者clang-14 之后的编译器, 使用这种格式包含头文件, 弃用std::experimental 命名空间!!
#include <iostream>
//using namespace std;
#include <coroutine>
//#include <experimental/coroutine>
//using namespace std::experimental;
#include <stdexcept>
#include <thread>





// coro_ret 协程函数的返回值, 内部定义promise_type, 承诺对象
template < typename T > struct coro_ret
{
	struct promise_type;
	using handle_type = std::coroutine_handle < promise_type >;
	handle_type coro_handle_;// 协程句柄

	coro_ret (handle_type h):coro_handle_ (h){}
	coro_ret (const coro_ret &) = delete;
	coro_ret (coro_ret && s):coro_handle_ (s.coro_handle_)
	{
		s.coro_handle_ = nullptr;
	}
	~coro_ret ()
	{
		if (coro_handle_)
			coro_handle_.destroy ();// 自行销毁
	}
	coro_ret & operator= (const coro_ret &) = delete;
	coro_ret & operator= (coro_ret && s)
	{
		coro_handle_ = s.coro_handle_;
		s.coro_handle_ = nullptr;
		return *this;
	}

	// 恢复协程, 返回是否结束
	bool move_next ()
	{
		coro_handle_.resume ();
		return coro_handle_.done ();
	}
	// 通过promise获取数据, 返回值
	T get ()
	{
		return coro_handle_.promise ().return_data_;
	}
	// promise_type就是承诺对象, 承诺对象用于协程内外交流
	struct promise_type
	{
		promise_type () = default;
		~promise_type () = default;

		// 生成协程返回值
		auto get_return_object ()
		{
			return coro_ret < T > { handle_type::from_promise (*this) };
		}

		// 注意这个函数,返回的就是awaiter;
		// * 默认返回std::suspend_never{}, 就不挂起;
		// * 默认返回std::suspend_always{} 挂起;
		// 当然你也可以默认返回其他awaiter, 可手动修改;
		auto initial_suspend ()
		{
			// return std::suspend_never();
			return std::suspend_always();
		}
		// co_return 后这个函数会被调用
		void return_value (T v)
		{
			return_data_ = v;
			return;
		}
		// 设置挂起返回值
		auto yield_value (T v)
		{
			std::cout << "yield_value invoked." << std::endl;
			return_data_ = v;
			return std::suspend_always();
		}
		// 在协程最后退出后调用的接口; 
		// 若final_suspend 返回std::suspend_always, 则需要用户自行调用handle.destroy() 进行销毁, 
		// 但注意final_suspend被调用时协程已结束, 返回std::suspend_always并不会挂起协程(实测VS C++2022)
		auto final_suspend () noexcept
		{
			std::cout << "final_suspend invoked." << std::endl;
			return std::suspend_always();
		}
		// 未处理的异常函数
		void unhandled_exception ()
		{
			std::exit (1);
		}
		// 返回值
		T return_data_;
	};
};



// 协程函数
coro_ret < int >coroutine_7in7out (void)
{
	// 进入协程看initial_suspend, 默认返回std::suspend_always(); 会有一次挂起

	// co_await std::suspend_never{} 不会挂起
	std::cout << "Coroutine co_await std::suspend_never" << std::endl;
	co_await std::suspend_never();

	// co_await std::suspend_always{} 总是会挂起(空闲即挂起)
	std::cout << "Coroutine co_await std::suspend_always" << std::endl;
	co_await std::suspend_always();

	std::cout << "Coroutine stage 1 ,co_yield" << std::endl;
	co_yield 101;//挂起, 并返回int = 101
	std::cout << "Coroutine stage 2 ,co_yield" << std::endl;
	co_yield 202;//挂起, 并返回int = 202
	std::cout << "Coroutine stage 3 ,co_yield" << std::endl;
	co_yield 303;//挂起, 并返回int = 303

	std::cout << "Coroutine stage end, co_return" << std::endl;
	co_return 808;//协程退出, 并返回int = 808
}



int main (void)
{
	bool done = false;

	// 调用协程,得到返回值c_r, 后面使用这个返回值来管理协程;
	std::cout << "Start coroutine_7in7out ()\n";
	auto c_r = coroutine_7in7out ();//coro_ret < int > c_r; 这个值的类型, 竟然不是coro_ret < int > , 直接使用auto 来逃避值类型的判断;

	// 第一次停止因为initial_suspend 返回的是suspend_always, 此时没有进入Stage 1
	std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get () << std::endl;
	done = c_r.move_next ();

	// 此时是, co_await std::suspend_always{}
	std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get () << std::endl;
	done = c_r.move_next ();

	// 此时打印Stage 1
	std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get () << std::endl;
	done = c_r.move_next ();
	std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get () << std::endl;
	done = c_r.move_next ();
	std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get () << std::endl;
	done = c_r.move_next ();
	std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get () << std::endl;
	return 0;
}
