#include <stdio.h>

int factorial(int a);

int main(void)
{
	int number;

	printf("Enter the Factorial number: ");

	scanf("%d",&number);

	printf("The Factorial value is :%d\n",factorial(number));
}

int factorial(int a)
{
	if(a>1)
	{
		return(a*(factorial(a-1)));
	}
	else
	{
		return (1);
	}
}
	
