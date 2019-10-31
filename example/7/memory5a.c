#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    char *some_memory = (char *)0;
    printf("A read form null %s\n", some_memory);
    sprintf(some_memory, "A write to null\n");
    return 0;
}