#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));
    // 自动关联副本. file_pipes为子进程的信息.
    if(pipe(file_pipes) != 0) {
        exit(EXIT_FAILURE);
    }
    // 子进程中.fork返回0.父进程中返回pid值.
    fork_result = fork();
    if(fork_result == (pid_t)-1) {
        fprintf(stderr, "Fork failure");
        exit(EXIT_FAILURE);
    }

    if(fork_result == (pid_t)0) {
        // 将子进程的标准输入给关闭了．
        close(0);
        // 将子进程的file_pipes[0]读文件描述符重定向至标准输入上.
        dup(file_pipes[0]);
        // 关闭子进程的file_pipes[0]读文件描述符
        close(file_pipes[0]);
        // 因为子进程不能操作写文件操作符,所以一起关闭.
        close(file_pipes[1]);
        execlp("od", "od", "-c", (char *)0);
        exit(EXIT_SUCCESS);
    }else{
        // 父进程不会从子进程的读文件操作符读取数据.所以关闭读文件操作符
        close(file_pipes[0]);
        // 开始向子进程的写文件操作符写入数据.
        data_processed = write(file_pipes[1], some_data, strlen(some_data));
        close(file_pipes[1]);
        printf("%d - wrote %d bytes\n", getpid(), data_processed);
    }
    
    exit(EXIT_SUCCESS);
}