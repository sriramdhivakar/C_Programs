#include <stdio.h>

int main(void)
{
	int a,b,c;
	printf("Enter the values:");
	scanf("%d %*d %d",&a,&b,&c);
	printf("%d %d %d",a,b,c);
	return 0;
}

