#include <stdio.h>

int main(void)
{
	int i,a[5] = {0,1,2,3,4};
	for(i=0;i<=4;i++)
		printf("Value of a is %d\n",*(a+i));
	return 0;
}
