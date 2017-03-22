/*
* Daniela Becerra Gonzalez --- semaphores!!
* The shared memory simulates a database between clients and a server that want to buy milk
* The clients can only buy less than 10 bottles at a time (program only reads one digit)
*  the '*' character is the program terminator, use it to exit
*  first run the server, many clients can run this program and they will all use the same inventory
*  If one client exits it will close the server but the other clients have to be exited
*  manually with the star character
*  Run the program and enter the number of milk bottles you want to buy
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27
#define LF 10
char SEM_NAME[]= "milk";

int main()
{
  char ch;
  int shmid;
  key_t key;
  char *shm,*s;
  sem_t *my_semaph;
  char num[2];
  //name the shared memory segment
  key = 1000;
  //create & initialize existing semaphore
  my_semaph = sem_open(SEM_NAME,0,0644,0);
  if(my_semaph == SEM_FAILED)
    {
      perror("reader:unable to execute semaphore");
      sem_close(my_semaph);
      exit(-1);
    }
  //create the shared memory segment with this key
  shmid = shmget(key,SHMSZ,0666);
  if(shmid<0)
    {
      perror("reader:failure in shmget");
      exit(-1);
    }
  //attach this segment to virtual memory
  shm = shmat(shmid,NULL,0);

  //start reading
  s = shm;

  printf("Enter how many bottles of milk you want to buy:\n");
  //once done signal exiting of reader
  for(;;){
      printf("#");
      //get user input
      if ( fgets (num , 2 , stdin) == NULL ) {
        printf("error while getting string from user\n");
        return 1;
      }
      if(num[0]=='*'){ //exit if user indicates it
        *shm = '*';
        break;
      }
      if(num[0]!=LF){ // read user entry of bottles quantity
        char to_buy = (num[0]-0x30);
        printf("you want to buy %d bottles of milk\n", to_buy);
        char bottles = 0;

        if(*shm!=NULL){ // read bottles inventory from server
          sem_wait(my_semaph);
          bottles = *shm;
          sem_post(my_semaph);
        }
        printf("there are %d bottles of milk\n", bottles);

        if(bottles>=to_buy){ // update inventory at server
          char left = bottles - to_buy;
          sem_wait(my_semaph); // write how many bottles are left now
          *shm = left;
          sem_post(my_semaph);
          //printf("there now should be %d bottles of milk\n", left);
          char left_lecture;
          sem_wait(my_semaph); // confirm we wrote it and tell the user
          left_lecture = *shm;
          sem_post(my_semaph);
          printf("there now are %d bottles of milk\n", left_lecture);
        }else if(bottles==0){
          printf("sorry, we're out of milk, please come back later\n");
        }else{
          printf("sorry, we don't have enough milk, please buy less\n");
        }

      }
  }

  sem_close(my_semaph);
  shmctl(shmid, IPC_RMID, 0);
  exit(0);
}
