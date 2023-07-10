//编译:
//		g++ -g3 ./cpp智能指针原理demo.cpp -o x



#include <iostream>
#include <string>



template<class T> class Smart_ptr{
public:
	Smart_ptr(T *t){
		std::cout << "Smart_ptr 智能指针-构造函数" << std::endl;
		this->t = t;
	}

	~Smart_ptr(){
		if(this->t != NULL){
			std::cout << "Smart_ptr 智能指针-析构函数" << std::endl;
			delete this->t;
		}
	}

	T* &operator->(){
		return this->t;
	}

private:
	T *t;
};



class Person{
public:
	Person(std::string name, int age) :name(name), age(age){
		std::cout << "Person 的构造函数" << std::endl;
	}

	Person(){
		std::cout << "Person 的默认构造" << std::endl;
	}

	void operator=(const Person& p){
		this->age = p.age;
		this->name = p.name;
	}

	~Person(){
		std::cout << "Person 的析构函数" << std::endl;
	}

	void Show(void){
		std::cout << this->name << "小朋友今年" << this->age << "岁了" << std::endl;
	}

private:
	std::string name;
	int age;
};



int main(void){
	Smart_ptr<Person> p = Smart_ptr<Person>(new Person("小花", 22));
	p->Show();
	return 0;
}
