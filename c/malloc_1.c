#include <stdio.h>
#include <stdlib.h>

int main (void){
int *p,n,i;

printf("Enter how many integer to be added : ");
scanf("%d",&n);

p = (int*)malloc(n*sizeof(int));
if(p == NULL)
{
	printf("Memory is not allocated\n");
	exit(1);
	/*return 1;*/
}

for(i=0;i<n;i++)
{
	printf("Enter the integer:");
	scanf("%d",p+i);
}

for(i=0;i<n;i++)
{
	printf("address at heap is %ld\n",p+i);
	printf("value at heap is %d\n",*p+i);
}

free(p);

return 0;
}

