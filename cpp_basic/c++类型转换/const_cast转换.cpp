//编译:
//		g++ -g3 ./const_cast转换.cpp -o x



//const_cast的作用:
/*
	一、常量指针 被强转为: 变量指针(可读写), 且仍然指向原来的对象;
	二、常量引用 被强转为: 变量引用(可读写), 且仍然指向原来的对象;
	三、常量对象 被强转为: 变量对象(可读写);
*/



#include <iostream>
using namespace std;

const int xx = 50;

class A{
public:
	int m_nNum;
	A(int nValue = 100);
};

A::A(int nValue) : m_nNum(nValue){}



void TestFun(){
	//第一种情况: const修饰指针指向对象(常量强转变量)
	const A *pA = new A(200);
	//pA->m_nNum = 100;						//compile error! pA指针指向的对象为常对象, 其成员变量值为只读的;
	A* pAA = const_cast<A*>(pA);	//去掉pA指针的const属性
	pAA->m_nNum = 199;						//pAA指针指向的对象为一般对象, 其成员变量值可读写;
	cout << pA->m_nNum << endl;		//199



	//第二种情况: const修饰指针
	A *pB = new A();
	pA = pB;											//思考这个原因;为什么这样子可以呢？且再看下面的这种情况:
	A* const pC = new A(1);
	cout << pC->m_nNum << endl;		//1
	A *pD = new A(2);
	//pC = pD;										//compile error! pC指针变量被const修饰, 其值是只读的;

	A* pE = const_cast<A*>(pC);		//去掉pC指针变量的const属性;再赋给指针引用变量
	pE = pD;
	cout << pC->m_nNum << endl;		//2

	A* pAS = const_cast<A*>(pC);	//去掉pC指针变量的const属性;再赋给一般指针变量
	pAS->m_nNum = 3;							//通过去掉const属性的指针变量修改其成员变量值
	cout << pC->m_nNum << endl;		//3



	//第三种情况:const修饰指针和指针对象
	const A* const pCC = new A(110);
	const A* pCC2 = const_cast<A*>(pCC);
	//pCC2->m_nNum = 119;					//error C3490: 由于正在通过常量对象访问"m_nNum", 因此无法对其进行修改
	pCC2 = NULL;
	A* const pCC3 = const_cast<A*>(pCC);
	pCC3->m_nNum = 119;
	//pCC3 = NULL;								//error C3892: "pCC3": 不能给常量赋值
	A* pCC4 = const_cast<A*>(pCC);
	pCC4->m_nNum = 120;
	pCC4 = NULL;



	//第四种情况:const修饰对象, 去const属性后赋给一般对象
	const A a;
	//a.m_nNum = 101;							//compile error! 常对象具有只读属性;
	A b = const_cast<A&>(a);
	b.m_nNum = 101;
	cout << a.m_nNum << endl;			//100
	cout << b.m_nNum << endl;			//101



	//第五种情况:const修饰对象, 去const属性后赋给引用对象
	const A c;
	//c.m_nNum = 101;							//compile error! 常对象具有只读属性;
	A& d = const_cast<A&>(c);
	d.m_nNum = 102;
	cout << c.m_nNum << endl;			//102
	cout << d.m_nNum << endl;			//102



	//第六种情况:const修饰对象, 对象指针去const属性后赋给指针
	const A e;
	//e.m_nNum = 103;							//compile error! 常对象具有只读属性;
	A* pe = const_cast<A*>(&e);
	pe->m_nNum = 103;
	cout << e.m_nNum << endl;			//103
	cout << pe->m_nNum << endl;		//103



	//第七种情况:const修饰局部变量
	const int xx = 50;
	int* yy = const_cast<int *>(&xx);
	*yy = 200;
	cout << xx << endl;						//50
	cout << *yy << endl;					//200
	int aa = xx;
	cout << aa << endl;						//50



	//第八种情况:const修饰局部变量;去const属性后赋给一般变量
	const int xxx = 50;
	int yyy = const_cast<int&>(xxx);
	yyy = 51;
	cout << xxx << endl;					//50
	cout << yyy << endl;					//51



	//第九种情况:const修饰局部变量;去const属性后赋给引用变量
	const int xxx2 = 50;
	int& yyy2 = const_cast<int&>(xxx2);
	yyy2 = 52;
	cout << xxx2 << endl;					//50
	cout << yyy2 << endl;					//52
}



int main(void){
	TestFun();
	return 0;
}

//run out(执行结果):
/*
199
1
2
3
100
101
102
102
103
103
50
200
50
50
51
50
52
*/
