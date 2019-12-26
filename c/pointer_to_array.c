#include <stdio.h>

int main(void)
{
	int a[5] = {0,1,2,3,4};
	int *p,i;
	p = a;
	for(i=0;i<=4;i++)
		printf("%d\n",*(p+i));
	return 0;
}
