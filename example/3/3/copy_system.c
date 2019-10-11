#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// 复制一个文件.
int main(void)
{
    char ch;
    int file = open("file.txt",O_RDONLY);
    int file2 = open("copy.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    while(read(file, &ch, 1) == 1) {
        write(file2, &ch, 1);
    }

    close(file);
    close(file2);

    return 0;
}