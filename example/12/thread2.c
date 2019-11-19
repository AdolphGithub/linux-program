#define _REENTRANT

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] = "Hello world";
int run_now;


int main(void)
{
    int res;

    int print_count1 = 0;
    run_now = 1;
    pthread_t a_thread;
    void *thread_result;

    res = pthread_create(&a_thread, NULL, thread_function, (void *)message);
    if(res != 0) {
        perror("Thread create failed");
        return 1;
    }

    while(print_count1++ < 20) {
        if(run_now == 1) {
            printf("1");
            run_now = 2;
        }else{
            sleep(1);
        }
    }

    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        return 1;
    }

    printf("Thread joined, it returned %s\n", (char *)thread_result);
    printf("Message is now %s\n", message);

    return 0;
}

void *thread_function(void *arg)
{
    int print_count2 = 0;
    run_now = 2;
    while(print_count2++ < 20) {
        if(run_now == 2) {
            printf("2");
            run_now = 1;
        }else{
            sleep(1);
        }
    }
    printf("thread_function is running. Argument was %s\n", (char *)arg);
    sleep(3);
    strcpy(message, "Bye!");
    pthread_exit("Thank you for the cpu time");
}