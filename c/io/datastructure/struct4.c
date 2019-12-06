#include <stdio.h>
#include <string.h>

struct student{
	char name[10];
	int roll_no;
	int age;
};
int main(void){
	struct student *angu,sriram = {"sriram",100,29};
	printf("%s,%d,%d\n",sriram.name,sriram.roll_no,sriram.age);
	angu = &sriram;
	//printf("%s,%d,%d\n",*angu.name,*angu.roll_no,*angu.age);
	printf("%s,%d,%d\n",angu->name,angu->roll_no,angu->age);
	printf("%ld\n",sizeof(angu));
	printf("%ld\n",sizeof(sriram));
	return 0;
}
