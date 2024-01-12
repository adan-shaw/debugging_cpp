#include <stdio.h>
#include <zlib.h>

int main(void){
	const unsigned char strSrc[]="hello world!! aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd中文测试 中文测试";

	unsigned char buf[1024]={0},strDst[1024]={0};
	unsigned long srcLen=sizeof(strSrc),bufLen=sizeof(buf),dstLen=sizeof(strDst);

	printf("Src string:\n\t%s\nSrc Length:\n\t%d\n",strSrc,srcLen);	//显示原始数据

	compress(buf,&bufLen,strSrc,srcLen);														//压缩
	printf("Compressed String:\n\t%s\n",buf);
	printf("After Compressed Length:\n\t%d\n",bufLen);

	uncompress(strDst,&dstLen,buf,bufLen);													//解压缩
	printf("UnCompressed String:\n\t%s\n",strDst);
	printf("After UnCompressed Length:\n\t%d\n",dstLen);

	return 0;
}
