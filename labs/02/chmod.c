#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/stat.h>

int main (){

    int rc;

    rc = chmod("password", 0531);  // r-x -wx --x

    if (rc == -1){ //catch the error
        fprintf(stderr, "chmod failed, errno = %d\n", errno);
    }
}
