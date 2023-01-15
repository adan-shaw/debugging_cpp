//编译:
//		g++ -std=c++11 -g3 ./c++迭代器辅助函数.cpp -o x



//迭代器辅助函数list:
/*
	advance(it, n)					it表示某个迭代器,n 为整数; 该函数的功能是将it迭代器前进或后退n个位置;
	distance(first, last)		first和last都是迭代器; 该函数的功能是计算first和last之间的距离;
	iter_swap(first, last)	first和last都是迭代器; 该函数是用来交换两个迭代器所指的元素值;

	begin(cont)							cont表示某个容器; 该函数可以返回一个指向cont容器中第一个元素的迭代器;
	end(cont)								cont表示某个容器; 该函数可以返回一个指向cont容器中最后一个元素之后位置的迭代器;
	prev(it)								it为指定的迭代器; 该函数默认可以返回一个指向上一个位置处的迭代器(注意,it至少为双向迭代器)
	next(it)								it为指定的迭代器; 该函数默认可以返回一个指向下一个位置处的迭代器(注意,it最少为前向迭代器)
*/

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
#include <cassert>
#include <deque>
#include <algorithm>



int main(void){
	int tmp,size;
	std::deque<int> x(40,100);
	std::deque<int>::iterator it, it2;



	//
	//第1部分
	//
	it = x.begin();
	std::advance(it, 10);										//advance(): 无返回值, 直接操作, 不会返回一个新的迭代器;
	*it += 10;
	printf("it=%d\n", *it);

	it2 = x.end();
	tmp = std::distance(it, it2);						//distance(): 正向迭代器, 不能于逆向迭代器一起比较, 否则编译器报错;
	printf("it[10]-->end()=it2[40] == [%d]\n", tmp);

	std::iter_swap(it, --it2);							//iter_swap()
	printf("it=%d,it2=%d\n", *it, *it2);



	//
	//第2部分
	//
	it = std::begin(x);							//begin()
	*it += 3;
	printf("%d\n", *it);
	printf("%d\n", *std::next(it));	//next()

	//it = std::end(x);							//end(): end() 等于第101 个元素, 一般情况下, end() 不等于容器最后一个元素;
	//it = std::prev(it);
	it = std::prev(std::end(x));		//it = std::prev(std::end(x)); 才是容器最后一个元素
	*it += 4;
	printf("%d\n", *it);
	printf("%d\n", *std::prev(it));	//prev()



	//打印整个deque, 查看最终结果
	assert(!x.empty());
	for(tmp=0,size=x.size(); tmp<size; tmp++)
		printf("at(%d)=%d  ", tmp, x.at(tmp));
	printf("\n");

	return 0;
}
