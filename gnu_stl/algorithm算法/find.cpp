//编译:
//		g++ -std=c++11 -g3 ./find.cpp -o x 



//std::find(), std::next(), std::prev() 等函数, 是正向/逆向, 主要看迭代器类型;
/*
	正向迭代器:
		std::find() 正向迭代器从第一个元素开始查找, 找到匹配值即返回指向该值位置的正向迭代器;
		std::next() 正向迭代器=第一个元素, next等于第二个元素;

	逆向迭代器:
		std::find() 逆向迭代器从最后一个元素开始查找, 找到匹配值即返回指向该值位置的逆向迭代器;
		std::next() 逆向迭代器=最后一个元素, next等于倒数二个元素;
		std::next() 逆向迭代器=倒数二个元素, prev等于倒数一个元素;

	ps:
		std::find() 使用正向迭代器时, 需要使用x.begin(), x.end();
		std::find() 使用逆向迭代器时, 需要使用x.rbegin(), x.rend();
*/



#include <cstdio>
#include <list>
#include <cassert>
#include <algorithm>



int main(void){
	int tmp;
	int arr_tmp[] = {16,2,77,29,29,29,29};
	std::list<int> x (arr_tmp, arr_tmp + sizeof(arr_tmp)/sizeof(int));
	std::list<int>::iterator it;						//顺向迭代器
	std::list<int>::reverse_iterator rit;		//逆向迭代器



	tmp = 29;
	it=std::find(x.begin(),x.end(),tmp);		//正向查找
	if(it != x.end())
		printf("it=std::find(x.begin(),x.end(),%d); found it okay!\n",tmp);
	else
		printf("target-%d cant find\n",tmp);
	printf("%d-%d-%d\n", *std::next(it),*std::next(it),*std::next(it));



	tmp = 29;
	rit=std::find(x.rbegin(),x.rend(),tmp);	//逆向查找
	if(rit != x.rend())
		printf("rit=std::find(x.rbegin(),x.rend(),%d); found rit okay!\n",tmp);
	else
		printf("target-%d cant find\n",tmp);
	printf("%d-%d-%d\n", *std::next(rit),*std::next(rit),*std::next(rit));


	return 0;
}
