#include <stdio.h>

int main(void)
{
       	unsigned int b = 0;

	unsigned int *p = &b;

	unsigned int a = 5;

	unsigned int *ptr;
	ptr = &a;//value at address a

	unsigned int **pptr;//*(*ptr)=*(&a)
	pptr = &ptr;//value at address ptr

	printf("Value of A is %d\n",a);
	printf("Address of A is %d\n",&a);

	printf("Value of *ptr is %d\n",*ptr);
	printf("Address of ptr is %d\n",ptr);
	
	
	printf("Value of **pptr is %d\n",**pptr);
	printf("Value of *pptr is %d\n",*pptr);
	printf("Address of pptr is %d\n",pptr);
	
	
	return *p;
}
