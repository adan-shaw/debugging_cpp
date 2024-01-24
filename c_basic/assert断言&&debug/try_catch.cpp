//
// try{}catch(...){}的实验和测试:
//
/*
	结论:
		try{}catch(...){} 还是不能处理空指针, 废物!!(不能解决致命错误的)

		try{}catch(...){} 异常机制, 一般都是用来:
			1.解决大型c++ 类程序中, 打印类错误的方法.
			2.也可以用来打印struct 结构体中的错误.
			3.unknow 未知错误, 你可以使用(...) 最后的模式, 打印这段代码有错误的警告, 告诉编码员, 这段代码有问题.
				不过还是不如c/c++ 直接定位xxx.cpp->第n行, 这种定位方式更实际.
				除非是大型程序需要调试class, struct 中的值, try{}catch(...){} 或者很有用.
*/


//编译:
//	g++ ./try_catch.cpp -g3 -o x.exe



#include <stdio.h>
#include <string.h>


struct s{
	int a;
};

class c{
public:
	int b;
};




int main(void){
	struct s *p = NULL;

	//int 异常-容器
	int excep_int=0;
	//struct 异常-容器
	s excep_struct;
	//class 异常-容器
	c excep_class;
	//字符串 异常-容器
	char excep_str[64];



	try{
		//空指针访问错误测试!! 结果: try{}catch(...){} 还是没办法解决'内存越界访问的问题'
		//p->a = 10;

		//终止执行, 抛出int 整形异常666
		excep_int = 999;
		//throw excep_int;

		//终止执行, 抛出struct 结构体异常
		excep_struct.a = 555;
		//throw excep_struct;

		//终止执行, 抛出class 类异常
		excep_class.b = 111;
		//throw excep_class;

		//终止执行, 抛出字符串常量异常
		strncpy(excep_str,"fuck you",64);
		throw excep_str;
		//只能抛出变量实体本身, 不能直接抛出字符串常量"fuck you"
		//throw "fuck you";//错误写法!!
	}



	//先尝试抓住'int整形异常'
	catch(int excep_int){
		printf("excep_int: %d\n",excep_int);
	}


	//再尝试抓住'struct结构体异常'
	catch(s excep_struct){
		printf("excep_struct.a: %d\n",excep_struct.a);
	}


	//再尝试抓住'class类异常'
	catch(c excep_class){
		printf("excep_class.b: %d\n",excep_class.b);
	}


	//再尝试抓住'字符串异常'
	catch(char excep_str[64]){
		printf("excep_str: %s\n",excep_str);
	}



	//抓住所有异常(... 不能承接, 你也不知道哪个变量类型会被抛出, 这里只能打印报错.)
	catch(...){
		//printf("...: %s\n",...);
		printf("unknow exception, it's a big mistake !!\n");
	}


	//标准c++ 并没有finally{}, 这或许是微软的私货
	//finally{}





	return 0;
}
