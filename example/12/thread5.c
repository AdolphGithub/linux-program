#define _REENTRANT

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] = "Hello world";
int thread_finished = 0;

int main(void)
{
    int res;

    pthread_t a_thread;
    pthread_attr_t thread_attr;

    res = pthread_attr_init(&thread_attr);
    if(res != 0) {
        perror("Attribute creation failed");
        return 1;
    }

    res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if(res != 0) {
        perror("Setting detached attribute failed");
        return 1;
    }

    res = pthread_create(&a_thread, &thread_attr, thread_function, (void *)message);
    if(res != 0) {
        perror("Thread create failed");
        return 1;
    }

    (void)pthread_attr_destroy(&thread_attr);
    while(!thread_finished) {
        printf("Waiting for thread to say it's finished...\n");
        sleep(1);
    }

    printf("Other thread finished, bye!\n");

    return 0;
}

void *thread_function(void *arg)
{
    printf("thread_function is running. Argument was %s\n", (char *)arg);
    sleep(4);
    printf("Second thread setting finished flag, and exiting now\n");
    thread_finished = 1;    
    pthread_exit(NULL);
}