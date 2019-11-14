#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired =  0;

void ding(int sig) {
    alarm_fired = 1;
}

int main(void)
{
    pid_t pid;

    printf("alarm application starting\n");
    // 将本进程复制一份给新进程重新运行.
    pid = fork();
    switch (pid)
    {
        case -1:
            perror("fork failed");
            return -1;
            break;
        case 0:
            sleep(5);
            // 发送信号给父进程.
            kill(getppid(), SIGALRM);
            return 0;
            break;
    }
    printf("Waiting for alarm to go off\n");
    (void)signal(SIGALRM, ding);

    pause();    // 挂起等到程序被信号唤起.
    if(alarm_fired) {
        printf("Ding!\n");
    }

    printf("done\n");
    return 0;
}