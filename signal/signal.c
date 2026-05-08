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
    void (*oldHandler)(int);
    oldHandler = signal(SIGINT, handlerSingal);
    

    while(1)
    {
        printf("Hello World\n");
        for(int i = 0; ; i++)
        {
            sleep(1);
            if(i == 10)
            {
                signal(SIGINT, SIG_DFL);
                printf("restore the signal handler\n");
            }
        }
    }
    
    return 0;
}
