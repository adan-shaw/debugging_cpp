//编译:
//	g++ -g3 ./demo范例\:goto.cpp -o x


//goto 少用的原因:
/*
	使用goto 不太安全, 必须指定main 函数才能正常工作, 不接受gnu 自定义入口函数
	(可能不固定的地址入口, 会导致地址偏移出错, 
	 所以goto 还是慎用比较好, 容易导致地址便宜出错的问题, 出了问题很难查)
		gcc -g3 -nostartfiles ./demo范例\:goto.cpp -e test_goto -o x
*/



// goto 的作用(一般用得比较少, 除非是频繁使用if 的场合, 但平凡使用if 本身就是有问题的)
/*
	如果大量错误, return 时需要重复释放资源, 
	那么这种情况下, 使用goto 能更高效地释放相同的资源(节省代码)

	但注意, goto 应该只出现在本函数逻辑中(即局部变量),
	而goto 标签, 不应该超出局部变量的范围!!

	而且goto 标签命名上, 通常是:
		xxx函数_fail:
*/



#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>



//goto 测试demo: 输入一个除数(除数为0,则goto_test_failed)
bool goto_test(int n){
	if(n == 0)
		goto goto_test_failed;

	return true;

goto_test_failed:
	printf("value 'ret' should <= 0!!\n");
	return false;
}



int main(void){
	int tmp;



	//1.测试goto语句
	tmp=0;
	printf("test1: goto_test(%d) = %d = false\n", tmp, goto_test(tmp));


	//2.测试不用goto的情况
	tmp=9;
	printf("test2: goto_test(%d) = %d = true\n", tmp, goto_test(tmp));


	//3.测试除数为0, 运行崩溃报错: Floating point exception
	//tmp=tmp/0;

	exit(0);
}
