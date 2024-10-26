//编译:
//		g++ -g3 ./cpp常见的四种构造函数.cpp -o x





void test_123(void){
	fprintf(stderr,"'显式构造函数'与'隐式构造函数'的区别[测试1]:\n");

	//Test1 t1;
	Test1 t1 = 12;
	//Test1 t1(999);

	//Test2 t2;
	//Test2 t2=12;
	Test2 t2(12);

	//没有默认赋值, 就必须赋值初始化类, 才能通过编译
	Test3 t3;
	Test3 t33 = 9999888;
	//Test3 t33(1110);
	return ;
}





void test_56(void){
	char test_text[] = "dont loveing kidding me !!";
	fprintf(stderr,"\n\n\n'拷贝构造函数'[测试2]:\n");

	//浅拷贝(没有指针实体拷贝)
	Test5 t51;
	t51.write_num_cp(999);//改写num_cp 拷贝目标值
	Test5 t52(t51);				//用'拷贝构造函数'的方式创建新的类t52[没有被拷贝的值num = 随机数]
	Test5 t53 = t51;			//同上, [没有被拷贝的值num = 随机数]

	//深拷贝(指针实体拷贝)
	Test6 t61;
	t61.write_num_cp(6666);
	t61.write_text("love you");
	t61.write_text(test_text);
	Test6 t62(t61);
	Test6 t63 = t61;
	return;
}





//Test8 mm = m + m2; 实际上是'两个'赋值拷贝构造函数 的调用了!!
//第一个调用, 是m + m2; 原理和'=' 号一样, m 由this 传入, m2 由(const Test8 &m2) 传入.
//第二个调用, 是mm=(m + m2) 的结果(还没想好, 慎用)

void test_8(void){
	fprintf(stderr,"\n\n\n'赋值拷贝构造函数'(即重载运算符号, 实现拷贝构造函数)[测试4]:\n");
	Test8 t81(100);
	Test8 t82(2);
	Test8 t83 = t81 + t82;
	Test8 t84 = t81 - t82;
	Test8 t85 = t81 * t82;
	Test8 t86 = t81 / t82;

	//比较不好的'赋值拷贝构造函数'的实现
	Test8 t87(1);					//没有默认构造函数:Test8(), 不能直接使用: Test8 t87; 必须要: Test8 t87(1)赋值
	t87 = t87 + t81;
	t87 = t87 + t82;

	//比较好的'赋值拷贝构造函数'的实现
	Test8 t88(2);
	t88 += t81;
	t88 += t82;
	return ;
}





int main(void){
	test_123();
	test_56();
	test_7();
	test_8();
	test_9();
	return 0;
}

