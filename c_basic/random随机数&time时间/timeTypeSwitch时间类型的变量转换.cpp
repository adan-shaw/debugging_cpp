//编译: 
//	g++ -lm -g3 ./timeTypeSwitch时间类型的变量转换.cpp -o x 



//
// time.h <格式转换> 有[线程安全_r 版本, 非线程安全版本]两种
//
/*
	1 将struct tm对象转换成char* 文本(32 bit 左右)
			失败返回: NULL, errno = EOVERFLOW
			成功返回: char* pbuf指针
	char* asctime(const struct tm* ptm);
	char* asctime_r(const struct tm* ptm, char* pbuf);//线程安全版本


	2 将time_t转换成char* 文本
			失败返回: NULL
			成功返回: char* pbuf指针
	char* ctime(const time_t* ptime);
	char* ctime_r(const time_t* ptime, char* pbuf);


	3 将time_t转换为struct tm(格林威治时间)
			失败返回: NULL
			成功返回: struct tm* 指针
	struct tm* gmtime(const time_t* ptime);
	struct tm* gmtime_r(const time_t* ptime, struct tm* presult);


	4 将time_t转换为struct tm(本地时间)
			失败返回: NULL
			成功返回: struct tm* 指针
	struct tm* localtime(const time_t* ptime);
	struct tm* localtime_r(const time_t* ptime, struct tm* presult);


	5 将struct tm转换为time_t格式
			(无线程安全版本, 少用, 应该不是线程安全的. )
			失败返回: -1
			成功返回: time_t 对应的值
	time_t mktime(struct tm* ptm);


	6 宽字符操作
	strftime() 将struct tm 对象转换成string自定义文本, 定义于头文件 <wchar.h>
	wcsftime	 将struct tm 对象转换成string自定义宽字符文本
*/





#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>



//声明:
//	由于ctime(), gmtime()等单线程版本, 实际用途不大,
//	且又是单线程的, 这里就不再描述这些版本的使用了.

//	尽量不要用(格林威治时间GMT), hour 不准!! 
//	中国16 小时, GMT=8 小时, 可能是不同时区的缘故





//
//1.时间time_t, struct tm, 与string字符串的转换.
//
void time2string(void){
	char str_time[64] = "";
	time_t t = time(NULL);
	struct tm tm1,tm2,tm3;



	//1.将time_t转换成char* 文本
	ctime_r(&t, (char*)&str_time);
	printf("time() = %d, ctime_r() = %s\n", t, str_time);



	//2.将time_t转换成struct tm

	//gmtime_r() 格林威治时间
	gmtime_r(&t,&tm1);
	//localtime_r() 本地时间
	localtime_r(&t,&tm2);

	//将struct tm对象转换成char* 文本(格林威治时间)
	memset(&str_time,'\0',64);
	asctime_r(&tm1,(char*)&str_time);
	printf("gmtime_r() to asctime_r() = %s\n", str_time);

	//将struct tm对象转换成char* 文本(本地时间)
	memset(&str_time,'\0',64);
	asctime_r(&tm2,(char*)&str_time);
	printf("localtime_r() to asctime_r() = %s\n", str_time);



	//3.将struct tm转换为time_t格式
	t = mktime(&tm1);
	printf("mktime(&tm1) = %d\n",t);
	t = mktime(&tm2);
	printf("mktime(&tm2) = %d\n",t);

	return;
}





//
//2.从struct tm时间结构体中,提取日期, 钟点时刻
//
void get_ymd_hms(void){
	time_t t = time(NULL);
	struct tm tm1,tm2,tm3;



	//将time_t转换成struct tm, 格林威治(GMT)
	gmtime_r(&t,&tm1);
	printf("year=%d mon=%d mday=%d \n",\
			tm1.tm_year+1900, tm1.tm_mon, tm1.tm_mday);

	printf("hour=%d min=%d sec=%d \n", \
			tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

	printf("wday=%d yday=%d isdst=%d\n", \
			tm1.tm_wday, tm1.tm_yday, tm1.tm_isdst);



	//将time_t转换成struct tm, 转换成本地时间
	localtime_r(&t,&tm2);
	printf("year=%d mon=%d mday=%d \n", \
			tm2.tm_year+1900, tm2.tm_mon, tm2.tm_mday);

	printf("hour=%d min=%d sec=%d \n", \
			tm2.tm_hour, tm2.tm_min, tm2.tm_sec);

	printf("tm_wday=%d tm_yday=%d tm_isdst=%d \n", \
			tm2.tm_wday, tm2.tm_yday, tm2.tm_isdst);

	return;
}





int main(void){
	//1.时间time_t, struct tm, 与string字符串的转换.
	time2string();

	//2.从struct tm时间结构体中,提取日期, 钟点时刻
	get_ymd_hms();

	printf("time test finish\n");
	return 0;
}


