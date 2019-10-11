#include <unistd.h>
#include <stdio.h>

int main(void)
{
    // 这里的1,2分别代表的是标准输出和标准错误.
    if(write(1, "Here is some data\n",18) != 18) {
        write(2, "A write error has occurred no file descriptor 1\n", 64);
    }
    
    return 0;
}