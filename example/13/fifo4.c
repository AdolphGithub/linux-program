#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int pipe_fd;
    int res;
    int open_mode = O_RDONLY;
    int bytes_read = 0;
    char buffer[BUFFER_SIZE + 1];

    memset(buffer, '\0', sizeof(buffer));    
    printf("Process %d opening FIFO O_RDONLY\n", getpid());
    // 打开命令管道．
    pipe_fd = open(FIFO_NAME, open_mode);
    printf("Process %d result %d\n", getpid(), res);

    if(pipe_fd != -1){
        do{
            res = read(pipe_fd, buffer, BUFFER_SIZE);
            bytes_read += res;
        }while(res > 0);
        (void)close(pipe_fd);
    }else{
        exit(EXIT_FAILURE);
    }
    
    printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);
    exit(EXIT_FAILURE);
}