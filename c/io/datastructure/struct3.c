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

	struct student sriram[3] = {
		{"sriram",100,29},
		{"angu",101,19},
		{"Bharati",102,39}
	};

	printf("%s,%d,%d\n",sriram[0].name,sriram[0].roll_no,sriram[0].age);

	printf("%s,%d,%d\n",sriram[1].name,sriram[1].roll_no,sriram[1].age);
	
	printf("%s,%d,%d\n",sriram[2].name,sriram[2].roll_no,sriram[2].age);
	return 0;
}
