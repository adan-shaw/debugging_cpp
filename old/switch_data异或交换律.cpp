//编译:
//		gcc -g3 switch_data异或交换律.cpp -o x



#include <stdio.h>




/*
交换两个常规变量的数据switch4b_N()函数的设计纲要:
	32/64 bit 系统差异:
		指针无论是int*,char*,void* 都一样, 只会根据系统32/64bit 变化而变化, 
		你在设计时, 没办法避免这样问题,
		但由于int 类型, 是永恒4 字节32bit 的!!
		char 类型, 也是永恒1 字节8bit 的
		因此char* 指针, 每次只移动1 字节8bit.
		(即便在64bit 系统中, char* 指针每次后移, 也是1 字节8bit)

		因此, 可以选定unsigned char* 为'交换指针', 交换粒度是: 1 字节8bit
		每次交换的数据量, 必须是:
				8bit * N [8bit 的N 倍, 最小粒度=8bit]
				1byte * N


	两个变量也需要相等的buf空间, 
	或者'两个变量中,最小的buf空间' > '交换空间量max'


	宏定义方法(失败):
		宏定义其实和内联函数差不多, 不要太抬举宏定义;
		只有在替换字符串的时候, 宏定义才有真实的优势!!
		定义宏函数, 优势不大, 还不如使用内联函数, 作用也差不多.

	#define switch64bit(a,b) ({ \ 
		a = (unsigned int*)(&a) ^ (&b); \
		b = (unsigned int*)(&a) ^ (&b); \
		a = (unsigned int*)(&a) ^ (&b); })
*/



//2 个变量交换'4 字节32bit * N'的数据(弃用)
/*
	(被交换的数据, 必须是对齐数据: (unsigned int* 指针)*N, N 至少大于1;)
	少于4 字节32bit, 会出现乱码, 扰乱数据的准确性!!
	非对齐数据'4 字节32bit * N', 同样会扰乱数据的准确性
	(交换后, buf头部/尾部, 会有少量错乱数据, 影响交换操作的准确性)
*/
inline void switch4b_N(unsigned int *pa, unsigned int *pb, unsigned int N){
	//N 少于1 不会执行交换, 直接退出for{}
	for(; N>0; N--,pa++,pb++){
		*pa = (*pa)^(*pb);
		*pb = (*pa)^(*pb);
		*pa = (*pa)^(*pb);
	}
	return;
}

//2 个变量交换'1 字节8bit * N'的数据(启用)
inline void switch1b_N(unsigned char *pa, unsigned char *pb, unsigned int N){
	//N 少于1 不会执行交换, 直接退出for{}
	for(; N>0; N--,pa++,pb++){
		*pa = (*pa)^(*pb);
		*pb = (*pa)^(*pb);
		*pa = (*pa)^(*pb);
	}
	return;
}





//测试函数for switch4b_N()
void test_switch4b_N(void){
	unsigned short s1=888,s2=77;
	unsigned int i1=999,i2=6666;
	float f1=111.1,f2=222.2;
	double d1=111.1,d2=222.2;
	char c1[8]="abcd1fg";
	char c2[8]="SitD2wn";
	char c3[16]="2itDown4bcdefg";
	char c4[16]="1bcdefg3itDown";
	char c5[12]="abcd1fgSit";
	char c6[12]="SitD2wnSit";
	char c55[11]="Abcd1fgSi";
	char c66[11]="QitD2wnSi";
	char c7[4]="abc";
	char c8[4]="sit";
	char c9[32]="2itDown4bcdefg1bcdefg3itDown";
	char c0[32]="1bcde2itDown4bcdefgfg5itDown";



	//打印unsigned int * 交换粒度
	printf("sizeof(unsigned int *)=%d\n",sizeof(unsigned int *));

	//交换16bit short(2 字节) -- 交换后数据错乱
	printf("sizeof(unsigned short)=%d\n",sizeof(unsigned short));
	printf("s1=%d,s2=%d\n",s1,s2);
	switch4b_N((unsigned int*)&s1, (unsigned int*)&s2 ,1);
	printf("s1=%d,s2=%d\n",s1,s2);
	printf("wrong!!\n");

	//交换32bit int(4 字节)
	printf("sizeof(unsigned int)=%d\n",sizeof(unsigned int));
	printf("i1=%d,i2=%d\n",i1,i2);
	switch4b_N((unsigned int*)&i1, (unsigned int*)&i2 ,1);
	printf("i1=%d,i2=%d\n",i1,i2);

	//交换32bit float(4 字节)
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("f1=%f,f2=%f\n",f1,f2);
	switch4b_N((unsigned int*)&f1, (unsigned int*)&f2, 1);
	printf("f1=%f,f2=%f\n",f1,f2);


	//交换64bit double(8 字节)
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("d1=%lf,d2=%lf\n",d1,d2);
	switch4b_N((unsigned int*)&d1, (unsigned int*)&d2 ,2);
	printf("d1=%lf,d2=%lf\n",d1,d2);


	//交换64bit-char(8 字节)
	printf("sizeof(char)=%d\n",sizeof(char));
	printf("c1=%s,c2=%s\n",c1,c2);
	switch4b_N((unsigned int*)&c1, (unsigned int*)&c2 ,2);
	printf("c1=%s,c2=%s\n",c1,c2);

	//交换128bit-char(16 字节)
	printf("c3=%s,c4=%s\n",c3,c4);
	switch4b_N((unsigned int*)&c3, (unsigned int*)&c4 ,4);
	printf("c3=%s,c4=%s\n",c3,c4);

	//交换96bit-char(12 字节, 4*3 也可以!!)
	printf("c5=%s,c6=%s\n",c5,c6);
	switch4b_N((unsigned int*)&c5, (unsigned int*)&c6 ,3);
	printf("c5=%s,c6=%s\n",c5,c6);

	//交换88bit-char(11 字节, 4*2+3, 就会出错!! 必须为4字节*N, 因为每次unsigned int* 后移4字节)
	printf("c55=%s,c66=%s\n",c55,c66);
	switch4b_N((unsigned int*)&c55, (unsigned int*)&c66 ,3);
	printf("c55=%s,c66=%s\n",c55,c66);

	//交换32bit-char(4 字节)
	printf("c7=%s,c8=%s\n",c7,c8);
	switch4b_N((unsigned int*)&c7, (unsigned int*)&c8, 1);
	printf("c7=%s,c8=%s\n",c7,c8);

	//交换256bit-char(32 字节)
	printf("c9=%s,c0=%s\n",c9,c0);
	switch4b_N((unsigned int*)&c9, (unsigned int*)&c0 ,8);
	printf("c9=%s,c0=%s\n",c9,c0);

	return;
}


//测试函数for switch1b_N()
void test_switch1b_N(void){
	unsigned short s1=888,s2=77;
	unsigned int i1=999,i2=6666;
	float f1=111.1,f2=222.2;
	double d1=111.1,d2=222.2;
	char c1[8]="abcd1fg";
	char c2[8]="SitD2wn";
	char c3[16]="2itDown4bcdefg";
	char c4[16]="1bcdefg3itDown";
	char c5[12]="abcd1fgSit";
	char c6[12]="SitD2wnSit";
	char c55[11]="Abcd1fgSi";
	char c66[11]="QitD2wnSi";
	char c7[4]="abc";
	char c8[4]="sit";
	char c9[32]="2itDown4bcdefg1bcdefg3itDown";
	char c0[32]="1bcde2itDown4bcdefgfg5itDown";



	//打印unsigned int * 交换粒度
	printf("sizeof(unsigned char*)=%d\n",sizeof(unsigned char*));

	//交换16bit short(2 字节)
	printf("sizeof(unsigned short)=%d\n",sizeof(unsigned short));
	printf("s1=%d,s2=%d\n",s1,s2);
	switch1b_N((unsigned char*)&s1, (unsigned char*)&s2 ,2);
	printf("s1=%d,s2=%d\n",s1,s2);

	//交换32bit int(4 字节)
	printf("sizeof(unsigned int)=%d\n",sizeof(unsigned int));
	printf("i1=%d,i2=%d\n",i1,i2);
	switch1b_N((unsigned char*)&i1, (unsigned char*)&i2 ,4);
	printf("i1=%d,i2=%d\n",i1,i2);

	//交换32bit float(4 字节)
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("f1=%f,f2=%f\n",f1,f2);
	switch1b_N((unsigned char*)&f1, (unsigned char*)&f2, 4);
	printf("f1=%f,f2=%f\n",f1,f2);


	//交换64bit double(8 字节)
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("d1=%lf,d2=%lf\n",d1,d2);
	switch1b_N((unsigned char*)&d1, (unsigned char*)&d2 ,8);
	printf("d1=%lf,d2=%lf\n",d1,d2);


	//交换64bit-char(8 字节)
	printf("sizeof(char)=%d\n",sizeof(char));
	printf("c1=%s,c2=%s\n",c1,c2);
	switch1b_N((unsigned char*)&c1, (unsigned char*)&c2 ,8);
	printf("c1=%s,c2=%s\n",c1,c2);

	//交换128bit-char(16 字节)
	printf("c3=%s,c4=%s\n",c3,c4);
	switch1b_N((unsigned char*)&c3, (unsigned char*)&c4 ,16);
	printf("c3=%s,c4=%s\n",c3,c4);

	//交换96bit-char(12 字节, 4*3 也可以!!)
	printf("c5=%s,c6=%s\n",c5,c6);
	switch1b_N((unsigned char*)&c5, (unsigned char*)&c6 ,12);
	printf("c5=%s,c6=%s\n",c5,c6);

	//交换88bit-char(11 字节, 4*2+3)
	printf("c55=%s,c66=%s\n",c55,c66);
	switch1b_N((unsigned char*)&c55, (unsigned char*)&c66 ,11);
	printf("c55=%s,c66=%s\n",c55,c66);

	//交换32bit-char(4 字节)
	printf("c7=%s,c8=%s\n",c7,c8);
	switch1b_N((unsigned char*)&c7, (unsigned char*)&c8, 4);
	printf("c7=%s,c8=%s\n",c7,c8);

	//交换256bit-char(32 字节)
	printf("c9=%s,c0=%s\n",c9,c0);
	switch1b_N((unsigned char*)&c9, (unsigned char*)&c0 ,32);
	printf("c9=%s,c0=%s\n",c9,c0);

	return;
}



//主测试函数
int main(void){
	//test_switch4b_N();
	test_switch1b_N();
	return 0;
}
