/*Author:  Bryce Ault
 *Course:  CSCI 4330
 *Lab number: Lab 1
 *Purpose: This program spawns a number of threads specified by the user
		and prints "Hello World" for each thread.
 *Due date: January 25, 2018 11:59pm
*/

#include <stdio.h>
#include <pthread.h>

/*maximum number of threads*/
const int MAX = 10;

/*Code executed by threads (and any other function that calls it)*/
void * childThread (void * childsParam)
{
	/*Typical extraction of which thread a child is*/
	int childNum = *(int*)childsParam;

	//print "Hello World" from current thread
	printf("Hello World from child thread %d\n", childNum);
}

int main(int argc, char *argv[])
{
	/* request number of threads to create from user	*/
	printf("Enter number of threads to spawn: \n");
	int numThreads;
	scanf("%d", &numThreads);

	//if input exceeds maximum number of threads, then exit
	if(numThreads>MAX || numThreads<1)
	{
		printf("Invalid number of threads...\nMax threads: %d\n", MAX);
		exit(-1);
	}

	/*thread id variables*/
	pthread_t ids[numThreads];

	/*array of ints used in spawning threads*/
	int nums[numThreads];

	/*other variables*/
	int i; 
	
	printf("Input number of threads: %d\n", numThreads);

	/*create threads and start them executing*/
	for(i=0;i<numThreads;i++)
	{
		nums[i]=i;
		pthread_create(&ids[i], NULL, childThread, (void*)&nums[i]);
	}	

	/*clean up threads*/
	for(i=0;i<numThreads;i++)
	{
		pthread_join(ids[i],NULL);
	}

	printf("That's all from the parent\n");

	return 0;
}
