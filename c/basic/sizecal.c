#include <stdio.h>

int main(void)
{
	printf("The size of char              	(-128 to 127) 			is %ld \n",sizeof(char));
	printf("The size of unsigned char     	(0 to 255) 			is %ld \n",sizeof(unsigned char));
	printf("The size of short int         	(-128 to 127) 			is %ld \n",sizeof(short int));
	printf("The size of unsigned short int	(0 to 255) 			is %ld \n",sizeof(unsigned short int));
	printf("The size of float             	(3.4E-38 to 3.4E+38) 		is %ld \n",sizeof(float));
	printf("The size of int               	(-32768 to 32767) 		is %ld \n",sizeof(int));
	printf("The size of unsigned int      	(0 to 65535) 			is %ld \n",sizeof(unsigned int));
	printf("THe size of long int          	(-2147483648 to 214748,3647) 	is %ld \n",sizeof(long int));
	printf("The size of unsigned long int	(o to 4294967295) 		is %ld \n",sizeof(unsigned long int));
	printf("The size of double           	(1.7E-308 to 1.7E+308) 		is %ld \n",sizeof(double));
	printf("The size of long double      	(3.4E-4932 to 1.1E+4932) 	is %ld \n",sizeof(long double));
	return 0;
}
