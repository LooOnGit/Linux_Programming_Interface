#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct data_name{
    char first[10];
    char last[10];
}data_name;

int break_flag = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* printNumber(void* arg)
{
    int *num = (int *)arg;
    pthread_t tid = pthread_self();
    printf("I'm number: %d, thread ID: %lu\n", *num, (unsigned long)tid);
    pthread_exit(NULL);
}

void* printName(void* arg)
{
    data_name *name = (data_name*)arg;
    pthread_t tid = pthread_self();
    printf("I'm %s %s, thread ID: %lu\n", name->first, name->last, (unsigned long)tid);
    sleep(10);
    pthread_exit(NULL);
}

void* syncThread1(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        printf("Check for flag ready...\n");
        while(break_flag == 0)
        {
            printf("Thread 1 is waiting for the break flag to be set.\n");
            pthread_cond_wait(&cond, &lock);
        }
        printf("Break flag is ready, Thread 1 is breaking the loop.\n");
        pthread_mutex_unlock(&lock);
        sleep(1);   
    }

    printf("Thread 1 is exiting.\n");
    pthread_exit(NULL);
}

void* syncThread2(void* arg)
{
    pthread_mutex_lock(&lock);
    break_flag = 1;
    sleep(3);
    pthread_cond_signal(&cond);
    printf("Thread 2 is setting the break flag to 1 and signaling the condition variable.\n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_name;
    pthread_t thread_num;
    pthread_t thread_sync1;
    pthread_t thread_sync2;

    int number = 10;
    data_name name;

    strcpy(name.first, "Loo");
    strcpy(name.last, "Dev");

    //Create threads to print number
    pthread_create(&thread_name, NULL, printName, &name);
    pthread_create(&thread_num, NULL, printNumber, &number);
    pthread_create(&thread_sync1, NULL, syncThread2, NULL);
    pthread_create(&thread_sync2, NULL, syncThread1, NULL);

    //waiting for threads to finish
    pthread_join(thread_name, NULL);
    pthread_join(thread_num, NULL);
    pthread_join(thread_sync1, NULL);
    pthread_join(thread_sync2, NULL);


    // pthread_detach(thread_name);
    // pthread_detach(thread_num);

    return 0;
}
