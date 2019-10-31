#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    int file_desc;
    int save_errno;
    // 要创建,读写,执行权限.
    file_desc = open("/tmp/LCK.test", O_RDWR | O_CREAT | O_EXCL, 0444);
    if(file_desc == -1) {
        save_errno = errno;
        printf("Open failed with error %d\n", save_errno);
    }else{
        printf("Open succeeded");
    }

    return 0;
}