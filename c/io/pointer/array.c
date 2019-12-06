#include <stdio.h>

int main(void)
{
	int i,a[] = {0,1,2,3,4};
	for(i = 0;i <= 4;i++)
		printf("value of a[%d] is %d\n",i,a[i]);
		printf("Address of a[%d] is %d\n",i,&a[i]);
	return 0;
}
