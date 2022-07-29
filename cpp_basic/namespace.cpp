//编译:
//		g++ -g ./namespace.cpp -o x



#include<cstdio>

namespace adan{
	//命名空间中的类
	class A{
		public:
			A(){
				fprintf(stderr,"namespace adan -> class A()\n");
			}
	};

	//命名空间中的函数
	void adan_func(void){
		fprintf(stderr,"namespace adan -> adan_func()\n");
	}

	//命名空间中的变量
	int a = 1;

	//命名空间中的声明(外部定义的'类'or'函数')
	class B;
	void adan_func2(void);
}

//命名空间外的声明补充定义
class adan::B{
	public:
		B(){
			fprintf(stderr,"namespace adan -> class B()\n");
		}
};

void adan::adan_func2(void){
	fprintf(stderr,"namespace adan -> adan_func2()\n");
}



int main(void){
	adan::A a;
	adan::B b;
	adan::adan_func();
	adan::adan_func2();
	printf("a=%d\n",adan::a);
	return 0;
}
