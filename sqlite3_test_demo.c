//编译:
//	gcc ./sqlite3_test_demo.c -g3 -o x -lsqlite3



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sqlite3.h>



// sqlite3 db存储路径
const char sqlite3_db_filepath[]="./sqlite3_test.db";

// Student 结构体声明(要存放的数据格式)
struct Student{
	unsigned int num;
	unsigned char name[16];
	unsigned char sex[4];
	unsigned int age;
	unsigned int score;
};





//sqlite3 创建数据库 and 插入数据测试
bool create_insert_test(void){
	// 命令: 创建sqlite3 db
	const char comm_create[]="create table student(num integer PRIMARY KEY, name text, sex text, age integer, score integer);";
	// 命令: 向sqlite3 插入数据
	const char *comm_insert[]={
		"insert into student values(10101,'liyuge','nan',18,99);",
		"insert into student values(10102,'tiancai','nan',18,98);",
		"insert into student values(10103,'hehe','nv',19,96);",
		"insert into student values(10104,'xixi','nv',19,94);",
		"insert into student values(10105,'haha','nan',18,90);"
	};
	int i, comm_insert_len = sizeof(comm_insert)/sizeof(comm_insert[0]);
	char *errmsg;
	sqlite3 *db3;



	//SQLITE_OK = 0 = sqlite3 命令操作成功的标志
	if(sqlite3_open(sqlite3_db_filepath, &db3) != SQLITE_OK){
		perror("cannot open sqlite3 database file");
		return false;
	}


	//执行sqlite3 数据库命令: 创建表student
	if(sqlite3_exec(db3,comm_create,NULL,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}


	//执行sqlite3 数据库命令: 向表内插入学生信息
	for(i=0;i<comm_insert_len;i++)
		if(sqlite3_exec(db3,comm_insert[i],NULL,NULL,&errmsg) != SQLITE_OK){
			printf("%s\n",errmsg);
			sqlite3_close(db3);
			return false;
		}


	//关闭sqlite3 数据库
	sqlite3_close(db3);
	//assert(sqlite3_close(db) == SQLITE_OK);//关闭sqlite3 db, 类似于关闭文件一样, 不会失败的
	return true;
}





//回调函数: 还原读取到的sqlite3 数据到struct 结构提中(必须是int 返回类型!! 否则报错)
/*
	每读取一个条数据, 都会触发一次callback 回调函数;
	每条数据中, 每一个数据都需要分开读取;
	每个数据都以字符串的形式存放, 数字需要做转换才行;
	读取时, 必须先创建缓冲区!! 不能传入未实体化的指针, sqlite3 不会帮你创建内存实体;
	因此在读取sqlite3 数据, 并且需要还原为struct 结构时, 必须要:
		* 先确定struct 格式, 保证格式正确且统一
		* 然后先查询一下, 命令会返回多少条命中数据
		* 然后创建n 个struct 缓冲区
		* 最后才执行sqlite 命令查询
*/
int callback_restore_data(void *para, int f_num, char *f_value[], char *f_name[]){
	//静态变量offset记录callback_print_all()回调函数执行次数
	static int offset=0;
	int i;
	struct Student *p = (struct Student*)para+offset;



	//每次只能打印一个数据, f_name=key, f_value=value, f_num=读取到的数据条目总数, void *para=数据实体指针, 读完之后需要释放
	//for(i=0;i<f_num;i++)
		//printf("%s: \t%s\n",f_name[i],f_value[i]);

	//字符串数据还原(还原后, 会被直接放入struct 中, 前提是: struct 指针已经被实体化了)
	p->num = atoi(f_value[0]);
	strncpy(p->name,f_value[1],16);
	strncpy(p->sex,f_value[2],4);
	p->age = atoi(f_value[3]);
	p->score = atoi(f_value[4]);


	//每读取一条数据, 就调用一次回调函数, 就执行offset加1
	offset++;
	return 0;
}

//打印已经还原的struct Student数据
void print_student_array(struct Student stu[],int len){
	int i;
	printf("num\tname\tsex\tage\tscore\n");
	for(i=0;i<len;i++)
		  printf("%d\t%s\t%s\t%d\t%d\n",stu[i].num,stu[i].name,stu[i].sex,stu[i].age,stu[i].score);
}

//回调函数: 获取'读到的sqlite3 数据'的条目数量
int callback_get_count(void *para, int f_num, char *f_value[], char *f_name[]){
	int *p = para;
	*p = atoi(f_value[0]);
	return 0;
}

//回调函数: 打印读取到的sqlite3 数据(只打印数据, 其它不管, 低级)
int callback_print_all(void *para, int f_num, char *f_value[], char *f_name[]){
	int i;
	//每次只能打印一个数据, f_name=key, f_value=value, f_num=读取到的数据条目总数, void *para=数据实体指针, 读完之后需要释放
	for(i=0;i<f_num;i++)
		printf("%s: \t%s\n",f_name[i],f_value[i]);
	return 0;
}

//sqlite3 打开数据库 and 读取数据测试
bool open_read_test(void){
	//命令: 查询student 表的所有数据
	const char comm_select[]="select * from student;";
	//命令: 查询student 表有多少条数据
	const char comm_select_count[]="select count(*) from student;";
	int data_count = 0;
	char *errmsg;
	sqlite3 *db3;
	struct Student *pdata_buf = NULL;



	//打开sqlite3 数据库
	if(sqlite3_open(sqlite3_db_filepath, &db3) != SQLITE_OK){
		perror("cannot open sqlite3 database file");
		return false;
	}


	//执行sqlite3 数据库命令: 查询student表所有数据, 将结果抛给回调函数, 直接打印数据后退出, 不返回, 不保存
	if(sqlite3_exec(db3,comm_select,callback_print_all,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}

	//执行sqlite3 数据库命令: 查询student表所有数据, 将结果抛给回调函数, 直接打印数据后退出, 不返回, 不保存
	if(sqlite3_exec(db3,comm_select_count,callback_get_count,(void*)&data_count,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}
	printf("data_count=%d\n",data_count);

	//创建struct 结构体读取缓冲区
	pdata_buf = malloc(sizeof(struct Student) * data_count);

	//执行sqlite3 数据库命令: 查询student表所有数据, 将结果抛给回调函数, 执行数据还原, 得到还原后的数据
	if(sqlite3_exec(db3,comm_select,callback_restore_data,(void*)pdata_buf,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}

	printf("打印已经还原的struct 数据:\n");
	print_student_array(pdata_buf, data_count);


	free(pdata_buf);
	sqlite3_close(db3);
	return true;
}





//不使用回调函数, 读取sqlite3 数据库中的数据
bool open_read_test2(void){
	//命令: 查询student 表的所有数据
	const char comm_select[]="select * from student;";
	char *errmsg;
	sqlite3 *db3;
	struct Student *pdata_buf = NULL;
	int nrow,ncolumn;
	unsigned int i,j;
	char **data_return;



	//打开sqlite3 数据库
	if(sqlite3_open(sqlite3_db_filepath, &db3) != SQLITE_OK){
		perror("cannot open sqlite3 database file");
		return false;
	}

	//不使用回调函数, 读取sqlite3 数据库中的数据
	if(sqlite3_get_table(db3,comm_select,&data_return,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}

	printf("有多少条数据count=nrow is %d, 每条数据有多少个数据=ncolumn is %d\n",nrow,ncolumn);

	//data_return内结果包含表头信息, 因此data_return[0]是表头信息, 实际数据范围为data_return[1]~data_return[nrow]
	//读出来的, 都是字符串数据, 需要转换才能变成struct 数据
	for(i=1;i<=nrow;i++){
		for(j=0;j<ncolumn;j++){
			printf("%s:%s\n",data_return[j],data_return[i*ncolumn+j]);
		}
	printf("------------------\n");
	}
	printf("表头信息data_return[0] is %p\n",data_return);


	sqlite3_close(db3);
	return true;
}





//sqlite3 打开数据库 and 修改/删除指定数据测试
bool open_write_test(void){
	//命令: 修改指定的值(table_name在创建sqlite3_db 的时候就已经指定了)
	const char comm_update[]="update student set name='adan shaw' where num='10101';";
	//命令: 删除指定的条目
	const char comm_delete[]="delete from student where num>'10102';";
	//const char comm_delete[]="delete from student where num='10102';";
	//命令: 查询student 表的所有数据
	const char comm_select[]="select * from student;";
	char *errmsg;
	sqlite3 *db3;



	//打开sqlite3 数据库
	if(sqlite3_open(sqlite3_db_filepath, &db3) != SQLITE_OK){
		perror("cannot open sqlite3 database file");
		return false;
	}


	//执行sqlite3 数据库命令: 修改指定的值
	if(sqlite3_exec(db3,comm_update,NULL,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}

	//执行sqlite3 数据库命令: 删除指定的值
	if(sqlite3_exec(db3,comm_delete,NULL,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}

	//执行sqlite3 数据库命令: 查询student表所有数据, 将结果抛给回调函数, 直接打印数据后退出, 不返回, 不保存
	if(sqlite3_exec(db3,comm_select,callback_print_all,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		sqlite3_close(db3);
		return false;
	}

	sqlite3_close(db3);
	return true;
}




int main(void){
	//sqlite3 创建数据库 and 插入数据测试
	if(create_insert_test())
		printf("create_insert_test() ok\n");

	//sqlite3 打开数据库 and 读取数据测试
	if(open_read_test())
		printf("open_read_test() ok\n");

	//sqlite3 打开数据库 and 修改/删除指定数据测试
	if(open_write_test())
		printf("open_write_test() ok\n");

	//sqlite3 打开数据库 and 读取数据测试(不使用回调函数, 读取sqlite3 数据库中的数据)
	if(open_read_test2())
		printf("open_read_test2() ok\n");

	return 0;
}


