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
    if(fork_result == -1) {
        fprintf(stderr, "Fork failure");
        exit(EXIT_FAILURE);
    }

    if(fork_result == 0) {
        data_processed = read(file_pipes[0],buffer, BUFSIZ);
        printf("Read %d bytes: %s\n", data_processed, buffer);
        exit(EXIT_SUCCESS);
    }else{
        data_processed = write(file_pipes[1], some_data, strlen(some_data));
        printf("Wrote %d bytes\n", data_processed);
    }
    
    exit(EXIT_SUCCESS);
}