#include <memory>



//定义导出结构体
typedef struct {
	double x1;
	int x2;
} lib_struct_t;



//这两个函数, 只用作在dll/so 共享链接库中操作类, 至于类是单例, 还是工厂, 请在类定义中实现, 与dll 类导入, 导出函数无关
extern "C" {

//定义类(把类编程结构体, 再建两个新建和删除函数即可)
class test{
public:
	//在dll/so 共享链接库中定义导出类, 是不能有构造函数和析构函数的(至少目前作者自己也不会定义, 让他自动生成反而会成功)
	//test();
	//~test();
	void padd(void){ x+=1;printf("x=%d\n",x); };
private:
	int x = 0;
};

__attribute__ ((visibility("default"))) test* get_test(void);
//test* (*get_test)(void);

//只能有一个导出类, 搞不懂(不知道为何)
/*
//class test2 : public test{
class test2{
public:
	void padd2(void){ x+=1;printf("x=%d\n",x); };
private:
	int x = 0;
}

__attribute__ ((visibility("default"))) test2* get_test2(void);
//test2* (*get_test2)(void);


}



//(disable)导出类, 不在本demo 中实现



//dll/so 共享链接库中的全局变量, 静态变量:
/*
	static 静态变量:
		在所有情况下, static 修饰的全局变量/函数, 永远不会被'so/dll/exe可执行文件'模块外部看到;
		C++ 标准要求它们具有内部链接, 这意味着static 变量不对外开放访问, 等价于类里面public/private 的关系;
		外部类想要访问private 变量, 得做一个public 跳转;
		外部'so/dll/exe可执行文件'访问内部static 静态变量, 也需要做一个跳转;
		windows/linux 都不允许在共享链接库中的static 变量对外暴露;


	全局变量:
		当你的dll 中有全局变量时, 事情就变得复杂了(Windows 和Unix 系统完全不同):
			Windows(.exe和.dll):
				全局变量不是导出符号的一部分, 不能使用extern 关键字引用全局变量;

			Unix环境(.exe和.so):
				.so 共享链接库, 可以导出全局变量, 可以使用extern 关键字引用全局变量;

	总结:
		为了保证跨平台, 线程安全原则, 编写'so/dll 共享链接库', 尽量禁用全局变量, 减少差异;
		而使用static 变量时, 尽量保证仅供'so/dll 共享链接库'内部可见;
*/



//导出常规变量/指针实体, 存放再make_so_class.cpp



//
//so function point demo:
//
//windows/linux 要导出dll/so 共享链接库API 函数, 都需要两步:
/*
	1.windows/linux 要导出dll/so 共享链接库API 函数, 都需要加一个导出声明前缀:
		for linux:
			// demo: 强制命令main() 函数在.so 中对外可见
			__attribute__ ((visibility("default"))) int main(void) {}
		for windows:
			// demo: 强制命令main() 函数在.dll 中对外可见
			__declspec(dllexport)

		否则call 引用dll/so API 时, 会报错:
			dlsym() failed: undefined symbol API_name


	2.是否需要加extern "C" {}:
		无论linux/windows 平台, 都必须要加extern "C" {}, 否则导出dll API 时, 函数符号不能识别
		(非extern "C" 标准的函数符号, 大部分不能被识别, 调用时会找不到改API 函数符号)
		[在.h 头文件中添加extern "C" {}即可, .cpp 文件中不需要添加]
		linux:
			linux 定义so 导出函数时, c/c++ 函数都要强制加一个extern "C" {} ;
			gcc 编译器强大, 兼容能力强, 能直接在API 返回类型中, 返回c++ 类实体(new/malloc);
			(gcc 编译器统一当成指针来处理, 兼容能力强一点)
			因此, 在.so 共享链接库中使用std::string 等c++ 特性时,
			不用管, 照样定义就行, 别以为在extern "C" {} 中就不能使用c++ 特性, 具体解析交给编译器去做;
		windows:
			windows 定义dll 导出函数时, 如果使用到std::string 等c++ 特性时, 规则约束比linux 复杂, 有可能不通过;
*/

extern "C" {
	//返回结构体/类等非常规变量实体时, 只能使用指针返回; (严格执行线程安全函数标准, 这种写法要直接放弃)
	//使用这种方法时, 需要注意: 使用之后, 别忘了释放内存(谨慎使用)
	__attribute__ ((visibility("default"))) lib_struct_t* func1(double x1, int x2);
	//lib_struct_t* (*func1)(double, int);



	//改进写法for old c++: 这样定义和释放都在调用者端, 逻辑非常明确
	__attribute__ ((visibility("default"))) void func11(lib_struct_t* pStruct, double x1, int x2);
	//void (*func11)(lib_struct_t*, double, int);



	//智能指针写法(c++11 起步) [既然使用智能指针, 就拒绝使用裸指针; unique_ptr不允许复制, 但可以用std::move()进行转移, 转移即相当于mv, 而不是cp]
	__attribute__ ((visibility("default"))) std::unique_ptr<lib_struct_t> func111(double x1, int x2);
	//std::unique_ptr<lib_struct_t> (*func111)(double, int);

}// extern "C" -- end
