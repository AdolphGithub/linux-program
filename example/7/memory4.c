#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define ONE_K (1024)

int main(void)
{
    char *some_memory;
    char *scan_ptr;

    some_memory = (char *)malloc(ONE_K);
    if(some_memory == NULL) exit(EXIT_FAILURE);

    scan_ptr = some_memory;
    // 无限制写入.长度才1024
    while(1) {
        *scan_ptr = '\0';
        scan_ptr++;
    }

    return 0;
}