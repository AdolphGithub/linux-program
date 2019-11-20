#define _REENTRANT

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);

int main(void)
{
    int res;

    pthread_t a_thread;
    void *thread_result;

    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    if(res != 0) {
        perror("Thread create failed");
        return 1;
    }

    sleep(3);
    printf("Canceling thread...\n");
    res = pthread_cancel(a_thread);
    if(res != 0) {
        perror("Thread cancelation failed");
        return 1;
    } 

    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        return 1;
    }

    printf("Thread joined, it returned %s\n", (char *)thread_result);
    return 0;
}

void *thread_function(void *arg)
{
    int i,res;
    res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if(res != 0) {
        printf("Thread pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }


    res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if(res != 0) {
        printf("Thread pthread_setcanceltype failed");
        exit(EXIT_FAILURE);
    }


    printf("thread_function is runnning.");

    for(i = 0; i < 10; i++) {
        printf("Thread is still running (%d)...\n",i);
        sleep(1);
    }

    pthread_exit("Thank you for the cpu time");
}