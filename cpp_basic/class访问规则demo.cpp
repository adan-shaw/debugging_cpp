//编译:
//		gcc -g3 ./class访问规则demo.cpp -o x


//public, private, protected 访问规则:
/*
	public:
		任意类, 都可以互相访问('类定义期间'的自我访问or '生成类实体后'互相访问)

	private:
		私有数据, 只能在'类定义期间'的自我访问('生成类实体后'互相访问, 需要在public 做跳板API[一般只有单例模式才会这样做])
		私有数据子类也不能继承;

	protected:
		保护数据, 类似private私有数据, 只能在'类定义期间'的自我访问, 
		或者派生类(即子类), 在'类定义期间'的自我访问('生成类实体后'互相访问, 需要在public 做跳板API)
		protected相当于: 可以继承的private;

	'类定义期间', 可以访问public, private, protected;
	'生成类实体后', 只能访问public, 访问private, protected 都需要做跳板API;
	('生成类实体后', 类实体也是不能自己访问自己的private, protected的!!)
*/



class CBase{
	private: int nPrivate;		//私有成员
	public: int nPublic;			//公有成员
	protected: int nProtected;//保护成员
};

class CDerived:public CBase{
	void AccessBase(){
		nPublic = 1;						//OK
		//nPrivate = 1;					//错, 不能访问父类私有成员
		nProtected = 1;					//OK, 访问从父类继承的protected成员
		//CBase f;
		//f.nProtected = 1;			//(偷换概念也没用, 在子类的构造函数中再定义一个父类实体, 也是没办法访问保护成员的)
	}
};

int main(void){
	CBase b;
	CDerived d;
	//int n = b.nProtected ;	//错, 不在派生类的成员函数内, 不能访问父类保护成员
	//n = d.nPrivate;					//错, 不能访问父类的私有成员
	int m = d.nPublic;				//OK
	return 0;
}
