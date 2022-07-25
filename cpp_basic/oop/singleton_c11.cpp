//编译:
//		g++ -std=c++11 -g3 singleton.cpp -o x



#include <iostream>
#include <cassert>



#ifndef _SINGLETON_C11_H
#define _SINGLETON_C11_H

namespace adan_cpplib{
	//c11 标准下的单例模式
	class singleton{
		public:
			static inline singleton& instance(){
				static singleton m_ins;
				return m_ins;
			}

			bool init_instance(void){
				if(!is_init){
					pm_ins = &instance();
					is_init = true;
					return true;
				}
					return false;
			}
			//用"=delete"去修饰函数, 表示该函数被deleted, 也就意味着这个函数不能再被调用, 否则就会出错
			//(c++11 标准下禁止编译器自动生成类basic构造函数的方法)
			singleton(const singleton&) = delete;
			singleton(singleton&) = delete;
			//'='等号赋值, 自动调用构造函数, 禁用
			singleton& operator=(const singleton&) = delete;
			singleton& operator=(singleton&) = delete;
		private:
			singleton() = default;
			~singleton() = default;
			bool is_init;
			singleton *pm_ins;
	};
}

#endif






int main(void){
	adan_cpplib::singleton &a = adan_cpplib::singleton::instance();
	adan_cpplib::singleton &b = adan_cpplib::singleton::instance();
	assert(&a == &b);



	if(a.init_instance())
		std::cout << "a.init_instance() okay" << std::endl;
	if(!b.init_instance())
		std::cout << "b.init_instance() failed" << std::endl;

	return 0;
}



