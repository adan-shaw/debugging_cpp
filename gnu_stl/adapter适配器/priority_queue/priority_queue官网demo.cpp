//编译:
//		g++ -std=c++11 -g3 ./priority_queue官网demo.cpp -o x



#include <queue>
#include <vector>
#include <iostream>
#include <functional>



using namespace std;



void test(void){
	priority_queue<int> c1;
	c1.push(5);
	cout << c1.size() << '\n';

	priority_queue<int> c2(c1);
	cout << c2.size() << '\n';

	vector<int> vec={3, 1, 4, 1, 5};
	priority_queue<int> c3(less<int>(), vec);
	cout << c3.size() << '\n';
	return ;
}



template<typename T> void print_queue(T& q) {
	while(!q.empty()) {
		cout << q.top() << " ";
		q.pop();
	}
	cout << '\n';
}



void test2() {
	priority_queue<int> q;

	for(int n : {1,8,5,6,3,4,0,9,7,2})
		q.push(n);

	print_queue(q);

	priority_queue<int, vector<int>, greater<int> > q2;

	for(int n : {1,8,5,6,3,4,0,9,7,2})
		q2.push(n);

	print_queue(q2);

	// 用 lambda 比较元素. 
	auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1);};
	priority_queue<int, vector<int>, decltype(cmp)> q3(cmp);

	for(int n : {1,8,5,6,3,4,0,9,7,2})
		q3.push(n);

	print_queue(q3);
	return ;
}



int main(void){
	test();
	test2();
	return 0;
}
