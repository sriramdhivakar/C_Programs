//  Must compile using 

//  gcc -o fileName fileName.c -Wall -Werror -lpthread

/* Includes */
#include <unistd.h>     		/* Symbolic Constants */
#include <sys/types.h>  		/* Primitive System Data Types */ 
#include <errno.h>      		/* Errors */
#include <stdio.h>      			/* Input/Output */
#include <stdlib.h>     			/* General Utilities */
#include <pthread.h>    		/* POSIX Threads */
#include <string.h>     		/* String handling */
#include <semaphore.h>  	/* Semaphore */

#define NUM_THREADS 5

/*  
 *  test function for each thread.  all threads are using the same function
 *  different threads may use different functions
 */
 void *print_hello(void *threadid) 
{
	long tid;
	tid = (long) threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) 
{
	pthread_t threads[NUM_THREADS];				//  number of threads to create
	int rc;												//  return error code
	long t;												//  working thread count variable
	
	/*
       *	create 5 threads
	 */
	for (t = 0; t < NUM_THREADS; t++) 
	{
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(threads + t, NULL, print_hello, (void *) t);
		if (rc) 
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* 
	 *  wait for all threads to complete
	 */
	for (t = 0; t < NUM_THREADS; t++) 
	{
		pthread_join(threads[t], NULL);
	}
	pthread_exit(NULL);
}
