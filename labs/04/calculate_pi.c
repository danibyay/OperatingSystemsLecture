/******************************************************************************
Daniela Becerra Gonzalez
calculate pi with threads
http://bit.ly/2mrhVzp   Public google doc with statistics

USAGE: the last line that says "final average" is the valid one.
* ******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//macros
#define NUM_THREADS    4
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100/NUM_THREADS      /* number of times "darts" is iterated */

//global variables
double	biggersumpi, biggeravepi;
int c;

//function prototype
double dboard (int darts);

// dboard subroutine retrieved from https://computing.llnl.gov/tutorials/mpi/samples/C/mpi_pi_reduce.c
double dboard(int darts)
{
  #define sqr(x)	((x)*(x))
  long random(void);
  double x_coord, y_coord, pi, r;
  int score, n;
  unsigned int cconst;  /* must be 4-bytes in size */
  /*************************************************************************
   * The cconst variable must be 4 bytes. We check this and bail if it is
   * not the right size
   ************************************************************************/
  if (sizeof(cconst) != 4) {
     printf("Wrong data size for cconst variable in dboard routine!\n");
     printf("See comments in source file. Quitting.\n");
     exit(1);
     }
     /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
     cconst = 2 << (31 - 1);
     score = 0;

     /* "throw darts at board" */
     for (n = 1; n <= darts; n++)  {
        /* generate random numbers for x and y coordinates */
        r = (double)random()/cconst;
        x_coord = (2.0 * r) - 1.0;
        r = (double)random()/cconst;
        y_coord = (2.0 * r) - 1.0;

        /* if dart lands in circle, increment score */
        if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
             score++;
        }

    /* calculate pi */
    pi = 4.0 * (double)score/(double)darts;
    return(pi);
}

void *average (void *threadid)
{
  int i;
  double	homepi,         /* value of pi calculated by current task */
          avepi,	        /* average pi value for all iterations */
          sumpi;          /* sum of all pi values for each iteration */
  avepi = 0;
  sumpi = 0;
  //c++; //control
  //printf ("hello from thread , c is %d\n", c);
  //printf ("rounds to do %d\n", ROUNDS);
  for (i = 0; i < ROUNDS; i++) {
     homepi = dboard(DARTS);
     sumpi += homepi;
     avepi = sumpi/(i + 1);
  }
  biggersumpi += avepi;
  biggeravepi = (biggersumpi)/(NUM_THREADS);
  //printf("Average value of pi = %10.13f\n", avepi);
  printf("Final average value of pi = %10.13f\n", biggeravepi);
  //printf ("goodbye from thread \n");
  pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
  int rc;
  long t;
  pthread_t threads[NUM_THREADS];
  biggersumpi = 0;
  biggeravepi = 0;
  //c = 0;

  for(t=0;t<NUM_THREADS;t++){
      //printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, average, (void *)t);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }
  printf ("Real value of PI    = 3.1415926535897 \n\n");
  pthread_exit(NULL);
}
