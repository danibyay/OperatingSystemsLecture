/* Taken from
 * http://www.linuxdevcenter.com/pub/a/linux/2007/05/24/semaphores-in-linux.html?page=5
 * Daniela Becerra Gonzalez -- Semaphores!
 * The shared memory simulates a database between clients and a server that want to buy milk
 * Any number of bottles can be used as the inventory, the '*' character is the program terminator
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
#define BOTTLES 10
char SEM_NAME[]= "milk";

int main()
{
  char ch;
  int shmid;
  key_t key;
  char *shm,*s;
  sem_t *my_semaph;
  //name the shared memory segment
  key = 1000;
  //create & initialize semaphore
  my_semaph = sem_open(SEM_NAME,O_CREAT,0644,1);
  if(my_semaph == SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(SEM_NAME);
      exit(-1);
    }
  //create the shared memory segment with this key
  shmid = shmget(key,SHMSZ,IPC_CREAT|0666);
  if(shmid<0)
    {
      perror("failure in shmget");
      exit(-1);
    }
  //attach this segment to virtual memory
  shm = shmat(shmid,NULL,0);

   //start writing into memory
   s = shm;
   sem_wait(my_semaph);
   *s++ = BOTTLES;
   sem_post(my_semaph);

  //the below loop could be replaced by binary semaphore
  while(*shm != '*') // '*''
    {
      sleep(1);
    }
  sem_close(my_semaph);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  _exit(0);
}
