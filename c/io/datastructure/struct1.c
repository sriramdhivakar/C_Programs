#include <stdio.h>
#include <string.h>

struct student{
	char name[10];
	int roll_no;
	/*struct dob{
		int d;
		int m;
		int y;
	};*/
	int age;
};
int main(void){
	struct student angu,sriram = {"sriram",100,29};
	printf("%s,%d,%d\n",sriram.name,sriram.roll_no,sriram.age);
	strcpy(angu.name,"angu");
	angu.roll_no = 101;
	angu.age = 18;
	printf("%s,%d,%d\n",angu.name,angu.roll_no,angu.age);
	return 0;
}
