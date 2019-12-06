//  rw0.c
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

typedef enum {wrt, rd} rw;

int fd0;    
char buf[MAX_BUF];

void funct0(int fd0, rw coms);
void funct1(int fd0);

int main()
{
    	char * myfifo0 = "/tmp/myfifo0";
	    
	/* create the FIFO (named pipe) */
    	mkfifo(myfifo0, 0666);

    	/* open the FIFO */
	
    	fd0 = open(myfifo0, O_RDWR);
	
	funct0(fd0, wrt);		//  write to fifo
	funct1(fd0);			//  read then write to the fifo
	funct0(fd0, rd);		//  read fromn the fifo
	
    	close(fd0);

    	/* remove the FIFO */
    	//unlink(myfifo0);

    	return 0;
}





/*
 *  Test function0
 */
 
 //  test write then read back
 
void funct0(int fd0, rw coms)
{	
	char buf[MAX_BUF];
	int size = 0;
	
	//  testing
	printf("in funct0\n");
	
	// check for write or read
	switch(coms)
	{
		case wrt: 
		{
			printf("send command\n");
			size = sizeof("Launch Rocket");
			write(fd0, "Launch Rocket", size);
			break;
		}
		case rd:
		{
			printf("receive response\n");
			size = sizeof("Roger That\n");
			read(fd0, buf, MAX_BUF);
			printf("Received: %s\n", buf);
			break;
		}
	}
	return;
}

 
/*
 *  Test function1
 */

//  test read then write back

void funct1(int fd0)
{
	char buf[MAX_BUF];
	int size = sizeof("Roger That\n");
	
	printf("in funct1\n");
		
	/* read and display the message from the FIFO */
    	read(fd0, buf, MAX_BUF);
	
    	printf("Received command: %s\n", buf);
	
	/* write message to the FIFO */	
	printf("ack command\n");	
	write(fd0, "Roger That\n", size);
	
	return;
}
