//编译:
//		gcc -g sha512.h sha512_test.c -o x 

//用法(usage):
//	# 两次计算结果一致, 即sha512正确
//	./x ./x
//	./x ./x



#include "sha512.h"



int main(int argc, char const *argv[]) {
  uint16_t blockcount;
  uint64_t* inputstring;
	FILE *input;

	//创建并初始化sha512 校验数存储空间(必须初始化为固定格式, 否则sha512 计算错误!!)
	//uint64_t buffers[8];//错误写法
  uint64_t buffers[] = {
    0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
    0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
  };
	

  if(argc != 2) {
    printf("usage: sha512 <filepath>\n");
    return -1;
  }

  input = fopen(argv[1], "rb");

  if (!input) {
    printf("Bad filepath\n");
    return -2;
  }

	inputstring = preprocess(input, &blockcount);

  calculateHash(blockcount, inputstring, (uint64_t*) &buffers);

	printf("buffers sizeof()=: %d\n", sizeof(buffers));
  printf("Final hash: ");
  for (int i = 0; i < 8; i++)
    printf("%016llx",(unsigned long long) buffers[i]);
  printf("\n");
  return 0;
}



