//编译:
//		g++ -g3 -lexpat ./expat.cpp -o x 

//test:
//		./x test_data1.xml
//		./x test_data2.xml
//		./x test_data.xml # big data test



// expat简介:
/*
	expat是一个基于SAX模型的、非验证(默认, v1.2开始提供验证接口, 需要用户手动处理)的轻量级XML解释器. 
	目前XML的解析主要有两大模型:
	* SAX:
		SAX(Simple API for XML)是基于事件的解析方法. 
		基本工作原理是分析XML文档, 通过触发事件来通知用户解析的结果. 
		这种方式占用内存少, 速度快, 但用户程序相应得会比较复杂. 
	* DOM:
		DOM(DocumentObject Model), 则是一次性将整个XML文档进行分析, 在内存中以树结构保存解析结果. 
		同时, 向用户提供一系列的接口来访问和编辑该树结构. 
		这种方式占用内存大, 速度往往慢于SAX, 但可以给用户提供一个面向对象的访问接口, 对用户更为友好. 

	ps:
		expat 不需要装载完整的xml 文档, 可以边装载边解析, 读到一个<A> </A>就提取一个元素, 
		但expat 不提供xml 文档生成;
		总的来说, expat 的解析速度是优秀的, 内存消耗的控制方面也是做得非常好的, 可靠的高性能xml 解析库;

		expat 支持分片读取, 它会自动分配缓冲区装载读取到的文件碎片.
		或者expat 的读取方式, 本来就不需要读取一个完整的xml 树.
		只有dom xml 才需要读取一个完整的xml 树去进行解析操作.
		如果是全部装载了整个xml文件, 那可以直接解析 + 生成, 双向操作了.


	Expat 是一个C语言编写的XML解析库, 它是一个面向流的解析器, 应用程序可以对解析中发现的XML元素注册处理函数;
	XML_Parser 是expat 解析器结构体(主要载体)
	XML_Char = char 跨平台描述(屏蔽多平台的char 差异)
	Expat 并无封装功能, 不能封装xml 树, 然后反向生成xml 文件;

	注意:
		Expat 默认并不支持中文, Expat不支持gb2312编码格式, 主要支持UTF-8编码格式, 
		如果你要解析'中文xml', 需要改expat 源码.
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//expat 相关的头文件
#include <expat.h>					//expat api 声明
#include <expat_external.h>	//跨平台描述



#define FILE_MAX_PARSER_SAX_XML (1*1024*1024*1024) //最大解析2GB 的xml 文件
//#define FILE_MAX_PARSER_SAX_XML 10



//用stat()检查一下路径是否被占用, 顺便读取文件大小
bool stat_check(char* pfile_name, int* filesize);



//expat 每解析一个xml 元素的头, 就触发一次的回调函数(every element Parse and work-start)
void XMLCALL startElement(void *userData, const char *name, const char **atts){
	int i, *depthPtr = (int *)userData;

	//计算当前name 的depth 树深
	for(i = 0; i < *depthPtr; i++)
		;

	printf("<%s", name);//打印name 头

	//遍历name 下的子属性, 一对一对打印
	for(i = 0; atts[i]; i+=2)
		printf(" \"%s\"=\"%s\"", atts[i], atts[i+1]);
	printf(">");

	//打印name 中的数据 / 子属性中的数据(如何打印出来?)
	//printf("%s", (char*)userData);

	*depthPtr += 1;
}

//expat 每解析一个xml 元素的尾, 就触发一次的回调函数(every element Parse and work-end)
void XMLCALL endElement(void *userData, const char *name){
	int *depthPtr = (int*)userData;
	printf("</%s>", name);//打印name  尾
	*depthPtr -= 1;
}

//expat 每解析一个xml 元素的内容, 就触发一次的回调函数
void XMLCALL dataHandler(void *userData, const XML_Char *s, int len){
	char buf[128];
	memset(buf, '\0', sizeof(buf));//xml 经常性会重复打印数据, 这里必须先做buf 清空
	strncpy(buf, s, len);					 //(内存操作消耗较大, 不先清空, copy 时会有很多乱码, 主要来自上一次的读取buf, 这是不允许的, 会脏读)
	printf("%s", buf);

	//printf("%s", s);						 //如果只是为了打印数据, 直接打印最好, 节省内存操作; 如果需要拷贝出内容, 进行其他判别, 则比较麻烦(需要考虑到效率的问题)
	//														 //ps: 直接打印有乱码, shit !!
}




//main() 主函数功能说明:
/*
	这里只做xml 解析操作, 充分发挥xml-SAX模型的expat 的读取性能, 快速且高效性.
	(不做双向xml 解析和xml 生成, 这是xml-DOM模型 的工作, xml-SAX模型passed)

	输入一个xml 文件名, 按BUFSIZ 缓冲区一截截读取xml 文件.
	由于expat 的特性(解析能力超强), 你不想要考虑xml 文件有多大, 一直读取解析即可.

	为了节省内存, 读取xml 文件的时候, 缓冲区是读定大小的,
	所以要一截截地读取xml 文件数据, 然后一段段地用expat 解析.

	那么, expat 会不会出现解析错误?
	例如, BUFSIZ = 8192, 刚好有一个xml 字节, 刚好横跨两个BUFSIZ = 8192, 这个时候, expat 还能正确读取xml 数据吗? 
	(出现这种问题, 纯粹是程序员编码能力的问题, 本demo 未实测验证)
*/
int main(int argc, char *argv[]){
	char buf[BUFSIZ];					//BUFSIZ 为stdio.h 的Default buffer size.
	XML_Parser parser;
	int tmp,filesize,done;
	int depth;								//xml root树深
	FILE *fp;



	//************
	//1.检查部分
	//************
	//检查参数数量
	if(argc != 2){
		printf("Usage: %s filename\n", argv[0]);
		return 0;
	}

	//打印文件信息, 检查文件大小
	if(stat_check(argv[1], &filesize) == false){
		printf("stat_check() failed");
		return -1;
	}

	//检查解析的xml 文件是否超出限制
	if(filesize > FILE_MAX_PARSER_SAX_XML){
		printf("file_max=%d > FILE_MAX_PARSER_SAX_XML=%d, EXCEEDS LIMIT!", filesize, FILE_MAX_PARSER_SAX_XML);
		return -1;
	}



	//************
	//2.操作部分部分
	//************
	//创建一颗空树根root = NULL
	parser = XML_ParserCreate(NULL);

	//设置expat 树的树深=0, 以此来初始化expat 树
	depth = 0;
	XML_SetUserData(parser, &depth);

	//设置expat 关键的回调函数:
	/*
		遇到<>  = startElement() 回调函数
		遇到</> = endElement() 回调函数
		(每解析出一个xml 数据, 都会本能地触发一次回调函数startElement() + endElement(), 处理数据;
		 你可以修改startElement() + endElement()回调函数的内容, 做一些其他操作)
	*/
	XML_SetElementHandler(parser, startElement, endElement);

	//每次读取<*> </*> 中的数据时, 都会触发的回调函数
	XML_SetCharacterDataHandler(parser, &dataHandler);

	//打开xml 文件
	if((fp = fopen(argv[1], "r")) == NULL){
		perror("fopen()");
		return -1;
	}
	else
		clearerr(fp);//先清空流错误

	//分次读取, 一截截将xml 数据读取出来, 边读边解析
	do{
		tmp = fread(buf, 1, sizeof(buf), fp);//fread()函数会自动移动读游标,不用操心
		if(done = ferror(fp))
			printf("warning: ferror() = %d\n",done);

		done = feof(fp);//feof()函数, 当fp 文件流未结束end-of-file, 返回一个非0值
		if(!XML_Parse (parser, buf, tmp, done)){//执行解析
			fprintf(stderr, "%s at line %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
			return -1;
		}
	}while(!done);

	//释放expat 数据树, expat解析过程占用过的内存, 都会被彻底释放.
	XML_ParserFree(parser);
	return 0;
}



//用stat()检查一下路径是否被占用, 顺便读取文件大小
bool stat_check(char* pfile_name, int* filesize){
	struct stat stat_buf;
	int ret;
	char ptimebuf[64];//打印修改时间的buf

	ret = stat(pfile_name, &stat_buf);
	if(ret == -1){
		//如果stat() 出错, 分析出错原因
		perror("stat()");
		switch(errno){
			case ENOENT:
				printf("File %s not found.\n", pfile_name);
				break;
			case EINVAL:
				printf("Invalid parameter to stat().\n");
				break;
		 default:
			 printf("Unexpected error in stat().\n");
		}
		return false;
	}
	else{
		//如果stat() 没有出错, 打印stat() 结果; Output some of the statistics(输出一些统计数据):
		printf("File size 文件大小: %ld\n", stat_buf.st_size);
		printf("Drive 驱动标记: %c:\n", stat_buf.st_dev + 'A');
		printf("st_dev: %lx, st_ino: %lx\n", (u_long) stat_buf.st_dev, (u_long) stat_buf.st_ino);

		//打印-最近修改时间
		memset(ptimebuf,'\0',sizeof(ptimebuf));
		//ctime_s() 是c11 特有, 线程安全 + 缓冲区安全
		//ctime_s(timebuf, 26, &stat_buf.st_mtime);
		ctime_r(&stat_buf.st_mtime, ptimebuf);//自己保障缓冲区不会溢出
		printf("Time modified 修改时间: %s\n", ptimebuf);

		*filesize = stat_buf.st_size;//取值文件大小
		return true;
	}
}
