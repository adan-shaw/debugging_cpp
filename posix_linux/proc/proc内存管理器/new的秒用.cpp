//使用指针创建块内存时, 顺便赋值(new 时指定值)
int new_val(){
	int *p = new int(100);				//单个
	printf("%d\n",p[0]);
	return 0;

	int *p = new int[2]{100,100};	//多个
	printf("%d\n",p[0]);
	return 0;
}



//new 如何只调用'由malloc()创建的类'的构造函数, 实现malloc 创建一个完整的类
//查看: malloc创建类&调用构造函数/test_all_space.cpp



//new 如何只分配类的内存, 不调用构造函数, 类似malloc()
//** 还没实现, 没有实现的必要, 这样不如直接调用malloc() **



//malloc & new 创建的变量的空间区别:
/*
1.空间区域的不同:
	malloc 创建的buf, 在'进程堆'中创建;
	new 创建的buf, 在'自由存储区'中创建, 
	但'自由存储区'其实也是堆实现的, 只不过被编译器翻译成: 存放到不一样的区域而已

2.new 会自动调用构造函数, delete 会自动调用析构函数;
	malloc/free 则不会自动调用;
*/
