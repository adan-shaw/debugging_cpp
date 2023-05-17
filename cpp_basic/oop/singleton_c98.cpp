//编译:
//		g++ -std=c++98 -g3 singleton_c98.cpp -o x



#include <iostream>
#include <cassert>



#ifndef _SINGLETON_C98_H
#define _SINGLETON_C98_H

namespace adan_cpplib{
	//
	//c98 标准下的单例模式
	//

	//中间tmp 临时类, 用来禁止编译器生成'c++默认成员函数'
	class uncopyable{
		protected:
			//允许derived对象的构造和解析
			uncopyable(){}
			~uncopyable(){}
		private:
			//但阻止copying
			uncopyable(const uncopyable&);
			uncopyable& operator=(const uncopyable&);
	};

	//单例实体类
	class singleton:private uncopyable{
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
		private:
			//(c++98 标准下禁止编译器自动生成类basic构造函数的方法)
			singleton(const singleton&){};
			singleton(singleton&){};
			//'='等号赋值, 自动调用构造函数, 禁用
			singleton& operator=(const singleton&){};
			singleton& operator=(singleton&){};
			singleton(){};
			~singleton(){};
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

