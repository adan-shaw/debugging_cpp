注意:
	xxx_s() 是微软的私货(微软c11 标准), 公版c11 不支持!!
	因此unix 系统编程中, 并没有这类函数, 丢弃幻想!!
	截断操作, 已经是最优!!



1>:字符分类<ctype.h>
isalnum()     检查一个字符是否是字母或数字
isalpha()     检查一个字符是否是英文字母
islower()     检查一个字符是否是小写字母
isupper()     检查一个字符是否是大写字母
isdigit()     检查一个字符是否为数字
isxdigit()    检查一个字符是否是十六进制的字符
iscntrl()     检查一个字符是否是控制字符
isgraph()     检查一个字符是否是可显示的字符
isspace()     检查一个字符是否是空白字符
isblank()     检查一个字符是否是空格字符
isprint()     检查一个字符是否是可打印字符
ispunct()     检查一个字符是否是一个标点符号



2>:字符操作<ctype.h>
tolower()     将单个字符转换成小写
toupper()     将单个字符转换成大写



3>:转换成数值格式<stdlib.h>
atof()        将'字节字符串'转换成浮点值-float
atoi()        将'字节字符串'转换成整数值-int
atol()        将'字节字符串'转换成整数值-long
atoll()       将'字节字符串'转换成整数值-long long
strtol()      将'字节字符串'转换成整数值-long
strtoll()     将'字节字符串'转换成整数值-long long
strtoul()     将'字节字符串'转换成无符号整数值-unsigned long
strtoull()    将'字节字符串'转换成无符号整数值-unsigned long long
strtod()      将'字节字符串'转换成浮点值-double
strtof()      将'字节字符串'转换成浮点值-float
strtold()     将'字节字符串'转换成浮点值-long double
strtoimax()   将'字节字符串'转换成 intmax_t, <inttypes.h>
strtoumax()   将'字节字符串'转换成 uintmax_t, <inttypes.h>



4>:字符串操作<string.h>
              (不带缓冲区截断, 弃用)
strcpy()      字符串拷贝,自动补'\0',返回des buf的首地址指针
stpcpy()      字符串拷贝2,自动补'\0',返回指向des结尾处字符(NULL)的指针
strcat()      连接两个字符串,返回des buf的首地址指针
strdup()      调用了malloc(),为变量字符串分配内存,创建副本(肯定返回副本指针)
strdupa()     调用了alloca(),为变量字符串分配内存,创建副本

              (截断版)
strncpy()     字符串拷贝,不补'\0',返回des buf的首地址指针
stpncpy()     字符串拷贝2,不补'\0',返回指向des结尾处字符(NULL)的指针
strncat()     连接两个字符串,返回des buf的首地址指针
strndup()     调用了malloc(),为变量字符串分配内存,创建副本
strndupa()    调用了alloca(),为变量字符串分配内存,创建副本(微软私货?)
strxfrm()     在"POSIX"或者"C"下,strxfrm()和strncpy()是等价的
strxfrm_l()   (locale 本地语言自适应版)



5>:字符串检验<string.h>
              (不带缓冲区截断, 弃用)
strlen()      返回字符串的长度
strcmp()      比较两个字符串;
              若str1=str2,则返回零;
              若str1<str2,则返回负数;
              若str1>str2,则返回正数;
strcasecmp()  比较两个字符串(忽略大小写差异的版本)
              若str1=str2,则返回零;
              若str1<str2,则返回负数;
              若str1>str2,则返回正数;

              (截断版)
strnlen()     返回字符串的长度
strncmp()     比较两个字符串
strncasecmp() 比较两个字符串(忽略大小写差异的版本)

              (子字符串查找API)
strchr()      扫描查找单个字符, 直到字符串结束, 或返回: 首个匹配字符的地址指针[void*](左边边开始扫描)
strrchr()     扫描查找单个字符, 直到字符串结束, 或返回: 最后一个匹配字符的地址指针[void*](右边开始扫描)
strspn()      扫描查找单个字符, 直到字符串结束, 或返回: 首个'不匹配的字符'的下标[int]
strcspn()     返回任何一个与'目标字符串中的"任意一个字符匹配"'的下标[int]
strpbrk()     在src字符串中,匹配出target子字符串的位置,
              并返回该位置后面的后续字符(不包括target子字符串),找不到返回空指针.
strstr()      在src字符串中,匹配出target子字符串的位置,
              并返回该位置后面的后续字符(包括target子字符串),找不到返回空指针.
strtok()      根据'子字符串',将src字符串切割成n份以'子字符串'间隔的孙字符串
              (但删除src中所有匹配到的'子字符串',src字符串会被破坏!!孙字符串中也不包含任何'子字符串'.
               而且返回的char*指针, 一般是char*[]指针数组,相当于char[][]二维字符串)
strtok_r()    切割字符串strtok()的(线程安全版)
strsep()      切割字符串,只要遇到'target匹配源中的任何一个字符'匹配,
              都将其进行切割,将切割好的字符串,用char*[]指针数组返回,类似于strtok()
strfry()      用rand()函数将字符串中的字符全部打乱,返回任意组合的新字符串.(gnu特有)

//strcoll()   在"POSIX"或者"C"下,与strcmp()是等价的.
//strcoll_l() 在"POSIX"或者"C"下,与strcmp()是等价的(locale本地语言自适应版)


6>:字符数组操作<string.h>(这类操作基本都带截断功能的)
memchr()      在buf中,匹配单个字符,返回匹配到的pos,的后续数据的void*指针
memcmp()      比较两块buf,只能比较是否相等,相等返回0[int]
memset()      以'某个字符'填充buf,返回被填充的buf指针
memcpy()      buf copy,返回目标buf的指针,返回des buf指针
memccpy()     buf copy,遇到'某个目标字符'则停止复制, 返回后续没有被复制的'src字符串指针'
mempcpy()     buf copy,返回des buf区中,剩余未被使用的buf指针首地址
memmove()     类似memset(),只是会将src buf清空.
memfrob()     用数字42对每个字符进行排他排序,从而对buf的前n个字节进行'加密'
              (可以防止内存数据被偷读,再次调用memfrob自身,可以反向解密)



7>:杂项<string.h>
//strerror()  返回指定的errno错误码的对应文本
strerror_r()  返回指定的errno错误码的对应文本(线程安全版)
strerror_l()  返回指定的errno错误码的对应文本(locale本地语言自适应版)
basename()    显示指定文件的文件名(不显示'路径'和'后缀')



8>:格式化字符串输入
//scanf()     从stdin流中读取数据, 回车分隔.
//sscanf()    从'格式化的字符串'中读取数据, 空格分隔. 例:
							sscanf("1 2 3","%d %d %d",buf1, buf2, buf3);//成功调用返回值为3,即buf1=1,buf2=2,buf3=3均成功转换.
							sscanf("1 2","%d %d %d",buf1, buf2, buf3);  //成功调用返回值为2,即只有buf1,buf2成功转换.
//printf()    格式化输出字符串到stdout
//sprintf()   格式化输出字符串到指定缓冲区
snprintf()    格式化输出字符串到指定缓冲区(截断版)








//
//xxx_s() 微软c11 字符串操作解析如下:
//
*************************************************************************
	c11 的_s() 版本, 都是绝对安全的, 因为它会做一下检查:
	例如:
		errno_t strncpy_s(char *restrict dest, rsize_t destsz,\\
				const char *restrict src, rsize_t count);
		1.src 或 dest 是空指针
		2.destsz 或count 为零或大于RSIZE_MAX
		3.count大于或等于destsz, 但destsz小于或等于strnlen_s(src, count),
			换言之, 会出现截断
		4.'源'和'目标字符串'间会出现重叠

	若dest所指的字符数组大小 < strnlen_s(src, destsz) <= destsz, 则行为未定义;
	换言之,错误的 destsz 值不暴露行将发生的缓冲区溢出.

	若src所指的字符数组大小 < strnlen_s(src, count) < destsz, 则行为未定义;
	换言之, 错误的count 值不暴露行将发生的缓冲区溢出.

	使用时要注意:
	同所有'边界检查函数', strncpy_s()仅若实现定义了 __STDC_LIB_EXT1__,
	且用户在包含 string.h 前定义 __STDC_WANT_LIB_EXT1__ 为整数常量1才保证可用.

*************************************************************************
