#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
	unsigned int a = 60;//0110 0000
	unsigned int b = 13;
	int c = 0;

	c = a & b;
	printf("C = %d\n",c);
	
	c = a | b;
	printf("C = %d\n",c);
	
	c = a ^ b;
	printf("C = %d\n",c);
	
	c = ~a;
	printf("C = %d\n",c);
	
	c = a<<2;
	printf("C = %d\n",c);
	
	c = a>>2;
	printf("C = %d\n",c);

	return 0;
}
       	
