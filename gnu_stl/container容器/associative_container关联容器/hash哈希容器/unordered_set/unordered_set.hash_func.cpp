//编译:
//		g++ -std=c++11 -g3 ./unordered_set.hash_func.cpp -w -o x 



#include <iostream>
#include <unordered_set>
#include <algorithm>

using namespace std;



class Customer{
	public:
		Customer(string _name):id(num++),name(_name){}	//构造函数id 自增
		~Customer() = default;

		friend ostream& operator<<(ostream& os,const Customer& a){
			os<<a.id<<" "<<a.name<<endl;									//重写了'<<'流输出运算符
			return os;
		}

		bool operator==(const Customer&a)const{
			return id == a.id;
		}

		int getId()const{
			return id;
		}

	private:
		static int num;
		int id;
		string name;
};

int Customer::num = 0;															//id 编号自增, 类Customer 的全局变量

class CustormerHash{																//hash function
	public:
		size_t operator()(const Customer& c)const {
			return c.getId();
		}
};

class CustormerKeyCmp{															//key compare function
	public:
		size_t operator()(const Customer& a, const Customer& b)const {
			//return a.getId() < b.getId() ? a.getId() : b.getId();
			return a.getId() == b.getId() ? true : false;
		}
};

//失败, hash 函数/比较函数, 不是一个函数指针, 而是一个子类!!
/*
//失败, hash 函数形参, 必须为一个类
int hash_func(const Customer& c){
	return c.getId();
}
//失败, 比较函数形参, 必须为一个类
int key_func(const Customer& a, const Customer& b){
	//return a.getId() < b.getId() ? a.getId() : b.getId();
	return a.getId() == b.getId() ? true : false;
}
*/



int main(void){
	unordered_set<Customer, CustormerHash, CustormerKeyCmp> x1;
	unordered_set<Customer, CustormerHash, CustormerKeyCmp>::iterator it;
	//unordered_set<Customer, hash_func, key_func> x2;						//失败
	//unordered_set<Customer, hash_func, key_func>::iterator it2;	//失败
	Customer m("陈0");

	x1.insert(m);
	x1.insert({"张1"});
	x1.insert({"赵2"});
	for_each(x1.begin(),x1.end(),[](const Customer& tmp){ cout << tmp ; });

	it=x1.find(m);
	if(it != x1.end())
		printf("1 -- x1.find(m) ok, id=%d\n", (*it).getId());
	else
		printf("1 -- x1.find(m) failed !!\n");

	return 0;
}
