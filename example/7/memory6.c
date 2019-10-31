#include <stdlib.h>
#include <stdio.h>

#define ONE_K (1024)

int main(void)
{
    char *some_memory;
    int exit_code = EXIT_FAILURE;

    some_memory = (char *)malloc(ONE_K);

    if(some_memory != NULL) {
        free(some_memory);
        printf("Memory allocated and freed again\n");
        exit_code = EXIT_FAILURE;
    }

    return 0;
}