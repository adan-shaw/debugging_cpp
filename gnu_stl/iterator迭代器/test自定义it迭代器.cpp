//编译:
//		g++ -g3 test自定义it迭代器.cpp -o x

#include<string>
#include<vector>
#include<iterator>
#include<iostream>



// 自定义迭代器func仿函数: my_swap
template <typename it>
void my_swap(it a, it b){
	typename std::iterator_traits<it>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}



int main(void){
	std::vector<std::string> words{"one", "two", "three"};



	std::cout << words.at(0).c_str() << words.at(1).c_str() << words.at(2).c_str() << std::endl;
	my_swap(std::begin(words), std::begin(words)+1);
	std::cout << words.at(0).c_str() << words.at(1).c_str() << words.at(2).c_str() << std::endl;

	return 0;
}
