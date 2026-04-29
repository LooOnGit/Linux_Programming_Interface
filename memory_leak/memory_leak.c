#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void memoryleak()
{
    int *ptr = (int*) malloc(100*sizeof(int));

    ptr[0] = 42;
}

int main()
{
    for(int i = 0; i < 10000; i++)
    {
        memoryleak();
        usleep(1);
    }
    return 0;
}