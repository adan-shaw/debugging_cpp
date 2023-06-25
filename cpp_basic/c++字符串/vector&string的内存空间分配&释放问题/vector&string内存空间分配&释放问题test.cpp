//编译:
//		g++ -std=c++11 ./vector\&string内存空间分配\&释放问题test.cpp -o x


#include<iostream>
#include<vector>
using namespace std;
using std::cout;
using std::cin;
using std::endl;



int main(void){
	int i;
	vector<int> ivec;
	//空初始化值
	cout << "capacity: " << ivec.capacity() << "  size: "  << ivec.size() << endl;

	//添加10个元素
	for(i = 0; i < 10; ++i){
		ivec.push_back(i);
		cout << "capacity: " << ivec.capacity() << "  size: " << ivec.size() << endl;
	}

	//将容量用完
	while(ivec.size() != ivec.capacity())
		ivec.push_back(0);

	//添加1个元素
	cout << "size = capacity. insert one element\n";
	ivec.push_back(0);
	cout << "capacity:" << ivec.capacity() << "  size:" << ivec.size() << endl;

	ivec.reserve(100);
	cout << "reserve capacity 100\n";
	cout << "capacity:" << ivec.capacity() << "  size:" << ivec.size() << endl;

	//将容量用完
	while(ivec.size() != ivec.capacity())
		ivec.push_back(42);

	//添加1个元素
	cout << "size = capacity. insert one element\n";
	ivec.push_back(0);
	cout << "capacity:" << ivec.capacity() << "  size:" << ivec.size() << endl;

	//尝试resize() 缩容
	ivec.resize(50);
	cout << "resize size 50\n";
	cout << "capacity:" << ivec.capacity() << "  size:" << ivec.size() << endl;

	//尝试reserve() 缩容
	ivec.reserve(80);
	cout << "reserve capacity 80\n";
	cout << "capacity:" << ivec.capacity() << "  size:" << ivec.size() << endl;

	//尝试shrink_to_fit() 缩容(只有这个缩容成功, c++11)
	ivec.shrink_to_fit();
	cout << "shrink_to_fit()\n";
	cout << "capacity:" << ivec.capacity() << "  size:" << ivec.size() << endl;

	return 0;
}
