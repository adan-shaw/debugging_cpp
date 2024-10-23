#include <stdio.h>

int sum(int x){
	int i, result=0;
	for(i=0; i<=x; i++){
		result+=i;
	}
	return result;
}

int main(void){
	int x;
	printf("Input an integer:\n");
	scanf("%d", &x);
	printf("sum=%d\n", sum(x));
	return 0;
}

