#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handlerSingal(int sig)
{
    printf("Receive the signal %d\n", sig);
}

int main()
{
    signal(SIGUSR1, handlerSingal);
    

    while(1)
    {
        printf("Hello World\n");
        sleep(1);
    }
    
    return 0;
}
