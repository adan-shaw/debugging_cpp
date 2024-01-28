//编译:
//		g++ -g3 ./libcurl_demo1.cpp -o x -lcurl



//libcurl进行HTTP GET获取JSON数据(转载)

#include <cstdio>
#include <iostream>
#include <sstream>
#include "curl/curl.h"

using namespace std;

// curl http GET/PORT 等method 的响应函数(收到http GET/PORT 操作请求时, 本程序作出的相应操作函数)
size_t write_data (void *ptr, size_t size, size_t nmemb, void *stream)
{
	string data ((const char *) ptr, (size_t) size * nmemb);
	*((stringstream *) stream) << data << endl;
	return size * nmemb;
}

int main (void)
{
	std::stringstream out;
	string str_json;
	void *curl;
	CURLcode res;
	time_t now = time (0);	//当前系统的当前时间
	char *dt = ctime (&now);//当前系统的当前日期
	cout << dt << "-------------------------------------" << endl;

	// 初始化curl 库SDK实体
	curl = curl_easy_init ();

	// 设置URL
	curl_easy_setopt (curl, CURLOPT_URL, "http://if.qdocument.net:705/bic/download/initFiles?uid=weidong0925@126.com&pageNum=1");

	// 设置接收数据的处理函数
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_data);

	// 设置接收数据的存放变量
	curl_easy_setopt (curl, CURLOPT_WRITEDATA, &out);

	// 执行HTTP GET操作
	res = curl_easy_perform (curl);
	if (res != CURLE_OK)
		fprintf (stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror (res));

	// 打印stdout 到命令行(根据CGI 原则, 这个stdout 就是http 协议返回的应用数据)
	cout << out.str() << endl;

	// 保存http 协议返回的应用数据到string 变量中
	str_json = out.str ();
	// 打印again
	printf ("%s", str_json.c_str ());

	// 释放curl 库SDK实体
	curl_easy_cleanup (curl);

	return 0;
}
