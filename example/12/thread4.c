#define _REENTRANT
#define WORK_SIZE 1024

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);

// 互斥量
pthread_mutex_t work_mutex;
// 工作区
char work_area[WORK_SIZE];
// 变量
int time_to_exit = 0;

int main(void)
{
    int res;

    pthread_t a_thread;
    void *thread_result;
    // 初始化互斥量.
    res = pthread_mutex_init(&work_mutex, NULL);
    if(res != 0) {
        perror("Mutex initialization failed");
        return 1;
    }
    // 创建一个新线程.
    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    if(res != 0) {
        perror("Thread create failed");
        return 1;
    }

    pthread_mutex_lock(&work_mutex);
    printf("Input some text. Enter 'end' to finish\n");
    while(!time_to_exit) {
        fgets(work_area, WORK_SIZE, stdin);
        pthread_mutex_unlock(&work_mutex);
        while(1) {
            pthread_mutex_lock(&work_mutex);
            if(work_area[0] != '\0') {
                pthread_mutex_unlock(&work_mutex);
                sleep(1);
            }else{
                break;
            }
        }
    }
    pthread_mutex_unlock(&work_mutex);
    printf("\nWaiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        return 1;
    }

    printf("Thread joined\n");
    pthread_mutex_destroy(&work_mutex);
    return 0;
}

void *thread_function(void *arg)
{
    sleep(1);
    pthread_mutex_lock(&work_mutex);
    while(strncmp("end", work_area, 3) != 0) {
        printf("You input %d characteers\n", (int)strlen(work_area) - 1);
        work_area[0] = '\0';
        pthread_mutex_unlock(&work_mutex);
        sleep(1);
        pthread_mutex_lock(&work_mutex);
        while(work_area[0] == '\0') {
            pthread_mutex_unlock(&work_mutex);
            sleep(1);
            pthread_mutex_lock(&work_mutex);
        }
    }

    time_to_exit = 1;
    work_area[0] = '\0';
    pthread_mutex_unlock(&work_mutex);
    pthread_exit("Thank you for the cpu time");
}