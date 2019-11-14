#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Running ps with execlp\n");
    execlp("ps", "ps", "ax", 0);
    printf("Done.\n");

    return 0;
}