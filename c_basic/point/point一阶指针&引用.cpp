//编译:
//		g++ -g3 ./point一阶指针\&引用.cpp -o x



#include<iostream>
#include<cstring>
#include"point_test_body.h"



//引用不适合: 函数指针;  函数指针不应该'引用'



void parr_test(int* parr, int* parr2){
	int tmp;

	//引用赋值
	for(tmp=0;tmp<64;tmp++)
		parr[tmp] = tmp;

	//引用赋值2
	for(tmp=0;tmp<64;tmp++)
		*(parr2+tmp)= tmp;
}

void parr_check(int* parr, int* parr2){
	int tmp;

	//指针访问值
	for(tmp=0;tmp<64;tmp++)
		std::cout << parr[tmp] << std::endl;

	//指针访问值2
	for(tmp=0;tmp<64;tmp++)
		std::cout << *(parr2+tmp) << std::endl;
		//std::cout << parr2[tmp] << std::endl;
}



void parr2_test(int (*parr)[32],int (*parr2)[32]){
	int tmp,row;

	//引用赋值
	for(row=0;row<32;row++)
		for(tmp=0;tmp<32;tmp++)
			parr[row][tmp] = tmp;

	//引用赋值2
	for(row=0;row<32;row++){
		for(tmp=0;tmp<32;tmp++)
			(*parr2)[tmp] = tmp;
		//后移一行
		parr2++;
	}
}

void parr2_check(int (*parr)[32],int (*parr2)[32]){
	int tmp,row;

	//指针访问值
	for(row=0;row<32;row++)
		for(tmp=0;tmp<32;tmp++)
			std::cout << parr[row][tmp] << std::endl;

	//指针访问值2
	for(row=0;row<32;row++){
		for(tmp=0;tmp<32;tmp++)
			std::cout << (*parr2)[tmp] << std::endl;
		//后移一行
		parr2++;
	}
}



void pbody_test(struct body* pbody){
	pbody->head = 1;
	pbody->hand = 1;
	pbody->penis = 1;
}

void pbody_check(struct body* pbody){
	std::cout << pbody->head << std::endl;
	std::cout << pbody->hand << std::endl;
	std::cout << pbody->penis << std::endl;
}



void pfather_test(class father* pfather){
	pfather->money = 999;
}

void pfather_check(class father* pfather){
	std::cout << pfather->money << std::endl;
}



void test1(void){
	int arr_i[64],arr_i2[64];
	int arr2_i[32][32],arr2_i2[32][32];
	struct body m_body;
	class father m_father;

	//parr_test(arr_i,arr_i2);
	//parr_check(arr_i,arr_i2);
	//parr2_test(arr2_i,arr2_i2);
	//parr2_check(arr2_i,arr2_i2);
	pbody_test(&m_body);
	pbody_check(&m_body);
	pfather_test(&m_father);
	pfather_check(&m_father);
	return;
}





int main(void){
	//一阶指针引用
	test1();

	return 0;
}
