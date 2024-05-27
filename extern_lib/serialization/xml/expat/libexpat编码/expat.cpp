//编译:
//		g++ -g3 -lexpat ./expat.cpp -o x 



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>//for stat(), man 2 stat
#include <sys/stat.h>
#include <unistd.h>

//expat 相关的头文件(只有这两个了, 不多)
#include <expat.h>//expat api 声明
#include <expat_external.h>//跨平台描述



// expat简介:
/*
	expat是一个基于SAX模型的、非验证的轻量级XML解释器(默认, v1.2开始提供验证接口, 需要用户手动处理)
	目前XML的解析主要有两大模型:SAX和DOM.
		其中SAX(Simple API for XML)是基于事件的解析方法.
		基本工作原理是分析XML文档, 通过触发事件来通知用户解析的结果; 这种方式占用内存少, 速度快, 但用户程序相应得会比较复杂.

		而DOM(DocumentObject Model), 则是一次性将整个XML文档进行分析, 在内存中以树结构保存解析结果.
		同时, 向用户提供一系列的接口来访问和编辑该树结构.
		这种方式占用内存大, 速度往往慢于SAX, 但可以给用户提供一个面向对象的访问接口, 对用户更为友好.

		ps: expat 不需要装载完整的xml 文档, 可以边装载边解析;
				读到一个<A> </A>就提取一个元素, 但expat 不提供xml 文档生成;
				总的来说, expat 的解析速度是优秀的, 内存消耗也是优秀的;

	expat 支持分片读取, 它会自动分配缓冲区装载读取到的文件碎片.
	或者expat 的读取方式, 本来就不需要读取一个完整的xml 树.
	只有dom xml 才需要读取一个完整的xml 树去进行解析操作.
	如果是全部装载了整个xml文件, 那可以直接解析 + 生成, 双向操作了.


	Expat是一个C语言编写的XML解析库, 它是一个面向流的解析器, 应用程序可以对解析中发现的XML元素注册处理函数.

	XML_Parser 是expat 解析器结构体(主要载体)
	XML_Char = char 跨平台描述
	由定义可以判断, expat 并无封装功能.

	注意:
		Expat 默认并不支持中文, Expat不支持gb2312编码格式,
		主要支持UTF-8编码格式, 如果你要解析'中文xml', 需要改expat 源码.
*/



#define FILE_MAX_PARSER_SAX_XML (1*1024*1024*1024)//最大解析2GB 的xml 文件
//#define FILE_MAX_PARSER_SAX_XML 10



//用stat()检查一下路径是否被占用, 顺便读取文件大小.
bool stat_check(char* pfile_name, int* filesize);



//expat 每解析一个xml 元素, 就调用一次回调函数, start and end (every element Parse and work-start)
void startElement(void *userData, const char *name, const char **atts){
	int i;
	int *depthPtr =(int *)userData;



	//打印编号?? what what what the hell is that ?
	for(i = 0; i < *depthPtr; i++)
		printf("*%d*:\t",i);

	puts(name);//打印子元素的名(一个元素名下, 可能有多个子属性, 所以下面做遍历)

	//遍历子属性, 一对一对打印, "名=值"为一对! 子属性可能有很多对, 所以要遍历.
	for(i = 0; atts[i]; i += 2){
		printf("*attr_code=%d*,%s='%s';\t", i, atts[i], atts[i + 1]);
	}

	printf("\n");
	*depthPtr += 1;
}



//expat 每解析一个xml 元素, 就调用一次回调函数, start and end (every element Parse and work-end)
void endElement(void *userData, const char *name){
	int *depthPtr = (int*)userData;
	*depthPtr -= 1;
}



/*
	主函数功能说明:
		这里只做xml 解析操作, 充分发挥xml-SAX模型的expat 的读取性能, 快速且高效性.
		不做双向xml 解析和xml 生成, 这是xml-DOM模型 的工作, xml-SAX模型passed.

		输入一个xml 文件名, 按BUFSIZ 缓冲区一截截读取xml 文件.
		由于expat 的特性, 你不想要考虑xml 文件有多大, 一直读取解析即可.

		为了节省内存, 读取xml 文件的时候, 缓冲区是读定大小的,
		所以要一截截地读取xml 文件数据, 然后一段段地用expat 解析.

		那么, expat 会不会出现解析错误?
		例如, BUFSIZ = 8192, 刚好有一个xml 字节, 刚好横跨两个BUFSIZ = 8192,
		那么这个时候,expat 还能正确读取xml 数据吗?
*/
int main(int argc, char *argv[]){
	char buf[BUFSIZ];//BUFSIZ 为stdio.h 的Default buffer size.
	XML_Parser parser;
	int tmp, filesize, done;
	int depth;//xml root树深
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
	tmp = stat_check(argv[1], &filesize);
	if(tmp == false){
		perror("stat_check() failed");
		return -1;
	}

	//检查解析的xml 文件是否超出限制
	if(filesize > FILE_MAX_PARSER_SAX_XML){
		printf("file_max=%d > FILE_MAX_PARSER_SAX_XML=%d, EXCEEDS LIMIT!",\
				filesize, FILE_MAX_PARSER_SAX_XML);
		return -1;
	}



	//************
	//2.操作部分部分
	//************
	//2.1.创建一颗空树根root = null, root树深 = 0, 然后设置为expat 初始化数据树
	parser = XML_ParserCreate(NULL);
	depth = 0;
	XML_SetUserData(parser, &depth);

	//2.2.设置expat 常用的回调函数:
	//		遇到<> = startElement
	//		遇到</> = endElement
	//		(每解析出一个xml 数据, 都会本能地调用一次回调函数, 处理数据;
	//		你可以修改回调函数的内容,做其他操作,不限于打印数据内存,也可以是其他操作)
	XML_SetElementHandler(parser, startElement, endElement);

	//2.3.打开文件
	if((fp = fopen(argv[1], "r")) == NULL){
		printf("Can't open %s\n", argv[1]);
		return -1;
	}
	else
		clearerr(fp);//先清空流错误

	//2.4.分次读取, 一截截将xml 数据读取出来, 边读边解析
	do{
		tmp = fread(buf, 1, sizeof(buf), fp);//fread()函数会自动移动读游标,不用操心
		if(done = ferror(fp))
			printf("warning: ferror() = %d\n",done);

		done=feof(fp);//feof()函数, 当fp 文件流未结束end-of-file, 返回一个非0值
		if(!XML_Parse (parser, buf, tmp, done)){//执行解析
			fprintf(stderr, "%s at line %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
			return -1;
		}
	}while(!done);

	//2.5.释放expat 数据树, expat解析过程占用过的内存, 都会被彻底释放.
	XML_ParserFree(parser);

	return 0;
}



//用stat()检查一下路径是否被占用, 顺便读取文件大小.
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
