#include <stdio.h>

int main(void)
{
	int a,b;
	printf("Enter the value of a:");
	scanf("%d",&a);
	printf("Enter the value of b:");
	scanf("%d",&b);
	printf("For less than operation: %d\n",a<b);
	printf("For less than equal operation: %d\n",a<=b);
	printf("For greater than operation: %d\n",a>b);
	printf("For greater than equal operation: %d\n",a>=b);
	printf("For equal to operation: %d\n",a==b);
	printf("For not equal operation: %d\n",a!=b);
	return 0;
}
