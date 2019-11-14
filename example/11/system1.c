#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    printf("Running ps with system\n");
    system("ps ax");
    printf("Done.\n");

    return 0;
}