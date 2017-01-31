//Daniela Becerra Gonzalez
//program that takes two arguments to write to a file,
//whether it's the first time or to append to it
//args[1] = name of the file    args[2] = string to be written
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false, true } bool;

bool check(int rc, char * msg)
{
  if(rc < 0)
  {
    printf("error while %s to file \n", msg);
    return true;
  }
  else
  {
    return false;
  }
}

 int main(int argc, char *argv[])
 {
   if( argc == 3 )
   {
      int file = open(argv[1], O_WRONLY | O_CREAT | O_APPEND);
      if(check(file, "opening")) return 1;
      int rc = syscall(SYS_write, file, argv[2], strlen(argv[2]));
      if(check(rc, "writing")) return 1;
      rc = syscall(SYS_write, file, "\n", 1);
      if(check(rc, "writing")) return 1;
      rc = close(file);
      if(check(rc, "closing")) return 1;
      printf("your content was successfully written to %s\n", argv[1]);
   }
   else
   {
      printf("Two arguments expected.\n Name of the file to be written, and what to write on it \n");
   }
   return 0;
 }
