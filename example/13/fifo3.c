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
#define TEN_MEG (1024 * 1024 * 10)

int main(int argc, char *argv[])
{
    int pipe_fd;
    int res;
    int open_mode = O_WRONLY;
    int bytes_send = 0;
    char buffer[BUFFER_SIZE + 1];

    if(argc < 2) {
        fprintf(stderr, "Usage: %s < some combination of \
            O_RDONLY O_WRONLY O_NONBLOCK>\n", *argv);
        exit(EXIT_FAILURE);
    }
    
    // 是否允许访问.
    if(access(FIFO_NAME, F_OK) == -1) {
        res = mkfifo(FIFO_NAME, 0777);
        if(res != 0) {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }

    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    // 打开命令管道．
    pipe_fd = open(FIFO_NAME, open_mode);
    printf("Process %d result %d\n", getpid(), res);
    if(pipe_fd != -1){
        while(bytes_send < TEN_MEG) {
            res = write(pipe_fd, buffer, BUFFER_SIZE);
            if(res == -1) {
                fprintf(stderr, "Write error on pipe\n");
                exit(EXIT_FAILURE);
            }
            bytes_send +=res;
        }
        (void)close(pipe_fd);
    }else{
        exit(EXIT_FAILURE);
    }

    printf("Process %d finished\n", getpid());
    exit(EXIT_FAILURE);
}