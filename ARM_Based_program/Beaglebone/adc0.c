/*
//Filename: adc0.c
//Version : 0.2
//
//Project : Argonne National Lab - Forest
//Author  : Gavin Strunk
//Contact : gavin.strunk@gmail.com
//Date    : 14 June 2013
//
//Description - This program demonstrates how to read
//		the onboard ADC channel 0 and display
//		the value to terminal.
//
//Revision History
//	0.1: Wrote the program and tested \GS
//    0.2  Added comments and second channel \jkp
*/

/*
Copyright (C) 2013 Gavin Strunk

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software i
s furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <time.h>

void pauseSec(int sec);									//  software timer

int main()
{
	FILE *ain,*aval0,*aval1;							//  create some buffers
	int value0,value1,i;								//  working variables

	//  enable the ADC ports
	ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
	fseek(ain,0,SEEK_SET);
	fprintf(ain,"cape-bone-iio");
	fflush(ain);

	while(1)
	{
			//  enable the ADC ports
		aval = fopen("/sys/devices/ocp.3/helper.15/AIN0", "r");
		fseek(aval0,0,SEEK_SET);						// go to beginning of buffer
		fscanf(aval0,"%d",&value0);						//  write analog value to buffer
		fclose(aval0);									//  close buffer
		
		//  delay
		for(i = 0; i<1000000;i++);

		//  enable the ADC ports
		aval1 = fopen("/sys/devices/ocp.3/helper.15/AIN1","r");
		fseek(aval1,0,SEEK_SET); 						// go to beginning of buffer
		fscanf(aval1,"%d",&value1); 					// write analog value to buffer
		fclose(aval1); 									// close buffer				

		//  display readings
		printf("value0: %d  value1: %d\n",value0,value1);

		//  delay
		for(i = 0; i<1000000;i++);
	}

	fclose(ain);
	return 0;
}

//  delay function
void pauseSec(int sec)
{
	time_t now,later;

	now = time(NULL);
	later = time(NULL);

	while((later - now) < (double) sec)
		later = time(NULL);
}
