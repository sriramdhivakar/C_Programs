#include <stdio.h>

enum week {
	Monday,
	Tuesday,
	Wednessday,
	Thursday,
	Friday,
	saturday,
	Sunday
};

int main(void)
{
	enum week day;
	day = Monday;
	printf("%i\n",day);
	return 0;
}
