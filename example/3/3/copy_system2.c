#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// 复制一个文件.
int main(void)
{
    char buffer[1024];
    int nread;

    int file = open("file.txt",O_RDONLY);
    int file2 = open("copy.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    while( (nread =  read(file, buffer, 1024)) > 0) {
        write(file2, buffer, nread);
    }

    close(file);
    close(file2);

    return 0;
}