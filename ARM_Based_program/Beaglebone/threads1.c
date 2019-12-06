#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char testRet[10];

/* Read characters from the pipe and echo them to stdout. */

void read_from_pipea (int file)
{
  	FILE *stream;			//  define a file pointer
  	int c;

	//  associates a stream with a file descriptor for reading from the stream
  	stream = fdopen (file, "r");
	
	//  read characters from the stream
  	while ((c = fgetc (stream)) != EOF)
  	{
  		putchar (c);							//  display to stdout
	}
  	fclose (stream);							//  close the stream
}

/* 
 *   Write some random text to the pipe.
 *   Test passing pointer to a container via pipe
 */

void write_to_pipea (int file, char* containerPtr)
{
  	FILE *stream; 								//  define a file pointer

	//  associates a stream with a file descriptor for writing to the stream
  	stream = fdopen (file, "w");
  
	//  write some data to the stream
	fprintf (stream, "hello, world!\n");
  	fprintf (stream, "goodbye, world!\n");
  	fprintf (stream, containerPtr);

  	fclose (stream);							//  close the stream
}


/* Read characters from the pipe and echo them to stdout. */

void read_from_pipeb (int file)
{
   	FILE *stream;								//  define a file pointer
  	int c;
	int i = 0;

	//  associates a stream with a file descriptor for reading from the stream
  	stream = fdopen (file, "r");

	//  read characters from the stream
  	while ((c = fgetc (stream)) != EOF)
  	{
  		putchar (c); 							//  display to stdout
		testRet[i++] = c;						//  write to a global test buffer
	}
  	fclose (stream);							//  close the stream
}

/* 
 *  Write some random text to the pipe. 
 */

void write_to_pipeb (int file, char* b)
{
    	FILE *stream; 							//  define a file pointer

	//  associates a stream with a file descriptor for writing to the stream
  	stream = fdopen (file, "w");
  
	//  write some data to the stream
 	 fprintf (stream, "bonjour, world!\n");
 	 fprintf (stream, "chao, world!\n");
 	 fprintf (stream, b);  

  	fclose (stream); 			//  close the stream
}

/*
 *   test putting child process in a function
 */ 
int childTest0(pid_t aPid, int mypipe0[2], int yourpipe0[2], char data[]);


int main (void)
{
	/* pid_t is a signed integer type which is capable of representing a process ID */
  	pid_t pid;

	//  define 2 pipes
  	int mypipe[2];
  	int yourpipe[2];

	//  test data  
  	char data[] = "testa \n";
  	char data2[] = "fire rockets \n";

  	/* Create the pipes mypipe and yourpipe */
  	if (pipe (mypipe))
    	{
      	fprintf (stderr, "Pipe failed.\n");
      	return EXIT_FAILURE;
    	}
	if (pipe (yourpipe))
    	{
      	fprintf (stderr, "Pipe failed.\n");
      	return EXIT_FAILURE;
    	}
		
  	/* Create the child process. to talk to */
  
	pid = fork ();
  	if (pid == (pid_t) 0)							//	if fork successful and child process created
    	{
		//  comment out function to run locally

		/*
     		 *  pass two pipes and ptr to container to a test function
		 */
		childTest0(pid, mypipe, yourpipe, data2); 


		//  uncomment following to run locally
		/*	
		   	printf("read in child from mypipe\n");

			// This is the child process. close other end first configure for reading.
      		
			close (mypipe[1]);
      		read_from_pipea (mypipe[0]);
	  
	  		printf("got data from parent following read \n");
	  
			//  delay 2 seconds
	  		sleep(2);

	  		printf("write in child to yourpipe\n");

			// This is the parent process. close other end first configure for writing.	  
	  		close (yourpipe[0]);
	  		write_to_pipeb (yourpipe[1], data2);
		*/	  
      	return EXIT_SUCCESS;
    	}

 	
	
	else if (pid < (pid_t) 0)
    	{
      	/* The fork failed. */
      	fprintf (stderr, "Fork failed.\n");
      	return EXIT_FAILURE;
    	}
  	
	else
    	{
      	/* 
		 * This is the parent process running locally  
		 *  Configure to write to child 
		 */
	  	printf("write in parent to child - mypipe\n");		 
      	close (mypipe[0]);
      	write_to_pipea (mypipe[1], data);
	 
		//  delay
		sleep(2);

		/*
         *  This is the parent process running locally
 		 *. Configure to read from child	
		 */  
	  	
		printf("read in parent from yourpipe\n");
      	close (yourpipe[1]);
      	read_from_pipeb (yourpipe[0]);
	  
		//  checking global test data from child
	  	printf("command is:  %s\n", testRet);
      	return EXIT_SUCCESS;
    }
}




//  Utilizing the pipe stricture from a function

int childTest0(pid_t aPid, int mypipe0[2], int yourpipe0[2], char dataRet[])
{
	/*
 	 * Pass in ids for two pipes, container local to main
	 */
	char data1[10];										//  local container
	
	printf("read in child from mypipe\n");				//  instrument the process
    		
	/*
	 *  prepare pipe from parent for reading
	 */
	close (mypipe0[1]); 								
	
	read_from_pipea (mypipe0[0]);						//  read from parent

	printf("got data from parent following read \n");	//  instrument the process
	sleep(2);											//  delay
	
	/*
	 *  test external interface to child process
	 */
	printf("prompting user - enter a command\n");	
	scanf("%s", &data1);								//  read command from user
	
	printf("1 the data is: %s\n", data1);				//  verify data locally
	sleep(2);											//  delay

	printf("write in child to yourpipe\n");	  			//  instrument the process
	close (yourpipe0[0]); 								//  prepare pipe for reading
	
	sleep(5);											//  delay
	
	strcpy(dataRet, data1);								//  copy local data to passed in 
														//  container
	printf("2 the data is: %s\n", dataRet);

	write_to_pipeb (yourpipe0[1], dataRet);				//  write same data to parent pipe
	sleep(5);											//  delay

	return 0;											//  exit
}
