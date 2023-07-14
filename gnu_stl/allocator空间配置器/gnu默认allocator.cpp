//编译:
//		g++ -g3 gnu默认allocator.cpp -o x



//gun 默认allocator 十分简陋, 没有二级重用, 低级封装, 等于new/delete, 没啥用;



#include <memory>
#include <iostream>



using namespace std;



class Animal{
	public:
	//即使为0, 没有默认构造也是可以
	#if 1
		Animal() : num(0){
			cout << "Animal constructor default" << endl;
		}
	#endif

		Animal(int _num) : num(_num){
			cout << "Animal constructor param" << endl;
		}

		~Animal(){
			cout << "Animal destructor" << endl;
		}

		void show(){
			cout << this->num << endl;
		}

	private:
		int num;
};



//测试gnu 公版的allocator 的简单使用
int main(void){
	allocator<Animal> alloc;					//1.
	Animal *a = alloc.allocate(5);		//2.

	//3.
	alloc.construct(a, 1);
	alloc.construct(a + 1);
	alloc.construct(a + 2, 3);
	alloc.construct(a + 3);
	alloc.construct(a + 4, 5);

	//4.
	a->show();
	(a + 1)->show();
	(a + 2)->show();
	(a + 3)->show();
	(a + 4)->show();

	//5.
	for (int i = 0; i < 5; i++){
		alloc.destroy(a + i);
	}
	//对象销毁之后还可以继续构建, 因为构建和内存的分配是分离的
	//6.
	alloc.deallocate(a, 5);

	cin.get();
	return 0;
}
