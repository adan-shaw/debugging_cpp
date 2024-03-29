//
//定义指针测试全局变量
//



//数组-指针
int arr_i[64];
char arr_c[64];
double arr2_d[32][32];
float arr2_f[32][32];


//结构体-指针
struct body{
	int head;
	int hand;
	int penis;
	void* buf[32];
};
struct body m_body;


//类-指针
class father{
	public:
		father(){};
		~father(){};
		int money;
};
class father m_father;


//函数-指针(不可以位移)
int ret_int(void){
	return 999;
}
const int ret_const_int(void){
	return 999;
}

typedef int (*Func)(void);
typedef const int (*Func2)(void);


//任意指针
void* pvoid = NULL;


//二阶指针(指向指针的指针)
void** ppvoid = &pvoid;



//变量实体:
void __loveing_test(void){
int t;
int *pt;
int **ppt;

t = 1;									//'访问/修改'-实体变量的值
pt = &t;								//'访问'    -实体变量的存储地址(&)

//指针(一阶指针)
pt = &t;								//'访问/修改'-指针的值
ppvoid = (void**)&pt;		//'访问'    -指针的存储地址(二阶指针赋值时才访问,其他情况下,不访问指针存储地址)
*(pt+0) = 2;						//'引用'    -指针'指向的实体变量'的值, 进行'访问/修改'
pt[0] = 2;							//'引用'    -指针'指向的实体变量'的值, 进行'访问/修改'
												//***省略*** 一般不'引用'指针'指向的实体变量'的存储地址

//二阶指针(指向指针的指针)
ppt = &pt;							//'访问/修改'-二阶指针的值
*(*ppt+0) = 2;					//'引用'    -二阶指针'指向的实体变量'的值, 进行'访问/修改'
*ppt[0] = 2;						//'引用'    -二阶指针'指向的实体变量'的值, 进行'访问/修改'
												//***省略*** 一般不'访问'二阶指针的存储地址
												//***省略*** 一般不'引用'二阶指针'指向的一阶指针'的存储地址
												//***省略*** 一般不'引用'二阶指针'指向的一阶指针', 所指向的实体变量的存储地址

//行指针引用, 比较麻烦, 用[][]方法简单写, 用*() 指针的方法, 比较麻烦; 参考: ./point一阶指针\&引用.cpp
}



//内联函数不应该有指针
/*
	内嵌静态代码编译, 根本没有引用这种静态编译的函数;
	而且, 每个inline 函数地址都是不一样的, 根本没办法用指针(普通函数的地址是唯一的, 全局的)

	类函数的函数指针, 可以用类实体的地址偏移量来表示;
	内联函数的函数地址, 真的没办法保证, 随机的, 哪里用到这个函数, 函数地址就在哪, 混乱;
	所以内联函数是没有函数指针的
*/
//inline double ret_double(void){ return 999.999; }



//static静态变量/全局常量, 也是不需要指针的, 可以轻松全局索引得到, 不需要指针;



//左值, 右值:
/*
	左值 = 变量内存地址;
	右值 = 变量内存地址里面存放的数据;

	当左值 = 一根指针的时候, 那么:
		左值=这根指针的内存地址;
		右值=这根指针指向的变量地址;(前提是指针已经赋值)

	指针引用问题:
		假设有:
			int arr[64];
			int* p = arr;
		则:
			p[63] = 999;
			*(p+63) = 999;
*/
