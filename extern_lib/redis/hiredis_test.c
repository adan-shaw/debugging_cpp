//编译:
//	gcc hiredis_test.c -g3 -o x -lhiredis


#include "hiredis-master/hiredis.h"
#include "hiredis-master/net.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>



//redisReply{} 结构体的定义详情(仅存储value, 不存储key; key 是索引值, 需要你自己输入)
/*
	typedef struct redisReply {
			int type;											// REDIS_REPLY_*
																			 返回值类型

			long long integer;						// The integer when type is REDIS_REPLY_INTEGER
																			 数值(仅'type=REDIS_REPLY_INTEGER'有效)

			size_t len;										// Length of string
																			 字符串len长度(仅'type=REDIS_REPLY_ERROR/REDIS_REPLY_STRING'有效)

			char *str;										// Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING
																			 字符串指针(仅'type=REDIS_REPLY_ERROR/REDIS_REPLY_STRING'有效)

			size_t elements;							// number of elements, for REDIS_REPLY_ARRAY
																			 数组向量len长度(仅'type=REDIS_REPLY_ARRAY'有效)

			struct redisReply **element;	// elements vector for REDIS_REPLY_ARRAY
																			 数组向量指针(仅'type=REDIS_REPLY_ARRAY'有效)
	}redisReply;

	其中, type类型有:
		REDIS_REPLY_STRING:  1
		REDIS_REPLY_ARRAY:   2
		REDIS_REPLY_INTEGER: 3
		REDIS_REPLY_NIL:     4
		REDIS_REPLY_STATUS:  5
		REDIS_REPLY_ERROR:   6
*/





//测试string 字符串操作
bool redis_test_str(redisContext *pCon);

//测试list操作
bool redis_test_list(redisContext *pCon);

//测试set操作
bool redis_test_set(redisContext *pCon);

//测试hash操作
bool redis_test_hash(redisContext *pCon);



int main(void){
	redisContext *pCon = NULL;
	redisReply *pReply = NULL;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds 设置连接等待时间
	const char ip[32]="127.0.0.1";
	const unsigned short port=6379;



	//建立redis 链接
	pCon = redisConnectWithTimeout(ip, port, timeout);
	//assert(pCon != NULL);//无需断言, 断言也于事无补; 如果出错了, 只能自己差错, 不能顾及那么多杂事;
	//											因此, 假设redis API 一定返回非空指针!! (如果返回一条NULL指针, 你做什么, 程序都会崩溃的)
	if(pCon->err){
		printf("redisConnectWithTimeout() Error: %s\n", pCon->errstr);
		return -1;
	}

	//发送redis 命令到server, 选择编号=15 的数据库
	pReply = redisCommand(pCon, "SELECT 15");
	if(pReply->type == REDIS_REPLY_ERROR){
		printf("redisCommand() Error: %s\n", pReply->str);
		redisFree(pCon);
		return -1;
	}

	//测试redis 的各种操作
	if(redis_test_str(pCon))
		printf("redis_test_str() test ok!!\n");
	if(redis_test_list(pCon))
		printf("redis_test_list() test ok!!\n");
	if(redis_test_set(pCon))
		printf("redis_test_set() test ok!!\n");
	if(redis_test_hash(pCon))
		printf("redis_test_hash() test ok!!\n");

	//释放redis 链接
	redisFree(pCon);

	return 0;
}





//测试string 字符串操作
bool redis_test_str(redisContext *pCon){
	redisReply *pReply;
	size_t len;
	char *reply_str = NULL;
	long long reply_num = 0;



	//纯字符串测试
	pReply = redisCommand(pCon, "set test_str fuckyou");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_str_failed;

	pReply = redisCommand(pCon, "get test_str");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_str_failed;

	assert(pReply->type == REDIS_REPLY_STRING);
	reply_str = pReply->str;
	len = pReply->len;
	printf("test_str = %s, len=%d\n", reply_str, len);
	freeReplyObject(pReply);



	//数字字符串测试(需要使用atoi()/atol()/atoll() 将字符串转换为int/long/long long 整形)
	pReply = redisCommand(pCon, "set test_str_num 999");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_str_failed;

	pReply = redisCommand(pCon, "incr test_str_num");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_str_failed;

	pReply = redisCommand(pCon, "get test_str_num");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_str_failed;

	assert(pReply->type == REDIS_REPLY_STRING);
	reply_num = atoll(pReply->str);
	printf("test_str_num = %d\n", reply_num);
	freeReplyObject(pReply);
	return true;


redis_test_str_failed:
	printf("redisCommand() Error: %s\n", pReply->str);
	freeReplyObject(pReply);//如果两次释放pReply, 会报错: free(): double free detected in tcache 2, Aborted
	return false;
}



//测试list操作(一次只能pop 出一个元素; 可以删除多个元素, 但不能pop出多个元素; 
//						每个元素都是字符串; 允许出现相同值的元素[值相同但存储位置和下标不同])
bool redis_test_list(redisContext *pCon){
	redisReply *pReply;
	size_t len;
	char *reply_str = NULL;



	pReply = redisCommand(pCon, "lpush test_list fuckyou 999 333 dead");
	//(元素统一为字符串数据, 就算全为number 也是字符串)
	//pReply = redisCommand(pCon, "lpush test_list 111 999 333 22 22");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_list_failed;

	pReply = redisCommand(pCon, "lpop test_list");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_list_failed;

	assert(pReply->type == REDIS_REPLY_STRING);
	reply_str = pReply->str;
	len = pReply->len;
	printf("test_str = %s, len=%d\n", reply_str, len);
	freeReplyObject(pReply);
	return true;


redis_test_list_failed:
	printf("redisCommand() Error: %s\n", pReply->str);
	freeReplyObject(pReply);
	return false;
}



//测试set操作
bool redis_test_set(redisContext *pCon){
	redisReply *pReply;
	unsigned int tmp = 0;



	//sadd 成功后, 返回0; 如果出现重复元素, 则覆盖(无原子操作)
	pReply = redisCommand(pCon, "sadd test_set 999 888 1");
	pReply = redisCommand(pCon, "sadd test_set 999 fuck 888 you aaa bbb 1");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_set_failed;
	else
		printf("sadd() return-value: %d[type=REDIS_REPLY_INTEGER]\n",pReply->integer);

	//返回set 中的所有元素
	pReply = redisCommand(pCon, "smembers test_set");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_set_failed;

	//打印smembers 返回的所有set 元素, 元素类型为:REDIS_REPLY_STRING
	assert(pReply->type == REDIS_REPLY_ARRAY);
	printf("print all set smembers:\n");
	for(tmp=0; tmp<pReply->elements; tmp++)
		printf("%s\n", pReply->element[tmp]->str);
	return true;

redis_test_set_failed:
	printf("redisCommand() Error: %s\n", pReply->str);
	freeReplyObject(pReply);
	return false;
}



//测试hash操作(field and value 要分开获取, 不能同时获取field and value)
bool redis_test_hash(redisContext *pCon){
	redisReply *pReply;
	unsigned int tmp = 0;



	pReply = redisCommand(pCon, "hmset test_hash a 1 q 2 a 1 a 2 s 2 fd 4");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_hash_failed;

	//仅获取hash 中所有的fields(没有value, hmget 才能获取键值对)
	pReply = redisCommand(pCon, "hkeys test_hash");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_hash_failed;

	assert(pReply->type == REDIS_REPLY_ARRAY);
	printf("print all hash keys:\n");
	for(tmp=0; tmp<pReply->elements; tmp++)
		printf("%s\n", pReply->element[tmp]->str);

	//根据field, 获取对应的value
	pReply = redisCommand(pCon, "hmget test_hash a q s fd");
	if(pReply->type == REDIS_REPLY_ERROR)
		goto redis_test_hash_failed;

	assert(pReply->type == REDIS_REPLY_ARRAY);
	printf("print all hash values:\n");
	for(tmp=0; tmp<pReply->elements; tmp++)
		printf("%s\n", pReply->element[tmp]->str);
	return true;


redis_test_hash_failed:
	printf("redisCommand() Error: %s\n", pReply->str);
	freeReplyObject(pReply);
	return false;
}
