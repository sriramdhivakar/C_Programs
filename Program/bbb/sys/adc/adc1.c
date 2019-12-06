#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>		//close()
#include <fcntl.h>		//define O_WONLY and O_RDONLY
#define MAX_BUF 64		//This is plenty large

//Function declarations  
int readADC (unsigned int pin);

//main program 
int main ()
{

  //Enable ADC pins within code
  //system ("echo BB-ADC > /sys/devices/bone_capemgr.*/slots");

  //Read ADCs  
  int adc0 = readADC (0);
  /*int adc1 = readADC (1);
  int adc2 = readADC (2);
  int adc3 = readADC (3);
  int adc4 = readADC (4);
  int adc5 = readADC (5);
  int adc6 = readADC (6);*/

  //Print ADC readings
  printf ("ADC 0: %d\n", adc0);
  /*printf ("ADC 1: %d\n", adc1);
  printf ("ADC 2: %d\n", adc2);
  printf ("ADC 3: %d\n", adc3);
  printf ("ADC 4: %d\n", adc4);
  printf ("ADC 5: %d\n", adc5);
  printf ("ADC 6: %d\n", adc6);*/

  return 0;
}				//end main 

//Function definitions 
int readADC (unsigned int pin)
{
  int fd;			//file pointer
  char buf[MAX_BUF];		//file buffer
  char val[4];			//holds up to 4 digits for ADC value 

  //Create the file path by concatenating the ADC pin number to the end of the string  
  //Stores the file path name string into "buf"
  //snprintf (buf, sizeof (buf), "/sys/devices/ocp.2/helper.14/AIN%d", pin);	//Concatenate ADC file name  
  snprintf (buf, sizeof (buf), "/sys/bus/iio/devices/iio\:device0/in_voltage%d_raw", pin);	//Concatenate ADC file name  
  fd = open (buf, O_RDONLY);	//open ADC as read only
  //Will trigger if the ADC is not enabled
  if (fd < 0)
    {
      perror ("ADC - problem opening ADC");
    }				//end if 

  read (fd, &val, 4);		//read ADC ing val (up to 4 digits 0-1799)
  close (fd);			//close file and stop reading 
  return atoi (val);		//returns an integer value (rather than ascii)
}				//end read ADC()
