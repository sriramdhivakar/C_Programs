#include <stdio.h>
#include <string.h>

union student{
	int roll_no;
	int age;
};
int main(void){
        union student sriram = {100,29};
	printf("%d,%d\n",sriram.roll_no,sriram.age);
	return 0;
}
