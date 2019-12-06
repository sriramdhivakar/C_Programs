#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MYSIG = 5;

void get (void* aNumber);						//  input task
void increment (void* aNumber);					//  computation task
void display (void* aNumber);					//  output task
void myHandler(int aSig);						//  interrupt / signal handler

//  Declare a TCB structure

typedef struct 
{
	void* taskDataPtr;
	void (*taskPtr)(void*);
}
TCB;

int main(void)
{
	signal(SIGUSR1, myHandler);	//  associate the signal with its handler 
	
	int i=0;									//  queue index
	int data;									//  declare a shared data
	int* aPtr = &data;							//  point to it

	TCB* queue[3];								//  declare queue as an array of pointers to TCBs

//  	Declare some TCBs
	TCB inTask;
	TCB compTask;
	TCB outTask;
	TCB* aTCBPtr;
	
	//  	Initialize the TCBs
	inTask.taskDataPtr = (void*)&data;
	inTask.taskPtr = get;

	compTask.taskDataPtr = (void*)&data;
	compTask.taskPtr = increment;

	outTask.taskDataPtr = (void*)&data;
	outTask.taskPtr = display;
	
	// 	Initialize the task queue
	queue[0] = &inTask;
	queue[1] = &compTask;
	queue[2] = &outTask;
	
	while(1)
	{
		aTCBPtr = queue[i];
		aTCBPtr->taskPtr( (aTCBPtr->taskDataPtr) );
                
		i = (i+1)%3;

        usleep(30);
	}
	return EXIT_SUCCESS;
}

void get (void* aNumber)						//  perform input operation
{
	printf ("Enter a number: 0..9 ");
	*(int*) aNumber  = getchar();
	getchar();									//  discard cr
	*(int*) aNumber -= '0';  					//  convert to decimal from ascii
	return;
}


void increment (void* aNumber)					//  perform computation
{
	int* aPtr = (int*) aNumber;
	(*aPtr)++;
	return;
}

void display (void* aNumber)					// perform output operation
{
	printf ("The result is: %d\n", *(int*)aNumber);
	// raise(SIGINT);
	raise(SIGUSR1);
	return;
}

void myHandler(int aSig)
{
	printf("in the handler\n");
	
	return;
}
