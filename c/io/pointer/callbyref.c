#include <stdio.h>

int func(int *a,int *b)
{
	int sum = *a + *b;
	return sum;
}
int main(void)
{
	int a=1,b=2;
	printf("value of sum is %d\n",func(&a,&b));
	return 0;
}

