//编译:
//		测试二进制8bit 数据 && 字符串互转
//		gcc -g3 -DNODEBUG -nostartfiles bit_oper.h bit2str.c -e test_byte2str -o x


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "bit_oper.h"



//静态变量, 节省内存
static char buf[9];
static char b;

//io 引脚编号(8 bit 中取两bit), 由于8051/arm/... 基本上MCU 都是8bit 一组io 引脚, 这里就定义一个char 表示io 引脚;
struct Pin8{
	char :6;
	char p1:1;
	char p0:1;
};
struct Pin8 io;

#define SDA (io.p0) //数据线
#define SCL (io.p1) //时钟线



// 将字符串11110000 转换为二进制char(char = 8bit)
char str2byte_1(char *pstr){
	int tmp;
	for(tmp=0;tmp<8;tmp++)
		if(pstr[tmp] == '1')
			SET_BIT(b,tmp);
		else if(pstr[tmp] == '0')
			CLEAR_BIT(b,tmp);
		else{
			printf("str2byte_1() error: %d\n", pstr);
			return 0;
		}
	return b;
}

// 将二进制char(char = 8bit)转换为字符串11110000
void byte2str_1(char b, char* pret){
	int tmp;
	for(tmp=0;tmp<8;tmp++)
		if(IF_BIT(b,tmp))
			pret[tmp] = '1';
		else
			pret[tmp] = '0';
	return;
}



void test_byte2str(void){
	strncpy(buf, "11110000", 9);
	b = str2byte_1(buf);
	memset(&buf,0,9);
	byte2str_1(b, buf);
	printf("test_himself() = %s\n", buf);
	return;
}



//
// (弃用部分)
//
//通知所有人, 我拉低了时钟线(TCP 轮循)
void broadcast_SCL_down(char *node_addr){

}

//通知所有人, 我拉高了时钟线(TCP 轮循)
void broadcast_SCL_up(char *node_addr){
	
}


//发送数据(可能是addr_data, 可能是data; 一次发送8bit, 不模拟摇号过程了)


//接收数据(可能是addr_data, 可能是data; 一次发送8bit, 不模拟摇号过程了)


//发送应答位(全部应答某个发起addr)
void unicast_SDA(char *node_addr){

}
