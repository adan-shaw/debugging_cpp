struct _struct{
	int a;			//4
	int b;			//4
};						//8
struct _struct struct_tmp;

//共用同一块内存区域, 取最大的来算, 更改一个全部都更改;
union _union{
	int a;			//4
	int b;			//4
	char c[8];	//8
};						//8
union _union union_tmp;

//只能搞枚举体里面的值, 而且值的大小会固定+1, 默认从0开始;
enum _enum{
	_x0,
	_x1,
	_x2
};
int a = _x0;
enum _enum enum_tmp;



enum 枚举体:
1.替代宏定义, 自动顺序赋值, 你可以指定首个元素的值, 而后每个元素都会自动+1, 默认首个元素的值 = 0;
	作用:
		方便编程使用, 做记号;
		实际上编译器会将enum 枚举体替换为const 常量, 
		所以enum 枚举体是没有指针的, 别搞这么傻逼的东西;

2.枚举体本质上就是: const常量, 存放在.code区, 你也可以做一个字符串枚举体: enum a{ b = "b", c = "c"};
	因此, enum 枚举体:
		* 没有指针
		* 不能遍历(没有遍历的必要)
	枚举体一般是用来做关键数据记号的, 方便程序员记忆而已;

	数值型枚举体demo[从11 开始]:
	enum ENumber {
		ONE=11,
		TOW,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NIGHT,
		TEN,
		NUM_MAX
	};



//枚举体声明(默认自动从0 开始, 这里指定从10开始)
enum DAY{
	MON=10, TUE, WED, THU, FRI, SAT, SUN
};
/*
(1) 枚举型是一个集合, 集合中的元素(枚举成员)是一些命名的整型常量, 元素之间用逗号, 隔开
(2) DAY是一个标识符,可以看成这个集合的名字,是一个可选项,即是可有可无的项
(3) 第一个枚举成员的默认值为整型的0, 后续枚举成员的值在前一个成员上加1
(4) 可以人为设定枚举成员的值, 从而自定义某个范围内的整数
(5) 枚举型是预处理指令#define的替代(但是枚举体是const实化的,宏只是预处理)
(6) 类型定义以分号;结束
*/

