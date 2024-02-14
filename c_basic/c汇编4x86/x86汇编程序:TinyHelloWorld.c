//编译器version: gcc 4.8.4 以上

//编译: 
//		gcc -c -fno-builtin x86汇编程序\:TinyHelloWorld.c -o x.o
//链接: 
//		ld -static -e nomain -o x.exe x.o



//测试结果: 打印出一个 "Hello world!"



char * str = "Hello world!\n";



// 使用x86汇编文打印字符串
void print(){
	asm(
		"movl $13,%%edx \n"
		"movl %0,%%ecx \n"
		"movl $0,%%ebx \n"
		"movl $4,%%eax \n"
		"int $0x80 \n"
		::"m"(str):"edx","ecx","ebx"
	);
	return;
}



// 使用x86汇编文退出程序
void exit(){
	asm(
		"movl $42, %ebx \n"
		"movl $1, %eax \n"
		"int $0x80 \n"
	);
	return;
}



// x86 程序入口
void nomain(){
	print();
	exit();
	return;
}
