#include <stdio.h>

int func(void){
	return 999;
}

#define do_func(x) {if((x=func()) == 999) printf("yes, x=%d\n",x);}

int main(void){
	int x = 0;
	do_func(x);
	return 0;
}
