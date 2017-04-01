// Sleeping barber problem.
// gcc barber1.c -lpthread -lm
// Daniela Becerra Gonzalez
// Took the Dickinson College example and made it simpler

#define _REENTRANT

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CUSTOMERS 5
#define CHAIRS 3
#define SEED 4

void *customer(void *number);
void *barber(void *);
void randwait(int secs);

sem_t waitingRoom;
sem_t barberChair;
sem_t barberPillow;
sem_t seatBelt;
int customersFinished = 0;
int customerInProgress = 20;

int main() {
    pthread_t barbThread;
    pthread_t tid[CUSTOMERS];
    int i;
    int Number[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    srand48(SEED);
    // create semaphores
    sem_init(&waitingRoom, 0, CHAIRS);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);
    //create threads
    pthread_create(&barbThread, NULL, barber, NULL);
    for (i=0; i<CUSTOMERS; i++) {
	    pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
    }
    //join
    for (i=0; i<CUSTOMERS; i++) {
	    pthread_join(tid[i],NULL);
    }
    // When customer threads are finished, kill barber thread.
    customersFinished = 1;
    sem_post(&barberPillow);  // wake the barber so he will exit.
    pthread_join(barbThread,NULL); // we can exit from main now
}

void *customer(void *number) {
    int id  = *(int *)number;
    printf("Customer %d leaving for barber shop.\n", id );
    randwait(5);
    printf("Customer %d arrived at barber shop.\n", id );
    // Wait for a space in waiting room
    sem_wait(&waitingRoom);
    printf("Customer %d entering waiting room.\n", id );
    sem_wait(&barberChair); // you got a chair, lock it
    sem_post(&waitingRoom); // free the waiting room space
    printf("Customer %d waking the barber.\n", id );
    sem_post(&barberPillow);
    // Wait for the barber to finish cutting customer's hair.
    sem_wait(&seatBelt);
    sem_post(&barberChair); // Give up the chair.
    printf("Customer %d leaving barber shop.\n", id );
}

void *barber(void *junk) {
    while (!customersFinished) {
  	  // Sleep until someone arrives and wakes you..
  	  printf("The barber is sleeping\n");
  	  sem_wait(&barberPillow);
  	  // Skip this stuff at the end...
  	  if (!customersFinished) {
  	    printf("The barber is cutting hair\n");
  	    randwait(3);
  	    printf("The barber has finished cutting hair.\n");
  	    sem_post(&seatBelt); // release customer
  	  }
  	  else {
  	    printf("The barber is going home for the day.\n");
  	  }
   }
}

void randwait(int secs) {
    int len;
    len = (int) ((drand48() * secs) + 1);
    sleep(len);
}
