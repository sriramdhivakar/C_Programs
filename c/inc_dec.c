#include <stdio.h>

int main(void)
{
	int a;
        printf("Enter the value of a:");
	scanf("%d",&a);
	printf("The Pre-increment of a is %d\n",++a);
	printf("The value of a is %d\n",a);
	printf("The Post-increment of a is %d\n",a++);
	printf("The value of a is %d\n",a);
	printf("The Pre-decrement of a is %d\n",--a);
	printf("The value of a is %d\n",a);
	printf("The Post-decrement of a is %d\n",a--);
	printf("The value of a is %d\n",a);
	return 0;
}
