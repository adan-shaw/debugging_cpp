//编译:
//		g++ -std=c++11 -g3 ./string.cpp -o x



// c++string方法list:
/*
	成员函数
		(构造函数)					构造basic_string
		(析构函数)					销毁字符串,若使用内部存储则解分配它
		operator=					为字符串赋值
		assign						赋值字符给字符串
		get_allocator			返回关联的分配器

	元素访问
		at								访问指定字符,有边界检查
		operator[]				访问指定字符
		front							访问首字符(C++11)
		back							访问最后的字符(C++11)
		data							返回指向字符串首字符的指针
		c_str							返回字符串的不可修改的C字符数组版本
		operatorbasic_string_view			返回到整个字符串的不可修改的basic_string_view(C++17)

	迭代器
		begin							返回指向起始的迭代器
		cbegin						(C++11)
		end								返回指向末尾的迭代器
		cend							(C++11)
		rbegin						返回指向起始的逆向迭代器
		crbegin						(C++11)
		rend							返回指向末尾的逆向迭代器
		crend							(C++11)

	容量
		empty							检查字符串是否为空
		size							返回字符数
		length
		max_size					返回字符数的最大值
		reserve						保留存储
		capacity					返回当前对象分配的存储空间能保存的字符数量
		shrink_to_fit			通过释放不使用内存减少内存使用(C++11)

	操作
		clear							清除内容
		insert						插入字符
		erase							移除字符
		push_back					后附字符到结尾
		pop_back					移除末尾字符(C++11)
		append						后附字符到结尾
		operator+=				后附字符到结尾
		compare						比较二个字符串
		starts_with				检查string是否始于给定前缀(C++20)
		ends_with					检查string是否终于给定后缀(C++20)
		replace						替换字符串的指定部分
		substr						返回子串
		copy							复制字符
		resize						更改存储的字符数
		swap							交换内容

	查找
		find							于字符串中寻找字符
		rfind							寻找子串的最后一次出现
		find_first_of			寻找字符的首次出现
		find_first_not_of	寻找字符的首次缺失
		find_last_of			寻找字符的最后一次出现
		find_last_not_of	寻找字符的最后一次缺失

	常量
		npos							特殊值,准确含义依赖语境(公开静态成员常量), 总是等于-1

	非成员函数
		operator+					连接两个字符串或者一个字符串和一个字符
		operator==
		operator!=
		operator<
		operator>
		operator<=
		operator>=				以字典序比较两个字符串
		std::swap(std::basic_string)		特化 std::swap 算法
		erase(std::basic_string)
		erase_if(std::basic_string)			擦除所有满足特定判别标准的元素(C++20)

	输入/输出
		operator<<
		operator>>				执行字符串的流输入与输出
		getline						从I/O流读取数据到字符串

	数值转换(C++11)
		stoi							转换字符串为有符号整数(C++11)
		stol							(C++11)
		stoll							(C++11)
		stoul							转换字符串为无符号整数(C++11)
		stoull						(C++11)
		stof							转换字符串为浮点值(C++11)
		stod							(C++11)
		stold							(C++11)
		to_string					转换整数或浮点值为string(C++11)
		to_wstring				转换整数或浮点值为wstring(C++11)

	字面量
		operator""s				转换字符数组字面量为basic_string(C++14)
											定义于内联命名空间 std::literals::string_literals
*/



#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>



using namespace std;



//1.字符串<->数字 (非格式化打印的方式) [c++ 风格]
void string2number(void);

//2.snprintf() 按格式打印字符串(c 语言风格)
void snprintf_test(void);

//3.正则表达式(c 语言风格)
void sscanf_test(void);



//0.c++ 字符串操作demo
int main(void){
	int tmp, count;
	// 定义string 容器
	string s = "hello world";
	string paper = "很大很长的一篇文章, 你读完就想释放这片内存, 免得占用内存空间";
	string *paper2 = new string("很大很长的一篇文章, 你读完就想释放这片内存, 免得占用内存空间");
	delete paper2;

	//string 缩容
	paper = "";
	cout << paper.capacity() << endl;
	paper.shrink_to_fit();
	cout << paper.capacity() << endl;

	//string 手动扩容
	paper.resize(512);
	cout << paper.capacity() << endl;

	//提取string 的buf 缓冲区
	const char* pbuf = s.c_str();
	cout << "c_str(): " << pbuf << endl;



	//运算符重载
	s = s + "  " + s + "  idiot, lol";			//'+'加号叠加
	cout << s << endl;

	paper = s;															//'='等号拷贝
	if(paper == s)													//'=='双等号判断
		cout << "(paper == s)" << endl;



	//测试std::string::npos 特殊值, 是否等于-1 (unsigned int)
	printf("std::string::npos=%d, %d\n", std::string::npos, s.npos);



	//查找匹配操作
	tmp = s.find("hello");									//返回第一个匹配的子字符串的起始下标
	if(tmp != s.npos)
		cout << "tmp = s.find(\"hello\"); tmp != s.npos;" << "\ntmp=pos=" << tmp << endl;

	tmp = s.rfind("hello");									//返回最后一个匹配的子字符串的起始下标
	if(tmp != s.npos)
		cout << "tmp = s.rfind(\"hello\"); tmp != s.npos;" << "\ntmp=pos=" << tmp << endl;



	//拷贝出子字符串
	paper = s.substr(0,12);									//拷贝子字符串, 返回string; 函数原型: string substr(起始pos, 拷贝字节数)
	cout << paper << endl;									//注意: 如果你选择手动输入(起始pos, 拷贝字节数), 起始pos不能超越s.capacity(), 否则报错;

	paper = s.substr(0,s.capacity()+12);		//拷贝子字符串越界测试: 结果, 自动扩容
	//paper = s.substr(s.capacity()+1,s.capacity()+12);//报错, 起始pos不能超越s.capacity()!!
	cout << paper << "  paper.capacity()=" << paper.capacity() << "  s.capacity()="<< s.capacity() << endl;



	//替换子字符串
	paper = s.replace(0,12, "hello");											//pos[0,12], 替换为子字符串"hello", 长度不匹配, 会触发字符串整体向前缩进
	cout << paper << endl;

	paper = s.replace(s.begin(), s.begin() + 12, "hello");//pos[0,12], 替换为子字符串"hello"(迭代器版本)
	cout << paper << endl;

	s = s + "  " + s + s + "  " + s + "  idiot, lol";
	paper = "hello";
	tmp = 0;
	while((tmp=s.find(paper,tmp))!=string::npos)					//整个字符串, 遇到"hello", 就替换为"unix"
		s.replace(tmp,paper.length(),"unix");
	cout << s << endl;

	count = 0;
	paper = "unix";
	tmp = 0;
	while((tmp=s.find(paper,tmp))!=string::npos){					//整个字符串, 统计"unix"出现的次数
		tmp+=paper.length();																//命中之后, tmp 必须移动paper.length() 位, 才正确;
		count++;
	}
	cout << count << endl;


	//其他测试函数
	string2number();
	snprintf_test();
	sscanf_test();
	return 0;
}



//1.字符串<->数字 (非格式化打印的方式) [c++ 风格]
void string2number(void){
	string str;

	str = str + to_string(0.001) + "\n";	//浮点常量
	str = str + to_string(100) + "\n";		//整形常量
	printf("number to string: %s",str.c_str());
	str.clear();

	//string to 整形
	str = "100";
	printf("string to int-number\n");
	printf("string to int: %d\n",stoi(str));
	printf("string to long: %d\n",stol(str));
	printf("string to long long: %d\n",stoll(str));
	printf("string to unsigned long: %d\n",stol(str));
	printf("string to unsigned long long: %d\n\n",stoll(str));
	str.clear();


	//string to 浮点
	str = "0.3101";
	printf("string to float-number\n");
	printf("string to float: %f\n",stof(str));
	printf("string to double: %lf\n",stod(str));
	printf("string to long double: %lf\n",stold(str));

	return ;
}



//2.snprintf() 按格式打印字符串(c 语言风格)
/*
	其中, 还用到的相关函数:
		strncpy() 字符串截断拷贝
		strncat() 字符串截断拼接
		strncmp() 字符串截断比较
	注意1:
		截断长度, 都是以第一个参数的buf sizeof() 为准!!
		( 铁律, 不会变的, 第三参数一般为: sizeof(第一参数的字符串) )
	注意2:
		结束标志2, 可以是第二参数的'\0' 字符串结束符号!!
		( 也可以是第三参数的截断长度, 触发函数结束 )
*/
void snprintf_test(void){
	char buf1[256] = "hello angel~~i want to love you...";
	char buf2[256],buf3[512];
	int tmp;
	char *p;



	printf("snprintf_test()\n\n");
	buf2[200] = 'a';
	printf("buf2[200] = %c\n",buf2[200]);


	//1.拷贝字符串, 带截断, 遇到第二参数的'\0'结束
	strncpy(buf2,buf1,sizeof(buf2));
	printf("buf1 len = %d, buf1: %s\n",strlen(buf1),buf1);
	printf("buf2 len = %d, buf2: %s\n\n\n",strlen(buf2),buf2);


	//2.字符串拼接, 带截断, 遇到第二参数的'\0'结束
	memset(buf3,'\0',sizeof(buf3));	//使用前, 必须先格式化, 防止乱值出现
	strncat(buf3,buf1,sizeof(buf3));//将buf1 并接到buf3
	strncat(buf3,buf2,sizeof(buf3));//将buf2 并接到buf3
	printf("buf3 len = %d, strncat() buf3: %s\n\n\n",strlen(buf3),buf3);


	//3.比较两个字符串, 遇到第一个不同的字符, 结束返回该字符的pos
	tmp = strncmp(buf1,buf2,sizeof(buf1));
	if(tmp == 0)
		printf("buf1 == buf2\n\n\n");	//两串字符串相同
	else
		printf("buf1 != buf2, buf1[%d]=%c, buf2[%d]=%c\n\n\n",tmp,buf1[tmp],tmp,buf2[tmp]);


	//4.1 strcspn() 字符串逐字匹配, 发现第一个'相同字符', 返回该字符pos
	//失败:
	tmp = strcspn(buf1,buf2);
	printf("strcspn(): %d\n", tmp);

	//4.2 strspn() 字符串逐字匹配, 发现第一个'不相同字符', 返回该字符pos
	//失败:
	tmp = strspn(buf1,buf2);
	printf("strspn(): %d\n", tmp);


	//5.strstr()'子字符串'匹配,发现第一个'相同字符串',返回该字符串的起始位置
	//失败: NULL
	p = strstr(buf2,"angel");
	printf("strstr(): %s, 'angel'\n",p);

	//实际这种替换毫无意义!! 因为被替换的区域, 长度不能修改!!
	//如果要修改长度, 还是需要重新改写字符串缓冲区.
	//C 语言根本不擅长做复杂字符串操作!!
	//********************************************
	//根据strstr() 匹配到的子字符串, 进行替换'字符串'
	//tmp = strlen("angel");//求替换内容的长度
	//strncpy(buf2,"ANGEL",tmp);//替换大写
	//printf("%s\n",buf2);//打印替换结果
	//********************************************


	//6.格化打印字符串
	memset(buf2,'\0',sizeof(buf2));
	snprintf(buf2,sizeof(buf2),"%s_%d_%s_%lf","hello",123456,"love",0.999);
	printf("snprintf(): %s\n",buf2);

	return ;
}



//3.正则表达式(c 语言风格)
//(只能用在sscanf()中,sscanf()无缓冲区截断,需要自己保证安全.)
/*
	(1) ^ 表示匹配--必须放进[] 里面工作, []前面必须要有%

	(2)当要匹配右方括号"]"时, 应把其放在set中第一个位置,
		 如"%[]abc]", 表示匹配的字符集为{], a, b, c},
		 又如"%[^]abc]", 表示匹配所有非"], a, b, c"中的字符

	(3) * 表示跳过--不能放进[],* 前面必须要有%
*/
void sscanf_test(void){
	char buf[256] = "";



	printf("sscanf_test()\n\n");

	sscanf("123456 ", "%4s", buf);					//最大长度限制--取最大长度为4字节的字符串
	printf("%s\n",buf);


	//正则--匹配a到z中任意字符(只要是a到z的字符都不拒绝, 直到遇到第一个不是的, 马上终止匹配)
	memset(buf,'\0',256);
	//sscanf("01239abcdmq","%[0-9a-z]",&buf);
	sscanf("01239abcdmq","%[a-z]",&buf);		//只要字母
	//sscanf("01239abcdmq","%[0-9]",&buf);	//只要数字
	//sscanf("abcdmq","%[9-0z-a]",&buf);		//不能翻转来写
	printf("%s\n",buf);


	//正则--直到遇到第一个不匹配的字符, 马上终止匹配(^9 除了9 之外所有字符)
	memset(buf,'\0',256);
	//sscanf("aa122229 ","%[^9]",&buf);			//结果: aa12222
	//sscanf("aa122229 ","%[^2]",&buf);			//结果: aa1
	sscanf("aa122229 ","%[^0-9]",&buf);			//结果: aa
	printf("%s\n",buf);


	//匹配多次
	memset(buf,'\0',256);
	sscanf("01239abcdmq","%3[0-9a-z]",&buf);//只想要前3 个匹配字符:012
	//sscanf("aa122229 ","%2[^2]",&buf);		//结果: aa . 支取前两个字符, 在%[^2] 的基础上支取前两个字符
	printf("%s\n",buf);

	return;
}
