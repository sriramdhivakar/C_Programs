#include <stdio.h>
#include <stdlib.h>

int main(void){
	int *p,i,n=5;
	p = (int *)malloc(5*(sizeof(int)));
	if(p == NULL)
	{
		printf("Memory is full\n");
		exit(1);
	}
	for(i=0;i<n;i++)
	{
		printf("Enter the integer :\n");
		scanf("%d",p+i);
	}
	for(i=0;i<n;i++)
	{
		printf("%d\t",*p+i);
	}
	free(p);

	return 0;
}
