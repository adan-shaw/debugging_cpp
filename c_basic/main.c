//编译:
//		gcc -g3 ./main.c -o x



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>



#define OPTSTR "i:o:f:h"
#define USAGE_FMT "%s [-i inputfile] [-o outputfile] [-n str_num] [-v] [-h]\n"
#define DEFAULT_PROGNAME "x"

extern int errno;
extern char *optarg;
extern int opterr, optind;

typedef struct {
	int					 val;
	unsigned int flags;
	FILE				 *input;
	FILE				 *output;
} options_t;



void usage(char *progname, int opt){
	fprintf(stderr, USAGE_FMT, progname ? progname : DEFAULT_PROGNAME);
	exit(EXIT_FAILURE);
}

int do_the_needful(options_t *options){
	if(!options){
		errno = EINVAL;
		return EXIT_FAILURE;
	}

	if(!options->input || !options->output){
		errno = ENOENT;
		return EXIT_FAILURE;
	}

	//未知错误处理方式
	printf("unknow error!!\n");

	return EXIT_SUCCESS;
}

/*
void on_exit_func(int exit_stat, void *arg){
	options_t *p = (options_t*)arg;
	printf("on_exit_func() called: status=%d, arg=%ld\n", exit_stat, (long) arg);
	p->val = 2;					//p 指针传递没问题
	//fflush(p->input);	//程序关闭时, 不需要重复释放资源
	//fflush(p->output);
	//fclose(p->input);
	//fclose(p->output);
	return;
}
*/



int main(int argc, char *argv[]){
	int opt = -1, opt_count = 0;
	//设置默认input = stdin, output = stdout
	options_t options = { 0, 0x0, stdin, stdout };
	opterr = 0;

	//一般不需要通过on_exit() 退出函数释放资源, 程序释放时会自动释放资源的
	/*
	if(on_exit(on_exit_func, (void *)&options) != 0){
		perror("on_exit()");
		return -1;
	}
	*/

	while((opt = getopt(argc, argv, OPTSTR)) != EOF)
		switch(opt){
			case 'i':
				if(!(options.input = fopen(optarg, "rb")) ){
					perror("fopen(input, r)");
					exit(EXIT_FAILURE);
				}
				opt_count++;
				break;
			case 'o':
				if(!(options.output = fopen(optarg, "wb")) ){
					perror("fopen(output, w)");
					exit(EXIT_FAILURE);
				}
				opt_count++;
				break;
			case 'n':
				//str to number
				options.flags = (unsigned int)strtoul(optarg, NULL, 16);
				break;
			case 'v':
				options.val += 1;
				break;
			case 'h':
			default:
				usage(basename(argv[0]), opt);
				/* NOTREACHED */
				break;
		}

	//如果option 只是为了传递参数, 这里可以传入必定处理的function API 调用(小功能程序常用)
	if(opt_count == 2){
		return EXIT_SUCCESS;
	}

	if(do_the_needful(&options) != EXIT_SUCCESS){
		perror("do_the_needful() blew up");
		exit(EXIT_FAILURE);
	}
	usage(basename(argv[0]), opt);
	return EXIT_SUCCESS;
	/* NOTREACHED */
}
