//编译:
//		g++ -g3 ./cpp三大特性\:封装,继承,多态.cpp -o x 



#include <cstdio>
#include <cstring>

//父类father
class father{
public:
	//father(): father(99,88) {}
	father(const int n = 10, const int m = 20){
		num = n;
		num2 = m;
		num3 = n*m;
		fprintf(stderr,"father: num=%d,num2=%d,num3=%d\n",num,num2,num3);
	}
	void print_text(void){
		fprintf(stderr,"class father print_text()\n");
	}
	int num3;

private:
	int num;

protected:
	int num2;
};

//子类son, public公有继承父类father
class son : public father{
	public:
		//子类必须调用: father(2,2) 构造函数, 初始化继承类部分
		son(const int n = 100, const int m = 200):father(1,1){
			x1 = n;
			x11 = m;
			//公有继承int num3
			fprintf(stderr,"son: x1=%d,x11=%d,num3=%d\n",x1,x11,num3);
			print_text();
		}
	private:
		int x1;

	protected:
		int x11;
};

//子类son2, protected保护继承父类father
class son2 : protected father{
	public:
		//子类必须调用 : father(2,2) 构造函数, 初始化继承类部分
		son2(const int n = 101, const int m = 201):father(2,2){
			x2 = n;
			x22 = m;
			//公有继承int num3, 保护继承int num2
			fprintf(stderr,"son2: x2=%d,x22=%d,num2=%d,num3=%d\n",x2,x22,num2,num3);
			print_text();
		}
	private:
		int x2;

	protected:
		int x22;
};

//子类son3, private私有继承父类father
class son3 : private son2{
public:
	//子类必须调用 : father(2,2) 构造函数, 初始化继承类部分
	son3(const int n = 1000, const int m = 2000){
		x3 = n;
		x33 = m;
		//公有继承int num3, 保护继承int num2, 保护继承int x22
		fprintf(stderr,"son3: x3=%d,x33=%d,num2=%d,num3=%d,x22=%d\n",x3,x33,num2,num3,x22);
		print_text();
	}
private:
	int x3;

protected:
	int x33;
};



//测试类继承
void test_class_inherited(void){
	father* p0;
	son* p1;
	son2* p2;
	son3* p3;

	fprintf(stderr,"封装, 继承[测试1]\n");
	father x0(-1,-1);
	fprintf(stderr,"\n\n");
	son x1(1,1);
	fprintf(stderr,"\n\n");
	son2 x2(2,2);
	fprintf(stderr,"\n\n");
	son3 x3(3,3);
	fprintf(stderr,"\n\n");


	fprintf(stderr,"封装, 继承之间, 指针乱串(父类指针, 子类指针)[测试2]\n");
	fprintf(stderr,"父类指针, 指向子类, 同样调用出public 变量\n");
	p0 = &x1;
	p0->print_text();
	fprintf(stderr,"public int value num3 = %d\n",p0->num3);

	fprintf(stderr,"子类指针, 指向父类, 同样调用出public 变量\n");
	p1 = (son*)&x0;
	p1->print_text();
	fprintf(stderr,"public int value num3 = %d\n",p1->num3);


	//跨代访问失败
	/*
		p0 = (son3*)&x3;//父类指针, 指向孙类, 同样调用出public 变量
		p0->print_text();
		fprintf(stderr,"public int value num3 = %d\n",p0->num3);

		p3 = (son3*)&x0;//父类指针, 指向孙类, 同样调用出public 变量
		p3->print_text();
		fprintf(stderr,"public int value num3 = %d\n",p3->num3);
	*/


	// 私有'变量/函数'不允许'运行时外部访问'(只允许在类定义期间, 内部访问)
	//x0.num = 1;

	return;
}



int main(void){
	test_class_inherited();
	return 0;
}
