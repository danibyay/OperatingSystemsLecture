/* Hello World program */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    printf("Hello World\n");
    int ret = getpid();
    printf("My name is Daniela Becerra Gonzalez \n");
    printf("pid is %d \n", ret);
    return 0;
}
 // send to victor.rodriguez.bahena@itesm.mx
