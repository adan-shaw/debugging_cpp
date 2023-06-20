//编译:
//	gcc -g3 ./swap_data.c swap_data.h -o x



#include<stdio.h>
#include"swap_data.h"



//测试函数for sw_4byteN()
void test_sw_4byteN(void){
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
	sw_4byteN((unsigned int*)&s1, (unsigned int*)&s2 ,1);
	printf("s1=%d,s2=%d\n",s1,s2);
	printf("wrong!!\n");

	//交换32bit int(4 字节)
	printf("sizeof(unsigned int)=%d\n",sizeof(unsigned int));
	printf("i1=%d,i2=%d\n",i1,i2);
	sw_4byteN((unsigned int*)&i1, (unsigned int*)&i2 ,1);
	printf("i1=%d,i2=%d\n",i1,i2);

	//交换32bit float(4 字节)
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("f1=%f,f2=%f\n",f1,f2);
	sw_4byteN((unsigned int*)&f1, (unsigned int*)&f2, 1);
	printf("f1=%f,f2=%f\n",f1,f2);


	//交换64bit double(8 字节)
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("d1=%lf,d2=%lf\n",d1,d2);
	sw_4byteN((unsigned int*)&d1, (unsigned int*)&d2 ,2);
	printf("d1=%lf,d2=%lf\n",d1,d2);


	//交换64bit-char(8 字节)
	printf("sizeof(char)=%d\n",sizeof(char));
	printf("c1=%s,c2=%s\n",c1,c2);
	sw_4byteN((unsigned int*)&c1, (unsigned int*)&c2 ,2);
	printf("c1=%s,c2=%s\n",c1,c2);

	//交换128bit-char(16 字节)
	printf("c3=%s,c4=%s\n",c3,c4);
	sw_4byteN((unsigned int*)&c3, (unsigned int*)&c4 ,4);
	printf("c3=%s,c4=%s\n",c3,c4);

	//交换96bit-char(12 字节, 4*3 也可以!!)
	printf("c5=%s,c6=%s\n",c5,c6);
	sw_4byteN((unsigned int*)&c5, (unsigned int*)&c6 ,3);
	printf("c5=%s,c6=%s\n",c5,c6);

	//交换88bit-char(11 字节, 4*2+3, 就会出错!! 必须为4字节*N, 因为每次unsigned int* 后移4字节)
	printf("c55=%s,c66=%s\n",c55,c66);
	sw_4byteN((unsigned int*)&c55, (unsigned int*)&c66 ,3);
	printf("c55=%s,c66=%s\n",c55,c66);

	//交换32bit-char(4 字节)
	printf("c7=%s,c8=%s\n",c7,c8);
	sw_4byteN((unsigned int*)&c7, (unsigned int*)&c8, 1);
	printf("c7=%s,c8=%s\n",c7,c8);

	//交换256bit-char(32 字节)
	printf("c9=%s,c0=%s\n",c9,c0);
	sw_4byteN((unsigned int*)&c9, (unsigned int*)&c0 ,8);
	printf("c9=%s,c0=%s\n",c9,c0);

	return;
}



//测试函数for sw_1byteN()
void test_sw_1byteN(void){
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
	sw_1byteN((unsigned char*)&s1, (unsigned char*)&s2 ,2);
	printf("s1=%d,s2=%d\n",s1,s2);

	//交换32bit int(4 字节)
	printf("sizeof(unsigned int)=%d\n",sizeof(unsigned int));
	printf("i1=%d,i2=%d\n",i1,i2);
	sw_1byteN((unsigned char*)&i1, (unsigned char*)&i2 ,4);
	printf("i1=%d,i2=%d\n",i1,i2);

	//交换32bit float(4 字节)
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("f1=%f,f2=%f\n",f1,f2);
	sw_1byteN((unsigned char*)&f1, (unsigned char*)&f2, 4);
	printf("f1=%f,f2=%f\n",f1,f2);


	//交换64bit double(8 字节)
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("d1=%lf,d2=%lf\n",d1,d2);
	sw_1byteN((unsigned char*)&d1, (unsigned char*)&d2 ,8);
	printf("d1=%lf,d2=%lf\n",d1,d2);


	//交换64bit-char(8 字节)
	printf("sizeof(char)=%d\n",sizeof(char));
	printf("c1=%s,c2=%s\n",c1,c2);
	sw_1byteN((unsigned char*)&c1, (unsigned char*)&c2 ,8);
	printf("c1=%s,c2=%s\n",c1,c2);

	//交换128bit-char(16 字节)
	printf("c3=%s,c4=%s\n",c3,c4);
	sw_1byteN((unsigned char*)&c3, (unsigned char*)&c4 ,16);
	printf("c3=%s,c4=%s\n",c3,c4);

	//交换96bit-char(12 字节, 4*3 也可以!!)
	printf("c5=%s,c6=%s\n",c5,c6);
	sw_1byteN((unsigned char*)&c5, (unsigned char*)&c6 ,12);
	printf("c5=%s,c6=%s\n",c5,c6);

	//交换88bit-char(11 字节, 4*2+3)
	printf("c55=%s,c66=%s\n",c55,c66);
	sw_1byteN((unsigned char*)&c55, (unsigned char*)&c66 ,11);
	printf("c55=%s,c66=%s\n",c55,c66);

	//交换32bit-char(4 字节)
	printf("c7=%s,c8=%s\n",c7,c8);
	sw_1byteN((unsigned char*)&c7, (unsigned char*)&c8, 4);
	printf("c7=%s,c8=%s\n",c7,c8);

	//交换256bit-char(32 字节)
	printf("c9=%s,c0=%s\n",c9,c0);
	sw_1byteN((unsigned char*)&c9, (unsigned char*)&c0 ,32);
	printf("c9=%s,c0=%s\n",c9,c0);

	return;
}



//主测试函数
int main(void){
	test_sw_4byteN();
	test_sw_1byteN();
	return 0;
}





