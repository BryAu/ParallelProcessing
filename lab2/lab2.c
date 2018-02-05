/*Author:  Bryce Ault
 *Course:  CSCI 4330
 *Lab number: Lab 2
 *Purpose: Pthreads program that calculates the integral of sqrt(4-x^2) using Trapezoidal and
        Simpson's rule. The program requests the number of threads to spawn and that will be 
        the same number of partitions generated to calculate the integral.
 *Due date: Febuary 8, 2018
*/

#include <stdio.h>
#include <pthread.h>
#include <math.h>

const int NUM = 50; 
double width;                /* the width of a segment   */
double tsum = 0.0;           /* the trapezoidal rule result   */
double ssum = 0.0;           /*  the Simpson's rule result   */
pthread_mutex_t mutex1;     /* mutual exclusion variable   */

/*  function calculates the area of a parabola as a single
    segment of the whole integral.
    aka Simpson's Rule    */
void *simpson(void *mine)
{
    double partial_sum = 0.0; /*  area under current segment  */ 
    int me = *((int *)mine);    /*  which segment belongs to this thread    */
	double mid;		      /* midpoint of my segment */
    
    /*  to help simplify calculating f(a) and f(b),
        store the X locations  */
    double posA, posB;
    posA = me*width;
    posB = posA+width;

    /*  find my midpoint    */
    mid = (posA+posB)/2.0;

    /* Calculate f(a), f(b) and f(m)   */
    double heightA, heightB, heightM;
    heightA = sqrt(4.0 - posA*posA);
    heightB = sqrt(4.0 - posB*posB);
    heightM = sqrt(4.0 - mid*mid);

    /*  Calculate segment area.
        Formula here is (deltaX/3)(f(a) + 4f(m) + f(b))
        We use width divided by 6 because deltaX = width/2  */
    partial_sum = (width/6.0)*(heightA + 4*heightM + heightB);

    /*  add to global area  */
    pthread_mutex_lock(&mutex1);
		ssum += partial_sum;
	pthread_mutex_unlock(&mutex1); 
}

/*  fucntion calculates a the area of a trapezoid
    as a single segment of the whole integral    */
void *trap(void *mine)
{
    double partial_sum = 0.0; /*  area under current segment  */
    int me = *((int *)mine);    /*  which segment belongs to this thread    */

    /*  to help simplify calculating f(a) and f(b),
        store the X locations  */
    double posA, posB;
    posA = me*width;
    posB = posA+width;

    /* Calculate f(a) and f(b)   */
    double heightA, heightB;
    heightA = sqrt(4.0 - (posA)*(posA));
    heightB = sqrt(4.0 - (posB)*(posB));

    /*  calculate segment area
        Formula here is (b-a)(f(a) + f(b))/2  */
    partial_sum = ((heightA + heightB)*width)/2;

    /*  add to global area  */
    pthread_mutex_lock(&mutex1);
		tsum += partial_sum;
	pthread_mutex_unlock(&mutex1); 
}

int main()
{
    /*  always i for flow control   */
    int i, noparts;    /*  partitions used to approx. area */

	int seg[NUM];          /* for designating segment numbers   */       
	pthread_t threadr[NUM]; /* store thread IDs   */

    /*  Request number of threads/partitions from user.
        Each thread will operate on its own partition of the integral.  */
    printf("Enter number of partitions: ");
    scanf("%d", &noparts);
    if(noparts > 50)
	{
		printf("number must be <= %d\n",NUM);
		return(0);
	}

    /*  Calculate width of each partition.
        Width is same for all partitions, OK to store in global var.
        Integral is calculated from 0 to 2, divide interval of 2 into number of partitions.   */
    width = 2.0/noparts;

    /*  create threads  */
	for(i=0;i<noparts;i++)
	{
		seg[i]=i;
		if(pthread_create(&threadr[i],NULL,trap,(void *)&seg[i]) != 0)
			perror("Pthread_create fails");
	}

    /*  wait for all threads    */
	for(i=0;i<noparts; i++)
		if (pthread_join(threadr[i],NULL) != 0)
			perror("Pthread_join fails");

    /*  create threads  */
	for(i=0;i<noparts;i++)
	{
		seg[i]=i;
		if(pthread_create(&threadr[i],NULL,simpson,(void *)&seg[i]) != 0)
			perror("Pthread_create fails");
	}

    /*  wait for all threads    */
	for(i=0;i<noparts; i++)
		if (pthread_join(threadr[i],NULL) != 0)
			perror("Pthread_join fails");


    /*  print the area for each */
	printf("The area under the curve using Trapezoidal Rule is: %f\n",tsum);
	printf("The area under the curve using Simpson's Rule is: %f\n",ssum);    

    return 0;
}
