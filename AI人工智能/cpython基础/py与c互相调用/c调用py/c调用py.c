//编译:
//		gcc -g3 ./c调用py.c -o x -lpython2.7
//		gcc -g3 ./c调用py.c -o x -lpython3.7m



#include <stdio.h>
#include <stdlib.h>
//本套代码同时支持python2.7/3.7
//#include <python2.7/Python.h>
#include <python3.7/Python.h>



//c调用py
/*
	c/c++ 也可以像lua 一样, 把python 直接装进c/c++ 内部, 当成一个python 沙盒一样使用, 用来计算部分python 程序, 非常方便;
	这种模式可以设计出skynet, openresty 之类的关键程序出来, 
	但问题是:
		人家已经做出来了, 而且软件工程质量非常可以, 想要把人家打倒, 几乎是不太可能的, 好好想想怎么用python 比较好;
		但这里只是提供一点理论依据, 如果有这方面的想法, 如果有这方面的需求, 可以想想的;
*/



//安装依赖库(当然必须先安装python, 否则编译链接的时候, 找不到runtime library, 找到了也不能执行的)
//	apt-get install python-dev python3-dev 



#define path_pyfile "./c调用py.py"

void run(void){
	//定义python 变量
	PyObject *pModuleName, *pModule, *pFunc;	//模块, 函数
	PyObject *type, *value;										//常规变量
	PyObject *traceback;											//处理致命错误的trace 变量

	//设置python 的runtime path(windows 专用, linux 可以省了)
	//Py_SetPythonHome("C:\Programs\Python\Python35-32");

	/*
		奇怪的就是: 
			python 不会像lua 一样, 返回一个可以供c/c++ 操纵的lua 实体, 
			python 直接就是init() 之后, 就默认装载了一个python 机器, 
			但不能多装载, 一个进程一个python only?
		这是个硬伤设计!!
	*/
	//初始化py 环境
	Py_Initialize();
	if (!Py_IsInitialized()){
		printf("Py_IsInitialized() failed!!");
		return;
	}

	//相当于在python中执行: import os (这个函数是宏, 相当于直接运行python语句, 可以用这句话, 不停写python 语句)
	PyRun_SimpleString("import time");
	PyRun_SimpleString("print(time.time())");
	PyRun_SimpleString("print(\"i am your father!! down on your knees!!\")");

	//直接读取*.py 文件中的代码也可以
	FILE *pFile = fopen(path_pyfile, "r");
	PyRun_SimpleFile(pFile, path_pyfile);

	//释放python 资源
	Py_Finalize();
}



int main(void){
	run();
	return 0;
}
